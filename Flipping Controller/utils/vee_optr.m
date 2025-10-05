function w = vee_optr(R)
%将反对称矩阵还原为其对应的向量
w = [-R(2,3) R(1,3) -R(1,2)]';