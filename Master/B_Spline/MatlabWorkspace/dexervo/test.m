clear
clc
format long
%% ����
%λ������
    Ss = 100;
    Se = 101;
%�ٶ�����
    Vs = 50;
    Ve = 5;
%�����ٶ�������ٶ�����
    Am = 500;
    Vm = 100;
%�����˶�ʱ������
    t = 0.3;
 %% ����   
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
%% ��ͼ��
subplot(3,1,1)
plot(1:n,Stable,'r')
xlabel('λ��')

subplot(3,1,2)
plot(1:n-1,Vtable,'r')
xlabel('�ٶ�')

subplot(3,1,3)
plot(1:n-2,Atable,'r')
xlabel('���ٶ�')