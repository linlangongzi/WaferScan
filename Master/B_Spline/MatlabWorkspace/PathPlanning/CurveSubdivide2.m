function [ Xout,Yout,Zout] = CurveSubdivide2(Xin,Yin,Zin,nout )
%对给定插值点进给全局曲线拟合后，按等弧长细分，细分段数为nout
%   input:
%       Xin,Yin,Zin:曲线采样点，均为n*1的矩阵
%       nout：计划细分的点的个数，n+1
%   output:
%       out,Yout,Zout：重新分割后的点，(n+1)*1
% disp('Start CurveSubdivide2');
Xout = zeros(nout+1,1);
Yout = zeros(nout+1,1);
Zout = zeros(nout+1,1);
Q = [Xin,Yin,Zin];
p = 3;%曲线插值基函数的阶数
[nin,~] = size(Xin);
[ ~,~,U,P] = GlobalCurveInterp(nin-1,Q,p);%反求控制点
Xctrl = P(:,1);%控制点的X,Y,Z坐标
Yctrl = P(:,2);
Zctrl = P(:,3);
%%
%求曲线长度
u = 0;
du = 0.001;
d = 1;%求导的阶次
L = 0;
while(u<1)
    x_temp = CurveDerivsAlg1(nin-1,p,U,Xctrl,u,d);
    y_temp = CurveDerivsAlg1(nin-1,p,U,Yctrl,u,d);
    z_temp = CurveDerivsAlg1(nin-1,p,U,Zctrl,u,d);
    dl = sqrt( (x_temp(2))^2 + (y_temp(2))^2 + (z_temp(2))^2 ) * du;%曲线段积分
    L = L + dl;
    u = u + du;
end
dL = L/nout;%nout+1个点，共有nout段
u = 0;
n_temp = 1;
L = 0;
while(u<1&&n_temp<nout)
    while(L<n_temp*dL)
        x_temp = CurveDerivsAlg1(nin-1,p,U,Xctrl,u,d);
        y_temp = CurveDerivsAlg1(nin-1,p,U,Yctrl,u,d);
        z_temp = CurveDerivsAlg1(nin-1,p,U,Zctrl,u,d);
        dl = (x_temp(2))^2 + (y_temp(2))^2 + (z_temp(2))^2;
        dl = sqrt(dl)*du;%曲线段积分 
        L = L+dl;
        u = u + du;
    end
    Xout(n_temp+1) = x_temp(1);
    Yout(n_temp+1) = y_temp(1);
    Zout(n_temp+1) = z_temp(1);
    n_temp = n_temp + 1;
end
Xout(1) = CurvePoint(nin-1,p,U,Xctrl,0);
Xout(nout+1) = CurvePoint(nin-1,p,U,Xctrl,1);
Yout(1) = CurvePoint(nin-1,p,U,Yctrl,0);
Yout(nout+1) = CurvePoint(nin-1,p,U,Yctrl,1);
Zout(1) = CurvePoint(nin-1,p,U,Zctrl,0);
Zout(nout+1) = CurvePoint(nin-1,p,U,Zctrl,1);
% disp('Done');
end

