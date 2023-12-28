function [ p1,m,U,P] = GlobalCurveInterp( n,Q,p)
%�Ը�����n+1�������ȫ�����߲�ֵ
%δָ���˵㵼ʸ��ֻ�ǽ������������䡣�൱�ڵ�ʸΪ0����ˮƽ
%   input:
%       n:��ֵ��ĸ���Ϊ��n+1
%       Q:n+1����ֵ��,Ϊ(n+1)*Qk�ľ���,QkΪ���ά��
%       p:B�����������Ľ״Σ�p�Σ�p+1�ף�
%   output:
%       p1:�������Ľ�����p1=p
%       m:�ڵ�ʸ���ڵ������m+1�����ǿ��Ƶ����
%       U:�ڵ�ʸ��
%       P:���Ƶ㣬Ϊ1x(n+1)�ľ��󣬸���Ϊn+1

    [~,Qn] = size(Q);%QnΪQ���ά��,����������ĸ��������ǵ�ĸ���,a = n + 1
    p1 = p;
    m = n + p + 1;
% ȷ��uk��ukΪ��Q��Ӧ�Ĳ���ֵ 9.5ʽ��9.6ʽ
    uk = zeros(1,n+1);
    temp = zeros(1,n);
    d = 0;
    for i = 1:n
        %n+1����ֵ�㣬����n���߶���Ҫ�ۼ�
        temp(i) = 0;
        for j = 1:Qn
            temp(i) = temp(i) + (Q(i,j)-Q(i+1,j))^2; %�����ٵ�֮��ľ���
        end
        temp(i) = sqrt(temp(i));
        d = d + temp(i);
    end
    uk(1) = 0;
    for i = 2:n+1
        uk(i) = uk(i-1)+temp(i-1)/d;%����ʽ9.5
    end
% ȷ���ڵ�ʸ��U
    U = zeros(1,m+1);
    for i = 1:m+1
        if(i<=p+1)
            U(i) = 0;
        elseif(i>=m-p+1)
            U(i) = 1;
        else
            U(i) = 1/p*sum(uk(i-p:i-1));
        end
    end
% ����ϵ������
    Nu = zeros(n+1,n+1);
    for i = 0:n
        if i == n %��ֹuk(n+1)������ۻ����µ�uk(n+1)>1�����
            span = n+1;
        else
            span = FindSpan(n,p,uk(i+1),U);
        end
        Nu(i+1,span-p:span) = BasisFuns(span,uk(i+1),p,U);   %
    end
% ���ø�˹�㷨�����Ƶ���ܻ�����޷����㣬��˲��ù��������
   P = zeros(n+1,Qn);
    for i = 1:Qn
        P(:,i) = gauss(Nu,Q(:,i));
    end
end

