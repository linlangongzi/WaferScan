function [ dis ] = DistanceToLine( linePoint, dir, point )
% 计算点到直线的距离
% 给出直线上的一点linePoint和直线方向dir
% A = linePoint;
% B = linePoint + dir;
% C = point;

AB = dir;
AC = point - linePoint;
dis = norm(cross(AB, AC)) / norm(AB);
end

