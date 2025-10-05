%% === Drone pose playback + tilt-to-risk (0~1) ===
% 需要的数据结构：
% out.x.Data, out.y.Data, out.z.Data       (nx1)
% out.phi.Data, out.theta.Data, out.psi.Data  (nx1)  [单位: 弧度]
% 可选：out.tout (nx1) 时间戳，若没有则默认 dt=0.1s

%% 读取数据
x = out.x.Data(:);
y = out.y.Data(:);
z = out.z.Data(:);
pos = [x, y, z];

phi   = out.phi.Data(:);    % roll
theta = out.theta.Data(:);  % pitch
psi   = out.psi.Data(:);    % yaw

n = min([size(pos,1), numel(phi), numel(theta), numel(psi)]);
pos   = pos(1:n, :);
phi   = phi(1:n);
theta = theta(1:n);
psi   = psi(1:n);

% 时间轴
if isfield(out, 'tout') && numel(out.tout) >= n
    t = out.tout(1:n);
else
    dt = 0.1;                      % 若没有tout，就假定0.1s
    t  = (0:n-1).' * dt;
end

%% 欧拉角 -> 四元数 (ZYX: yaw, pitch, roll). 注意：MATLAB eul2quat输入是[Z Y X]且单位为弧度
% 若你的phi/theta/psi是度，请先用 deg2rad 转换！
quat = eul2quat([psi, theta, phi], 'ZYX');   % n×4

%% 参数：风险映射
a_safe_deg   = 10;    % 低于此角认为安全
a_crit_deg   = 70;    % 高于此角认为极危险
use_logistic = false; % true使用Logistic，false使用线性分段
k_logistic   = 0.15;  % Logistic斜率
alpha0_deg   = 45;    % Logistic中心角

alpha_smooth = 0.2;   % 概率一阶平滑系数 (0~1, 越大越灵敏)
p_prev = 0;

%% 无人机几何模型（简十字 + 机体z轴）
droneBody   = [ 1 0 0; -1 0 0; 0 0 0; 0 1 0; 0 -1 0]; % 十字
rotorLength = 2;
zAxisLength = 2.0;

%% 初始化图形
figure('Color','w');
axis equal; hold on; grid on; view(3);
xlabel('X'); ylabel('Y'); zlabel('Z');

% 视域范围
pad = 10;
xlim([min(x)-pad, max(x)+pad]);
ylim([min(y)-pad, max(y)+pad]);
zlim([min(z)-pad, max(z)+pad]);

rotate3d on;

% 画初始几何
hDrone  = plot3(droneBody(:,1), droneBody(:,2), droneBody(:,3), 'b', 'LineWidth', 4);
hRotor1 = plot3([-rotorLength, rotorLength], [0, 0], [0, 0], 'g', 'LineWidth', 4);
hRotor2 = plot3([0, 0], [-rotorLength, rotorLength], [0, 0], 'r', 'LineWidth', 4);
hZAxis  = plot3([0, 0], [0, 0], [0, zAxisLength], 'm', 'LineWidth', 4);

% 时间文本
hTimeText = text(min(x)-pad*0.8, min(y)-pad*0.8, min(z)-pad*0.8, ...
    'Time: 0.00 s', 'FontSize', 12, 'Color', 'k');

% 风险文本 + 进度条
hProbText = text(min(x)-pad*0.8, min(y)-pad*0.8, min(z)+pad*0.8, ...
    'Risk: 0.00 (tilt=0.0°)', 'FontSize', 12, 'Color', 'k');

barOrigin = [min(x)-pad*0.8, min(y)-pad*0.8, min(z)+pad*0.6];
barLen    = 6; 
hBarBg = plot3([0 barLen] + barOrigin(1), [0 0]+barOrigin(2), [0 0]+barOrigin(3), ...
    'k-', 'LineWidth', 6);
