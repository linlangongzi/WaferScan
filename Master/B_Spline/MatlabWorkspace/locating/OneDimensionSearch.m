function [ lanpta,error ] = OneDimensionSearch( Ps,Pw,xin,d )
% 功能：采用一维搜索，查找最优步长，使得Ps-Pw之间的误差在方向d上达到最小
%input:
%       Ps,Pw:机床坐标系下的点云集Ps,工件坐标系下的点云集Pw
%       x:变量的当前位置,是一个6维向量(a,b,c,x,y,z)
%       d:一维搜索方向,是个6维向量(a,b,c,x,y,z)
%output:
%       lanpta:步长

%% 采用进退法确定搜索区间[a,b]
k = 1; %表示方向，1为正向，-1为反向
h = 1/norm(d); %试探步长,共轭梯度法系数用0.2
lanpta1 = 0 - k * h ;
lanpta2 = lanpta1 + k * h; % ==0
lanpta3 = lanpta2 + k * h;
%确定搜索方向
if ErrorDistance2(Ps,Pw,xin + 0 * d) > ErrorDistance2(Ps,Pw,xin + h * d)
    k = 1;%正向
elseif  ErrorDistance2(Ps,Pw,xin + 0 * d) > ErrorDistance2(Ps,Pw,xin - h * d)
    k = -1;%反向
end

while 1
    if ErrorDistance2(Ps,Pw,xin + lanpta2 * d) < ErrorDistance2(Ps,Pw,xin + lanpta1 * d) &&...
       ErrorDistance2(Ps,Pw,xin + lanpta2 * d) < ErrorDistance2(Ps,Pw,xin + lanpta3 * d)
        a = lanpta1;
        b = lanpta3;
        break;
    else
        lanpta1 = lanpta1 + k * h; %区间平移
        lanpta2 = lanpta1 + k * h;
        lanpta3 = lanpta2 + k * h;
    end
end

%% 采用0.618法或Fibonacci法查找最优步长
while b - a > 1e-5
    a1 = a + (1-0.618) * (b-a);
    b1 = a + 0.618 * (b-a);
    if ErrorDistance2(Ps,Pw,xin + a1 * d) < ErrorDistance2(Ps,Pw,xin + b1 * d)
        b = b1;
        %a不变
    elseif ErrorDistance2(Ps,Pw,xin + a1 * d) >= ErrorDistance2(Ps,Pw,xin + b1 * d)
        a = a1;
        %b不变
    end
end
lanpta = (a+b) / 2;
error = ErrorDistance2(Ps,Pw,xin + lanpta * d);
end

