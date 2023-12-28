%�ó�����Թ���B��������C
clear;
format long
n = 7;
i=4;
p=2;
U = [0 0 0 1 2 3 4 4 5 5 5]/5;

P = zeros(n+1,n+1);
x = linspace(0,1,n+1);
for i = 0:n   %������Ƶ�
    for j = 0:n
        P(i+1,j+1) = (n/2-i)^2;%-(n/2-j)^2;
    end
end

k = 100; %���ÿ���ĵ���ܶ�
u = linspace(0,1,k);
v = linspace(0,1,k);
C = zeros(k,k);
for i = 1:k
    for j = 1:k
        C(i,j) = SurfacePoint(n,p,U,n,p,U,P,u(i),v(j));%�������(u,v)��Ӧ������ֵ
    end
end

mesh(x,x,P') %���Ƶ㹹�ɵĶ��������
hold on
mesh(u,v,C') %���Ƶ����ɵ�B��������
%˵����meshǰ����������ά������һ��
