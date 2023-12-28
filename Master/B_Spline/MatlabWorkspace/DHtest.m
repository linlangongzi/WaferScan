%% �����ṹͼ
clear;
clc;
joint_x = 100;
joint_y = 200;
joint_z = 100;
joint_b = -30;
joint_a = 30;
%����ϵͳ�����ò���
%������������������A���B���ƫ��
offsetBAZ = 0;    %B����A����z����ľ���
offsetBAX = 59;     %B����A����x����ľ���
%�����ĸ����������˵���ĩ�˵�ƫ��
offsetAToolZ = 101;  %A���빤����z����ľ���
offsetAToolY = 0;  %A���빤����y����ľ���
water_depth = 0;
lengthOfTool = 0 + water_depth;  %���߳���,��̽ͷ��װ�����ӵĳ��ȣ�ͬʱ��ˮ��Ҳֱ�������ڹ��߳�����
%����ϵͳ���ؽڵı任����
Tx = [1,   0,  0,  joint_x ;...
      0,   1,  0,  0	;...
      0,   0,  1,  0 ;...
      0,   0,  0,  1       ];        %B����������������ϵԭ��֮���ƽ�Ʊ任����
Ty =  [1,   0,  0,  0 ;...
      0,   1,  0,  joint_y	;...
      0,   0,  1,  0 ;...
      0,   0,  0,  1       ]; 
Tz =  [1,   0,  0,  0 ;...
      0,   1,  0,  0	;...
      0,   0,  1,  -joint_z ;...
      0,   0,  0,  1       ]; 
Rb = [cosd(joint_b),    0,  sind(joint_b),  0;...
      0,                1,  0,              0;...
      -sind(joint_b),   0,  cosd(joint_b),  0;...
      0,                0,  0,              1   ];%B�����ת�任����
Tba = [1,   0,  0,  -offsetBAX;...
       0,   1,  0,  0         ;...
       0,   0,  1,  offsetBAZ ;...
       0,   0,  0,  1          ];                 %AB��ԭ��֮���ƽ�Ʊ任����
Ra = [1,    0,              0,              0;...
      0,    cosd(joint_a),  -sind(joint_a), 0;...
      0,    sind(joint_a), cosd(joint_a),   0;...
      0,    0,              0,              1];     %A�����ת�任����

TCP_a = [1,0,0,0                           ;...
         0,1,0,0              ;...
         0,0,1,-offsetAToolZ ;...
         0,0,0,1                           ];  %TCP��A������ϵ�µ�����
NT = [0;...
      0;...
      1;...
      1];%������A������ϵ�µ�ָ�򡢷�ʸ
T = cat(3, Tx, Ty, Tz, Rb, Ra, Tba, TCP_a);
origin = [0; 0; 0; 1];
points = origin;
Ti = eye(4);
for index = 1: size(T, 3)
    Ti = Ti * T(:, :, index);
    point = Ti * origin;
    points = [points, point];
end
point
plot3(points(1, :), points(2, :), points(3, :), 'o-');
axis equal
%%
%�����������
[x,y,z,nx,ny,nz] = ForwardSolver( joint_x,joint_y,joint_z,joint_b,joint_a );
tcp = [x,y,z,nx,ny,nz]'

[jx,jy,jz,jb,ja] = InverseSolver( x,y,z,nx,ny,nz );
joint = [jx,jy,jz,jb,ja]'

