clc
clear
format long

% a = 15;
% b = 10;
% c = 35;
% x = 0;
% y = 0;
% z = 0;
% axisPara = [a,b,c,x,y,z];
% [Ps,Pw] = GetDataFromSTL(axisPara);
%% 读取超声采样点
disp('begin to read data')
Ps1= GetDataFromSTL2('1.csv'); %获得工件坐标系上的点Pw
Ps2 = GetDataFromSTL2('2.csv');
% Ps3 = GetDataFromSTL2('3.csv');
Ps = [Ps1;Ps2];
disp('the number of sampling points is: ')
length(Ps)
disp('finish to read data')

disp('begin to transfer the coordination')
water_path = ( Ps(:,1)+Ps(:,2) ) * 750e-3;
joint_x = Ps(:,3);
joint_y = Ps(:,4);
joint_z = -Ps(:,5);
joint_b = -Ps(:,6);
joint_a = Ps(:,7);
joint_w = Ps(:,8);
% clear Ps;
[numOfPoint,~] = size(joint_x);
Ps = zeros(numOfPoint,4);
for i = 1:numOfPoint
    [Ps(i,1),Ps(i,2),Ps(i,3),~,~,~] = ForwardSolver( water_path(i),joint_x(i),joint_y(i),joint_z(i),joint_b(i),joint_a(i),joint_w(i) );
    Ps(i,4) = 1;
end
clear joint_x joint_y joint_z joint_b joint_a;
disp('finish to transfer the coordination')

plot3(Ps(:,1),Ps(:,2),Ps(:,3),'.')
axis equal
%% 读取CAD模型采样点
disp('begin to read the CAD data')
Pw = GetDataFromSTL3('pw.csv');
disp('the number of CAD points is: ');
length(Pw)
one = ones(length(Pw),1);
Pw = [Pw,one];
disp('finish to read the CAD data')
hold on
plot3(Pw(:,1),Pw(:,2),Pw(:,3),'.','Color','r')
axis equal
%% 算法验证数据
% Pw = GetDataFromSTL2('2.csv');
% %Ps = A*Pw,求得A即可，指定机床到达Pw点，变换后即机床到达Ps点，物理空间上是同一个位置
% [row,~]=size(Pw);
% noise = wgn(row,3,1);
% Pw(:,1:3) = Pw(:,1:3) + noise; %添加白噪声
% % plot3(Ps(:,1),Ps(:,2),Ps(:,3),'.','Color','r');
% % hold on
% % plot3(Pw(:,1),Pw(:,2),Pw(:,3),'.','Color','b');
% axis equal
% xlabel('x axis')
% ylabel('y axis')
% zlabel('z axis')
%% 变换矩阵计算
disp('初始误差')
error = ErrorDistance(Ps,Pw);
disp('begin to caculate the transform Matrix')
tic
[ xout,error ] = FRCG( Ps,Pw ) %做共轭梯度法
% % [ xout,error ] = UnivariateSearch( Ps,Pw )
toc
disp('finish to caculate the transform Matrix')
showResults
