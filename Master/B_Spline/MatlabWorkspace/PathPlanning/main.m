% clear
clc
close
tic
%% 读取STL文件，并进行拟合
% [interpolationX, interpolationY, interpolationZ] = readSTL('../DataInput/SampleAndShapeBaseOnPoints.csv');
% [interpolationX, interpolationY, interpolationZ] = readSTL('E:\svn-develop\build-ars-Desktop_Qt_5_5_1_MSVC2013_64bit-Release\WaferScan\SampleAndShapeBaseOnPoints.csv');
[interpolationX, interpolationY, interpolationZ] = readSTL('E:\svn-develop\build-ars-Desktop_Qt_5_5_1_MSVC2013_64bit-Release\WaferScan\SampleAndShape.csv');
% interpolationX = interpolationX';
% interpolationY = interpolationY';
% interpolationZ = interpolationZ';
[row,~] = size(interpolationX);
for index = 1 : row
    plot3(interpolationX(index, :), interpolationY(index, :), interpolationZ(index, :), '.-', 'Color', 'r');
    hold on;
end
text(interpolationX(1), interpolationY(1), interpolationZ(1), 'first line');
axis equal;
xlabel('x axis')
ylabel('y axis')
zlabel('z axis')


% interpolationX = interpolationX';
% interpolationY = interpolationY';
% interpolationZ = interpolationZ';
% [row, ~] = size(interpolationX);

%% 删除不合适的扫描线
StartIgnoration = input('enter the line numbers to be deleted from the first line: ');%input('请输入起点处需要删除的切割线条数：');
% while isempty(StartIgnoration)
%     StartIgnoration = input('enter the line numbers to be deleted from the first line: ');%input('请输入起点处需要删除的切割线条数：');
% end
EndIgnoration = input('enter the line numbers to be deleted from the last line: ');%input('请输入终点处需要删除的切割线条数：');
% while isempty(EndIgnoration)
%     EndIgnoration = input('enter the line numbers to be deleted from the last line: ');%input('请输入终点处需要删除的切割线条数：');
% end
Sx = interpolationX(StartIgnoration + 1 : row - EndIgnoration, :);
Sy = interpolationY(StartIgnoration + 1 : row - EndIgnoration, :);
Sz = interpolationZ(StartIgnoration + 1 : row - EndIgnoration, :);

[m, ~] = size(Sx);
close
for i = 1 : m
    plot3(Sx(i,:), Sy(i,:), Sz(i,:),'.-','Color','g');
    hold on;
end
text(Sx(1,1),Sy(1,1),Sz(1,1),'N0.1 sampling line')
axis equal;
xlabel('x axis')
ylabel('y axis')
zlabel('z axis')
clear row_length P U V p u P Pctrl count k i j temp Ptemp StartIgnoration EndIgnoration
clear xOrder yOrder zOrder StartIgnoration EndIgnoration m n row column xOrder1 yOrder1 zOrder1
[ Sx, Sy, Sz ] = SurfaceCut( Sx, Sy, Sz);
[m, ~] = size(Sx);
close
for i = 1 : m
    plot3(Sx(i,:), Sy(i,:), Sz(i,:),'*','Color','b');
    hold on;
end
axis equal

%% 曲面重采样
disp('下面进行曲面重采样，减少插值点，对曲面进行平滑处理')
ku = input('Density of sampling in cut line direction(5~10): ');%input('切割线方向采样密度(建议5~10）：'); %设置考查的点的密度，建议重采样密度较小！！！
while isempty(ku)
    ku = input('Density of sampling in cut line direction(5~10): ');%input('切割线方向采样密度(建议5~10）：'); %设置考查的点的密度，建议重采样密度较小！！！
end
kv = input('Density of sampling in feed direction(10~20): ');%input('前进方向采样密度（建议10~20）：');
while isempty(ku)
    kv = input('Density of sampling in feed direction(10~20): ');%input('前进方向采样密度（建议10~20）：');
end
[ Sx, Sy, Sz ] = SurfaceResample( Sx, Sy, Sz, ku, kv );
%% 曲面延展
flag = input('The surface extend? Yes;0  No:1   :');
while flag == 0
    externPos = input('Extern aera mark number: ');
    externDis = input('Width of extern aera: ');
    [Sx,Sy,Sz] = SurfaceExtend(Sx,Sy,Sz,externDis,externPos);
    [m,~] = size(Sx);
    for i = 1: m
        plot3(Sx(i,:),Sy(i,:),Sz(i,:),'.-','Color','r');
        hold on;
    end
    xlabel('x axis')
    ylabel('y axis')
    zlabel('z axis')
    axis equal
    flag = input('Finish the surface extern? Yes;1  No:0   :');
end
% 数据清理与保存
clear i j p q u v U V ignoration m n
clear ku kv row column
clear Xctrl Yctrl Zctrl


%% 反求曲面控制点
close
[n,m] = size(Sx);
x = zeros(m, n);
y = zeros(m, n);
z = zeros(m, n);

