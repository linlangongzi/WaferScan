function [U0,V0,Xctrl0,Yctrl0,Zctrl0] = GlobalSurfInterp( nu,nv,Xin0,Yin0,Zin0,p0,q0)
%ȫ�������ֵ��ָ����ֵ�㣬����ڵ�ʸ�������Ƶ�
%   input:
%       n:u�����ֵ�����,n+1
%       m:v�����ֵ�����,m+1
%       Xin,Yin.Zin:(n+1)*(m+1)�ľ��󣬶�Ӧ(n+1)*(m+1)����ֵ���x,y,z����
%       p:u�������߽���
%       q:v�������߽���
%   output:
%       U:u����ڵ�ʸ��
%       V:v����ڵ�ʸ��
%       Xctrl,Yctrl,Zctrl:(n+1)*(m+1)�ľ��󣬶�Ӧ(n+1)*(m+1)�����Ƶ��x,y,z����
%       Nu:u�����ϵ������
%       Nv:v�����ϵ������

    [uk,vl,U0,V0] = SurfMeshParams( p0,q0,nu,nv,Xin0,Yin0,Zin0 );

%% ����ϵ������Nu
    Nu = zeros(nu+1,nu+1);
    for i = 0:nu
        span = FindSpan(nu,p0,uk(i+1),U0);
        Nu(i+1,span-p0:span) = BasisFuns(span,uk(i+1),p0,U0);   %
    end
    
%% ���R
    Rx = zeros(nu+1,nv+1);
    Ry = zeros(nu+1,nv+1);
    Rz = zeros(nu+1,nv+1);
    for j = 0:nv
        % ���ø�˹��Ԫ�����Ƶ�
        Rx(:,j+1) = gauss(Nu,Xin0(:,j+1));
        Ry(:,j+1) = gauss(Nu,Yin0(:,j+1));
        Rz(:,j+1) = gauss(Nu,Zin0(:,j+1));
    end
    
%% ����ϵ������Nv
    Nv = zeros(nv+1,nv+1);
    for i = 0:nv
        span = FindSpan(nv,q0,vl(i+1),V0);
        Nv(i+1,span-q0:span) = BasisFuns(span,vl(i+1),q0,V0);
    end
    
%% ���P
   Xctrl0 = zeros(nu+1,nv+1);
   Yctrl0 = zeros(nu+1,nv+1);
   Zctrl0 = zeros(nu+1,nv+1);
    for j = 0:nu
        Xctrl0(j+1,:) = (gauss(Nv,Rx(j+1,:)'))';
        Yctrl0(j+1,:) = (gauss(Nv,Ry(j+1,:)'))';
        Zctrl0(j+1,:) = (gauss(Nv,Rz(j+1,:)'))';
    end   
end

