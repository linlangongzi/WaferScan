function [ uv_out,Nout] = CurveSubdivide( uv_in,n,p,U,m,q,V,X,Y,Z,L )
%�������ϵ����ߣ���(uin,vin)���ƣ����еȻ���Lϸ��
%   input:
%       uv_in:���߿��Ƶ㣬Ϊ������,n*2
%       n,p,U,m,q,V,X,Y,Z:������Ϣ
%   output:
%       uv_out:ϸ�ֺ�ĵ����У�n*2
%       Nout:��Ӧϸ�ֺ��ķ�����
% disp('start the CurveSubdivide');
uin = uv_in(:,1);
vin = uv_in(:,2);
%%
%��(uin,vin)�����������
num = length(uin);
pt = 2;
temp = [uin,vin];
[ ~,~,Uuv,Puv] = GlobalCurveInterp( num-1,temp,pt);%�Խ����߽���p��ȫ�ֲ�ֵ,
%����3����Ͽ��ܵ�������Ť����ʹ��u,v����[0,1]�ķ�Χ,���ѡ��2�����
%%
%���¶����߽���ϸ��
t = 0;
dt = 0.0002;
uout = zeros(2,1);
vout = zeros(2,1);
Nout = zeros(1,3);
index = 0;%ϸ�ֵ����
d = 1;

%�����һ����ķ�����
u1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,1),t,d);%�������
v1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,2),t,d);
x_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,X,u1(1),v1(1),d );
y_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Y,u1(1),v1(1),d );
z_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Z,u1(1),v1(1),d );
du = [x_temp(2,1),y_temp(2,1),z_temp(2,1)];%du = [dx/du,dy/du,dz/du]
dv = [x_temp(1,2),y_temp(1,2),z_temp(1,2)];%dv = [dx/dv,dy/dv,dz/dv]
N = cross(du,dv);%����ķ�����ΪN = du X dv�������
N = N/norm(N);
Nout(index+1,:) = N;%��Ӧ��ķ�����    
    
while t<1
    dl = 0;
    u1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,1),t,d);%�������
    v1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,2),t,d);
    index = index +1;%ϸ�ֵ������1
    if u1(1)>=1
        uout(index) = 1;%u1(1);
    else
        uout(index) = u1(1);
    end
    if v1(1)>=1
        vout(index) = 1;
    else
        vout(index) = v1(1);
    end
    while  dl<L && t<1
        x_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,X,u1(1),v1(1),d );
        y_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Y,u1(1),v1(1),d );
        z_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Z,u1(1),v1(1),d );
        ddu = u1(2)*dt;
        ddv = v1(2)*dt;
        dlPower = (abs(x_temp(2,1)*ddu) + abs(x_temp(1,2)*ddv))^2 + ...
                  (abs(y_temp(2,1)*ddu) + abs(y_temp(1,2)*ddv))^2 + ...
                  (abs(z_temp(2,1)*ddu) + abs(z_temp(1,2)*ddv))^2;
        dl = dl + sqrt(dlPower);
        t = t + dt;
        if t>1
            t = 1;
        end
        u1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,1),t,d);%�������
        v1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,2),t,d);
    end
    %����ڶ����㵽���һ����ķ�����
    du = [x_temp(2,1),y_temp(2,1),z_temp(2,1)];%du = [dx/du,dy/du,dz/du]
    dv = [x_temp(1,2),y_temp(1,2),z_temp(1,2)];%dv = [dx/dv,dy/dv,dz/dv]
    N = cross(du,dv);%����ķ�����ΪN = du X dv�������
    N = N/norm(N);
    Nout(index+1,:) = N;%��Ӧ��ķ�����
end
uout(index+1) = 1;%CurvePoint(num-1,pt,Uuv,Puv(:,2),0);
vout(index+1) = CurvePoint(num-1,pt,Uuv,Puv(:,2),1);

uv_out = [uout,vout];%���е��Ӧ��(u,v)
% disp('finish the CurveSubdivide');
end

