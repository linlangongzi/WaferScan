function [ Xout, Yout, Zout ] = SurfaceUniform( Xin, Yin, Zin, times )
%���������ƽ������ֵ����Ȼ�����
%����ķ����Ƕ�������ж���ز�����ÿ�β������õȻ�������

[totalRow, totalCol] = size(Xin);
for index = 1 : times
    fprintf(' Surface uniform: %d/%d \n',index, times);
    for row = 1 : totalRow
        [x, y, z] = CurveSubdivide2(Xin(row, :)', Yin(row, :)', Zin(row, :)',totalCol - 1 );
        Xin(row, :) = x';
        Yin(row, :) = y';
        Zin(row, :) = z';        
    end
   for col = 1 : totalCol
        [x, y, z] = CurveSubdivide2(Xin(:, col), Yin(:, col), Zin(:, col),totalRow - 1 );
        Xin(:, col) = x;
        Yin(:, col) = y;
        Zin(:, col) = z;
    end
end
Xout = Xin;
Yout = Yin;
Zout = Zin;

end

