%%
%S形速度规划，将速度和加速度降到0，对位置/时间无要求
%%
clear
clc
Ttest = 1*1e-3;
Am = 1e3;
Jm = 2e4;
Vmax2S = Am^2/Jm;%%% Am = 1e3,Jm = 2e4 --> Vmax2S = 50;

count = 1;
V = zeros(1,1);
A = zeros(1,1);
J = zeros(1,1);

Vcorr = zeros(1,1);
Jcorr = zeros(1,1);

Vs1 = 1000;
Vs1corr = Vs1;
As1 = 200;
Js1 = -2e4;


%%%%%%%%%%%%%%%%%%%%%%%%%%% 自加测试
% counttest = 1;
% Vtest = zeros(1,1);
% Atest = zeros(1,1);
% Jtest = zeros(1,1);
% Vs1test = 500;
% As1test = 200;
% Js1test = -2e4;
%%%%%%%%%%%%%%%%%%%%%%%%%%%


while 1
    if Js1 == -Jm
        if Vs1>=(2*Am^2-As1^2)/(2*Jm) && As1>=-Am
            Js1 = -Jm;
        elseif Vs1>As1^2/(2*Jm) && Vs1<Am^2/(2*Jm)
            Js1 = -Jm;          
        elseif Vs1 <= As1^2/(2*Jm)
            Js1 = Jm;
        end   
    end
    
    if Js1==Jm && As1 >0
        break;
    end

    As1 = As1 + Js1*Ttest;
        %%% 加速度限制
    if As1<-Am
        As1 = -Am;
        Jcorr(count) = 0 ; %%% 测试改动
    else
        Jcorr(count) = Js1;%%% 测试改动
    end
    
    Vs1 = Vs1 + As1*Ttest+0.5 * Js1 * Ttest^2; %%% ? Js1 ? Jm
    Vs1corr = Vs1corr + As1 * Ttest + 0.5 * Jcorr(count) * Ttest^2;%%% 测试改动
    V(count) = Vs1;
    Vcorr(count) = Vs1corr;%%% 测试改动
    A(count) = As1;
    J(count) = Js1;
    count = count + 1;
end

forseeV = zeros(3,count-1);
forseeV(1,:) = Vcorr;
forseeV(2,:) = V;
forseeV(3,:) = V - Vcorr;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 自加测试
% while Vs1test > 0
%     
%    if As1test>0
%        if Js1test>0 
%            break;
%        else
%            Js1test = -Jm;
%        end
%    elseif  Vs1test > As1test^2/(2 *Jm)
%        Js1test = -Jm;
%    else
%        Js1test = Jm;
%    end
% 
%    As1test = As1test + Js1test * Ttest;
% 
%    if As1test < -Am
%        As1test = -Am;
%    end
%    if As1test >Am
%        As1test = Am;
%    end
%    
%    Vs1test = Vs1test + As1test*Ttest+0.5 * Js1test * Ttest^2; 
%    
%    Vtest(counttest) = Vs1test;
%    Atest(counttest) = As1test;
%    Jtest(counttest) = Js1test;
%    counttest = counttest + 1;
% end
% cmax = max(count,counttest);
% 
% forseeV = zeros(2,cmax);
% forseeV(1,1:count-1) = V;
% forseeV(2,1:counttest-1) = Vtest;
% 
% forseeA = zeros(2,cmax);
% forseeA(1,1:count-1) = A;
% forseeA(2,1:counttest-1) = Atest;
% 
% forseeJ = zeros(2,cmax);
% forseeJ(1,1:count-1) = J;
% forseeJ(2,1:counttest-1) = Jtest;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 绘图区
Vs1
As1
subplot(3,1,1)
plot(1:count-1,linspace(0,0,count-1),'g');

hold on
plot(1:count-1,V','.','Color','r')
plot(1:count-1,Vcorr','x','Color','b')
xlabel('速度曲线')

subplot(3,1,2)
plot(1:count-1,A','.','Color','r')
%plot(1:counttest-1,Atest','x','Color','b')

hold on
plot(1:count-1,linspace(Am,Am,count-1),'g');
plot(1:count-1,linspace(-Am,-Am,count-1),'g');
plot(1:count-1,linspace(0,0,count-1),'g');
xlabel('加速度曲线')

subplot(3,1,3)
plot(1:count-1,J','.','Color','r')
plot(1:count-1,Jcorr','x','Color','b')
xlabel('加加速度曲线')