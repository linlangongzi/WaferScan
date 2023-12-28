function [ theta ] = VectorsAngle( a,b )
%计算向量，a,b的夹角,单位是：度

theta = a*b';
theta = theta/norm(a)/norm(b);
theta = acos(theta)*180/pi;

end

