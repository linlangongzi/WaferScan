%使用例2.4进行程序验证，验证DersBasisFuns()
% clear;
% clc
p = 2;
U = [0 0 0 1 2 3 4 4 5 5 5]/5;
u = 0.4;
n = 7;
span = FindSpan(n,p,u,U);%为u在U中节点区间的下标
ders = DersBasisFuns(span,u,p,n,U)