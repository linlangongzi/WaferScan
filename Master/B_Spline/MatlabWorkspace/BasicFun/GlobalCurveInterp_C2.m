function [ p1,m,U,P,Nu,uk0,Q1] = GlobalCurveInterp_C2( n,Q,p,D0,Dn,A0,An)
%�Ը�����n+1�������ȫ�����߲�ֵ
%ָ����β�˵��һ�׵�ʸD0,Dn�����׵�ʸA0,An
%   input:
%       n:��ֵ��ĸ���Ϊ��n+1
%       Q:n+1����ֵ��,Ϊ(n+1)*Qk�ľ���,QkΪ���ά��
%       p:B�����������Ľ״Σ�p�Σ�p+1�ף�,���Ҫ��P>=2
%       D0:�׶˵�һ�׵�ʸ��ģ��С��ͬ��������߲���Ӱ��
%       Dn:ĩ�˵�һ�׵�ʸ��ģ��С��ͬ��������߲���Ӱ��
%       A0:�׶˵���׵�ʸ��ģ��С��ͬ��������߲���Ӱ��
%       An:ĩ�˵���׵�ʸ��ģ��С��ͬ��������߲���Ӱ��
%   output:
%       p1:�������Ľ�����p1=p
%       m:�ڵ�ʸ���ڵ������m+1�����ǿ��Ƶ������m = n+p+5
%       U:�ڵ�ʸ��
%       P:���Ƶ㣬Ϊ1x(n+1)�ľ��󣬸���Ϊn+1

    [~,Qn] = size(Q);%QnΪQ���ά��,����������ĸ��������ǵ�ĸ���,a = n + 1
    p1 = p;
    m = n + p + 5;%������ʸ�ᵼ�¶����������ڵ�
%% ȷ��uk��ukΪ��Q��Ӧ�Ĳ���ֵ 9.5ʽ��9.6ʽ
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
%% ȷ���ڵ�ʸ��U
    U = zeros(1,m+1);
    uk0 = zeros(1,n+3); %����һ��uk0������uk����ͷ��չһ����һͷΪ0��һͷΪ1
    uk0(2:n+2) = uk;
    uk0(n+3) = 1;
    for i = 1:m+1
        if(i<=p+1)
            U(i) = 0;
        elseif(i>=m-p+1)
            U(i) = 1;
        else
            U(i) = 1/p * sum( uk0(i-p-1:i-2) );  
        end
    end
%% ����ϵ������
    Nu = zeros(n+5,n+5);
    span = FindSpan(n+4,p,uk(1),U);
    Nu(1,span-p:span) = BasisFuns(span,uk(1),p,U);   %
    Nu(2,1:2) = p / U(p+1+1) * [-1,1];%D0 = 
    Nu(3,1:3) = p*(p-1)/U(p+1+1) * [ 1/U(p+1+1),-(U(p+1+1)+U(p+2+1)) / (U(p+1+1)*U(p+2+1)),1/U(p+2+1) ];%A0 = 
    for i = 1:n-1
        span = FindSpan(n+4,p,uk(i+1),U);
        Nu(i+3,span-p:span) = BasisFuns(span,uk(i+1),p,U);   %
    end
    Nu(n+3,n+3:n+5) = p*(p-1)/(1-U(m-p-1+1)) * [ 1/(1-U(m-p-2+1)),...
                     -(2-U(m-p-1+1)-U(m-p-2+1)) / ((1-U(m-p-1+1))*(1-U(m-p-2+1))),...
                     1/(1-U(m-p-1+1)) ];%An = 
    Nu(n+4,n+4:n+5) = p / (1-U(m-p-1+1)) * [-1,1]; %Dn = 
    span = FindSpan(n+4,p,uk(n+1),U);
    Nu(n+5,span-p:span) = BasisFuns(span,uk(n+1),p,U);   %
%% �ع���ֵ��
    Q1 = zeros(n+5,Qn);
    Q1(1,:) = Q(1,:);
    Q1(2,:) = D0;
    Q1(3,:) = A0;  
    %�������ϵ��2���ᵼ�¼�����������ߵĶ��׵�ʸ��ָ����1/2��������� 
    Q1(4:n+2,:) = Q(2:n,:);
    Q1(n+3,:) = An;
    Q1(n+4,:) = Dn;
    Q1(n+5,:) = Q(n+1,:);
%% ���ø�˹�㷨�����Ƶ�
   P = zeros(n+5,Qn);
    for i = 1:Qn
        P(:,i) = gauss(Nu,Q1(:,i));
    end
end

