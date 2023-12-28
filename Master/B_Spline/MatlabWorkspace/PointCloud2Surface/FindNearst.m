function [ pOut, flagOut, pos ] = FindNearst( pointCloud, pIn, flagIn )
%�����������pointCloud�е�pIn�������
    minDis = 100000;
    minPos = 0;
    for j = 1 : length(pointCloud)
       dis = Distance3D([pointCloud(j, 1:2), 0], pIn);
       if (dis < minDis && flagIn(j) == 0)
           minDis = dis;
           pOut = pointCloud(j, :);
           minPos = j;
       end        
    end
    flagIn(minPos) = 1;
    flagOut = flagIn;
    pos = minPos;

end

