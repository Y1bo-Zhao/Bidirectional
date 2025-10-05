
path = ginput() * 100.0;

% n次项
n_order       = 7;
% 段数
n_seg         = size(path,1)-1;
% 每段参数数目
n_poly_perseg = (n_order+1);

% 每段轨迹的时间，按距离均匀分配时间，或直接赋值
ts = zeros(n_seg, 1);
for i = 1:n_seg
   ts(i) = 1.0;
end

% x,y轴分别计算参数
poly_coef_x = MinimumSnapQPSolver(path(:, 1), ts, n_seg, n_order);
poly_coef_y = MinimumSnapQPSolver(path(:, 2), ts, n_seg, n_order);

% display the trajectory
X_n = [];
Y_n = [];
k = 1;
tstep = 0.1;
for i=0:n_seg-1
    % 取计算好的每段参数，此时参数根据阶数由低到高
    Pxi = poly_coef_x(i*n_poly_perseg +1:(i+1)*n_poly_perseg );
    % 阶数由高到低排列
    Pxi = flipud(Pxi);
    Pyi = poly_coef_y(i*n_poly_perseg +1:(i+1)*n_poly_perseg );
    Pyi = flipud(Pyi); 
    % 计算坐标
    for t = 0:tstep:ts(i+1)
        X_n(k)  = polyval(Pxi, t);
        Y_n(k)  = polyval(Pyi, t);
        k = k + 1;
    end
end
 
plot(X_n, Y_n , 'Color', [0 1.0 0], 'LineWidth', 2);
hold on
scatter(path(1:size(path, 1), 1), path(1:size(path, 1), 2));

[~, n] = size(X_n);
Z_n = zeros(1,n);
Psi = zeros(1,n);
matrix = [X_n',Y_n',Z_n',Psi'];
time_vector = linspace(0, n-1, n);
ref_input_timed = [time_vector', matrix];
ref_input_struct = timeseries(ref_input_timed(:, 2:end), ref_input_timed(:, 1));

