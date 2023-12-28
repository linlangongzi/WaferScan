%用以测试指定端点导矢的全局曲线插值
clear
clc
format long;
n = 5;%数据点个数
x = linspace(0,4,n+1)';
y = sin(x*pi/2);      
% x = [0,30,100,30,0]';
% y = [0,50,50,50,100]';
% x = linspace(0,100,n+1)';
% y = x.^2;
Q = [x,y];  %数据点：Q为(n+1)*2的矩阵,二维点(x,y)
p = 3;  %B样条基函数阶次

[ p1,m,U,P] = GlobalCurveInterp( n,Q,p);
k=1001;%插值点数，用来显示
u = linspace(0,1,k);
Cx = zeros(k,1);
Cy = zeros(k,1);
for i = 1:k
    Cx(i) = CurvePoint( n,p1,U,P(:,1),u(i));%利用B样条曲线定义，由控制点及阶次来建立出该曲线x坐标
    Cy(i) = CurvePoint( n,p1,U,P(:,2),u(i));%利用B样条曲线定义，由控制点及阶次来建立出该曲线y坐标
end
%%

%%
%计算曲线的插值位置、曲率、曲率半径信息
d = 3;
Cx = zeros(1,1);
Cy = zeros(1,1);
CK1x = zeros(1,1);
CK1y = zeros(1,1);
CK2x = zeros(1,1);
CK2y = zeros(1,1);
CK3x = zeros(1,1);
CK3y = zeros(1,1);
dydx1 = zeros(1,1);
dydx2 = zeros(1,1);
for i = 1:k
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i),d);
    Cx(i) = tempx(1);
    CK1x(i) = tempx(2);%即dx/du
    CK2x(i) = tempx(3);
    CK3x(i) = tempx(4);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i),d);
    Cy(i) = tempy(1);
    CK1y(i) = tempy(2);%即dy/du
    CK2y(i) = tempy(3);
    CK3y(i) = tempy(4);
    dydx1(i) =  tempy(2)/tempx(2); %dy/dx
    dydx2(i) = ( tempy(3)*tempx(2) - tempx(3)*tempy(2) )/(tempx(2))^3;%dy^2/dx^2
end
ax(1) = subplot(3,1,1);
plot(x,y,'r');
hold on;
plot(x,y,'*');
plot(Cx,Cy);

ax(2) = subplot(3,1,2);
plot(Cx,dydx1);

ax(3) = subplot(3,1,3);
plot(Cx(10:k-20),dydx2(10:k-20));
linkaxes(ax,'x');
pi2 = max(dydx2(10:k-20))
pi^2/4
% du = u(2)-u(1);
% CK1xtest = zeros(1,k-1);
% for i=1:k-1
%     CK1xtest(i) = (Cx(i+1)-Cx(i))/du;
% end
% CK1ytest = zeros(1,k-1);
% for i=1:k-1
%     CK1ytest(i) = (Cy(i+1)-Cy(i))/du;
% end
% CK2xtest = zeros(1,k-1);
% for i=1:k-1
%     CK2xtest(i) = (CK1x(i+1)-CK1x(i))/du;
% end
% CK2ytest = zeros(1,k-1);
% for i=1:k-1
%     CK2ytest(i) = (CK1y(i+1)-CK1y(i))/du;
% end
% CK3xtest = zeros(1,k-1);
% for i=1:k-1
%     CK3xtest(i) = (CK2x(i+1)-CK2x(i))/du;
% end
% CK3ytest = zeros(1,k-1);
% for i=1:k-1
%     CK3ytest(i) = (CK2y(i+1)-CK2y(i))/du;
% end
% close
% subplot(3,1,1)
% plot(1:k,CK1x)
% hold on
% plot(1:k-1,CK1xtest,'g')
% 
% 
% subplot(3,1,2)
% plot(1:k,CK2x)
% hold on
% plot(1:k-1,CK2xtest,'g')
% 
% 
% subplot(3,1,3)
% plot(1:k,CK3x)
% hold on
% plot(1:k-1,CK3xtest,'g')
% % 
