function [ N ] = BasisFuns( span,u,p,U )
%	计算参数值u对应的所有非零B样条基函数的值，一直计算到第i个p次B样条
%   input:
%           span:为u在U中的节点区间的下标span,U在MATLAB中的下标从1开始
%           u:考查的位置
%           p:B样条基函数的阶次，p次表示P+1阶
%           U:节点矢量
%    output
%           N:参数值u对应的B样条基函数的值，阶次为p
%             实际上p次样条只有第span-p+1到span+1个是非零的。
%             下标从1开始算。因此只需要输出p+1个基函数的值
    if u>1
        u = 1;
    elseif u<0
        u = 0;
    end
    N = zeros(1,p+1);
    N(1) = 1.0;
    left = zeros(1,p+1);
    right = zeros(1,p+1);
    for j = 1:p
        left(j) = u-U(span+1-j);
        right(j) = U(span+j)-u;
        saved = 0.0;
        for r = 0:j-1
            temp = N(r+1)/(right(r+1)+left(j-r));
            N(r+1) = saved +right(r+1)*temp;
            saved = left(j-r)*temp;
        end
        N(j+1) = saved;
    end        
end

