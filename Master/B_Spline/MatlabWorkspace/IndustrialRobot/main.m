%% 程序说明
% 用来测试机器人运动学算法

%% 机器人正运动学
clear
clc
close
j1 = 30;
j2 = 30;
j3 = 30;
j4 = 30; 
j5 = 30;
j6 = 30;
[ x, y, z, a, b, c] = ForwardKinematics( j1, j2, j3, j4, j5, j6 );
tcp = [ x, y, z, a, b, c]'
