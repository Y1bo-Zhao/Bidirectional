% 定义圆的参数
r = 5; % 圆的半径
t = linspace(0, 2*pi, 100); % 参数t，从0到2*pi，生成100个点

% 圆的参数方程
x = zeros(size(t)); % x坐标为0，表示在y-z平面上
y = r*sin(t); % y坐标，使用sin(t)使得t=0时y=0
z = -r*cos(t) + r; % z坐标，使用-cos(t)+r使得t=0时z=0

% 绘制圆圈轨迹
plot3(x, y, z);
axis equal; % 保证各坐标轴的比例相同
grid on; % 显示网格
xlabel('X');
ylabel('Y');
zlabel('Z');
title('竖直圆圈轨迹，起点(0,0,0)');

[~, n] = size(x);
Psi = zeros(1,n);
matrix = [x',y',z',Psi'];
time_vector = linspace(0, n-1, n);
ref_input_timed = [time_vector', matrix];
ref_input_struct = timeseries(ref_input_timed(:, 2:end), ref_input_timed(:, 1));
