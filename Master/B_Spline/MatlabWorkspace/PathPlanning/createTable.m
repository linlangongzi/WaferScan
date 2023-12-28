clear
clc
close
load('path.mat');
begin = 180;
stop = length(ScanningNum)-20;
csvwrite('ScanningNum.csv',ScanningNum(begin:stop)');
%% 做反解
joint_x = zeros(1,1);
joint_y = zeros(1,1);
joint_z = zeros(1,1);
joint_a = zeros(1,1);
joint_b = zeros(1,1);
disp('开始反解')
for i = begin:stop   %先找到对应的扫描线
%     plot3(Xout(i,:), Yout(i,:), Zout(i,:),'g');
%     hold on;
    for j = 1:ScanningNum(i)      %找到扫描线上的点
        if i == begin
%             plot3(Xout(i,j), Yout(i,j), Zout(i,j),'.','Color','g');
            hold on;
            plot3([Xout(i,j)+100*Nxout(i,j)],...
              [Yout(i,j)+100*Nyout(i,j)],...
              [Zout(i,j)+100*Nzout(i,j)],'.-','Color','r');
        else
            plot3(Xout(i,j), Yout(i,j), Zout(i,j),'.','Color','b');
            hold on;
            plot3([Xout(i,j)+100*Nxout(i,j)],...
              [Yout(i,j)+100*Nyout(i,j)],...
              [Zout(i,j)+100*Nzout(i,j)],'.-','Color','r');
        end
        hold on;
        if Nzout(i,j) <= 0
            test = 1;
        end
        [joint_x(i-begin+1, j),joint_y(i-begin+1, j),joint_z(i-begin+1, j),joint_b(i-begin+1, j),joint_a(i-begin+1, j)] ...
            = InverseSolver( Xout(i,j)  + 567.21 - 59, Yout(i,j) + 932.49, Zout(i,j), Nxout(i,j),Nyout(i,j),Nzout(i,j) );
    end
end
axis equal;
xlabel('x axis')
ylabel('y axis')
zlabel('z axis')
csvwrite('joint_x.csv',joint_x);
csvwrite('joint_y.csv',joint_y);
csvwrite('joint_z.csv',384.47 + 120.52 - (joint_z + 10.9) );
csvwrite('joint_a.csv',joint_a);
csvwrite('joint_b.csv',-joint_b);
disp('反解完成')
% w轴：0度
% lp：120+101mm


%% 看各个轴的速度加速度
time = 3.5;
for dim = 1:1%length(ScanningNum)
    %位移
    ax(1) = subplot(3,1,1);
    plot((1:ScanningNum(dim)) * time / ScanningNum(dim), joint_x(dim,1:ScanningNum(dim)),'.-','Color','b');
    hold on
    plot((1:ScanningNum(dim)) * time / ScanningNum(dim), joint_y(dim,1:ScanningNum(dim)),'.-','Color','r');
    plot((1:ScanningNum(dim)) * time / ScanningNum(dim), joint_z(dim,1:ScanningNum(dim)),'.-','Color','g');
    plot((1:ScanningNum(dim)) * time / ScanningNum(dim), joint_a(dim,1:ScanningNum(dim)),'.-','Color','m');
    plot((1:ScanningNum(dim)) * time / ScanningNum(dim), joint_b(dim,1:ScanningNum(dim)),'.-','Color','k');
    xlabel('time');
    ylabel('displacement');
    
    %速度
    ax(2) = subplot(3,1,2);
    xv = zeros(ScanningNum(dim)-1,1);
    yv = zeros(ScanningNum(dim)-1,1);
    zv = zeros(ScanningNum(dim)-1,1);
    av = zeros(ScanningNum(dim)-1,1);
    bv = zeros(ScanningNum(dim)-1,1);
    for i = 1:ScanningNum(dim) - 1
        xv(i) = (joint_x(dim,i+1) - joint_x(dim,i)) / (time / ScanningNum(dim));
        yv(i) = (joint_y(dim,i+1) - joint_y(dim,i)) / (time / ScanningNum(dim));
        zv(i) = (joint_z(dim,i+1) - joint_z(dim,i)) / (time / ScanningNum(dim));
        av(i) = (joint_a(dim,i+1) - joint_a(dim,i)) / (time / ScanningNum(dim));
        bv(i) = (joint_b(dim,i+1) - joint_b(dim,i)) / (time / ScanningNum(dim));
    end
    plot((1:length(xv)) * time / length(xv), xv,'.-','Color','b');
    hold on
    plot((1:length(xv)) * time / length(xv), yv,'.-','Color','r');
    plot((1:length(xv)) * time / length(xv), zv,'.-','Color','g');
    plot((1:length(xv)) * time / length(xv), av,'.-','Color','m');
    plot((1:length(xv)) * time / length(xv), bv,'.-','Color','k');
    xlabel('time');
    ylabel('velocity');

    %加速度
    ax(3) = subplot(3,1,3);
    xa = zeros(length(xv)-1,1);
    ya = zeros(length(yv)-1,1);
    za = zeros(length(zv)-1,1);
    aa = zeros(length(av)-1,1);
    ba = zeros(length(bv)-1,1);
    for i = 1:length(xv) - 1
        xa(i) = (xv(i+1) - xv(i)) / (time / ScanningNum(dim));
        ya(i) = (yv(i+1) - yv(i)) / (time / ScanningNum(dim));
        za(i) = (zv(i+1) - zv(i)) / (time / ScanningNum(dim));
        aa(i) = (av(i+1) - av(i)) / (time / ScanningNum(dim));
        ba(i) = (bv(i+1) - bv(i)) / (time / ScanningNum(dim));
    end
    plot((1:length(xa)) *time / length(xa) ,xa,'.-','Color','b');
    hold on
    plot((1:length(xa)) *time / length(xa) ,ya,'.-','Color','r');
    plot((1:length(xa)) *time / length(xa) ,za,'.-','Color','g');
    plot((1:length(xa)) *time / length(xa) ,aa,'.-','Color','m');
    plot((1:length(xa)) *time / length(xa) ,ba,'.-','Color','k');
    xlabel('time');
    ylabel('accelaration');
end
linkaxes(ax,'x');
