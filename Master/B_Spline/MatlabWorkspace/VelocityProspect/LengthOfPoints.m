function [ L ] = LengthOfPoints(Q)

[m,n] = size(Q);
L = 0;
for i = 1:m-1
    dl = 0;
    for j = 1:n
        dl = dl + (Q(i+1,j) - Q(i,j))^2;
    end
    L = L + sqrt(dl);
end