% close
% clc
hold on
for i = 1 : length(triangle)
   p1 = triangle(i, 1:3);
   p2 = triangle(i, 4:6);
   p3 = triangle(i, 7:9);
   plot3([p1(1), p2(1)], [p1(2), p2(2)], [p1(3),p2(3)],'-','Color','r');
   hold on;
   plot3([p1(1), p3(1)], [p1(2), p3(2)], [p1(3),p3(3)],'-','Color','r');
   plot3([p3(1), p2(1)], [p3(2), p2(2)], [p3(3),p2(3)],'-','Color','r');
end
% axis equal
% xlabel('x axis')
% ylabel('y axis')
% zlabel('z axis')
% view(-130, 18)
axis([220,350,-40,40,-60,20])