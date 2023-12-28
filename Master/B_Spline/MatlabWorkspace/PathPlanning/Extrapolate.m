function [ Pout ] = Extrapolate( P1,P2,L )
%由已知的2点，外推第3点，顺序为p1-p2-pout
%原理：线性拟合
%   input:
%       P1,P2:均为3维点，[x,y,z]
%       L：外推的深度
%   output:
%       Pout:由3点外推出的第4点，[x,y,z]
Direction = P2-P1;
Direction = Direction/norm(Direction);%归一化
Pout = P2 + L*Direction;
end

