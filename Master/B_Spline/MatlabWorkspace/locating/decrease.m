function [ Pout ] = decrease( Pin )
%去重算法

[n,~] = size(Pin);
Pout = Pin(1,:);
count = 1;
for i = 1:n
    flag = 0;
    for j = 1:count
        if Pin(i,:) == Pout(j,:)
            flag = 1;%有重复的点
        end
    end
    if flag == 0
        Pout = [Pout;Pin(i,:)];
        count = count + 1;
    end
end


end

