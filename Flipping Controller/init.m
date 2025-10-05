ModelParam_c_T = 1.105e-05;    % 螺旋桨拉力系数
ModelParam_c_M = 1.779e-07*2;  % 螺旋桨力矩系数
ModelParam_d = 0.225;          % 机体中心和任一电机的距离(m)
ModelParam_m = 1.4;            % 四旋翼飞行器质量(kg)
ModelParam_g = 9.8;            % 重力加速度(m/s^2)
ModelParam_I_xx = 0.0211;      % 四旋翼x轴转动惯量(kg·m^2)
ModelParam_I_yy = 0.0219;      % 四旋翼y轴转动惯量(kg·m^2)
ModelParam_I_zz = 0.0366;      % 四旋翼z轴转动惯量(kg·m^2)
ModelParam_J_RP = 0.0001287;   % 整个电机转子和螺旋桨绕转轴的总转动惯量(kg·m^2)
ModelInit_Pos_x = 0;
ModelInit_Pos_y = 0;
ModelInit_Pos_z = 0;        % 四旋翼初始高度
ModelInit_Att_x = 0;
ModelInit_Att_y = 0;
ModelInit_Att_z = 0;
C_T = 3.5611;    % 推力系数
C_M = 1.1366;  % 转矩系数
p = 1.225;        % 空气密度 (kg/m^3)，这是在海平面，温度为15摄氏度时的空气密度
D_p = 0.1;        % 螺旋桨直径 (m)，这是一个常见的螺旋桨直径
f_max = 10;
tau_max = 0.1;
ModelInit_Att_qw = 0;
ModelInit_Att_qx = 1;
ModelInit_Att_qy = 0;
ModelInit_Att_qz = 0;

%控制器输入需要
% 定义惯性矩阵 I
I = diag([ModelParam_I_xx, ModelParam_I_yy, ModelParam_I_zz]);

% 创建 model_param 结构体并将所有参数添加进去
model_param = struct(...
    'I', I, ...
    'mass', ModelParam_m, ...
    'g', ModelParam_g, ...
    'c_T', ModelParam_c_T, ...
    'c_M', ModelParam_c_M, ...
    'd', ModelParam_d, ...
    'J_RP', ModelParam_J_RP, ...
    'init_pos', [ModelInit_Pos_x, ModelInit_Pos_y, ModelInit_Pos_z], ...
    'init_att', [ModelInit_Att_x, ModelInit_Att_y, ModelInit_Att_z], ...
    'C_T', C_T, ...
    'C_M', C_M, ...
    'air_density', p, ...
    'prop_diameter', D_p, ...
    'f_max', f_max, ...
    'tau_max', tau_max ...
);

% 创建总线元素
elems(1) = Simulink.BusElement;
elems(1).Name = 'I';
elems(1).Dimensions = [3, 3];zero_length = 500;  % 设定时间长度
x_target = 1;  % 目标x值
y_target = 0;  % 目标y值
z_target = 0;  % 目标z值
steps_to_target = 100;  % 达到目标位置的步数

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
elems(1).DataType = 'double';

elems(2) = Simulink.BusElement;
elems(2).Name = 'mass';
elems(2).Dimensions = 1;
elems(2).DataType = 'double';

elems(3) = Simulink.BusElement;
elems(3).Name = 'g';
elems(3).Dimensions = 1;
elems(3).DataType = 'double';

elems(4) = Simulink.BusElement;
elems(4).Name = 'c_T';
elems(4).Dimensions = 1;
elems(4).DataType = 'double';

elems(5) = Simulink.BusElement;
elems(5).Name = 'c_M';
elems(5).Dimensions = 1;
elems(5).DataType = 'double';

elems(6) = Simulink.BusElement;
elems(6).Name = 'd';
elems(6).Dimensions = 1;
elems(6).DataType = 'double';

elems(7) = Simulink.BusElement;
elems(7).Name = 'J_RP';
elems(7).Dimensions = 1;
elems(7).DataType = 'double';

