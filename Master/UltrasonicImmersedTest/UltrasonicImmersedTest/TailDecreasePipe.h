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
	CTailDecreasePipeParam m_Param;		//尾减管参数设置
	CTailDecreasePipeSketch m_Sketch;	//尾减管示意图参数设置
	CTailDecreasePipeUnfold m_Unfold;	//尾减管展开图参数设置
};


