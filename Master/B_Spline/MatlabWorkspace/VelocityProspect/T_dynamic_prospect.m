%%
%速度前瞻，采用T形匀加减速规划
%   由当前速度做最大加速度减速到零，得到一个虚拟轨迹速度，与速度限制比较
%如果未超速，则加速度设为最大进行插补运动，反之按最大加速度减速  
%% 设置运动轨迹
clear
clc
format long;
n = 5;%数据点个数
x = 4*[0,40,80,120,160,200]';
y = 10*[0,-2,5,-10,15,0]';
% x = [0,50,100,50,0]';
% y = [0,50,50,50,100]';
Q = [x,y];  %数据点：Q为(n+1)*2的矩阵,二维点(x,y)

p = 3;  %B样条基函数阶次
D0 = 1*(Q(2,:)-Q(1,:)); %端点导矢设置为第一个弦方向
Dn = 1*(Q(n+1,:)-Q(n,:)); %终点导矢设置为最后一个弦方向
[ p1,m,U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
n = n + 2;%使用前后导矢后相当于前后增加两个节点

%% 运动参数设置
attenuation = 0.5;
Vm = 1500*attenuation;%m/s
Am = 4000;%m/s^2
T = 1e-3;%s
delta = Am*T^2/8;%弦高误差，由最大加速度决定，只控制向心加速度，不控制直线加速度
roum = (Vm*T/2)^2/2/delta; %最大速度来决定最大曲率半径

%% 计算曲线的插值位置、曲率、曲率半径信息
numofP=1000;%插值点数，用来显示
u = linspace(0,1,numofP);
du = u(2)-u(1);
Cx = zeros(numofP,1);
Cy = zeros(numofP,1);
curvity = zeros(numofP,1);
V = zeros(numofP,1);%记录速度,V-u表
L = zeros(numofP,1);%每一点到起点的总弧长，L-u表
d = 2;
for i = 1:numofP
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i),d);
    Cx(i) = tempx(1);
    CK1x = tempx(2);%即dx/du
    CK2x = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i),d);
    Cy(i) = tempy(1);
    CK1y = tempy(2);%即dy/du
    CK2y = tempy(3);
    curvity(i) = abs(CK1x*CK2y - CK1y*CK2x) / (sqrt(CK1x^2 + CK1y^2))^3;
    Rou = 1/curvity(i);
    if Rou>roum
       Rou=roum;
    end
    V(i) = 2*sqrt(Rou^2 - (Rou-delta)^2)/T;
     if i<numofP
        L(i+1) = L(i) + sqrt((CK1x*du)^2 + (CK1y*du)^2);
     end
end
V(numofP,1) = 0;

%% 插补过程中参数
Vs = 0;     %插补的真实速度
Vs1 = 0;    %虚拟插补点的速度
Xchabu = zeros(1,1);    %插补点X坐标
Ychabu = zeros(1,1);    %插补点Y坐标
Lchabu = zeros(1,1);    %插补点对应的弧长
Vchabu = zeros(1,1);
Ls = 0;     %插补点对应的弧长
Ls1 = 0;    %虚拟插补点的速度
As = Am;    %插补点的加速度
As1 = Am;   %虚拟插补点的加速度
us = 0;     %插补点对应的参数u
Ve = 0;     %虚拟插补点的速度限制
count_chabu = 1;    %插补点个数

%% 插补过程进行中
disp('开始T形速度规划进行插补');
tic
while us<1 && Vs>=0
    %对当前速度Vs做虚拟减速，查看减速到0的过程中是否会超速
    Vs1 = Vs;
    Ls1 = Ls;
    while(Vs1>0) %一直查询到虚拟速度减为0,或者达到速度限制
        As1 = -Am;
        Vs1 = Vs1 + As1*T;
        Ls1 = Ls1 + Vs1*T;
        Ve = LinearInterpolation(L,V,Ls1);  %找到Ls1对应的速度限制
        if Ve-10 > Vs1
            As = Am;
        else
            As = -Am;
            break;
        end
    end
    Vs = Vs + As*T;
    Ls = Ls + Vs*T;
    us = LinearInterpolation(L,u,Ls);   %找到插补点对应的u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = CurvePoint(n,p1,U,P(:,1),us);
    Ychabu(count_chabu) = CurvePoint(n,p1,U,P(:,2),us);
    Lchabu(count_chabu) = Ls;

end
toc
disp('插补结束')
disp('插补节点个数：')
disp(count_chabu)
%% 加速度考查
%1、代数加速度
Acheck1 = zeros(1,1);
for i = 2:count_chabu-1
    Acheck1(i-1) = ((Vchabu(i)-Vchabu(i-1)))/T;
end
% Acheck1Max = max(Acheck1)%速度模差产生的最大加速度
%2、矢量加速度
Acheck2 = zeros(1,1);
for i = 1:count_chabu-2
    if i ==1
        Norm1 = [Xchabu(1),Ychabu(1)];
        Norm2 = [Xchabu(2)-Xchabu(1),Ychabu(2)-Ychabu(1)];
        theta = VectorsAngle(Norm1,Norm2);  
    else
        Norm1 = [Xchabu(i)-Xchabu(i-1),Ychabu(i)-Ychabu(i-1)];
        Norm2 = [Xchabu(i+1)-Xchabu(i),Ychabu(i+1)-Ychabu(i)];
        theta = VectorsAngle(Norm1,Norm2);
    end
    dV = sqrt((Vchabu(i+1)*sind(theta))^2+(Vchabu(i)-Vchabu(i+1)*cosd(theta))^2);
    Acheck2(i) = dV/T;
end
% Acheck2Max = max(Acheck2)

%% 画图区
subplot(3,1,1)  %轨迹
plot(x,y,'*','Color','r');              %插值点
hold on;
plot(Cx,Cy,'r');                        %轨迹
plot(Xchabu,Ychabu,'.','Color','b');    %插补点
xlabel('轨迹')

subplot(3,1,2)  %速度
plot(L,V,'r');                              %速度限-弧长曲线
hold on;
plot(Lchabu(1:count_chabu-1),Vchabu,'g');   %插补速度-弧长曲线
xlabel('速度及速度限制')

subplot(3,1,3)  %加速度
plot(Lchabu(1:count_chabu-2),Acheck1/Am*200,'g');    %代数加速度
hold on;
plot(Lchabu(1:count_chabu-2),Acheck2/Am*200,'b');    %矢量加速度
plot(linspace(0,max(Lchabu),200),linspace(400,400,200),'r');    %参考线
plot(linspace(0,max(Lchabu),200),linspace(-400,-400,200),'r');  %参考线
xlabel('矢量加速度，代数加速度')