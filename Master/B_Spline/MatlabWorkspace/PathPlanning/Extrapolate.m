function [ Pout ] = Extrapolate( P1,P2,L )
%����֪��2�㣬���Ƶ�3�㣬˳��Ϊp1-p2-pout
%ԭ���������
%   input:
%       P1,P2:��Ϊ3ά�㣬[x,y,z]
%       L�����Ƶ����
%   output:
%       Pout:��3�����Ƴ��ĵ�4�㣬[x,y,z]
Direction = P2-P1;
Direction = Direction/norm(Direction);%��һ��
Pout = P2 + L*Direction;
end

