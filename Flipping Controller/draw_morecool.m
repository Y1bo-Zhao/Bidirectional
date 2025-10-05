% ==============================================================
%  Drone Visualization   |   multi-snapshots • live-switch • colorbar
% ==============================================================

clear snap    % 若上次运行遗留 snap 变量，先清掉

%% === 0. 配置 ===========================================================
cfg.showLive   = true;                 % ★ 是否绘制实时机体
cfg.showTrail  = true;                 % 是否绘制尾迹
cfg.showGhost  = false;                % 是否绘制残影
cfg.ghostCountG = 5;                   % 残影缓冲帧数
%cfg.snapTimes   = [0, 0.2, 0.3,0.5,1.0,1.5];       % 关键帧时刻（秒）——想加删直接改这里
cfg.snapTimes   = [0, 0.2, 0.4, 2.0]; 
cfg.pauseEach   = 0.10;                % 预览每帧暂停；录视频设 0
% =======================================================================

%% === 1. 读入仿真数据  (假定 out 结构体已在工作区) ======================
x  = out.x.Data(:);   y = out.y.Data(:);   z = out.z.Data(:);
qw = out.qw.Data(:);  qx = out.qx.Data(:); qy = out.qy.Data(:); qz = out.qz.Data(:);
w4 = out.xuanzhuan.Data(:,1:4);            % n×4 旋翼角速度
pos  = [x y z];       quat = [qw qx qy qz];
dt   = 0.05;                              % 时间步长（若与仿真不符请改）
t    = (0:length(x)-1).' * dt;            % 时间轴

%% === 2. 颜色映射  (turbo) =============================================
w_min = min(w4,[],'all');   w_max = max(w4,[],'all');
cmap  = turbo(256);
speed2color = @(w) cmap( round( ...
                     rescale(w,1,256,'InputMin',w_min,'InputMax',w_max)), : );

