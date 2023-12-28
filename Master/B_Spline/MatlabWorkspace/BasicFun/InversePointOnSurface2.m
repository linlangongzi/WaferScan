function [ pointOut,uk, vk, count ] = InversePointOnSurface2( n,p,U,m,q,V,ctrlX, ctrlY, ctrlZ, pointIn )
% ��ռ��еĵ�pointIn�������ϵ������pointOut
% ������ţ�ٵ�����,�����ٶȿ죬��x,yΪ���ս��м���
% �ο����ף����򹤳���NURBS������ֱ�߽�����ټ���_������

% f1 = x(u,v) - x0;
% f2 = y(u,v) - y0;
% f3 = z(u,v) - z0;

x0 = pointIn(1);
y0 = pointIn(2);

%��ʼֵ
uk = 0.5;
vk = 0.5;

f1k = SurfacePoint(n,p,U,m,q,V,ctrlX,uk,vk) - x0;
f2k = SurfacePoint(n,p,U,m,q,V,ctrlY,uk,vk) - y0;

df1 = SurfaceDerivsAlg1( n,p,U,m,q,V,ctrlX,uk,vk,1 );
df2 = SurfaceDerivsAlg1( n,p,U,m,q,V,ctrlY,uk,vk,1 );
%ֻ����x,y�����м������
Ak = [df1(2,1), df1(1,2);...
      df2(2,1), df2(1,2)];
  
error = [f1k; f2k];
sk = [1;1];
count = 0;
while norm(sk) > 1.e-3
    uvk1  = [uk, vk] - (Ak\[f1k; f2k])';
    uk1 = uvk1(1);
    vk1 = uvk1(2);
    f1k1 = SurfacePoint(n,p,U,m,q,V,ctrlX,uk1,vk1) - x0;
    f2k1 = SurfacePoint(n,p,U,m,q,V,ctrlY,uk1,vk1) - y0;
    yk = [f1k1 - f1k; f2k1 - f2k];
    sk = [uk1- uk; vk1 - vk];
    
    Ak1 = Ak + (yk - Ak*sk) * sk' / (sk' * sk);
    
    %׼����һ�ֵ���
    Ak = Ak1;
    uk = uk1;
    vk = vk1;
    f1k = f1k1;
    f2k = f2k1;
    error = [f1k; f2k];
    count = count + 1;
end
    pointOut = [SurfacePoint(n,p,U,m,q,V,ctrlX,uk,vk), ...
                SurfacePoint(n,p,U,m,q,V,ctrlY,uk,vk), ...
                SurfacePoint(n,p,U,m,q,V,ctrlZ,uk,vk)];
end
  