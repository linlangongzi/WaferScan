clear;
clc

x = [0,20,40,60,80,100,120];
nin = length(x)-1;
y = linspace(0,0,nin+1);
z = [0,20,0,-40,0,80,0]/2;

%% 画出曲线
Q = [x',z'];
[ p1,m,U,P] = GlobalCurveInterp( nin,Q,3);
k = 200;
u = linspace(0,1,k+1);
Cx1 = zeros(k+1,1);
Cz1 = zeros(k+1,1);
for i = 1:k+1
   Cx1(i) = CurvePoint(nin,p1,U,P(:,1), u(i));
   Cz1(i) = CurvePoint(nin,p1,U,P(:,2), u(i));
end

%% 等参数细分
k = 30;
u = linspace(0,1,k+1);
Cx = zeros(k+1,1);
Cz = zeros(k+1,1);
for i = 1:k+1
   Cx(i) = CurvePoint(nin,p1,U,P(:,1), u(i));
   Cz(i) = CurvePoint(nin,p1,U,P(:,2), u(i));
end
plot(Cx1,Cz1,'Color','k');%原曲线
hold on;
plot(Cx,Cz,'*','Color','k');%等参数细分点
hold on;

%% 等弧长细分
plot(Cx1,Cz1+30,'Color','b');%原曲线
hold on;
nout = 30;
[ Xout,Yout,Zout,L ] = CurveSubdivide2(x',y',z',nout );
plot(Xout,Zout+30,'*');
axis equal
xlabel('x(mm)')
ylabel('y(mm)')
% text(20,60,'blue: constant arc increment method')
% text(20,50,'black: iso-parametric increment method')
