function [ p1,m,U,P] = GlobalCurveInterp( n,Q,p)
%对给定的n+1个点进行全局曲线插值
%未指定端点导矢，只是将其向两侧扩充。相当于导矢为0，即水平
%   input:
%       n:插值点的个数为：n+1
%       Q:n+1个插值点,为(n+1)*Qk的矩阵,Qk为点的维度
%       p:B样条基函数的阶次，p次（p+1阶）
%   output:
%       p1:基函数的阶数，p1=p
%       m:节点矢量节点个数，m+1，不是控制点个数
%       U:节点矢量
%       P:控制点，为1x(n+1)的矩阵，个数为n+1

    [~,Qn] = size(Q);%Qn为Q点的维度,即坐标分量的个数，不是点的个数,a = n + 1
    p1 = p;
    m = n + p + 1;
% 确定uk，uk为与Q对应的参数值 9.5式或9.6式
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
% 确定节点矢量U
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
% 建立系数矩阵
    Nu = zeros(n+1,n+1);
    for i = 0:n
        if i == n %防止uk(n+1)因误差累积导致的uk(n+1)>1的情况
            span = n+1;
        else
            span = FindSpan(n,p,uk(i+1),U);
        end
        Nu(i+1,span-p:span) = BasisFuns(span,uk(i+1),p,U);   %
    end
% 利用高斯算法求解控制点可能会产生无法计算，因此采用共轭迭代法
   P = zeros(n+1,Qn);
    for i = 1:Qn
        P(:,i) = gauss(Nu,Q(:,i));
    end
end

