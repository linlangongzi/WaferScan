%%
%��̬ǰհ������S���ٶȹ滮
%   �ɵ�ǰ�ٶ��������ٶȡ��Ӽ��ٶȣ����ٶȣ������ٵ��㣬�õ�һ������켣�ٶȣ����ٶ����ƱȽ�
%   ���δ���٣�����ٶȡ����ٶȣ��Ӽ��ٶȣ�����Ϊ�����в岹�˶�����֮�������ٶȼ���  
%% �����˶��켣
clear
clc
format long;
ab = 1;
x = ab*[0,200,200,200,400]';
y = ab*[0,0,200,200,200]';
z = ab*[0,0,0,200,200]';
% x = 0:50:1000;
% y = x;
% x = 0.5*[0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600]';
% y = 0.5*[0,100,0,100,0,100,0,100,0,100,0,100,0,100,0,100,0]';
% x = [0,100,200,100,0]';
% y = [0,100,100,100,200]';
n = length(x)-1;%���ݵ����
% z = linspace(0,0,n+1)';
Q = [x,y,z];  %���ݵ㣺QΪ(n+1)*2�ľ���,��ά��(x,y)

%% ���������߶μнǹ�Сʱ����ʾ�޷���ϣ������˶���ʱ
p = 4;  %B�����������״�
D0 = 1*(Q(2,:)-Q(1,:)); %�˵㵼ʸ����Ϊ��һ���ҷ���
Dn = 1*(Q(n+1,:)-Q(n,:)); %�յ㵼ʸ����Ϊ���һ���ҷ���
A0 = D0;
An = Dn;
% [ p1,m,U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
[ p1,m,U,P] = GlobalCurveInterp_C2( n,Q,p,D0,Dn,A0,An);
n = n + 4;%ʹ��ǰ��ʸ���൱��ǰ�����������ڵ�

%% �˶���������
attenuation = 1;%�ٶ�˥��
Vm = 1500*attenuation;%m/s
Am = 3000;%m/s^2
Jm = 20000;%m/s^3
T = 1e-3;%s
delta = 1*Am*T^2/8;%�Ҹ����������ٶȾ�����ֻ�������ļ��ٶȣ�������ֱ�߼��ٶ�
roum = (Vm*T/2)^2/2/delta; %����ٶ�������������ʰ뾶
tic
%% �������ߵĲ�ֵλ�á����ʡ����ʰ뾶��Ϣ
% L = LengthOfPoints(Q);%���߳��ȣ�������ϸ�ֲο�
% numofP = floor(40*L)  %��ֵ������������ʾ
numofP = 1;
% u = linspace(0,1,numofP);
% du = u(2)-u(1);
u = zeros(1,1);
Cx = zeros(numofP,1);
Cy = zeros(numofP,1);
Cz = zeros(numofP,1);
curvity = zeros(numofP,1);
Rou = zeros(numofP,1);
Routest = zeros(numofP,1);
V = zeros(numofP,1);%��¼�ٶ�,V-u��
L = zeros(numofP,1);%ÿһ�㵽�����ܻ�����L-u��
d = 2;
% for i = 1:numofP
i_temp = 1;
while 1
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i_temp),d);
    Cx(i_temp,1) = tempx(1);
    CK1x = tempx(2);%��dx/du
    CK2x = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i_temp),d);
    Cy(i_temp,1) = tempy(1);
    CK1y = tempy(2);%��dy/du
    CK2y = tempy(3);
    tempz = CurveDerivsAlg1(n,p1,U,P(:,3),u(i_temp),d);
    Cz(i_temp,1) = tempz(1);
    CK1z = tempz(2);%��dz/du
    CK2z = tempz(3);
    curvity1 = (CK1x^2 + CK1y^2 + CK1z^2)*(CK2x^2 + CK2y^2 + CK2z^2) -...
                   (CK1x*CK2x + CK1y*CK2y + CK1z*CK2z )^2;
    curvity2 = CK1x^2 + CK1y^2 + CK1z^2;
    curvity(i_temp) = sqrt(curvity1) / sqrt(curvity2^3);
    
    Rou(i_temp) =  sqrt(curvity2^3) / sqrt(curvity1) ;%1/curvity(i_temp);
        
    if Rou(i_temp)>roum
       Rou(i_temp)=roum;
    end
    Routest(i_temp) =Rou(i_temp);%1/curvity(i_temp);
    
    V(i_temp,1) = 2*sqrt(Rou(i_temp)^2 - (Rou(i_temp)-delta)^2)/T;
    
    if V(i_temp,1)>=Vm;
        V(i_temp,1) = Vm;
    end
    
    dL = 0.1*2*sqrt(Rou(i_temp)^2 - (Rou(i_temp)-delta)^2);
    du = dL/sqrt((CK1x)^2 + (CK1y)^2 + (CK1z)^2 );
    i_temp = i_temp + 1;%%% 
    u(i_temp) = u(i_temp-1) + du;
     if u(i_temp)>=1
         u(i_temp) = 1;
         break;
     end
    if i_temp>=2
        L(i_temp) = L(i_temp-1) + dL;
    end
