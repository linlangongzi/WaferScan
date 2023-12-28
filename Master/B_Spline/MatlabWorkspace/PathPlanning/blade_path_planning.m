%% 路径规划
clear
clc
tic
%read_stl; %获得STL文件采样点
load('Sxyz.mat'); %来自ReSample.m
disp('从STL文件中采样时间');
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
%% 反求控制点部分 
%Xctrl的列方向为参数曲面u方向
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterpC1( n,m,x,y,z,p,q);
n = n + 2;
m = m + 2;
%% 查看曲面
% ku = 50; %设置考查的点的密度
% kv = 50;
% u = linspace(0,1,ku);
% v = linspace(0,1,kv);
% CX = zeros(ku,kv);
% CY = zeros(ku,kv);
% CZ = zeros(ku,kv);
% for i = 1:ku
%     for j = 1:kv
%         CX(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));%实际上CX列向量相同，因此无须每列都计算一遍
%         CY(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));%实际上CY行微量相同，因此无须每行都计算一遍
%         CZ(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));%坐标参数(u,v)对应的z曲面值
%     end
% end
% plot_surface(CX,CY,CZ,'k');
% hold on;
% axis equal

%% 做路径规划
disp('start path planning')
 RoughScanInterval = 15;%扫描线粗分间隔
 ScanInterval = 1;%扫描线细分间隔，直接下发电子凸轮
 JogInterval = 0.2;%进给间隔，直接下发电子凸轮
 thetaN = 23;
 thetaL = 180;
tic
 [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
      SurfaceRoutePlanning2( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval,JogInterval,thetaN,thetaL ); 
%     SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,thetaN,thetaL );
%     SurfaceRoutePlanning3( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,JogInterval,thetaN,thetaL,10,100);

disp('finish path planning：');
toc
axis equal;
xlabel('x axis');
ylabel('y axis');
zlabel('z axis');

%% 数据清理与保存
clear JogInterval m n p q RoughScanInterval ScanInterval 
clear Sx Sy Sz U V Xctrl Yctrl Zctrl x y z i j xOrder yOrder zOrder
save('path.mat');
