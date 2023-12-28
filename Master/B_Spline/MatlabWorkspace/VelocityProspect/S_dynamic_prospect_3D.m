%%
%动态前瞻，采用S形速度规划
%   由当前速度做最大加速度【加加速度？加速度？】减速到零，得到一个虚拟轨迹速度，与速度限制比较
%   如果未超速，则加速度【加速度？加加速度？】设为最大进行插补运动，反之按最大加速度减速  
%% 设置运动轨迹
clear
clc
format long;
ab = 1;
x = ab*[0,200,200,200,400]';
y = ab*[0,0,200,200,200]';
z = ab*[0,0,0,200,200]';
% x = 0:50:1000;
% y = x;
% x = 0.5*[0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600]';
% y = 0.5*[0,100,0,100,0,100,0,100,0,100,0,100,0,100,0,100,0]';
% x = [0,100,200,100,0]';
% y = [0,100,100,100,200]';
n = length(x)-1;%数据点个数
% z = linspace(0,0,n+1)';
Q = [x,y,z];  %数据点：Q为(n+1)*2的矩阵,二维点(x,y)

%% 当相邻两线段夹角过小时，提示无法拟合，避免运动耗时
p = 4;  %B样条基函数阶次
D0 = 1*(Q(2,:)-Q(1,:)); %端点导矢设置为第一个弦方向
Dn = 1*(Q(n+1,:)-Q(n,:)); %终点导矢设置为最后一个弦方向
A0 = D0;
An = Dn;
% [ p1,m,U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
[ p1,m,U,P] = GlobalCurveInterp_C2( n,Q,p,D0,Dn,A0,An);
n = n + 4;%使用前后导矢后相当于前后增加两个节点

%% 运动参数设置
attenuation = 1;%速度衰减
Vm = 1500*attenuation;%m/s
Am = 3000;%m/s^2
Jm = 20000;%m/s^3
T = 1e-3;%s
delta = 1*Am*T^2/8;%弦高误差，由最大加速度决定，只控制向心加速度，不控制直线加速度
roum = (Vm*T/2)^2/2/delta; %最大速度来决定最大曲率半径
tic
%% 计算曲线的插值位置、曲率、曲率半径信息
% L = LengthOfPoints(Q);%折线长度，用来作细分参考
% numofP = floor(40*L)  %插值点数，用来显示
numofP = 1;
% u = linspace(0,1,numofP);
% du = u(2)-u(1);
u = zeros(1,1);
Cx = zeros(numofP,1);
Cy = zeros(numofP,1);
Cz = zeros(numofP,1);
curvity = zeros(numofP,1);
Rou = zeros(numofP,1);
Routest = zeros(numofP,1);
V = zeros(numofP,1);%记录速度,V-u表
L = zeros(numofP,1);%每一点到起点的总弧长，L-u表
d = 2;
% for i = 1:numofP
i_temp = 1;
while 1
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i_temp),d);
    Cx(i_temp,1) = tempx(1);
    CK1x = tempx(2);%即dx/du
    CK2x = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i_temp),d);
    Cy(i_temp,1) = tempy(1);
    CK1y = tempy(2);%即dy/du
    CK2y = tempy(3);
    tempz = CurveDerivsAlg1(n,p1,U,P(:,3),u(i_temp),d);
    Cz(i_temp,1) = tempz(1);
    CK1z = tempz(2);%即dz/du
    CK2z = tempz(3);
    curvity1 = (CK1x^2 + CK1y^2 + CK1z^2)*(CK2x^2 + CK2y^2 + CK2z^2) -...
                   (CK1x*CK2x + CK1y*CK2y + CK1z*CK2z )^2;
    curvity2 = CK1x^2 + CK1y^2 + CK1z^2;
    curvity(i_temp) = sqrt(curvity1) / sqrt(curvity2^3);
    
    Rou(i_temp) =  sqrt(curvity2^3) / sqrt(curvity1) ;%1/curvity(i_temp);
        
    if Rou(i_temp)>roum
       Rou(i_temp)=roum;
    end
    Routest(i_temp) =Rou(i_temp);%1/curvity(i_temp);
    
    V(i_temp,1) = 2*sqrt(Rou(i_temp)^2 - (Rou(i_temp)-delta)^2)/T;
    
    if V(i_temp,1)>=Vm;
        V(i_temp,1) = Vm;
    end
    
    dL = 0.1*2*sqrt(Rou(i_temp)^2 - (Rou(i_temp)-delta)^2);
    du = dL/sqrt((CK1x)^2 + (CK1y)^2 + (CK1z)^2 );
    i_temp = i_temp + 1;%%% 
    u(i_temp) = u(i_temp-1) + du;
     if u(i_temp)>=1
         u(i_temp) = 1;
         break;
     end
    if i_temp>=2
        L(i_temp) = L(i_temp-1) + dL;
    end
