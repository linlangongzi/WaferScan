function [ ] = plot_point( X,Y,Z,a)
%给点一个点阵列，将所有点画出来
%   input:
%       X,Y,Z:为一个三维曲面，每个变量为m*n的矩阵
%       a:plot()中线、点的参数，如'o','*','r'等

[m,n] = size(X);
for column = 1:n
    for row = 1:m
        plot3(X(row,column),Y(row,column),Z(row,column),'*', 'Color', a);
        hold on;
    end
end       

