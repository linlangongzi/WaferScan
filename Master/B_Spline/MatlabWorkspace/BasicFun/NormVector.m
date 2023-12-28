function [ Pt,N ] = NormVector( n,p,U,m,q,V,X,Y,Z,u,v,d)
%   input:
%       n：u方向控制点个数，n+1
%       p：u方向B样条基函数阶次，p次p+1阶
%       U：u方向节点矢量
%       m：v方向控制点个数，m+1
%       q：v方向B样条基函数阶次，q次q+1阶
%       V：v方向节点矢量
%       X:三维点的X坐标对应曲面的控制点
%       Y:三维点的Y坐标对应曲面的控制点
%       Z:三维点的Z坐标对应曲面的控制点
%       u：u方向考查参数
%       v：v方向考查参数
%       d:直到d阶的偏导矢，实际上只需要计算0阶（即曲面值）和1阶
%   output:
%       Pt:(u,v)对应的三维点(x,y,z)
%       N:(u,v)对应的三维点Pt对应的法向量

    X_temp = SurfaceDerivsAlg1(n,p,U,m,q,V,X,u,v,d);
    Y_temp = SurfaceDerivsAlg1(n,p,U,m,q,V,Y,u,v,d);
    Z_temp = SurfaceDerivsAlg1(n,p,U,m,q,V,Z,u,v,d);
    du = [X_temp(2,1),Y_temp(2,1),Z_temp(2,1)];%du = [dx/du,dy/du,dz/du]
    dv = [X_temp(1,2),Y_temp(1,2),Z_temp(1,2)];%dv = [dx/dv,dy/dv,dz/dv]
    
    N = cross(du,dv);%曲面的法向量为N = du X dv，即叉积
    N = N/norm(N);
    Pt = [X_temp(1,1),Y_temp(1,1),Z_temp(1,1)];%(u,v)对应的点（x,y,z)
end

