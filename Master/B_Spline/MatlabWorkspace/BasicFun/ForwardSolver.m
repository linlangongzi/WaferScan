function [x,y,z,nx,ny,nz] = ForwardSolver( joint_x,joint_y,joint_z,joint_b,joint_a,wp )
%由机床五个轴的轴坐标，求解TCP
%世界坐标系的原点：当轴坐标都为0时，原点对应当前位置下的B轴的轴心
%   input:
%       joint_x,joint_y,joint_z,joint_b,joint_a分别为对应的坐标（毫米）及角度（度）
%   output:
%       x,y,z即TCP在世界坐标系下的位置
%%
%进行参数检查
% if joint_b>=90 
%     error('the angle of B axis >= 90');
% elseif joint_a>=90
%     error('the angle of A axis >=90');
% end

%%
%五轴系统的配置参数
%以下两个参数导致了A轴对B轴的偏移
offsetBAZ = 0;    %B轴与A轴在z方向的距离
offsetBAX = 59;     %B轴与A轴在x方向的距离
%以下四个参数导致了刀具末端的偏移
offsetAToolZ = -60.02;  %A轴与工具在z方向的距离
offsetAToolY = 0;  %A轴与工具在y方向的距离
water_depth = -wp;
lengthOfTool = -(45.83-0.95) + water_depth;  %工具长度,即探头安装后增加的长度，同时把水距也直接设置在工具长度中
%其中：45.83为探头长度，0.95为声波出射面内凹深度
%五轴系统各关节的变换矩阵
Tx =  [1,   0,  0,  joint_x ;...
      0,   1,  0,  0	;...
      0,   0,  1,  0 ;...
      0,   0,  0,  1       ];        %B轴轴心与世界坐标系原点之间的平移变换矩阵
Ty =  [1,   0,  0,  0 ;...
      0,   1,  0,  joint_y	;...
      0,   0,  1,  0 ;...
      0,   0,  0,  1       ]; 
Tz =  [1,   0,  0,  0 ;...
      0,   1,  0,  0	;...
      0,   0,  1,  joint_z ;...
      0,   0,  0,  1       ]; 
T = Tx*Ty*Tz;
% T =  [1,   0,  0,  joint_x ;...
%       0,   1,  0,  joint_y	;...
%       0,   0,  1,  joint_z ;...
%       0,   0,  0,  1       ];   
 Rb = [cosd(joint_b),    0,  sind(joint_b),  0;...
      0,                1,  0,              0;...
      -sind(joint_b),   0,  cosd(joint_b),  0;...
      0,                0,  0,              1   ];%B轴的旋转变换矩阵
Tba = [1,   0,  0,  -offsetBAX;...
       0,   1,  0,  0         ;...
       0,   0,  1,  offsetBAZ ;...
       0,   0,  0,  1          ];                 %AB轴原点之间的平移变换矩阵
Ra = [1,    0,              0,              0;...
      0,    cosd(joint_a),  -sind(joint_a), 0;...
      0,    sind(joint_a), cosd(joint_a),   0;...
      0,    0,              0,              1];     %A轴的旋转变换矩阵

TCP_a = [0                           ;...
         -offsetAToolY               ;...
         offsetAToolZ + lengthOfTool ;...
         1                           ];  %TCP在A轴坐标系下的坐标
NT = [0;...
      0;...
      1;...
      1];%工具在A轴坐标系下的指向、法矢
%%
%进行轴坐标与世界坐标系的变换
TCP = T*Rb*Tba*Ra*TCP_a; %将工具末端从A轴坐标系下变换成世界坐标系下
N = Rb*Ra*NT;
x = TCP(1);
y = TCP(2);
z = TCP(3);
nx = N(1);
ny = N(2);
nz = N(3);
end
