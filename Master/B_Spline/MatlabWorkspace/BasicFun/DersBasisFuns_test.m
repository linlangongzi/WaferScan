%ʹ����2.4���г�����֤����֤DersBasisFuns()
% clear;
% clc
p = 2;
U = [0 0 0 1 2 3 4 4 5 5 5]/5;
u = 0.4;
n = 7;
span = FindSpan(n,p,u,U);%Ϊu��U�нڵ�������±�
ders = DersBasisFuns(span,u,p,n,U)