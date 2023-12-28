clc
clear
close
tic
gridSize = 0.1;
dir = [0, 0, 1];
original = [0, 0, 0];
theta = 11.25;
count = 32;
grids = cell(count, 4);
figure;
hold on;
parfor index = 0 : count - 1
    %取数据 
    fprintf('   import csv file for blade %d / %d \n', index + 1, count);
    filename = sprintf('F:/work/build-ars-Desktop_Qt_5_5_0_MSVC2013_64bit-Release/WaferScan/Blade%d.csv', index);
    d = Importfile(filename);
    bottomAmp = d(:, 18);
    defectAmp = d(:, 23);
    surfaceAmp = d(:, 16);
    surfacePoints = d(:, 1:3);
    %画图并旋转
    fprintf('   get griddata of blade %d / %d \n', index + 1, count);
    [x, y, z, c] = GridSurface(gridSize, surfacePoints, defectAmp);
    grids(index + 1, :) = {x, y, z, c};
end
for index = 0 : count -1
    fprintf('   plot figure of blade %d / %d \n', index + 1, count);
    fig = surf(grids{index + 1, 1}, grids{index + 1, 2}, grids{index + 1, 3}, grids{index + 1, 4}, 'EdgeColor', 'none');
    caxis([0 100]);
    rotate(fig, dir, theta * index, original);     
end
colorbar;
% clear index count
axis equal;
xlabel('x');
ylabel('y'); 
zlabel('z');
%调整视角
% view(0,90);%xy平面
view(45,30);%透视
%保存图片数据
saveas(gcf,'BliskDefectAmp.fig')
toc