%该程序用来测试CurverDerivsAlg1()，即算法A2.3
%具体例子见P66
clear;
p = 2;
U = [0 0 0 1 2 3 4 4 5 5 5]/5;
%u = 4;
n = 7;
%P = [1 2 3 8 5 6 7 4];
P = zeros(1,n+1);
for i = 0:n
    P(i+1) = (n/2-i)^2;
end

%画出控制点及对应的曲线
nx = linspace(0,1,n+1);
plot(nx,P,'*');
hold on;
k = 100;
u = linspace(0,1,k);
C = zeros(1,k);
CK = zeros(1,k);
d = 1;
for i=1:100
    %C(i) = CurvePoint(n,p,U,P,u(i));
    temp = CurveDerivsAlg1(n,p,U,P,u(i),d);
    C(i) = temp(1);
    CK(i) = temp(2);%一阶导数
end
plot(u,C,'r');
hold on;
plot(u,CK/5,'g');
d = 1;%计算到1阶导矢
temp = CurveDerivsAlg1(n,p,U,P,2.2/5,d) %计算曲线上的点的1阶导矢
temp2 = CurvePoint(n,p,U,P,2.2/5) %直接计算曲线上的点的值
%查看计算结果是否正确，曲线在u=2.5处的导矢
dc_du = temp(2)
