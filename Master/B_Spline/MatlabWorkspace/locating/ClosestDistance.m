function [ dis ] = closestDistance( Ps,P )
%计算一个点p到点云Ps的最近距离
[nPs,dimention] = size(Ps);
for i = 1:nPs
    dis_temp = 0;
    for j = 1:dimention
        dis_temp = dis_temp + (P(j) - Ps(i,j))^2;
    end
    dis_temp = sqrt(dis_temp); %P与Ps(i)的距离
    if i == 1
        dis = dis_temp;
    else
        dis = min(dis,dis_temp); %取较小值
    end
end
end

