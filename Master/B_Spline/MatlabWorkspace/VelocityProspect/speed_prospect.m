%%
%�㶨ǰհ����
%�ٶ�ǰհ�����ú㶨ǰհ����ķ�ʽȷ��ǰհ���ٶȣ����ȼӼ��ٹ滮
%�ٶ�ǰհ�Ĺ��̲��ò��������������ۻ���ͬʱ�ٶȴ�����
%%
clear
clc
format long;
x = 2*[0,40,80,120,160,200]';
y = 10*[0,-2,5,-10,15,0]';
% x = [0,50,100,50,0]';
% y = [0,50,50,50,100]';
% x = [0,0,500,500,0]';
% y = [0,500,500,0,0]';
n = length(x)-1;%���ݵ����
Q = [x,y];  %���ݵ㣺QΪ(n+1)*2�ľ���,��ά��(x,y)
hold on;
p = 3;  %B�����������״�
subplot(3,1,1)
plot(x,y,'*','Color','r');
hold on;
%  [ p1,m,U,P] = GlobalCurveInterp( n,Q,p);
% 
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
Lm = Vm^2/(2*Am); %���ǰհ����28~280mm
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

plot(Cx,Cy,'r');
hold on;
%% Ѱ���ٶȲ��ȵ㲢���㲨�ȵ�֮��Ļ������ҵ�������Сֵ,���¼�������ٶ�
VminP = zeros(1,1);%�ٶȲ��ȵ�����
uSingular = zeros(1,1);%�ٶ�����¼
numofVminP = 2;
VminP(1) = 1;%���Ҳ��һ�����ʲ����
for i = 2:numofP-1
    if (curvity(i)>curvity(i-1) && curvity(i)>curvity(i+1))
        VminP(numofVminP) = i;%��¼�ٶȲ��ȵ��λ�ã������Ƕ�Ӧ���ٶ�
        uSingular(1,numofVminP) = u(i);
        numofVminP = numofVminP + 1;%�ٶ���������1
        VminPx = CurvePoint( n,p1,U,P(:,1),u(i));
        VminPy = CurvePoint( n,p1,U,P(:,2),u(i));
        plot(VminPx,VminPy,'o');%�����ٶȲ��ȵ��λ��
        hold on;
    end
end
uSingular(1,numofVminP) = 1;%u = 1Ҳ��һ���ٶ����
numofVminP = numofVminP-1;
%�����ٶȲ��ȵ��Ļ������ҵ���Сֵ
for i = 1:numofVminP-1
    Ltemp = L(VminP(i+1))-L(VminP(i));%�����ٶȲ��ȵ�֮��Ļ���
    Lm = min(Lm,Ltemp);%�ҵ���Сֵ
end
%ǰհ����ȷʵ Lm
%���¼����˶�������������ٶ�����
Vm = min(Vm,sqrt(2*Am*Lm+(min(V))^2)); %
roum = (Vm*T/2)^2/2/delta; %����ٶ�������������ʰ뾶
%���¶��ٶ����ߡ����ʰ뾶���߽���Լ��
for i = 1:numofP
    if V(i)>Vm
        V(i) = Vm;
    end
end
Lm = Lm*0.8;
%% Ѱ�ҵ�һ��ǰհ��
% Ltemp = 0;
% ue = 0;%ǰհ��
%us = 0;%�岹��
Ve = Vm;%ǰհ���ٶ�
Vs = 0;%�岹���ٶ�
Vs1 = Vm;%�岹�������ٶ���
ue = LinearInterpolation(L,u,Lm);%����ҵ�һ��ǰհ���λ��
Ve1 = LinearInterpolation(L,V,Lm);%����ҵ�һ��ǰհ����ٶ�
Ve = min(Ve,Ve1);
Acc = 0;
disp('��һ��ǰհ���ҵ�');
%��һ��ǰհ�����ҵ�
%% �岹�˶�׼��
tic;
Vchabu = zeros(1,1);%�岹���Ӧ���ٶ�����
Xs = zeros(1,1);%�岹����������
Ys = zeros(1,1);
us =0;
Us = zeros(1,1);%��¼�岹���λ�ò���u
count_chabu = 0;
ndl = 5;
Lchabu0 = 0;
Lchabu = zeros(1,1);
flagIn = 0;%�״ο��ٶ����
flag = 0;%��¼�Ƿ��Խ�ٶ����
%% ǰհ��δ���������յ�
while Vs>=0 && us<1                                     %ǰհ����δ���������յ�
    for i = 2:length(uSingular)
        if us<uSingular(i) && ue>=uSingular(i)
            flag = 1;
            Ve = LinearInterpolation(u,V,uSingular(i));%Veȡ��㴦�ٶ�
            if flagIn == 0
                Acc = (Ve^2-Vs^2)/(2*Lm);
                flagIn = 1;
