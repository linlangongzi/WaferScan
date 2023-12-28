%%
%动态前瞻，采用S形速度规划
%   由当前速度做最大加速度减速到零，得到一个虚拟轨迹速度，与速度限制比较
%如果未超速，则加速度设为最大进行插补运动，反之按最大加速度减速  
%% 设置运动轨迹
clear
clc
format long;
% x = 8*[0,40,80,120,160,200]';%轨迹
% y = 40*[0,-2,5,-10,15,0]';
% x = [0,50,100,50,0]';
% y = [0,50,50,50,100]';
x = [0,200,200,400,400]';
y = [0,0,200,200,400]';
n = length(x)-1;%数据点个数
Q = [x,y];  %数据点：Q为(n+1)*2的矩阵,二维点(x,y)
%% 当相邻两线段夹角过小时小时，提示无法拟合，避免运动耗时
p = 3;  %B样条基函数阶次
D0 = 1*(Q(2,:)-Q(1,:)); %端点导矢设置为第一个弦方向
Dn = 1*(Q(n+1,:)-Q(n,:)); %终点导矢设置为最后一个弦方向
[ p1,m,U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
n = n + 2;%使用前后导矢后相当于前后增加两个节点

%% 运动参数设置
attenuation = 1;%速度衰减
Vm = 1500*attenuation;%m/s
Am = 4000;%m/s^2
Jm = 20000;%m/s^3
T = 1e-3;%s
delta = Am*T^2/8;%弦高误差，由最大加速度决定，只控制向心加速度，不控制直线加速度
roum = (Vm*T/2)^2/2/delta; %最大速度来决定最大曲率半径
tic
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
     if i>=2
        L(i) = L(i-1) + sqrt((CK1x*du)^2 + (CK1y*du)^2);
     end
end
V(numofP,1) = 0;
%终点前把速度降下来
%  for i = 1:20
%     V(numofP-100+i,1) = max(V)*0.8;
%  end
for i =1:40
    V(numofP-40+i,1) = V(numofP-40,1)*(40-i)/40;
end
V1 = V2step(V);%速度台阶化
disp('table ready')
toc
%% 插补过程中参数
Ls = 0;     %插补点对应的弧长
Ls1 = 0;    %虚拟插补点的速度
Vs = 0;     %插补的真实速度
Vs1 = 0;    %虚拟插补点的速度
As = 0;    %插补点的加速度
As1 = 0;   %虚拟插补点的加速度
Js = Jm;    %插补点的加加速度
Js1 = Jm;   %虚拟插补点的加加速度

Xchabu = zeros(1,1);    %插补点X坐标
Ychabu = zeros(1,1);    %插补点Y坐标
Lchabu = zeros(1,1);    %插补点对应的弧长
Vchabu = zeros(1,1);
Vlimit = zeros(1,1);
Vlimit1 = zeros(1,1);
us = 0;     %插补点对应的参数u
Ve = 0;     %虚拟插补点的速度限制
count_chabu = 1;    %插补点个数
Ttest = 10*T;    %预测插补周期
%% 插补过程进行中
% Am = Am;
disp('开始插补');
tic
while us<0.9%us<1&&Ls<max(L)*0.95
    %对当前速度Vs做虚拟减速，查看减速到0的过程中是否会超速
    As1 = As;
    Vs1 = Vs;
    Ls1 = Ls;
%     count_chabu
%一直查询到虚拟速度减为0,或者达到速度限制
    count_error = 0;
    while 1%~(Vs1<5&&Vs1>0 && abs(As1)<40)
        count_error = count_error + 1;
        if count_error>1000
%             error('迭代次数过多')
            break;
        end
    %速度和加速度均减小到0停止，查看速度曲面是否会存在超速
        %分3种情况
        Js1 = -Jm;
        if Vs1>=(2*Am^2-As1^2)/(2*Jm) && As1>=-Am
            Js1 = -Jm;
        elseif Vs1>As1^2/(2*Jm) && Vs1<Am^2/(2*Jm)
            Js1 = -Jm;          
        elseif As1^2/(2*Vs1)>=Jm
            Js1 = Jm;
        end   
        As1 = As1 + Js1*Ttest;
        if As1<-Am
            As1 = -Am;
        end        
        Vs1 = Vs1 + As1*Ttest;
        Ls1 = Ls1 + Vs1*Ttest;    
        if Vs1<0
            break;
        end
        
        if Ls1>=max(L)
            Js = -Jm;
            break;
%             Ve = 0;
        else
            Ve = LinearInterpolation(L,V1,Ls1);  %找到Ls1对应的速度限制 
        end
    %虚拟速度与限制速度对比        
        if Ve < Vs1      %要超速，开始减速
            Js = -Jm;
            break;
        else        
            Js = Jm;        %未超速，继续加速
        end        
    end
    %实际进行插补运动，也保证运动时速度、加速度可同时到达0
    if Js == -Jm %判断是否超速
        if Vs>=(2*Am^2-As^2)/(2*Jm) && As>=-Am
            Js = -Jm;
        elseif Vs>As^2/(2*Jm) && Vs<Am^2/(2*Jm)
            Js = -Jm;          
        elseif As^2/(2*Vs)>=Jm
            Js = Jm;
        end
    end   
    As = As + Js*T;
    if As>Am            %防止加速度超速度
        As = Am;
        Js = 0;
    elseif As<-Am
        As = -Am;
        Js = 0;
    end
    Vs = Vs + As*T;   
%     Vs = min(Vs,LinearInterpolation(L,V,Ls));
    if Vs<0
        As   %查看加速度突变值
        Vs = 0;
        Js = Jm;
        As = 0 + Js*T;
        Vs = Vs + As*T;
        count_chabu
    end
    Ls = Ls + Vs*T;
    us = LinearInterpolation(L,u,Ls);   %找到插补点对应的u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = CurvePoint(n,p1,U,P(:,1),us);
    Ychabu(count_chabu) = CurvePoint(n,p1,U,P(:,2),us);
    Lchabu(count_chabu) = Ls;   
%     if us>0.5     %用来处理暂停
%         V = zeros(length(V),1);
%         if Vs<1&&abs(As)<21
%             break;
%         end
%     end
end
disp('前瞻点到达终点')
%% 终点前一段单独规划，做5次多项式速度规划
us
Lleft = max(L)-Ls
Ls0 = Ls;
Vs
As 
Ve = 0;
Ae = 0;
Ttotal = Lleft/Vs*2.5;
% T = 1;
Nt1 = [Ttotal^5     Ttotal^4     Ttotal^3    Ttotal^2  Ttotal  1;...
       5*Ttotal^4   4*Ttotal^3   3*Ttotal^2  2*Ttotal  1       0;...
       20*Ttotal^3  12*Ttotal^2  6*Ttotal    2         0       0;...
       0            0            0           2         0       0;...
       0            0            0           0         1       0;...
       0            0            0           0         0       1];
BoundaryCondition = [Lleft,Ve,Ae,As,Vs,0]';
%s = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F;
PolyCoefficient = gauss(Nt1,BoundaryCondition);
A = PolyCoefficient(1);
B = PolyCoefficient(2);
C = PolyCoefficient(3);
D = PolyCoefficient(4);
E = PolyCoefficient(5);
F = PolyCoefficient(6);
SPolyCoefficient = [A,B,C,D,E,F];
VPolyCoefficient = [0,5*A,4*B,3*C,2*D,E];
APolyCoefficient = [0,0,20*A,12*B,6*C,2*D];
tcount = 1;

while Vs>-3&&us<1
    t = tcount * T;
    Ttest = [t.^5;t.^4;t.^3;t.^2;t;1];
    Stest = SPolyCoefficient*Ttest;
    Vs = VPolyCoefficient*Ttest;
    As = APolyCoefficient*Ttest;
    Ls = Ls0 + Stest;
    if Vs<-3
        break;
    end
    us = LinearInterpolation(L,u,Ls);   %找到插补点对应的u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = CurvePoint(n,p1,U,P(:,1),us);
    Ychabu(count_chabu) = CurvePoint(n,p1,U,P(:,2),us);
    Lchabu(count_chabu) = Ls;   

    tcount = tcount + 1;
end
toc
disp('插补结束')
disp('插补节点个数：')
disp(count_chabu)
Vs
As
Lerror = max(Xchabu)-max(x)
%% 速度考查
% for i = 2:count_chabu
%     Vchabu(i-1) = sqrt( (Xchabu(i)-Xchabu(i-1))^2 + (Ychabu(i)-Ychabu(i-1))^2 )/T;
% end
%% 加速度考查
%1、代数加速度
Acheck1 = zeros(1,1);
for i = 2:count_chabu-1
    Acheck1(i-1) = ((Vchabu(i)-Vchabu(i-1)))/T;
end
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
%% 加加速度
Jcheck = zeros(1,1);
for i = 2:count_chabu-2
    Jcheck(i-1) = ((Acheck1(i)-Acheck1(i-1)))/T;
    if Jcheck(i-1)>30000
        Jcheck(i-1) = 30000;
    elseif Jcheck(i-1)<-30000;
        Jcheck(i-1) = -30000;
    end
end

%% 画图区
subplot(3,1,1)  %轨迹
plot(x,y,'*','Color','r');              %插值点
hold on;
plot(Cx,Cy,'r');                        %轨迹
plot(Xchabu,Ychabu,'.','Color','b');    %插补点
xlabel('轨迹')

subplot(3,1,2)  %速度
plot(L,V,'k');                              %速度限-弧长曲线
hold on
plot(Lchabu(1:count_chabu-1),Vlimit,'r');  
hold on;
plot(Lchabu(1:count_chabu-1),Vchabu,'b');   %插补速度-弧长曲线
plot(Lchabu(1:count_chabu-1),Vlimit1,'g');   %速度限制，梯形化速度限制
xlabel('速度及速度限制')

subplot(3,1,3)  %加速度
plot(Lchabu(1:count_chabu-2),Acheck1,'m');    %代数加速度
hold on;
plot(Lchabu(1:count_chabu-2),Acheck2,'g');    %矢量加速度
plot(linspace(0,max(Lchabu),200),linspace(Am,Am,200),'b');    %参考线
plot(linspace(0,max(Lchabu),200),linspace(-Am,-Am,200),'b');  %参考线
plot(linspace(0,max(Lchabu),200),linspace(0,0,200),'b');  %参考线
xlabel('矢量加速度，代数加速度')
% 
close
subplot(3,1,1)
plot(1:length(Vchabu),Vchabu);
hold on
plot(1:length(Vlimit),Vlimit,'r');
plot(1:length(Vlimit),Vlimit1,'g');
xlabel('速度')
subplot(3,1,2)
plot(1:length(Acheck1),Acheck1,'m');
hold on
plot(1:length(Acheck2),Acheck2,'g');
plot(1:length(Acheck2),linspace(0,0,length(Acheck2)),'b');  %参考线
plot(linspace(0,count_chabu,200),linspace(Am,Am,200),'b');    %参考线
plot(linspace(0,count_chabu,200),linspace(-Am,-Am,200),'b');  %参考线
xlabel('加速度')
subplot(3,1,3)
plot(1:length(Jcheck),Jcheck,'r')
xlabel('加加速度')