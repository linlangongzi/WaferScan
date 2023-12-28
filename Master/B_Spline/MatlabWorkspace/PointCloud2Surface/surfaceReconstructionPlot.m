%% ���������Զ��ؽ��е����ĳ�ͼ

%�ó���ʵ�ָ������ݵ�ȫ��������ϣ����õ���B����������
clear
clc
format long
%% ���������ֵ��
%�������ò���
n = 6;
m = 5;
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
plot_point(Xin,Yin,Zin,'r');
hold on;
%% ������Ƶ�P
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,Xin,Yin,Zin,p,q);
%% ��XYZ�ֱ�����������
ku = 49; %���ÿ���ĵ���ܶ�
kv = 51;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
CX = zeros(ku,kv);
CY = zeros(ku,kv);
CZ = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        CX(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));%ʵ����CX��������ͬ���������ÿ�ж�����һ��
        CY(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));%ʵ����CY��������ͬ���������ÿ�ж�����һ��
        CZ(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));%�������(u,v)��Ӧ��z����ֵ
    end
end
plot_surface(CX,CY,CZ,'k');
hold on;


%% �ֶ��ɼ�����ɨ���ߣ����д������ؽ�
n1 = 6;
m1 = 2;
InterX = zeros(n1 + 1, m1 + 1);
InterY = zeros(n1 + 1, m1 + 1);
InterZ = zeros(n1 + 1, m1 + 1);
for i = 1 : n1 + 1
    for j = 1 : m1 + 1
        InterX(i, j) = SurfacePoint(n,p, U, m, q, V, Xctrl, u((i - 1) * (ku - 1) / n1 + 1), v((j - 1) * 5 + 1));
        InterY(i, j) = SurfacePoint(n,p, U, m, q, V, Yctrl, u((i - 1) * (ku - 1) / n1 + 1), v((j - 1) * 5 + 1));
        InterZ(i, j) = SurfacePoint(n,p, U, m, q, V, Zctrl, u((i - 1) * (ku - 1) / n1 + 1), v((j - 1) * 5 + 1));
    end
end
plot_point(InterX, InterY, InterZ, 'r');

%% �Բɼ�������������
p = 3;
q = 2;
[ U1,V1,Xctrl1,Yctrl1,Zctrl1] = GlobalSurfInterp( n1, m1,InterX, InterY, InterZ, p, q);
CX1 = zeros(ku,kv);
CY1 = zeros(ku,kv);
CZ1 = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        CX1(i,j) = SurfacePoint(n1,p,U1,m1,q,V1,Xctrl1,u(i),v(j));%ʵ����CX��������ͬ���������ÿ�ж�����һ��
        CY1(i,j) = SurfacePoint(n1,p,U1,m1,q,V1,Yctrl1,u(i),v(j));%ʵ����CY��������ͬ���������ÿ�ж�����һ��
        CZ1(i,j) = SurfacePoint(n1,p,U1,m1,q,V1,Zctrl1,u(i),v(j));%�������(u,v)��Ӧ��z����ֵ
    end
end
plot_surface(CX1,CY1,CZ1,'g');

axis equal;
xlabel('x(mm)������u����ɨ�跽��')
ylabel('y(mm)������v���򣬽�������')
zlabel('z(mm)')

%% �Թ�����������������ɨ����
newX = zeros(ku, 1);
newY = zeros(ku, 1);
newZ = zeros(ku, 1);
d = 1;
step = 20;
for i = 1 : ku
    %������v�����ƫ���������в���
   dxdv = SurfaceDerivsAlg1(n1,p,U1,m1,q,V1,Xctrl1,u(i),1,d); %dv = (1, 2)
   dydv = SurfaceDerivsAlg1(n1,p,U1,m1,q,V1,Yctrl1,u(i),1,d); %dv = (1, 2)
   dzdv = SurfaceDerivsAlg1(n1,p,U1,m1,q,V1,Zctrl1,u(i),1,d); %dv = (1, 2)
   dir = [dxdv(1,2), dydv(1,2), dzdv(1,2)];
   dir = dir / norm(dir);   %��һ��
   newX(i) = CX1(i, kv) + dir(1) * step;
   newY(i) = CY1(i, kv) + dir(2) * step;
   newZ(i) = CZ1(i, kv) + dir(3) * step;
end
% plot_point(CX1(:, kv), CY1(:,kv), CZ1(:, kv), 'm');
% plot_point(newX, newY, newZ, 'm');

for i = 1 : n1+1
    plot3(newX((i - 1) * (ku - 1) / n1 + 1), newY((i - 1) * (ku - 1) / n1 + 1), newZ((i - 1) * (ku - 1) / n1 + 1), '*', 'Color', 'm');
end
