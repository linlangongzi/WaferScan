function [ xout,error ] = FRCG( Ps,Pw )
%功能：采用FR共轭梯度法查找误差的极小值，并找到对应的变换矩阵的6个参数
%input:
%       Ps,Pw:机床坐标系下的点云集Ps,工件坐标系下的点云集Pw
%       xin:变量的初始位置,是一个6维向量(a,b,c,x,y,z),直接取zeros(1,6)
%output:
%       xout:误差极小值点对应的变换矩阵的参数xout=[a,b,c,x,y,z]

x1 = zeros(1,6);
s1 = ErrorDistance2(Ps,Pw,x1);
t1 = zeros(1,6); %梯度
t2 = zeros(1,6);
for i = 1:6 %计算梯度艰
    x1 = zeros(1,6);
    e = zeros(1,6);
    e(i) = 1e-3;
    s = ErrorDistance2(Ps,Pw,x1 + e);
    t1(i) = (s - s1)/e(i); %梯度，初始搜索方向
end
count = 0;
count1 = 0;
% t1 = t1 / norm(t1);
d = -t1;
while count1<100 
    [lanpta,s2] = OneDimensionSearch(Ps,Pw,x1,d);
    x2 = x1 + lanpta * d;
%     if abs(s2-s1) < 1e-5 % 当误差低于10时，认为已经足够精确了
%         break;
%     end
    count = count + 1;
    count1 = count1 + 1
    for i = 1:6 %计算梯度
        e = zeros(1,6);
        e(i) = 1e-3;
        s = ErrorDistance2(Ps,Pw,x2 + e);
        t2(i) = (s - s2)/e(i); %梯度，初始搜索方向
    end
%     t2 = t2 / norm(t2);
    if count == 6
        count = 0;
        d = -t2;
    else
%         alfa = t2 * (t2 - t1)' / (t1 * t1') ; %PRP公式
%         alfa = t2 * t2' / (d * t1');           %DM公式,收敛速度最慢
        alfa = t2 * t2' /( t1 * t1');        %FR公式
        d = -t2 + alfa * d;
    end
    x1 = x2;
    s1 = s2
    t1 = t2;
    x2'
end
xout = x2;
error = s2;
end