end
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i_temp),d);
    Cx(i_temp,1) = tempx(1);
    CK1x = tempx(2);%即dx/du
    CK2x = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i_temp),d);
    Cy(i_temp,1) = tempy(1);
    CK1y = tempy(2);%即dy/du
    CK2y = tempy(3);
    tempz = CurveDerivsAlg1(n,p1,U,P(:,3),u(i_temp),d);
    Cz(i_temp,1) = tempz(1);
    CK1z = tempz(2);%即dz/du
    CK2z = tempz(3);
    curvity1 = (CK1x^2 + CK1y^2 + CK1z^2)*(CK2x^2 + CK2y^2 + CK2z^2) -...
                   (CK1x*CK2x + CK1y*CK2y + CK1z*CK2z )^2;
    curvity2 = CK1x^2 + CK1y^2 + CK1z^2;
    curvity(i_temp) = sqrt(curvity1) / sqrt(curvity2^3);
    Rou(i_temp) = 1/curvity(i_temp);
    if Rou(i_temp)>roum
       Rou(i_temp)=roum;
    end
    Routest(i_temp) = Rou(i_temp);%1/curvity(i_temp);
    
    V(i_temp,1) = 2*sqrt(Rou(i_temp)^2 - (Rou(i_temp)-delta)^2)/T;
    if V(i_temp,1)>=Vm;
        V(i_temp,1) = Vm;
    end
    dL = (u(i_temp)-u(i_temp-1))*sqrt((CK1x)^2 + (CK1y)^2 + (CK1z)^2 );
    L(i_temp) = L(i_temp-1) + dL;
    numofP = i_temp-1;

 %% 显示曲率、速度限制
%  subplot(2,2,1)
%  plot3(Cx,Cy,Cz)
%  xlabel('x轴')
%  ylabel('y轴')
%  zlabel('z轴')
%  subplot(2,2,2)
%  plot(1:length(curvity),curvity);
%  ylabel('曲率')
%  subplot(2,2,3)
%  plot(1:length(Rou),Rou);
%  ylabel('曲率半径')
%  subplot(2,2,4)
%  plot(1:length(V),V)
%  ylabel('速度限制')
    
V1 = V2step(V);%速度台阶化
% V1 = V;
disp('table ready')
toc
%% 插补过程中参数
Ls = 0;     %插补点对应的弧长
Ls1 = 0;    %虚拟插补点的弧长
Vs = 0;     %插补的真实速度
Vs1 = 0;    %虚拟插补点的速度
As = 0;    %插补点的加速度
As1 = 0;   %虚拟插补点的加速度
Js = Jm;    %插补点的加加速度
Js1 = Jm;   %虚拟插补点的加加速度

Xchabu = zeros(1,1);    %插补点X坐标
Ychabu = zeros(1,1);    %插补点Y坐标
Zchabu = zeros(1,1);
Lchabu = zeros(1,1);    %插补点对应的弧长
Rouchabu = zeros(1,1);
Vchabu = zeros(1,1);
Vlimit = zeros(1,1);
Vlimit1 = zeros(1,1);
us = 0;     %插补点对应的参数u
% Ve = 0;     %虚拟插补点的速度限制
count_chabu = 1;    %插补点个数
Ttest = 10e-3;    %预测插补周期【？T的单位？ T vs Ttest】【Ttest：前瞻过程中的“虚拟插补周期”】
pause = 0; %程序暂停标志，0表示不暂停，1表示暂停
%% 插补过程进行中
% Am = Am;
disp('开始插补');
tic
while  pause == 0 && Ls1<max(L)
    if us>1
        pause = 1;
        disp('暂停时插补点数：');
        count_chabu
        break;
    end
    %对当前速度Vs【真实插补点的速度】做虚拟减速【S型规划】，查看减速到0的过程中是否会超速
%%% 【1】对虚拟插补点运动状态的更新①：【As1，Vs1,Ls1】
    As1 = As + Jm*T;
    Vs1 = Vs + As1*T;
    Ls1 = Ls;
