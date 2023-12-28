jx = [528.46, 726.65, 445.04, 615.8];
jy = [769.59,980.16, 1047.27,854.52];
jz = -[354.49,354.49, 354.49, 354.49];
ja = [0.06,0.06,0.06, 0.06];
jb = -[0.30,0.30,0.30,0.3];
wp = [42.34,42.09,42.61, 44.11];
x = zeros(1,1);
y = zeros(1,1)
z = zeros(1,1);
for i = 1:length(jx)
   [x(i),y(i),z(i)] =  ForwardSolver(jx(i),jy(i),jz(i),jb(i),ja(i),wp(i));
end
%前三个点为球靶点，最后一个点为定位孔
plot3(x,y,z,'o')

% 圆心：(508.21+59,932.49)
% w偏移：-32.41度时，定位孔在y轴上
%反射镜安装深度：28.71mm

% disp('first line, first point')
% disp([joint_x(1,1), joint_y(1,1), 384.47 + 120.52 - (joint_z(1,1) + 10.9), -joint_b(1,1), joint_a(1,1)]')
% 
% disp('first line, end point')
% disp([joint_x(1,75), joint_y(1,75), 384.47 + 120.52 - (joint_z(1,75) + 10.9), -joint_b(1,75), joint_a(1,75)]')
% 
% disp('end line, first point')
% disp([joint_x(30,1), joint_y(30,1), 384.47 + 120.52 - (joint_z(30,1) + 10.9), -joint_b(30,1), joint_a(30,1)]')
% 
% disp('end line, end point')
% disp([joint_x(30,75), joint_y(30,75), 384.47 + 120.52 - (joint_z(30,75) + 10.9), -joint_b(30,75), joint_a(30,75)]')