end
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i_temp),d);
    Cx(i_temp,1) = tempx(1);
    CK1x = tempx(2);%��dx/du
    CK2x = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i_temp),d);
    Cy(i_temp,1) = tempy(1);
    CK1y = tempy(2);%��dy/du
    CK2y = tempy(3);
    tempz = CurveDerivsAlg1(n,p1,U,P(:,3),u(i_temp),d);
    Cz(i_temp,1) = tempz(1);
    CK1z = tempz(2);%��dz/du
    CK2z = tempz(3);
    curvity1 = (CK1x^2 + CK1y^2 + CK1z^2)*(CK2x^2 + CK2y^2 + CK2z^2) -...
                   (CK1x*CK2x + CK1y*CK2y + CK1z*CK2z )^2;
    curvity2 = CK1x^2 + CK1y^2 + CK1z^2;
    curvity(i_temp) = sqrt(curvity1) / sqrt(curvity2^3);
    Rou(i_temp) = 1/curvity(i_temp);
    if Rou(i_temp)>roum
       Rou(i_temp)=roum;
    end
    Routest(i_temp) = Rou(i_temp);%1/curvity(i_temp);
    
    V(i_temp,1) = 2*sqrt(Rou(i_temp)^2 - (Rou(i_temp)-delta)^2)/T;
    if V(i_temp,1)>=Vm;
        V(i_temp,1) = Vm;
    end
    dL = (u(i_temp)-u(i_temp-1))*sqrt((CK1x)^2 + (CK1y)^2 + (CK1z)^2 );
    L(i_temp) = L(i_temp-1) + dL;
    numofP = i_temp-1;

 %% ��ʾ���ʡ��ٶ�����
%  subplot(2,2,1)
%  plot3(Cx,Cy,Cz)
%  xlabel('x��')
%  ylabel('y��')
%  zlabel('z��')
%  subplot(2,2,2)
%  plot(1:length(curvity),curvity);
%  ylabel('����')
%  subplot(2,2,3)
%  plot(1:length(Rou),Rou);
%  ylabel('���ʰ뾶')
%  subplot(2,2,4)
%  plot(1:length(V),V)
%  ylabel('�ٶ�����')
    
V1 = V2step(V);%�ٶ�̨�׻�
% V1 = V;
disp('table ready')
toc
%% �岹�����в���
Ls = 0;     %�岹���Ӧ�Ļ���
Ls1 = 0;    %����岹��Ļ���
Vs = 0;     %�岹����ʵ�ٶ�
Vs1 = 0;    %����岹����ٶ�
As = 0;    %�岹��ļ��ٶ�
As1 = 0;   %����岹��ļ��ٶ�
Js = Jm;    %�岹��ļӼ��ٶ�
Js1 = Jm;   %����岹��ļӼ��ٶ�

