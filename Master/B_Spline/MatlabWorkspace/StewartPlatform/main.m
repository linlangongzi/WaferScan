clear
clc
close

topRadius = 75;
topInterval = 140;
bottomRadius = 110;
bottomInterval = 80.48;
lengthOfSteelWheel = 16.5;
lengthOfCardan = 0;
lengthOfBar = 200;

%% ����ߴ����
%��ƽ̨�ϵ�6���ο���topPlatform = zeros(6,4);
%   �òο���Ϊ����ڵ�����

topPlatform(1, :) = [-topInterval / 2, -topRadius, 0, 1];
topPlatform(2, :) = [topInterval / 2, -topRadius, 0, 1];
Rc = [cosd(120),   -sind(120),  0, 0;...
      sind(120),  cosd(120),  0, 0;...
      0,         0,        1, 0;...
      0,         0,        0, 1];
topPlatform(3,:) = (Rc * topPlatform(1, :)')';
topPlatform(4,:) = (Rc * topPlatform(2, :)')';
topPlatform(5,:) = (Rc * topPlatform(3, :)')';
topPlatform(6,:) = (Rc * topPlatform(4, :)')';
x = 0;
y = 0;
z = 195;
a = 5;
b = 0;
c = 0;
topPlatform = Inverse(x,y,z,a,b,c,topPlatform);     %�������ƽ̨�ο����ʵ��λ��
plot3(topPlatform(:,1),topPlatform(:,2),topPlatform(:,3),'o-','Color','r'); %������ƽ̨�ο����ʵ��λ��
hold on;

%��ƽ̨�ϲο����λ��bottomPlatform = zeros(6,4)
%   ���ڵ綯��ʽ���òο���Ϊ��������ģ�
%   ��������ʽ���òο���Ϊ��������ģ�
bottomPlatform = zeros(6,4);
bottomPlatform(1, :) = [-bottomInterval / 2, -bottomRadius, 0, 1];
bottomPlatform(2, :) = [bottomInterval / 2, -bottomRadius, 0, 1];
Rc = [cosd(120),   -sind(120),  0, 0;...
      sind(120),  cosd(120),  0, 0;...
      0,         0,        1, 0;...
      0,         0,        0, 1];
bottomPlatform(3,:) = (Rc * bottomPlatform(1, :)')';
bottomPlatform(4,:) = (Rc * bottomPlatform(2, :)')';
bottomPlatform(5,:) = (Rc * bottomPlatform(3, :)')';
bottomPlatform(6,:) = (Rc * bottomPlatform(4, :)')';
plot3(bottomPlatform(:,1),bottomPlatform(:,2),bottomPlatform(:,3),'o-','Color','r'); %������ƽ̨�ο����ʵ��λ��


%% �������
%   �綯��ʽ������˳�
%   ����ʽ������ת�ǣ�����ˮƽʱ����Ϊ�Ƕ�Ϊ0������������Ϊ����ϵ

% �綯��ʽ������˳�lengthOfBar
lengthOfBar1 = zeros(6,1);
for index = 1 : 6
    lengthOfBar1(index) = Distance2Point(bottomPlatform(index, :), topPlatform(index, :));
    plot3([bottomPlatform(index,1), topPlatform(index,1)],...
          [bottomPlatform(index,2), topPlatform(index,2)],...
          [bottomPlatform(index,3), topPlatform(index,3)],...
          'o-','Color','k');
end

%����ʽ
%�Ƕ�theta1
theta = zeros(6,1);
for index = 1 : 6
    theta(index, 1) = -45;
    theta(index, 1) = fzero(@(theta) Error(theta, index, topPlatform, lengthOfBar, lengthOfSteelWheel, lengthOfCardan, bottomRadius, bottomInterval), 0);
end
motorBar = sqrt(lengthOfSteelWheel^2 + lengthOfCardan^2);
theta0 = rad2deg(atan(lengthOfCardan / lengthOfSteelWheel));
theta1 = theta + theta0;
%����������ˣ���������Զ��
endOfSteelWheel(1, :) = [-bottomInterval / 2 - lengthOfSteelWheel * cosd(theta(1)), -bottomRadius, lengthOfSteelWheel * sind(theta(1)), 1];
endOfSteelWheel(2, :) = [bottomInterval / 2 + lengthOfSteelWheel * cosd(theta(2)), -bottomRadius, lengthOfSteelWheel * sind(theta(2)), 1];       
%����ڽ���
cardanCenter(1, :) = [-bottomInterval / 2 - motorBar * cosd(theta1(1)), -bottomRadius, motorBar * sind(theta1(1)), 1];
cardanCenter(2, :) = [bottomInterval / 2 + motorBar * cosd(theta1(2)), -bottomRadius, motorBar * sind(theta1(2)), 1];       

endOfSteelWheel(3, :) = [-bottomInterval / 2 - lengthOfSteelWheel * cosd(theta(3)), -bottomRadius, lengthOfSteelWheel * sind(theta(3)), 1];
endOfSteelWheel(4, :) = [bottomInterval / 2 + lengthOfSteelWheel * cosd(theta(4)), -bottomRadius, lengthOfSteelWheel * sind(theta(4)), 1];
cardanCenter(3, :) = [-bottomInterval / 2 - motorBar * cosd(theta1(3)), -bottomRadius, motorBar * sind(theta1(3)), 1];
cardanCenter(4, :) = [bottomInterval / 2 + motorBar * cosd(theta1(4)), -bottomRadius, motorBar * sind(theta1(4)), 1];  
Rc = [cosd(120),   -sind(120),  0, 0;...
      sind(120),  cosd(120),  0, 0;...
      0,         0,        1, 0;...
      0,         0,        0, 1];
endOfSteelWheel(3, :) = (Rc * endOfSteelWheel(3, :)')';
endOfSteelWheel(4, :) = (Rc * endOfSteelWheel(4, :)')';
cardanCenter(3, :) = (Rc * cardanCenter(3, :)')';
cardanCenter(4, :) = (Rc * cardanCenter(4, :)')';

endOfSteelWheel(5, :) = [-bottomInterval / 2 - lengthOfSteelWheel * cosd(theta(5)), -bottomRadius, lengthOfSteelWheel * sind(theta(5)), 1];
endOfSteelWheel(6, :) = [bottomInterval / 2 + lengthOfSteelWheel * cosd(theta(6)), -bottomRadius, lengthOfSteelWheel * sind(theta(6)), 1];
cardanCenter(5, :) = [-bottomInterval / 2 - motorBar * cosd(theta1(5)), -bottomRadius, motorBar * sind(theta1(5)), 1];
cardanCenter(6, :) = [bottomInterval / 2 + motorBar * cosd(theta1(6)), -bottomRadius, motorBar * sind(theta1(6)), 1];  
Rc = [cosd(240),   -sind(240),  0, 0;...
      sind(240),  cosd(240),  0, 0;...
      0,         0,        1, 0;...
      0,         0,        0, 1];
endOfSteelWheel(5, :) = (Rc * endOfSteelWheel(5, :)')';
endOfSteelWheel(6, :) = (Rc * endOfSteelWheel(6, :)')';
cardanCenter(5, :) = (Rc * cardanCenter(5, :)')';
cardanCenter(6, :) = (Rc * cardanCenter(6, :)')';
for index = 1 : 6
    plot3([bottomPlatform(index,1), cardanCenter(index,1)],...
      [bottomPlatform(index,2), cardanCenter(index,2)],...
      [bottomPlatform(index,3), cardanCenter(index,3)],...
      'o-','Color','b');
    plot3([cardanCenter(index,1), topPlatform(index,1)],...
      [cardanCenter(index,2), topPlatform(index,2)],...
      [cardanCenter(index,3), topPlatform(index,3)],...
      'o-','Color','g');
    plot3([bottomPlatform(index,1), endOfSteelWheel(index,1)],...
      [bottomPlatform(index,2), endOfSteelWheel(index,2)],...
      [bottomPlatform(index,3), endOfSteelWheel(index,3)],...
      'o-','Color','b');
     plot3([cardanCenter(index,1), endOfSteelWheel(index,1)],...
      [cardanCenter(index,2), endOfSteelWheel(index,2)],...
      [cardanCenter(index,3), endOfSteelWheel(index,3)],...
      'o-','Color','b');   
  lengthA(index,1) = Distance2Point(cardanCenter(index,:), topPlatform(index,:));
end
lengthA
theta
axis equal
pose = [x, y, z, a, b, c]'
topPlatform(:, 1:3)
lengthOfBar1
theta