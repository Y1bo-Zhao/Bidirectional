function mode = switchMode(b3)
    % 持久化变量，用于保存上一次的模式和点积
    persistent prev_mode prev_dot_product
    
    % 设置α的值
    alpha = 0.5;  % 你可以根据需要调整这个值
    
    % 初始模式和点积
    if isempty(prev_mode)
        prev_mode = 'N';
        prev_dot_product = 1;
    end
    
    % 计算b3与(0,0,1)的点积
    dot_product = b3(3);  % 因为b3*(0,0,1) = c

    % 根据点积的变化情况切换模式
    if prev_dot_product > -alpha && dot_product < -alpha
        mode = 'S';
    elseif prev_dot_product < alpha && dot_product > alpha
        mode = 'N';
    else
        mode = prev_mode; % 保持前一个模式
    end
    
    % 更新持久化变量
    prev_mode = mode;
    prev_dot_product = dot_product;
end