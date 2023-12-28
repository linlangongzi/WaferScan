function [ C ] = deCasteljau1( P,n,u )
%   input:
%           P:���߿��Ƶ㼯,����Ϊn+1��
%           n:�������״Σ�������Ϊbernstein������
%           u:����㣬0��1
%   output:
%           C:������Ӧ�Ĳ�ֵ
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

