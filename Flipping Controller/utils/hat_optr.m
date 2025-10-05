function w_hat = hat_optr(w)
%用于表示向量的叉积操作
w_hat = [0 -w(3) w(2);w(3) 0 -w(1);-w(2) w(1) 0];

end