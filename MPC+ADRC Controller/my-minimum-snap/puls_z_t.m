[~, n] = size(X_n);
Z_n = zeros(1,n);
Psi = zeros(1,n);
matrix = [X_n',Y_n',Z_n',Psi'];
time_vector = linspace(0, n-1, n);
ref_input_timed = [time_vector', matrix];
ref_input_struct = timeseries(ref_input_timed(:, 2:end), ref_input_timed(:, 1));
