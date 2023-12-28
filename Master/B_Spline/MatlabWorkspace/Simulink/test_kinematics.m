joint_x= 0;
joint_y = 0;
joint_z = 0;
joint_b = 45;
joint_a = 32;

[x,y,z,nx,ny,nz] = ForwardSolution(joint_x,joint_y,joint_z,joint_b,joint_a);
[x1,y1,z1,nx1,ny1,nz1] = ForwardSolver(joint_x,joint_y,joint_z,joint_b,joint_a);

% [jx,jy,jz,jb,ja] = InverseSolver(x,y,z,nx,ny,nz);

input = [joint_x,joint_y,joint_z,joint_b,joint_a]
middle = [x,y,z,nx,ny,nz]
middle1 = [x1,y1,z1,nx1,ny1,nz1]
% output = [jx,jy,jz,jb,ja]