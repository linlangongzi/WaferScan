function [n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,pointCloud] = STL2Surface(n, m, p, q, pointCloud0)

%% 导入点云数据 
%归并排序
sizePointCloud = size(pointCloud0);
pointCloud0 = Mysort(pointCloud0' , 1 , sizePointCloud(1))';
%去重
pointCloud2 = pointCloud0(1, :);
count = 1;
for i = 2 : sizePointCloud(1)
    if (Distance3D(pointCloud2(count, :), pointCloud0(i, :)) > 0.1 )
        count  = count + 1;
        pointCloud2(count, :) = pointCloud0(i, :);
    end    
end
pointCloud0 = pointCloud2;

plot3(pointCloud0(:,1), pointCloud0(:,2), pointCloud0(:,3), '.', 'Color', 'b')
hold on

a = 0;
b = 0;
c = 0;
pointCloud = RotatePointCloud(pointCloud0, a, b, c);
plot3(pointCloud(:,1), pointCloud(:,2), pointCloud(:,3), '.', 'Color', 'b')


xlabel('x axis');
ylabel('y axis');
zlabel('z axis');


%% B样条曲线的插值点个数及阶次
% p = 3; 
% q = 3;
% n = 7;
% m = 7;

%% 求最小矩形包围盒，并将包围盒进行细分得到参考点矩阵
minX = min(pointCloud(:,1));
minY = min(pointCloud(:,2));
maxX = max(pointCloud(:,1));
maxY = max(pointCloud(:,2));
%构造参考点矩阵
refX = zeros(n + 1, m + 1);
refY = zeros(n + 1, m + 1);
refZ = zeros(n + 1, m + 1);

for row = 1 : n + 1
    y = minY + (maxY - minY) / n * (row - 1);
    for col = 1 : m + 1
        x = minX + (maxX - minX) / m * (col - 1);
        refX(row, col) = x;
        refY(row, col) = y;        
    end
end
plot_surface(refX, refY, refZ, 'r');

%% 求最小曲线包围盒，分别为四条曲线
bottomX = refX(1, :);
bottomY = refY(1, :);
topX = refX(n + 1, :);
topY = refY(n + 1, :);
leftX = refX(:, 1);
leftY = refY(:, 1);
rightX = refX(:, m + 1);
rightY = refY(:, m + 1);
flag = zeros(length(pointCloud), 1);
for i = 1 : m + 1
    refPoint = [bottomX(i), bottomY(i), 0];
    [point, flag, ~] = FindNearst(pointCloud, refPoint, flag);
    bottomX(i) = point(1);
    bottomY(i) = point(2);
    plot3(bottomX(i), bottomY(i), zeros(1, m + 1), '*', 'Color', 'r');

    refPoint = [topX(i), topY(i), 0];
    [point, flag, ~] = FindNearst(pointCloud, refPoint, flag);
    topX(i) = point(1);
    topY(i) = point(2);
    plot3(topX(i), topY(i), zeros(1, m + 1), '*', 'Color', 'r');
end
[x, y, ~] = CurveSubdivide2(bottomX', bottomY', zeros(1, m + 1)', m);
bottomX = x';
bottomY = y';
[x, y, ~] = CurveSubdivide2(topX', topY', zeros(1, m + 1)', m);
topX = x';
topY = y';

flag = zeros(length(pointCloud), 1);
for i = 1 : n + 1
    refPoint = [leftX(i), leftY(i), 0];
    [point, flag, ~] = FindNearst(pointCloud, refPoint, flag);
    leftX(i) = point(1);
    leftY(i) = point(2);
    plot3(leftX(i), leftY(i), zeros(n + 1, 1), '*', 'Color', 'r');

    refPoint = [rightX(i), rightY(i), 0];
    [point, flag, ~] = FindNearst(pointCloud, refPoint, flag);
    rightX(i) = point(1);
    rightY(i) = point(2);
    plot3(rightX(i), rightY(i), zeros(n + 1, 1), '*', 'Color', 'r');
end
[x, y, ~] = CurveSubdivide2(rightX, rightY, zeros(n + 1, 1), n);
rightX = x';
rightY = y';
[x, y, ~] = CurveSubdivide2(leftX, leftY, zeros(n + 1, 1), n);
leftX = x';
leftY = y';

%% 利用曲线包围盒对参考点矩阵进行变形收缩
%横向收缩,由left, right控制,处理x值
for i = 1 : n + 1
    for j = 1 : m + 1
       refX(i, j) = leftX(i) + (rightX(i) - leftX(i)) / m *(j - 1);
    end
end
%纵向收缩，由top，bottom控制，处理y值
for i = 1 : n + 1
    for j = 1 : m + 1
        refY(i, j) = bottomY(j) + (topY(j) - bottomY(j)) / n * (i - 1);
    end
end
plot_surface(refX, refY, refZ, 'g');
axis equal
%% 求点云中到变形后的参考点的最近点，即为插值点
interpMatrixX = zeros(n + 1, m + 1);
interpMatrixY = zeros(n + 1, m + 1);
interpMatrixZ = zeros(n + 1, m + 1);
flag = zeros(length(pointCloud), 1);
for row = 1 : n + 1
    for col = 1 : m + 1
        refPoint = [refX(row, col), ...
                    refY(row, col), ...
                    0];
        [~, flag, pos] = FindNearst(pointCloud, refPoint, flag);
        point = pointCloud0(pos, :);
        plot3(point(1), point(2), point(3), 'o', 'Color', 'g');
        interpMatrixX(row, col) = point(1);
        interpMatrixY(row, col) = point(2);
        interpMatrixZ(row, col) = point(3);
        plot3(interpMatrixX(row, col), interpMatrixY(row, col), interpMatrixZ(row, col), 'o', 'Color', 'g');
    end
    hold on;
end
plot_point(interpMatrixX, interpMatrixY, interpMatrixZ, 'g');

%% 曲面拟合及延展
Sxout = interpMatrixX;
Syout = interpMatrixY;
Szout = interpMatrixZ;
[ Sxout,Syout,Szout ] = SurfaceExtend( Sxout,Syout,Szout,5,0 );

[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n+2,m+2,Sxout,Syout,Szout,p,q);
n = n + 2;
m = m + 2;
end
