function [ dis ] = DistanceToLine( linePoint, dir, point )
% ����㵽ֱ�ߵľ���
% ����ֱ���ϵ�һ��linePoint��ֱ�߷���dir
% A = linePoint;
% B = linePoint + dir;
% C = point;

AB = dir;
AC = point - linePoint;
dis = norm(cross(AB, AC)) / norm(AB);
end