hBarFg = plot3([0 0]       + barOrigin(1), [0 0]+barOrigin(2), [0 0]+barOrigin(3), ...
    'r-', 'LineWidth', 6);

%% 播放动画
for i = 1:n
    % 当前位置 & 姿态
    position   = pos(i, :);
    quaternion = quat(i, :);           % [w x y z]
    R = quat2rotm(quaternion);         % 3x3

    % 旋转刚体几何
    rotatedBody   = (R * droneBody')';
    rotatedRotor1 = (R * [ -rotorLength,  rotorLength; 0, 0; 0, 0 ])';
    rotatedRotor2 = (R * [ 0, 0; -rotorLength, rotorLength; 0, 0 ])';
    rotatedZAxis  = (R * [ 0, 0; 0, 0; 0, zAxisLength ])';

    % 更新图元位置
    set(hDrone,  'XData', rotatedBody(:,1)   + position(1), ...
                 'YData', rotatedBody(:,2)   + position(2), ...
                 'ZData', rotatedBody(:,3)   + position(3));
    set(hRotor1, 'XData', rotatedRotor1(:,1) + position(1), ...
                 'YData', rotatedRotor1(:,2) + position(2), ...
                 'ZData', rotatedRotor1(:,3) + position(3));
    set(hRotor2, 'XData', rotatedRotor2(:,1) + position(1), ...
                 'YData', rotatedRotor2(:,2) + position(2), ...
                 'ZData', rotatedRotor2(:,3) + position(3));
    set(hZAxis,  'XData', rotatedZAxis(:,1)  + position(1), ...
                 'YData', rotatedZAxis(:,2)  + position(2), ...
                 'ZData', rotatedZAxis(:,3)  + position(3));

    % 更新时间
    set(hTimeText, 'String', sprintf('Time: %.2f s', t(i)));

    % ===== 计算机体z轴与世界竖直夹角 α =====
    zb = R(:,3);                   % 机体z轴(世界系)
    zw = [0;0;1];
    dotv   = dot(zb, zw);
    crossv = cross(zb, zw);
    alpha  = atan2(norm(crossv), dotv);   % 弧度
    alpha_deg = rad2deg(alpha);

    % ===== 角度 -> 风险概率p =====
    if use_logistic
        p = 1 ./ (1 + exp(-k_logistic * (alpha_deg - alpha0_deg)));
    else
        p = (alpha_deg - a_safe_deg) / (a_crit_deg - a_safe_deg);
        p = max(0, min(1, p));
    end

    % 平滑
    p = alpha_smooth * p + (1 - alpha_smooth) * p_prev;
    p_prev = p;

    % ===== UI: 文本 + 进度条 + 颜色提示 =====
    set(hProbText, 'String', sprintf('Risk: %.2f', p));

    len_now = barLen * p;
    set(hBarFg, 'XData', [0 len_now] + barOrigin(1), ...
                'YData', [0 0]       + barOrigin(2), ...
                'ZData', [0 0]       + barOrigin(3));

    % 颜色：绿->黄->红的简单过渡
    col = [p, max(0,1-abs(p-0.5)*2), 1-p];
    set(hBarFg, 'Color', col);

    % 高风险时给机体z轴变色
    if p > 0.7
        set(hZAxis, 'Color', [1 0 0]);      % 红
    elseif p > 0.4
        set(hZAxis, 'Color', [1 0.5 0]);    % 橙
    else
        set(hZAxis, 'Color', [0.6 0 0.6]);  % 品红(默认)
    end

    drawnow;
    % 若你希望更慢些，可取消下一行注释：
    pause(0.1); 
end

%% 可选：保存视频（解除注释使用）
% v = VideoWriter('drone_motion.avi');
% v.FrameRate = 1 / median(diff(t));
% open(v);
% set(gcf,'Position',[100 100 1280 720]);
% for i = 1:n
%     frame = getframe(gcf);
%     writeVideo(v, frame);
% end
% close(v);
