%该程序测试构造B样条曲面C及其导矢
clear;
clc
format long
n = 7;
p= 2 ;
U = [0 0 0 1 2 3 4 4 5 5 5]/5;

P = zeros(1,n+1);
x = linspace(0,1,n+1);
for i = 0:n   %构造控制点
        P(i+1) = (n/2-i)^2;
end
k = 100; %设置考查的点的密度
u = linspace(0,1,k);
C = zeros(1,k);
CK = zeros(1,k);
d = 1;
% for i=1:100
%     C(i) = CurvePoint(n,p,U,P,u(i));
%     temp = CurveDerivsAlg1(n,p,U,P,u(i),d);
%     CK(i) = temp(2);
% end
% plot(u,C);
% hold on;
% plot(u,CK/5,'r');
% hold on;
% plot(x,P,'*')
u = -1;
span = FindSpan(n,p,u,U )
basis = BasisFuns(span,u,p,U )
Cu = CurvePoint(n,p,U,P,u)
ders = DersBasisFuns(span,u,p,n,U)
Cdu =  CurveDerivsAlg1(n,p,U,P,u,2)
