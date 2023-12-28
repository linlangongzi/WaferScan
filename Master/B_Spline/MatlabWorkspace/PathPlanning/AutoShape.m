function [ X,Y,Z ] = AutoShape( x_aera,y_aera,z_aera,nu,nv )
%给定仿形区域，实现自动仿形
%   input:
%       x_aera,y_aera:给出了仿形区域，[x_begin,x_end],[y_begin,y_end]
%       z_aera:z的大概上下限，[z_begin,z_end]
%              其中z_begin认为是起始线的高度
%       nu,nv:u和v方向的点的个数，u为扫描方向，v为进给方向
%   output:
%       X,Y,Z:仿形后曲面的插值点
X = zeros(nu+1,nv+1);
Y = zeros(nu+1,nv+1);
Z = zeros(nu+1,nv+1);
%%
%构造一个工件曲面
k = 3;%衰减系数
x_max = x_aera(2);
y_max = y_aera(2);
x = linspace(x_aera(1),x_max,nu+1);
y = linspace(y_aera(1),y_max,nv+1);
z = zeros(nu+1,nv+1);
for i = 0:nu   %构造插值点
    for j = 0:nv
        z(i+1,j+1) = z_aera(1) + sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
%%
%外推第一条线
Xtest = linspace(x_aera(1),x_aera(2),nu+1);
Ytest = linspace(y_aera(1),y_aera(1),nu+1);%即第一条线沿x扫描，y不变
Ztest = linspace(z_aera(1),z_aera(1),nu+1);%默认第一条线是水平线
%多次测量开始

%多次测量后得到结果如下，虚拟的真实值
X(:,1) = x;
Y(:,1) = linspace(y_aera(1),y_aera(1),nu+1);
Z(:,1) = z(:,1);
%%
%外推第二条线
dy = y_aera(2)-y_aera(1);
Xtest = linspace(x_aera(1),x_aera(2),nu+1);
Ytest = linspace(y_aera(1)+dy/nv,y_aera(1)+dy/nv,nu+1);%即第二条线沿x扫描，y不变
Ztest = linspace(z_aera(1),z_aera(1),nu+1);%默认第二条线是水平线
%多次测量开始

%多次测量 后得到结果如下，虚拟的真实值
X(:,1) = x;
Y(:,1) = linspace(y_aera(1)+dy/nv,y_aera(1)+dy/nv,nu+1);
Z(:,1) = z(:,2);
%%
%外推3～nv条线
for index = 3:nv+1
[ Xtest,Ytest,Ztest,Nx_test,Ny_test,Nz_test ] = ...
                        ExtrapolateCurve( X(:,index-2:index-1),...
                                          Y(:,index-2:index-1),...
                                          Z(:,index-2:index-1) );
end
end

