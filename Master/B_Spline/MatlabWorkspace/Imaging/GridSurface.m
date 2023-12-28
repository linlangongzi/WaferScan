function [xi, yi, zi, ci] = GridSurface(gridSize, data, c)
    %Interpolate points in data, and their corresponding color c, to a
    %mesh. Then plot the mesh as a surface.
    %   c is optional. When omitted, the plot use mesh grid instead.
    x = data(:, 1);
    y = data(:, 2);
    z = data(:, 3);

    x_range = min(x): gridSize: max(x);
    y_range = min(y): gridSize: max(y);

    [xi, yi] = meshgrid(x_range, y_range);
    zi = griddata(x, y, z, xi, yi);

    if nargin > 2
        ci = griddata(x, y, c, xi, yi);
%         fig = surf(xi, yi, zi, ci, 'EdgeColor', 'none');
    else
%         mesh(xi, yi, zi, 'FaceColor', 'none','EdgeColor','g');
    end
%     colorbar;
end