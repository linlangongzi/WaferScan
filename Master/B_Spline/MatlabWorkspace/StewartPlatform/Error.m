function [ error ] = Error( theta, pos, topPoint, lengthOfBar, motorBar1, motorBar2, radius, interval)

motorBar = sqrt(motorBar1^2 + motorBar2^2);
theta0 = rad2deg(atan(motorBar2 / motorBar1));
%计算电机连杆端点的符号解
theta = theta + theta0;
switch pos
    case 1
        motorPoint = [-interval / 2 - motorBar*cosd(theta), -radius, motorBar*sind(theta), 1];
    case 2
        motorPoint = [interval / 2 + motorBar * cosd(theta), -radius, motorBar*sind(theta), 1];       
    case 3
        motorPoint = [-interval / 2 - motorBar*cosd(theta), -radius, motorBar*sind(theta), 1];
        c = 120;
        Rc = [cosd(c),   -sind(c),  0, 0;...
              sind(c),  cosd(c),  0, 0;...
              0,         0,        1, 0;...
              0,         0,        0, 1];
        motorPoint = (Rc * motorPoint')';
    case 4
        motorPoint = [interval / 2 + motorBar * cosd(theta), -radius, motorBar*sind(theta), 1];
        c = 120;
        Rc = [cosd(c),   -sind(c),  0, 0;...
              sind(c),  cosd(c),  0, 0;...
              0,         0,        1, 0;...
              0,         0,        0, 1];
        motorPoint = (Rc * motorPoint')';
    case 5
        motorPoint = [-interval / 2 - motorBar*cosd(theta), -radius, motorBar*sind(theta), 1];
        c = 240;
        Rc = [cosd(c),   -sind(c),  0, 0;...
              sind(c),  cosd(c),  0, 0;...
              0,         0,        1, 0;...
              0,         0,        0, 1];
        motorPoint = (Rc * motorPoint')';
    case 6
        motorPoint = [interval / 2 + motorBar * cosd(theta), -radius, motorBar*sind(theta), 1];
        c = 240;
        Rc = [cosd(c),   -sind(c),  0, 0;...
              sind(c),  cosd(c),  0, 0;...
              0,         0,        1, 0;...
              0,         0,        0, 1];
        motorPoint = (Rc * motorPoint')';
    otherwise    
end
    distance = Distance2Point(motorPoint, topPoint(pos, :));
    error = distance - lengthOfBar;
end

