function [joint_x,joint_y,joint_z,joint_b,joint_a] = InverseSolver1( x,y,z,nx,ny,nz,wp )
%由TCP位置及法矢求五轴系统的轴坐标
%   input:
%       x,y,z,nx,ny,nz(曲面法向量)
%   output:
%       joint_x,joint_y,joint_z,joint_b,joint_a分别为对应的坐标（毫米）及角度（度）
%% 保证法向量归一化
N = norm([nx,ny,nz]);
nx = nx / N;
ny = ny / N;
nz = nz / N;
%% 首先对ab角度进行反解
 if nz <=0              %保证nz>0
     error('nz <= 0');
 elseif ny <= 0         %保证ny>0，反射镜安装角为90度
     error('ny <= 0');
% elseif ny >= 0         %保证ny<0,反射镜安装角为-90度
%      error('ny >= 0');
 end
%  joint_a = -acos(-ny) * 180 / pi;   %反射镜安装角为90度
 joint_a = acos(ny) * 180 / pi;     %反射镜安装角为-90度
 joint_b = atan(nx / nz) * 180 /pi;
 
 %% 计算偏移量
lengthOfAB = 60;
lengthOfA = 60;
lengthOfAssamble = 88.76 ;%45.83-0.95;
lengthOfTool = 0;  %反射镜拐折端长度
%平移矩阵，处理x,y,z三个直线轴的平移
% Txyz = [1,0,0,joint_x;...
%      0,1,0,joint_y;...
%      0,0,1,joint_z;...
%      0,0,0,1];
 %处理B轴旋转。B轴原点为B轴轴线与其旋转平面的交点，即认为是(jx,jy,jz)
 Rb = [cosd(joint_b),   0,  sind(joint_b), 0;...
       0,               1,  0,              0;...
       -sind(joint_b),   0,  cosd(joint_b),  0;...
       0,               0,  0,              1];
 %处理A轴与B轴的原点偏移。A轴原点为A轴轴线与其旋转平面的交点
 Tab = [1,0,0,-lengthOfAB;...
        0,1,0,0;...
        0,0,1,0;...
        0,0,0,1];
 %处理A轴的旋转
 Ra = [1,   0,              0,              0;...
       0,   cosd(joint_a),  -sind(joint_a),  0;...
       0,   sind(joint_a),  cosd(joint_a),  0;...
       0,   0,              0,              1];
 %处理直探头与A轴的原点偏移
 Ttool_1 = [1,0,0,0;...
          0,1,0,0;...
          0,0,1,-lengthOfA - lengthOfAssamble;...
          0,0,0,1];
 %处理反射镜与A轴的旋转，反射镜的安装线与A轴重合,即绕Z轴旋转,该转角为固定值，安装后确定，需要测量
 %反射镜工具的原点认为在其拐角处，z轴为与A轴平行的安装轴，声束出射方向在xoz平面内,与z轴方位角为toolAngle
 %反射镜工具的x轴认为是声束出射方向与
 toolAssamble = -90;
 Rassamble = [cosd(toolAssamble),  -sind(toolAssamble),    0,  0;...
              sind(toolAssamble), cosd(toolAssamble),    0,  0;...
              0,                0,                  1,  0;...
              0,                0,                  0,  1];
 %处理声束出射角，即与z轴方位角toolAngle,绕y轴旋转toolAngle
 toolAngle = 90;
 Rtool = [cosd(toolAngle),   0,  -sind(toolAngle), 0;...
          0,                 1,  0,              0;...
          sind(toolAngle),   0,  cosd(toolAngle),  0;...
          0,                 0,  0,              1];
 %焦斑在探头坐标系下的位置
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