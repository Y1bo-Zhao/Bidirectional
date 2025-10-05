x = out.x.Data;      % [n,1]
y = out.y.Data;
z = out.z.Data;
phi = out.phi.Data;
theta = out.theta.Data;
psi = out.psi.Data;

% 组合在一起，每一行是一个时刻
sample = [x, y, z, phi, theta, psi];  % [n,6]矩阵
sample_extract = sample(41:60, :);
%writeNPY(sample_extract, 'sample_1.npy');
writeNPY(sample_extract, sprintf('dataset/sample_%d.npy', 7));
risk_label = 0; % 1表示危险，0表示安全
writeNPY(risk_label, sprintf('dataset/label_%d.npy', 7));