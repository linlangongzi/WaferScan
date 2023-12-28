function [ theta ] = CalculateAngle( p0, p1, L1, L2 )
% 计算连杆结构的夹角，p0为舵机原点，p1为动平台上的万向节中心点，L1为舵盘半径，L2为杆长
    theta = zeros(1,2);
    dp = p1 - p0;
    dx = dp(1);
    dy = dp(2);
    dz = dp(3);
    if (dx ~=0)
        temp = -(L2^2 - dx^2 - dy^2 - dz^2 - L1^2) / (2 * dx * L1);                 %(L2^2 - dx^2 - dy^2 - dz^2 - L1^2) / (2 * dx * L1);，朝左
    else
        stheta = (L1^2 + dx^2 + dy^2 + dz^2 - L2^2) / (2 * dz * L1);
        theta(1) = asin(stheta) * 180 / pi;
        theta(2) = theta(1);
        return;
    end
    a = dz^2 / dx^2 + 1;
    b = -2 * temp * dz / dx;                                                    %2 * temp * dz / dx， 朝左
    c = temp^2 - 1;
    if (b^2 - 4*a*c < 0) 
        theta(1) = 91;
        theta(2) = 91;
        return
    else
        stheta = (-b - (b^2 - 4*a*c)^0.5) / (2 * a);
        if stheta > -1 && stheta < 1
            theta(1) = asin(stheta) * 180 / pi;                                 %回代验证一下即可
        end
        stheta = (-b + (b^2 - 4*a*c)^0.5) / (2 * a);
        if stheta > -1 && stheta < 1
            theta(2) = asin(stheta) * 180 / pi;
        end
    end

end

