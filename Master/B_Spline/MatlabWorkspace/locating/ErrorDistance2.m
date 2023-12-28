function [ error ] = errorDistance2( Ps,Pw,axisPara )
%����Pw��A(x)�任��õ�����Pw1������Pw1�����еĵ�p������Ps����������ƽ����

%% ����任
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
 Pw1 = (A*Pw')'; %��Pw�任��Pw1

 %% ��ƫ���ƽ����
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
            dis = min(dis,dis_temp); %ȡ��Сֵ
        end
    end
    error = error + dis;
end

end