%                 uSingular(i)
            end
            break;
        end  
        flag = 0;
    end
    if flag==0 %��ʾû�п�Խ�ٶ����
        flagIn = 0;
        Ve = Ve1;
        Acc = (Ve^2-Vs^2)/(2*Lm);
        if Acc>0
            Acc = Am;
        end
    end

    Vs = Vs + Acc*T;%�岹�ٶ�
    Vs1 = LinearInterpolation(u,V,us);%������ٶ�����
    Vs = min(Vs,Vs1);
    count_chabu = count_chabu + 1;
    Vchabu(count_chabu) = Vs;       %�岹��Ĳ岹�ٶ�
    dL = Vs*T;%�岹����
    Lchabu0 = Lchabu0 + dL; %�岹�㵽���Ļ���
    Lchabu(1,count_chabu) = Lchabu0;
    %Ѱ����һ���岹��us->x,y
    us = LinearInterpolation(L,u,Lchabu0);  %���Ѱ����һ���岹���us
    if us<0 || us>1
        us = 1;
        %error('����us<0 �� us>1')
    end
    Xs(count_chabu) = CurvePoint( n,p1,U,P(:,1),us);       %��һ���岹���λ��
    Ys(count_chabu) = CurvePoint( n,p1,U,P(:,2),us);
    Us(count_chabu) = us;
    %�ҵ���һ���岹��

    %Ѱ����һ��ǰհ�� 
    %Le = Le + dL;
    ue = LinearInterpolation(L,u,Lchabu0+Lm);
    if ue>1
        ue = 1;
    end   
    Ve1 = LinearInterpolation(L,V,Lchabu0+Lm);
    %��һ��ǰհ���ٶ�����Ve1ȷ��
end  
 %% ǰհ�㵽�������յ�
    disp('ǰհ�㵽�������յ�');
%     Ve = 0;
%     Acc = (Ve^2-Vs^2)/(2*Lm);
%     while us<1 && Vs>0                                    %�岹��δ���������յ�
%         Vs = Vs + Acc*T;
%         count_chabu = count_chabu + 1;
%         Vchabu(count_chabu) = Vs;       %�岹��Ĳ岹�ٶ�
%         dL = Vs*T;%�岹����
%         Lchabu0 = Lchabu0 + dL;
%         Lchabu(1,count_chabu) = Lchabu0;
%         %Ѱ����һ���岹��us->x,y
%         us = LinearInterpolation(L,u,Lchabu0);
%         if us>1
%             us = 1;
%         end
%         Xs(count_chabu) = CurvePoint( n,p1,U,P(:,1),us);       %��һ���岹���λ��
%         Ys(count_chabu) = CurvePoint( n,p1,U,P(:,2),us);
%         Us(count_chabu) = us;
%         %�ҵ���һ���岹��
%     end
%     disp('�岹�㵽�������յ�');
    disp('�岹ʱ�䣺');
    toc;
%% ���ٶ���֤
%1���ٶ�ģ������ļ��ٶ�
Acheck1 = zeros(1,1);
for i = 2:count_chabu
    Acheck1(i-1) = ((Vchabu(i)-Vchabu(i-1)))/T;
end
% Acheck1Max = max(Acheck1)%�ٶ�ģ������������ٶ�
%2��ʸ�����ٶ�
Acheck2 = zeros(1,1);
for i = 1:count_chabu-1
    if i ==1
        Norm1 = [Xs(1),Ys(1)];
        Norm2 = [Xs(2)-Xs(1),Ys(2)-Ys(1)];
        theta = VectorsAngle(Norm1,Norm2);  
    else
        Norm1 = [Xs(i)-Xs(i-1),Ys(i)-Ys(i-1)];
        Norm2 = [Xs(i+1)-Xs(i),Ys(i+1)-Ys(i)];
        theta = VectorsAngle(Norm1,Norm2);
    end
    dV = sqrt((Vchabu(i+1)*sind(theta))^2+(Vchabu(i)-Vchabu(i+1)*cosd(theta))^2);
    Acheck2(i) = dV/T;
end
% Acheck2Max = max(Acheck2)
%% ��ͼ��
plot(Xs,Ys,'.')
xlabel('x�ᣬmm');
ylabel('y�ᣬmm');

subplot(3,1,2)
plot(Lchabu,Vchabu,'m');%�ٶ�-��������
hold on;
plot(L,V,'b');
xlabel('������mm');
ylabel('�ٶȣ�mm/s');

subplot(3,1,3)
plot(Lchabu(1,1:count_chabu-1),Acheck2/4000*400,'b');%ʸ�����ٶ�
hold on;
plot(Lchabu(1,1:count_chabu-1),Acheck1/4000*400,'g');%�������ٶ�
hold on;
plot(linspace(0,max(Lchabu),200),linspace(400,400,200),'r'); %�ο���
plot(linspace(0,max(Lchabu),200),linspace(-400,-400,200),'r'); %�ο���
xlabel('������mm');
ylabel('���ٶȣ�mm/s^2');
axis auto
%% ��������
disp('�岹�����');
disp(count_chabu)
disp('ǰհ���룺')
disp(Lm)