% 假设out.x, out.y, out.z, out.tout是你的数据
x = out.x.Data;
y = out.y.Data;
z = out.z.Data;
z = z;  % 注意z轴反转

% 创建一个新的3D图形窗口
figure;

% 使用plot3函数绘制3D轨迹
plot3(x, y, z, 'LineWidth', 2);
xlim([-10, 50]);
ylim([-10, 50]);
zlim([-10, 50]);
set(gca,'ytick',-10:10:50);
set(gca,'xtick',-10:10:50);
set(gca,'ztick',-10:10:50);
set(gca, 'ZDir', 'reverse');

% 添加标题和坐标轴标签
title('无人机轨迹');
xlabel('X');
ylabel('Y');
zlabel('Z');
hold on;

% 启用网格
grid on;

%% 🔥绘制火坑
center_x = 10;
center_y = 10;
radius = 3;
z_level = 5;

theta = linspace(0, 2*pi, 100);  % 更平滑的圆
x_circle = center_x + radius * cos(theta);
y_circle = center_y + radius * sin(theta);
z_circle = z_level * ones(size(theta));

% 使用 patch 绘制一个红色的透明圆盘，模拟火坑
patch(x_circle, y_circle, z_circle, 'r', ...
      'FaceAlpha', 0.4, 'EdgeColor', 'none');

% 如果你希望加一点立体效果（比如边缘更明显），可以加边线
plot3(x_circle, y_circle, z_circle, 'r--', 'LineWidth', 1);

%% 🚁绘制机体Z轴方向
phi = out.phi.Data;
theta = out.theta.Data;
psi = out.psi.Data;
axis_length = 2;  % 轴的长度
step = 20;        % 每隔step帧绘制一次

for k = 1:step:length(x)
    R = eul2rotm([phi(k), theta(k), psi(k)]);  % 欧拉角转旋转矩阵

    w = R(:,3) * axis_length;  % 机体Z轴
    quiver3(x(k), y(k), z(k), w(1), w(2), w(3), 'b', 'LineWidth', 2, 'AutoScale', 'off');
end

% 手动绘制最后一个点的机体坐标系（如果未绘制到）
if mod(length(x), step) ~= 0
    k = length(x);
    R = eul2rotm([phi(k), theta(k), psi(k)]);
    w = R(:,3) * axis_length;
    quiver3(x(k), y(k), z(k), w(1), w(2), w(3), 'b', 'LineWidth', 2, 'AutoScale', 'off');
end

hold off;
