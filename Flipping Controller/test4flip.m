zero_length = 500;  % 设定时间长度
x_target = 0.1;  % 目标x值
y_target = 0;  % 目标y值
z_target = 0;  % 目标z值
steps_to_target = 10;  % 达到目标位置的步数

% 生成时间向量
%time_vector = linspace(0, zero_length-1, zero_length);
time_vector = linspace(0, (zero_length-1) * 0.1, zero_length); 
% 初始化矩阵，包含x, y, z, yaw, eta五列
matrix = zeros(zero_length, 5);  % 对应x, y, z, yaw, eta

% 设置x，y和z值，在前n步内逐渐增加到目标值，然后保持不变
matrix(1:steps_to_target, 1) = linspace(0, x_target, steps_to_target)';  % x轴逐渐增加到目标值
matrix(steps_to_target+1:end, 1) = x_target;  % x轴保持不变

matrix(1:steps_to_target, 2) = linspace(0, y_target, steps_to_target)';  % y轴逐渐增加到目标值
matrix(steps_to_target+1:end, 2) = y_target;  % y轴保持不变

matrix(1:steps_to_target, 3) = linspace(0, z_target, steps_to_target)';  % z轴逐渐增加到目标值
matrix(steps_to_target+1:end, 3) = z_target;  % z轴保持不变

% yaw和eta为定值
matrix(:, 4) = 0;  % yaw始终为0
matrix(:, 5) = 1;  % eta始终为1/-1

% 组合时间向量和矩阵
ref_input_timed = [time_vector', matrix];

% 生成timeseries对象
ref_input_struct = timeseries(ref_input_timed(:, 2:end), ref_input_timed(:, 1));

% 显示生成的timeseries对象
disp(ref_input_struct);