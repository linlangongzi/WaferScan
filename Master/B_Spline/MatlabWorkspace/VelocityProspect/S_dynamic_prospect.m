%%
%��̬ǰհ������S���ٶȹ滮
%   �ɵ�ǰ�ٶ��������ٶȼ��ٵ��㣬�õ�һ������켣�ٶȣ����ٶ����ƱȽ�
%���δ���٣�����ٶ���Ϊ�����в岹�˶�����֮�������ٶȼ���  
%% �����˶��켣
clear
clc
format long;
% x = 8*[0,40,80,120,160,200]';%�켣
% y = 40*[0,-2,5,-10,15,0]';
% x = [0,50,100,50,0]';
% y = [0,50,50,50,100]';
x = [0,200,200,400,400]';
y = [0,0,200,200,400]';
n = length(x)-1;%���ݵ����
Q = [x,y];  %���ݵ㣺QΪ(n+1)*2�ľ���,��ά��(x,y)
%% ���������߶μнǹ�СʱСʱ����ʾ�޷���ϣ������˶���ʱ
p = 3;  %B�����������״�
D0 = 1*(Q(2,:)-Q(1,:)); %�˵㵼ʸ����Ϊ��һ���ҷ���
Dn = 1*(Q(n+1,:)-Q(n,:)); %�յ㵼ʸ����Ϊ���һ���ҷ���
[ p1,m,U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
n = n + 2;%ʹ��ǰ��ʸ���൱��ǰ�����������ڵ�

%% �˶���������
attenuation = 1;%�ٶ�˥��
Vm = 1500*attenuation;%m/s
Am = 4000;%m/s^2
Jm = 20000;%m/s^3
T = 1e-3;%s
delta = Am*T^2/8;%�Ҹ����������ٶȾ�����ֻ�������ļ��ٶȣ�������ֱ�߼��ٶ�
roum = (Vm*T/2)^2/2/delta; %����ٶ�������������ʰ뾶
tic
%% �������ߵĲ�ֵλ�á����ʡ����ʰ뾶��Ϣ
numofP=1000;%��ֵ������������ʾ
u = linspace(0,1,numofP);
du = u(2)-u(1);
Cx = zeros(numofP,1);
Cy = zeros(numofP,1);
curvity = zeros(numofP,1);
V = zeros(numofP,1);%��¼�ٶ�,V-u��
L = zeros(numofP,1);%ÿһ�㵽�����ܻ�����L-u��
d = 2;
for i = 1:numofP
    tempx = CurveDerivsAlg1(n,p1,U,P(:,1),u(i),d);
    Cx(i) = tempx(1);
    CK1x = tempx(2);%��dx/du
    CK2x = tempx(3);
    tempy = CurveDerivsAlg1(n,p1,U,P(:,2),u(i),d);
    Cy(i) = tempy(1);
    CK1y = tempy(2);%��dy/du
    CK2y = tempy(3);
    curvity(i) = abs(CK1x*CK2y - CK1y*CK2x) / (sqrt(CK1x^2 + CK1y^2))^3;
    Rou = 1/curvity(i);
    if Rou>roum
       Rou=roum;
    end
    V(i) = 2*sqrt(Rou^2 - (Rou-delta)^2)/T;
     if i>=2
        L(i) = L(i-1) + sqrt((CK1x*du)^2 + (CK1y*du)^2);
     end
end
V(numofP,1) = 0;
%�յ�ǰ���ٶȽ�����
%  for i = 1:20
%     V(numofP-100+i,1) = max(V)*0.8;
%  end
for i =1:40
    V(numofP-40+i,1) = V(numofP-40,1)*(40-i)/40;
end
V1 = V2step(V);%�ٶ�̨�׻�
disp('table ready')
toc
%% �岹�����в���
Ls = 0;     %�岹���Ӧ�Ļ���
Ls1 = 0;    %����岹����ٶ�
Vs = 0;     %�岹����ʵ�ٶ�
Vs1 = 0;    %����岹����ٶ�
As = 0;    %�岹��ļ��ٶ�
As1 = 0;   %����岹��ļ��ٶ�
Js = Jm;    %�岹��ļӼ��ٶ�
Js1 = Jm;   %����岹��ļӼ��ٶ�

Xchabu = zeros(1,1);    %�岹��X����
Ychabu = zeros(1,1);    %�岹��Y����
Lchabu = zeros(1,1);    %�岹���Ӧ�Ļ���
Vchabu = zeros(1,1);
Vlimit = zeros(1,1);
Vlimit1 = zeros(1,1);
us = 0;     %�岹���Ӧ�Ĳ���u
Ve = 0;     %����岹����ٶ�����
count_chabu = 1;    %�岹�����
Ttest = 10*T;    %Ԥ��岹����
%% �岹���̽�����
% Am = Am;
disp('��ʼ�岹');
tic
while us<0.9%us<1&&Ls<max(L)*0.95
    %�Ե�ǰ�ٶ�Vs��������٣��鿴���ٵ�0�Ĺ������Ƿ�ᳬ��
    As1 = As;
    Vs1 = Vs;
    Ls1 = Ls;
%     count_chabu
%һֱ��ѯ�������ٶȼ�Ϊ0,���ߴﵽ�ٶ�����
    count_error = 0;
    while 1%~(Vs1<5&&Vs1>0 && abs(As1)<40)
        count_error = count_error + 1;
        if count_error>1000
%             error('������������')
            break;
        end
    %�ٶȺͼ��ٶȾ���С��0ֹͣ���鿴�ٶ������Ƿ����ڳ���
        %��3�����
        Js1 = -Jm;
        if Vs1>=(2*Am^2-As1^2)/(2*Jm) && As1>=-Am
            Js1 = -Jm;
        elseif Vs1>As1^2/(2*Jm) && Vs1<Am^2/(2*Jm)
            Js1 = -Jm;          
        elseif As1^2/(2*Vs1)>=Jm
            Js1 = Jm;
        end   
        As1 = As1 + Js1*Ttest;
        if As1<-Am
            As1 = -Am;
        end        
        Vs1 = Vs1 + As1*Ttest;
        Ls1 = Ls1 + Vs1*Ttest;    
        if Vs1<0
            break;
        end
        
        if Ls1>=max(L)
            Js = -Jm;
            break;
%             Ve = 0;
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
    if As>Am            %��ֹ���ٶȳ��ٶ�
        As = Am;
        Js = 0;
    elseif As<-Am
        As = -Am;
        Js = 0;
    end
    Vs = Vs + As*T;   
%     Vs = min(Vs,LinearInterpolation(L,V,Ls));
    if Vs<0
        As   %�鿴���ٶ�ͻ��ֵ
        Vs = 0;
        Js = Jm;
        As = 0 + Js*T;
        Vs = Vs + As*T;
        count_chabu
    end
    Ls = Ls + Vs*T;
    us = LinearInterpolation(L,u,Ls);   %�ҵ��岹���Ӧ��u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = CurvePoint(n,p1,U,P(:,1),us);
    Ychabu(count_chabu) = CurvePoint(n,p1,U,P(:,2),us);
    Lchabu(count_chabu) = Ls;   
%     if us>0.5     %����������ͣ
%         V = zeros(length(V),1);
%         if Vs<1&&abs(As)<21
%             break;
%         end
%     end
end
disp('ǰհ�㵽���յ�')
%% �յ�ǰһ�ε����滮����5�ζ���ʽ�ٶȹ滮
us
Lleft = max(L)-Ls
Ls0 = Ls;
Vs
As 
Ve = 0;
Ae = 0;
Ttotal = Lleft/Vs*2.5;
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
tcount = 1;

while Vs>-3&&us<1
    t = tcount * T;
    Ttest = [t.^5;t.^4;t.^3;t.^2;t;1];
    Stest = SPolyCoefficient*Ttest;
    Vs = VPolyCoefficient*Ttest;
    As = APolyCoefficient*Ttest;
    Ls = Ls0 + Stest;
    if Vs<-3
        break;
    end
    us = LinearInterpolation(L,u,Ls);   %�ҵ��岹���Ӧ��u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    Vlimit(count_chabu) = LinearInterpolation(u,V,us);
    Vlimit1(count_chabu) = LinearInterpolation(u,V1,us);
    
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = CurvePoint(n,p1,U,P(:,1),us);
    Ychabu(count_chabu) = CurvePoint(n,p1,U,P(:,2),us);
    Lchabu(count_chabu) = Ls;   

    tcount = tcount + 1;
end
toc
disp('�岹����')
disp('�岹�ڵ������')
disp(count_chabu)
Vs
As
Lerror = max(Xchabu)-max(x)
%% �ٶȿ���
% for i = 2:count_chabu
%     Vchabu(i-1) = sqrt( (Xchabu(i)-Xchabu(i-1))^2 + (Ychabu(i)-Ychabu(i-1))^2 )/T;
% end
%% ���ٶȿ���
%1���������ٶ�
Acheck1 = zeros(1,1);
for i = 2:count_chabu-1
    Acheck1(i-1) = ((Vchabu(i)-Vchabu(i-1)))/T;
end
%2��ʸ�����ٶ�
Acheck2 = zeros(1,1);
for i = 1:count_chabu-2
    if i ==1
        Norm1 = [Xchabu(1),Ychabu(1)];
        Norm2 = [Xchabu(2)-Xchabu(1),Ychabu(2)-Ychabu(1)];
        theta = VectorsAngle(Norm1,Norm2);  
    else
        Norm1 = [Xchabu(i)-Xchabu(i-1),Ychabu(i)-Ychabu(i-1)];
        Norm2 = [Xchabu(i+1)-Xchabu(i),Ychabu(i+1)-Ychabu(i)];
        theta = VectorsAngle(Norm1,Norm2);
    end
    dV = sqrt((Vchabu(i+1)*sind(theta))^2+(Vchabu(i)-Vchabu(i+1)*cosd(theta))^2);
    Acheck2(i) = dV/T;
end
%% �Ӽ��ٶ�
Jcheck = zeros(1,1);
for i = 2:count_chabu-2
    Jcheck(i-1) = ((Acheck1(i)-Acheck1(i-1)))/T;
    if Jcheck(i-1)>30000
        Jcheck(i-1) = 30000;
    elseif Jcheck(i-1)<-30000;
        Jcheck(i-1) = -30000;
    end
end

%% ��ͼ��
subplot(3,1,1)  %�켣
plot(x,y,'*','Color','r');              %��ֵ��
hold on;
plot(Cx,Cy,'r');                        %�켣
plot(Xchabu,Ychabu,'.','Color','b');    %�岹��
xlabel('�켣')

subplot(3,1,2)  %�ٶ�
plot(L,V,'k');                              %�ٶ���-��������
hold on
plot(Lchabu(1:count_chabu-1),Vlimit,'r');  
hold on;
plot(Lchabu(1:count_chabu-1),Vchabu,'b');   %�岹�ٶ�-��������
plot(Lchabu(1:count_chabu-1),Vlimit1,'g');   %�ٶ����ƣ����λ��ٶ�����
xlabel('�ٶȼ��ٶ�����')

subplot(3,1,3)  %���ٶ�
plot(Lchabu(1:count_chabu-2),Acheck1,'m');    %�������ٶ�
hold on;
plot(Lchabu(1:count_chabu-2),Acheck2,'g');    %ʸ�����ٶ�
plot(linspace(0,max(Lchabu),200),linspace(Am,Am,200),'b');    %�ο���
plot(linspace(0,max(Lchabu),200),linspace(-Am,-Am,200),'b');  %�ο���
plot(linspace(0,max(Lchabu),200),linspace(0,0,200),'b');  %�ο���
xlabel('ʸ�����ٶȣ��������ٶ�')
% 
close
subplot(3,1,1)
plot(1:length(Vchabu),Vchabu);
hold on
plot(1:length(Vlimit),Vlimit,'r');
plot(1:length(Vlimit),Vlimit1,'g');
xlabel('�ٶ�')
subplot(3,1,2)
plot(1:length(Acheck1),Acheck1,'m');
hold on
plot(1:length(Acheck2),Acheck2,'g');
plot(1:length(Acheck2),linspace(0,0,length(Acheck2)),'b');  %�ο���
plot(linspace(0,count_chabu,200),linspace(Am,Am,200),'b');    %�ο���
plot(linspace(0,count_chabu,200),linspace(-Am,-Am,200),'b');  %�ο���
xlabel('���ٶ�')
subplot(3,1,3)
plot(1:length(Jcheck),Jcheck,'r')
xlabel('�Ӽ��ٶ�')