Xchabu = zeros(1,1);    %�岹��X����
Ychabu = zeros(1,1);    %�岹��Y����
Zchabu = zeros(1,1);
Lchabu = zeros(1,1);    %�岹���Ӧ�Ļ���
Rouchabu = zeros(1,1);
Vchabu = zeros(1,1);
Vlimit = zeros(1,1);
Vlimit1 = zeros(1,1);
us = 0;     %�岹���Ӧ�Ĳ���u
% Ve = 0;     %����岹����ٶ�����
count_chabu = 1;    %�岹�����
Ttest = 10e-3;    %Ԥ��岹���ڡ���T�ĵ�λ�� T vs Ttest����Ttest��ǰհ�����еġ�����岹���ڡ���
pause = 0; %������ͣ��־��0��ʾ����ͣ��1��ʾ��ͣ
%% �岹���̽�����
% Am = Am;
disp('��ʼ�岹');
tic
while  pause == 0 && Ls1<max(L)
    if us>1
        pause = 1;
        disp('��ͣʱ�岹������');
        count_chabu
        break;
    end
    %�Ե�ǰ�ٶ�Vs����ʵ�岹����ٶȡ���������١�S�͹滮�����鿴���ٵ�0�Ĺ������Ƿ�ᳬ��
%%% ��1��������岹���˶�״̬�ĸ��¢٣���As1��Vs1,Ls1��
    As1 = As + Jm*T;
    Vs1 = Vs + As1*T;
    Ls1 = Ls;
%     count_chabu
%һֱ��ѯ�������ٶȼ�Ϊ0,���ߴﵽ�ٶ�����
    count_error = 0;
    Js1 = -Jm;
    while 1 %����岹��ǰհ����
        count_error = count_error + 1;
        if count_error>1000
             %error('������������')
            disp('������������');
            Vs1
            As1
            break;
        end
    %�ٶȺͼ��ٶȾ���С��0ֹͣ���鿴�ٶ������Ƿ����ڳ���
        %��3�����
       if Js1 == -Jm; 
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           if Vs1>=(2*Am^2-As1^2)/(2*Jm) && As1>=-Am%Am^2 / Jm <=  ( Vs1 + As1^2 /( 2*Jm ) ) && As1>=-Am
               Js1 = -Jm;
           elseif Vs1>As1^2/(2*Jm) && Vs1<Am^2/(2*Jm)%As1^2/(2*Jm) < Vs1 &&  Vs1<Am^2/(2*Jm)
               Js1 = -Jm;          
           elseif Vs1 <= As1^2/(2*Jm)
               Js1 = Jm;
           end 
       end
       if Js1==Jm && As1 >0
          break;
       end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        As1 = As1 + Js1*Ttest;%%%  Ttest����ǰհ���̣�����S�滮�еĲ岹���ڣ�
        %ȷ��������ٶ�����
        rou1 = LinearInterpolation(L,Rou,Ls1);
        Ar1 = Vs1^2/rou1;
        if Ar1>=Am %���ٶȳ����ٶ�����ʱ�����������������������ٶ��Ѿ�����
            Am_temp1 = 0;
        else
            Am_temp1 = sqrt(Am^2-Ar1^2);
        end
        if As1<-Am_temp1 
            As1 = -Am_temp1;
        end     
  %%% ��4��������岹���˶�״̬�ĸ��¢ܣ���Vs1,Ls1��
        if As1<=Am
            Vs1 = Vs1 + As1*Ttest;
        else
            Vs1 = Vs1 + As1*Ttest + 0.5*Js1*Ttest^2;
        end
        Ls1 = Ls1 + Vs1*Ttest + 0.5*As1*Ttest^2;    
        if Vs1<0
            break;
        end
        if Ls1>=max(L)
            Js = -Jm;
            break;
        else
            Ve = LinearInterpolation(L,V1,Ls1);  %�ҵ�Ls1��Ӧ���ٶ����� 
        end
    %�����ٶ��������ٶȶԱ�        
        if Ve < Vs1      %Ҫ���٣���ʼ����
            Js = -Jm;
            break;
        else        
            Js = Jm;        %δ���٣���������
        end        
    end
 %%%
    %ʵ�ʽ��в岹�˶���Ҳ��֤�˶�ʱ�ٶȡ����ٶȿ�ͬʱ����0
    if Js == -Jm %�ж��Ƿ���
        if Vs>=(2*Am^2-As^2)/(2*Jm) && As>=-Am
            Js = -Jm;
        elseif Vs>As^2/(2*Jm) && Vs<Am^2/(2*Jm)
            Js = -Jm;          
        elseif As^2/(2*Vs)>=Jm
            Js = Jm;
        end
    end   
    As = As + Js*T;
    rou = LinearInterpolation(L,Rou,Ls); %���Ƿ�����ٶ�
    Ar = Vs^2/rou;   
    if Ar>Am
        Am_temp = 0;
    else
        Am_temp = sqrt(Am^2-Ar^2);
    end
    if As>Am_temp            %��ֹ���ٶȳ��ٶ�
