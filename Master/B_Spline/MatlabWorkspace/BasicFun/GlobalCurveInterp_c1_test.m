%用以测试指定端点导矢的全局曲线插值
clear
clc
format long;
n = 4;%数据点个数
x = linspace(0,2,n+1)';
y = cos(x*pi);      
% x = [0,30,100,30,0]';
% y = [0,50,50,50,100]';
Q = [x,y];  %数据点：Q为(n+1)*2的矩阵,二维点(x,y)
plot(Q(:,1),Q(:,2),'o','Color','r') %画出数据点
hold on;
p = 3;  %B样条基函数阶次

D0 = 1*(Q(2,:)-Q(1,:));%端点导矢设置为第一个弦方向
Dn = 1*(Q(n+1,:)-Q(n,:));%终点导矢设置为最后一个弦方向
[ U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
n = n + 2;%使用前后导矢后相当于前后增加两个节点
k=50;%插值点数，用来显示
u = linspace(0,1,k);
C = zeros(k,2);
for i = 1:k
    C(i,1) = CurvePoint( n,p1,U,P(:,1),u(i));%利用B样条曲线定义，由控制点及阶次来建立出该曲线x坐标
    C(i,2) = CurvePoint( n,p1,U,P(:,2),u(i));%利用B样条曲线定义，由控制点及阶次来建立出该曲线y坐标
end
% subplot(2,1,1)
plot(C(:,1),C(:,2),'b');
hold on;
plot(C(:,1),C(:,2),'*');

%%

%%
%计算曲线的插值位置、曲率、曲率半径信息
d = 2;
Cx = zeros(1,1);
Cy = zeros(1,1);
CK1x = zeros(1,1);
CK1y = zeros(1,1);
CK2x = zeros(1,1);
CK2y = zeros(1,1);
for i = 1:k
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i),d);
    Cx(i) = tempx(1);
    CK1x(i) = tempx(2);%即dx/du
    CK2x(i) = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i),d);
    Cy(i) = tempy(1);
    CK1y(i) = tempy(2);%即dy/du
    CK2y(i) = tempy(3);
end
D0
D0test = [CK1x(1),CK1y(1)] %起点一阶导矢
Dn
Dntest = [CK1x(k),CK1y(k)] %终点一阶导矢

A0test = [CK2x(1),CK2y(1)] %起点的二阶导矢
Antest = [CK2x(k),CK2y(k)]

