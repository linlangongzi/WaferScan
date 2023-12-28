function [ CK ] = CurveDerivsAlg1( n,p,U,P,u,d )
%计算曲线上的d阶导矢，见算法A3.2
%这个算法利用A2.1和算法A2.3
%返回数组CK(k)，分别为k阶导矢
%   input:
%       n:控制点个数n+1
%       p:B样条曲线阶次，p次p+1阶
%       U:节点矢量，个数为m+1,m = n+p+1
%       P:控制点，此处是一维点
%       u:待考查点的参数
%       d：d阶导矢
%   output:
%       CK:CK(k)返回k阶导矢，0<=k<=d

    du = min(d,p);
    CK = zeros(1,du+1);
%     for k =p+1:d
%         CK(k) = 0;
%     end
    if u >= 1
        span = n+1;
    else
        span = FindSpan(n,p,u,U);
    end
    nders = DersBasisFuns(span,u,p,du,U);
    for k =0:du
        for j = 0:p
            CK(k+1) = CK(k+1) + nders(k+1,j+1)*P(span-p+j);
            %span是u在U节点区间的下角标，该角标从1开始，因此，此处不需要使用P(span-p+j+1)
        end
    end
    %调整导数大小，保证与实际匹配
%     if du==2    
%         CK(3) = 2*CK(3);%二阶导数
%     end
%     if du==3
%         CK(3) = 2*CK(3);%二阶导数
%         CK(4) = 2*CK(4);%三阶导数
%     end
end