%         disp('ʸ�����ٶ�������');
        As = Am_temp;
        Js = 0;
    elseif As<-Am_temp
%         disp('ʸ�����ٶȷ�����');
        As = -Am_temp;
        Js = 0;
    end
    Vs = Vs + As*T;% + 0.5 * Js*T^2 ;   %������������ᵼ�¼Ӽ��ٶ���������ʱ���ּ��ٶȼ��
    if Vs<0
        disp('�ٶȼ���0')
        As
        Vs = 0;
        Js = Jm;
        As = 0 + Js*T;
        Vs = Vs + As*T;
    end
    Ls = Ls + Vs*T;% + 0.5 * As*T^2;
    %�����ٶȲ��������ӵĻ����������ʽ�滮�ν�ʱ����ּ��ٶȳ��������Ϊ���˼��ٶ�������������
    %����������������ᵼ�¼Ӽ��ٶ���������ʱ���ּӼ��ٶȼ��
    us = LinearInterpolation(L,u,Ls);   %�ҵ��岹���Ӧ��u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = LinearInterpolation(L,Cx,Ls);
    Ychabu(count_chabu) = LinearInterpolation(L,Cy,Ls);
    Zchabu(count_chabu) = LinearInterpolation(L,Cz,Ls);
    Rouchabu(count_chabu) = LinearInterpolation(L,Routest,Ls);
    
    Lchabu(count_chabu) = Ls;   
end
disp('ǰհ�㵽���յ�')
%% ����pause = 1 ����5�ζ���滮���ٵ�0
if pause ==1
%     disp('��ͣ��ǰ����');
%     count_chabu
%     us
    Vs = Vs + As*T;
%     As
    Lleft = Vs^2/(2*Am/2);
    Lleft = min(max(L)-Ls,Lleft);
    Ls0 = Ls;
%     Vs
%     As 
    Ve = 0;
    Ae = 0;
    Ttotal = Lleft/Vs*2.5;%ǡ��ѡȡ���ʱ��ϵ��
    Nt1 = [Ttotal^5     Ttotal^4     Ttotal^3    Ttotal^2  Ttotal  1;
           5*Ttotal^4   4*Ttotal^3   3*Ttotal^2  2*Ttotal  1       0;
           20*Ttotal^3  12*Ttotal^2  6*Ttotal    2         0       0;
           0            0            0           2         0       0;
           0            0            0           0         1       0;
           0            0            0           0         0       1];
    BoundaryCondition = [Lleft,Ve,Ae,As,Vs,0]';
    %s = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F;
    PolyCoefficient = gauss(Nt1,BoundaryCondition);
    Am_temp = PolyCoefficient(1);
    B = PolyCoefficient(2);
    C = PolyCoefficient(3);
    D = PolyCoefficient(4);
    E = PolyCoefficient(5);
    F = PolyCoefficient(6);
    SPolyCoefficient = [Am_temp,B,C,D,E,F];
    VPolyCoefficient = [0,5*Am_temp,4*B,3*C,2*D,E];
    APolyCoefficient = [0,0,20*Am_temp,12*B,6*C,2*D];
    tcount = 1;  
    t = tcount * T;
end
disp('������ͣ')
while pause == 1&& t<Ttotal && us < 1
    t = tcount * T;
    Ttest = [t.^5;t.^4;t.^3;t.^2;t;1];
    Stest = SPolyCoefficient*Ttest;
    Vs = VPolyCoefficient*Ttest;
    As = APolyCoefficient*Ttest;
    Ls = Ls0 + Stest;
    us = LinearInterpolation(L,u,Ls);   %�ҵ��岹���Ӧ��u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = LinearInterpolation(L,Cx,Ls);
    Ychabu(count_chabu) = LinearInterpolation(L,Cy,Ls);
    Zchabu(count_chabu) = LinearInterpolation(L,Cz,Ls);
    Lchabu(count_chabu) = Ls;  
    
    tcount = tcount + 1;  

