function [ Xout, Yout, Zout ] = SurfaceUniform( Xin, Yin, Zin, times )
%将曲面进行平滑及插值点均匀化处理
%处理的方法是对曲面进行多次重采样，每次采样采用等弧长采样

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

