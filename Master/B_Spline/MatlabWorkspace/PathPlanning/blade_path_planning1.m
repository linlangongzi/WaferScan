%% 论文画图

%该程序实现给定数据的全局曲面拟合，采用的是B样条基函数
clear
format long
%设置配置参数
n = 10;
m = 10;
p = 3;
q = 3;
%构造插值点
Q = zeros(n+1,3*(m+1));
Xin = zeros(n+1,m+1);
Yin = zeros(n+1,m+1);
Zin = zeros(n+1,m+1);
k = 3;%衰减系数
x_max = 200;
y_max = 200;
x = linspace(0,x_max,n+1);%105
y = linspace(0,y_max,m+1);%75
M = zeros(n+1,m+1);
for i = 0:n   %构造插值点
    for j = 0:m
        M(i+1,j+1) = sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*50;
    end
end
%补充几个常用形状
%马鞍形：(1-x(i+1))^2-(0.5-y(j+1))^2
%圆坑：(1-x(i+1))^2+(0.5-y(j+1))^2
for i = 0:n  %对插值点进行处理，集成到矩阵中，用来传递
    for j = 0:m
        Xin(i+1,j+1) = x(i+1);
        Yin(i+1,j+1) = y(j+1);
        Zin(i+1,j+1) =  sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*50;
    end
end
%%
%反求控制点P
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,Xin,Yin,Zin,p,q);

%% 画出曲面
ku = 40; %设置考查的点的密度
kv = 40;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
CX = zeros(ku,kv);
CY = zeros(ku,kv);
CZ = zeros(ku,kv);
Pt = zeros(ku,3*kv);
N = zeros(ku,3*kv); 
for i = 1:ku
    for j = 1:kv
        CX(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));%实际上CX列向量相同，因此无须每列都计算一遍
        CY(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));%实际上CY行微量相同，因此无须每行都计算一遍
        CZ(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));%坐标参数(u,v)对应的z曲面值
       % [Pt(i,j),N(i,j)] = NormVector(n,p,U,m,q,V,X,Y,Z,u(i),v(j),1);
    end
end
plot_surface(CX,CY,CZ,'b');




%% 做路径规划
 RoughScanInterval = 10;    %扫描线粗分间隔
 ScanInterval = 5;          %扫描线细分间隔，直接下发电子凸轮
 JogInterval = 20;          %进给间隔，直接下发电子凸轮
 theta = 70;                %横波入射角
tic
 [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
    SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,theta);
disp('路径规划完成时间：');
toc
axis equal;
axis([0,200,0,200,0,80])
xlabel('x(mm)');
ylabel('y(mm)');
zlabel('z(mm)');
