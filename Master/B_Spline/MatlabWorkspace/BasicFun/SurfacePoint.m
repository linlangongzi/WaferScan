function [ S ] = SurfacePoint( n,p,U,m,q,V,P,u,v )
%计算B样条曲面上的点（u,v)
%   input:
%       n：u方向控制点个数，n+1
%       p：u方向B样条基函数阶次，p次p+1阶
%       U：u方向节点矢量
%       m：v方向控制点个数，m+1
%       q：v方向B样条基函数阶次，q次q+1阶
%       V：v方向节点矢量
%       P：二维控制点，(n+1)*(m+1)，实际上只用了(1+p)*(1+q)，未计算基函数N为0的位置
%       u：u方向考查参数
%       v：v方向考查参数
%   output:
%       S：参数点(u,v)处的值
    if u>=1
        uspan = n+1;
    else
        uspan = FindSpan(n,p,u,U);
    end
    Nu = BasisFuns(uspan,u,p,U);%计算参数值u对应的所有非零B样条基函数的值
    if v>=1
        vspan = m+1;
    else
        vspan = FindSpan(m,q,v,V);
    end
    Nv = BasisFuns(vspan,v,q,V);%计算参数值v对应的所有非零B样条基函数的值
    uind = uspan - p;
    temp = zeros(1,q+1);
    S = 0;
    for l = 0:q
        temp(l+1) = 0.0;        %将方向v上的点固定，来寻找u对应的值
        vind = vspan - q +l;
        for k = 0:p
            temp(l+1) = temp(l+1) +Nu(k+1)*P(uind+k,vind);
        end
    end

    for l = 0:q
        S = S + Nv(l+1)*temp(l+1);
    end
end

