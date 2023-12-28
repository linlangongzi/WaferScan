function [Ps,Pw] = GetDataFromSTL(axisPara)
% input:
%       xin:变换矩阵的各变量xin=[a,b,c,x,y,z]
% output:
%       Ps,Pw:机床坐标系下的点云Ps与工件坐标系下的点云Pw
%% get the points on the workpiece coordinate system
count = 0;
column = 0; %数据点阵列的行数，由STL文件切片数决定
Pw = zeros(1,3); %工件坐标系上的点
while 1
    %切分平面切分stl每一行的数据点数，需要从文件中读取
    row_length = csvread('test.csv',count,0,[count,0,count,0]);
    if row_length == 0
        disp('数据读取及有序化完成');
        break;
    elseif row_length>=3
        Ptemp = csvread('test.csv',count+1,0,[count+1,0,count+row_length,2]);
        column = column + 1;
    end
    count = count+row_length+1; %准备下一次取点
%     Ptemp点个数为row_length
    if column == 1
        Pw = Ptemp;
    else
        Pw = [Pw;Ptemp];
    end
end
[nPw,~] = size(Pw);
 one = ones(nPw,1);
 Pw = [Pw,one];
% plot3(Pw(:,1),Pw(:,2),Pw(:,3),'.','Color','r')
% hold on;
% xlabel('x axis')
% ylabel('y axis')
% zlabel('z axis')

%% design a transform matrix A
% syms a b c x y z
a = axisPara(1);
b = axisPara(2);
c = axisPara(3);
x = axisPara(4);
y = axisPara(5);
z = axisPara(6);
T = [1 0 0 x;...
     0 1 0 y;...
     0 0 1 z;...
     0 0 0 1];
 Ra = [1,   0,        0,        0;...
       0,   cosd(a),  sind(a),  0;...
       0,   -sind(a), cosd(a),  0;...
       0    0         0         1];
 Rb = [cosd(b),  0,  sind(b), 0;...
       0        1   0       0;...
       -sind(b), 0,  cosd(b), 0;...
       0        0   0       1];
 Rc = [cosd(c),   sind(c),  0, 0;...
       -sind(c),  cosd(c),  0, 0;...
       0,         0,        1, 0;...
       0,         0,        0, 1];
 A = Ra*Rb*Rc*T;
 Ps = zeros(1,4);  %机床坐标系测算的点
 Ps = (A*Pw')';
%  plot3(Ps(:,1),Ps(:,2),Ps(:,3),'.','Color','b')
%  axis equal
 
 %% 测试最近点距离
%  dis = closestDistance(Ps(:,1:3),Pw(1,1:3))
end
