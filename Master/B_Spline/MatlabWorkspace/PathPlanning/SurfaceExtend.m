function [ Sxout,Syout,Szout ] = SurfaceExtend( Sxin,Syin,Szin,extern,pos )
%进曲面向四周进行延展Extern的宽度
%pos表示哪个方向进行延展，一共4个方向
%pos = 0:4个方向全部延展
%pos = 1:左侧延展
%pos = 2:右侧延展
%pos = 3:上侧延展
%pos = 4:下侧延展
[m, n] = size(Sxin);
switch pos
    case 0
        Sxout = zeros(m+2, n+2);
        Syout = zeros(m+2, n+2);
        Szout = zeros(m+2, n+2);
        for i = 1 : m
           p1 = [Sxin(i,1),Syin(i,1),Szin(i,1)];             %左侧延展
           p2 = [Sxin(i,2),Syin(i,2),Szin(i,2)];
           p =  p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(i+1, 1) = p(1);
           Syout(i+1, 1) = p(2);
           Szout(i+1, 1) = p(3);
           p1 = [Sxin(i,n),Syin(i,n),Szin(i,n)];              %右侧延展
           p2 = [Sxin(i,n-1),Syin(i,n-1),Szin(i,n-1)];       
           p = p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(i+1, n+2) = p(1);
           Syout(i+1, n+2) = p(2);
           Szout(i+1, n+2) = p(3);
        end
        for i = 1 : n
           p1 = [Sxin(1,i),Syin(1,i),Szin(1,i)];             %上侧延展
           p2 = [Sxin(2,i),Syin(2,i),Szin(2,i)];
           p =  p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(1, i+1) = p(1);
           Syout(1, i+1) = p(2);
           Szout(1, i+1) = p(3);       
           p1 = [Sxin(m,i),Syin(m,i),Szin(m,i)];             %下侧延展
           p2 = [Sxin(m-1,i),Syin(m-1,i),Szin(m-1,i)];
           p =  p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(m+2, i+1) = p(1);
           Syout(m+2, i+1) = p(2);
           Szout(m+2, i+1) = p(3);  
        end
        p1 = [Sxout(1,2),Syout(1,2),Szout(1,2)];             %左上角延展
        p2 = [Sxout(1,3),Syout(1,3),Szout(1,3)];
        p =  p1 + extern * (p1 - p2) / norm(p1-p2);
        Sxout(1,1) = p(1);
        Syout(1,1) = p(2);
        Szout(1,1) = p(3);
        p1 = [Sxout(1,n+1),Syout(1,n+1),Szout(1,n+1)];             %右上角延展
        p2 = [Sxout(1,n),Syout(1,n),Szout(1,n)];
        p =  p1 + extern * (p1 - p2) / norm(p1-p2);
        Sxout(1,n+2) = p(1);
        Syout(1,n+2) = p(2);
        Szout(1,n+2) = p(3);     
        p1 = [Sxout(m+2,2),Syout(m+2,2),Szout(m+2,2)];             %左下角延展
        p2 = [Sxout(m+2,3),Syout(m+2,3),Szout(m+2,3)];
        p =  p1 + extern * (p1 - p2) / norm(p1-p2);
        Sxout(m+2,1) = p(1);
        Syout(m+2,1) = p(2);
        Szout(m+2,1) = p(3);
        p1 = [Sxout(m+2,n+1),Syout(m+2,n+1),Szout(m+2,n+1)];             %右下角延展
        p2 = [Sxout(m+2,n),Syout(m+2,n),Szout(m+2,n)];
        p =  p1 + extern * (p1 - p2) / norm(p1-p2);
        Sxout(m+2,n+2) = p(1);
        Syout(m+2,n+2) = p(2);
        Szout(m+2,n+2) = p(3);    
        %中间部分
        for i = 1:m
            for j = 1:n
                Sxout(i + 1, j + 1) = Sxin(i,j);
                Syout(i + 1, j + 1) = Syin(i,j);
                Szout(i + 1, j + 1) = Szin(i,j);
            end
        end
    case 1  
        %左侧延展
        Sxout = zeros(m, n+1);
        Syout = zeros(m, n+1);
        Szout = zeros(m, n+1);
        for i = 1 : m
           p1 = [Sxin(i,1),Syin(i,1),Szin(i,1)];
           p2 = [Sxin(i,2),Syin(i,2),Szin(i,2)];
           p =  p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(i, 1) = p(1);
           Syout(i, 1) = p(2);
           Szout(i, 1) = p(3);
        end
        %未延展区
        for i = 1:m
            for j = 1:n
                Sxout(i, j + 1) = Sxin(i,j);
                Syout(i, j + 1) = Syin(i,j);
                Szout(i, j + 1) = Szin(i,j);
            end
        end         
    case 2  
        %右侧延展
        Sxout = zeros(m, n+1);
        Syout = zeros(m, n+1);
        Szout = zeros(m, n+1);
        for i = 1: m
           p1 = [Sxin(i,n),Syin(i,n),Szin(i,n)];
           p2 = [Sxin(i,n-1),Syin(i,n-1),Szin(i,n-1)];       
           p = p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(i, n+1) = p(1);
           Syout(i, n+1) = p(2);
           Szout(i, n+1) = p(3);
        end
        %未延展区
        for i = 1:m
            for j = 1:n
                Sxout(i, j) = Sxin(i,j);
                Syout(i, j) = Syin(i,j);
                Szout(i, j) = Szin(i,j);
            end
        end         
    case 3  
        %上侧延展
        Sxout = zeros(m+1, n);
        Syout = zeros(m+1, n);
        Szout = zeros(m+1, n);  
        for i = 1 : n
           p1 = [Sxin(1,i),Syin(1,i),Szin(1,i)];             %上侧延展
           p2 = [Sxin(2,i),Syin(2,i),Szin(2,i)];
           p =  p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(1, i) = p(1);
           Syout(1, i) = p(2);
           Szout(1, i) = p(3); 
        end
        %未延展区
        for i = 1:m
            for j = 1:n
                Sxout(i+1, j) = Sxin(i,j);
                Syout(i+1, j) = Syin(i,j);
                Szout(i+1, j) = Szin(i,j);
            end
        end  
    case 4  
        %下侧延展
        Sxout = zeros(m+1, n);
        Syout = zeros(m+1, n);
        Szout = zeros(m+1, n);
        for i = 1: n
           p1 = [Sxin(m,i),Syin(m,i),Szin(m,i)];             %下侧延展
           p2 = [Sxin(m-1,i),Syin(m-1,i),Szin(m-1,i)];
           p =  p1 + extern * (p1 - p2) / norm(p1-p2);
           Sxout(m+1, i) = p(1);
           Syout(m+1, i) = p(2);
           Szout(m+1, i) = p(3);  
        end
        for i = 1:m
            for j = 1:n
                Sxout(i, j) = Sxin(i,j);
                Syout(i, j) = Syin(i,j);
                Szout(i, j) = Szin(i,j);
            end
        end  
    otherwise
end

end

