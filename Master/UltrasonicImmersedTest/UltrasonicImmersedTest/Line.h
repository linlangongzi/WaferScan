#pragma once

class CLine : public CObject
{
public:
	CLine(void);
public:
	~CLine(void);
public:
	CPoint m_ptLeftPoint;   //ֱ����˵�
	CPoint m_ptRightPoint;  //ֱ���Ҷ˵�
	CRgn  m_LineRgn;    //ֱ������
	CRgn  m_LeftPointRgn;  //��˵�����
	CRgn  m_RightPointRgn;  //�Ҷ˵�����
	BOOL m_bSelectPos; // ���ѡ����ʾ�� 0Ϊδѡ�У�1Ϊѡ��
	COLORREF m_LineClr;  //ֱ����ɫ
public:
	void Move(CDC* pDC,int x,int y);  //����϶�
	void LeftExtend(CDC* pDC,int x);  //������
	void RightExtend(CDC* pDC,int x);  //������
	void GetRgn(CRgn* rgn,CPoint pt1,CPoint pt2); //��ȡ����ֵ
	void SaveLine(int nLeftPointX,int nLeftPointY,int nRightPointX,int nRightPointY,COLORREF m_clrLineClr);  //��ȡ����ֱ�ߡ�ֱ�ߵĶ˵㡢ֱ�ߵ���ɫ��ֱ�ߵ��������˵������


};
