%该程序测试构造B样条曲面C
clear;
format long
n = 7;
i=4;
p=2;
U = [0 0 0 1 2 3 4 4 5 5 5]/5;

P = zeros(n+1,n+1);
x = linspace(0,1,n+1);
for i = 0:n   %构造控制点
    for j = 0:n
        P(i+1,j+1) = (n/2-i)^2;%-(n/2-j)^2;
    end
end

k = 100; %设置考查的点的密度
u = linspace(0,1,k);
v = linspace(0,1,k);
C = zeros(k,k);
for i = 1:k
    for j = 1:k
        C(i,j) = SurfacePoint(n,p,U,n,p,U,P,u(i),v(j));%坐标参数(u,v)对应的曲面值
    end
end

mesh(x,x,P') %控制点构成的多边形曲面
hold on
mesh(u,v,C') %控制点生成的B样条曲面
%说明：mesh前两个参数的维数必须一致
