function [ p1,m,U,P,Nu,uk0,Q1] = GlobalCurveInterp_C2( n,Q,p,D0,Dn,A0,An)
%对给定的n+1个点进行全局曲线插值
%指定首尾端点的一阶导矢D0,Dn，二阶导矢A0,An
%   input:
%       n:插值点的个数为：n+1
%       Q:n+1个插值点,为(n+1)*Qk的矩阵,Qk为点的维度
%       p:B样条基函数的阶次，p次（p+1阶）,最低要求：P>=2
%       D0:首端点一阶导矢，模大小不同，会对曲线产生影响
%       Dn:末端点一阶导矢，模大小不同，会对曲线产生影响
%       A0:首端点二阶导矢，模大小不同，会对曲线产生影响
%       An:末端点二阶导矢，模大小不同，会对曲线产生影响
%   output:
%       p1:基函数的阶数，p1=p
%       m:节点矢量节点个数，m+1，不是控制点个数，m = n+p+5
%       U:节点矢量
%       P:控制点，为1x(n+1)的矩阵，个数为n+1

    [~,Qn] = size(Q);%Qn为Q点的维度,即坐标分量的个数，不是点的个数,a = n + 1
    p1 = p;
    m = n + p + 5;%两个导矢会导致多增加两个节点
%% 确定uk，uk为与Q对应的参数值 9.5式或9.6式
    uk = zeros(1,n+1);
    temp = zeros(1,n);
    d = 0;
    for i = 1:n
        %n+1个插值点，由有n个线段需要累加
        temp(i) = 0;
        for j = 1:Qn
            temp(i) = temp(i) + (Q(i,j)-Q(i+1,j))^2; %即相临点之间的距离
        end
        temp(i) = sqrt(temp(i));
        d = d + temp(i);
    end
    uk(1) = 0;
    for i = 2:n+1
        uk(i) = uk(i-1)+temp(i-1)/d;%利用式9.5
    end
%% 确定节点矢量U
    U = zeros(1,m+1);
    uk0 = zeros(1,n+3); %构造一个uk0，即将uk向两头扩展一个，一头为0，一头为1
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
%% 建立系数矩阵
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
%% 重构插值点
    Q1 = zeros(n+5,Qn);
    Q1(1,:) = Q(1,:);
    Q1(2,:) = D0;
    Q1(3,:) = A0;  
    %不加这个系数2，会导致计算出来的曲线的二阶导矢是指定的1/2，真是奇怪 
    Q1(4:n+2,:) = Q(2:n,:);
    Q1(n+3,:) = An;
    Q1(n+4,:) = Dn;
    Q1(n+5,:) = Q(n+1,:);
%% 利用高斯算法求解控制点
   P = zeros(n+5,Qn);
    for i = 1:Qn
        P(:,i) = gauss(Nu,Q1(:,i));
    end
end

