clear
clc
format long
%% 设置
%位移设置
    Ss = 100;
    Se = 101;
%速度设置
    Vs = 50;
    Ve = 5;
%最大加速度与最大速度设置
    Am = 500;
    Vm = 100;
%期望运动时间设置
    t = 0.3;
 %% 计算   
[flag, n,Stable ] = TMove2( Ss,Se,Vs,Ve,Am,Vm,t );
flag
n
T = 1e-3;
Vtable = zeros(1,n-1);
for i = 1:n-1
    Vtable(i) = ( Stable(i+1) - Stable(i) )/ T;
end
Atable = zeros(1,n-2);
for i = 1:n-2
    Atable(i) = ( Vtable(i+1) - Vtable(i) ) / T;
end
S1 = Stable(1)
S2 = Stable(n)
%% 画图区
subplot(3,1,1)
plot(1:n,Stable,'r')
xlabel('位移')

subplot(3,1,2)
plot(1:n-1,Vtable,'r')
xlabel('速度')

subplot(3,1,3)
plot(1:n-2,Atable,'r')
xlabel('加速度')