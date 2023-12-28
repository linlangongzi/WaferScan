%% ·���滮
clear
clc
tic
%read_stl; %���STL�ļ�������
load('Sxyz.mat'); %����ReSample.m
disp('��STL�ļ��в���ʱ��');
toc
close
%%
[n,m] = size(Sx);
x = zeros(n,m);
y = zeros(n,m);
z = zeros(n,m);
for i = 1:n
    for j = 1:m
        x(i,j) = Sx(n + 1 - i,j);
        y(i,j) = Sy(n + 1 - i,j);
        z(i,j) = Sz(n + 1 - i,j);
    end
end
n = n - 1;
m = m - 1;
p = 3;
q = 2;
%% ������Ƶ㲿�� 
%Xctrl���з���Ϊ��������u����
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterpC1( n,m,x,y,z,p,q);
n = n + 2;
m = m + 2;
%% �鿴����
% ku = 50; %���ÿ���ĵ���ܶ�
% kv = 50;
% u = linspace(0,1,ku);
% v = linspace(0,1,kv);
% CX = zeros(ku,kv);
% CY = zeros(ku,kv);
% CZ = zeros(ku,kv);
% for i = 1:ku
%     for j = 1:kv
%         CX(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));%ʵ����CX��������ͬ���������ÿ�ж�����һ��
%         CY(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));%ʵ����CY��΢����ͬ���������ÿ�ж�����һ��
%         CZ(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));%�������(u,v)��Ӧ��z����ֵ
%     end
% end
% plot_surface(CX,CY,CZ,'k');
% hold on;
% axis equal

%% ��·���滮
disp('start path planning')
 RoughScanInterval = 15;%ɨ���ߴַּ��
 ScanInterval = 1;%ɨ����ϸ�ּ����ֱ���·�����͹��
 JogInterval = 0.2;%���������ֱ���·�����͹��
 thetaN = 23;
 thetaL = 180;
tic
 [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
      SurfaceRoutePlanning2( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval,JogInterval,thetaN,thetaL ); 
%     SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,thetaN,thetaL );
%     SurfaceRoutePlanning3( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,JogInterval,thetaN,thetaL,10,100);

disp('finish path planning��');
toc
axis equal;
xlabel('x axis');
ylabel('y axis');
zlabel('z axis');

%% ���������뱣��
clear JogInterval m n p q RoughScanInterval ScanInterval 
clear Sx Sy Sz U V Xctrl Yctrl Zctrl x y z i j xOrder yOrder zOrder
save('path.mat');
