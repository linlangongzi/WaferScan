function [ uv_out ] = Jog( uv_in,n,p,U,m,q,V,X,Y,Z,L )
%给定点序列（即一条线）信息，以及曲面的控制信息，进给步长
%求出下一条点序列，即下一条扫描线
%   input:
%       uv_in:当前的考查点序列，每一行为(u,v)，行数为点数num
%       n,p,U,m,q,V,X,Y,Z:曲面控制信息
%       L:进给步长
%   output:
%       uv_out:下一条扫描线对应的点
% disp('start the jog');
d = 1;
[num,dim] = size(uv_in);
uv_out = zeros(num,dim);
%%
for index = 1:num
    u0 = uv_in(index,1);
    v0 = uv_in(index,2);
    dis = 0;
    %对于等进给而言，此处的D应该取各点(u,v)的法向量
     D = [0,1];     %%伪等进给，实际上是v方向等进给，而不是曲线全局等进给
    %此法偏保守，但优于等参数进给
%     if index ==1 || index == num
%         D = [0,1];
%     else
%         D = uv_in(index+1,1:2)-uv_in(index-1,1:2);
%         D = [-D(2),D(1)]/norm(D);
%     end
    ddu = D(1)*0.0002;%相当于进行了5000份细分
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
%修正起点和终点
uv_out(1,1) = uv_in(1,1);
uv_out(num,1) = uv_in(num,1);
% disp('finish the jog');
end

