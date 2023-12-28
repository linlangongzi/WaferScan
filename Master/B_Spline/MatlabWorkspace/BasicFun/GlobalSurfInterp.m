function [U0,V0,Xctrl0,Yctrl0,Zctrl0] = GlobalSurfInterp( nu,nv,Xin0,Yin0,Zin0,p0,q0)
%全局曲面插值，指定插值点，反求节点矢量及控制点
%   input:
%       n:u方向插值点个数,n+1
%       m:v方向插值点个数,m+1
%       Xin,Yin.Zin:(n+1)*(m+1)的矩阵，对应(n+1)*(m+1)个插值点的x,y,z坐标
%       p:u方向曲线阶数
%       q:v方向曲线阶数
%   output:
%       U:u方向节点矢量
%       V:v方向节点矢量
%       Xctrl,Yctrl,Zctrl:(n+1)*(m+1)的矩阵，对应(n+1)*(m+1)个控制点的x,y,z坐标
%       Nu:u方向的系数矩阵
%       Nv:v方向的系数矩阵

    [uk,vl,U0,V0] = SurfMeshParams( p0,q0,nu,nv,Xin0,Yin0,Zin0 );

%% 建立系数矩阵Nu
    Nu = zeros(nu+1,nu+1);
    for i = 0:nu
        span = FindSpan(nu,p0,uk(i+1),U0);
        Nu(i+1,span-p0:span) = BasisFuns(span,uk(i+1),p0,U0);   %
    end
    
%% 求解R
    Rx = zeros(nu+1,nv+1);
    Ry = zeros(nu+1,nv+1);
    Rz = zeros(nu+1,nv+1);
    for j = 0:nv
        % 利用高斯消元求解控制点
        Rx(:,j+1) = gauss(Nu,Xin0(:,j+1));
        Ry(:,j+1) = gauss(Nu,Yin0(:,j+1));
        Rz(:,j+1) = gauss(Nu,Zin0(:,j+1));
    end
    
%% 建立系数矩阵Nv
    Nv = zeros(nv+1,nv+1);
    for i = 0:nv
        span = FindSpan(nv,q0,vl(i+1),V0);
        Nv(i+1,span-q0:span) = BasisFuns(span,vl(i+1),q0,V0);
    end
    
%% 求解P
   Xctrl0 = zeros(nu+1,nv+1);
   Yctrl0 = zeros(nu+1,nv+1);
   Zctrl0 = zeros(nu+1,nv+1);
    for j = 0:nu
        Xctrl0(j+1,:) = (gauss(Nv,Rx(j+1,:)'))';
        Yctrl0(j+1,:) = (gauss(Nv,Ry(j+1,:)'))';
        Zctrl0(j+1,:) = (gauss(Nv,Rz(j+1,:)'))';
    end   
end

