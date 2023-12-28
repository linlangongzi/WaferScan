function [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
    SurfaceRoutePlanning( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval )
%功能介绍：对曲面扫查进行逐行扫描式路径规划，得到扫描各行对应的点数、点坐标、法向量
%   input:
%       n,p,U,m,q,V,Xctrl,Yctrl,Zctrl：曲面控制信息
%       ScanInterval：扫描线等弧长细分间隔
%       JogInterval：沿v方向等弧长进给间隔
%   output:
%       SanningNum：1*n矩阵，每个元素对应每条扫描线的细分点数
%       Xout,Yout,Zout,Nxout,Nyout,Nzout：n*m，每一列对应一条扫描线的细分点的坐标及法向量

%% 程序开始
% Xtemp = zeros(1,1);
% Ytemp = zeros(1,1);
% Ztemp = zeros(1,1);
ScanningNum = zeros(1,1);
Xout = zeros(1,1);
Yout = zeros(1,1);
Zout = zeros(1,1);
Nxout = zeros(1,1);
Nyout = zeros(1,1);
Nzout = zeros(1,1);
temp = zeros(1,2); %(u,v)临时变量
u = linspace(0,1,21);
for i = 1:21
    temp(i,:) = [u(i),0]; %第一条扫描线对应的(u,v)
end
[temp,~] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval );%曲线按曲率粗分
count_jog = 0; %记录扫描线的数量
while min(temp(:,2))<1 %如此的话，v=1这条线没法走到
    count_jog = count_jog + 1
    [temp1,Ntemp] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval );%曲线按曲率细分
    [num,~] =  size(temp1);
    num = num -1 ;
    ScanningNum(count_jog) = num;%记录每条扫描线细分点个数,去掉每行的最后一个点
    for j = 1:num
        Xout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Xctrl,temp1(j,1),temp1(j,2) );
        %计算细分点的坐标，一会放到细分算法里减少重复计算量
        Yout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Yctrl,temp1(j,1),temp1(j,2) );
        Zout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Zctrl,temp1(j,1),temp1(j,2) );
        Nxout(j,count_jog) = Ntemp(j,1);
        Nyout(j,count_jog) = Ntemp(j,2);
        Nzout(j,count_jog) = Ntemp(j,3);
        %画出法向量
        plot3([Xout(j,count_jog),Xout(j,count_jog)+10*Nxout(j,count_jog)],...
              [Yout(j,count_jog),Yout(j,count_jog)+10*Nyout(j,count_jog)],...
              [Zout(j,count_jog),Zout(j,count_jog)+10*Nzout(j,count_jog)],'r');
        hold on;
    end
    %选择1：num行数据，可以保证这些数据不是0
    plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'k');
    hold on;
    plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'*');
    hold on;
    temp = Jog(temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,JogInterval);%将粗分点等弧长进给
end
% 最后一条线，v=1,不扫了
% count_jog = count_jog + 1
% [temp,Ntemp] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval );%曲线按曲率细分
% [num,~] =  size(temp);
% num = num -1;
% ScanningNum(count_jog) = num;%记录每条扫描线细分点个数，去掉每行的最后一个点
% for j = 1:num
%     Xout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Xctrl,temp1(j,1),temp1(j,2) );
%     Yout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Yctrl,temp1(j,1),temp1(j,2) );
%     Zout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Zctrl,temp1(j,1),temp1(j,2) );
%     Nxout(j,count_jog) = -Ntemp(j,1);
%     Nyout(j,count_jog) = -Ntemp(j,2);
%     Nzout(j,count_jog) = -Ntemp(j,3);
%     plot3([Xout(j,count_jog),Xout(j,count_jog)+20*Nxout(j,count_jog)],...
%           [Yout(j,count_jog),Yout(j,count_jog)+20*Nyout(j,count_jog)],...
%           [Zout(j,count_jog),Zout(j,count_jog)+20*Nzout(j,count_jog)],'r');
%     hold on;
% end
% %画出最后一条扫描线的法向量
% plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'k');
% plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'*'); 
end

