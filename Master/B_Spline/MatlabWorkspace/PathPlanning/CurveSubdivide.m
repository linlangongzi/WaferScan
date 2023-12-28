function [ uv_out,Nout] = CurveSubdivide( uv_in,n,p,U,m,q,V,X,Y,Z,L )
%对曲面上的曲线（由(uin,vin)控制）进行等弧长L细分
%   input:
%       uv_in:曲线控制点，为点序列,n*2
%       n,p,U,m,q,V,X,Y,Z:曲面信息
%   output:
%       uv_out:细分后的点序列，n*2
%       Nout:对应细分后点的法向量
% disp('start the CurveSubdivide');
uin = uv_in(:,1);
vin = uv_in(:,2);
%%
%对(uin,vin)进行曲线拟合
num = length(uin);
pt = 2;
temp = [uin,vin];
[ ~,~,Uuv,Puv] = GlobalCurveInterp( num-1,temp,pt);%对进给线进行p次全局插值,
%此种3次拟合可能导致曲线扭曲，使得u,v超出[0,1]的范围,因此选择2次拟合
%%
%以下对曲线进行细分
t = 0;
dt = 0.0002;
uout = zeros(2,1);
vout = zeros(2,1);
Nout = zeros(1,3);
index = 0;%细分点个数
d = 1;

%计算第一个点的法向量
u1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,1),t,d);%二阶拟合
v1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,2),t,d);
x_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,X,u1(1),v1(1),d );
y_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Y,u1(1),v1(1),d );
z_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Z,u1(1),v1(1),d );
du = [x_temp(2,1),y_temp(2,1),z_temp(2,1)];%du = [dx/du,dy/du,dz/du]
dv = [x_temp(1,2),y_temp(1,2),z_temp(1,2)];%dv = [dx/dv,dy/dv,dz/dv]
N = cross(du,dv);%曲面的法向量为N = du X dv，即叉积
N = N/norm(N);
Nout(index+1,:) = N;%对应点的法向量    
    
while t<1
    dl = 0;
    u1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,1),t,d);%二阶拟合
    v1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,2),t,d);
    index = index +1;%细分点个数加1
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
        u1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,1),t,d);%二阶拟合
        v1 = CurveDerivsAlg1(num-1,pt,Uuv,Puv(:,2),t,d);
    end
    %计算第二个点到最后一个点的法向量
    du = [x_temp(2,1),y_temp(2,1),z_temp(2,1)];%du = [dx/du,dy/du,dz/du]
    dv = [x_temp(1,2),y_temp(1,2),z_temp(1,2)];%dv = [dx/dv,dy/dv,dz/dv]
    N = cross(du,dv);%曲面的法向量为N = du X dv，即叉积
    N = N/norm(N);
    Nout(index+1,:) = N;%对应点的法向量
end
uout(index+1) = 1;%CurvePoint(num-1,pt,Uuv,Puv(:,2),0);
vout(index+1) = CurvePoint(num-1,pt,Uuv,Puv(:,2),1);

uv_out = [uout,vout];%所有点对应的(u,v)
% disp('finish the CurveSubdivide');
end

