function [ ders ] = DersBasisFuns( span,u,p,n,U )
%计算非零B样条基函数及其导数
%说明：目前可以正确地计算出1阶导数
%   input:
%       span:为u在U中的节点区间的下标span,U在MATLAB中的下标从1开始
%       u:待考查点的参数
%       p:B样条曲线阶次，p次p+1阶
%       n:控制点个数n+1
%       U:节点矢量，个数为m+1,m = n+p+1
%   output:
%       ders:ders(k,j)存储的是基函数N(span-p+j,p)的k阶导数
%                     每一行表示某一阶的基函数，每一列表示某一基函数的各阶导数
    if u>1
        u = 1;
    elseif u<0
        u = 0;
    end
   
    ndu = zeros(p+1,p+1);%见P52第一个表，载入的是p次基函数
    ndu(1,1) = 1;
    left = zeros(1,p+1);
    right = zeros(1,p+1);
    for j = 1:p
        left(j) = u - U(span+1-j);%
        right(j) = U(span+j) - u;
        saved = 0;
        for r = 0:j-1
            ndu(j+1,r+1) = right(r+1) + left(j-r);%将节点之差置于下三角部分,实现
            temp = ndu(r+1,j)/ndu(j+1,r+1);
            ndu(r+1,j+1) = saved + right(r+1)*temp;%将B样条基函数置于数组的上三角部分
            saved = left(j-r)*temp;
        end
        ndu(j+1,j+1) = saved;%ndu保存了有效的非零各阶基函数
    end
    
    ders = zeros(p+1,p+1);  %zeros(n+1,p+1)
    for j = 0:p  %载入基函数的值,ders第一行为0阶导数
        ders(1,j+1) = ndu(j+1,p+1);
    end
    
%以上程序经过检验，没有问题，问题主要发生在下面%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%按书上例子，例2.1和例2.2，一阶和二阶导数都计算正确，可以认为程序没问题了
    %下面计算导数，根据式2.9
    for r = 0:p %对函数的下标进行循环
        s1 = 0;
        s2 = 1; %改变数组a的行
        a = zeros(2,p+1);
        a(1,1) = 1;
        %循环计算k阶导数，k=1,2,...,n
        for k =1:p      %1:n
            d = 0;
            rk = r-k;
            pk = p-k;
            if(r>=k)
                a(s2+1,1) = a(s1+1,1)/ndu(pk+2,rk+1);
                d = a(s2+1,1)*ndu(rk+1,pk+1);
            end
            
            if(rk>=-1)
                j1 = 1;
            else
                j1 = -rk;
            end
            
            if(r-1<=pk)
                j2 = k-1;
            else
                j2 = p-r;
            end
            
            for j = j1:j2
                a(s2+1,j+1) = (a(s1+1,j+1)-a(s1+1,j))/ndu(pk+2,rk+j+1);%此处的下标需要注意
                d = d + a(s2+1,j+1)*ndu(rk+j+1,pk+1);
            end
            if(r<=pk)
                a(s2+1,k+1) = -a(s1+1,k)/ndu(pk+2,r+1);
                d = d + a(s2+1,k+1)*ndu(r+1,pk+1);
            end
            ders(k+1,r+1) = d;
            j = s1;
            s1 = s2;
            s2 = j;
        end
    end
    %对结果乘以正确的因子，根据2.9式,该部分正确
    r = p;
    for k = 1:p         %1:n
        for j =0:p
            ders(k+1,j+1) = ders(k+1,j+1)*r;
        end
        r = r*(p-k);
    end    
end

