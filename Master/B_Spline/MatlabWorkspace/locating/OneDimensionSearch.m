function [ lanpta,error ] = OneDimensionSearch( Ps,Pw,xin,d )
% ���ܣ�����һά�������������Ų�����ʹ��Ps-Pw֮�������ڷ���d�ϴﵽ��С
%input:
%       Ps,Pw:��������ϵ�µĵ��Ƽ�Ps,��������ϵ�µĵ��Ƽ�Pw
%       x:�����ĵ�ǰλ��,��һ��6ά����(a,b,c,x,y,z)
%       d:һά��������,�Ǹ�6ά����(a,b,c,x,y,z)
%output:
%       lanpta:����

%% ���ý��˷�ȷ����������[a,b]
k = 1; %��ʾ����1Ϊ����-1Ϊ����
h = 1/norm(d); %��̽����,�����ݶȷ�ϵ����0.2
lanpta1 = 0 - k * h ;
lanpta2 = lanpta1 + k * h; % ==0
lanpta3 = lanpta2 + k * h;
%ȷ����������
if ErrorDistance2(Ps,Pw,xin + 0 * d) > ErrorDistance2(Ps,Pw,xin + h * d)
    k = 1;%����
elseif  ErrorDistance2(Ps,Pw,xin + 0 * d) > ErrorDistance2(Ps,Pw,xin - h * d)
    k = -1;%����
end

while 1
    if ErrorDistance2(Ps,Pw,xin + lanpta2 * d) < ErrorDistance2(Ps,Pw,xin + lanpta1 * d) &&...
       ErrorDistance2(Ps,Pw,xin + lanpta2 * d) < ErrorDistance2(Ps,Pw,xin + lanpta3 * d)
        a = lanpta1;
        b = lanpta3;
        break;
    else
        lanpta1 = lanpta1 + k * h; %����ƽ��
        lanpta2 = lanpta1 + k * h;
        lanpta3 = lanpta2 + k * h;
    end
end

%% ����0.618����Fibonacci���������Ų���
while b - a > 1e-5
    a1 = a + (1-0.618) * (b-a);
    b1 = a + 0.618 * (b-a);
    if ErrorDistance2(Ps,Pw,xin + a1 * d) < ErrorDistance2(Ps,Pw,xin + b1 * d)
        b = b1;
        %a����
    elseif ErrorDistance2(Ps,Pw,xin + a1 * d) >= ErrorDistance2(Ps,Pw,xin + b1 * d)
        a = a1;
        %b����
    end
end
lanpta = (a+b) / 2;
error = ErrorDistance2(Ps,Pw,xin + lanpta * d);
end

