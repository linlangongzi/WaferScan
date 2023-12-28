function [ X,Y,Z ] = AutoShape( x_aera,y_aera,z_aera,nu,nv )
%������������ʵ���Զ�����
%   input:
%       x_aera,y_aera:�����˷�������[x_begin,x_end],[y_begin,y_end]
%       z_aera:z�Ĵ�������ޣ�[z_begin,z_end]
%              ����z_begin��Ϊ����ʼ�ߵĸ߶�
%       nu,nv:u��v����ĵ�ĸ�����uΪɨ�跽��vΪ��������
%   output:
%       X,Y,Z:���κ�����Ĳ�ֵ��
X = zeros(nu+1,nv+1);
Y = zeros(nu+1,nv+1);
Z = zeros(nu+1,nv+1);
%%
%����һ����������
k = 3;%˥��ϵ��
x_max = x_aera(2);
y_max = y_aera(2);
x = linspace(x_aera(1),x_max,nu+1);
y = linspace(y_aera(1),y_max,nv+1);
z = zeros(nu+1,nv+1);
for i = 0:nu   %�����ֵ��
    for j = 0:nv
        z(i+1,j+1) = z_aera(1) + sin(x(i+1)*pi/x_max*2).*exp(-k*x(i+1)/x_max) * sin(y(j+1)*pi/y_max)*100;
    end
end
%%
%���Ƶ�һ����
Xtest = linspace(x_aera(1),x_aera(2),nu+1);
Ytest = linspace(y_aera(1),y_aera(1),nu+1);%����һ������xɨ�裬y����
Ztest = linspace(z_aera(1),z_aera(1),nu+1);%Ĭ�ϵ�һ������ˮƽ��
%��β�����ʼ

%��β�����õ�������£��������ʵֵ
X(:,1) = x;
Y(:,1) = linspace(y_aera(1),y_aera(1),nu+1);
Z(:,1) = z(:,1);
%%
%���Ƶڶ�����
dy = y_aera(2)-y_aera(1);
Xtest = linspace(x_aera(1),x_aera(2),nu+1);
Ytest = linspace(y_aera(1)+dy/nv,y_aera(1)+dy/nv,nu+1);%���ڶ�������xɨ�裬y����
Ztest = linspace(z_aera(1),z_aera(1),nu+1);%Ĭ�ϵڶ�������ˮƽ��
%��β�����ʼ

%��β��� ��õ�������£��������ʵֵ
X(:,1) = x;
Y(:,1) = linspace(y_aera(1)+dy/nv,y_aera(1)+dy/nv,nu+1);
Z(:,1) = z(:,2);
%%
%����3��nv����
for index = 3:nv+1
[ Xtest,Ytest,Ztest,Nx_test,Ny_test,Nz_test ] = ...
                        ExtrapolateCurve( X(:,index-2:index-1),...
                                          Y(:,index-2:index-1),...
                                          Z(:,index-2:index-1) );
end
end

