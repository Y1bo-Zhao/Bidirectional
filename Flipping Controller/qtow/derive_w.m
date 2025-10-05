% 定义符号变量
syms a b c psi real
syms t real

% 定义四元数 q_abc 和 q_psi
q_abc = 1/sqrt(2*(1+c)) * [1+c, -b, a, 0];
q_psi = [cos(psi/2), 0, 0, sin(psi/2)];

% 定义四元数乘法函数
quaternion_mult = @(q1, q2) [
    q1(1)*q2(1) - q1(2)*q2(2) - q1(3)*q2(3) - q1(4)*q2(4), ...
    q1(1)*q2(2) + q1(2)*q2(1) + q1(3)*q2(4) - q1(4)*q2(3), ...
    q1(1)*q2(3) - q1(2)*q2(4) + q1(3)*q2(1) + q1(4)*q2(2), ...
    q1(1)*q2(4) + q1(2)*q2(3) - q1(3)*q2(2) + q1(4)*q2(1)
];

% 计算 q
q = quaternion_mult(q_abc, q_psi);

% 定义四元数逆
quaternion_inverse = @(q) [
    q(1), -q(2), -q(3), -q(4)
] / (q(1)^2 + q(2)^2 + q(3)^2 + q(4)^2);

% 计算 q 的逆
q_inv = quaternion_inverse(q);

% 定义符号四元数 q_t 和其时间导数 dq_t
q_t = sym('q_t', [1, 4]);
dq_t = sym('dq_t', [1, 4]);

% 将 q 替换为函数 q(t)
q_t = symfun(q, t);
dq_t = diff(q_t, t);

% 计算角速度 w
w = 2 * quaternion_mult(q_inv, dq_t);



% 显示结果
disp('四元数 q:')
disp(q)
disp('四元数 q 的逆:')
disp(q_inv)
disp('角速度 w:')
disp(w)
