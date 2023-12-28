%% 用于曲面自动重建中的论文出图

%该程序实现给定数据的全局曲面拟合，采用的是B样条基函数
clear
clc
format long
%% 构造曲面插值点
%设置配置参数
n = 6;
m = 5;
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
        M(i+1,j+1) = sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
%补充几个常用形状
%马鞍形：(1-x(i+1))^2-(0.5-y(j+1))^2
%圆坑：(1-x(i+1))^2+(0.5-y(j+1))^2
for i = 0:n  %对插值点进行处理，集成到矩阵中，用来传递
    for j = 0:m
        Xin(i+1,j+1) = x(i+1);
        Yin(i+1,j+1) = y(j+1);
        Zin(i+1,j+1) =  sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
plot_point(Xin,Yin,Zin,'r');
hold on;
%% 反求控制点P
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,Xin,Yin,Zin,p,q);
%% 对XYZ分别做曲面生成
ku = 49; %设置考查的点的密度
kv = 51;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
CX = zeros(ku,kv);
CY = zeros(ku,kv);
CZ = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        CX(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));%实际上CX列向量相同，因此无须每列都计算一遍
        CY(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));%实际上CY行向量相同，因此无须每行都计算一遍
        CZ(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));%坐标参数(u,v)对应的z曲面值
    end
end
plot_surface(CX,CY,CZ,'k');
hold on;


%% 手动采集三条扫描线，进行粗曲面重建
n1 = 6;
m1 = 2;
InterX = zeros(n1 + 1, m1 + 1);
InterY = zeros(n1 + 1, m1 + 1);
InterZ = zeros(n1 + 1, m1 + 1);
for i = 1 : n1 + 1
    for j = 1 : m1 + 1
        InterX(i, j) = SurfacePoint(n,p, U, m, q, V, Xctrl, u((i - 1) * (ku - 1) / n1 + 1), v((j - 1) * 5 + 1));
        InterY(i, j) = SurfacePoint(n,p, U, m, q, V, Yctrl, u((i - 1) * (ku - 1) / n1 + 1), v((j - 1) * 5 + 1));
        InterZ(i, j) = SurfacePoint(n,p, U, m, q, V, Zctrl, u((i - 1) * (ku - 1) / n1 + 1), v((j - 1) * 5 + 1));
    end
end
plot_point(InterX, InterY, InterZ, 'r');

%% 对采集点进行曲面拟合
p = 3;
q = 2;
[ U1,V1,Xctrl1,Yctrl1,Zctrl1] = GlobalSurfInterp( n1, m1,InterX, InterY, InterZ, p, q);
CX1 = zeros(ku,kv);
CY1 = zeros(ku,kv);
CZ1 = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        CX1(i,j) = SurfacePoint(n1,p,U1,m1,q,V1,Xctrl1,u(i),v(j));%实际上CX列向量相同，因此无须每列都计算一遍
        CY1(i,j) = SurfacePoint(n1,p,U1,m1,q,V1,Yctrl1,u(i),v(j));%实际上CY行向量相同，因此无须每行都计算一遍
        CZ1(i,j) = SurfacePoint(n1,p,U1,m1,q,V1,Zctrl1,u(i),v(j));%坐标参数(u,v)对应的z曲面值
    end
end
plot_surface(CX1,CY1,CZ1,'g');

axis equal;
xlabel('x(mm)，参数u方向，扫描方向')
ylabel('y(mm)，参数v方向，进给方向')
zlabel('z(mm)')

%% 对构造的曲面进行外推新扫描线
newX = zeros(ku, 1);
newY = zeros(ku, 1);
newZ = zeros(ku, 1);
d = 1;
step = 20;
for i = 1 : ku
    %计算沿v方向的偏导数，进行步进
   dxdv = SurfaceDerivsAlg1(n1,p,U1,m1,q,V1,Xctrl1,u(i),1,d); %dv = (1, 2)
   dydv = SurfaceDerivsAlg1(n1,p,U1,m1,q,V1,Yctrl1,u(i),1,d); %dv = (1, 2)
   dzdv = SurfaceDerivsAlg1(n1,p,U1,m1,q,V1,Zctrl1,u(i),1,d); %dv = (1, 2)
   dir = [dxdv(1,2), dydv(1,2), dzdv(1,2)];
   dir = dir / norm(dir);   %归一化
   newX(i) = CX1(i, kv) + dir(1) * step;
   newY(i) = CY1(i, kv) + dir(2) * step;
   newZ(i) = CZ1(i, kv) + dir(3) * step;
end
% plot_point(CX1(:, kv), CY1(:,kv), CZ1(:, kv), 'm');
% plot_point(newX, newY, newZ, 'm');

for i = 1 : n1+1
    plot3(newX((i - 1) * (ku - 1) / n1 + 1), newY((i - 1) * (ku - 1) / n1 + 1), newZ((i - 1) * (ku - 1) / n1 + 1), '*', 'Color', 'm');
end
