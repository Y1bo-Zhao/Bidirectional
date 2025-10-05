zero_length = 200;
matrix = zeros(zero_length, 5);  % 生成全为0的矩阵
matrix(1:zero_length/2, 5) = -1;  % eta始终为0
matrix(zero_length/2+1:zero_length, 5) = -1;  % eta始终为0

% 生成时间向量，时间间隔为0.1
time_vector = linspace(0, (zero_length-1) * 0.1, zero_length);  

ref_input_timed = [time_vector', matrix];
ref_input_struct = timeseries(ref_input_timed(:, 2:end), ref_input_timed(:, 1));
