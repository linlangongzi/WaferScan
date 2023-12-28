function [ Xout,Yout,Zout,Nx,Ny,Nz ] = ExtrapolateCurve( Xin,Yin,Zin,deepth )
%已知两条曲线的插值点，外推下一条曲线的插值点及法向量
%程序流程：
%   1、外推下一条曲线的插值点
%   2、将三条曲线的插值点进行曲面拟合，反求控制点
%   4、对第三第曲线进行等弧长细分，求出细分点及对应的法向量
%   input:
%       Xin,Yin,Zin:均为nu*2的矩阵，即每条线的点数为nu，线数为2
%       deepth：外推的深度
%   output:
%       Xout,Yout,Zout:均nu*1的矩阵，为外推的下一条曲线的插值点
%       Nx,Ny,Nz:每个插值点对应法向量
[nu_temp,~] = size(Xin);%nu_temp 为u方向的节点个数
Xout = zeros(nu_temp,1);
Yout = zeros(nu_temp,1);
Zout = zeros(nu_temp,1);
Nx = zeros(nu_temp,1);
Ny = zeros(nu_temp,1);
Nz = zeros(nu_temp,1);
for index = 1:nu_temp
    P1 = [Xin(index,1),Yin(index,1),Zin(index,1)];
    P2 = [Xin(index,2),Yin(index,2),Zin(index,2)];
    Ptemp = Extrapolate(P1,P2,deepth);
    Xout(index) = Ptemp(1);
    Yout(index) = Ptemp(2);
    Zout(index) = Ptemp(3);
%     Nx(index) = 1;
%     Ny(index) = 0;
%     Nz(index) = 0;
end

%对第三条线进行细分重排
% %对这个小曲面进行曲面插值拟合
 p_temp = 1;
 q_temp = 1;
 x_temp = [Xin,Xout];
 y_temp = [Yin,Yout];
 z_temp = [Zin,Zout];
[U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( nu_temp-1,3-1,x_temp,y_temp,z_temp,p_temp,q_temp) ;

%%
%此部分查看拟合的曲面的正确性，可忽略
% ku = 20; %设置考查的点的密度
% kv = 20;
% u = linspace(0,1,ku);
% v = linspace(0,1,kv);
% Cxt = zeros(ku,kv);%点位坐标
% Cyt = zeros(ku,kv);
% Czt = zeros(ku,kv);
% for i = 1:ku
%     for j = 1:kv
%  %       Cxt(i,j) = SurfacePoint( nu_temp-1,p_temp,U,2,q_temp,V,Xctrl,u(i),v(j));
%  %       Cyt(i,j) = SurfacePoint( nu_temp-1,p_temp,U,2,q_temp,V,Yctrl,u(i),v(j));
%  %       Czt(i,j) = SurfacePoint( nu_temp-1,p_temp,U,2,q_temp,V,Zctrl,u(i),v(j));
%        
%         [pt,nt] = NormVector(nu_temp-1,p_temp,U,2,q_temp,V,Xctrl,Yctrl,Zctrl,u(i),v(j),1);
%         Cxt(i,j) = pt(1);
%         Cyt(i,j) = pt(2);
%         Czt(i,j) = pt(3);
%         
%         ptn = pt+10*nt;
%         a = ptn(1);
%         b = ptn(2);
%         c = ptn(3);
%         plot3([pt(1),a],[pt(2),b],[pt(3),c],'color','g');%画出（u,v)对应的点及法向量
%     end
% end
% % subplot(2,2,1);
% plot_surface(Cxt,Cyt,Czt,'b');%自定义画面算法
%%
%对v = 1这条曲线进行等弧长细分，并求对应点的法向量
L = 0;
ddu = 0.0002;
ddv = 0;
utemp = 0;
while(utemp<1)%计算外推的曲线的总长度
    x_temp = SurfaceDerivsAlg1( nu_temp-1,p_temp,U,2,q_temp,V,Xctrl,utemp,1,1 );
    y_temp = SurfaceDerivsAlg1( nu_temp-1,p_temp,U,2,q_temp,V,Yctrl,utemp,1,1 );
    z_temp = SurfaceDerivsAlg1( nu_temp-1,p_temp,U,2,q_temp,V,Zctrl,utemp,1,1 );
    dLsPower = (abs(x_temp(2,1)*ddu) + abs(x_temp(1,2)*ddv))^2 + ...
               (abs(y_temp(2,1)*ddu) + abs(y_temp(1,2)*ddv))^2 + ...
               (abs(z_temp(2,1)*ddu) + abs(z_temp(1,2)*ddv))^2;
    L = L + sqrt(dLsPower);
    utemp = utemp + ddu;
end
dL = L/(nu_temp-1);     
utemp = 0;
L = 0;
nu_index = 1;
while(utemp<1&&nu_index<nu_temp-1)
    while(L<nu_index*dL)
    x_temp = SurfaceDerivsAlg1( nu_temp-1,p_temp,U,2,q_temp,V,Xctrl,utemp,1,1 );
    y_temp = SurfaceDerivsAlg1( nu_temp-1,p_temp,U,2,q_temp,V,Yctrl,utemp,1,1 );
    z_temp = SurfaceDerivsAlg1( nu_temp-1,p_temp,U,2,q_temp,V,Zctrl,utemp,1,1 ); 
    dLsPower = (abs(x_temp(2,1)*ddu) + abs(x_temp(1,2)*ddv))^2 + ...
               (abs(y_temp(2,1)*ddu) + abs(y_temp(1,2)*ddv))^2 + ...
               (abs(z_temp(2,1)*ddu) + abs(z_temp(1,2)*ddv))^2;
    L = L + sqrt(dLsPower); 
    utemp = utemp + ddu;
    end
    [ Ptemp,Ntemp ] = NormVector( nu_temp-1,p_temp,U,2,q_temp,V,Xctrl,Yctrl,Zctrl,utemp,1,1);
    nu_index = nu_index + 1;
    Xout(nu_index) = Ptemp(1);
    Yout(nu_index) = Ptemp(2);
    Zout(nu_index) = Ptemp(3);
    Nx(nu_index) = Ntemp(1);
    Ny(nu_index) = Ntemp(2);
    Nz(nu_index) = Ntemp(3); 
end
    utemp = 0;
    [ Ptemp,Ntemp ] = NormVector( nu_temp-1,p_temp,U,2,q_temp,V,Xctrl,Yctrl,Zctrl,utemp,1,1);
    Xout(1) = Ptemp(1);
    Yout(1) = Ptemp(2);
    Zout(1) = Ptemp(3);
    Nx(1) = Ntemp(1);
    Ny(1) = Ntemp(2);
    Nz(1) = Ntemp(3); 
    utemp = 1;
    [ Ptemp,Ntemp ] = NormVector( nu_temp-1,p_temp,U,2,q_temp,V,Xctrl,Yctrl,Zctrl,utemp,1,1);
    Xout(nu_temp) = Ptemp(1);
    Yout(nu_temp) = Ptemp(2);
    Zout(nu_temp) = Ptemp(3);
    Nx(nu_temp) = Ntemp(1);
    Ny(nu_temp) = Ntemp(2);
    Nz(nu_temp) = Ntemp(3); 
end

