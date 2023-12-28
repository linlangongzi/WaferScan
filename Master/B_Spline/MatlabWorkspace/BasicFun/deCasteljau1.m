function [ C ] = deCasteljau1( P,n,u )
%   input:
%           P:曲线控制点集,个数为n+1个
%           n:基函数阶次，基函数为bernstein基函数
%           u:考查点，0～1
%   output:
%           C:考查点对应的插值
    Q = zeros(1,n+1);
    for i = 0:n
        Q(i+1) = P(i+1);
    end
    for k = 1:n
        for i = 0:n-k
            Q(i+1) = (1.0 - u)*Q(i+1) + u*Q(i+2);
        end
        C = Q(1);
    end
end

