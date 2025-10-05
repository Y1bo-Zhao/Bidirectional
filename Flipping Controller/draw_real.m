% 假设 x, y, z 分别为 nx1 的列向量
x = out.x.Data;
y = out.y.Data;
z = out.z.Data;
%z = -z; % 可能是为了调整方向
pos = [x, y, z];

qw = out.qw.Data;
qx = out.qx.Data;
qy = out.qy.Data;
qz = out.qz.Data;
quat = [qw, qx, qy, qz]; % 100 个时间点的四元数信息

% 初始化图形
figure;
axis equal;
hold on;
grid on;
view(3); % 3D 视图            
xlabel('X');
ylabel('Y');
zlabel('Z');

% 设置固定视角和范围
xlim([min(x)-10, max(x)+10]);
ylim([min(y)-10, max(y)+10]);
zlim([min(z)-10, max(z)+10]);

%启用3的旋转
rotate3d on;

% 定义无人机模型（简单的十字形表示无人机机身和旋翼）
droneBody = [1 0 0; -1 0 0; 0 0 0; 0 1 0; 0 -1 0]; % 十字形表示无人机
rotorLength = 0.5; % 旋翼长度
zAxisLength = 2; % 机体z轴长度

% 初始化无人机姿态
hDrone = plot3(droneBody(:,1), droneBody(:,2), droneBody(:,3), 'b', 'LineWidth', 2);
hRotor1 = plot3([-rotorLength, rotorLength], [0, 0], [0, 0], 'g', 'LineWidth', 2);
hRotor2 = plot3([0, 0], [-rotorLength, rotorLength], [0, 0], 'r', 'LineWidth', 2);
hZAxis = plot3([0, 0], [0, 0], [0, zAxisLength], 'm', 'LineWidth', 2); % 新增的z轴表示

% 添加时间文本
hTimeText = text(min(x)-8, min(y)-8, min(z)-8, 'Time: 0', 'FontSize', 12, 'Color', 'k');

% 生成动画
for i = 1:length(pos)
    % 获取当前时间点的位置和四元数
    position = pos(i, :);
    quaternion = quat(i, :);
    
    % 将四元数转换为旋转矩阵
    R = quat2rotm(quaternion);
    
    % 旋转无人机模型
    rotatedBody = (R * droneBody')';
    rotatedRotor1 = (R * [-rotorLength, rotorLength; 0, 0; 0, 0])';
    rotatedRotor2 = (R * [0, 0; -rotorLength, rotorLength; 0, 0])';
    rotatedZAxis = (R * [0, 0; 0, 0; 0, zAxisLength])';
    
    % 更新无人机的位置和姿态
zero_length = 500;  % 设定时间长度
z_target = 10;  % 目标z值
steps_to_target = 100;  % 达到目标z值的步数

% 生成时间向量
time_vector = linspace(0, (zero_length-1) * 0.1, zero_length); 
%time_vector = linspace(0, zero_length-1, zero_length);

% 初始化矩阵，包含z, yaw, eta三列
matrix = zeros(zero_length, 5);  % 假设矩阵有五列，对应x, y, z, yaw, eta

% 设置z值，在前十步内逐渐增加到目标值1，然后保持不变
matrix(1:steps_to_target, 3) = linspace(0, z_target, steps_to_target)';  % z轴逐渐增加
matrix(steps_to_target+1:end, 3) = z_target;  % z轴保持不变

% yaw和eta保持为0
matrix(:, 4) = 0;  % yaw始终为0
matrix(:, 5) = 1;  % eta始终为0

% 组合时间向量和矩阵
ref_input_timed = [time_vector', matrix];

% 生成timeseries对象
ref_input_struct = timeseries(ref_input_timed(:, 2:end), ref_input_timed(:, 1));

% 显示生成的timeseries对象
disp(ref_input_struct);
    set(hDrone, 'XData', rotatedBody(:,1) + position(1), 'YData', rotatedBody(:,2) + position(2), 'ZData', rotatedBody(:,3) + position(3));
    set(hRotor1, 'XData', rotatedRotor1(:,1) + position(1), 'YData', rotatedRotor1(:,2) + position(2), 'ZData', rotatedRotor1(:,3) + position(3));
    set(hRotor2, 'XData', rotatedRotor2(:,1) + position(1), 'YData', rotatedRotor2(:,2) + position(2), 'ZData', rotatedRotor2(:,3) + position(3));
    set(hZAxis, 'XData', rotatedZAxis(:,1) + position(1), 'YData', rotatedZAxis(:,2) + position(2), 'ZData', rotatedZAxis(:,3) + position(3));
    
    % 更新时间文本
    set(hTimeText, 'String', sprintf('Time: %.2f', i*0.1)); % 假设每一步对应 0.1 秒
    
    % 更新图形
    drawnow;
    
    % 暂停一段时间以减慢播放速度
    pause(0.1); % 暂停 0.1 秒，可以根据需要调整暂停时间
end

% 如果需要保存为视频，可以使用 VideoWriter 函数
% videoWriter = VideoWriter('drone_motion.avi');
% open(videoWriter);
% close(videoWriter);
