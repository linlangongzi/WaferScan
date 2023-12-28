function [flag, I ] = IntersectRayTriangle( ray, triangle)
%计算直线ray与三角形Triangle的交点（交点必须在三角形内部）
%input
%       ray:(px,py,pz;nx,ny,nz)
%       triangle:(x1,y1,z1;x2,y2,z2;x3,y3,z3;nx,ny,nz)
%output
%       flag:0-无交点；1-有交点
%       I：(x,y,z)
    %三角形参数
    u = triangle(2,:) - triangle(1,:);
    v = triangle(3,:) - triangle(1,:);
    n = triangle(4,:);
    %直线参数
    dir = ray(2,:);
    w1 = ray(1,:) - triangle(1,:);
    r = - (w1 * n') / (dir * n'); %标量
    I = ray(1,:) + r * dir;     %算出直线在三角形平面上的交点
    w = I - triangle(1,:);
    uv = u * v';
    wv = w * v';
    vv = v * v';
    wu = w * u';
    uu = u * u';
    s = (uv * wv - vv * wu) / (uv * uv - uu * vv);
    t = (uv * wu - uu * wv) / ( uv * uv - uu * vv);
    if (s == 0 && t <= 1 && t >= 0)
        flag = 1;           %点在边线上
    elseif (t == 0 && s <= 1 && s >= 0)
        flag = 1;           %点在边线上
    elseif (s >= 0 && t >= 0 && s + t == 1)
        flag = 1;           %点在边线上
    elseif (s > 0 && t >0 && s + t < 1)
        flag = 1;           %交点在三角形内
    else
        flag = 0;           %无交点
    end
end

