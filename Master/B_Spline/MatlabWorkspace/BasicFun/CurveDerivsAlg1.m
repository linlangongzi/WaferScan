function [ CK ] = CurveDerivsAlg1( n,p,U,P,u,d )
%���������ϵ�d�׵�ʸ�����㷨A3.2
%����㷨����A2.1���㷨A2.3
%��������CK(k)���ֱ�Ϊk�׵�ʸ
%   input:
%       n:���Ƶ����n+1
%       p:B�������߽״Σ�p��p+1��
%       U:�ڵ�ʸ��������Ϊm+1,m = n+p+1
%       P:���Ƶ㣬�˴���һά��
%       u:�������Ĳ���
%       d��d�׵�ʸ
%   output:
%       CK:CK(k)����k�׵�ʸ��0<=k<=d

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
            %span��u��U�ڵ�������½Ǳ꣬�ýǱ��1��ʼ����ˣ��˴�����Ҫʹ��P(span-p+j+1)
        end
    end
    %����������С����֤��ʵ��ƥ��
%     if du==2    
%         CK(3) = 2*CK(3);%���׵���
%     end
%     if du==3
%         CK(3) = 2*CK(3);%���׵���
%         CK(4) = 2*CK(4);%���׵���
%     end
end

