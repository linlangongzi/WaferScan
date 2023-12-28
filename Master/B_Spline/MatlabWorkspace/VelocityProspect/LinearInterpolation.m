function [ y ] = LinearInterpolation( xOrder,yOrder,x )
%�����x��Ӧ��yֵ
%xOrder,yOrderΪ������ͬ�ĵ����У���һһ��Ӧ��ϵ
num = length(xOrder);
low = 1;
high = num;
%% ���ֲ��ҷ�
while  high - low >1 %��high - low = 1ʱ�˳�
    mid = floor((low + high)/2);
    if x<=xOrder(mid)
        high = mid;
    else
        low = mid;
    end
end
%% ���²��ò�ֵ�����㷨
%��ֵ�����㷨ʱ��ʱ�������ȶ�����˲���ȡ
% while high > low 
%     mid = floor(low + (x - xOrder(low)) / (xOrder(high) - xOrder(low)) * (high - low));
%     if x < xOrder(mid-1)
%           high = mid;
%     elseif x > xOrder(mid+1)
%           low = mid +1 ;
%     else
%         break; 
%     end
%     count = count + 1;
% end
%%
y = yOrder(low) + (x-xOrder(low)) /(xOrder(high)-xOrder(low)) * (yOrder(high)-yOrder(low));
end

