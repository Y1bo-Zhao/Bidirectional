# -*- coding: utf-8 -*-
"""
train_cnn_lstm_attention_holdout.py

- 参考[11]结构：CNN(1D) → LSTM → 时间注意力 → 回归一个风险概率logit（sigmoid后∈[0,1]）
- 数据：dataset/ 下 sample_i.npy([T,D]) 与 label_i.npy(标量∈[0,1])
- 训练：BCEWithLogitsLoss（兼容软标签）
- 验证：val_MSE / val_MAE / Pearson Corr（若标签为0/1则额外打印 acc@0.5）
- 划分：分层留出（连续标签按分位数分桶）
- 可视化：单张 2x2 子图，一次性展示 Train Loss / Val MSE&MAE / Val Corr / 验证散点图
"""

import os
import glob
import numpy as np
import torch
import torch.nn as nn
import matplotlib.pyplot as plt
from torch.utils.data import Dataset, DataLoader, Subset

# ===================== 可调参数 =====================
DATA_DIR     = "dataset"
SAVE_DIR     = "models_cnn_lstm_attn"   # 与RNN版本分开存放
VAL_RATIO    = 0.2
BINS         = 5
SEED         = 42

# 模型规模（可与RNN+Attention脚本保持同量级便于公平对比）
HIDDEN_SIZE  = 64        # LSTM 隐状态
NUM_LAYERS   = 1
CNN_OUT      = 64        # CNN输出通道数
KERNEL_SIZE  = 3
LR           = 1e-3
EPOCHS       = 50
BATCH_SIZE   = 16
DROPOUT      = 0.0       # 需要时可>0
# ===================================================

# ----------------------------
# 数据集
# ----------------------------
class SequenceNPYDataset(Dataset):
    def __init__(self, root: str):
        self.root = root
        xs = sorted(glob.glob(os.path.join(root, "sample_*.npy")))
        ys = sorted(glob.glob(os.path.join(root, "label_*.npy")))
        def to_id(p): return os.path.splitext(os.path.basename(p))[0].split("_")[-1]
        x_map = {to_id(p): p for p in xs}
        y_map = {to_id(p): p for p in ys}
        common_ids = sorted(set(x_map.keys()).intersection(y_map.keys()), key=lambda s: int(s))
        self.pairs = [(x_map[i], y_map[i], i) for i in common_ids]
        if not self.pairs:
            raise RuntimeError(f"未找到成对 sample_*.npy / label_*.npy in {root}")

        x0 = np.load(self.pairs[0][0])
        if x0.ndim != 2:
            raise ValueError(f"{self.pairs[0][0]} 形状应为 [T,D]，实际 {x0.shape}")
        self.T, self.D = x0.shape

    def __len__(self):
        return len(self.pairs)

    def __getitem__(self, idx):
        x_path, y_path, _ = self.pairs[idx]
        X = np.load(x_path).astype(np.float32)            # [T, D]
        y = float(np.squeeze(np.load(y_path)))            # 标量
        return torch.from_numpy(X), torch.tensor([y], dtype=torch.float32)

# ----------------------------
# 注意力
# ----------------------------
class AttnHead(nn.Module):
    def __init__(self, hidden_size: int):
        super().__init__()
        self.w = nn.Linear(hidden_size, 1, bias=False)   # score_t = w^T h_t
    def forward(self, H):                                 # H: [B,T,H]
        scores = self.w(H).squeeze(-1)                   # [B,T]
        alpha = torch.softmax(scores, dim=1)             # [B,T]
        ctx = torch.bmm(alpha.unsqueeze(1), H).squeeze(1)# [B,H]
        return ctx, alpha

