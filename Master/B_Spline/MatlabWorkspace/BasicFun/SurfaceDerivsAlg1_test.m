%该程序测试构造B样条曲面C及其导矢
clear;
clc
format long
n = 7;
m = n;
p=2;
q=p;
U = [0 0 0 1 2 3 4 4 5 5 5]/5;
V = U;

P = zeros(n+1,n+1);
x = linspace(0,1,n+1);
for i = 0:n   %构造控制点
    for j = 0:m
        P(i+1,j+1) = (n/2-i)^2 +(n/2-j)^2;
    end
end

k = 100; %设置考查的点的密度
u = linspace(0,1,k);
v = linspace(0,1,k);
C = zeros(k,k);
Cdu = zeros(k,k);
Cdv = zeros(k,k);
d = 1;
for i = 1:k
    for j = 1:k
        temp = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u(i),v(j),d);%坐标参数(u,v)对应的曲面值
        C(i,j) = temp(1,1);
        Cdu(i,j) = temp(2,1);
        Cdv(i,j) = temp(1,2);
    end
end
% mesh(x,x,P') %控制点构成的多边形曲面
hold on
mesh(u,v,C') %控制点生成的B样条曲面
%说明：mesh前两个参数的维数必须一致
hold on 
% mesh(u,v,Cdu'/5);%曲面的偏导矢曲面
%mesh(u,v,Cdv'/5);%曲面的偏导矢曲面
% axis equal
u = 0.1;
v = 1.9;
d = 1;
SC = SurfacePoint(n,p,U,m,q,V,P,u,v)
temp = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u,v,d)
Cu = temp(1,1)
cdu = temp(2,1)
cdv = temp(1,2)
% 下面程序弦截法估算导矢，与实际导矢差距不大，证明了程序的正确性
% temp1 = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u(60)-0.1,0.5,d);
% temp2 = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u(60)+0.1,0.5,d);
% c1 = temp1(1,1);
% c2 = temp2(1,1);
% cdu2 = (c2-c1)/0.2


