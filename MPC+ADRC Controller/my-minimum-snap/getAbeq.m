function [Aeq beq]= getAbeq(n_seg, n_order, waypoints, ts, start_cond, end_cond)
    % n_seg 段数
    % n_order， 最高阶数
    % waypoints，点集
    % ts每段时间
    % start_cond = [waypoints(1), 0, 0, 0];起始时位置，速度，加速度，jerk
    % end_cond   = [waypoints(end), 0, 0, 0];终止时位置，速度，加速度，jerk
    
	% 所有参数的个数
	n_all_poly = n_seg*(n_order+1);
    %#####################################################
    % p,v,a,j constraint in start, 起点位置，速度，加速度，jerk的限制，时间为0，矩阵只剩4项
    Aeq_start = zeros(4, n_all_poly);
    beq_start = start_cond(:) ;
    for r=1:4
        Aeq_start(r,r) = factorial(r-1);
    end
    %#####################################################
    % p,v,a constraint in end，终点位置、速度、加速度、jerk的限制
    Aeq_end = zeros(4, n_all_poly);
    beq_end =end_cond(:);
    for r = 0:3
        for c=r:n_order
            Aeq_end(r+1,(n_seg-1)*(n_order+1)+c+1) =  (factorial(c)/ factorial(c-r))*(ts(n_seg)^(c-r));
        end
    end;
    %#####################################################
    % position constrain in all middle waypoints，中间路标点对位置的限制
    Aeq_wp = zeros(n_seg-1, n_all_poly);
    beq_wp = zeros(n_seg-1, 1);
    for r=0:n_seg-2
        for c=0:n_order
            Aeq_wp(r+1,r*(n_order+1)+c+1)= (ts(r+1)^c);
        end
        beq_wp(r+1) = waypoints(r+2);
    end
            
    %#####################################################
    % position continuity constrain between each 2
    % segments，前后两段对位置连续性的限制
    Aeq_con_p = zeros(n_seg-1, n_all_poly);
    beq_con_p = zeros(n_seg-1, 1);
    for r=0:n_seg-2
        for c=0:n_order
        	%第r段的位置
            Aeq_con_p(r+1,r*(n_order+1)+c+1)= ts(r+1)^c;
            %第r+1段的位置
            Aeq_con_p(r+1,(r+1)*(n_order+1)+c+1) = -0^c;
        end
    end
    %#####################################################
    % velocity continuity constrain between each 2 segments
    % segments，前后两段对速度连续性的限制
    Aeq_con_v = zeros(n_seg-1, n_all_poly);
    beq_con_v = zeros(n_seg-1, 1);
    for r=0:n_seg-2
        for c=1:n_order
            Aeq_con_v(r+1,r*(n_order+1)+c+1)= c*ts(r+1)^(c-1);
            Aeq_con_v(r+1,(r+1)*(n_order+1)+c+1) = -c*0^(c-1);
        end
    end
    %#####################################################
    % acceleration continuity constrain between each 2 segments
    % segments，前后两段对加速度连续性的限制
    Aeq_con_a = zeros(n_seg-1, n_all_poly);
    beq_con_a = zeros(n_seg-1, 1);
    for r=0:n_seg-2
        for c=2:n_order
            Aeq_con_a(r+1,r*(n_order+1)+c+1)= c*(c-1)*ts(r+1)^(c-2);
            Aeq_con_a(r+1,(r+1)*(n_order+1)+c+1) = -c*(c-1)*0^(c-2);
        end
    end
    %#####################################################
    % jerk continuity constrain between each 2 segments
    % segments，前后两段对jerk连续性的限制
    Aeq_con_j = zeros(n_seg-1, n_all_poly);
    beq_con_j = zeros(n_seg-1, 1);
    for r=0:n_seg-2
        for c=3:n_order
            Aeq_con_j(r+1,r*(n_order+1)+c+1)= c*(c-1)*(c-2)*ts(r+1)^(c-3);
            Aeq_con_j(r+1,(r+1)*(n_order+1)+c+1) = -c*(c-1)*(c-2)*0^(c-3);
        end
    end
    %#####################################################
    % combine all components to form Aeq and beq   
    Aeq_con = [Aeq_con_p; Aeq_con_v; Aeq_con_a; Aeq_con_j];
    beq_con = [beq_con_p; beq_con_v; beq_con_a; beq_con_j];
    Aeq = [Aeq_start; Aeq_end; Aeq_wp; Aeq_con];
    beq = [beq_start; beq_end; beq_wp; beq_con];
end
