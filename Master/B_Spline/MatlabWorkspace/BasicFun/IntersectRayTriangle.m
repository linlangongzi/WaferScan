function [flag, I ] = IntersectRayTriangle( ray, triangle)
%����ֱ��ray��������Triangle�Ľ��㣨����������������ڲ���
%input
%       ray:(px,py,pz;nx,ny,nz)
%       triangle:(x1,y1,z1;x2,y2,z2;x3,y3,z3;nx,ny,nz)
%output
%       flag:0-�޽��㣻1-�н���
%       I��(x,y,z)
    %�����β���
    u = triangle(2,:) - triangle(1,:);
    v = triangle(3,:) - triangle(1,:);
    n = triangle(4,:);
    %ֱ�߲���
    dir = ray(2,:);
    w1 = ray(1,:) - triangle(1,:);
    r = - (w1 * n') / (dir * n'); %����
    I = ray(1,:) + r * dir;     %���ֱ����������ƽ���ϵĽ���
    w = I - triangle(1,:);
    uv = u * v';
    wv = w * v';
    vv = v * v';
    wu = w * u';
    uu = u * u';
    s = (uv * wv - vv * wu) / (uv * uv - uu * vv);
    t = (uv * wu - uu * wv) / ( uv * uv - uu * vv);
    if (s == 0 && t <= 1 && t >= 0)
        flag = 1;           %���ڱ�����
    elseif (t == 0 && s <= 1 && s >= 0)
        flag = 1;           %���ڱ�����
    elseif (s >= 0 && t >= 0 && s + t == 1)
        flag = 1;           %���ڱ�����
    elseif (s > 0 && t >0 && s + t < 1)
        flag = 1;           %��������������
    else
        flag = 0;           %�޽���
    end
end

