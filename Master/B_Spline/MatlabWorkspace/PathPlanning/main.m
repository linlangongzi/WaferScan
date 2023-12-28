% clear
clc
close
tic
%% ��ȡSTL�ļ������������
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

%% ɾ�������ʵ�ɨ����
StartIgnoration = input('enter the line numbers to be deleted from the first line: ');%input('��������㴦��Ҫɾ�����и���������');
% while isempty(StartIgnoration)
%     StartIgnoration = input('enter the line numbers to be deleted from the first line: ');%input('��������㴦��Ҫɾ�����и���������');
% end
EndIgnoration = input('enter the line numbers to be deleted from the last line: ');%input('�������յ㴦��Ҫɾ�����и���������');
% while isempty(EndIgnoration)
%     EndIgnoration = input('enter the line numbers to be deleted from the last line: ');%input('�������յ㴦��Ҫɾ�����и���������');
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

%% �����ز���
disp('������������ز��������ٲ�ֵ�㣬���������ƽ������')
ku = input('Density of sampling in cut line direction(5~10): ');%input('�и��߷�������ܶ�(����5~10����'); %���ÿ���ĵ���ܶȣ������ز����ܶȽ�С������
while isempty(ku)
    ku = input('Density of sampling in cut line direction(5~10): ');%input('�и��߷�������ܶ�(����5~10����'); %���ÿ���ĵ���ܶȣ������ز����ܶȽ�С������
end
kv = input('Density of sampling in feed direction(10~20): ');%input('ǰ����������ܶȣ�����10~20����');
while isempty(ku)
    kv = input('Density of sampling in feed direction(10~20): ');%input('ǰ����������ܶȣ�����10~20����');
end
[ Sx, Sy, Sz ] = SurfaceResample( Sx, Sy, Sz, ku, kv );
%% ������չ
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
% ���������뱣��
clear i j p q u v U V ignoration m n
clear ku kv row column
clear Xctrl Yctrl Zctrl


%% ����������Ƶ�
close
[n,m] = size(Sx);
x = zeros(m, n);
y = zeros(m, n);
z = zeros(m, n);

for i = 1:m
    for j = 1:n
        x(i,j) = Sx(j, i);       %�������˳�򣬱�֤ɨ����̷�������ϵ
        y(i,j) = Sy(j, i);
        z(i,j) = Sz(j, i);
    end
end
ntemp = n;
n = m - 1;
m = ntemp - 1;
p = 3;
q = 2;
% ������Ƶ㲿�� 
%Xctrl���з���Ϊ��������u����
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterp( n,m,x,y,z,p,q);
% n = n + 2;
% m = m + 2;

