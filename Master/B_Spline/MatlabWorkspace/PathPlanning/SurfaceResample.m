function [ Sx, Sy, Sz ] = SurfaceResample( Sx, Sy, Sz, ku, kv )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
% 对有序点阵列进行曲线拟合 Sx,Sy,Sz
p = 3;
q = 3;
[n, m] = size(Sx);
%反求控制点部分,Q1 = zeros(n+1,3*(m+1));
[ U,V,Xctrl,Yctrl,Zctrl] = GlobalSurfInterpC1(n-1,m-1,Sx,Sy,Sz,p,q);
n = n + 2;
m = m + 2;
% 对曲面进行重新采样
disp('Resampling processing');
close
u = linspace(0,1,ku);
v = linspace(0,1,kv);
Sx = zeros(ku,kv);%点位坐标，列为u方向，行为v方向
Sy = zeros(ku,kv);
Sz = zeros(ku,kv);
for i = 1:ku
    for j = 1:kv
        Sx(i,j) = SurfacePoint(n-1,p,U,m-1,q,V,Xctrl,u(i),v(j));
        Sy(i,j) = SurfacePoint(n-1,p,U,m-1,q,V,Yctrl,u(i),v(j));
        Sz(i,j) = SurfacePoint(n-1,p,U,m-1,q,V,Zctrl,u(i),v(j));
    end
    plot3(Sx(i,:),Sy(i,:),Sz(i,:),'*-','Color','b');
    hold on;
end
% close
% plot_surface(Sx,Sy,Sz,'b')
text(Sx(1,1),Sy(1,1),Sz(1,1),'NO.1 resampling line')
hold on
text(Sx(floor(ku/2),1), Sy(floor(ku/2),1), Sz(floor(ku/2),1), 'Extern aera 1');
text(Sx(floor(ku/2),kv), Sy(floor(ku/2),kv), Sz(floor(ku/2),kv), 'Extern aera 2');
text(Sx(1,floor(kv/2)), Sy(1,floor(kv/2)), Sz(1,floor(kv/2)), 'Extern aera 3');
text(Sx(ku,floor(kv/2)), Sy(ku,floor(kv/2)), Sz(ku,floor(kv/2)), 'Extern aera 4');
axis equal
disp('Finish resample processing')

end

