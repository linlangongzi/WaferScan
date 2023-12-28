#pragma once



// CTailDecreasePipe

class CTailDecreasePipe : public CPropertySheet
{
	DECLARE_DYNAMIC(CTailDecreasePipe)

public:
	CTailDecreasePipe(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTailDecreasePipe(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTailDecreasePipe();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CTailDecreasePipeParam m_Param;		//β���ܲ�������
	CTailDecreasePipeSketch m_Sketch;	//β����ʾ��ͼ��������
	CTailDecreasePipeUnfold m_Unfold;	//β����չ��ͼ��������
};


