function [ error ] = errorDistance( Ps,Pw )
%�������Pw�����еĵ�p������Ps����������ƽ����
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
            dis = min(dis,dis_temp); %ȡ��Сֵ
        end
    end
    error = error + dis;
end

end

