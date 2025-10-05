Data = out.xuanzhuan.Data;
w1 = Data(:, 1); % 提取第1列
w2 = Data(:, 2); % 提取第2列
w3 = Data(:, 3); % 提取第3列
w4 = Data(:, 4); % 提取第4列
% 定义时间轴，假设每行之间时间间隔为0.1s，共有101行数据
t = 0:0.1:(size(Data, 1)-1) * 0.1;

% 对数据进行平滑处理
w1_smooth = smoothdata(w1, 'movmean', 5);
w2_smooth = smoothdata(w2, 'movmean', 5);
w3_smooth = smoothdata(w3, 'movmean', 5);
w4_smooth = smoothdata(w4, 'movmean', 5);

% 绘制平滑后的图形
figure;
plot(t, w1_smooth, 'DisplayName', 'w1', 'LineWidth', 2);
hold on;
plot(t, w2_smooth, 'DisplayName', 'w2', 'LineWidth', 2);
plot(t, w3_smooth, 'DisplayName', 'w3', 'LineWidth', 2);
plot(t, w4_smooth, 'DisplayName', 'w4', 'LineWidth', 2);
hold off;

% 添加图例和标签
xlabel('Time(s)');
ylabel('Motor Speed(rad/s)');
%title('Angular Velocity over Time');
legend;
grid on;


%xlim([-1 10]);
%ylim([-600 0]);