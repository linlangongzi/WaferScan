function [ N ] = BasisFuns( span,u,p,U )
%	�������ֵu��Ӧ�����з���B������������ֵ��һֱ���㵽��i��p��B����
%   input:
%           span:Ϊu��U�еĽڵ�������±�span,U��MATLAB�е��±��1��ʼ
%           u:�����λ��
%           p:B�����������Ľ״Σ�p�α�ʾP+1��
%           U:�ڵ�ʸ��
%    output
%           N:����ֵu��Ӧ��B������������ֵ���״�Ϊp
%             ʵ����p������ֻ�е�span-p+1��span+1���Ƿ���ġ�
%             �±��1��ʼ�㡣���ֻ��Ҫ���p+1����������ֵ
    if u>1
        u = 1;
    elseif u<0
        u = 0;
    end
    N = zeros(1,p+1);
    N(1) = 1.0;
    left = zeros(1,p+1);
    right = zeros(1,p+1);
    for j = 1:p
        left(j) = u-U(span+1-j);
        right(j) = U(span+j)-u;
        saved = 0.0;
        for r = 0:j-1
            temp = N(r+1)/(right(r+1)+left(j-r));
            N(r+1) = saved +right(r+1)*temp;
            saved = left(j-r)*temp;
        end
        N(j+1) = saved;
    end        
end

