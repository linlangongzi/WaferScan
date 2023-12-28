%% load file
for bladeIndex = 0 : 0
    filename = sprintf('F:/work/build-ars-Desktop_Qt_5_5_0_MSVC2013_64bit-Release/WaferScan/Blade%d.csv', bladeIndex);
    saveFileName = sprintf('D:/Dropbox/blisk/Figures/Blade%d', bladeIndex);
    saveFigure = sprintf('D:/Dropbox/blisk/Figures/Blade%d.fig', bladeIndex);
    d = Importfile(filename);
    close
    %% ȡ����
    fprintf('           read data blade%d / 32 \n', bladeIndex + 1);
    materialSpeedFactor = 6070 / 2000;

    % modelSurfacePoints = d(:, 7:9);
    % modelBottomPoints = d(:, 10:12);

    waterPath = d(:, 32);
    surfaceAmp = d(:, 16);
    % bottomDepth = d(:, 17) .* materialSpeedFactor;
    bottomAmp = d(:, 18);
    % defectDepth = d(:, 22) .* materialSpeedFactor;
    defectAmp = d(:, 23);
    thicknessOfSTL = d(:, 23 + 6 ) * materialSpeedFactor;         %����������STL�ཻ�õ��ĺ��
    thicknessOfSound = d(:, 17) * materialSpeedFactor;            %�ײ��Ĵ���ʱ��
    delta = (thicknessOfSTL - thicknessOfSound);
    % thickness = d(:, 13);
    % thicknessError = bottomDepth - thickness;
    surfacePoints = d(:, 1:3);
    % direction = d(:, 4:6);
    % clear d;
    % defectPoints = surfacePoints + direction .* repmat(defectDepth, 1, 3);
    % bottomPoints = surfacePoints + direction .* repmat(bottomDepth, 1, 3);

    gridSize = 0.05;

    %% ��ͼ
    fprintf('           plot blade%d / 32 \n', bladeIndex + 1);
    [xi, yi, zi, ci] = GridSurface(gridSize, surfacePoints+repmat([0, 0, 0], length(surfacePoints), 1), defectAmp);
    surface = surf(xi,yi,zi,ci, 'EdgeColor', 'none');    
    caxis([0,max(thicknessOfSound)]);
    colorbar
    hold on
    filename = '..\dataInput\surfaceGrid.csv';
%     [X, Y, Z] = GetGrid(filename);
%     plot_surface(X,Y,Z,'m')
    axis equal;
    xlabel('x axis');
    ylabel('y axis'); 
    zlabel('z axis');
%     title(sprintf('Blade%d', bladeIndex));
    view(-146,32);
%     saveas(surface, saveFigure);
%     print('-f1', '-r600', '-djpeg', saveFileName);
end