#pragma once


// TailDecreasePipeUnfold dialog

class CTailDecreasePipeUnfold : public CPropertyPage
{
	DECLARE_DYNAMIC(CTailDecreasePipeUnfold)

public:
	CTailDecreasePipeUnfold();
	virtual ~CTailDecreasePipeUnfold();

// Dialog Data
	enum { IDD = IDD_DIALOG_WEIJIANGUAN_UNFOLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	float m_fLeft1Start;
	float m_fLeft1Stop;
	float m_fLeft2Start;
	float m_fLeft2Stop;
	float m_fRight1Start;
	float m_fRight1Stop;
	float m_fRight2Start;
	float m_fRight2Stop;
};
