function [ ScanningNum,Xout,Yout,Zout,Mxout,Myout,Mzout ] = ...
    SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,thetaN,thetaL )
%横波检测
%功能介绍：对曲面扫查进行逐行扫描式路径规划，得到扫描各行对应的点数、点坐标、横波指向Mxout
%   input:
%       n,p,U,m,q,V,Xctrl,Yctrl,Zctrl：曲面控制信息
%       ScanInterval：扫描线等弧长细分间隔
%       JogInterval：沿v方向等弧长进给间隔
%       thetaN：声速入射方向与法向量N的夹角，
%       thetaL：声束入射方向向量在LM平面的投影与扫描线切向量L的夹角，M = cross(L,N)
%   output:
%       SanningNum：1*n矩阵，每个元素对应每条扫描线的细分点数
%       Xout,Yout,Zout,Nxout,Nyout,Nzout：n*m，每一行对应一条扫描线的细分点的坐标及法向量

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
Mxout = zeros(1,1);
Myout = zeros(1,1);
Mzout = zeros(1,1);
temp = zeros(1,2); %(u,v)临时变量
u = linspace(0,1,21);
for i = 1:21
    temp(i,:) = [u(i),0]; %第一条扫描线对应的(u,v)
end
[temp,~] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval );%曲线按曲率粗分
count_jog = 0; %记录扫描线的数量
while min(temp(:,2))<1 %如此的话，v=1这条线没法走到
    fprintf('  Jog line No %d \n', count_jog+1);
    count_jog = count_jog + 1;
    [temp1,Ntemp] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval );%曲线按曲率细分
    [num,~] =  size(temp1);
    num = num -1 ;
    ScanningNum(count_jog) = num;%记录每条扫描线细分点个数,去掉每行的最后一个点
    for j = 1:num + 1
        %计算细分点的坐标，一会放到细分算法里减少重复计算量
        Xout(count_jog,j) = SurfacePoint( n,p,U,m,q,V,Xctrl,temp1(j,1),temp1(j,2) );
        Yout(count_jog,j) = SurfacePoint( n,p,U,m,q,V,Yctrl,temp1(j,1),temp1(j,2) );
        Zout(count_jog,j) = SurfacePoint( n,p,U,m,q,V,Zctrl,temp1(j,1),temp1(j,2) );
        Nxout(count_jog,j) = Ntemp(j,1);
        Nyout(count_jog,j) = Ntemp(j,2);
        Nzout(count_jog,j) = Ntemp(j,3);
        %画出法向量
%         plot3([Xout(count_jog,j),Xout(count_jog,j)+100*Nxout(count_jog,j)],...
%               [Yout(count_jog,j),Yout(count_jog,j)+100*Nyout(count_jog,j)],...
%               [Zout(count_jog,j),Zout(count_jog,j)+100*Nzout(count_jog,j)],'r');
        hold on;
    end
   
    %计算扫描线横波入射方向向量Mtemp
    for j = 2:num + 1
        xtemp = Xout(count_jog, j) - Xout(count_jog, j - 1); 
        ytemp = Yout(count_jog, j) - Yout(count_jog, j - 1);
        ztemp = Zout(count_jog, j) - Zout(count_jog, j - 1);
        Ltemp = [xtemp, ytemp, ztemp];
        Ltemp = Ltemp / norm(Ltemp);    %切线向量
        Mtemp = cross(Ltemp, [Nxout(count_jog, j - 1), Nyout(count_jog, j - 1), Nzout(count_jog, j - 1)]);
        Mtemp = Mtemp / norm(Mtemp);    %切线-法线的公垂线向量
        %横波入射方向，的N-L平面内
        Mxout(count_jog,j-1) = (sind(thetaN) * (cosd(thetaL) * Ltemp(1) + sind(thetaL) * Mtemp(1)) + cosd(thetaN) * Nxout(count_jog,j-1));
        Myout(count_jog,j-1) = (sind(thetaN) * (cosd(thetaL) * Ltemp(2) + sind(thetaL) * Mtemp(2)) + cosd(thetaN) * Nyout(count_jog,j-1));
        Mzout(count_jog,j-1) = (sind(thetaN) * (cosd(thetaL) * Ltemp(3) + sind(thetaL) * Mtemp(3)) + cosd(thetaN) * Nzout(count_jog,j-1));
        %画出横波入射角向量
        plot3([Xout(count_jog,j-1),Xout(count_jog,j-1) + 5 * Mxout(count_jog,j-1)],...
              [Yout(count_jog,j-1),Yout(count_jog,j-1) + 5 * Myout(count_jog,j-1)],...
              [Zout(count_jog,j-1),Zout(count_jog,j-1) + 5 * Mzout(count_jog,j-1)],'r');
        hold on;
    end
     %选择1：num行数据，可以保证这些数据不是0
    plot3(Xout(count_jog,1:num),Yout(count_jog,1:num),Zout(count_jog,1:num),'*-','Color','b');
    hold on; 
    text(Xout(count_jog,1),Yout(count_jog,1),Zout(count_jog,1),['扫描线',num2str(count_jog)]);

    %将粗分点等弧长进给
    temp = Jog(temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,JogInterval);
end
end