%% === 3. 机体简易几何 / 旋翼圆盘 =======================================
body = [ 1 0 0 ; -1 0 0 ; 0 0 0 ; 0 1 0 ; 0 -1 0 ]; % 十字机架
rotor_r = 0.35;  rotor_z = 0.05;  nDisc = 40;
th  = linspace(0,2*pi,nDisc);
disc = [ rotor_r*cos(th') , rotor_r*sin(th') , repmat(rotor_z,size(th')) ];

%% === 4. 画布 & 颜色条 ==================================================
figure; clf; axis equal; grid on; hold on;
view([1 1 1]);
xlabel('X (m)'); ylabel('Y (m)'); zlabel('Z (m)');
xlim([min(x)-2, max(x)+2]); ylim([min(y)-2, max(y)+2]); zlim([min(z)-2, max(z)+2]);

colormap(cmap);        caxis([w_min w_max]);
cb = colorbar('eastoutside');
cb.Label.String = 'Rotor speed (rad/s)';

%% 4-A 尾迹句柄
if cfg.showTrail
    trail = animatedline('LineWidth',1.5,'MaximumNumPoints',500);
end

%% 4-B 实时机体句柄（受 showLive 控制）
if cfg.showLive
    hBody = plot3(NaN,NaN,NaN,'k','LineWidth',2);
    hDisc = gobjects(4,1);
    for k = 1:4
        hDisc(k) = patch(NaN,NaN,NaN,'EdgeColor','none','FaceAlpha',0.6);
    end
    hZ = plot3([0 0],[0 0],[0 1.5],'m','LineWidth',2);
end
hTxt = text(min(x),min(y),min(z),'','FontSize',12,'Color',[.3 .3 .3]);

%% 4-C 残影句柄
if cfg.showGhost
    G = cfg.ghostCountG;   alphaSet = linspace(0.15,0.45,G);
    hGhostBody = gobjects(G,1);  hGhostDisc = gobjects(G,4);
    for g = 1:G
        hGhostBody(g) = plot3(NaN,NaN,NaN,'Color',[.35 .35 .35],'LineWidth',1.2);
        for j = 1:4
            hGhostDisc(g,j) = patch(NaN,NaN,NaN,'EdgeColor','none','FaceAlpha',alphaSet(g));
        end
    end
end

%% 4-D 多关键帧句柄（自动个数）
nSnap = numel(cfg.snapTimes);
template = struct('t',0,'done',false,'body',gobjects(1), ...
                  'disc',gobjects(4,1),'z',gobjects(1));
snap = repmat(template,1,nSnap);

for s = 1:nSnap
    % 颜色方案：首帧红、次帧蓝，其余沿色条渐变
    if s==1,  clr=[1 0 0];
    elseif s==2, clr=[0 0.3 1];
    else, clr = cmap(round(1+(s-3)/max(nSnap-3,1)*255),:);
    end
    snap(s).t    = cfg.snapTimes(s);
    snap(s).body = plot3(NaN,NaN,NaN,'Color',clr,'LineWidth',2);
    for j = 1:4
        snap(s).disc(j) = patch(NaN,NaN,NaN,'EdgeColor','none','FaceAlpha',0.6);
    end
    snap(s).z    = plot3(NaN,NaN,NaN,'Color','m','LineWidth',1.5);
end

%% === 5. 主循环 ========================================================
N = numel(x);
for i = 1:N
    p = pos(i,:);  R = quat2rotm(quat(i,:));

    % -------- 实时机体（可关）
    if cfg.showLive
        bRot = (R*body.').';                         % 机架
        set(hBody,'XData',bRot(:,1)+p(1),'YData',bRot(:,2)+p(2),'ZData',bRot(:,3)+p(3));
        armTip = bRot([1 2 4 5],:)+p;                % 臂端
        for k = 1:4
            discRot = (R*disc.').'; discXYZ = discRot + armTip(k,:);
            set(hDisc(k),'XData',discXYZ(:,1),'YData',discXYZ(:,2),'ZData',discXYZ(:,3), ...
                         'FaceColor',speed2color(w4(i,k)));
        end
        zRot = (R*[0 0 0;0 0 0.5].').' + p;
        set(hZ,'XData',zRot(:,1),'YData',zRot(:,2),'ZData',zRot(:,3));
    else
        bRot = (R*body.').';         % 仍需几何数据供快照，下方会用到
        armTip = bRot([1 2 4 5],:)+p;
        zRot = (R*[0 0 0;0 0 0.5].').' + p;
    end

    % -------- 尾迹
    if cfg.showTrail
        addpoints(trail,p(1),p(2),p(3));
        trail.Color = speed2color(mean(w4(i,:)));
    end

    % -------- 时间标签
    set(hTxt,'Position',p+[.3 .3 .3],'String',sprintf('t = %.1f s',t(i)));

    % -------- 残影
    if cfg.showGhost
        slot = mod(i-1,cfg.ghostCountG)+1;
        set(hGhostBody(slot),'XData',bRot(:,1)+p(1),'YData',bRot(:,2)+p(2),'ZData',bRot(:,3)+p(3));
        for k = 1:4
            discRot = (R*disc.').'; discXYZ = discRot + armTip(k,:);
            set(hGhostDisc(slot,k),'XData',discXYZ(:,1),'YData',discXYZ(:,2),'ZData',discXYZ(:,3), ...
                                    'FaceColor',speed2color(w4(i,k)));
        end
    end

    % -------- 多关键帧捕获
    for s = 1:nSnap
        if ~snap(s).done && abs(t(i)-snap(s).t) <= dt/2
            snap(s).done = true;
            % 机体
            set(snap(s).body,'XData',bRot(:,1)+p(1),'YData',bRot(:,2)+p(2),'ZData',bRot(:,3)+p(3));
            set(snap(s).z,'XData',zRot(:,1),'YData',zRot(:,2),'ZData',zRot(:,3));
            % 四旋翼
            for k = 1:4
                discRot = (R*disc.').'; discXYZ = discRot + armTip(k,:);
                set(snap(s).disc(k),'XData',discXYZ(:,1),'YData',discXYZ(:,2),'ZData',discXYZ(:,3), ...
                                    'FaceColor',speed2color(w4(i,k)));
            end
        end
    end

    drawnow limitrate;
    pause(cfg.pauseEach);
end

% === 6. 并排绘制所有关键帧 + colorbar（紧凑 + 大字体 + 手动位置微调） ============
if nSnap > 0
    fig = figure('Name', 'All Snapshots Side-by-Side', 'NumberTitle', 'off');
    % 不用 tiledlayout，手动设置子图位置
    set(fig, 'Units', 'normalized');

    % 参数设定
    axFontSize = 13;
    tickFontSize = 11;
    gap = 0.01;                    % 子图之间的间隔
    ax_w = 0.18;                   % 每个子图宽度
    ax_h = 0.7;                    % 高度
    ax_bottom = 0.18;             % y方向起点
    ax_left0 = 0.05;              % 第一个子图左起点

    for s = 1:nSnap
        idx = find(abs(t - cfg.snapTimes(s)) <= dt/2, 1);
        if isempty(idx)
            warning("时间 %.3f s 没有找到对应帧。", cfg.snapTimes(s));
            continue;
        end

        % ===== 手动设置子图位置 =====
        ax_left = ax_left0 + (s-1)*(ax_w + gap);
        ax = axes('Position', [ax_left, ax_bottom, ax_w, ax_h]);

        hold on; axis equal; grid on;
        view([1 1 1]);

        % 设置紧凑的绘图区域
        xlim([min(x)-2, max(x)+2]); 
        ylim([min(y)-2, max(y)+2]); 
        zlim([min(z)-2, max(z)+2]);

        % 数据准备
        p = pos(idx,:);  
        R = quat2rotm(quat(idx,:));
        bRot = (R * body.').';                    
        armTip = bRot([1 2 4 5],:) + p;
        zRot = (R * [0 0 0; 0 0 -0.5].').' + p;

        % 绘制元素
        plot3(bRot(:,1)+p(1), bRot(:,2)+p(2), bRot(:,3)+p(3), ...
              'k', 'LineWidth', 2);
        for k = 1:4
            discRot = (R * disc.').'; 
            discXYZ = discRot + armTip(k,:);
            patch(discXYZ(:,1), discXYZ(:,2), discXYZ(:,3), ...
                  speed2color(w4(idx,k)), 'EdgeColor', 'none', 'FaceAlpha', 0.7);
        end
        plot3(zRot(:,1), zRot(:,2), zRot(:,3), 'm', 'LineWidth', 1.5);

        % 标签和刻度
        title(sprintf('t = %.2f s', cfg.snapTimes(s)), 'FontSize', axFontSize);
        xlabel('X (m)', 'FontSize', axFontSize);
        ylabel('Y (m)', 'FontSize', axFontSize);
        zlabel('Z (m)', 'FontSize', axFontSize);
        ax.FontSize = tickFontSize;
        xticks(linspace(-2, 2, 3));
        yticks(linspace(-2, 2, 3));
        zticks(linspace(-5, 2, 3));
    end

    % === 添加 colorbar（右上角） ===
    ax_cb = axes('Position', [0.83, 0.25, 0.015, 0.5]);  % 更靠右
    axis off;

    colormap(cmap);
    caxis([w_min w_max]);
    cb = colorbar(ax_cb, 'eastoutside');
    cb.Label.String = 'Rotor Speed (rad/s)';
    cb.Label.FontSize = axFontSize;
    cb.FontSize = tickFontSize;
    cb.Ticks = linspace(w_min, w_max, 3);
    cb.TickLabels = round(cb.Ticks, 0);
end

