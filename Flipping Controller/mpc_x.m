% 定义状态空间矩阵
A = [0 1; 0 0];
B = [0; 1];
C = [1 0];
D = [0]; 
% 创建状态空间模型
sys = ss(A, B, C, D);
Ts = 0.1; % 控制时间步长，单位：秒 应与实际系统一致
% 创建MPC控制器
mpcobj_x = mpc(sys, Ts);
mpcobj_x.PredictionHorizon = 10;
mpcobj_x.ControlHorizon = 2;
% 定义输入的上下限
umin = -2; % 输入的最小值
umax = 2; % 输入的最大值
% 设置输入的约束
mpcobj_x.MV = struct('Min', umin, 'Max', umax);
mpcobj_x.MV.RateMin = -1;
mpcobj_x.MV.RateMax = 1;
mpcobj_x.Weights.OutputVariables = [5]; % 输出变量权重
mpcobj_x.Weights.ManipulatedVariables = [0.1]; % 控制输入权重
mpcobj_x.Weights.ManipulatedVariablesRate = [0.1]; % 控制输入变化率权重