function [ Xout, Yout, Zout ] = SurfaceCut( Xin, Yin, Zin )
%将曲面进行平滑及插值点均匀化处理
%处理的方法是对曲面进行多次重采样，每次采样采用等弧长采样

%% 计算x,y范围
[totalRow, totalCol] = size(Xin);


p = 3;
du = 0.001;
%按行处理
minX = max(Xin(:,1));
maxX = min(Xin(:,totalCol));
fprintf('插值点均匀化并切割处理：处理行 \n');
for row = 1 : totalRow

    [ ~,~,U,ctrlP] = GlobalCurveInterp( totalCol - 1, [Xin(row, :)', Yin(row, :)', Zin(row, :)'], p);
    %求曲线总长度，在[minX,maxX]区间中
    u = 0;
    lengthOfCurve = 0;
    xnow = CurvePoint( totalCol - 1, p, U, ctrlP(:,1), u );
    ynow = CurvePoint( totalCol - 1, p, U, ctrlP(:,2), u );
    znow = CurvePoint( totalCol - 1, p, U, ctrlP(:,3), u );
    while (u < 1)
        u = u + du;
        xnext = CurvePoint( totalCol - 1, p, U, ctrlP(:,1), u );
        ynext = CurvePoint( totalCol - 1, p, U, ctrlP(:,2), u );
        znext = CurvePoint( totalCol - 1, p, U, ctrlP(:,3), u );
        if (xnext >= minX && xnext <= maxX) 
            lengthOfCurve = lengthOfCurve + sqrt((xnext - xnow) ^ 2 + (ynext - ynow) ^ 2 + (znext - znow) ^ 2);
        end
        xnow = xnext;
        ynow = ynext;
        znow = znext;          
    end
    %divide to totalCol points
    deltaL = lengthOfCurve / (totalCol - 1);
    x = zeros(1, totalCol);
    y = zeros(1, totalCol);
    z = zeros(1, totalCol);
    dL = 0;
    u = 0;
    count = 0;
    xnow = CurvePoint( totalCol - 1, p, U, ctrlP(:,1), u );
    ynow = CurvePoint( totalCol - 1, p, U, ctrlP(:,2), u );
    znow = CurvePoint( totalCol - 1, p, U, ctrlP(:,3), u );
    while (u < 1 && count < totalCol)
        u = u + du;
        xnext = CurvePoint( totalCol - 1, p, U, ctrlP(:,1), u );
        ynext = CurvePoint( totalCol - 1, p, U, ctrlP(:,2), u );
        znext = CurvePoint( totalCol - 1, p, U, ctrlP(:,3), u );
        if (xnext >= minX && xnext <= maxX)
            dL = dL + sqrt((xnext - xnow) ^ 2 + (ynext - ynow) ^ 2 + (znext - znow) ^ 2);
        end
        while (dL >= deltaL * count && xnext >= minX)
            count = count + 1;
            x(count) = xnext;
            y(count) = ynext;
            z(count) = znext;
        end
        xnow = xnext;
        ynow = ynext;
        znow = znext;   
    end
    x(totalCol) = maxX;       %保证最后一个点的可靠性
    y(totalCol) = ynext;
    z(totalCol) = znext;
    Xin(row, :) = x;
    Yin(row, :) = y;
    Zin(row, :) = z;        
end
%按列处理
minY = max(Yin(1,:));
maxY = min(Yin(totalRow, :));
fprintf('插值点均匀化并切割处理：处理列 \n');
for col = 1 : totalCol
    [ ~,~,U,ctrlP] = GlobalCurveInterp( totalRow - 1, [Xin(:, col), Yin(:, col), Zin(:, col)], p);
    %求曲线总长度，在[minX,maxX]区间中
    u = 0;
    lengthOfCurve = 0;
    xnow = CurvePoint( totalRow - 1, p, U, ctrlP(:,1), u );
    ynow = CurvePoint( totalRow - 1, p, U, ctrlP(:,2), u );
    znow = CurvePoint( totalRow - 1, p, U, ctrlP(:,3), u );
    while (u < 1)
        u = u + du;
        xnext = CurvePoint( totalRow - 1, p, U, ctrlP(:,1), u );
        ynext = CurvePoint( totalRow - 1, p, U, ctrlP(:,2), u );
        znext = CurvePoint( totalRow - 1, p, U, ctrlP(:,3), u );
        if (ynext >= minY && ynext <= maxY)
            lengthOfCurve = lengthOfCurve + sqrt((xnext - xnow) ^ 2 + (ynext - ynow) ^ 2 + (znext - znow) ^ 2);
        end
        xnow = xnext;
        ynow = ynext;
        znow = znext;          
    end
    %divide to totalRow points
    deltaL = lengthOfCurve / (totalRow - 1);
    x = zeros(totalRow, 1);
    y = zeros(totalRow, 1);
    z = zeros(totalRow, 1);
    dL = 0;
    u = 0;
    count = 0;
    xnow = CurvePoint( totalRow - 1, p, U, ctrlP(:,1), u );
    ynow = CurvePoint( totalRow - 1, p, U, ctrlP(:,2), u );
    znow = CurvePoint( totalRow - 1, p, U, ctrlP(:,3), u );
    while (u < 1 && count < totalRow)
        u = u + du;
        xnext = CurvePoint( totalRow - 1, p, U, ctrlP(:,1), u );
        ynext = CurvePoint( totalRow - 1, p, U, ctrlP(:,2), u );
        znext = CurvePoint( totalRow - 1, p, U, ctrlP(:,3), u );
        if (ynext >= minY && ynext <= maxY) 
            dL = dL + sqrt((xnext - xnow) ^ 2 + (ynext - ynow) ^ 2 + (znext - znow) ^ 2);
        end
        while (dL >= deltaL * count && ynext >= minY)
            count = count + 1;
            x(count) = xnext;
            y(count) = ynext;
            z(count) = znext;
        end
        xnow = xnext;
        ynow = ynext;
        znow = znext;   
    end
    x(totalRow) = xnext;       %保证最后一个点的可靠性
    y(totalRow) = maxY;
    z(totalRow) = znext;
    Xin(:, col) = x;
    Yin(:, col) = y;
    Zin(:, col) = z;        
end

Xout = Xin;
Yout = Yin;
Zout = Zin;

end