%% ��·���滮
flag = 2;%input('Choose the path planning method(1.eqaul arc length feed, 2.iso-parameter feed): ');%input('��ѡ��·���滮����(1.�Ȼ���������2.�Ȳ�����������');
switch flag
    case 1
        disp('You chose the equal arc length feed method');
        %disp('ѡ���˵Ȼ�����������');
        RoughScanInterval = 40;%input('RoughScanInterval(10~20mm): ');%input('ɨ���ߴַּ�����ֵַ������������������ᵼ��ɨ����Ť��������10~20mm����');
        ScanInterval = 10;%input('ScanInterval(1mm): ');%input('ɨ����ϸ�ּ������ͬһ��ɨ��������������ļ��(����1mm����');
        JogInterval = 10;%input('FeedInterval(0.2~10mm): ');%input('�������������0.2~10mm����');
        thetaN = 0;%input('Incidence angle: ');%input('������ɨ���ķ������ļнǣ�');
        thetaL = 0;%input('Azimuth: ');%input('����ָ����ɨ���ķ�������ֱƽ���ϵ�ͶӰ��ɨ�����������ļнǣ�');
        disp('Start path planning, please wait');%disp('·���滮��ʼ�����Եȣ�')
         [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
             SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,thetaN,thetaL );     
        disp('Finish path planning');%disp('·���滮����')
    case 2
        disp('You chose the iso-parameter');%disp('ѡ���˵Ȳ�����������');
        ScanInterval = 5;%input('ScanInterval(1mm): ');%input('ɨ����ϸ�ּ������ͬһ��ɨ��������������ļ��(����1mm����');
        ScanNum = 150;
        JogInterval = input('FeedInterval(0.2~10mm): ');%input('�������������0.2~10mm����');
        thetaN = 0;%input('Incidence angle: ');%input('������ɨ���ķ������ļнǣ�');
        thetaL = 0;%input('Azimuth: ');%input('����ָ����ɨ���ķ�������ֱƽ���ϵ�ͶӰ��ɨ�����������ļн⣺');
        disp('Start path planning, please wait');%disp('·���滮��ʼ�����Եȣ�')
         [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
            SurfaceRoutePlanning3( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanNum,JogInterval,thetaN,thetaL ); 
        disp('Finish path planning');%disp('·���滮����')
    otherwise
        disp('·���滮����ѡ�����Ĭ�ϲ��õȲ�������');
        ScanInterval = input('ScanInterval(1mm): ');%input('ɨ����ϸ�ּ������ͬһ��ɨ��������������ļ��(����1mm����');
        JogInterval = input('FeedInterval(0.2~10mm): ');%input('�������������0.2~10mm����');
        thetaN = input('Incidence angle: ');%input('������ɨ���ķ������ļнǣ�');
        thetaL = input('Azimuth: ');%input('����ָ����ɨ���ķ�������ֱƽ���ϵ�ͶӰ��ɨ�����������ļн⣺');
        disp('Start path planning, please wait');%disp('·���滮��ʼ�����Եȣ�')
         [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
            SurfaceRoutePlanning2( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval,JogInterval,thetaN,thetaL ); 
        disp('Finish path planning');%disp('·���滮����')   
end
axis equal;
xlabel('x axis');
ylabel('y axis');
zlabel('z axis');
% ���������뱣��
clear JogInterval m n p q RoughScanInterval ScanInterval thetaN thetaL flag
clear Sx Sy Sz U V Xctrl Yctrl Zctrl x y z i j xOrder yOrder zOrder

%% �˶�ѧ����
begin = input('First scan line mark number: ');%input('������ʼɨ���߱�ţ�');
stop =  input('Last scan line mark number: ');%input('�����յ�ɨ���߱�ţ�');%length(ScanningNum)-20;
stop = min(stop,length(ScanningNum));   %����ʼɨ���߼��յ�ɨ���ߴ�����ֹ������Χ
begin = min(begin,stop);

% ������
nn = stop - begin + 1;
[~, mm] = size(Xout);
jx = zeros(nn, mm);
jy = zeros(nn, mm);
jz = zeros(nn, mm);
ja = zeros(nn, mm);
jb = zeros(nn, mm);
thickness = zeros(1,1);
disp('Starting reverse kinematics');
%disp('��ʼ����')
close
%�Է������ķ������д���
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
lengthOfAB = 0;%60;                                                        %���÷��η�ʱ�����˲�������Ϊ0������STL�ļ�ʱ����Ҫ����Ϊ60mm
waterpathWithoutMirror = 100;
waterpathWithMirror = 40 - 22.15;        %17.85
waterpath = waterpathWithoutMirror;

for i = begin:stop   %���ҵ���Ӧ��ɨ����
    
    fprintf(' Reverse kinematics: Scan line No %d/%d \n',i - begin + 1, stop - begin + 1);
    text(Xout(i, 1), Yout(i, 1), Zout(i, 1), ['Scan Line No ', num2str(i)]);
    hold on;
    plot3(Xout(i, 1:ScanningNum(i)), Yout(i, 1:ScanningNum(i)), Zout(i, 1:ScanningNum(i)), '-', 'Color', 'b');
    hold on;
    
    for j = 1:ScanningNum(i)      %�ҵ�ɨ�����ϵĵ�
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
% ��jx,jy,jz,ja,jb���п��Ƶ㡢�ڵ�ʸ������

% order = 3;
% joint_x = zeros(nn, 3 * mm + mm + order + 1);
% joint_y = zeros(nn, 3 * mm + mm + order + 1);
% joint_z = zeros(nn, 3 * mm + mm + order + 1);
% joint_a = zeros(nn, 3 * mm + mm + order + 1);
% joint_b = zeros(nn, 3 * mm + mm + order + 1);
% for i = 1 : nn
%     [joint_x(i, :), joint_y(i, :), joint_z(i, :), joint_a(i, :), joint_b(i, :)] = SurfaceRoutePlanning4(jx(i, :), jy(i, :), jz(i, :), ja(i, :), jb(i, :), order);
% end

%% д������
fprintf(' Write the data in csv files\n')
csvwrite('..\dataOutput\ScanningNum.csv',ScanningNum(begin:stop));
csvwrite('..\dataOutput\joint_x.csv',jx );
csvwrite('..\dataOutput\joint_y.csv',jy); 
csvwrite('..\dataOutput\joint_z.csv',jz);
csvwrite('..\dataOutput\joint_a.csv',ja);
csvwrite('..\dataOutput\joint_b.csv',jb);
% csvwrite('..\dataOutput\thickness.csv',thickness);
disp('done')
%disp('�������')
% w�᣺0��
% lp��120+101mm
% ��������
% rangeX = [minX, maxX; 0, 185]
% rangeY = [minY, maxY; 0, 160]
% rangeZ = [minZ, maxZ; 0, 170]
clear i j begin stop I flag ray ScanLineNo
toc