function [flag, n,Stable ] = TMove1( Ss,Se,Vs,Ve,Am,Vm )
%指定路程及期望速度的T形速度规划，总时间待定

S = Se-Ss;%位移差
T = 1e-3;%插补周期为1ms

%% 情况1 加速-减速
if S>0 && (2*Vm^2-Vs^2-Ve^2)/(2*Am) >= S 
    flag = 1;
    V1 = sqrt(Am*S + 0.5*Vs^2 + 0.5*Ve^2);
    t1 = (V1 - Vs) / Am;
    t2 = (2*V1 - Vs - Ve) / Am;
%     table = T:T:t2;
    n = floor(t2/T);
    Stable = zeros(1,n);
    t = 0;
    for i = 1:n
        t = t + T;
        if t<=t1
            Stable(i) = Ss + Vs * t + 0.5 * Am * t^2;
        else
            Stable(i) = ( Ss + Vs * t1 + 0.5 * Am * t1^2 ) +...
                        V1 * (t - t1) -...
                        0.5 * Am * (t - t1)^2;
        end
    end
%% 情况2 加速-匀速-减速
elseif S>0 && (2*Vm^2-Vs^2-Ve^2)/(2*Am) < S
    flag = 2;
    t1 = (Vm - Vs) / Am;
    t2 = t1 + ( S - (2*Vm^2 - Vs^2 - Ve^2) / (2*Am) ) / Vm;
    t3 = t2 + (Vm - Ve) / Am;
    n = floor(t3/T);
    Stable = zeros(1,n);
    t = 0;
    for i = 1:n
        t = t + T;
        if t<=t1
            Stable(i) = Ss + Vs * t + 0.5 * Am * t^2;
        elseif t>t1 && t<=t2
            Stable(i) = (Ss + Vs * t1 + 0.5 * Am * t1^2) + Vm * (t - t1);
        else
            Stable(i) = (Ss + Vs * t1 + 0.5 * Am * t1^2) + Vm * (t2 - t1) + ...
                        Vm * (t - t2) -...
                        0.5 * Am * (t - t2)^2;
        end
    end
%% 情况3 减速-加速    
elseif S<0 && (2*Vm^2-Vs^2-Ve^2)/(2*Am) > -S
    flag = 3;
    V1 = -sqrt(-Am*S - 0.5*Vs^2 - 0.5*Ve^2);
    t1 = (Vs - V1) / Am;
    t2 = (-2*V1 + Vs + Ve) / Am;
%     table = T:T:t2;
    n = floor(t2/T);
    Stable = zeros(1,n);
    t = 0;
    for i = 1:n
        t = t + T;
        if t<=t1
            Stable(i) = Ss + Vs * t - 0.5 * Am * t^2;
        else
            Stable(i) = ( Ss + Vs * t1 - 0.5 * Am * t1^2 ) +...
                        V1 * (t - t1) +...
                        0.5 * Am * (t - t1)^2;
        end
    end
%% 情况4 减速-匀速-加速
elseif S<0 && (2*Vm^2-Vs^2-Ve^2)/(2*Am) <= -S
    flag = 4;
    t1 = (Vs + Vm) / Am;
    t2 = t1 + ( S + (2*Vm^2 - Vs^2 - Ve^2) / (2*Am) ) / (-Vm);
    t3 = t2 + (Ve + Vm) / Am;
    n = floor(t3/T);
    Stable = zeros(1,n);
    t = 0;
    for i = 1:n
        t = t + T;
        if t<=t1
            Stable(i) = Ss + Vs * t - 0.5 * Am * t^2;
        elseif t>t1 && t<=t2
            Stable(i) = (Ss + Vs * t1 - 0.5 * Am * t1^2) - Vm * (t - t1);
        else
            Stable(i) = (Ss + Vs * t1 - 0.5 * Am * t1^2) - Vm * (t2 - t1) - ...
                        Vm * (t - t2) +...
                        0.5 * Am * (t - t2)^2;
        end
    end
else
    disp('规划位移太短')
    error('plan fails')
end    
 
end

