function [ span ] = FindSpan( n,p,u,U )
%   算法A2.1
%   确定参数u所在的节点区间的下标
%   input:
%       n:控制点的个数为n+1
%       p:基函数的阶次，p次,p+1阶
%       u:查考的位置u
%       U:节点序列 U={0...0,a1,a2...an-1,1...1}
%         个数为m+1,m = n+p+1,在MATLAB中第一个元素下标为1
%   output:
%       span:输出u所在的节点区间的下标
    if u>1
        u = 1;
    elseif u<0
        u = 0;
    end
    if(u == U(n+2)) %原来为 u==U(n+2)
        span = n+1;
        return;
    end
    low = p;
    high = n+2;
    mid = floor((low+high)/2);
    while((u<U(mid))||(u>=U(mid+1)))
        if(u<U(mid))
            high = mid;
        else
            low = mid;
        end
        mid = floor((low+high)/2);
    end
    span = mid;
end

