function [Pw] = GetDataFromSTL2(fileName)
%读取超声采样点
% input:
%       xin:变换矩阵的各变量xin=[a,b,c,x,y,z]
% output:
%       Pw：[water_path,x,y,z,b,a,w]
%% get the points on the workpiece coordinate system for Pw
count = floor(csvread(fileName,0,0,[0,0,0,0])/10);
Pw = zeros(count,8); %工件坐标系上的点
for i = 1:count
%     i
    row = i * 10;
    %切分平面切分stl每一行的数据点数，需要从文件中读取
    water_path = csvread(fileName,row,0,[row,0,row,1]);
    joints = csvread(fileName,row,2,[row,2,row,6]);
    w = csvread(fileName,row,7,[row,7,row,7]);
    Pw(i,:) = [water_path,joints,w];
end

end
