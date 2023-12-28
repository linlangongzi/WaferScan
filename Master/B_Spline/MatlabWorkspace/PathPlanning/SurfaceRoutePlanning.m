function [ ScanningNum,Xout,Yout,Zout,Nxout,Nyout,Nzout ] = ...
    SurfaceRoutePlanning( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval,ScanInterval,JogInterval )
%���ܽ��ܣ�������ɨ���������ɨ��ʽ·���滮���õ�ɨ����ж�Ӧ�ĵ����������ꡢ������
%   input:
%       n,p,U,m,q,V,Xctrl,Yctrl,Zctrl�����������Ϣ
%       ScanInterval��ɨ���ߵȻ���ϸ�ּ��
%       JogInterval����v����Ȼ����������
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
temp = zeros(1,2); %(u,v)��ʱ����
u = linspace(0,1,21);
for i = 1:21
    temp(i,:) = [u(i),0]; %��һ��ɨ���߶�Ӧ��(u,v)
end
[temp,~] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,RoughScanInterval );%���߰����ʴַ�
count_jog = 0; %��¼ɨ���ߵ�����
while min(temp(:,2))<1 %��˵Ļ���v=1������û���ߵ�
    count_jog = count_jog + 1
    [temp1,Ntemp] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval );%���߰�����ϸ��
    [num,~] =  size(temp1);
    num = num -1 ;
    ScanningNum(count_jog) = num;%��¼ÿ��ɨ����ϸ�ֵ����,ȥ��ÿ�е����һ����
    for j = 1:num
        Xout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Xctrl,temp1(j,1),temp1(j,2) );
        %����ϸ�ֵ�����꣬һ��ŵ�ϸ���㷨������ظ�������
        Yout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Yctrl,temp1(j,1),temp1(j,2) );
        Zout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Zctrl,temp1(j,1),temp1(j,2) );
        Nxout(j,count_jog) = Ntemp(j,1);
        Nyout(j,count_jog) = Ntemp(j,2);
        Nzout(j,count_jog) = Ntemp(j,3);
        %����������
        plot3([Xout(j,count_jog),Xout(j,count_jog)+10*Nxout(j,count_jog)],...
              [Yout(j,count_jog),Yout(j,count_jog)+10*Nyout(j,count_jog)],...
              [Zout(j,count_jog),Zout(j,count_jog)+10*Nzout(j,count_jog)],'r');
        hold on;
    end
    %ѡ��1��num�����ݣ����Ա�֤��Щ���ݲ���0
    plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'k');
    hold on;
    plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'*');
    hold on;
    temp = Jog(temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,JogInterval);%���ֵַ�Ȼ�������
end
% ���һ���ߣ�v=1,��ɨ��
% count_jog = count_jog + 1
% [temp,Ntemp] = CurveSubdivide( temp,n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval );%���߰�����ϸ��
% [num,~] =  size(temp);
% num = num -1;
% ScanningNum(count_jog) = num;%��¼ÿ��ɨ����ϸ�ֵ������ȥ��ÿ�е����һ����
% for j = 1:num
%     Xout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Xctrl,temp1(j,1),temp1(j,2) );
%     Yout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Yctrl,temp1(j,1),temp1(j,2) );
%     Zout(j,count_jog) = SurfacePoint( n,p,U,m,q,V,Zctrl,temp1(j,1),temp1(j,2) );
%     Nxout(j,count_jog) = -Ntemp(j,1);
%     Nyout(j,count_jog) = -Ntemp(j,2);
%     Nzout(j,count_jog) = -Ntemp(j,3);
%     plot3([Xout(j,count_jog),Xout(j,count_jog)+20*Nxout(j,count_jog)],...
%           [Yout(j,count_jog),Yout(j,count_jog)+20*Nyout(j,count_jog)],...
%           [Zout(j,count_jog),Zout(j,count_jog)+20*Nzout(j,count_jog)],'r');
%     hold on;
% end
% %�������һ��ɨ���ߵķ�����
% plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'k');
% plot3(Xout(1:num,count_jog),Yout(1:num,count_jog),Zout(1:num,count_jog),'*'); 
end

