function [ D ] = Distance3D( Q1,Q2 )
%计算两个三维点的距离 
%Q1 = [x1,y1,z1],Q2 = [x2,y2,z2]
    temp = 0;
    for i = 1:3
        temp = temp + (Q1(i)-Q2(i))^2;
    end
    D = sqrt(temp);
end