for i = 1:m
    for j = 1:n
        x(i,j) = Sx(j, i);       %调整点的顺序，保证扫描过程符合右手系
        y(i,j) = Sy(j, i);
        z(i,j) = Sz(j, i);
    end
end
ntemp = n;
n = m - 1;
m = ntemp - 1;
p = 3;
q = 2;
% 反求控制点部分 
%Xctrl的列方向为参数曲面u方向
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,x,y,z,p,q);
% n = n + 2;
% m = m + 2;

%% 做路径规划
flag = 2;%input('Choose the path planning method(1.eqaul arc length feed, 2.iso-parameter feed): ');%input('请选择路径规划策略(1.等弧长进给；2.等参数进给）：');
switch flag
    case 1
        disp('You chose the equal arc length feed method');
        %disp('选择了等弧长进给策略');
        RoughScanInterval = 40;%input('RoughScanInterval(10~20mm): ');%input('扫描线粗分间隔（粗分点用来做进给，点过多会导致扫描线扭曲，建议10~20mm）：');
        ScanInterval = 10;%input('ScanInterval(1mm): ');%input('扫描线细分间隔（即同一条扫描线上相邻两点的间隔(建议1mm）：');
        JogInterval = 10;%input('FeedInterval(0.2~10mm): ');%input('进给间隔（建议0.2~10mm）：');
        thetaN = 0;%input('Incidence angle: ');%input('声束与扫查点的法向量的夹角：');
        thetaL = 0;%input('Azimuth: ');%input('声束指向在扫查点的法向量垂直平面上的投影与扫描线切向量的夹角：');
        disp('Start path planning, please wait');%disp('路径规划开始，请稍等：')
         [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
             SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,thetaN,thetaL );     
        disp('Finish path planning');%disp('路径规划结束')
    case 2
        disp('You chose the iso-parameter');%disp('选择了等参数进给策略');
        ScanInterval = 5;%input('ScanInterval(1mm): ');%input('扫描线细分间隔（即同一条扫描线上相邻两点的间隔(建议1mm）：');
        ScanNum = 150;
        JogInterval = input('FeedInterval(0.2~10mm): ');%input('进给间隔（建议0.2~10mm）：');
        thetaN = 0;%input('Incidence angle: ');%input('声束与扫查点的法向量的夹角：');
        thetaL = 0;%input('Azimuth: ');%input('声束指向在扫查点的法向量垂直平面上的投影与扫描线切向量的夹解：');
        disp('Start path planning, please wait');%disp('路径规划开始，请稍等：')
         [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
            SurfaceRoutePlanning3( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanNum,JogInterval,thetaN,thetaL ); 
        disp('Finish path planning');%disp('路径规划结束')
    otherwise
        disp('路径规划策略选择错误，默认采用等参数进给');
        ScanInterval = input('ScanInterval(1mm): ');%input('扫描线细分间隔（即同一条扫描线上相邻两点的间隔(建议1mm）：');
        JogInterval = input('FeedInterval(0.2~10mm): ');%input('进给间隔（建议0.2~10mm）：');
        thetaN = input('Incidence angle: ');%input('声束与扫查点的法向量的夹角：');
        thetaL = input('Azimuth: ');%input('声束指向在扫查点的法向量垂直平面上的投影与扫描线切向量的夹解：');
        disp('Start path planning, please wait');%disp('路径规划开始，请稍等：')
         [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
            SurfaceRoutePlanning2( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval,JogInterval,thetaN,thetaL ); 
        disp('Finish path planning');%disp('路径规划结束')   
end
axis equal;
xlabel('x axis');
ylabel('y axis');
zlabel('z axis');
% 数据清理与保存
clear JogInterval m n p q RoughScanInterval ScanInterval thetaN thetaL flag
clear Sx Sy Sz U V Xctrl Yctrl Zctrl x y z i j xOrder yOrder zOrder

%% 运动学反解
begin = input('First scan line mark number: ');%input('输入起始扫描线编号：');
stop =  input('Last scan line mark number: ');%input('输入终点扫描线编号：');%length(ScanningNum)-20;
stop = min(stop,length(ScanningNum));   %对起始扫描线及终点扫描线处理，防止超出范围
begin = min(begin,stop);

% 做反解
nn = stop - begin + 1;
[~, mm] = size(Xout);
jx = zeros(nn, mm);
jy = zeros(nn, mm);
jz = zeros(nn, mm);
ja = zeros(nn, mm);
jb = zeros(nn, mm);
thickness = zeros(1,1);
disp('Starting reverse kinematics');
%disp('开始反解')
close
%对法向量的方法进行处理
if Nzout(1,1) > 0
    dir = 1;
else
    dir = -1;
end
inspect_method = 0;%input('method of inspection(refection = 1) :');
maxX = 0;
maxY = 0;
maxZ = 0;
minX = 1000;
minY = 1000;
minZ = 1000;
origin = [0, 0, 0];%-424.28 + 420];
offsetOfMainAxis = [0, 0, 0];
lengthOfAB = 0;%60;                                                        %采用仿形法时，将此参数设置为0，采用STL文件时，需要设置为60mm
waterpathWithoutMirror = 100;
waterpathWithMirror = 40 - 22.15;        %17.85
waterpath = waterpathWithoutMirror;

for i = begin:stop   %先找到对应的扫描线
    
    fprintf(' Reverse kinematics: Scan line No %d/%d \n',i - begin + 1, stop - begin + 1);
    text(Xout(i, 1), Yout(i, 1), Zout(i, 1), ['Scan Line No ', num2str(i)]);
    hold on;
    plot3(Xout(i, 1:ScanningNum(i)), Yout(i, 1:ScanningNum(i)), Zout(i, 1:ScanningNum(i)), '-', 'Color', 'b');
    hold on;
    
    for j = 1:ScanningNum(i)      %找到扫描线上的点
        if i == begin 
            plot3(Xout(i,j), Yout(i,j), Zout(i,j),'.','Color','b');
            hold on;
            plot3([Xout(i,j), Xout(i,j) + dir * 10 * Nxout(i,j)],...
              [Yout(i,j), Yout(i,j) + dir * 10 * Nyout(i,j)],...
              [Zout(i,j), Zout(i,j) + dir * 10 * Nzout(i,j)],'-','Color','g');
        else
            plot3(Xout(i,j), Yout(i,j), Zout(i,j),'.','Color','b');
            hold on;
            plot3([Xout(i,j), Xout(i,j) + dir * 10 * Nxout(i,j)],...
              [Yout(i,j), Yout(i,j) + dir * 10 * Nyout(i,j)],...
              [Zout(i,j), Zout(i,j) + dir * 10 * Nzout(i,j)],'-','Color','g');
        end
        hold on;
        switch inspect_method
            case 0
                [x, y, z, b, a] ...
                   = InverseSolver( Xout(i, j) + origin(1) - lengthOfAB - offsetOfMainAxis(1), ...
                                    Yout(i, j) + origin(2) - offsetOfMainAxis(2), ...
                                    Zout(i, j), dir*Nxout(i, j), dir*Nyout(i, j), dir*Nzout(i, j), waterpath);   
                jx(i - begin + 1, j) = x;
                jy(i - begin + 1, j) = y;
                jz(i - begin + 1, j) = -z + origin(3);
                ja(i - begin + 1, j) = a;
                jb(i - begin + 1, j) = -b;
            case 1
                [x, y, z, b, a] ...
                   = InverseSolver1( Xout(i, j) + origin(1) - lengthOfAB - offsetOfMainAxis(1), ...
                                    Yout(i, j) + origin(2) - offsetOfMainAxis(2), ...
                                    Zout(i, j), dir*Nxout(i, j), dir*Nyout(i, j), dir*Nzout(i, j), waterpath);   
                jx(i - begin + 1, j) = x;
                jy(i - begin + 1, j) = y;
                jz(i - begin + 1, j) = -z + origin(3);
                ja(i - begin + 1, j) = a;
                jb(i - begin + 1, j) = -b;
               if xtemp < 20 - 50
                    ScanningNum(i) = j;
                    break;
                end
        end
    end
end
axis equal;
xlabel('x axis')
ylabel('y axis')
zlabel('z axis')
% 对jx,jy,jz,ja,jb进行控制点、节点矢量反求

% order = 3;
% joint_x = zeros(nn, 3 * mm + mm + order + 1);
% joint_y = zeros(nn, 3 * mm + mm + order + 1);
% joint_z = zeros(nn, 3 * mm + mm + order + 1);
% joint_a = zeros(nn, 3 * mm + mm + order + 1);
% joint_b = zeros(nn, 3 * mm + mm + order + 1);
% for i = 1 : nn
%     [joint_x(i, :), joint_y(i, :), joint_z(i, :), joint_a(i, :), joint_b(i, :)] = SurfaceRoutePlanning4(jx(i, :), jy(i, :), jz(i, :), ja(i, :), jb(i, :), order);
% end

%% 写入数据
fprintf(' Write the data in csv files\n')
csvwrite('..\dataOutput\ScanningNum.csv',ScanningNum(begin:stop));
csvwrite('..\dataOutput\joint_x.csv',jx );
csvwrite('..\dataOutput\joint_y.csv',jy); 
csvwrite('..\dataOutput\joint_z.csv',jz);
csvwrite('..\dataOutput\joint_a.csv',ja);
csvwrite('..\dataOutput\joint_b.csv',jb);
% csvwrite('..\dataOutput\thickness.csv',thickness);
disp('done')
%disp('反解完成')
% w轴：0度
% lp：120+101mm
% 清理数据
% rangeX = [minX, maxX; 0, 185]
% rangeY = [minY, maxY; 0, 160]
% rangeZ = [minZ, maxZ; 0, 170]
clear i j begin stop I flag ray ScanLineNo
toc