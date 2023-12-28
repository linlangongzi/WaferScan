function [ error ] = errorDistance2( Ps,Pw,axisPara )
%点云Pw经A(x)变换后得到点云Pw1，计算Pw1上所有的点p到点云Ps的最近距离的平方和

%% 坐标变换
a = axisPara(1);
b = axisPara(2);
c = axisPara(3);
x = axisPara(4);
y = axisPara(5);
z = axisPara(6);
T = [1 0 0 x;...
     0 1 0 y;...
     0 0 1 z;...
     0 0 0 1];
 Ra = [1,   0,        0,        0;...
       0,   cosd(a),  sind(a),  0;...
       0,   -sind(a), cosd(a),  0;...
       0    0         0         1];
 Rb = [cosd(b),  0,  sind(b), 0;...
       0        1   0       0;...
       -sind(b), 0,  cosd(b), 0;...
       0        0   0       1];
 Rc = [cosd(c),   sind(c),  0, 0;...
       -sind(c),  cosd(c),  0, 0;...
       0,         0,        1, 0;...
       0,         0,        0, 1];
 A = Ra*Rb*Rc*T;
 Pw1 = (A*Pw')'; %将Pw变换成Pw1

 %% 求偏差的平方和
[n1,dimention] = size(Ps);
[n2,~] = size(Pw1);
error = 0;
for k = 1:n2
    for i = 1:n1
        dis_temp = 0;
        for j = 1:dimention
            dis_temp = dis_temp + (Pw1(k,j) - Ps(i,j))^2;
        end
        if i == 1
            dis = dis_temp;
        else
            dis = min(dis,dis_temp); %取较小值
        end
    end
    error = error + dis;
end

end

