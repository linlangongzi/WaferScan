function [] = plot_surface( X,Y,Z,a)
%����һ�������У�������ά���棬�������ٸ������ӳ���
%   input:
%       X,Y,Z:Ϊһ����ά���棬ÿ������Ϊm*n�ľ���
%       a:plot()���ߡ���Ĳ�������'o','*','r'��

[m,n] = size(X);

for row = 1:m
    for column = 1:n-1
        x = [X(row,column),X(row,column+1)];
        y = [Y(row,column),Y(row,column+1)];
        z = [Z(row,column),Z(row,column+1)];
        plot3(x,y,z,a);
        hold on;
    end
end

for column = 1:n
    for row = 1:m-1
        x = [X(row,column),X(row+1,column)];
        y = [Y(row,column),Y(row+1,column)];
        z = [Z(row,column),Z(row+1,column)];
        plot3(x,y,z,a);
        hold on;
    end
end       

