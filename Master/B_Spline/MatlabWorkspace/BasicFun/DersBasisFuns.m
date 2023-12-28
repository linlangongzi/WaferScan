function [ ders ] = DersBasisFuns( span,u,p,n,U )
%�������B�������������䵼��
%˵����Ŀǰ������ȷ�ؼ����1�׵���
%   input:
%       span:Ϊu��U�еĽڵ�������±�span,U��MATLAB�е��±��1��ʼ
%       u:�������Ĳ���
%       p:B�������߽״Σ�p��p+1��
%       n:���Ƶ����n+1
%       U:�ڵ�ʸ��������Ϊm+1,m = n+p+1
%   output:
%       ders:ders(k,j)�洢���ǻ�����N(span-p+j,p)��k�׵���
%                     ÿһ�б�ʾĳһ�׵Ļ�������ÿһ�б�ʾĳһ�������ĸ��׵���
    if u>1
        u = 1;
    elseif u<0
        u = 0;
    end
   
    ndu = zeros(p+1,p+1);%��P52��һ�����������p�λ�����
    ndu(1,1) = 1;
    left = zeros(1,p+1);
    right = zeros(1,p+1);
    for j = 1:p
        left(j) = u - U(span+1-j);%
        right(j) = U(span+j) - u;
        saved = 0;
        for r = 0:j-1
            ndu(j+1,r+1) = right(r+1) + left(j-r);%���ڵ�֮�����������ǲ���,ʵ��
            temp = ndu(r+1,j)/ndu(j+1,r+1);
            ndu(r+1,j+1) = saved + right(r+1)*temp;%��B������������������������ǲ���
            saved = left(j-r)*temp;
        end
        ndu(j+1,j+1) = saved;%ndu��������Ч�ķ�����׻�����
    end
    
    ders = zeros(p+1,p+1);  %zeros(n+1,p+1)
    for j = 0:p  %�����������ֵ,ders��һ��Ϊ0�׵���
        ders(1,j+1) = ndu(j+1,p+1);
    end
    
%���ϳ��򾭹����飬û�����⣬������Ҫ����������%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%���������ӣ���2.1����2.2��һ�׺Ͷ��׵�����������ȷ��������Ϊ����û������
    %������㵼��������ʽ2.9
    for r = 0:p %�Ժ������±����ѭ��
        s1 = 0;
        s2 = 1; %�ı�����a����
        a = zeros(2,p+1);
        a(1,1) = 1;
        %ѭ������k�׵�����k=1,2,...,n
        for k =1:p      %1:n
            d = 0;
            rk = r-k;
            pk = p-k;
            if(r>=k)
                a(s2+1,1) = a(s1+1,1)/ndu(pk+2,rk+1);
                d = a(s2+1,1)*ndu(rk+1,pk+1);
            end
            
            if(rk>=-1)
                j1 = 1;
            else
                j1 = -rk;
            end
            
            if(r-1<=pk)
                j2 = k-1;
            else
                j2 = p-r;
            end
            
            for j = j1:j2
                a(s2+1,j+1) = (a(s1+1,j+1)-a(s1+1,j))/ndu(pk+2,rk+j+1);%�˴����±���Ҫע��
                d = d + a(s2+1,j+1)*ndu(rk+j+1,pk+1);
            end
            if(r<=pk)
                a(s2+1,k+1) = -a(s1+1,k)/ndu(pk+2,r+1);
                d = d + a(s2+1,k+1)*ndu(r+1,pk+1);
            end
            ders(k+1,r+1) = d;
            j = s1;
            s1 = s2;
            s2 = j;
        end
    end
    %�Խ��������ȷ�����ӣ�����2.9ʽ,�ò�����ȷ
    r = p;
    for k = 1:p         %1:n
        for j =0:p
            ders(k+1,j+1) = ders(k+1,j+1)*r;
        end
        r = r*(p-k);
    end    
end

