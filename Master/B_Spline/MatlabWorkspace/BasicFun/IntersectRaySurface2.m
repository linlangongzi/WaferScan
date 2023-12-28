function [ point, u, v ] = IntersectRaySurface2( n,p,U,m,q,V,ctrlX, ctrlY, ctrlZ, linePoint, dir )
% 求空间中的直线与曲面的交点point
% 将曲面及直线进行旋转，使直线与Z轴平行
% 计算曲面上的点，使其x,y与直线的x,y相等即可
% 采用拟牛顿迭代法,计算速度快
%% 计算旋转矩阵
dir = dir / norm(dir);
% 将直线方向向量转化为z分量为正
if (dir(3) < 0)
    dir = -dir;
end
if (dir(3) == 0 )
    b = 90;
else
    b = atan(dir(1) / dir(3));
end
a = asin(dir(2));
Rb = [cos(b),    0,  -sin(b),  0;...
      0,                1,  0,              0;...
      sin(b),   0,  cos(b),  0;...
      0,                0,  0,              1   ];%B轴的旋转变换矩阵
Ra = [1,    0,              0,              0;...
      0,    cos(a),  -sin(a), 0;...
      0,    sin(a), cos(a),   0;...
      0,    0,              0,              1];     %A轴的旋转变换矩阵
Rotate = Ra * Rb;

%% 对曲面控制点和直线特征点进行旋转变换
ctrlSize = size(ctrlX);
RctrlX = zeros(ctrlSize);
RctrlY = zeros(ctrlSize);
RctrlZ = zeros(ctrlSize);
for i = 1 : ctrlSize(1)
    for j = 1 : ctrlSize(2)
        p1 = [ctrlX(i, j); ctrlY(i, j); ctrlZ(i, j); 1];
        p2 = Rotate * p1;
        RctrlX(i,j) = p2(1);
        RctrlY(i,j) = p2(2);
        RctrlZ(i,j) = p2(3);        
    end
end
point = Rotate * [linePoint;1];
linePoint = point(1:3);

%% 利用曲面上的点的反解程序计算交点
[ ~,u, v, ~ ] = InversePointOnSurface2( n,p,U,m,q,V,RctrlX, RctrlY, RctrlZ, linePoint);
point = [SurfacePoint(n,p,U,m,q,V,ctrlX, u, v);...
         SurfacePoint(n,p,U,m,q,V,ctrlY, u, v);...
         SurfacePoint(n,p,U,m,q,V,ctrlZ, u, v)];
end