elems(8) = Simulink.BusElement;
elems(8).Name = 'init_pos';
elems(8).Dimensions = [1, 3];
elems(8).DataType = 'double';

elems(9) = Simulink.BusElement;
elems(9).Name = 'init_att';
elems(9).Dimensions = [1, 3];
elems(9).DataType = 'double';

elems(10) = Simulink.BusElement;
elems(10).Name = 'C_T';
elems(10).Dimensions = 1;
elems(10).DataType = 'double';

elems(11) = Simulink.BusElement;
elems(11).Name = 'C_M';
elems(11).Dimensions = 1;
elems(11).DataType = 'double';

elems(12) = Simulink.BusElement;
elems(12).Name = 'air_density';
elems(12).Dimensions = 1;
elems(12).DataType = 'double';

elems(13) = Simulink.BusElement;
elems(13).Name = 'prop_diameter';
elems(13).Dimensions = 1;
elems(13).DataType = 'double';

elems(14) = Simulink.BusElement;
elems(14).Name = 'f_max';
elems(14).Dimensions = 1;
elems(14).DataType = 'double';

elems(15) = Simulink.BusElement;
elems(15).Name = 'tau_max';
elems(15).Dimensions = 1;
elems(15).DataType = 'double';

% 创建总线对象
model_param_bus = Simulink.Bus;
model_param_bus.Elements = elems;

% 将总线对象保存到 MATLAB 工作区
assignin('base', 'model_param_bus', model_param_bus);


elems_2(1) = Simulink.BusElement;
elems_2(1).Name = 'pos';
elems_2(1).Dimensions = [3, 1];
elems_2(1).DataType = 'double';

elems_2(2) = Simulink.BusElement;
elems_2(2).Name = 'vel';
elems_2(2).Dimensions = [3, 1];
elems_2(2).DataType = 'double';

elems_2(3) = Simulink.BusElement;
elems_2(3).Name = 'acc';
elems_2(3).Dimensions = [3, 1];
elems_2(3).DataType = 'double';

elems_2(4) = Simulink.BusElement;
elems_2(4).Name = 'jerk';
elems_2(4).Dimensions = [3, 1];
elems_2(4).DataType = 'double';

elems_2(5) = Simulink.BusElement;
elems_2(5).Name = 'snap';
elems_2(5).Dimensions = [3, 1];
elems_2(5).DataType = 'double';

elems_2(6) = Simulink.BusElement;
elems_2(6).Name = 'yaw';
elems_2(6).Dimensions = 1;
elems_2(6).DataType = 'double';

elems_2(7) = Simulink.BusElement;
elems_2(7).Name = 'yawdot';
elems_2(7).Dimensions = 1;
elems_2(7).DataType = 'double';

elems_2(8) = Simulink.BusElement;
elems_2(8).Name = 'yawddot';
elems_2(8).Dimensions = 1;
elems_2(8).DataType = 'double';

desired_state_bus = Simulink.Bus;
desired_state_bus.Elements = elems_2;

% 将总线对象保存到 MATLAB 工作区
assignin('base', 'desired_state_bus', desired_state_bus);

% 定义总线元素
elems_3(1) = Simulink.BusElement;
elems_3(1).Name = 'Kp';
elems_3(1).Dimensions = [3, 3];
elems_3(1).DataType = 'double';

elems_3(2) = Simulink.BusElement;
elems_3(2).Name = 'Kv';
elems_3(2).Dimensions = [3, 3];
elems_3(2).DataType = 'double';

elems_3(3) = Simulink.BusElement;
elems_3(3).Name = 'KR';
elems_3(3).Dimensions = [3, 3];
elems_3(3).DataType = 'double';

elems_3(4) = Simulink.BusElement;
elems_3(4).Name = 'K_omega';
elems_3(4).Dimensions = [3, 3];
elems_3(4).DataType = 'double';

% 创建总线对象
K_bus = Simulink.Bus;
K_bus.Elements = elems_3;

% 将总线对象保存到 MATLAB 工作区
assignin('base', 'K_bus', K_bus);

run test4flip.m;



