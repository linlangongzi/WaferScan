function [ theta ] = CalculateAngle( p0, p1, L1, L2 )
% �������˽ṹ�ļнǣ�p0Ϊ���ԭ�㣬p1Ϊ��ƽ̨�ϵ���������ĵ㣬L1Ϊ���̰뾶��L2Ϊ�˳�
    theta = zeros(1,2);
    dp = p1 - p0;
    dx = dp(1);
    dy = dp(2);
    dz = dp(3);
    if (dx ~=0)
        temp = -(L2^2 - dx^2 - dy^2 - dz^2 - L1^2) / (2 * dx * L1);                 %(L2^2 - dx^2 - dy^2 - dz^2 - L1^2) / (2 * dx * L1);������
    else
        stheta = (L1^2 + dx^2 + dy^2 + dz^2 - L2^2) / (2 * dz * L1);
        theta(1) = asin(stheta) * 180 / pi;
        theta(2) = theta(1);
        return;
    end
    a = dz^2 / dx^2 + 1;
    b = -2 * temp * dz / dx;                                                    %2 * temp * dz / dx�� ����
    c = temp^2 - 1;
    if (b^2 - 4*a*c < 0) 
        theta(1) = 91;
        theta(2) = 91;
        return
    else
        stheta = (-b - (b^2 - 4*a*c)^0.5) / (2 * a);
        if stheta > -1 && stheta < 1
            theta(1) = asin(stheta) * 180 / pi;                                 %�ش���֤һ�¼���
        end
        stheta = (-b + (b^2 - 4*a*c)^0.5) / (2 * a);
        if stheta > -1 && stheta < 1
            theta(2) = asin(stheta) * 180 / pi;
        end
    end

end

