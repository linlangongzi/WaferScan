function [ ScanningNum,Xout,Yout,Zout,Mxout,Myout,Mzout ] = ...
    SurfaceRoutePlanning3( n,p,U,m,q,V,Xctrl,Yctrl,Zctrl, ScanNum, JogInterval,thetaN,thetaL )
%横波检测
%功能介绍：对曲面扫查进行逐行扫描式路径规划，得到扫描各行对应的点数、点坐标、横波指向Mxout
%规划策略：等参数进给，等张长插补。在此之前，先将控制点迭代均匀
%   input:
%       n,p,U,m,q,V,Xctrl,Yctrl,Zctrl：曲面控制信息
%       ScanNum：扫描线等弧长细分点数
%       JogInterval：沿v方向等弧长进给间隔
%       thetaN：声速入射方向与法向量N的夹角，
%       thetaL：声束入射方向向量在LM平面的投影与扫描线切向量L的夹角，M = cross(L,N)
%   output:
%       SanningNum：1*n矩阵，每个元素对应每条扫描线的细分点数
%       Xout,Yout,Zout,Nxout,Nyout,Nzout：n*m，每一行对应一条扫描线的细分点的坐标及法向量

    %% 计算参数曲面沿v方向的最大曲线长度Lvmax，决定沿v方向的细分个数，n = Lmax / JogInterval
    numTemp = 10;
    u = linspace(0,1,10);
    Lvmax = 0;
    duv = 1 / (20*ScanNum);
    for i = 1:numTemp
        v = 0;
        dL = 0;
        xtemp1 = SurfacePoint(n,p,U,m,q,V,Xctrl,u(i),v);
        ytemp1 = SurfacePoint(n,p,U,m,q,V,Yctrl,u(i),v);
        ztemp1 = SurfacePoint(n,p,U,m,q,V,Zctrl,u(i),v);
        while v < 1
            v = v + duv;            
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
    %% 沿v方向进给，计算各扫描线的扫描点，按等弧长细分
    numJog = floor(Lvmax / JogInterval); %沿v方向细分个数为numJog+1
    
    ScanningNum = zeros(numJog + 1, 1);
    Xout = zeros(numJog + 1, ScanNum);
    Yout = zeros(numJog + 1, ScanNum);
    Zout = zeros(numJog + 1, ScanNum);
    Mxout = zeros(numJog + 1, ScanNum);
    Myout = zeros(numJog + 1, ScanNum);
    Mzout = zeros(numJog + 1, ScanNum);
    Nxout = zeros(numJog + 1, ScanNum);
    Nyout = zeros(numJog + 1, ScanNum);
    Nzout = zeros(numJog + 1, ScanNum);
    
    v = linspace(0, 1, numJog + 1);
    for i = 1 : numJog + 1
        u = 0;
        fprintf('  Jog line No %d/%d \n', i, numJog +1);
        %求曲线总弧长
        totalL = 0;
        while u < 1
            xtemp1 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
            ytemp1 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
            ztemp1 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i));
            u = u + duv;
            xtemp2 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
            ytemp2 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
            ztemp2 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i)); 
            totalL = totalL + sqrt((xtemp2 - xtemp1)^ 2 + (ytemp2 - ytemp1)^ 2 +(ztemp2 - ztemp1)^ 2);
        end
        
        ScanInterval = totalL / (ScanNum - 1);
        u = 0;
        numScanningPoints = 1;
        dL = 0;
        [Pt,Nt] = NormVector(n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,u,v(i),1);%计算点的坐标及法向量
        Xout(i, numScanningPoints) = Pt(1);                                %第一个点
        Yout(i, numScanningPoints) = Pt(2);
        Zout(i, numScanningPoints) = Pt(3);   
        Nxout(i, numScanningPoints) = Nt(1);
        Nyout(i, numScanningPoints) = Nt(2);
        Nzout(i, numScanningPoints) = Nt(3);
        while u < 1 && numScanningPoints < ScanNum
            xtemp1 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
            ytemp1 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
            ztemp1 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i));
            while dL <= ScanInterval * numScanningPoints && u < 1
                u = u + duv;
                xtemp2 = SurfacePoint(n,p,U,m,q,V,Xctrl,u,v(i));
                ytemp2 = SurfacePoint(n,p,U,m,q,V,Yctrl,u,v(i));
                ztemp2 = SurfacePoint(n,p,U,m,q,V,Zctrl,u,v(i));
                dL = dL + sqrt((xtemp2 - xtemp1)^ 2 + (ytemp2 - ytemp1)^ 2 +(ztemp2 - ztemp1)^ 2);
                xtemp1 = xtemp2;
                ytemp1 = ytemp2;
                ztemp1 = ztemp2;
            end
            numScanningPoints = numScanningPoints + 1;
            [Pt,Nt] = NormVector(n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,u,v(i),1);%计算点的坐标及法向量
            Xout(i, numScanningPoints) = Pt(1);
            Yout(i, numScanningPoints) = Pt(2);
            Zout(i, numScanningPoints) = Pt(3);
            Nxout(i, numScanningPoints) = Nt(1);
            Nyout(i, numScanningPoints) = Nt(2);
            Nzout(i, numScanningPoints) = Nt(3);
            if numScanningPoints >= 2
                L = [Xout(i, numScanningPoints) - Xout(i, numScanningPoints-1),...
                     Yout(i, numScanningPoints) - Yout(i, numScanningPoints-1),...
                     Zout(i, numScanningPoints) - Zout(i, numScanningPoints-1)]; %利用差分求切向量L
                L = L / norm(L);
                N = [Nxout(i,numScanningPoints-1),Nyout(i,numScanningPoints-1),Nzout(i,numScanningPoints-1)];%法向量
                M = cross(N,L); %法向量与切向量的共正交向量
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
        [Pt,Nt] = NormVector(n,p,U,m,q,V,Xctrl,Yctrl,Zctrl,1,v(i),1);%计算点的坐标及法向量
        Xout(i, ScanNum) = Pt(1);
        Yout(i, ScanNum) = Pt(2);
        Zout(i, ScanNum) = Pt(3);
        Nxout(i, ScanNum) = Nt(1);
        Nyout(i, ScanNum) = Nt(2);
        Nzout(i, ScanNum) = Nt(3);
        L = [Xout(i, ScanNum) - Xout(i, ScanNum-1),...
             Yout(i, ScanNum) - Yout(i, ScanNum-1),...
             Zout(i, ScanNum) - Zout(i, ScanNum-1)]; %利用差分求切向量L
        L = L / norm(L);
        N = [Nxout(i,ScanNum),Nyout(i,ScanNum),Nzout(i,ScanNum)];%法向量
        M = cross(N,L); %法向量与切向量的共正交向量
        Mout = cosd(thetaN) * N + sind(thetaN) * ( cosd(thetaL) * L + sind(thetaL) * M );
        Mxout(i,ScanNum) = Mout(1);
        Myout(i,ScanNum) = Mout(2);
        Mzout(i,ScanNum) = Mout(3); 
%         plot3(Xout(i,:),Yout(i,:),Zout(i,:),'*-','Color','b');
%          text(Xout(i,1),Yout(i,1),Zout(i,1),['扫描线',num2str(i)]);
%         hold on;
        
        ScanningNum(i) = ScanNum; 
    end
end

