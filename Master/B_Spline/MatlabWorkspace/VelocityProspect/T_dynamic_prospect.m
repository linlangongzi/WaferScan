%%
%�ٶ�ǰհ������T���ȼӼ��ٹ滮
%   �ɵ�ǰ�ٶ��������ٶȼ��ٵ��㣬�õ�һ������켣�ٶȣ����ٶ����ƱȽ�
%���δ���٣�����ٶ���Ϊ�����в岹�˶�����֮�������ٶȼ���  
%% �����˶��켣
clear
clc
format long;
n = 5;%���ݵ����
x = 4*[0,40,80,120,160,200]';
y = 10*[0,-2,5,-10,15,0]';
% x = [0,50,100,50,0]';
% y = [0,50,50,50,100]';
Q = [x,y];  %���ݵ㣺QΪ(n+1)*2�ľ���,��ά��(x,y)

p = 3;  %B�����������״�
D0 = 1*(Q(2,:)-Q(1,:)); %�˵㵼ʸ����Ϊ��һ���ҷ���
Dn = 1*(Q(n+1,:)-Q(n,:)); %�յ㵼ʸ����Ϊ���һ���ҷ���
[ p1,m,U,P] = GlobalCurveInterp_C1( n,Q,p,D0,Dn);
n = n + 2;%ʹ��ǰ��ʸ���൱��ǰ�����������ڵ�

%% �˶���������
attenuation = 0.5;
Vm = 1500*attenuation;%m/s
Am = 4000;%m/s^2
T = 1e-3;%s
delta = Am*T^2/8;%�Ҹ����������ٶȾ�����ֻ�������ļ��ٶȣ�������ֱ�߼��ٶ�
roum = (Vm*T/2)^2/2/delta; %����ٶ�������������ʰ뾶

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
     if i<numofP
        L(i+1) = L(i) + sqrt((CK1x*du)^2 + (CK1y*du)^2);
     end
end
V(numofP,1) = 0;

%% �岹�����в���
Vs = 0;     %�岹����ʵ�ٶ�
Vs1 = 0;    %����岹����ٶ�
Xchabu = zeros(1,1);    %�岹��X����
Ychabu = zeros(1,1);    %�岹��Y����
Lchabu = zeros(1,1);    %�岹���Ӧ�Ļ���
Vchabu = zeros(1,1);
Ls = 0;     %�岹���Ӧ�Ļ���
Ls1 = 0;    %����岹����ٶ�
As = Am;    %�岹��ļ��ٶ�
As1 = Am;   %����岹��ļ��ٶ�
us = 0;     %�岹���Ӧ�Ĳ���u
Ve = 0;     %����岹����ٶ�����
count_chabu = 1;    %�岹�����

%% �岹���̽�����
disp('��ʼT���ٶȹ滮���в岹');
tic
while us<1 && Vs>=0
    %�Ե�ǰ�ٶ�Vs��������٣��鿴���ٵ�0�Ĺ������Ƿ�ᳬ��
    Vs1 = Vs;
    Ls1 = Ls;
    while(Vs1>0) %һֱ��ѯ�������ٶȼ�Ϊ0,���ߴﵽ�ٶ�����
        As1 = -Am;
        Vs1 = Vs1 + As1*T;
        Ls1 = Ls1 + Vs1*T;
        Ve = LinearInterpolation(L,V,Ls1);  %�ҵ�Ls1��Ӧ���ٶ�����
        if Ve-10 > Vs1
            As = Am;
        else
            As = -Am;
            break;
        end
    end
    Vs = Vs + As*T;
    Ls = Ls + Vs*T;
    us = LinearInterpolation(L,u,Ls);   %�ҵ��岹���Ӧ��u
    if us>1
        us = 1;
    end
    Vchabu(count_chabu) = Vs;
    count_chabu = count_chabu + 1;
    Xchabu(count_chabu) = CurvePoint(n,p1,U,P(:,1),us);
    Ychabu(count_chabu) = CurvePoint(n,p1,U,P(:,2),us);
    Lchabu(count_chabu) = Ls;

end
toc
disp('�岹����')
disp('�岹�ڵ������')
disp(count_chabu)
%% ���ٶȿ���
%1���������ٶ�
Acheck1 = zeros(1,1);
for i = 2:count_chabu-1
    Acheck1(i-1) = ((Vchabu(i)-Vchabu(i-1)))/T;
end
% Acheck1Max = max(Acheck1)%�ٶ�ģ������������ٶ�
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
% Acheck2Max = max(Acheck2)

%% ��ͼ��
subplot(3,1,1)  %�켣
plot(x,y,'*','Color','r');              %��ֵ��
hold on;
plot(Cx,Cy,'r');                        %�켣
plot(Xchabu,Ychabu,'.','Color','b');    %�岹��
xlabel('�켣')

subplot(3,1,2)  %�ٶ�
plot(L,V,'r');                              %�ٶ���-��������
hold on;
plot(Lchabu(1:count_chabu-1),Vchabu,'g');   %�岹�ٶ�-��������
xlabel('�ٶȼ��ٶ�����')

subplot(3,1,3)  %���ٶ�
plot(Lchabu(1:count_chabu-2),Acheck1/Am*200,'g');    %�������ٶ�
hold on;
plot(Lchabu(1:count_chabu-2),Acheck2/Am*200,'b');    %ʸ�����ٶ�
plot(linspace(0,max(Lchabu),200),linspace(400,400,200),'r');    %�ο���
plot(linspace(0,max(Lchabu),200),linspace(-400,-400,200),'r');  %�ο���
xlabel('ʸ�����ٶȣ��������ٶ�')