function [ flag, I ] = IntersectRaySTL( ray, triGroup)
    countMax = length(triGroup) / 3;
    count = 1;
    flag = 0;
    while 1
       triangle = triGroup(:,count*3-2:count*3);
       [flag,I] = IntersectRayTriangle(ray,triangle);
        if (flag == 1 || count >= countMax - 1)
            break;
        end
        count = count + 1;
    end
end

