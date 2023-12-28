function [ unsorted ] = Mysort( unsorted ,p , r )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
clc;
if p<r
    [q,unsorted] = Mysort1(unsorted,p,r); %这是什么意思
    unsorted = Mysort(unsorted,p,q-1);
    unsorted = Mysort(unsorted,q+1,r);
end


end