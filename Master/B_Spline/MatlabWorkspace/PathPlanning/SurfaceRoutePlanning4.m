function [ ctrlX, ctrlY, ctrlZ, ctrlA, ctrlB ] = SurfaceRoutePlanning4( x, y, z, a, b, order )
%1���Ե�(x,y,z,a,b)��һ��ʱ����Ϊ��׼������ϣ���������Ƶ㼰�ڵ�ʸ��
%2�����SurfaceRoutePlanning3ʹ�ã�����SurfaceRoutePlanning3�õ�ÿ��ɨ��������ͬ�����ĵ�
%3����SurfaceRoutePlanning3�õ���ɨ���߽��з����õ���������
%input:
%   x,y,z,a,b:��Ϊ��������Ԫ�ظ���n+1��Ϊ�����꣬��ֱ���·�����λ��
%   order:���߽״Σ�ֻ����3
%output:
%   ctrlX:��������ǰ3*(n+1)Ϊ���Ƶ㣬��n+p+2��Ԫ��Ϊ�ڵ�ʸ�������Ƶ��ʽΪ��x,t,0��,����һ��

n = length(x) - 1;
T = linspace(0,1,n+1)';
dx = max(abs(x)) - min(abs(x));
dy = max(abs(y)) - min(abs(y));
dz = max(abs(z)) - min(abs(z));
da = max(abs(a)) - min(abs(a));
db = max(abs(b)) - min(abs(b));
cof = 10;
Tx = cof * dx * T;
Ty = cof * dy * T;
Tz = cof * dz * T;
Ta = cof * da * T;
Tb = cof * db * T;


[ ~, ~, Ux, X] = GlobalCurveInterp( n, [x', Tx, zeros(n+1, 1)], order);
[ ~, ~, Uy, Y] = GlobalCurveInterp( n, [y', Ty, zeros(n+1, 1)], order);
[ ~, ~, Uz, Z] = GlobalCurveInterp( n, [z', Tz, zeros(n+1, 1)], order);
[ ~, ~, Ua, A] = GlobalCurveInterp( n, [a', Ta, zeros(n+1, 1)], order);
[ ~, ~, Ub, B] = GlobalCurveInterp( n, [b', Tb, zeros(n+1, 1)], order);
ctrlX = zeros(0, 2 * (n + 1) + n + order + 2);
for i = 0 : n
   ctrlX((2 * i + 1) :(2 * i + 2)) = X(i + 1, 1 : 2);
   ctrlY((2 * i + 1) :(2 * i + 2)) = Y(i + 1, 1 : 2);
   ctrlZ((2 * i + 1) :(2 * i + 2)) = Z(i + 1, 1 : 2);
   ctrlA((2 * i + 1) :(2 * i + 2)) = A(i + 1, 1 : 2);
   ctrlB((2 * i + 1) :(2 * i + 2)) = B(i + 1, 1 : 2);
end
ctrlX = [ctrlX, Ux];
ctrlY = [ctrlY, Uy];
ctrlZ = [ctrlZ, Uz];
ctrlA = [ctrlA, Ua];
ctrlB = [ctrlB, Ub];

end

