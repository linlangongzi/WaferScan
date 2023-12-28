function [Pw] = GetDataFromSTL3(fileName)
%读取CAD模型采样点
% input:
%       xin:变换矩阵的各变量xin=[a,b,c,x,y,z]
% output:
%       Pw：[water_path,x,y,z,b,a]
%% get the points on the workpiece coordinate system for Pw
count = floor(csvread(fileName,0,0,[0,0,0,0])/2);
Pw = zeros(count,3); %工件坐标系上的点
for i = 1:count
%     i
    row = i * 2;
    %切分平面切分stl每一行的数据点数，需要从文件中读取
    Pw(i,:) = csvread(fileName,row,0,[row,0,row,2]);
end

end