%     count_chabu
%一直查询到虚拟速度减为0,或者达到速度限制
    count_error = 0;
    Js1 = -Jm;
    while 1 %虚拟插补，前瞻过程
        count_error = count_error + 1;
        if count_error>1000
             %error('迭代次数过多')
            disp('迭代次数过多');
            Vs1
            As1
            break;
        end
    %速度和加速度均减小到0停止，查看速度曲面是否会存在超速
        %分3种情况
       if Js1 == -Jm; 
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           if Vs1>=(2*Am^2-As1^2)/(2*Jm) && As1>=-Am%Am^2 / Jm <=  ( Vs1 + As1^2 /( 2*Jm ) ) && As1>=-Am
               Js1 = -Jm;
           elseif Vs1>As1^2/(2*Jm) && Vs1<Am^2/(2*Jm)%As1^2/(2*Jm) < Vs1 &&  Vs1<Am^2/(2*Jm)
               Js1 = -Jm;          
           elseif Vs1 <= As1^2/(2*Jm)
               Js1 = Jm;
           end 
       end
       if Js1==Jm && As1 >0
          break;
       end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        As1 = As1 + Js1*Ttest;%%%  Ttest——前瞻过程（虚拟S规划中的插补周期）
        %确定切向加速度限制
        rou1 = LinearInterpolation(L,Rou,Ls1);
        Ar1 = Vs1^2/rou1;
        if Ar1>=Am %当速度超过速度限制时，会出现这个情况，即法向加速度已经超速
            Am_temp1 = 0;
        else
            Am_temp1 = sqrt(Am^2-Ar1^2);
        end
        if As1<-Am_temp1 
            As1 = -Am_temp1;
        end     
  %%% 【4】对虚拟插补点运动状态的更新④：【Vs1,Ls1】
        if As1<=Am
            Vs1 = Vs1 + As1*Ttest;
        else
            Vs1 = Vs1 + As1*Ttest + 0.5*Js1*Ttest^2;
        end
        Ls1 = Ls1 + Vs1*Ttest + 0.5*As1*Ttest^2;    
        if Vs1<0
            break;
        end
        if Ls1>=max(L)
            Js = -Jm;
            break;
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
 %%%
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
    rou = LinearInterpolation(L,Rou,Ls); %考虑法向加速度
    Ar = Vs^2/rou;   
    if Ar>Am
        Am_temp = 0;
    else
        Am_temp = sqrt(Am^2-Ar^2);
    end
    if As>Am_temp            %防止加速度超速度
%         disp('矢量加速度正向超速');
        As = Am_temp;
        Js = 0;
    elseif As<-Am_temp
%         disp('矢量加速度反向超速');
        As = -Am_temp;
        Js = 0;
    end
    Vs = Vs + As*T;% + 0.5 * Js*T^2 ;   %加这个补偿，会导致加加速度正反交替时出现加速度尖峰
    if Vs<0
        disp('速度减到0')
        As
        Vs = 0;
        Js = Jm;
        As = 0 + Js*T;
        Vs = Vs + As*T;
    end
    Ls = Ls + Vs*T;% + 0.5 * As*T^2;
    %做加速度补偿，不加的话，在与多项式规划衔接时会出现加速度冲击，就因为少了加速度这个三角形面积
    %但加了这个补偿，会导致加加速度正反交替时出现加加速度尖峰
    us = LinearInterpolation(L,u,Ls);   %找到插补点对应的u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = LinearInterpolation(L,Cx,Ls);
    Ychabu(count_chabu) = LinearInterpolation(L,Cy,Ls);
    Zchabu(count_chabu) = LinearInterpolation(L,Cz,Ls);
    Rouchabu(count_chabu) = LinearInterpolation(L,Routest,Ls);
    
    Lchabu(count_chabu) = Ls;   
end
disp('前瞻点到达终点')
%% 处理pause = 1 采用5次多项规划减速到0
if pause ==1
%     disp('暂停段前处理：');
%     count_chabu
%     us
    Vs = Vs + As*T;
%     As
    Lleft = Vs^2/(2*Am/2);
    Lleft = min(max(L)-Ls,Lleft);
    Ls0 = Ls;
%     Vs
%     As 
    Ve = 0;
    Ae = 0;
    Ttotal = Lleft/Vs*2.5;%恰当选取这个时长系数
    Nt1 = [Ttotal^5     Ttotal^4     Ttotal^3    Ttotal^2  Ttotal  1;
           5*Ttotal^4   4*Ttotal^3   3*Ttotal^2  2*Ttotal  1       0;
           20*Ttotal^3  12*Ttotal^2  6*Ttotal    2         0       0;
           0            0            0           2         0       0;
           0            0            0           0         1       0;
           0            0            0           0         0       1];
    BoundaryCondition = [Lleft,Ve,Ae,As,Vs,0]';
    %s = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F;
    PolyCoefficient = gauss(Nt1,BoundaryCondition);
    Am_temp = PolyCoefficient(1);
    B = PolyCoefficient(2);
    C = PolyCoefficient(3);
    D = PolyCoefficient(4);
    E = PolyCoefficient(5);
    F = PolyCoefficient(6);
    SPolyCoefficient = [Am_temp,B,C,D,E,F];
    VPolyCoefficient = [0,5*Am_temp,4*B,3*C,2*D,E];
    APolyCoefficient = [0,0,20*Am_temp,12*B,6*C,2*D];
    tcount = 1;  
    t = tcount * T;
