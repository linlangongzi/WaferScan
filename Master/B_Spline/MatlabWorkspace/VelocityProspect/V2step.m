function [ Vout ] = V2step( Vin )
%将连续的速度曲线调整成梯形
n = length(Vin);
k = 0.8;
Vout = 1*Vin;
ns = 1;
for i = 2:n-1
    if Vin(i-1)<Vin(i) && Vin(i+1) <= Vin(i) %再次到达制高点起点
%         disp('制高点起点')
%         disp(i)
        for j = ns:i-floor((i-ns)/3*2)
            Vout(j,1) = Vin(ns,1)*k;
        end 
        ns = i;
    end
    if Vin(i-1) <= Vin(i) && Vin(i+1)<Vin(i)  %制高点终点
%         disp('制高点终点');
%         disp(i)
        ns = i;
    end
    if Vin(i-1) >= Vin(i) && Vin(i+1) > Vin(i)  %最低点终点
%         disp('制低点终点')
%         disp(i)
        for j = i-floor((i-ns)/3):i
            
            Vout(j,1) = Vin(i,1)*k;
        end
        ns = i;
    end

end

