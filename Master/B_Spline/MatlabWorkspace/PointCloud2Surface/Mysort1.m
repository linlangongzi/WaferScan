function [ t, unsorted ] = Mysort1( unsorted , p , r )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
x = unsorted(3,r);  %取未排序的数组的最后一个数据
i = p-1;
temp =zeros(3,1);
for j = p:r-1
    if unsorted(3,j) <= x
        i=i+1;
        temp=unsorted(:,i);
        unsorted(:,i)=unsorted(:,j);
        unsorted(:,j)=temp;
    end
end
temp = unsorted(:,i+1);
unsorted(:,i+1) = unsorted(:,r);
unsorted(:,r) = temp;
t = i+1;
end