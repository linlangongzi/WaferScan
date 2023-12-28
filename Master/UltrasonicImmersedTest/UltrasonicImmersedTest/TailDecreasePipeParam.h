#pragma once


// CTailDecreasePipeParam dialog

//machao modify start on 2013-08-8
//class CTailDecreasePipeParam : public CDialog
class CTailDecreasePipeParam : public CPropertyPage
//machao modify end on 2013-08-8
{
	DECLARE_DYNAMIC(CTailDecreasePipeParam)

public:
	CTailDecreasePipeParam(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~CTailDecreasePipeParam();

// Dialog Data
	enum { IDD = IDD_DIALOG_WEIJIANGUAN_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_f11LayerMin;
	float m_f11LayerMax;
	float m_f14LayerMin;
	float m_f14LayerMax;
	float m_f17LayerMin;
	float m_f17LayerMax;
	float m_f18LayerMin;
	float m_f18LayerMax;
	float m_fWeiJianW1;
};
