function [ distance ] = Distance2Point( point1, point2 )

distance = 0;
for dim = 1 : length(point1)
   distance = distance + (point2(dim) - point1(dim))^2; 
end
distance = sqrt(distance);

end

