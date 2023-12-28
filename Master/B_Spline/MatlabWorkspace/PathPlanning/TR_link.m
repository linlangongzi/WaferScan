%% 采用TR法对五轴系统进行建模，每个运动副都采用先平移后旋转的方法
%以下参数具体数值需要在设备上实测
clear
clc
%%
offsetBAZ = 50;    %B轴与A轴在z方向的距离
offsetBAX = 25;     %B轴与A轴在x方向的距离
%以下四个参数导致了刀具末端的偏移
offsetAToolZ = 100;  %A轴与工具末端在z方向的距离
offsetAToolY = 10;  %A轴与工具在y方向的距离
water_depth = 0;
lengthOfTool = offsetAToolZ + water_depth;%为末端沿Z方向的长度

jx = 0;
jy = 0;
jz = 0;
ja = 90;
jb = 0;

%% DH table 
%先平移后旋转
%link  a        b       c       x           y            z
% 1    0        0       0       jx          jy           jz                 %3个平移副
% 2    0        jb      0       0           0            0                  %b轴
% 3    ja       0       0    -offsetBAX     0           -offsetBAZ          %a轴
% 4    0        0       0       0       -offsetAToolY   -lengthOfTool      %水距轴

A1 = [1     0       0       jx;...
      0     1       0       jy;...
      0     0       1       jz;...
      0     0       0       1];
  
A2 = [cosd(jb)   0    -sind(jb)   0;...
      0         1      0        0;...
      sind(jb)   0     cosd(jb)   0;...
      0         0      0        1];

Ttemp = [1     0       0       -offsetBAX;...
         0     1       0       0;...
         0     0       1       -offsetBAZ;...
         0     0       0       1];
Rtemp = [1      0           0       0;...
         0      cosd(ja)   -sind(ja)  0;...
         0      sind(ja)    cosd(ja)  0;...
         0      0           0       1];
A3 =Ttemp*Rtemp;

A4 = [1     0       0       0;...
      0     1       0       -offsetAToolY;...
      0     0       1       -lengthOfTool;...
      0     0       0       1];



A = A1*A2*A3*A4;
tcp = [0 0 0 1]';
TCPbase = A*tcp
  