function [ theta ] = VectorsAngle( a,b )
%����������a,b�ļн�,��λ�ǣ���

theta = a*b';
theta = theta/norm(a)/norm(b);
theta = acos(theta)*180/pi;

end

