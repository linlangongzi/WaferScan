%�ó�����Թ���B��������C���䵼ʸ
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
for i = 0:n   %������Ƶ�
    for j = 0:m
        P(i+1,j+1) = (n/2-i)^2 +(n/2-j)^2;
    end
end

k = 100; %���ÿ���ĵ���ܶ�
u = linspace(0,1,k);
v = linspace(0,1,k);
C = zeros(k,k);
Cdu = zeros(k,k);
Cdv = zeros(k,k);
d = 1;
for i = 1:k
    for j = 1:k
        temp = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u(i),v(j),d);%�������(u,v)��Ӧ������ֵ
        C(i,j) = temp(1,1);
        Cdu(i,j) = temp(2,1);
        Cdv(i,j) = temp(1,2);
    end
end
% mesh(x,x,P') %���Ƶ㹹�ɵĶ��������
hold on
mesh(u,v,C') %���Ƶ����ɵ�B��������
%˵����meshǰ����������ά������һ��
hold on 
% mesh(u,v,Cdu'/5);%�����ƫ��ʸ����
%mesh(u,v,Cdv'/5);%�����ƫ��ʸ����
% axis equal
u = 0.1;
v = 1.9;
d = 1;
SC = SurfacePoint(n,p,U,m,q,V,P,u,v)
temp = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u,v,d)
Cu = temp(1,1)
cdu = temp(2,1)
cdv = temp(1,2)
% ��������ҽط����㵼ʸ����ʵ�ʵ�ʸ��಻��֤���˳������ȷ��
% temp1 = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u(60)-0.1,0.5,d);
% temp2 = SurfaceDerivsAlg1(n,p,U,m,q,V,P,u(60)+0.1,0.5,d);
% c1 = temp1(1,1);
% c2 = temp2(1,1);
% cdu2 = (c2-c1)/0.2


