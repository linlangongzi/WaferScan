function [ uk,vl,U0,V0 ] = SurfMeshParams( p0,q0,nu,nv,Xin0,Yin0,Zin0 )
%Ϊȫ�������ֵ�������U,V,uk,vl
%   input:
%          Xin0,Yin0,Zin0����ΪV��Ӧ�ĵ㣬��ΪU��Ӧ�ĵ�
%����uk,�̶�v����
    num = nv+1;%���˻�����
    uk = zeros(1,nu+1);
    uk(nu+1) = 1;
    cds = zeros(max(nv,nu)+1);%��ʱ��������������ҳ�
    for index_v = 0:nv     %���̶�v�����l
        total = 0;  %��1�е����ҳ�
        for  index_u =1:nu
            cds(index_u) = Distance3D([Xin0(index_u,index_v+1),Yin0(index_u,index_v+1),Zin0(index_u,index_v+1)],...
                                      [Xin0(index_u+1,index_v+1),Yin0(index_u+1,index_v+1),Zin0(index_u+1,index_v+1)]);
            %cds(k) = Distance3D(Q(k,3*l+1:3*l+3),Q(k+1,3*l+1:3*l+3));%�ҳ�
           total = total +cds(index_u); %��1�е����ҳ�
        end
        if(total == 0)
            num = num -1;
        else
            d = 0;
            for index_u =1:nu-1
                d = d + cds(index_u)/total;
                uk(index_u+1) = uk(index_u+1) + d; 
            end
        end
    end
    for index_u =1:nu-1
        uk(index_u+1) = uk(index_u+1)/num;%ƽ��������
    end
%����vl���̶�u����
    nun = nu+1;%���˻�����  
    vl = zeros(1,nv+1);
    vl(nv+1) = 1;
   % cds = zeros(max(m,n)+1);%��ʱ��������������ҳ�
    for index_u =0:nu      %���̶�u�����k
        total = 0;  %��1�е����ҳ�
        for  index_v = 0:nv-1
            cds(index_v+1) = Distance3D([Xin0(index_u+1,index_v+1),Yin0(index_u+1,index_v+1),Zin0(index_u+1,index_v+1)],[Xin0(index_u+1,index_v+2),Yin0(index_u+1,index_v+2),Zin0(index_u+1,index_v+2)]);
            %cds(l+1) = Distance3D(Q(k+1,3*l+1:3*l+3),Q(k+1,3*l+4:3*l+6));%�ҳ�
           total = total +cds(index_v+1); %��1�е����ҳ�
        end
        if(total == 0)
            nun = nun -1;
        else
            d = 0;
            for index_v = 1:nv-1
                d = d +cds(index_v)/total;
                vl(index_v+1) = vl(index_v+1) +d;
            end
        end
    end
    for index_v = 1:nv-1
        vl(index_v+1) = vl(index_v+1)/nun;%ƽ��������
    end
    
% ȷ���ڵ�ʸ��U
    mu = nu+p0+1;
    U0 = zeros(1,mu+1);
    for i = 1:mu+1
        if(i<=p0+1)
            U0(i) = 0;
        elseif(i>=mu-p0+1)
            U0(i) = 1;
        else
            U0(i) = 1/p0*sum(uk(i-p0:i-1));
        end
    end    

% ȷ���ڵ�ʸ��V
    mv = nv+q0+1;%�ڵ�ʸ��V�ĸ���mv+1
    V0 = zeros(1,mv+1);
    for i = 1:mv+1
        if(i<=q0+1)
            V0(i) = 0;
        elseif(i>=mv-q0+1)
            V0(i) = 1;
        else
            V0(i) = 1/q0*sum(vl(i-q0:i-1));
        end
    end     
end

