
row = length(num);
x = zeros(row, max(num));
y = zeros(row, max(num));
z = zeros(row, max(num));
thick = zeros(row, max(num));
for index = 1 : row
% index = 1;
   x(index, 1 : num(index)) = csvread( 'F:/work/B_Spline/Qt_project/build-BSpline-Desktop_Qt_5_5_0_MinGW_32bit-Debug/pathX.csv',...
                                     index - 1, 0,  [index - 1, 0, index - 1, num(index) - 1]);   
   y(index, 1 : num(index)) = csvread( 'F:/work/B_Spline/Qt_project/build-BSpline-Desktop_Qt_5_5_0_MinGW_32bit-Debug/pathY.csv',...
                                     index - 1, 0,  [index - 1, 0, index - 1, num(index) - 1]);
   z(index, 1 : num(index)) = csvread( 'F:/work/B_Spline/Qt_project/build-BSpline-Desktop_Qt_5_5_0_MinGW_32bit-Debug/pathZ.csv',...
                                     index - 1, 0,  [index - 1, 0, index - 1, num(index) - 1]);
%    thick(index, 1 : num(index)) = csvread( 'F:/work/B_Spline/Qt_project/build-BSpline-Desktop_Qt_5_5_0_MinGW_32bit-Debug/thickness.csv',...
%                                      index - 1, 0,  [index - 1, 0, index - 1, num(index) - 1]);
    plot3(x(index, 1:num(index)), y(index, 1 : num(index)), z(index, 1 : num(index)));
    hold on ;
    text(x(index,1), y(index,1), z(index,1), ['Scan Line', num2str(index)]);
end
% plot_surface(x, y, z, '-.');
%% 
% count = 0;
% x = zeros(1,1);
% y = zeros(1,1);
% z = zeros(1,1);
% thick = zeros(1,1);
% for index = 1 : length(thickness)
%     if thickness(index) ~= 0
%         count = count + 1;
% %         x(count,1) = pathX(index);
% %         y(count,1) = pathY(index);
% %         z(count,1) = pathZ(index);
%         thick(count,1) = thickness(index);
%     end
% end
% [xi, yi, zi, ci] = GridSurface(0.2, [x, y, z], thick);
% surface = surf(xi,yi,zi,ci, 'EdgeColor', 'none');
% colorbar
%%
axis equal 
xlabel('x axis')
ylabel('y axis')
zlabel('z axis')
view(-130, 18)
