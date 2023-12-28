%�ó�����������CurverDerivsAlg1()�����㷨A2.3
%�������Ӽ�P66
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

%�������Ƶ㼰��Ӧ������
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
    CK(i) = temp(2);%һ�׵���
end
plot(u,C,'r');
hold on;
plot(u,CK/5,'g');
d = 1;%���㵽1�׵�ʸ
temp = CurveDerivsAlg1(n,p,U,P,2.2/5,d) %���������ϵĵ��1�׵�ʸ
temp2 = CurvePoint(n,p,U,P,2.2/5) %ֱ�Ӽ��������ϵĵ��ֵ
%�鿴�������Ƿ���ȷ��������u=2.5���ĵ�ʸ
dc_du = temp(2)
