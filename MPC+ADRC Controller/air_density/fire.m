% 定义火坑的中心位置
center_x = 0;
center_y = 0;

% 定义火坑的半径
radius = 3;

% 定义火坑中心的空气密度
density_center = 1.078;

% 定义火坑边缘的空气密度为日常密度值
density_edge = 1.225;

% 创建一个网格来表示环境
[x, y] = meshgrid(-radius:0.1:radius, -radius:0.1:radius);

% 计算每个点到火坑中心的距离
r = sqrt((x - center_x).^2 + (y - center_y).^2);

% 定义高斯函数的标准差
sigma = radius / 3;

% 使用高斯函数计算空气密度
density = density_edge + (density_center - density_edge) * exp(-(r.^2) / (2 * sigma^2));


% 绘制空气密度图
surf(x, y, density);
xlabel('X');
ylabel('Y');
zlabel('Air Density');
title('Air Density over the Fire Pit');
