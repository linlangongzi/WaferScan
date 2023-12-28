function [Pw] = GetDataFromSTL3(fileName)
%��ȡCADģ�Ͳ�����
% input:
%       xin:�任����ĸ�����xin=[a,b,c,x,y,z]
% output:
%       Pw��[water_path,x,y,z,b,a]
%% get the points on the workpiece coordinate system for Pw
count = floor(csvread(fileName,0,0,[0,0,0,0])/2);
Pw = zeros(count,3); %��������ϵ�ϵĵ�
for i = 1:count
%     i
    row = i * 2;
    %�з�ƽ���з�stlÿһ�е����ݵ�������Ҫ���ļ��ж�ȡ
    Pw(i,:) = csvread(fileName,row,0,[row,0,row,2]);
end

end
