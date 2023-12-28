%% ����TR��������ϵͳ���н�ģ��ÿ���˶�����������ƽ�ƺ���ת�ķ���
%���²���������ֵ��Ҫ���豸��ʵ��
clear
clc
%%
offsetBAZ = 50;    %B����A����z����ľ���
offsetBAX = 25;     %B����A����x����ľ���
%�����ĸ����������˵���ĩ�˵�ƫ��
offsetAToolZ = 100;  %A���빤��ĩ����z����ľ���
offsetAToolY = 10;  %A���빤����y����ľ���
water_depth = 0;
lengthOfTool = offsetAToolZ + water_depth;%Ϊĩ����Z����ĳ���

jx = 0;
jy = 0;
jz = 0;
ja = 90;
jb = 0;

%% DH table 
%��ƽ�ƺ���ת
%link  a        b       c       x           y            z
% 1    0        0       0       jx          jy           jz                 %3��ƽ�Ƹ�
% 2    0        jb      0       0           0            0                  %b��
% 3    ja       0       0    -offsetBAX     0           -offsetBAZ          %a��
% 4    0        0       0       0       -offsetAToolY   -lengthOfTool      %ˮ����

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
  