end
disp('程序暂停')
while pause == 1&& t<Ttotal && us < 1
    t = tcount * T;
    Ttest = [t.^5;t.^4;t.^3;t.^2;t;1];
    Stest = SPolyCoefficient*Ttest;
    Vs = VPolyCoefficient*Ttest;
    As = APolyCoefficient*Ttest;
    Ls = Ls0 + Stest;
    us = LinearInterpolation(L,u,Ls);   %找到插补点对应的u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = LinearInterpolation(L,Cx,Ls);
    Ychabu(count_chabu) = LinearInterpolation(L,Cy,Ls);
    Zchabu(count_chabu) = LinearInterpolation(L,Cz,Ls);
    Lchabu(count_chabu) = Ls;  
    
    tcount = tcount + 1;  

end

%% 终点段处理
if pause == 1
%% 终点前一段单独规划，做5次多项式速度规划
    disp('终点段处理：');
    count_chabu
    Lleft = max(L)-Ls
    Ls0 = Ls;
    Vs = Vs + As*T %当前插补点应该达到的速度，若使用Vs,会导致加速度冲击
    As 
    Ve = 0;
    Ae = 0;

    Ttotal = Lleft/Vs*1.5;
    Nt1 = [Ttotal^5     Ttotal^4     Ttotal^3    Ttotal^2  Ttotal  1;
           5*Ttotal^4   4*Ttotal^3   3*Ttotal^2  2*Ttotal  1       0;
           20*Ttotal^3  12*Ttotal^2  6*Ttotal    2         0       0;
           0            0            0           2         0       0;
           0            0            0           0         1       0;
           0            0            0           0         0       1];
    BoundaryCondition = [Lleft,Ve,Ae,As,Vs,0]';
    %s = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F;

    PolyCoefficient = gauss(Nt1,BoundaryCondition);
    Am_temp = PolyCoefficient(1);
    B = PolyCoefficient(2);
    C = PolyCoefficient(3);
    D = PolyCoefficient(4);
    E = PolyCoefficient(5);
    F = PolyCoefficient(6);
    SPolyCoefficient = [Am_temp,B,C,D,E,F];
    VPolyCoefficient = [0,5*Am_temp,4*B,3*C,2*D,E];
    APolyCoefficient = [0,0,20*Am_temp,12*B,6*C,2*D];
    tcount = 1;
    t = tcount * T;
    while Vs>-3 && us<1 && t<=Ttotal
        t = tcount * T;
        Ttest = [t.^5;t.^4;t.^3;t.^2;t;1];
        Stest = SPolyCoefficient*Ttest;
        Vs = VPolyCoefficient*Ttest;
        As = APolyCoefficient*Ttest;
        Ls = Ls0 + Stest;
%         Ls = Ls + Vs*T + 0.5 * As*T^2;
        if Vs<-3
            break;
        end
        us = LinearInterpolation(L,u,Ls);   %找到插补点对应的u
        if us>1
            us = 1;
        end
        Vchabu(count_chabu) = Vs;
        Vlimit(count_chabu) = LinearInterpolation(L,V,Ls);
        Vlimit1(count_chabu) = LinearInterpolation(L,V1,Ls);

        count_chabu = count_chabu + 1;
        Xchabu(count_chabu) = LinearInterpolation(L,Cx,Ls);
        Ychabu(count_chabu) = LinearInterpolation(L,Cy,Ls);
        Zchabu(count_chabu) = LinearInterpolation(L,Cz,Ls);
        Rouchabu(count_chabu) = LinearInterpolation(L,Routest,Ls);
        Lchabu(count_chabu) = Ls;  

        tcount = tcount + 1;
    end

end
disp('插补结束，消耗时间：')
toc

disp('插补节点个数：')
disp(count_chabu)
Vs
As
Lerror = sqrt( (Xchabu(count_chabu)-x(length(x)))^2 +...
               (Ychabu(count_chabu)-y(length(x)))^2 +...
               (Zchabu(count_chabu)-z(length(x)))^2 )
%% 速度考查
Vchabu1 = zeros(1,1);
for i = 2:count_chabu
    Vchabu1(i-1) = sqrt( (Xchabu(i)-Xchabu(i-1))^2 + (Ychabu(i)-Ychabu(i-1))^2 + (Zchabu(i)-Zchabu(i-1))^2)/T;
