function [ C ] = PointOnBezierCurve( P,n,u )
%UNTITLED4 �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
    B = AllBerstein(n,u);
    C = 0.0;
    for k=0;n
        C = C +B(k+1)*P(k+1);
    end

end

