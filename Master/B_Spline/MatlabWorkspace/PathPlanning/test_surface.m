%%
clear
clc
%读取采样插值点部分
% [tx,ty,tz,ta,tb,tL]=textread('shujujiayi3.txt','%f %f %f %f %f %f');
% row = 9; 
% column = 8;
% Q = zeros(row,column*3);
% for column_temp = 0:column-1
%     for row_temp = 0:row-1
%         Q(row_temp+1,column_temp*3+1) = tx(row_temp+1+column_temp*row);
%         Q(row_temp+1,column_temp*3+2) = ty(row_temp+1+column_temp*row);
%         Q(row_temp+1,column_temp*3+3) = tz(row_temp+1+column_temp*row);
%         %plot3(Q(row_temp+1,column_temp*3+1),Q(row_temp+1,column_temp*3+2),Q(row_temp+1,column_temp*3+3),'*')
%         %hold on;
%     end
% end
% Q1 = zeros(row-1,column*3);
% Q1 = Q(1:row-1,:);
%%
format long;
%自定义曲面
%设置配置参数
n = 20;
m = 20;
p = 3;
q = 3;
%构造插值点
%Q1 = zeros(n+1,3*(m+1));
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
%叶片： 2*(1/(10+y(j+1))-1/35)*x(i+1);
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
%反求控制点部分,Q1 = zeros(n+1,3*(m+1));
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,Xin,Yin,Zin,p,q);
 
%%
%对XYZ分别做曲面生成
ku = 40; %设置考查的点的密度
kv = 20;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
Cx = zeros(ku,kv);%点位坐标
Cy = zeros(ku,kv);
Cz = zeros(ku,kv);
Nx = zeros(ku,kv);%法矢
Ny = zeros(ku,kv);
Nz = zeros(ku,kv);
Pt = zeros(ku,3*kv);%存储曲面上的计算点
N = zeros(ku,3*kv);%存储曲面上的计算点对应的法向量
for i = 1:ku
    for j = 1:kv
        [Pt(ku,3*kv-2:3*kv),N(ku,3*kv-2:3*kv)] = NormVector(n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,u(i),v(j),1);
        Cx(i,j) = Pt(ku,3*kv-2);
        Cy(i,j) = Pt(ku,3*kv-1);
        Cz(i,j) = Pt(ku,3*kv);
        Nx(i,j) = N(ku,3*kv-2);
        Ny(i,j) = N(ku,3*kv-1);
        Nz(i,j) = N(ku,3*kv);
    end
end

% plot_surface(Cx,Cy,Cz,'r');%自定义画面算法
% axis equal;
% hold on;

%%
%以下部分为等进给算法验证
 RoughScanInterval = 20;%扫描线粗分间隔
 ScanInterval = 15;%扫描线细分间隔，直接下发电子凸轮
 JogInterval = 10;%进给间隔，直接下发电子凸轮

 [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
    SurfaceRoutePlanning( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval );
axis equal;
xlabel('扫描方向');
ylabel('进给方向');
zlabel('高度');
%保存数据
csvwrite('Xout.csv',Xout);
csvwrite('Yout.csv',Yout);
csvwrite('Zout.csv',Zout);
csvwrite('Nxout.csv',Nxout);
csvwrite('Nyout.csv',Nyout);
csvwrite('Nzout.csv',Nzout);
csvwrite('ScanningNum.csv',ScanningNum);

%% 五轴系统逆解
% x = zeros(ku,kv);
% y = zeros(ku,kv);
% z = zeros(ku,kv);
% b = zeros(ku,kv);
% a = zeros(ku,kv);
% for i = 1:ku
%     for j = 1:kv
%         [x(i,j),y(i,j),z(i,j),b(i,j),a(i,j)] = InverseSolver(Cx(i,j),Cy(i,j),200+Cz(i,j),-Nx(i,j),-Ny(i,j),Nz(i,j));
%     end
% end

%% 将逆解做路径规划，即排列点序列
% t = 5*ku;%扫查总耗时,即每条线用时5秒
% time = linspace(0,t,2*kv*ku);
% xx = zeros(1,2*kv*ku);
% yy = zeros(1,2*kv*ku);
% zz = zeros(1,2*kv*ku);
% bb = zeros(1,2*kv*ku);
% aa = zeros(1,2*kv*ku);
% for j = 0:ku-1
%     for i = 1:kv
%         xx(i+2*kv*j) = x(j+1,i);
%         yy(i+2*kv*j) = y(j+1,i);
%         zz(i+2*kv*j) = -z(j+1,i);
%         bb(i+2*kv*j) = b(j+1,i);
%         aa(i+2*kv*j) = a(j+1,i);
%     end
%     for i = 1:kv
%         xx(i+2*kv*j+kv) = x(j+1,kv-i+1);
%         yy(i+2*kv*j+kv) = y(j+1,kv-i+1);
%         zz(i+2*kv*j+kv) = -z(j+1,kv-i+1);
%         bb(i+2*kv*j+kv) = b(j+1,kv-i+1);
%         aa(i+2*kv*j+kv) = a(j+1,kv-i+1);
%     end
% end
