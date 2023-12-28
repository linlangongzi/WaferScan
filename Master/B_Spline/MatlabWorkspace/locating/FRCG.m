function [ xout,error ] = FRCG( Ps,Pw )
%���ܣ�����FR�����ݶȷ��������ļ�Сֵ�����ҵ���Ӧ�ı任�����6������
%input:
%       Ps,Pw:��������ϵ�µĵ��Ƽ�Ps,��������ϵ�µĵ��Ƽ�Pw
%       xin:�����ĳ�ʼλ��,��һ��6ά����(a,b,c,x,y,z),ֱ��ȡzeros(1,6)
%output:
%       xout:��Сֵ���Ӧ�ı任����Ĳ���xout=[a,b,c,x,y,z]

x1 = zeros(1,6);
s1 = ErrorDistance2(Ps,Pw,x1);
t1 = zeros(1,6); %�ݶ�
t2 = zeros(1,6);
for i = 1:6 %�����ݶȼ�
    x1 = zeros(1,6);
    e = zeros(1,6);
    e(i) = 1e-3;
    s = ErrorDistance2(Ps,Pw,x1 + e);
    t1(i) = (s - s1)/e(i); %�ݶȣ���ʼ��������
end
count = 0;
count1 = 0;
% t1 = t1 / norm(t1);
d = -t1;
while count1<100 
    [lanpta,s2] = OneDimensionSearch(Ps,Pw,x1,d);
    x2 = x1 + lanpta * d;
%     if abs(s2-s1) < 1e-5 % ��������10ʱ����Ϊ�Ѿ��㹻��ȷ��
%         break;
%     end
    count = count + 1;
    count1 = count1 + 1
    for i = 1:6 %�����ݶ�
        e = zeros(1,6);
        e(i) = 1e-3;
        s = ErrorDistance2(Ps,Pw,x2 + e);
        t2(i) = (s - s2)/e(i); %�ݶȣ���ʼ��������
    end
%     t2 = t2 / norm(t2);
    if count == 6
        count = 0;
        d = -t2;
    else
%         alfa = t2 * (t2 - t1)' / (t1 * t1') ; %PRP��ʽ
%         alfa = t2 * t2' / (d * t1');           %DM��ʽ,�����ٶ�����
        alfa = t2 * t2' /( t1 * t1');        %FR��ʽ
        d = -t2 + alfa * d;
    end
    x1 = x2;
    s1 = s2
    t1 = t2;
    x2'
end
xout = x2;
error = s2;
end

