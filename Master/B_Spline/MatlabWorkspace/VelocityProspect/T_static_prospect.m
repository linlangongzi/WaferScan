%%
%恒定前瞻距离
%速度前瞻，采用恒定前瞻距离的方式确定前瞻点速度，做匀加减速规划
%速度前瞻的过程采用插表法，避免了误差累积，同时速度大大提高
%%
clear
clc
format long;
x = 2*[0,40,80,120,160,200]';
y = 10*[0,-2,5,-10,15,0]';
% x = [0,50,100,50,0]';
% y = [0,50,50,50,100]';
% x = [0,0,500,500,0]';
% y = [0,500,500,0,0]';
n = length(x)-1;%数据点个数
Q = [x,y];  %数据点：Q为(n+1)*2的矩阵,二维点(x,y)
hold on;
p = 3;  %B样条基函数阶次
subplot(3,1,1)
plot(x,y,'*','Color','r');
hold on;
%  [ p1,m,U,P] = GlobalCurveInterp( n,Q,p);
% 
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
Lm = Vm^2/(2*Am); %最大前瞻距离28~280mm
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

plot(Cx,Cy,'r');
hold on;
%% 寻找速度波谷点并计算波谷点之间的弧长，找到弧长最小值,重新计算最大速度
VminP = zeros(1,1);%速度波谷点序列
uSingular = zeros(1,1);%速度奇点记录
numofVminP = 2;
VminP(1) = 1;%起点也算一个曲率波峰点
for i = 2:numofP-1
    if (curvity(i)>curvity(i-1) && curvity(i)>curvity(i+1))
        VminP(numofVminP) = i;%记录速度波谷点的位置，而不是对应的速度
        uSingular(1,numofVminP) = u(i);
        numofVminP = numofVminP + 1;%速度奇点个数加1
        VminPx = CurvePoint( n,p1,U,P(:,1),u(i));
        VminPy = CurvePoint( n,p1,U,P(:,2),u(i));
        plot(VminPx,VminPy,'o');%画出速度波谷点的位置
        hold on;
    end
end
uSingular(1,numofVminP) = 1;%u = 1也算一个速度奇点
numofVminP = numofVminP-1;
%计算速度波谷点间的弧长并找到最小值
for i = 1:numofVminP-1
    Ltemp = L(VminP(i+1))-L(VminP(i));%计算速度波谷点之间的弧长
    Lm = min(Lm,Ltemp);%找到最小值
end
%前瞻距离确实 Lm
%重新计算运动参数——最大速度限制
Vm = min(Vm,sqrt(2*Am*Lm+(min(V))^2)); %
roum = (Vm*T/2)^2/2/delta; %最大速度来决定最大曲率半径
%重新对速度曲线、曲率半径曲线进行约束
for i = 1:numofP
    if V(i)>Vm
        V(i) = Vm;
    end
end
Lm = Lm*0.8;
%% 寻找第一个前瞻点
% Ltemp = 0;
% ue = 0;%前瞻点
%us = 0;%插补点
Ve = Vm;%前瞻点速度
Vs = 0;%插补点速度
Vs1 = Vm;%插补点曲率速度限
ue = LinearInterpolation(L,u,Lm);%查表法找第一个前瞻点的位置
Ve1 = LinearInterpolation(L,V,Lm);%查表法找第一个前瞻点的速度
Ve = min(Ve,Ve1);
Acc = 0;
disp('第一个前瞻点找到');
%第一个前瞻点已找到
%% 插补运动准备
tic;
Vchabu = zeros(1,1);%插补点对应的速度序列
Xs = zeros(1,1);%插补点坐标序列
Ys = zeros(1,1);
us =0;
Us = zeros(1,1);%记录插补点的位置参数u
count_chabu = 0;
ndl = 5;
Lchabu0 = 0;
Lchabu = zeros(1,1);
flagIn = 0;%首次跨速度奇点
flag = 0;%记录是否跨越速度奇点
%% 前瞻点未到达曲线终点
while Vs>=0 && us<1                                     %前瞻点尚未到达曲线终点
    for i = 2:length(uSingular)
        if us<uSingular(i) && ue>=uSingular(i)
            flag = 1;
            Ve = LinearInterpolation(u,V,uSingular(i));%Ve取奇点处速度
            if flagIn == 0
                Acc = (Ve^2-Vs^2)/(2*Lm);
                flagIn = 1;
