function [ ] = plot_path_helix( X,Y,Z,a)
%����һ�������У�������ά���棬�����������ӳ���
%���߷�ʽΪ������
%   input:
%       X,Y,Z:Ϊһ����ά���棬ÿ������Ϊm*n�ľ���
%       a:plot()���ߡ���Ĳ�������'o','*','r'��

[m,n] = size(X);
circle_num = min(m,n); %ɨ���Ȧ��

% for row = 1:m
%     for column = 1:n-1
%         x = [X(row,column),X(row,column+1)];
%         y = [Y(row,column),Y(row,column+1)];
%         z = [Z(row,column),Z(row,column+1)];
%         plot3(x,y,z,a);
%         hold on;
%     end
% end
%     
for circle_now = 1:floor(circle_num/2)
    for bottom = circle_now:(m-circle_now)%�����ߵײ�����������
        x = [X(bottom,circle_now),X(bottom+1,circle_now)];
        y = [Y(bottom,circle_now),Y(bottom+1,circle_now)];
        z = [Z(bottom,circle_now),Z(bottom+1,circle_now)];
        plot3(x,y,z,a);
        hold on;
    end
    for right = circle_now:(n-circle_now)%�������Ҳ�����������
        x = [X(m-circle_now+1,right),X(m-circle_now+1,right+1)];
        y = [Y(m-circle_now+1,right),Y(m-circle_now+1,right+1)];
        z = [Z(m-circle_now+1,right),Z(m-circle_now+1,right+1)];
        plot3(x,y,z,a);
    end
    for top = (m-circle_now+1):-1:(circle_now+1)%�������ϲ�����������
        x = [X(top,n-circle_now+1),X(top-1,n-circle_now+1)];
        y = [Y(top,n-circle_now+1),Y(top-1,n-circle_now+1)];
        z = [Z(top,n-circle_now+1),Z(top-1,n-circle_now+1)];
        plot3(x,y,z,a);
    end        
    for left = (n+1-circle_now):-1:(circle_now+1)
        x = [X(circle_now,left),X(circle_now,left-1)];
        y = [Y(circle_now,left),Y(circle_now,left-1)];
        z = [Z(circle_now,left),Z(circle_now,left-1)];
        plot3(x,y,z,a);
    end        
end
