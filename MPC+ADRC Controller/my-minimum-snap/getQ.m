function Q = getQ(n_seg, n_order, ts)
    % ts，每段的时间，相对时间轴坐标系
    % n_seg，段数
    % n_order ,最高次项
    Q = [];
    % 每段分别计算Q
    for k = 1:n_seg
    	% 初始化，对于7次多项式，维度为8*8
        Q_k = zeros(n_order+1, n_order+1);
        %行
        for r = 4:n_order
            %列
            for c= 4:n_order
            	% t_i为第i段的时间，t_{i-1}为第i段的起始时间，取相对时间轴，值为0
                Q_k(r+1,c+1) = (factorial(r)/factorial(r-4))*(factorial(c)/factorial(c-4))*(1/(r+c-7))*(ts(k)^(r+c-7));
            end
        end
        Q = blkdiag(Q, Q_k);
    end
end