%                 uSingular(i)
            end
            break;
        end  
        flag = 0;
    end
    if flag==0 %表示没有跨越速度奇点
        flagIn = 0;
        Ve = Ve1;
        Acc = (Ve^2-Vs^2)/(2*Lm);
        if Acc>0
            Acc = Am;
        end
    end

    Vs = Vs + Acc*T;%插补速度
    Vs1 = LinearInterpolation(u,V,us);%查表法找速度限制
    Vs = min(Vs,Vs1);
    count_chabu = count_chabu + 1;
    Vchabu(count_chabu) = Vs;       %插补点的插补速度
    dL = Vs*T;%插补长度
    Lchabu0 = Lchabu0 + dL; %插补点到起点的弧长
    Lchabu(1,count_chabu) = Lchabu0;
    %寻找下一个插补点us->x,y
    us = LinearInterpolation(L,u,Lchabu0);  %查表法寻找下一个插补点的us
    if us<0 || us>1
        us = 1;
        %error('错误：us<0 或 us>1')
    end
    Xs(count_chabu) = CurvePoint( n,p1,U,P(:,1),us);       %下一个插补点的位置
    Ys(count_chabu) = CurvePoint( n,p1,U,P(:,2),us);
    Us(count_chabu) = us;
    %找到下一个插补点

    %寻找下一个前瞻点 
    %Le = Le + dL;
    ue = LinearInterpolation(L,u,Lchabu0+Lm);
    if ue>1
        ue = 1;
    end   
    Ve1 = LinearInterpolation(L,V,Lchabu0+Lm);
    %下一个前瞻点速度限制Ve1确定
end  
 %% 前瞻点到达曲张终点
    disp('前瞻点到达曲线终点');
%     Ve = 0;
%     Acc = (Ve^2-Vs^2)/(2*Lm);
%     while us<1 && Vs>0                                    %插补点未到达曲线终点
%         Vs = Vs + Acc*T;
%         count_chabu = count_chabu + 1;
%         Vchabu(count_chabu) = Vs;       %插补点的插补速度
%         dL = Vs*T;%插补长度
%         Lchabu0 = Lchabu0 + dL;
%         Lchabu(1,count_chabu) = Lchabu0;
%         %寻找下一个插补点us->x,y
%         us = LinearInterpolation(L,u,Lchabu0);
%         if us>1
%             us = 1;
%         end
%         Xs(count_chabu) = CurvePoint( n,p1,U,P(:,1),us);       %下一个插补点的位置
%         Ys(count_chabu) = CurvePoint( n,p1,U,P(:,2),us);
%         Us(count_chabu) = us;
%         %找到下一个插补点
%     end
%     disp('插补点到达曲线终点');
    disp('插补时间：');
    toc;
%% 加速度验证
%1、速度模差产生的加速度
Acheck1 = zeros(1,1);
for i = 2:count_chabu
    Acheck1(i-1) = ((Vchabu(i)-Vchabu(i-1)))/T;
end
% Acheck1Max = max(Acheck1)%速度模差产生的最大加速度
%2、矢量加速度
Acheck2 = zeros(1,1);
for i = 1:count_chabu-1
    if i ==1
        Norm1 = [Xs(1),Ys(1)];
        Norm2 = [Xs(2)-Xs(1),Ys(2)-Ys(1)];
        theta = VectorsAngle(Norm1,Norm2);  
    else
        Norm1 = [Xs(i)-Xs(i-1),Ys(i)-Ys(i-1)];
        Norm2 = [Xs(i+1)-Xs(i),Ys(i+1)-Ys(i)];
        theta = VectorsAngle(Norm1,Norm2);
    end
    dV = sqrt((Vchabu(i+1)*sind(theta))^2+(Vchabu(i)-Vchabu(i+1)*cosd(theta))^2);
    Acheck2(i) = dV/T;
end
% Acheck2Max = max(Acheck2)
%% 画图区
plot(Xs,Ys,'.')
xlabel('x轴，mm');
ylabel('y轴，mm');

subplot(3,1,2)
plot(Lchabu,Vchabu,'m');%速度-弧长曲线
hold on;
plot(L,V,'b');
xlabel('弧长，mm');
ylabel('速度，mm/s');

subplot(3,1,3)
plot(Lchabu(1,1:count_chabu-1),Acheck2/4000*400,'b');%矢量加速度
hold on;
plot(Lchabu(1,1:count_chabu-1),Acheck1/4000*400,'g');%代数加速度
hold on;
plot(linspace(0,max(Lchabu),200),linspace(400,400,200),'r'); %参考线
plot(linspace(0,max(Lchabu),200),linspace(-400,-400,200),'r'); %参考线
xlabel('弧长，mm');
ylabel('加速度，mm/s^2');
axis auto
%% 考查数据
disp('插补点个数');
disp(count_chabu)
disp('前瞻距离：')
disp(Lm)