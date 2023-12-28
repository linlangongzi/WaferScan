function [ C ] = CurvePoint( n,p,U,P,u )
%����p��B����������u���Ĳ�ֵ��
%�㷨A3.1,�������Ƶ㡢�ڵ�ʸ�����������״�
%   input:
%       n:���ƽڵ�����������Ƶ�P�е�ĸ���Ϊ��n+1
%       p:B�����������Ľ״Σ�p�α�ʾP+1��
%       U���ڵ�ʸ��
%       P�����Ƶ����
%       u��������Ĳ���
%   output:
%       C:�����ڲ���u����ֵ����ֵ���
%�������������ߣ�ֻ��Ҫ�����в���u����һ�鼴��
    span = FindSpan(n,p,u,U);
%   N = zeros(1,p+1);
    N = BasisFuns(span,u,p,U);%ʵ����N�Ǵ�0��n,����ֻ��ȡ��span-p~span��p+1��������Ϊ0
    C = 0;
    for i = 0:p
        C = C +N(i+1)*P(span-p+i);
    end

end
