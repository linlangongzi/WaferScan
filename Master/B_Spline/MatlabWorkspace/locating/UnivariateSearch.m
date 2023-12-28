function [ xout,error ] = UnivariateSearch( Ps,Pw )
%���������ֻ���Ѱ��Ps��Pw֮��ı任����Ps = A*Pw������������ϵPw�任����������ϵPs
x1 = zeros(1,6);
error = 0;
s1 = 0;
count = 0;
while 1
    for i = 1:6
        count = count + 1
        d = zeros(1,6);
        d(i) = 1;
        [lanpta,s2] = OneDimensionSearch(Ps,Pw,x1,d);
        x1 = x1 + lanpta * d;
        x1'
        if abs( s2 - s1 ) < 1e-8
            error = s2;
            xout = x1;
            break;
        end
        s1 = s2
    end
   if abs( s2 - s1 ) < 1e-8
        error = s2;
        xout = x1;
        break;
    end
end
end

