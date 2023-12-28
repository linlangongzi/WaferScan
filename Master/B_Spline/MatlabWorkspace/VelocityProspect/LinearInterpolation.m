function [ y ] = LinearInterpolation( xOrder,yOrder,x )
%查表法找x对应的y值
%xOrder,yOrder为个数相同的点序列，成一一对应关系
num = length(xOrder);
low = 1;
high = num;
%% 二分查找法
while  high - low >1 %当high - low = 1时退出
    mid = floor((low + high)/2);
    if x<=xOrder(mid)
        high = mid;
    else
        low = mid;
    end
end
%% 以下采用插值查找算法
%插值查找算法时快时慢，不稳定，因此不可取
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