end

%% �յ�δ���
if pause == 1
%% �յ�ǰһ�ε����滮����5�ζ���ʽ�ٶȹ滮
    disp('�յ�δ���');
    count_chabu
    Lleft = max(L)-Ls
    Ls0 = Ls;
    Vs = Vs + As*T %��ǰ�岹��Ӧ�ôﵽ���ٶȣ���ʹ��Vs,�ᵼ�¼��ٶȳ��
    As 
    Ve = 0;
    Ae = 0;

    Ttotal = Lleft/Vs*1.5;
    Nt1 = [Ttotal^5     Ttotal^4     Ttotal^3    Ttotal^2  Ttotal  1;
           5*Ttotal^4   4*Ttotal^3   3*Ttotal^2  2*Ttotal  1       0;
           20*Ttotal^3  12*Ttotal^2  6*Ttotal    2         0       0;
           0            0            0           2         0       0;
           0            0            0           0         1       0;
           0            0            0           0         0       1];
    BoundaryCondition = [Lleft,Ve,Ae,As,Vs,0]';
    %s = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F;

    PolyCoefficient = gauss(Nt1,BoundaryCondition);
    Am_temp = PolyCoefficient(1);
    B = PolyCoefficient(2);
    C = PolyCoefficient(3);
    D = PolyCoefficient(4);
    E = PolyCoefficient(5);
    F = PolyCoefficient(6);
    SPolyCoefficient = [Am_temp,B,C,D,E,F];
    VPolyCoefficient = [0,5*Am_temp,4*B,3*C,2*D,E];
    APolyCoefficient = [0,0,20*Am_temp,12*B,6*C,2*D];
    tcount = 1;
    t = tcount * T;
    while Vs>-3 && us<1 && t<=Ttotal
        t = tcount * T;
        Ttest = [t.^5;t.^4;t.^3;t.^2;t;1];
        Stest = SPolyCoefficient*Ttest;
        Vs = VPolyCoefficient*Ttest;
        As = APolyCoefficient*Ttest;
        Ls = Ls0 + Stest;
%         Ls = Ls + Vs*T + 0.5 * As*T^2;
        if Vs<-3
            break;
        end
        us = LinearInterpolation(L,u,Ls);   %�ҵ��岹���Ӧ��u
        if us>1
            us = 1;
        end
        Vchabu(count_chabu) = Vs;
        Vlimit(count_chabu) = LinearInterpolation(L,V,Ls);
        Vlimit1(count_chabu) = LinearInterpolation(L,V1,Ls);

        count_chabu = count_chabu + 1;
        Xchabu(count_chabu) = LinearInterpolation(L,Cx,Ls);
        Ychabu(count_chabu) = LinearInterpolation(L,Cy,Ls);
        Zchabu(count_chabu) = LinearInterpolation(L,Cz,Ls);
        Rouchabu(count_chabu) = LinearInterpolation(L,Routest,Ls);
        Lchabu(count_chabu) = Ls;  

        tcount = tcount + 1;
    end

end
disp('�岹����������ʱ�䣺')
toc

disp('�岹�ڵ������')
disp(count_chabu)
Vs
As
Lerror = sqrt( (Xchabu(count_chabu)-x(length(x)))^2 +...
               (Ychabu(count_chabu)-y(length(x)))^2 +...
               (Zchabu(count_chabu)-z(length(x)))^2 )
%% �ٶȿ���
Vchabu1 = zeros(1,1);
for i = 2:count_chabu
    Vchabu1(i-1) = sqrt( (Xchabu(i)-Xchabu(i-1))^2 + (Ychabu(i)-Ychabu(i-1))^2 + (Zchabu(i)-Zchabu(i-1))^2)/T;
end
% Vchabu = Vchabu1;
%% ���ٶȿ���
%1���������ٶ�
Acheck1 = zeros(1,1);
for i = 2:count_chabu-1
    Acheck1(i-1) = ((Vchabu1(i)-Vchabu1(i-1)))/T;
