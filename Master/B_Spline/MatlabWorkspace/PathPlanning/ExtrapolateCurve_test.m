%%
clear
format long;
%�Զ�������
%�������ò���
n = 10;
m = 10;
p = 3;
q = 3;
%�����ֵ��
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
%ҶƬ�� 2*(1/(10+y(j+1))-1/35)*x(i+1);
%���Σ�(1-x(i+1))^2-(0.5-y(j+1))^2
%Բ�ӣ�(1-x(i+1))^2+(0.5-y(j+1))^2
for i = 0:n  %�Բ�ֵ����д������ɵ������У���������
    for j = 0:m
        Xin(i+1,j+1) = x(i+1);
        Yin(i+1,j+1) = y(j+1);
        Zin(i+1,j+1) =  sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
% plot3(Xin,Yin,Zin,'*','Color','b')%��ֵ��
% hold on
%%
%������Ƶ㲿��
%[ U,V,P,Nu,Nv ] = GlobalSurfInterp( n,m,Q1,p,q);
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,Xin,Yin,Zin,p,q);
%%
%��XYZ�ֱ�����������
ku = 20; %���ÿ���ĵ���ܶ�
kv = 20;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
Cx = zeros(ku,kv);%��λ����
Cy = zeros(ku,kv);
Cz = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        Cx(i,j) = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v(j));
        Cy(i,j) = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v(j));
        Cz(i,j) = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v(j));
    end
end
% subplot(2,2,1);
plot_surface(Cx,Cy,Cz,'k');%�Զ��廭���㷨
hold on
axis equal
plot3(Cx(:,1),Cy(:,1),Cz(:,1),'*');
plot3(Cx(:,3),Cy(:,3),Cz(:,3),'*');
plot3(Cx(:,5),Cy(:,5),Cz(:,5),'*');
plot3(Cx(:,7),Cy(:,7),Cz(:,7),'*');
Xin = [Cx(:,5),Cx(:,7)];
Yin = [Cy(:,5),Cy(:,7)];
Zin = [Cz(:,5),Cz(:,7)];
[ Xout,Yout,Zout,Nx,Ny,Nz ] = ExtrapolateCurve( Xin,Yin,Zin,20 );
[a,~] = size(Xout);
X1 = Xout + Nx*10;
Y1 = Yout + Ny*10;
Z1 = Zout + Nz*10;
for index = 1:a
     plot3([Xout(index),X1(index)],[Yout(index),Y1(index)],[Zout(index),Z1(index)],'r');
end
plot_surface(Xout,Yout,Zout,'o');
xlabel('x(mm)');
ylabel('y(mm)');
zlabel('z(mm)');
