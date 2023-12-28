%�ó���ʵ�ָ������ݵ�ȫ��������ϣ����õ���B����������
clear
clc
format long
%% ���������ֵ��
%�������ò���
n = 20;
m = 20;
p = 3;
q = 3;
%�����ֵ��
Q = zeros(n+1,3*(m+1));
Xin = zeros(n+1,m+1);
Yin = zeros(n+1,m+1);
Zin = zeros(n+1,m+1);
k = 3;%˥��ϵ��
x_max = 200;
y_max = 200;
x = linspace(0,x_max,n+1);%105
y = linspace(0,y_max,m+1);%75
M = zeros(n+1,m+1);
for i = 0:n   %�����ֵ��
    for j = 0:m
        M(i+1,j+1) = sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
%���伸��������״
%���Σ�(1-x(i+1))^2-(0.5-y(j+1))^2
%Բ�ӣ�(1-x(i+1))^2+(0.5-y(j+1))^2
for i = 0:n  %�Բ�ֵ����д������ɵ������У���������
    for j = 0:m
        Xin(i+1,j+1) = x(i+1);
        Yin(i+1,j+1) = y(j+1);
        Zin(i+1,j+1) =  sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
%% ������Ƶ�P
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterpC1( n,m,Xin,Yin,Zin,p,q);
%% ��XYZ�ֱ�����������
ku = 30; %���ÿ���ĵ���ܶ�
kv = 30;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
CX = zeros(ku,kv);
CY = zeros(ku,kv);
CZ = zeros(ku,kv);
Pt = zeros(ku,3*kv);
N = zeros(ku,3*kv); 
n = n + 2;
m = m + 2;
for i = 1:ku
    for j = 1:kv
        CX(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));%ʵ����CX��������ͬ���������ÿ�ж�����һ��
        CY(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));%ʵ����CY��������ͬ���������ÿ�ж�����һ��
        CZ(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));%�������(u,v)��Ӧ��z����ֵ
       % [Pt(i,j),N(i,j)] = NormVector(n,p,U,m,q,V,X,Y,Z,u(i),v(j),1);
    end
end
plot_surface(CX,CY,CZ,'k');
hold on;
% plot3(Xin,Yin,Zin,'*','Color','b')
%mesh(X(:,1),Y(1,:),Z');%���Ƶ�
mesh(x,y,M');%��ֵ����� a = length(x),b =length(y) [b,a] = size(M')
axis equal;
xlabel('x(mm)')
ylabel('y(mm)')
zlabel('z(mm)')
% u = 0.66;
% v = 0.3;
% S = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v)
% D = SurfaceDerivsAlg1(n,p,U,m,q,V,Zctrl,u,v,2) 