function [ point, u, v ] = IntersectRaySurface2( n,p,U,m,q,V,ctrlX, ctrlY, ctrlZ, linePoint, dir )
% ��ռ��е�ֱ��������Ľ���point
% �����漰ֱ�߽�����ת��ʹֱ����Z��ƽ��
% ���������ϵĵ㣬ʹ��x,y��ֱ�ߵ�x,y��ȼ���
% ������ţ�ٵ�����,�����ٶȿ�
%% ������ת����
dir = dir / norm(dir);
% ��ֱ�߷�������ת��Ϊz����Ϊ��
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
      0,                0,  0,              1   ];%B�����ת�任����
Ra = [1,    0,              0,              0;...
      0,    cos(a),  -sin(a), 0;...
      0,    sin(a), cos(a),   0;...
      0,    0,              0,              1];     %A�����ת�任����
Rotate = Ra * Rb;

%% ��������Ƶ��ֱ�������������ת�任
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

%% ���������ϵĵ�ķ��������㽻��
[ ~,u, v, ~ ] = InversePointOnSurface2( n,p,U,m,q,V,RctrlX, RctrlY, RctrlZ, linePoint);
point = [SurfacePoint(n,p,U,m,q,V,ctrlX, u, v);...
         SurfacePoint(n,p,U,m,q,V,ctrlY, u, v);...
         SurfacePoint(n,p,U,m,q,V,ctrlZ, u, v)];
end