end
% Vchabu = Vchabu1;
%% 加速度考查
%1、代数加速度
Acheck1 = zeros(1,1);
for i = 2:count_chabu-1
    Acheck1(i-1) = ((Vchabu1(i)-Vchabu1(i-1)))/T;
end
%2、矢量加速度
Acheck2 = zeros(1,1);
thetatest = zeros(1,1);
for i = 1:count_chabu-2
    if i ==1
        V1 = [Xchabu(2)-Xchabu(1),Ychabu(2)-Ychabu(1),Zchabu(2)-Zchabu(1)]/T;
        V2 = [Xchabu(3)-Xchabu(2),Ychabu(3)-Ychabu(2),Zchabu(3)-Zchabu(2)]/T;
    else
        V1 = [Xchabu(i+1)-Xchabu(i),Ychabu(i+1)-Ychabu(i),Zchabu(i+1)-Zchabu(i)]/T;
        V2 = [Xchabu(i+2)-Xchabu(i+1),Ychabu(i+2)-Ychabu(i+1),Zchabu(i+2)-Zchabu(i+1)]/T;
    end
    dV = V2-V1;
    Acheck2(i) = norm(dV)/T;
end
%% 加加速度
Jcheck = zeros(1,1);
for i = 2:count_chabu-2
    Jcheck(i-1) = ((Acheck1(i)-Acheck1(i-1)))/T;
end

%% 画图区
subplot(3,2,1)  %轨迹
plot3(x,y,z,'*','Color','r');              %插值点
hold on;
plot3(Cx,Cy,Cz,'r');                        %轨迹
% plot(Xchabu,Ychabu,'.','Color','b');    %插补点
xlabel('x轴')
ylabel('y轴')
zlabel('z轴')

subplot(3,2,3)  %速度
% plot(L,V,'k');                              %速度限-弧长曲线
% hold on
plot(Lchabu(1:count_chabu-1),Vlimit,'r');  
hold on;
plot(Lchabu(1:count_chabu-1),Vchabu,'b');   %插补速度-弧长曲线
plot(Lchabu(1:count_chabu-1),Vlimit1,'g');   %速度限制，梯形化速度限制
ylabel('速度')
xlabel('弧长');

subplot(3,2,5)  %加速度
plot(Lchabu(1:count_chabu-2),Acheck1,'g');    %代数加速度
hold on;
plot(Lchabu(1:count_chabu-2),Acheck2,'r');    %矢量加速度
plot(linspace(0,max(Lchabu),200),linspace(Am,Am,200),'b');    %参考线
plot(linspace(0,max(Lchabu),200),linspace(-Am,-Am,200),'b');  %参考线
plot(linspace(0,max(Lchabu),200),linspace(0,0,200),'b');  %参考线
ylabel('加速度')
xlabel('弧长');
% 
% close
ax(1) = subplot(3,2,2);
plot(1:length(Vchabu1),Vchabu1,'b');
hold on
plot(1:length(Vchabu1),Vchabu,'m');
plot(1:length(Vlimit),Vlimit,'r');
plot(1:length(Vlimit),Vlimit1,'g');
ylabel('速度')
xlabel('时间/ms')

ax(2) = subplot(3,2,4);
plot(1:length(Acheck1),Acheck1,'g');%切向加速度
hold on
plot(1:length(Acheck2),Acheck2,'r');%矢量加速度
hold on
a1 = Vchabu.^2./Rouchabu(1:length(Vchabu)); %法向加速度
plot(1:length(Vchabu),a1,'b')
plot(1:length(a1)-1,sqrt(a1(1:length(a1)-1).^2+Acheck1.^2),'k'); %法向加速度与切向加速度 叠加
plot(1:length(Acheck2),linspace(0,0,length(Acheck2)),'b');  %参考线
plot(linspace(0,count_chabu,200),linspace(Am,Am,200),'b');    %参考线
plot(linspace(0,count_chabu,200),linspace(-Am,-Am,200),'b');  %参考线
ylabel('加速度')
xlabel('时间/ms')
ax(3) = subplot(3,2,6);
plot(1:length(Jcheck),Jcheck,'r')
hold on
% plot(1:length(Jcheck),linspace(2e4,2e4,length(Jcheck)));
% plot(1:length(Jcheck),linspace(-2e4,-2e4,length(Jcheck)));
% xlabel('加加速度')
ylabel('加加速度')
xlabel('时间/ms')
linkaxes(ax,'x');
Jmax = max(abs(Jcheck))