function [ C ] = CurvePoint( n,p,U,P,u )
%计算p次B样条曲线在u处的插值。
%算法A3.1,给定控制点、节点矢量、基函数阶次
%   input:
%       n:控制节点个数，即控制点P中点的个数为：n+1
%       p:B样条基函数的阶次，p次表示P+1阶
%       U：节点矢量
%       P：控制点矩阵
%       u：待计算的参数
%   output:
%       C:曲线在参数u处的值，插值结果
%若想获得整条曲线，只需要把所有参数u计算一遍即可
    span = FindSpan(n,p,u,U);
%   N = zeros(1,p+1);
    N = BasisFuns(span,u,p,U);%实际上N是从0～n,这里只是取了span-p~span这p+1个，其它为0
    C = 0;
    for i = 0:p
        C = C +N(i+1)*P(span-p+i);
    end

end

