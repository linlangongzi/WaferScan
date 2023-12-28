function [ SKL ] = SurfaceDerivsAlg1( n,p,U,m,q,V,P,u,v,d )
%����B���������ϵĵ㼰������ֱ��d�׵�ƫ��ʸ
%��ʵ�ʹ�����ֻ��Ҫ���Ե�1��ƫ��ʸ�����ݶ�
%   input:
%       n��u������Ƶ������n+1
%       p��u����B�����������״Σ�p��p+1��
%       U��u����ڵ�ʸ��
%       m��v������Ƶ������m+1
%       q��v����B�����������״Σ�q��q+1��
%       V��v����ڵ�ʸ��
%       P����ά���Ƶ㣬(n+1)*(m+1)��ʵ����ֻ����(1+p)*(1+q)��δ���������NΪ0��λ��
%       u��u���򿼲����
%       v��v���򿼲����
%       d:ֱ��d�׵�ƫ��ʸ��ʵ����ֻ��Ҫ����0�ף�������ֵ����1��
%   output:
%       SKL��SKL[K][L]����S(u,v)��u,v����ֱ���k�κ�l�����õ�ƫ��ʸ��k/l��������d
    
    du = min(d,p);%��d>pʱ������ʸΪ0
    dv = min(d,q);
    SKL = zeros(du+1,dv+1);
%�¶γ�����Ҫ��SKL����ʱĬ����δ��ֵ��Ԫ�ؾ�Ϊ0 
%     for k = p+1:d
%        for l = 0:d-k
%            SKL(k+1,l+1) = 0;%p+1��u����ƫ����Ϊ0
%        end       
%     end
%     for l = q+1:d
%         for k = 0:d-l
%             SKL(k+1,l+1) = 0;%q+1��v����ƫ����Ϊ0
%         end
%     end
    if u>=1 
        uspan = n+1;
    else
        uspan = FindSpan(n,p,u,U);
    end
    Nu = DersBasisFuns(uspan,u,p,du,U);%��һ��Ϊ���߶�Ӧ��ֵ���ڶ���Ϊһ�׵�ʸ
    if v>=1
        vspan = m+1;
    else
        vspan = FindSpan(m,q,v,V);
    end
    Nv = DersBasisFuns(vspan,v,q,dv,V);%��һ��Ϊ���߶�Ӧ��ֵ���ڶ���Ϊһ�׵�ʸ
    for k = 0:du%�൱�ڹ̶�u
        temp = zeros(1,q+1);
        for s = 0:q
            for r = 0:p
                temp(s+1) = temp(s+1) + Nu(k+1,r+1)*P(uspan-p+r,vspan-q+s);
            end
        end
        dd = min(d-k,dv);
        for l = 0:dd
%             SKL(k+1,l+1) = 0;
            for s = 0:q
                SKL(k+1,l+1) = SKL(k+1,l+1) + Nv(l+1,s+1)*temp(s+1);
            end
        end
    end
end

