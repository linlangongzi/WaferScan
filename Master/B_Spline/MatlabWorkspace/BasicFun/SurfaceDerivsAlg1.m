function [ SKL ] = SurfaceDerivsAlg1( n,p,U,m,q,V,P,u,v,d )
%计算B样条曲面上的点及其所有直到d阶的偏导矢
%在实际工作中只需要各自的1阶偏导矢，即梯度
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
%       d:直到d阶的偏导矢，实际上只需要计算0阶（即曲面值）和1阶
%   output:
%       SKL：SKL[K][L]返回S(u,v)在u,v方向分别求导k次和l次所得的偏导矢，k/l均不大于d
    
    du = min(d,p);%当d>p时，各导矢为0
    dv = min(d,q);
    SKL = zeros(du+1,dv+1);
%下段程序不需要，SKL声明时默认了未赋值的元素均为0 
%     for k = p+1:d
%        for l = 0:d-k
%            SKL(k+1,l+1) = 0;%p+1阶u方向偏导数为0
%        end       
%     end
%     for l = q+1:d
%         for k = 0:d-l
%             SKL(k+1,l+1) = 0;%q+1阶v方向偏导数为0
%         end
%     end
    if u>=1 
        uspan = n+1;
    else
        uspan = FindSpan(n,p,u,U);
    end
    Nu = DersBasisFuns(uspan,u,p,du,U);%第一行为曲线对应的值，第二行为一阶导矢
    if v>=1
        vspan = m+1;
    else
        vspan = FindSpan(m,q,v,V);
    end
    Nv = DersBasisFuns(vspan,v,q,dv,V);%第一行为曲线对应的值，第二行为一阶导矢
    for k = 0:du%相当于固定u
        temp = zeros(1,q+1);
        for s = 0:q
            for r = 0:p
                temp(s+1) = temp(s+1) + Nu(k+1,r+1)*P(uspan-p+r,vspan-q+s);
            end
        end
        dd = min(d-k,dv);
        for l = 0:dd
%             SKL(k+1,l+1) = 0;
            for s = 0:q
                SKL(k+1,l+1) = SKL(k+1,l+1) + Nv(l+1,s+1)*temp(s+1);
            end
        end
    end
end

