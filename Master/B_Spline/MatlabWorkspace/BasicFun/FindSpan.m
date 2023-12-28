function [ span ] = FindSpan( n,p,u,U )
%   �㷨A2.1
%   ȷ������u���ڵĽڵ�������±�
%   input:
%       n:���Ƶ�ĸ���Ϊn+1
%       p:�������Ľ״Σ�p��,p+1��
%       u:�鿼��λ��u
%       U:�ڵ����� U={0...0,a1,a2...an-1,1...1}
%         ����Ϊm+1,m = n+p+1,��MATLAB�е�һ��Ԫ���±�Ϊ1
%   output:
%       span:���u���ڵĽڵ�������±�
    if u>1
        u = 1;
    elseif u<0
        u = 0;
    end
    if(u == U(n+2)) %ԭ��Ϊ u==U(n+2)
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

