% 遍历每一个时间点的四元数
for i = 1:size(quat, 1)
    % 获取当前时间点的四元数
    q = quat(i, :);
    
    % 将四元数转换为旋转矩阵
    R = quat2rotm(q);
    
    % 提取机体 z 轴在全局坐标系下的方向向量
    z_body = R(:, 3); % 第三列为 z 轴方向向量
    
    % 计算 z 轴与全局 z 轴 (0, 0, 1) 夹角的 cos 值
    global_z = [0; 0; 1];
    cos_theta(i) = dot(z_body, global_z) / (norm(z_body) * norm(global_z));
end

% 定义时间轴，假设每行之间时间间隔为 0.1s，共有 100 个时间点
t = 0:0.1:(size(quat, 1)-1) * 0.1;

expected_value = zeros(size(t));
expected_value(t >= 0) = 1; % 0s 以后是 1
expected_value(t < 0) = -1; % 0s 以前是 -1

% 扩展时间轴，从 -1s 开始
t_extended = [-1:0.1:0, t]; % 添加 -1s 到 0s 的时间

% 在 cos_theta 和 expected_value 前面补充 -1s 到 0s 的值
cos_theta_extended = [-1 * ones(1, length(-1:0.1:0)), cos_theta]; % cos(\theta) 延伸
expected_value_extended = [-1 * ones(1, length(-1:0.1:0)), expected_value]; % 期望值延伸

% 绘制图形
figure;
plot(t_extended, -cos_theta_extended, 'LineWidth', 2, 'DisplayName', 'Actual cos(\theta)');
hold on;
plot(t_extended, -expected_value_extended, 'r', 'LineWidth', 2, 'DisplayName', 'Expected Value');
hold off;

% 添加图例和标签
xlabel('Time(s)');
ylabel('cos(\theta)');
%title('Cosine of Angle between Body Z-Axis and Global Z-Axis (with Extended Data)');
legend;
grid on;

xlim([-1 9]);      % 设置时间范围 
ylim([-1.1 1.1]);   % 设置转速范围
xticks(-1:2:9);  % 每秒一个刻度 
yticks(-1:1:1);  % 每秒一个刻度 