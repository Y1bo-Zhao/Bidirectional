function eR = calculateAttitudeError(q, qd)
    % 计算期望姿态四元数的共轭
    qd_conj = [qd(1), -qd(2), -qd(3), -qd(4)];
    
    % 计算误差四元数
    qe = quatmultiply(qd_conj, q);
    
    % 将误差四元数转换为误差向量
    eR = 2 * qe(2:4)';
end