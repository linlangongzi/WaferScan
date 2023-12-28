function [ uv_out ] = Jog( uv_in,n,p,U,m,q,V,X,Y,Z,L )
%���������У���һ���ߣ���Ϣ���Լ�����Ŀ�����Ϣ����������
%�����һ�������У�����һ��ɨ����
%   input:
%       uv_in:��ǰ�Ŀ�������У�ÿһ��Ϊ(u,v)������Ϊ����num
%       n,p,U,m,q,V,X,Y,Z:���������Ϣ
%       L:��������
%   output:
%       uv_out:��һ��ɨ���߶�Ӧ�ĵ�
% disp('start the jog');
d = 1;
[num,dim] = size(uv_in);
uv_out = zeros(num,dim);
%%
for index = 1:num
    u0 = uv_in(index,1);
    v0 = uv_in(index,2);
    dis = 0;
    %���ڵȽ������ԣ��˴���DӦ��ȡ����(u,v)�ķ�����
     D = [0,1];     %%α�Ƚ�����ʵ������v����Ƚ���������������ȫ�ֵȽ���
    %�˷�ƫ���أ������ڵȲ�������
%     if index ==1 || index == num
%         D = [0,1];
%     else
%         D = uv_in(index+1,1:2)-uv_in(index-1,1:2);
%         D = [-D(2),D(1)]/norm(D);
%     end
    ddu = D(1)*0.0002;%�൱�ڽ�����5000��ϸ��
    ddv = D(2)*0.0002;  
    while( dis<L && v0<1 )
        x_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,X,u0,v0,d );
        y_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Y,u0,v0,d );
        z_temp = SurfaceDerivsAlg1( n,p,U,m,q,V,Z,u0,v0,d );
        dLsPower = (abs(x_temp(2,1)*ddu) + abs(x_temp(1,2)*ddv))^2 + ...
                   (abs(y_temp(2,1)*ddu) + abs(y_temp(1,2)*ddv))^2 + ...
                   (abs(z_temp(2,1)*ddu) + abs(z_temp(1,2)*ddv))^2;
        dis = dis + sqrt(dLsPower);
        u0 = u0 + ddu;
        v0 = v0 + ddv;
        if(v0>=1)
            v0 = 1;
        elseif v0<0
            v0 = 0;
        end
        if u0>1
            u0 = 1;
        elseif u0 < 0
            u0 = 0; 
        end
    end
   % plot3(x_now,y_now,z_now,'o');
    uv_out(index,:) = [u0,v0];
end
% uv_out(num,1) = 1;
%���������յ�
uv_out(1,1) = uv_in(1,1);
uv_out(num,1) = uv_in(num,1);
% disp('finish the jog');
end

