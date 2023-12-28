function [U0,V0,Xctrl0,Yctrl0,Zctrl0] = GlobalSurfInterpC1( nu,nv,Xin0,Yin0,Zin0,p0,q0)
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

    [uk,vl,U0,V0] = SurfMeshParamsC1( p0,q0,nu,nv,Xin0,Yin0,Zin0 );
     mu = nu + p0 + 3;
     mv = nv + q0 + 3;
    
%% ���R
    Rx = zeros(nu+3,nv+1);
    Ry = zeros(nu+3,nv+1);
    Rz = zeros(nu+3,nv+1);
    for j = 0:nv
        %����ϵ������Nu
        Nu = zeros(nu+3,nu+3);
        span = FindSpan(nu+2,p0,uk(1),U0);
        Nu(1,span-p0:span) = BasisFuns(span,uk(1),p0,U0);
        Nu(2,1:2) =p0/U0(p0+2)* [-1,1]; %D0       
        for i = 3:nu+2
            span = FindSpan(nu+2,p0,uk(i-1),U0);
            Nu(i,span-p0:span) = BasisFuns(span,uk(i-1),p0,U0);   %
        end
        Nu(nu+2,nu+2:nu+3) = p0/(1-U0(mu-p0))*[-1,1]; %Dn
        span = nu+3; %FindSpan(n+2,p,uk(n+1),U)���˴�uk(n+1)=1,����U�ĵ�n+3��ֵ
        Nu(nu+3,span-p0:span) = BasisFuns(span,uk(nu+1),p0,U0);         
        %������ֵ��
        Xin1 = zeros(nu+3,1);
        Xin1(1,1) = Xin0(1,j+1);
        Xin1(2,1) = Xin0(2,j+1) - Xin0(1,j+1);  %�׶˵�һ�׵�ʸ����Ϊ�����ҷ���
        Xin1(3:nu+1,1) = Xin0(2:nu,j+1);
        Xin1(nu+2,1) = Xin0(nu+1,j+1) - Xin0(nu,j+1); %ĩ�˵�һ�׵�ʸ����Ϊ�����ҷ���
        Xin1(nu+3,1) = Xin0(nu+1,j+1);
        
        Yin1 = zeros(nu+3,1);
        Yin1(1,1) = Yin0(1,j+1);
        Yin1(2,1) = Yin0(2,j+1) - Yin0(1,j+1);  %�׶˵�һ�׵�ʸ����Ϊ�����ҷ���
        Yin1(3:nu+1,1) = Yin0(2:nu,j+1);
        Yin1(nu+2,1) = Yin0(nu+1,j+1) - Yin0(nu,j+1); %ĩ�˵�һ�׵�ʸ����Ϊ�����ҷ���
        Yin1(nu+3,1) = Yin0(nu+1,j+1);
        
        Zin1 = zeros(nu+3,1);
        Zin1(1,1) = Zin0(1,j+1);
        Zin1(2,1) = Zin0(2,j+1) - Zin0(1,j+1);  %�׶˵�һ�׵�ʸ����Ϊ�����ҷ���
        Zin1(3:nu+1,1) = Zin0(2:nu,j+1);
        Zin1(nu+2,1) = Zin0(nu+1,j+1) - Zin0(nu,j+1); %ĩ�˵�һ�׵�ʸ����Ϊ�����ҷ���
        Zin1(nu+3,1) = Zin0(nu+1,j+1);
        
        % ���ø�˹��Ԫ�����Ƶ�
        Rx(:,j+1) = gauss(Nu,Xin1);
        Ry(:,j+1) = gauss(Nu,Yin1);
        Rz(:,j+1) = gauss(Nu,Zin1);
    end
    
%% ���P
   Xctrl0 = zeros(nu+3,nv+3);
   Yctrl0 = zeros(nu+3,nv+3);
   Zctrl0 = zeros(nu+3,nv+3);
    for j = 0:nu+2
         %����ϵ������Nv
        Nv = zeros(nv+3,nv+3);
        span = FindSpan(nv+2,q0,vl(1),V0);
        Nv(1,span-q0:span) = BasisFuns(span,vl(1),q0,V0);
        Nv(2,1:2) =q0/V0(q0+2)* [-1,1]; %D0       
        for i = 3:nv+2
            span = FindSpan(nv+2,q0,vl(i-1),V0);
            Nv(i,span-q0:span) = BasisFuns(span,vl(i-1),q0,V0);   %
        end
        Nv(nv+2,nv+2:nv+3) = q0/(1-V0(mv-q0))*[-1,1]; %Dn
        span = nv+3; %FindSpan(n+2,p,uk(n+1),U)���˴�uk(n+1)=1,����U�ĵ�n+3��ֵ
        Nv(nv+3,span-q0:span) = BasisFuns(span,vl(nv+1),q0,V0);     
 
        %������ֵ��
        Rx1 = zeros(nv+3,1);
        Rx1(1,1) = Rx(j+1,1);
        Rx1(2,1) = Rx(j+1,2) - Rx(j+1,1);  %�׶˵�һ�׵�ʸ����Ϊ�����ҷ���
        Rx1(3:nv+1,1) = Rx(j+1,2:nv)';
        Rx1(nv+2,1) = Rx(j+1,nv+1) - Rx(j+1,nv); %ĩ�˵�һ�׵�ʸ����Ϊ�����ҷ���
        Rx1(nv+3,1) = Rx(j+1,nv+1);

        Ry1 = zeros(nv+3,1);
        Ry1(1,1) = Ry(j+1,1);
        Ry1(2,1) = Ry(j+1,2) - Ry(j+1,1);  %�׶˵�һ�׵�ʸ����Ϊ�����ҷ���
        Ry1(3:nv+1,1) = Ry(j+1,2:nv)';
        Ry1(nv+2,1) = Ry(j+1,nv+1) - Ry(j+1,nv); %ĩ�˵�һ�׵�ʸ����Ϊ�����ҷ���
        Ry1(nv+3,1) = Ry(j+1,nv+1);

        Rz1 = zeros(nv+3,1);
        Rz1(1,1) = Rz(j+1,1);
        Rz1(2,1) = Rz(j+1,2) - Rz(j+1,1);  %�׶˵�һ�׵�ʸ����Ϊ�����ҷ���
        Rz1(3:nv+1,1) = Rz(j+1,2:nv)';
        Rz1(nv+2,1) = Rz(j+1,nv+1) - Rz(j+1,nv); %ĩ�˵�һ�׵�ʸ����Ϊ�����ҷ���
        Rz1(nv+3,1) = Rz(j+1,nv+1);        
        
        
        Xctrl0(j+1,:) = (gauss(Nv,Rx1))';
        Yctrl0(j+1,:) = (gauss(Nv,Ry1))';
        Zctrl0(j+1,:) = (gauss(Nv,Rz1))';
    end   
end

