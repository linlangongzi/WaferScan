%指定初速度、末速度、初加速度、末加速度，路径长度L。
%做5次多项式速度规划
clear ;
clc;

Vs = 3.225675000000001e+02;
As = 875;
Am = 4000;
Lleft =  1.657009041629068e+02;
Ve = 0;
Ae = 0;
Ttotal = Lleft/Vs*1.3;
% T = 1;
Nt1 = [Ttotal^5     Ttotal^4     Ttotal^3    Ttotal^2  Ttotal  1;...
       5*Ttotal^4   4*Ttotal^3   3*Ttotal^2  2*Ttotal  1       0;...
       20*Ttotal^3  12*Ttotal^2  6*Ttotal    2         0       0;...
       0            0            0           2         0       0;...
       0            0            0           0         1       0;...
       0            0            0           0         0       1];
BoundaryCondition = [Lleft,Ve,Ae,As,Vs,0]';
%s = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F;
PolyCoefficient = gauss(Nt1,BoundaryCondition);
A = PolyCoefficient(1);
B = PolyCoefficient(2);
C = PolyCoefficient(3);
D = PolyCoefficient(4);
E = PolyCoefficient(5);
F = PolyCoefficient(6);

SPolyCoefficient = [A,B,C,D,E,F];
VPolyCoefficient = [0,5*A,4*B,3*C,2*D,E];
APolyCoefficient = [0,0,20*A,12*B,6*C,2*D];

T = 1e-3;
t = 0:T:Ttotal;
k = length(t);
Ttest = [t.^5;t.^4;t.^3;t.^2;t;linspace(1,1,k)];
Stest = SPolyCoefficient*Ttest;
Vtest = VPolyCoefficient*Ttest;
Atest = APolyCoefficient*Ttest;
for i = 1:k-1
    V(i) = (Stest(i+1)-Stest(i))/T;
end
for i = 1:k-2
    A(i) = (V(i+1)-V(i))/T;
end

ax(1) = subplot(3,1,1);
plot(1:k,linspace(Lleft,Lleft,k),'r');
hold on;
plot(1:k,Stest,'b');
xlabel('位移曲线')

ax(2) = subplot(3,1,2);
plot(1:k,linspace(0,0,k),'r')
hold on;
plot(1:k,Vtest,'b');
% plot(1:k-1,V,'r');
xlabel('速度曲线')

ax(3) = subplot(3,1,3);
plot(1:k,linspace(0,0,k),'r')
hold on;
plot(1:k,linspace(Am,Am,k),'r')
plot(1:k,linspace(-Am,-Am,k),'r')
plot(1:k,Atest,'b');
% plot(1:k-2,A,'r')
xlabel('加速度曲线')

Smax = max(Stest)
Vmin = min(Vtest)
Amax = max(abs(A))
linkaxes(ax,'x')