function [ Pt,N ] = NormVector( n,p,U,m,q,V,X,Y,Z,u,v,d)
%   input:
%       n��u������Ƶ������n+1
%       p��u����B�����������״Σ�p��p+1��
%       U��u����ڵ�ʸ��
%       m��v������Ƶ������m+1
%       q��v����B�����������״Σ�q��q+1��
%       V��v����ڵ�ʸ��
%       X:��ά���X�����Ӧ����Ŀ��Ƶ�
%       Y:��ά���Y�����Ӧ����Ŀ��Ƶ�
%       Z:��ά���Z�����Ӧ����Ŀ��Ƶ�
%       u��u���򿼲����
%       v��v���򿼲����
%       d:ֱ��d�׵�ƫ��ʸ��ʵ����ֻ��Ҫ����0�ף�������ֵ����1��
%   output:
%       Pt:(u,v)��Ӧ����ά��(x,y,z)
%       N:(u,v)��Ӧ����ά��Pt��Ӧ�ķ�����

    X_temp = SurfaceDerivsAlg1(n,p,U,m,q,V,X,u,v,d);
    Y_temp = SurfaceDerivsAlg1(n,p,U,m,q,V,Y,u,v,d);
    Z_temp = SurfaceDerivsAlg1(n,p,U,m,q,V,Z,u,v,d);
    du = [X_temp(2,1),Y_temp(2,1),Z_temp(2,1)];%du = [dx/du,dy/du,dz/du]
    dv = [X_temp(1,2),Y_temp(1,2),Z_temp(1,2)];%dv = [dx/dv,dy/dv,dz/dv]
    
    N = cross(du,dv);%����ķ�����ΪN = du X dv�������
    N = N/norm(N);
    Pt = [X_temp(1,1),Y_temp(1,1),Z_temp(1,1)];%(u,v)��Ӧ�ĵ㣨x,y,z)
end

