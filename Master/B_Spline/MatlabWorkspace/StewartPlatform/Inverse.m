function [ pointOut ] = Inverse( x,y,z,a,b,c,pointIn )
%ָ����ƽ̨�任�������x,y,z,a,b,c�����㶯ƽ̨�ϵĵ�pointIn�ھ�������ϵ�µ�����pointOut
%��ƽ̨�˶��任˳����ƽ��-����-���-ƫ��

%ԭ��ƽ��
T = [1 0 0 x;...
     0 1 0 y;...
     0 0 1 z;...
     0 0 0 1];
 %���� YAW
 Ra = [1,   0,          0,          0;...
       0,   cosd(a),    -sind(a),   0;...
       0,   sind(a),    cosd(a),    0;...
       0    0           0           1];
 %��� ROLL
 Rb = [cosd(b),     0,  sind(b),    0;...
       0            1   0           0;...
       -sind(b),    0,  cosd(b),    0;...
       0            0   0           1];
%ƫ��
 Rc = [cosd(c),     -sind(c),  0, 0;...
       sind(c),     cosd(c),   0, 0;...
       0,           0,         1, 0;...
       0,           0,         0, 1];
 [dim, ~] = size(pointIn);
 pointOut = zeros(dim, 4);
 for index = 1 : dim
    pointOut(index,:) = ( T * Rc * Rb * Ra * pointIn(index, :)')';
 end
end
