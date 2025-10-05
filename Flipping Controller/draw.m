% 假设out.x, out.y, out.z, out.tout是你的数据
x = out.x.Data;
y = out.y.Data;
z = out.z.Data;
z = -z;

% 创建一个新的3D图形窗口
figure;

% 使用plot3函数绘制3D轨迹
plot3(x, y, z, 'LineWidth', 2);
xlim([-50, 50]);
ylim([-50, 50]);
zlim([-50, 50]);
set(gca,'ytick',-50:10:50);
set(gca,'xtick',-50:10:50);
set(gca,'ztick',-50:10:50);
% 添加标题和坐标轴标签
title('无人机轨迹');
xlabel('X');
ylabel('Y');
zlabel('Z');

% 启用网格
grid on;

% 定义火坑的中心位置和半径
center_x = 10;
center_y = 10;
radius = 3;

% 在z=-5的位置绘制一个半径为r的火坑
hold on;
theta = 0:0.01:2*pi;
x_circle = center_x + radius * cos(theta);
y_circle = center_y + radius * sin(theta);
z_circle = -5 * ones(size(theta));
patch(x_circle, y_circle, z_circle, 'r', 'FaceAlpha', 0.3, 'EdgeColor', 'none');

% 每隔十步绘制机体坐标系


phi = out.phi.Data;
theta = out.theta.Data;
psi = out.psi.Data;
axis_length = 2;  % 轴的长度
step = 20;  % 每隔十步绘制一次
for k = 1:step:length(x)
    % 欧拉角转换为旋转矩阵
    R = eul2rotm([phi(k), theta(k), psi(k)]);

%     % 机体X轴
%     u = R(:,1) * axis_length;
%     quiver3(x(k), y(k), z(k), u(1), u(2), u(3), 'r', 'LineWidth', 2, 'AutoScale', 'off');
% 
%     % 机体Y轴
%     v = R(:,2) * axis_length;
%     quiver3(x(k), y(k), z(k), v(1), v(2), v(3), 'g', 'LineWidth', 2, 'AutoScale', 'off');

    % 机体Z轴
    w = R(:,3) * axis_length;
    quiver3(x(k), y(k), z(k), w(1), w(2), w(3), 'b', 'LineWidth', 2, 'AutoScale', 'off');
end

% 手动绘制最后一个点的机体坐标系
if mod(length(x), step) ~= 0
    k = length(x);  % 最后一个点的索引
    R = eul2rotm([psi(k), theta(k), phi(k)]);  % 欧拉角转换为旋转矩阵
    w = R(3,:) * axis_length;  % 机体Z轴
    quiver3(x(k), y(k), z(k), w(1), w(2), w(3), 'b', 'LineWidth', 2, 'AutoScale', 'off');
end

hold off;
