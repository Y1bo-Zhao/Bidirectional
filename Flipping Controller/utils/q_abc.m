function q = q_abc(a,b,c)
    % Extract the components of q1
    w = (1+c)/sqrt(2*(1+c));
    x = (-b)/sqrt(2*(1+c));
    y = a/sqrt(2*(1+c));
    z = 0;

    
    % Combine the results into a single quaternion
    q = [w, x, y, z];
end
