function [joint_x,joint_y,joint_z,joint_b,joint_a] = InverseSolver1( x,y,z,nx,ny,nz,wp )
%��TCPλ�ü���ʸ������ϵͳ��������
%   input:
%       x,y,z,nx,ny,nz(���淨����)
%   output:
%       joint_x,joint_y,joint_z,joint_b,joint_a�ֱ�Ϊ��Ӧ�����꣨���ף����Ƕȣ��ȣ�
%% ��֤��������һ��
N = norm([nx,ny,nz]);
nx = nx / N;
ny = ny / N;
nz = nz / N;
%% ���ȶ�ab�ǶȽ��з���
 if nz <=0              %��֤nz>0
     error('nz <= 0');
 elseif ny <= 0         %��֤ny>0�����侵��װ��Ϊ90��
     error('ny <= 0');
% elseif ny >= 0         %��֤ny<0,���侵��װ��Ϊ-90��
%      error('ny >= 0');
 end
%  joint_a = -acos(-ny) * 180 / pi;   %���侵��װ��Ϊ90��
 joint_a = acos(ny) * 180 / pi;     %���侵��װ��Ϊ-90��
 joint_b = atan(nx / nz) * 180 /pi;
 
 %% ����ƫ����
lengthOfAB = 60;
lengthOfA = 60;
lengthOfAssamble = 88.76 ;%45.83-0.95;
lengthOfTool = 0;  %���侵���۶˳���
%ƽ�ƾ��󣬴���x,y,z����ֱ�����ƽ��
% Txyz = [1,0,0,joint_x;...
%      0,1,0,joint_y;...
%      0,0,1,joint_z;...
%      0,0,0,1];
 %����B����ת��B��ԭ��ΪB������������תƽ��Ľ��㣬����Ϊ��(jx,jy,jz)
 Rb = [cosd(joint_b),   0,  sind(joint_b), 0;...
       0,               1,  0,              0;...
       -sind(joint_b),   0,  cosd(joint_b),  0;...
       0,               0,  0,              1];
 %����A����B���ԭ��ƫ�ơ�A��ԭ��ΪA������������תƽ��Ľ���
 Tab = [1,0,0,-lengthOfAB;...
        0,1,0,0;...
        0,0,1,0;...
        0,0,0,1];
 %����A�����ת
 Ra = [1,   0,              0,              0;...
       0,   cosd(joint_a),  -sind(joint_a),  0;...
       0,   sind(joint_a),  cosd(joint_a),  0;...
       0,   0,              0,              1];
 %����ֱ̽ͷ��A���ԭ��ƫ��
 Ttool_1 = [1,0,0,0;...
          0,1,0,0;...
          0,0,1,-lengthOfA - lengthOfAssamble;...
          0,0,0,1];
 %�����侵��A�����ת�����侵�İ�װ����A���غ�,����Z����ת,��ת��Ϊ�̶�ֵ����װ��ȷ������Ҫ����
 %���侵���ߵ�ԭ����Ϊ����սǴ���z��Ϊ��A��ƽ�еİ�װ�ᣬ�������䷽����xozƽ����,��z�᷽λ��ΪtoolAngle
 %���侵���ߵ�x����Ϊ���������䷽����
 toolAssamble = -90;
 Rassamble = [cosd(toolAssamble),  -sind(toolAssamble),    0,  0;...
              sind(toolAssamble), cosd(toolAssamble),    0,  0;...
              0,                0,                  1,  0;...
              0,                0,                  0,  1];
 %������������ǣ�����z�᷽λ��toolAngle,��y����תtoolAngle
 toolAngle = 90;
 Rtool = [cosd(toolAngle),   0,  -sind(toolAngle), 0;...
          0,                 1,  0,              0;...
          sind(toolAngle),   0,  cosd(toolAngle),  0;...
          0,                 0,  0,              1];
 %������̽ͷ����ϵ�µ�λ��
 Ori = [0;...
        0;...
        -lengthOfTool - wp;...
        1];
 T = Rb * Tab * Ra * Ttool_1 * Rassamble * Rtool;
 TCP = T*Ori;
 
 joint_x = x - TCP(1);
 joint_y = y - TCP(2);
 joint_z = z - TCP(3);
 

end