function q = q_psi(psi)
    % Extract the components of q1
    w = cos(psi/2);
    x = 0;
    y = 0;
    z = sin(psi/2);

    
    % Combine the results into a single quaternion
    q = [w, x, y, z];
end
