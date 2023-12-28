function [ ScanningNum,Xout,Yout,Zout,Mxout,Myout,Mzout ] = ...
    SurfaceRoutePlanning1( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval,thetaN,thetaL )
%�Შ���
%���ܽ��ܣ�������ɨ���������ɨ��ʽ·���滮���õ�ɨ����ж�Ӧ�ĵ����������ꡢ�Შָ��Mxout
%   input:
%       n,p,U,m,q,V,Xctrl,Yctrl,Zctrl�����������Ϣ
%       ScanInterval��ɨ���ߵȻ���ϸ�ּ��
%       JogInterval����v����Ȼ����������
%       thetaN���������䷽���뷨����N�ļнǣ�
%       thetaL���������䷽��������LMƽ���ͶӰ��ɨ����������L�ļнǣ�M = cross(L,N)
%   output:
%       SanningNum��1*n����ÿ��Ԫ�ض�Ӧÿ��ɨ���ߵ�ϸ�ֵ���
%       Xout,Yout,Zout,Nxout,Nyout,Nzout��n*m��ÿһ�ж�Ӧһ��ɨ���ߵ�ϸ�ֵ�����꼰������

%% ����ʼ
% Xtemp = zeros(1,1);
% Ytemp = zeros(1,1);
% Ztemp = zeros(1,1);
ScanningNum = zeros(1,1);
Xout = zeros(1,1);
Yout = zeros(1,1);
Zout = zeros(1,1);
Nxout = zeros(1,1);
Nyout = zeros(1,1);
Nzout = zeros(1,1);
Mxout = zeros(1,1);
Myout = zeros(1,1);
Mzout = zeros(1,1);
temp = zeros(1,2); %(u,v)��ʱ����
u = linspace(0,1,21);
for i = 1:21
    temp(i,:) = [u(i),0]; %��һ��ɨ���߶�Ӧ��(u,v)
end
[temp,~] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval );%���߰����ʴַ�
count_jog = 0; %��¼ɨ���ߵ�����
while min(temp(:,2))<1 %��˵Ļ���v=1������û���ߵ�
    fprintf('  Jog line No %d \n', count_jog+1);
    count_jog = count_jog + 1;
    [temp1,Ntemp] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval );%���߰�����ϸ��
    [num,~] =  size(temp1);
    num = num -1 ;
    ScanningNum(count_jog) = num;%��¼ÿ��ɨ����ϸ�ֵ����,ȥ��ÿ�е����һ����
    for j = 1:num + 1
        %����ϸ�ֵ�����꣬һ��ŵ�ϸ���㷨������ظ�������
        Xout(count_jog,j) = SurfacePoint( n,p,U,m,q,V,Xctrl,temp1(j,1),temp1(j,2) );
        Yout(count_jog,j) = SurfacePoint( n,p,U,m,q,V,Yctrl,temp1(j,1),temp1(j,2) );
        Zout(count_jog,j) = SurfacePoint( n,p,U,m,q,V,Zctrl,temp1(j,1),temp1(j,2) );
        Nxout(count_jog,j) = Ntemp(j,1);
        Nyout(count_jog,j) = Ntemp(j,2);
        Nzout(count_jog,j) = Ntemp(j,3);
        %����������
%         plot3([Xout(count_jog,j),Xout(count_jog,j)+100*Nxout(count_jog,j)],...
%               [Yout(count_jog,j),Yout(count_jog,j)+100*Nyout(count_jog,j)],...
%               [Zout(count_jog,j),Zout(count_jog,j)+100*Nzout(count_jog,j)],'r');
        hold on;
    end
   
    %����ɨ���ߺᲨ���䷽������Mtemp
    for j = 2:num + 1
        xtemp = Xout(count_jog, j) - Xout(count_jog, j - 1); 
        ytemp = Yout(count_jog, j) - Yout(count_jog, j - 1);
        ztemp = Zout(count_jog, j) - Zout(count_jog, j - 1);
        Ltemp = [xtemp, ytemp, ztemp];
        Ltemp = Ltemp / norm(Ltemp);    %��������
        Mtemp = cross(Ltemp, [Nxout(count_jog, j - 1), Nyout(count_jog, j - 1), Nzout(count_jog, j - 1)]);
        Mtemp = Mtemp / norm(Mtemp);    %����-���ߵĹ���������
        %�Შ���䷽�򣬵�N-Lƽ����
        Mxout(count_jog,j-1) = (sind(thetaN) * (cosd(thetaL) * Ltemp(1) + sind(thetaL) * Mtemp(1)) + cosd(thetaN) * Nxout(count_jog,j-1));
        Myout(count_jog,j-1) = (sind(thetaN) * (cosd(thetaL) * Ltemp(2) + sind(thetaL) * Mtemp(2)) + cosd(thetaN) * Nyout(count_jog,j-1));
        Mzout(count_jog,j-1) = (sind(thetaN) * (cosd(thetaL) * Ltemp(3) + sind(thetaL) * Mtemp(3)) + cosd(thetaN) * Nzout(count_jog,j-1));
        %�����Შ���������
        plot3([Xout(count_jog,j-1),Xout(count_jog,j-1) + 5 * Mxout(count_jog,j-1)],...
              [Yout(count_jog,j-1),Yout(count_jog,j-1) + 5 * Myout(count_jog,j-1)],...
              [Zout(count_jog,j-1),Zout(count_jog,j-1) + 5 * Mzout(count_jog,j-1)],'r');
        hold on;
    end
     %ѡ��1��num�����ݣ����Ա�֤��Щ���ݲ���0
    plot3(Xout(count_jog,1:num),Yout(count_jog,1:num),Zout(count_jog,1:num),'*-','Color','b');
    hold on; 
    text(Xout(count_jog,1),Yout(count_jog,1),Zout(count_jog,1),['ɨ����',num2str(count_jog)]);

    %���ֵַ�Ȼ�������
    temp = Jog(temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,JogInterval);
end
end