end
%2��ʸ�����ٶ�
Acheck2 = zeros(1,1);
thetatest = zeros(1,1);
for i = 1:count_chabu-2
    if i ==1
        V1 = [Xchabu(2)-Xchabu(1),Ychabu(2)-Ychabu(1),Zchabu(2)-Zchabu(1)]/T;
        V2 = [Xchabu(3)-Xchabu(2),Ychabu(3)-Ychabu(2),Zchabu(3)-Zchabu(2)]/T;
    else
        V1 = [Xchabu(i+1)-Xchabu(i),Ychabu(i+1)-Ychabu(i),Zchabu(i+1)-Zchabu(i)]/T;
        V2 = [Xchabu(i+2)-Xchabu(i+1),Ychabu(i+2)-Ychabu(i+1),Zchabu(i+2)-Zchabu(i+1)]/T;
    end
    dV = V2-V1;
    Acheck2(i) = norm(dV)/T;
end
%% �Ӽ��ٶ�
Jcheck = zeros(1,1);
for i = 2:count_chabu-2
    Jcheck(i-1) = ((Acheck1(i)-Acheck1(i-1)))/T;
end

%% ��ͼ��
subplot(3,2,1)  %�켣
plot3(x,y,z,'*','Color','r');              %��ֵ��
hold on;
plot3(Cx,Cy,Cz,'r');                        %�켣
% plot(Xchabu,Ychabu,'.','Color','b');    %�岹��
xlabel('x��')
ylabel('y��')
zlabel('z��')

subplot(3,2,3)  %�ٶ�
% plot(L,V,'k');                              %�ٶ���-��������
% hold on
plot(Lchabu(1:count_chabu-1),Vlimit,'r');  
hold on;
plot(Lchabu(1:count_chabu-1),Vchabu,'b');   %�岹�ٶ�-��������
plot(Lchabu(1:count_chabu-1),Vlimit1,'g');   %�ٶ����ƣ����λ��ٶ�����
ylabel('�ٶ�')
xlabel('����');

subplot(3,2,5)  %���ٶ�
plot(Lchabu(1:count_chabu-2),Acheck1,'g');    %�������ٶ�
hold on;
plot(Lchabu(1:count_chabu-2),Acheck2,'r');    %ʸ�����ٶ�
plot(linspace(0,max(Lchabu),200),linspace(Am,Am,200),'b');    %�ο���
plot(linspace(0,max(Lchabu),200),linspace(-Am,-Am,200),'b');  %�ο���
plot(linspace(0,max(Lchabu),200),linspace(0,0,200),'b');  %�ο���
ylabel('���ٶ�')
xlabel('����');
% 
% close
ax(1) = subplot(3,2,2);
plot(1:length(Vchabu1),Vchabu1,'b');
hold on
plot(1:length(Vchabu1),Vchabu,'m');
plot(1:length(Vlimit),Vlimit,'r');
plot(1:length(Vlimit),Vlimit1,'g');
ylabel('�ٶ�')
xlabel('ʱ��/ms')

ax(2) = subplot(3,2,4);
plot(1:length(Acheck1),Acheck1,'g');%������ٶ�
hold on
plot(1:length(Acheck2),Acheck2,'r');%ʸ�����ٶ�
hold on
a1 = Vchabu.^2./Rouchabu(1:length(Vchabu)); %������ٶ�
plot(1:length(Vchabu),a1,'b')
plot(1:length(a1)-1,sqrt(a1(1:length(a1)-1).^2+Acheck1.^2),'k'); %������ٶ���������ٶ� ����
plot(1:length(Acheck2),linspace(0,0,length(Acheck2)),'b');  %�ο���
plot(linspace(0,count_chabu,200),linspace(Am,Am,200),'b');    %�ο���
plot(linspace(0,count_chabu,200),linspace(-Am,-Am,200),'b');  %�ο���
ylabel('���ٶ�')
xlabel('ʱ��/ms')
ax(3) = subplot(3,2,6);
plot(1:length(Jcheck),Jcheck,'r')
hold on
% plot(1:length(Jcheck),linspace(2e4,2e4,length(Jcheck)));
% plot(1:length(Jcheck),linspace(-2e4,-2e4,length(Jcheck)));
% xlabel('�Ӽ��ٶ�')
ylabel('�Ӽ��ٶ�')
xlabel('ʱ��/ms')
linkaxes(ax,'x');
Jmax = max(abs(Jcheck))