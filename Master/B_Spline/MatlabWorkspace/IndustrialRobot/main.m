%% ����˵��
% �������Ի������˶�ѧ�㷨

%% ���������˶�ѧ
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
