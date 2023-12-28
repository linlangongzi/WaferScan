function [ L2 ] = Refraction( N1, L1, c1, c2 )
% 声折射定律
% N1为面的法向量，朝内,列向量
% L1为声束的传播方向，列向量
% c1为第一介质的声速
% c2为第二介质的声速
% L2 = a * N1 + b * L1;
t1= acos(N1' * L1);
t2 = asin(c2 * sin(t1) / c1);

b = c2 / c1;  %显然b>0
% a = cos(t2) - b * cos(t1);
a = sqrt(1 - b^2 + b^2 * (L1'*N1)^2) - b *(L1' * N1);

L2 = a * N1 + b * L1;

end

