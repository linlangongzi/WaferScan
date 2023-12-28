function [ ] = plot_path_row( X,Y,Z,a)
%给点一个点阵列，画出三维曲面，即将各点连接成线
%连线方式为一列一列地进行连接
%   input:
%       X,Y,Z:为一个三维曲面，每个变量为m*n的矩阵
%       a:plot()中线、点的参数，如'o','*','r'等

[m,n] = size(X);
%沿U的方向扫查，沿V方向进给
for column = 1:n
    for row = 1:m-1
        x = [X(row,column),X(row+1,column)];
        y = [Y(row,column),Y(row+1,column)];
        z = [Z(row,column),Z(row+1,column)];
        plot3(x,y,z,a);
        hold on;
    end
end       

