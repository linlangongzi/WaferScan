function [ Vout ] = V2step( Vin )
%���������ٶ����ߵ���������
n = length(Vin);
k = 0.8;
Vout = 1*Vin;
ns = 1;
for i = 2:n-1
    if Vin(i-1)<Vin(i) && Vin(i+1) <= Vin(i) %�ٴε����Ƹߵ����
%         disp('�Ƹߵ����')
%         disp(i)
        for j = ns:i-floor((i-ns)/3*2)
            Vout(j,1) = Vin(ns,1)*k;
        end 
        ns = i;
    end
    if Vin(i-1) <= Vin(i) && Vin(i+1)<Vin(i)  %�Ƹߵ��յ�
%         disp('�Ƹߵ��յ�');
%         disp(i)
        ns = i;
    end
    if Vin(i-1) >= Vin(i) && Vin(i+1) > Vin(i)  %��͵��յ�
%         disp('�Ƶ͵��յ�')
%         disp(i)
        for j = i-floor((i-ns)/3):i
            
            Vout(j,1) = Vin(i,1)*k;
        end
        ns = i;
    end

end

