function [Pw] = GetDataFromSTL2(fileName)
%��ȡ����������
% input:
%       xin:�任����ĸ�����xin=[a,b,c,x,y,z]
% output:
%       Pw��[water_path,x,y,z,b,a,w]
%% get the points on the workpiece coordinate system for Pw
count = floor(csvread(fileName,0,0,[0,0,0,0])/10);
Pw = zeros(count,8); %��������ϵ�ϵĵ�
for i = 1:count
%     i
    row = i * 10;
    %�з�ƽ���з�stlÿһ�е����ݵ�������Ҫ���ļ��ж�ȡ
    water_path = csvread(fileName,row,0,[row,0,row,1]);
    joints = csvread(fileName,row,2,[row,2,row,6]);
    w = csvread(fileName,row,7,[row,7,row,7]);
    Pw(i,:) = [water_path,joints,w];
end

end
