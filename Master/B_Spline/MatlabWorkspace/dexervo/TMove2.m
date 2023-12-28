function [ flag,n,Stable ] = TMove2( Ss,Se,Vs,Ve,Am,Vm,t )
%ָ����ʱ���T���ٶȹ滮����ʱ�����

S = Se-Ss;%λ�Ʋ�
T = 1e-3;%�岹����Ϊ1ms
n = floor(t/T); 
Stable = zeros(1,n);
flag = 0;
j = 0;
while  j<=4
    j = j + 1;
%% ���1 ����-����-����  �ȼ������֤
         delta = 4 * ( Am * t + Vs + Ve)^2 - 8 * ( Vs^2 + Ve^2 + 2 * Am * S );
         if delta < 0 %��������
             flag = 0;
%              error('plan fails')
         elseif delta == 0
             V1 = ( Am * t + Vs + Ve ) / 2;
             if ~(V1>=Vs && V1>=Ve && abs(V1)<= Vm && t-(V1-Vs)/Am - (Ve-V1)/(-Am)>=0)
                 flag = 0;
%                  error('plan fails')
             end
         else
             V(1) = (2 * ( Am * t + Vs + Ve) + sqrt(delta))/4;
             V(2) = (2 * ( Am * t + Vs + Ve) - sqrt(delta))/4;
             if V(1)>=Vs && V(1)>=Ve  && abs(V(1))<= Vm && t-(V(1)-Vs)/Am - (Ve-V(1))/(-Am)>=0
                 V1 = V(1);
                 flag = 1;
             elseif V(2)>=Vs && V(2)>=Ve && abs(V(2))<= Vm && t-(V(2)-Vs)/Am - (Ve-V(2))/(-Am)>=0
                 V1 = V(2);
                 flag = 1;
             else
                 flag = 0;
%                  error('plan fails')
             end  
         end  
         if flag == 1   %��֤�ɹ�
                t1 = (V1-Vs)/Am;
                t2 = t-(Ve-V1)/(-Am);
                t = 0;
                for i = 1:n
                    t = t + T;
                    if t<=t1
                        Stable(i) = Ss + Vs * t + 0.5 * Am * t^2;
                    elseif t>t1 && t<=t2
                        Stable(i) = (Ss + Vs * t1 + 0.5 * Am * t1^2) + V1 * (t - t1);
                    else
                        Stable(i) = (Ss + Vs * t1 + 0.5 * Am * t1^2) + V1 * (t2 - t1) + ...
                                    V1 * (t - t2) -...
                                    0.5 * Am * (t - t2)^2;
                    end
                end    
                break;
         end
%% ���2 ����-����-����  �ȼ������֤
        V1 = (2 * Am * S - Ve^2 + Vs^2 )/( 2 * Am * t + 2 * Vs - 2 * Ve );
        flag = 2;
        if ~(V1>=Vs && V1<=Ve  && abs(V1)<= Vm && t-(V1-Vs)/Am - (Ve-V1)/Am>=0)
             flag = 0;
%              error('plan fails')  
        end  
        if flag == 2    %��֤�ɹ�
            t1 = (V1-Vs)/Am;
            t2 = t-(Ve-V1)/Am;
            t = 0;
            for i = 1:n
                t = t + T;
                if t<=t1
                    Stable(i) = Ss + Vs * t + 0.5 * Am * t^2;
                elseif t>t1 && t<=t2
                    Stable(i) = (Ss + Vs * t1 + 0.5 * Am * t1^2) + V1 * (t - t1);
                else
                    Stable(i) = (Ss + Vs * t1 + 0.5 * Am * t1^2) + V1 * (t2 - t1) + ...
                                V1 * (t - t2) +...
                                0.5 * Am * (t - t2)^2;
                end
            end   
            break;
        end

%% ���3 ����-����-����  �ȼ������֤
        delta = 4 * ( Vs + Ve - Am * t )^2 - 8 * ( Ve^2 + Vs^2 -2 * Am * S);
        if delta < 0 
            flag = 0;
%             error('plan fails')
        elseif delta == 0
            V1 = ( Vs + Ve - Am * t ) / 2;
            if ~(V1<=Vs && V1<=Ve  && abs(V1)<= Vm && t-(V1-Vs)/(-Am) - (Ve-V1)/Am>=0)
                flag = 0;
%                 error('plan fails')
            end
        else
            V(1) = ( 2 * ( Vs + Ve - Am * t ) + sqrt(delta) ) / 4;
            V(2) = ( 2 * ( Vs + Ve - Am * t ) - sqrt(delta) ) / 4;
            if V(1)<=Vs && V(1)<=Ve  && abs(V(1))<= Vm && t-(V(1)-Vs)/(-Am) - (Ve-V(1))/Am>=0
                V1 = V(1);
                flag = 3;
            elseif V(2)<=Vs && V(2)<=Ve  && abs(V(2))<= Vm && t-(V(2)-Vs)/(-Am) - (Ve-V(2))/Am>=0
                V1 = V(2);
                flag = 3;
            else
                flag = 0;
%                 error('plan fails');
            end        
        end
        if flag == 3    %��֤�ɹ�
            t1 = (V1-Vs)/(-Am);
            t2 = t-(Ve-V1)/Am;
            t = 0;
            for i = 1:n
                t = t + T;
                if t<=t1
                    Stable(i) = Ss + Vs * t - 0.5 * Am * t^2;
                elseif t>t1 && t<=t2
                    Stable(i) = (Ss + Vs * t1 - 0.5 * Am * t1^2) + V1 * (t - t1);
                else
                    Stable(i) = (Ss + Vs * t1 - 0.5 * Am * t1^2) + V1 * (t2 - t1) + ...
                                V1 * (t - t2) +...
                                0.5 * Am * (t - t2)^2;
                end
            end 
            break;
        end

%% ���4 ����-����-����  �ȼ������֤
        V1 = (2 * Am * S + Ve^2 - Vs^2 )/( 2 * Am * t - 2 * Vs + 2 * Ve );
        flag = 4;
        if ~(V1<=Vs && V1>=Ve  && abs(V1)<= Vm && t-(V1-Vs)/(-Am) - (Ve-V1)/(-Am)>=0)
            flag = 0; 
%             error('plan fails')  
        end
        if flag == 4    %��֤�ɹ�
             t1 = (V1-Vs)/(-Am);
             t2 = t-(Ve-V1)/(-Am);
             t = 0;
            for i = 1:n
                t = t + T;
                if t<=t1
                    Stable(i) = Ss + Vs * t - 0.5 * Am * t^2;
                elseif t>t1 && t<=t2
                    Stable(i) = (Ss + Vs * t1 - 0.5 * Am * t1^2) + V1 * (t - t1);
                else
                    Stable(i) = (Ss + Vs * t1 - 0.5 * Am * t1^2) + V1 * (t2 - t1) + ...
                                V1 * (t - t2) -...
                                0.5 * Am * (t - t2)^2;
                end
            end   
            break;
        end
end
if flag == 0
    error('plan fails')
end
end