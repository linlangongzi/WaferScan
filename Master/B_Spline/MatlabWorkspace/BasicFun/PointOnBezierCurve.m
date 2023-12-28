function [ C ] = PointOnBezierCurve( P,n,u )
%UNTITLED4 此处显示有关此函数的摘要
%   此处显示详细说明
    B = AllBerstein(n,u);
    C = 0.0;
    for k=0;n
        C = C +B(k+1)*P(k+1);
    end

end

