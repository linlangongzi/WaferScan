%%
clear
format long;
%自定义曲面
%设置配置参数
n = 10;
m = 10;
p = 3;
q = 3;
%构造插值点
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
%叶片： 2*(1/(10+y(j+1))-1/35)*x(i+1);
%马鞍形：(1-x(i+1))^2-(0.5-y(j+1))^2
%圆坑：(1-x(i+1))^2+(0.5-y(j+1))^2
for i = 0:n  %对插值点进行处理，集成到矩阵中，用来传递
    for j = 0:m
        Xin(i+1,j+1) = x(i+1);
        Yin(i+1,j+1) = y(j+1);
        Zin(i+1,j+1) =  sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
% plot3(Xin,Yin,Zin,'*','Color','b')%插值点
% hold on
%%
%反求控制点部分
%[ U,V,P,Nu,Nv ] = GlobalSurfInterp( n,m,Q1,p,q);
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,Xin,Yin,Zin,p,q);
%%
%对XYZ分别做曲面生成
ku = 20; %设置考查的点的密度
kv = 20;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
Cx = zeros(ku,kv);%点位坐标
Cy = zeros(ku,kv);
Cz = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        Cx(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));
        Cy(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));
        Cz(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));
    end
end
% subplot(2,2,1);
plot_surface(Cx,Cy,Cz,'k');%自定义画面算法
hold on
axis equal
plot3(Cx(:,1),Cy(:,1),Cz(:,1),'*');
plot3(Cx(:,3),Cy(:,3),Cz(:,3),'*');
plot3(Cx(:,5),Cy(:,5),Cz(:,5),'*');
plot3(Cx(:,7),Cy(:,7),Cz(:,7),'*');
Xin = [Cx(:,5),Cx(:,7)];
Yin = [Cy(:,5),Cy(:,7)];
Zin = [Cz(:,5),Cz(:,7)];
[ Xout,Yout,Zout,Nx,Ny,Nz ] = ExtrapolateCurve( Xin,Yin,Zin,20 );
[a,~] = size(Xout);
X1 = Xout + Nx*10;
Y1 = Yout + Ny*10;
Z1 = Zout + Nz*10;
for index = 1:a
     plot3([Xout(index),X1(index)],[Yout(index),Y1(index)],[Zout(index),Z1(index)],'r');
end
plot_surface(Xout,Yout,Zout,'o');
xlabel('x(mm)');
ylabel('y(mm)');
zlabel('z(mm)');
