clear
clc
ku = 1;
kv = 1;
t = 5*ku;%扫查总耗时,即每条线用时5秒
time = linspace(0,t,2*kv*ku);
xx = zeros(1,2*kv*ku);
yy = zeros(1,2*kv*ku);
zz = zeros(1,2*kv*ku);
bb = zeros(1,2*kv*ku);
aa = zeros(1,2*kv*ku);