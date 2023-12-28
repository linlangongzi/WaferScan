function [ uk,vl,U0,V0 ] = SurfMeshParams( p0,q0,nu,nv,Xin0,Yin0,Zin0 )
%为全局曲面插值计算参数U,V,uk,vl
%   input:
%          Xin0,Yin0,Zin0：行为V对应的点，列为U对应的点
%计算uk,固定v方向
    num = nv+1;%非退化的列
    uk = zeros(1,nu+1);
    uk(nu+1) = 1;
    cds = zeros(max(nv,nu)+1);%临时变量，用来存放弦长
    for index_v = 0:nv     %即固定v方向的l
        total = 0;  %第1列的总弦长
        for  index_u =1:nu
            cds(index_u) = Distance3D([Xin0(index_u,index_v+1),Yin0(index_u,index_v+1),Zin0(index_u,index_v+1)],...
                                      [Xin0(index_u+1,index_v+1),Yin0(index_u+1,index_v+1),Zin0(index_u+1,index_v+1)]);
            %cds(k) = Distance3D(Q(k,3*l+1:3*l+3),Q(k+1,3*l+1:3*l+3));%弦长
           total = total +cds(index_u); %第1列的总弦长
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
        uk(index_u+1) = uk(index_u+1)/num;%平均化处理
    end
%计算vl，固定u方向
    nun = nu+1;%非退化的行  
    vl = zeros(1,nv+1);
    vl(nv+1) = 1;
   % cds = zeros(max(m,n)+1);%临时变量，用来存放弦长
    for index_u =0:nu      %即固定u方向的k
        total = 0;  %第1行的总弦长
        for  index_v = 0:nv-1
            cds(index_v+1) = Distance3D([Xin0(index_u+1,index_v+1),Yin0(index_u+1,index_v+1),Zin0(index_u+1,index_v+1)],[Xin0(index_u+1,index_v+2),Yin0(index_u+1,index_v+2),Zin0(index_u+1,index_v+2)]);
            %cds(l+1) = Distance3D(Q(k+1,3*l+1:3*l+3),Q(k+1,3*l+4:3*l+6));%弦长
           total = total +cds(index_v+1); %第1行的总弦长
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
        vl(index_v+1) = vl(index_v+1)/nun;%平均化处理
    end
    
% 确定节点矢量U
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

% 确定节点矢量V
    mv = nv+q0+1;%节点矢量V的个数mv+1
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

