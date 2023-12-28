function [ S ] = SurfacePoint( n,p,U,m,q,V,P,u,v )
%����B���������ϵĵ㣨u,v)
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
%   output:
%       S��������(u,v)����ֵ
    if u>=1
        uspan = n+1;
    else
        uspan = FindSpan(n,p,u,U);
    end
    Nu = BasisFuns(uspan,u,p,U);%�������ֵu��Ӧ�����з���B������������ֵ
    if v>=1
        vspan = m+1;
    else
        vspan = FindSpan(m,q,v,V);
    end
    Nv = BasisFuns(vspan,v,q,V);%�������ֵv��Ӧ�����з���B������������ֵ
    uind = uspan - p;
    temp = zeros(1,q+1);
    S = 0;
    for l = 0:q
        temp(l+1) = 0.0;        %������v�ϵĵ�̶�����Ѱ��u��Ӧ��ֵ
        vind = vspan - q +l;
        for k = 0:p
            temp(l+1) = temp(l+1) +Nu(k+1)*P(uind+k,vind);
        end
    end

    for l = 0:q
        S = S + Nv(l+1)*temp(l+1);
    end
end

