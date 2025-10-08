%% Monte Carlo Test Table Generator
N = 100;  % 实验总数
rng(42);  % 固定随机种子，便于复现

% 参数范围
f_min = 5;  f_max = 10;                  % 外力范围
tau_min = -0.25;  tau_max = 0.25;          % 力矩范围

% 生成随机样本
disturbance_f     = f_min   + (f_max - f_min)   * rand(N, 1);
disturbance_tau_x = tau_min + (tau_max - tau_min) * rand(N, 1);
disturbance_tau_y = tau_min + (tau_max - tau_min) * rand(N, 1);
disturbance_tau_z = tau_min + (tau_max - tau_min) * rand(N, 1);

% 组合为表格
T = table((1:N)', disturbance_f, disturbance_tau_x, ...
          disturbance_tau_y, disturbance_tau_z, ...
          'VariableNames', {'ID','disturbance_f','tau_x','tau_y','tau_z'});

% 保存结果
writetable(T, 'MonteCarlo_Test_Table.xlsx');
writetable(T, 'MonteCarlo_Test_Table.csv');

% 显示部分结果
disp(T(1:10,:));
fprintf('✅ 已生成 100 组扰动参数并保存为 MonteCarlo_Test_Table.xlsx\n');
