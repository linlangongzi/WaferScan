%% ���Ļ�ͼ

%�ó���ʵ�ָ������ݵ�ȫ��������ϣ����õ���B����������
clear
format long
%�������ò���
n = 10;
m = 10;
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
        M(i+1,j+1) = sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*50;
    end
end
%���伸��������״
%���Σ�(1-x(i+1))^2-(0.5-y(j+1))^2
%Բ�ӣ�(1-x(i+1))^2+(0.5-y(j+1))^2
for i = 0:n  %�Բ�ֵ����д������ɵ������У���������
    for j = 0:m
        Xin(i+1,j+1) = x(i+1);
        Yin(i+1,j+1) = y(j+1);
        Zin(i+1,j+1) =  sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*50;
    end
end
%%
%������Ƶ�P
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,Xin,Yin,Zin,p,q);

%% ��������
ku = 40; %���ÿ���ĵ���ܶ�
kv = 40;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
CX = zeros(ku,kv);
CY = zeros(ku,kv);
CZ = zeros(ku,kv);
Pt = zeros(ku,3*kv);
N = zeros(ku,3*kv); 
for i = 1:ku
    for j = 1:kv
        CX(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));%ʵ����CX��������ͬ���������ÿ�ж�����һ��
        CY(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));%ʵ����CY��΢����ͬ���������ÿ�ж�����һ��
        CZ(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));%�������(u,v)��Ӧ��z����ֵ
       % [Pt(i,j),N(i,j)] = NormVector(n,p,U,m,q,V,X,Y,Z,u(i),v(j),1);
    end
end
plot_surface(CX,CY,CZ,'b');




%% ��·���滮
 RoughScanInterval = 10;    %ɨ���ߴַּ��
 ScanInterval = 5;          %ɨ����ϸ�ּ����ֱ���·�����͹��
 JogInterval = 20;          %���������ֱ���·�����͹��
 theta = 70;                %�Შ�����
tic
 [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
    SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,theta);
disp('·���滮���ʱ�䣺');
toc
axis equal;
axis([0,200,0,200,0,80])
xlabel('x(mm)');
ylabel('y(mm)');
zlabel('z(mm)');
