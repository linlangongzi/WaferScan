function [ error ] = errorDistance( Ps,Pw )
%计算点云Pw上所有的点p到点云Ps的最近距离的平方和
[nPs1,dimention] = size(Ps);
[nPs2,~] = size(Pw);
error = 0;
for k = 1:nPs2
    for i = 1:nPs1
        dis_temp = 0;
        for j = 1:dimention
            dis_temp = dis_temp + (Pw(k,j) - Ps(i,j))^2;
        end
        if i == 1
            dis = dis_temp;
        else
            dis = min(dis,dis_temp); %取较小值
        end
    end
    error = error + dis;
end

end

