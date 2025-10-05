function poly_coef = MinimumSnapQPSolver(waypoints, ts, n_seg, n_order)
    % waypoints 点集
    % ts，每段的时间，相对时间轴坐标系
    % n_seg，段数
    % n_order ,最高阶数
    % 起点位置、速度、加速度、jerk 数值
    start_cond = [waypoints(1), 0, 0, 0];
    % 终点位置、速度、加速度、jerk 数值
    start_cond = [waypoints(1), 0, 0, 0];
    end_cond   = [waypoints(end), 0, 0, 0];
    % 计算Q矩阵
    Q = getQ(n_seg, n_order, ts);
    % 计算等式约束
    [Aeq, beq] = getAbeq(n_seg, n_order, waypoints, ts, start_cond, end_cond);
    % 用solver优化，得到优化后的所有参数poly_coef 
    f = zeros(size(Q,1),1);
    poly_coef = quadprog(Q,f,[],[],Aeq, beq);
end
