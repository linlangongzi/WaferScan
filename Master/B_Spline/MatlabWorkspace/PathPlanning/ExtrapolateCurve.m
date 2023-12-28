function [ Xout,Yout,Zout,Nx,Ny,Nz ] = ExtrapolateCurve( Xin,Yin,Zin,deepth )
%��֪�������ߵĲ�ֵ�㣬������һ�����ߵĲ�ֵ�㼰������
%�������̣�
%   1��������һ�����ߵĲ�ֵ��
%   2�����������ߵĲ�ֵ�����������ϣ�������Ƶ�
%   4���Ե��������߽��еȻ���ϸ�֣����ϸ�ֵ㼰��Ӧ�ķ�����
%   input:
%       Xin,Yin,Zin:��Ϊnu*2�ľ��󣬼�ÿ���ߵĵ���Ϊnu������Ϊ2
%       deepth�����Ƶ����
%   output:
%       Xout,Yout,Zout:��nu*1�ľ���Ϊ���Ƶ���һ�����ߵĲ�ֵ��
%       Nx,Ny,Nz:ÿ����ֵ���Ӧ������
[nu_temp,~] = size(Xin);%nu_temp Ϊu����Ľڵ����
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

%�Ե������߽���ϸ������
% %�����С������������ֵ���
 p_temp = 1;
 q_temp = 1;
 x_temp = [Xin,Xout];
 y_temp = [Yin,Yout];
 z_temp = [Zin,Zout];
[U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( nu_temp-1,3-1,x_temp,y_temp,z_temp,p_temp,q_temp) ;

%%
%�˲��ֲ鿴��ϵ��������ȷ�ԣ��ɺ���
% ku = 20; %���ÿ���ĵ���ܶ�
% kv = 20;
% u = linspace(0,1,ku);
% v = linspace(0,1,kv);
% Cxt = zeros(ku,kv);%��λ����
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
%         plot3([pt(1),a],[pt(2),b],[pt(3),c],'color','g');%������u,v)��Ӧ�ĵ㼰������
%     end
% end
% % subplot(2,2,1);
% plot_surface(Cxt,Cyt,Czt,'b');%�Զ��廭���㷨
%%
%��v = 1�������߽��еȻ���ϸ�֣������Ӧ��ķ�����
L = 0;
ddu = 0.0002;
ddv = 0;
utemp = 0;
while(utemp<1)%�������Ƶ����ߵ��ܳ���
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

