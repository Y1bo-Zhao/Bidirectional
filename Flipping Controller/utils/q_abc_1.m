function q = q_abc_1(a,b,c)
    % Extract the components of q1
    w = (-b)/sqrt(2*(1-c));
    x = (1-c)/sqrt(2*(1-c));
    y = 0;
    z = a/sqrt(2*(1-c));

    
    % Combine the results into a single quaternion
    q = [w, x, y, z];
end
