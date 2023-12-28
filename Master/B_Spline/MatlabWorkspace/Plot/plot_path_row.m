function [ ] = plot_path_row( X,Y,Z,a)
%����һ�������У�������ά���棬�����������ӳ���
%���߷�ʽΪһ��һ�еؽ�������
%   input:
%       X,Y,Z:Ϊһ����ά���棬ÿ������Ϊm*n�ľ���
%       a:plot()���ߡ���Ĳ�������'o','*','r'��

[m,n] = size(X);
%��U�ķ���ɨ�飬��V�������
for column = 1:n
    for row = 1:m-1
        x = [X(row,column),X(row+1,column)];
        y = [Y(row,column),Y(row+1,column)];
        z = [Z(row,column),Z(row+1,column)];
        plot3(x,y,z,a);
        hold on;
    end
end       

