function [ Pout ] = decrease( Pin )
%ȥ���㷨

[n,~] = size(Pin);
Pout = Pin(1,:);
count = 1;
for i = 1:n
    flag = 0;
    for j = 1:count
        if Pin(i,:) == Pout(j,:)
            flag = 1;%���ظ��ĵ�
        end
    end
    if flag == 0
        Pout = [Pout;Pin(i,:)];
        count = count + 1;
    end
end


end

