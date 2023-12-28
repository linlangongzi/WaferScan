function [ xout,error ] = UnivariateSearch( Ps,Pw )
%采用坐标轮换法寻找Ps和Pw之间的变换矩阵，Ps = A*Pw，将工件坐标系Pw变换到机床坐标系Ps
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

