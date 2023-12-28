function [joint_x,joint_y,joint_z,joint_b,joint_a] = InverseSolver( x,y,z,nx,ny,nz,wp )
%由TCP位置及法矢求五轴系统的轴坐标
%   input:
%       x,y,z,nx,ny,nz 
%   output:
%       joint_x,joint_y,joint_z,joint_b,joint_a分别为对应的坐标（毫米）及角度（度）
%%
%五轴系统的配置参数
%以下两个参数导致了A轴对B轴的偏移
offsetBAX = 60;     %B轴与A轴在x方向的距离
%以下四个参数导致了刀具末端的偏移
offsetAToolZ = 60;  %A轴与工具在z方向的距离
offsetAToolY = 0;  %A轴与工具在y方向的距离
water_depth = wp;
lengthOfTool = (36-0) + water_depth;  %工具长度,即探头安装后增加的长度，同时把水距也直接设置在工具长度中
%其中：45.63为探头长度，0.95为声波出射面内凹深度
%五轴系统各关节的变换矩阵
%%
%分情况求joint_a,joint_b
% N_temp =
% [cos(joint_a)*sin(joint_b);-sin(joint_a);cos(joint_a)*cos(joint_b);1] 
%   = k*[nx;ny;nz];
if nz<=0
    error('the norm vector is wrong(nz<=0), and the angle of A or B will >=90');
else

        joint_b = atan(nx / nz);
        joint_a = -asin(ny);%atan(ny / sqrt(1 - ny * ny));
    joint_b = joint_b * 180 / pi;
    joint_a = joint_a * 180 / pi;
end
%%
%五轴系统各关节的变换矩阵
Rb = [cosd(joint_b),    0,  sind(joint_b),  0;...
      0,                1,  0,              0;...
      -sind(joint_b),   0,  cosd(joint_b),  0;...
      0,                0,  0,              1   ];%B轴的旋转变换矩阵
Tba = [1,   0,  0,  -offsetBAX;...
       0,   1,  0,  0         ;...
       0,   0,  1,  0         ;...
       0,   0,  0,  1          ];                 %AB轴原点之间的平移变换矩阵
Ra = [1,    0,              0,              0;...
      0,    cosd(joint_a),  -sind(joint_a), 0;...
      0,    sind(joint_a), cosd(joint_a),   0;...
      0,    0,              0,              1];     %A轴的旋转变换矩阵

TCP_a = [0                           ;...
         -offsetAToolY               ;...
         -offsetAToolZ - lengthOfTool ;...
         1                           ];  %TCP在a轴坐标系下的坐标
% NT = [0;...
%       0;...
%       1;...
%       1];%工具在A轴坐标系下的指向、法矢

%%
%计算joint_x,joint_y,joint_z
temp = Rb*Tba*Ra*TCP_a;
temp = [x,y,z,1] - temp';
joint_x = temp(1);
joint_y = temp(2);
joint_z = temp(3);
end
