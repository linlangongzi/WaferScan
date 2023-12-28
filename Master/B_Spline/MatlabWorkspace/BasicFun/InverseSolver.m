function [joint_x,joint_y,joint_z,joint_b,joint_a] = InverseSolver( x,y,z,nx,ny,nz,wp )
%��TCPλ�ü���ʸ������ϵͳ��������
%   input:
%       x,y,z,nx,ny,nz 
%   output:
%       joint_x,joint_y,joint_z,joint_b,joint_a�ֱ�Ϊ��Ӧ�����꣨���ף����Ƕȣ��ȣ�
%%
%����ϵͳ�����ò���
%������������������A���B���ƫ��
offsetBAX = 60;     %B����A����x����ľ���
%�����ĸ����������˵���ĩ�˵�ƫ��
offsetAToolZ = 60;  %A���빤����z����ľ���
offsetAToolY = 0;  %A���빤����y����ľ���
water_depth = wp;
lengthOfTool = (36-0) + water_depth;  %���߳���,��̽ͷ��װ�����ӵĳ��ȣ�ͬʱ��ˮ��Ҳֱ�������ڹ��߳�����
%���У�45.63Ϊ̽ͷ���ȣ�0.95Ϊ�����������ڰ����
%����ϵͳ���ؽڵı任����
%%
%�������joint_a,joint_b
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
%����ϵͳ���ؽڵı任����
Rb = [cosd(joint_b),    0,  sind(joint_b),  0;...
      0,                1,  0,              0;...
      -sind(joint_b),   0,  cosd(joint_b),  0;...
      0,                0,  0,              1   ];%B�����ת�任����
Tba = [1,   0,  0,  -offsetBAX;...
       0,   1,  0,  0         ;...
       0,   0,  1,  0         ;...
       0,   0,  0,  1          ];                 %AB��ԭ��֮���ƽ�Ʊ任����
Ra = [1,    0,              0,              0;...
      0,    cosd(joint_a),  -sind(joint_a), 0;...
      0,    sind(joint_a), cosd(joint_a),   0;...
      0,    0,              0,              1];     %A�����ת�任����

TCP_a = [0                           ;...
         -offsetAToolY               ;...
         -offsetAToolZ - lengthOfTool ;...
         1                           ];  %TCP��a������ϵ�µ�����
% NT = [0;...
%       0;...
%       1;...
%       1];%������A������ϵ�µ�ָ�򡢷�ʸ

%%
%����joint_x,joint_y,joint_z
temp = Rb*Tba*Ra*TCP_a;
temp = [x,y,z,1] - temp';
joint_x = temp(1);
joint_y = temp(2);
joint_z = temp(3);
end
