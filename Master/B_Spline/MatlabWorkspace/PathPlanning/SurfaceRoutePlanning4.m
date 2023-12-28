function [ ctrlX, ctrlY, ctrlZ, ctrlA, ctrlB ] = SurfaceRoutePlanning4( x, y, z, a, b, order )
%1、对点(x,y,z,a,b)以一个时间轴为基准进行拟合，计算各控制点及节点矢量
%2、配合SurfaceRoutePlanning3使用，利用SurfaceRoutePlanning3得到每条扫描线有相同数量的点
%3、对SurfaceRoutePlanning3得到的扫描线进行反解后得到各轴坐标
%input:
%   x,y,z,a,b:均为行向量，元素个数n+1，为轴坐标，将直接下发给下位机
%   order:曲线阶次，只能是3
%output:
%   ctrlX:行向量，前3*(n+1)为控制点，后n+p+2个元素为节点矢量，控制点格式为（x,t,0）,三个一组

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

