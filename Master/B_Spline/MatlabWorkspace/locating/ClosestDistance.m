function [ dis ] = closestDistance( Ps,P )
%����һ����p������Ps���������
[nPs,dimention] = size(Ps);
for i = 1:nPs
    dis_temp = 0;
    for j = 1:dimention
        dis_temp = dis_temp + (P(j) - Ps(i,j))^2;
    end
    dis_temp = sqrt(dis_temp); %P��Ps(i)�ľ���
    if i == 1
        dis = dis_temp;
    else
        dis = min(dis,dis_temp); %ȡ��Сֵ
    end
end
end

