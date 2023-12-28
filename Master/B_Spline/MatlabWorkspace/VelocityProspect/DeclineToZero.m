function [ result DeclineSParam ] = DeclineToZero( DeclineSParam, StartNodeNum, T, t, a, v, s)
%Function: 
%   Calculate the s curve starting from the giving initial motion
%   information to such a motion status that the speed reaches zero when
%   acceleration is zero.
%Param:
%   t: the starting time
%   a: the starting acceleration
%   v: the starting speed
%   s: the starting distance
%   A: the maximum acceleration of CNC
%   J: the maximum jerk of CNC
%   node_index: index of the node which is the first node after the current
%   point int the new decline s curve. This param is used to combine the
%   decline s curve with the initial s curve to a new complete curve  

global g_half_jerk;
global g_double_jerk;
global g_sixth_jerk;
global g_single_acc_speed_inc;
global ZeroDouble ZERO_VEL;
global g_single_acc_time;
global g_single_acc_distance_inc;
global MaxAcc;
global MaxJa;

A = MaxAcc;
J = MaxJa;
result = true;

%Check the validity of the param
if(v < 0.0)
    result = false;
    return;
end
if(A < ZeroDouble) 
    result = false;
    return;
end
if(J < ZeroDouble)
    result = false;
    return;
end

square_a = a^2;
%Check the situation when a < 0, the zero speed can be reached by rising a
%to zero, or the target motion status cannot be reached.
if(a < 0.0) 
    limit_v = square_a / g_double_jerk;
    if(v < limit_v - ZERO_VEL)
        result = false;
        return;
    end
end

DeclineSParam.T(StartNodeNum) = T;
DeclineSParam.t(StartNodeNum) = t;
DeclineSParam.J(StartNodeNum) = -J;
DeclineSParam.a(StartNodeNum) = a;
DeclineSParam.v(StartNodeNum) = v;
DeclineSParam.s(StartNodeNum) = s;
DeclineSParam.T(StartNodeNum+1:end) = [];
DeclineSParam.t(StartNodeNum+1:end) = [];
DeclineSParam.J(StartNodeNum+1:end) = [];
DeclineSParam.a(StartNodeNum+1:end) = [];
DeclineSParam.v(StartNodeNum+1:end) = [];
DeclineSParam.s(StartNodeNum+1:end) = [];

%Calculate the first decline phase
T1 = (A + a) / J;
%V1 = a * T1 - g_half_jerk * T1^2; 
V1 = (a^2 - A^2) / g_double_jerk;               %the increment of speed during phase 1

%V3 = -g_single_acc_speed_inc;
left_speed = v + V1 - g_single_acc_speed_inc;
if(left_speed < 0.0)
    %the acceleration cannot reach -A, recalculate the max acceleration
    %Assume the new max acceleration is new_A, we have the following
    %equation: (a - new_A) * a / J - 0.5 * J * ((a - A) / J)^2 - A^2 / J + 0.5 * J * A^2 / J^2 = 0 
    new_A = -sqrt(0.5 * square_a + J * v);
    T1 = (a - new_A) / J;
    square_T1 = T1^2;
    CurNodeNum = StartNodeNum + 1;
    DeclineSParam.T(CurNodeNum) = T1;
    DeclineSParam.t(CurNodeNum) = T1 + t;
    DeclineSParam.J(CurNodeNum) = J;
    DeclineSParam.a(CurNodeNum) = a - J * T1;
    DeclineSParam.v(CurNodeNum) = v + a * T1 - g_half_jerk * square_T1;
    DeclineSParam.s(CurNodeNum) = s + v * T1 + 0.5 * a * square_T1 - g_sixth_jerk * square_T1 * T1;
    
    T2 = -new_A / J;
    StartNodeNum = CurNodeNum;
    CurNodeNum = CurNodeNum + 1;
    DeclineSParam.T(CurNodeNum) = T2;
    DeclineSParam.t(CurNodeNum) = T2 + DeclineSParam.t(StartNodeNum);
    DeclineSParam.J(CurNodeNum) = 0.0;
    DeclineSParam.a(CurNodeNum) = 0.0;
    DeclineSParam.v(CurNodeNum) = 0.0;
    DeclineSParam.s(CurNodeNum) = DeclineSParam.s(StartNodeNum) + DeclineSParam.a(StartNodeNum) * (DeclineSParam.a(StartNodeNum)^2 / (3.0 * J) - DeclineSParam.v(StartNodeNum)) / J;
    
    DeclineSParam.node_num = CurNodeNum;
else
    square_T1 = T1^2;
    CurNodeNum = StartNodeNum + 1;
    DeclineSParam.T(CurNodeNum) = T1;
    DeclineSParam.t(CurNodeNum) = T1 + t;
    DeclineSParam.J(CurNodeNum) = 0.0;
    DeclineSParam.a(CurNodeNum) = -A;
    DeclineSParam.v(CurNodeNum) = v + a * T1 - g_half_jerk * square_T1;
    DeclineSParam.s(CurNodeNum) = s + v * T1 + 0.5 * a * square_T1 - g_sixth_jerk * square_T1 * T1;
    
    T2 = left_speed / A;
    square_T2 = T2^2;
    StartNodeNum = CurNodeNum;
    CurNodeNum = CurNodeNum + 1;
    DeclineSParam.T(CurNodeNum) = T2;
    DeclineSParam.t(CurNodeNum) = T2 + DeclineSParam.t(StartNodeNum);
    DeclineSParam.J(CurNodeNum) = J;
    DeclineSParam.a(CurNodeNum) = -A;
    DeclineSParam.v(CurNodeNum) = DeclineSParam.v(StartNodeNum) + DeclineSParam.a(StartNodeNum) * T2;
    DeclineSParam.s(CurNodeNum) = DeclineSParam.s(StartNodeNum) + DeclineSParam.v(StartNodeNum) * T2 + 0.5 * DeclineSParam.a(StartNodeNum) * square_T2;
    
    StartNodeNum = CurNodeNum;
    CurNodeNum = CurNodeNum + 1;
    DeclineSParam.T(CurNodeNum) = g_single_acc_time;
    DeclineSParam.t(CurNodeNum) = g_single_acc_time + DeclineSParam.t(StartNodeNum);
    DeclineSParam.J(CurNodeNum) = 0.0;
    DeclineSParam.a(CurNodeNum) = 0.0;
    DeclineSParam.v(CurNodeNum) = 0.0;
    DeclineSParam.s(CurNodeNum) = DeclineSParam.s(StartNodeNum) + g_single_acc_distance_inc;
    
    DeclineSParam.node_num = CurNodeNum;
end
end

