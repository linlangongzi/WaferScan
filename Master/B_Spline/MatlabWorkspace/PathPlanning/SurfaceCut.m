function [ Xout, Yout, Zout ] = SurfaceCut( Xin, Yin, Zin )
%���������ƽ������ֵ����Ȼ�����
%����ķ����Ƕ�������ж���ز�����ÿ�β������õȻ�������

%% ����x,y��Χ
[totalRow, totalCol] = size(Xin);


p = 3;
du = 0.001;
%���д���
minX = max(Xin(:,1));
maxX = min(Xin(:,totalCol));
fprintf('��ֵ����Ȼ����и�������� \n');
for row = 1 : totalRow

    [ ~,~,U,ctrlP] = GlobalCurveInterp( totalCol - 1, [Xin(row, :)', Yin(row, :)', Zin(row, :)'], p);
    %�������ܳ��ȣ���[minX,maxX]������
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
    x(totalCol) = maxX;       %��֤���һ����Ŀɿ���
    y(totalCol) = ynext;
    z(totalCol) = znext;
    Xin(row, :) = x;
    Yin(row, :) = y;
    Zin(row, :) = z;        
end
%���д���
minY = max(Yin(1,:));
maxY = min(Yin(totalRow, :));
fprintf('��ֵ����Ȼ����и�������� \n');
for col = 1 : totalCol
    [ ~,~,U,ctrlP] = GlobalCurveInterp( totalRow - 1, [Xin(:, col), Yin(:, col), Zin(:, col)], p);
    %�������ܳ��ȣ���[minX,maxX]������
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
    x(totalRow) = xnext;       %��֤���һ����Ŀɿ���
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