# ----------------------------
# CNN–LSTM–Attention 模型
# ----------------------------
class CNN_LSTM_Attn(nn.Module):
    """
    输入: x [B,T,D]
      - 先转为 Conv1d 期望的 [B,C_in,L]=[B,D,T]
      - CNN: Conv1d(D→CNN_OUT,k=3,pad=1) + ReLU (+ Dropout)
      - 转回 [B,T,CNN_OUT]
      - LSTM: 输入维度CNN_OUT → 隐状态HIDDEN_SIZE
      - Attention over time → context
      - FC → 1 logit
    """
    def __init__(self, input_dim: int, cnn_out=CNN_OUT, lstm_hidden=HIDDEN_SIZE, lstm_layers=NUM_LAYERS, dropout=DROPOUT):
        super().__init__()
        self.conv = nn.Conv1d(in_channels=input_dim, out_channels=cnn_out, kernel_size=KERNEL_SIZE, padding=KERNEL_SIZE//2)
        self.act  = nn.ReLU(inplace=True)
        self.drop = nn.Dropout(dropout) if dropout > 0 else nn.Identity()

        self.lstm = nn.LSTM(input_size=cnn_out, hidden_size=lstm_hidden,
                            num_layers=lstm_layers, batch_first=True, bidirectional=False, dropout=0.0)
        self.attn = AttnHead(lstm_hidden)
        self.fc   = nn.Linear(lstm_hidden, 1)

    def forward(self, x):          # x: [B,T,D]
        B, T, D = x.shape
        # CNN over time: 将特征作为通道，在时间维上做卷积
        x_cnn_in = x.permute(0, 2, 1)              # [B,D,T]
        feats = self.conv(x_cnn_in)                # [B,CNN_OUT,T]
        feats = self.act(feats)
        feats = self.drop(feats)
        feats = feats.permute(0, 2, 1)             # [B,T,CNN_OUT]

        # LSTM over time
        H, _ = self.lstm(feats)                    # [B,T,HIDDEN_SIZE]

        # Attention
        ctx, alpha = self.attn(H)                  # [B,HIDDEN_SIZE], [B,T]

        # Head
        logits = self.fc(ctx).squeeze(-1)          # [B]
        return logits, alpha

# ----------------------------
# 工具函数
# ----------------------------
def compute_norm_stats(ds: Dataset, idxs):
    Xs = [np.load(ds.pairs[i][0]) for i in idxs]  # list of [T,D]
    Xcat = np.concatenate(Xs, axis=0)             # [sumT, D]
    mean = Xcat.mean(0).astype(np.float32)
    std  = Xcat.std(0).astype(np.float32) + 1e-8
    return mean, std

def apply_norm(xb, mean, std):
    mean = torch.tensor(mean, device=xb.device)
    std  = torch.tensor(std,  device=xb.device)
    return (xb - mean) / std

def stratified_holdout(ds, val_ratio=0.2, bins=5, seed=42):
    rng = np.random.default_rng(seed)
    labels = np.array([float(np.squeeze(np.load(y))) for _, y, _ in ds.pairs], dtype=np.float32)

    uniq = np.unique(labels)
    if np.all((uniq == 0) | (uniq == 1)) and len(uniq) <= 2:
        groups = labels.astype(int)  # 0/1 两组
    else:
        qs = np.quantile(labels, np.linspace(0, 1, bins + 1))
        qs = np.unique(qs)
        edges = qs[1:-1] if len(qs) > 2 else []
        groups = np.digitize(labels, edges, right=True)

    train_idx, val_idx = [], []
    for g in np.unique(groups):
        idx = np.where(groups == g)[0]
        rng.shuffle(idx)
        n_val = max(1, int(len(idx) * val_ratio))
        val_idx.extend(idx[:n_val])
        train_idx.extend(idx[n_val:])
    rng.shuffle(train_idx); rng.shuffle(val_idx)
    return np.array(train_idx), np.array(val_idx)

@torch.no_grad()
def evaluate(model, val_loader, device, mean, std):
    model.eval()
    preds, gts = [], []
    for xb, yb in val_loader:
        xb = xb.to(device)                        # [B,T,D]
        yb = yb.to(device).view(-1)               # [B]
        xb = apply_norm(xb, mean, std)
        logits, _ = model(xb)                     # [B]
        prob = torch.sigmoid(logits)              # [B]
        preds.append(prob.cpu().numpy())
        gts.append(yb.cpu().numpy())
    preds = np.concatenate(preds) if preds else np.array([])
    gts   = np.concatenate(gts)   if gts   else np.array([])

    if len(preds) == 0:
        return {"mse":np.nan, "mae":np.nan, "corr":0.0, "acc":None, "preds":preds, "gts":gts}

    mse = float(np.mean((preds - gts)**2))
    mae = float(np.mean(np.abs(preds - gts)))
    if len(preds) > 1 and np.std(preds) > 1e-6 and np.std(gts) > 1e-6:
        corr = float(np.corrcoef(preds, gts)[0,1])
    else:
        corr = 0.0
    if np.all((gts==0)|(gts==1)):
        acc = float(((preds>=0.5)==gts).mean())
    else:
        acc = None
    return {"mse":mse, "mae":mae, "corr":corr, "acc":acc, "preds":preds, "gts":gts}

# ----------------------------
# 训练（留出法）
# ----------------------------
def train_holdout(ds,
                  train_idx, val_idx,
                  hidden_size=HIDDEN_SIZE, lr=LR, epochs=EPOCHS, batch_size=BATCH_SIZE,
                  device=None, save_dir=SAVE_DIR):
    os.makedirs(save_dir, exist_ok=True)
    device = device or torch.device("cuda" if torch.cuda.is_available() else "cpu")

    # 归一化参数用训练集统计
    mean, std = compute_norm_stats(ds, train_idx)

    # DataLoader
    train_loader = DataLoader(Subset(ds, train_idx), batch_size=batch_size, shuffle=True)
    val_loader   = DataLoader(Subset(ds, val_idx),   batch_size=batch_size, shuffle=False)

    # 模型/损失/优化
    model = CNN_LSTM_Attn(ds.D, CNN_OUT, hidden_size, NUM_LAYERS, DROPOUT).to(device)
    crit  = nn.BCEWithLogitsLoss()
    opt   = torch.optim.Adam(model.parameters(), lr=lr)

    # 记录训练过程
    history = {"train_loss": [], "val_mse": [], "val_mae": [], "val_corr": []}
    best_mse = float("inf")
    best_state = None
    final_metrics = None

    for ep in range(1, epochs+1):
        model.train()
        total_loss = 0.0
        for xb, yb in train_loader:
            xb = xb.to(device)                  # [B,T,D]
            yb = yb.to(device).view(-1)         # [B]
            xb = apply_norm(xb, mean, std)
            opt.zero_grad()
            logits, _ = model(xb)               # [B]
            loss = crit(logits, yb)
            loss.backward()
            opt.step()
            total_loss += loss.item() * xb.size(0)
        train_loss = total_loss / len(train_loader.dataset)

        # 验证
        metrics = evaluate(model, val_loader, device, mean, std)
        final_metrics = metrics  # 记录最后一轮指标与 preds/gts
        history["train_loss"].append(train_loss)
        history["val_mse"].append(metrics["mse"])
        history["val_mae"].append(metrics["mae"])
        history["val_corr"].append(metrics["corr"])

        log = (f"Epoch {ep:02d}/{epochs} | train_loss={train_loss:.4f} | "
               f"val_MSE={metrics['mse']:.4f} | val_MAE={metrics['mae']:.4f} | val_corr={metrics['corr']:.3f}")
        if metrics["acc"] is not None:
            log += f" | val_acc@0.5={metrics['acc']:.3f}"
        print(log)

        # 以最小 val_MSE 作为最佳
        if np.isfinite(metrics["mse"]) and metrics["mse"] < best_mse:
            best_mse = metrics["mse"]
            best_state = {
                "model_state": model.state_dict(),
                "mean": mean, "std": std,
                "T": ds.T, "D": ds.D,
                "config": dict(hidden_size=hidden_size, lr=lr, epochs=epochs, batch_size=batch_size,
                               cnn_out=CNN_OUT, kernel_size=KERNEL_SIZE, dropout=DROPOUT)
            }

    # 保存最佳
    if best_state is not None:
        torch.save(best_state, os.path.join(save_dir, "cnn_lstm_attention_best.pth"))

    return model, history, final_metrics  # 返回最后一轮 metrics（含 preds/gts）

# ----------------------------
# 主流程
# ----------------------------
def main():
    # 随机种子
    np.random.seed(SEED)
    torch.manual_seed(SEED)

    os.makedirs(SAVE_DIR, exist_ok=True)
    ds = SequenceNPYDataset(DATA_DIR)
    print(f"样本={len(ds)} | 形状 [T={ds.T}, D={ds.D}]")

    # 分层留出
    train_idx, val_idx = stratified_holdout(ds, val_ratio=VAL_RATIO, bins=BINS, seed=SEED)
    print(f"分层留出：训练 {len(train_idx)}，验证 {len(val_idx)}")

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model, history, final_metrics = train_holdout(
        ds, train_idx, val_idx,
        hidden_size=HIDDEN_SIZE, lr=LR, epochs=EPOCHS, batch_size=BATCH_SIZE,
        device=device, save_dir=SAVE_DIR
    )

    # ====== 打印留出法结果 ======
    print("\n====== 📊 留出法结果（CNN–LSTM–Attention） ======")
    line = f"val_MSE={final_metrics['mse']:.4f} | val_MAE={final_metrics['mae']:.4f} | val_corr={final_metrics['corr']:.3f}"
    if final_metrics["acc"] is not None:
        line += f" | val_acc@0.5={final_metrics['acc']:.3f}"
    print(line)
    print(f"最佳模型已保存至 {os.path.join(SAVE_DIR, 'cnn_lstm_attention_best.pth')}")

    # ====== 单张大图（2×2 子图）集中展示 ======
    epochs_arr = np.arange(1, EPOCHS+1)
    fig, axes = plt.subplots(2, 2, figsize=(12, 8))

    # (1) Train Loss
    ax = axes[0, 0]
    ax.plot(epochs_arr, history["train_loss"], label="Train Loss")
    ax.set_title("Training Loss (CNN-LSTM-Attn)"); ax.set_xlabel("Epoch"); ax.set_ylabel("Loss")
    ax.grid(True); ax.legend()

    # (2) Val MSE & MAE
    ax = axes[0, 1]
    ax.plot(epochs_arr, history["val_mse"], label="Val MSE")
    ax.plot(epochs_arr, history["val_mae"], label="Val MAE")
    ax.set_title("Validation Errors"); ax.set_xlabel("Epoch"); ax.set_ylabel("Error")
    ax.grid(True); ax.legend()

    # (3) Val Corr
    ax = axes[1, 0]
    ax.plot(epochs_arr, history["val_corr"], label="Val Corr")
    ax.set_title("Validation Correlation"); ax.set_xlabel("Epoch"); ax.set_ylabel("Correlation")
    ax.grid(True); ax.legend()

    # (4) 验证集散点图（Pred vs True）
    ax = axes[1, 1]
    if len(final_metrics["preds"]) > 0:
        ax.scatter(final_metrics["gts"], final_metrics["preds"], alpha=0.7)
        ax.set_title(f"Pred vs True (Corr={final_metrics['corr']:.3f})")
        ax.set_xlabel("True risk label"); ax.set_ylabel("Predicted risk")
        ax.grid(True)
    else:
        ax.axis('off')
        ax.text(0.5, 0.5, "No validation predictions", ha='center', va='center')

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
