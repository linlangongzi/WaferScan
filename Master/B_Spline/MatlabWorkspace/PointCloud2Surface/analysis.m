%% 导入数据并进行曲面拟合
n = 5;
m = 5;
p = 3;
q = 3;
[nBottom,~,UBottom,mBottom,~,VBottom,XctrlBottom,YctrlBottom,ZctrlBottom,bladeBottom2] = STL2Surface(n, m, p, q, bladeBottom);
[nTop,~,UTop,mTop,~,VTop,XctrlTop,YctrlTop,ZctrlTop,bladeTop2] = STL2Surface(n, m, p, q, bladeTop);
close 
plot3(bladeBottom2(:,1), bladeBottom2(:,2), bladeBottom2(:,3), '.', 'Color', 'b');
hold on
plot3(bladeTop2(:,1), bladeTop2(:,2), bladeTop2(:,3), '.', 'Color', 'r');

%% 曲面显示
ku = 50; %设置考查的点的密度
kv = 50;
u = linspace(0,1,ku);
v = linspace(0,1,kv);
CXTop = zeros(ku,kv);
CYTop = zeros(ku,kv);
CZTop = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        CXTop(i,j) = SurfacePoint(nTop,p,UTop,mTop,q,VTop,XctrlTop,u(i),v(j));%实际上CX列向量相同，因此无须每列都计算一遍
        CYTop(i,j) = SurfacePoint(nTop,p,UTop,mTop,q,VTop,YctrlTop,u(i),v(j));%实际上CY行向量相同，因此无须每行都计算一遍
        CZTop(i,j) = SurfacePoint(nTop,p,UTop,mTop,q,VTop,ZctrlTop,u(i),v(j));%坐标参数(u,v)对应的z曲面值
    end
end
% plot_surface(CXTop,CYTop,CZTop,'r');

CXBottom = zeros(ku,kv);
CYBottom = zeros(ku,kv);
CZBottom = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        CXBottom(i,j) = SurfacePoint(nBottom,p,UBottom,mBottom,q,VBottom,XctrlBottom,u(i),v(j));%实际上CX列向量相同，因此无须每列都计算一遍
        CYBottom(i,j) = SurfacePoint(nBottom,p,UBottom,mBottom,q,VBottom,YctrlBottom,u(i),v(j));%实际上CY行向量相同，因此无须每行都计算一遍
        CZBottom(i,j) = SurfacePoint(nBottom,p,UBottom,mBottom,q,VBottom,ZctrlBottom,u(i),v(j));%坐标参数(u,v)对应的z曲面值
    end
end
plot_surface(CXBottom,CYBottom,CZBottom,'b');

axis equal

%% 拟合误差分析
% distance = zeros(length(bladeBottom2), 1);
% pointCloudOut = zeros(length(bladeBottom2), 3);
% for i = 1 : length(bladeBottom2)
%     pointIn = bladeBottom2(i, :);
%     [ pointOut,u, v ] = InversePointOnSurface( nBottom,p,UBottom,mBottom,q,VBottom,XctrlBottom, YctrlBottom, ZctrlBottom, pointIn);
%     [~, N] = NormVector(nBottom,p,UBottom,mBottom,q,VBottom,XctrlBottom, YctrlBottom, ZctrlBottom, u, v, 1);
%     pointCloudOut(i, :) = pointOut;
%     distance(i) = abs((pointOut - pointIn) * N');         %距离误差向法向量方向投影才能反映点到曲面的误差
% end
% gridSize = 0.1;
% [xi, yi, zi, ci] = GridSurface(gridSize, bladeBottom2, distance);
% surf(xi,yi,zi,ci, 'EdgeColor', 'none');
% colorbar
% axis equal;
% xlabel('x axis');
% ylabel('y axis'); 
% zlabel('z axis');

%% 厚度分析
% thickness = zeros(length(bladeTop2), 1);
% pointCloudOut = zeros(length(bladeTop2), 3);
% for i = 1 : length(bladeTop2)
%     pointIn = bladeTop2(i, :);
%     [ ~,u, v ] = InversePointOnSurface2( nTop,p,UTop,mTop,q,VTop,XctrlTop, YctrlTop, ZctrlTop, pointIn); %计算上交点
%     [linePoint, dir] = NormVector(nTop,p,UTop,mTop,q,VTop,XctrlTop, YctrlTop, ZctrlTop, u, v, 1);   %得到上交点及法向量作为直线
%     [ intersect, ~, ~ ] = IntersectRaySurface2(nBottom,p,UBottom,mBottom,q,VBottom,XctrlBottom, YctrlBottom, ZctrlBottom, linePoint', dir); %计算下交点
%     thickness(i) = Distance3D(intersect, linePoint);
% end
% gridSize = 0.1;
% [xi, yi, zi, ci] = GridSurface(gridSize, bladeTop2, thickness);
% surf(xi,yi,zi,ci, 'EdgeColor', 'none');
% colorbar
% axis equal;
% xlabel('x axis');
% ylabel('y axis'); 
% zlabel('z axis');

%% 测试牛顿迭代法反求点
% u = 0.1;
% v = 0.5;
% point = [SurfacePoint(nBottom,p,UBottom,mBottom,q,VBottom,XctrlBottom,u,v);...
%          SurfacePoint(nBottom,p,UBottom,mBottom,q,VBottom,YctrlBottom,u,v);...
%          SurfacePoint(nBottom,p,UBottom,mBottom,q,VBottom,ZctrlBottom,u,v)];
% [ pointOut,uk, vk, count ] = InversePointOnSurface2(nBottom,p,UBottom,mBottom,q,VBottom,XctrlBottom,YctrlBottom,ZctrlBottom, point );
% % plot3(point(1), point(2), point(3), '*', 'Color', 'g');
% % plot3(point(1), point(2), point(3), '*', 'Color', 'r');
% pointShow = [point,pointOut']
% err = point - pointOut'
% distance = norm(err)
% count
% uk
% vk
%% 直线与曲面相交
point = bladeBottom(100, :)';
dir = [0, 1, 1]';
[ pointOut, u, v ] = IntersectRaySurface2( nBottom,p,UBottom,mBottom,q,VBottom,XctrlBottom,YctrlBottom,ZctrlBottom,point, dir );
point1 = point + 20 * dir;
point2 = point - 20 * dir;
plot3([point1(1), point2(1)], ...
      [point1(2), point2(2)], ...
      [point1(3), point2(3)], 'Color', 'r');
axis equal;
xlabel('x axis');
ylabel('y axis'); 
zlabel('z axis');
