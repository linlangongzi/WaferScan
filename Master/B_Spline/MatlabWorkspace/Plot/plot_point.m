function [ ] = plot_point( X,Y,Z,a)
%����һ�������У������е㻭����
%   input:
%       X,Y,Z:Ϊһ����ά���棬ÿ������Ϊm*n�ľ���
%       a:plot()���ߡ���Ĳ�������'o','*','r'��

[m,n] = size(X);
for column = 1:n
    for row = 1:m
        plot3(X(row,column),Y(row,column),Z(row,column),'*', 'Color', a);
        hold on;
    end
end       

