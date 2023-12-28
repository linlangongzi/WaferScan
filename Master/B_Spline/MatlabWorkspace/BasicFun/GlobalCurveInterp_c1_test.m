%���Բ���ָ���˵㵼ʸ��ȫ�����߲�ֵ
clear
clc
format long;
n = 4;%���ݵ����
x = linspace(0,2,n+1)';
y = cos(x*pi);      
% x = [0,30,100,30,0]';
% y = [0,50,50,50,100]';
Q = [x,y];  %���ݵ㣺QΪ(n+1)*2�ľ���,��ά��(x,y)
plot(Q(:,1),Q(:,2),'o','Color','r') %�������ݵ�
hold on;
p = 3;  %B�����������״�

D0 = 1*(Q(2,:)-Q(1,:));%�˵㵼ʸ����Ϊ��һ���ҷ���
Dn = 1*(Q(n+1,:)-Q(n,:));%�յ㵼ʸ����Ϊ���һ���ҷ���
[ U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
n = n + 2;%ʹ��ǰ��ʸ���൱��ǰ�����������ڵ�
k=50;%��ֵ������������ʾ
u = linspace(0,1,k);
C = zeros(k,2);
for i = 1:k
    C(i,1) = CurvePoint( n,p1,U,P(:,1),u(i));%����B�������߶��壬�ɿ��Ƶ㼰�״���������������x����
    C(i,2) = CurvePoint( n,p1,U,P(:,2),u(i));%����B�������߶��壬�ɿ��Ƶ㼰�״���������������y����
end
% subplot(2,1,1)
plot(C(:,1),C(:,2),'b');
hold on;
plot(C(:,1),C(:,2),'*');

%%

%%
%�������ߵĲ�ֵλ�á����ʡ����ʰ뾶��Ϣ
d = 2;
Cx = zeros(1,1);
Cy = zeros(1,1);
CK1x = zeros(1,1);
CK1y = zeros(1,1);
CK2x = zeros(1,1);
CK2y = zeros(1,1);
for i = 1:k
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i),d);
    Cx(i) = tempx(1);
    CK1x(i) = tempx(2);%��dx/du
    CK2x(i) = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i),d);
    Cy(i) = tempy(1);
    CK1y(i) = tempy(2);%��dy/du
    CK2y(i) = tempy(3);
end
D0
D0test = [CK1x(1),CK1y(1)] %���һ�׵�ʸ
Dn
Dntest = [CK1x(k),CK1y(k)] %�յ�һ�׵�ʸ

A0test = [CK2x(1),CK2y(1)] %���Ķ��׵�ʸ
Antest = [CK2x(k),CK2y(k)]

