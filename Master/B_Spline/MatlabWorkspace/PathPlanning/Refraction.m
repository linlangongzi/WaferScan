function [ L2 ] = Refraction( N1, L1, c1, c2 )
% �����䶨��
% N1Ϊ��ķ�����������,������
% L1Ϊ�����Ĵ�������������
% c1Ϊ��һ���ʵ�����
% c2Ϊ�ڶ����ʵ�����
% L2 = a * N1 + b * L1;
t1= acos(N1' * L1);
t2 = asin(c2 * sin(t1) / c1);

b = c2 / c1;  %��Ȼb>0
% a = cos(t2) - b * cos(t1);
a = sqrt(1 - b^2 + b^2 * (L1'*N1)^2) - b *(L1' * N1);

L2 = a * N1 + b * L1;

end

