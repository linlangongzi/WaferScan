function [ ScanningNum,Xout,Yout,Zout,Mxout,Myout,Mzout ] = ...
    SurfaceRoutePlanning2( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,ScanInterval,JogInterval,thetaN,thetaL )
%�Შ���
%���ܽ��ܣ�������ɨ���������ɨ��ʽ·���滮���õ�ɨ����ж�Ӧ�ĵ����������ꡢ�Შָ��Mxout
%�滮���ԣ��Ȳ����������Ȼ����岹���ڴ�֮ǰ���Ƚ����Ƶ��������
%   input:
%       n,p,U,m,q,V,Xctrl,Yctrl,Zctrl�����������Ϣ
%       ScanInterval��ɨ���ߵȻ���ϸ�ּ��
%       JogInterval����v����Ȼ����������
%       thetaN���������䷽���뷨����N�ļнǣ�
%       thetaL���������䷽��������LMƽ���ͶӰ��ɨ����������L�ļнǣ�M = cross(L,N)
%   output:
%       SanningNum��1*n����ÿ��Ԫ�ض�Ӧÿ��ɨ���ߵ�ϸ�ֵ���
%       Xout,Yout,Zout,Nxout,Nyout,Nzout��n*m��ÿһ�ж�Ӧһ��ɨ���ߵ�ϸ�ֵ�����꼰������

    %% �������������v�����������߳���Lvmax��������v�����ϸ�ָ�����n = Lmax / JogInterval
    numTemp = 10;
    u = linspace(0,1,10);
    Lvmax = 0;
    for i = 1:numTemp
        v = 0;
        dL = 0;
        xtemp1 = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v);
        ytemp1 = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v);
        ztemp1 = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v);
        while v < 1
            v = v + 0.005;            
            xtemp2 = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v);
            ytemp2 = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v);
            ztemp2 = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v);
            dL = dL + sqrt((xtemp2 - xtemp1)^ 2 + (ytemp2 - ytemp1)^ 2 + (ztemp2 - ztemp1)^ 2);
            xtemp1 = xtemp2;
            ytemp1 = ytemp2;
            ztemp1 = ztemp2;
        end
        Lvmax = max(Lvmax,dL);            
    end
    %% �������������u�����������߳���Lumax,������u�����ϸ�ָ�����m = Lumax / ScanInterval
        v = linspace(0,1,10);
    Lumax = 0;
    for i = 1:numTemp
        u = 0;
        dL = 0;
        xtemp1 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
        ytemp1 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
        ztemp1 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i));
        while u < 1
            u = u + 0.005;            
            xtemp2 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
            ytemp2 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
            ztemp2 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i));
            dL = dL + sqrt((xtemp2 - xtemp1)^ 2 + (ytemp2 - ytemp1)^ 2 + (ztemp2 - ztemp1)^ 2);
            xtemp1 = xtemp2;
            ytemp1 = ytemp2;
            ztemp1 = ztemp2;
        end
        Lumax = max(Lumax,dL);            
    end
    %% ��v��������������ɨ���ߵ�ɨ��㣬���Ȼ���ϸ��
    numJog = floor(Lvmax / JogInterval); %��v����ϸ�ָ���ΪnumJog+1
    numScan = floor(Lumax / ScanInterval);
    
    ScanningNum = zeros(numJog + 1, 1);
    Xout = zeros(numJog + 1, numScan + 1);
    Yout = zeros(numJog + 1, numScan + 1);
    Zout = zeros(numJog + 1, numScan + 1);
    Mxout = zeros(numJog + 1, numScan + 1);
    Myout = zeros(numJog + 1, numScan + 1);
    Mzout = zeros(numJog + 1, numScan + 1);
    Nxout = zeros(numJog + 1, numScan + 1);
    Nyout = zeros(numJog + 1, numScan + 1);
    Nzout = zeros(numJog + 1, numScan + 1);
    
    v = linspace(0, 1, numJog + 1);
    for i = 1 : numJog + 1
        u = 0;
        fprintf('  Jog line No %d/%d \n', i, numJog +1);
        numScanningPoints = 0;
        while u < 1
            xtemp1 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
            ytemp1 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
            ztemp1 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i));
            dL = 0;
            while dL <= ScanInterval && u < 1
                u = u + 0.001;
                xtemp2 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
                ytemp2 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
                ztemp2 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i));
                dL = dL + sqrt((xtemp2 - xtemp1)^ 2 + (ytemp2 - ytemp1)^ 2 +(ztemp2 - ztemp1)^ 2);
                xtemp1 = xtemp2;
                ytemp1 = ytemp2;
                ztemp1 = ztemp2;
            end
            numScanningPoints = numScanningPoints + 1;
            [Pt,Nt] = NormVector(n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,u,v(i),1);%���������꼰������
            Xout(i, numScanningPoints) = Pt(1);
            Yout(i, numScanningPoints) = Pt(2);
            Zout(i, numScanningPoints) = Pt(3);
            Nxout(i, numScanningPoints) = Nt(1);
            Nyout(i, numScanningPoints) = Nt(2);
            Nzout(i, numScanningPoints) = Nt(3);
            if numScanningPoints >= 2
                L = [Xout(i, numScanningPoints) - Xout(i, numScanningPoints-1),...
                     Yout(i, numScanningPoints) - Yout(i, numScanningPoints-1),...
                     Zout(i, numScanningPoints) - Zout(i, numScanningPoints-1)]; %���ò����������L
                L = L / norm(L);
                N = [Nxout(i,numScanningPoints-1),Nyout(i,numScanningPoints-1),Nzout(i,numScanningPoints-1)];%������
                M = cross(N,L); %���������������Ĺ���������
                Mout = cosd(thetaN) * N + sind(thetaN) * ( cosd(thetaL) * L + sind(thetaL) * M );
                Mxout(i,numScanningPoints-1) = Mout(1);
                Myout(i,numScanningPoints-1) = Mout(2);
                Mzout(i,numScanningPoints-1) = Mout(3);
%                 plot3([Xout(i,numScanningPoints-1),Xout(i,numScanningPoints-1) + 5 * Mxout(i,numScanningPoints-1)],...
%                   [Yout(i,numScanningPoints-1),Yout(i,numScanningPoints-1) + 5 * Myout(i,numScanningPoints-1)],...
%                   [Zout(i,numScanningPoints-1),Zout(i,numScanningPoints-1) + 5 * Mzout(i,numScanningPoints-1)],...
%                   '*-','Color','r');
%                 hold on
            end
        end
%         plot3(Xout(i,:),Yout(i,:),Zout(i,:),'*-','Color','b');
%          text(Xout(i,1),Yout(i,1),Zout(i,1),['ɨ����',num2str(i)]);
%         hold on;
        
        ScanningNum(i) = numScanningPoints - 1; 
    end
end
