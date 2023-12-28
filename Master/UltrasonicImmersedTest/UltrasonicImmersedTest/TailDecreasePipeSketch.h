#pragma once


// TailDecreasePipeSketch dialog

class CTailDecreasePipeSketch : public CPropertyPage
{
	DECLARE_DYNAMIC(CTailDecreasePipeSketch)

public:
	CTailDecreasePipeSketch();
	virtual ~CTailDecreasePipeSketch();

// Dialog Data
	enum { IDD = IDD_DIALOG_WEIJIANGUAN_SKETCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnPaint();		// machao add on 2013-09-23

	DECLARE_MESSAGE_MAP()

public:
	float m_fASketch;
	float m_fABSketch;
	float m_fBSketch;
	float m_fBCSketch;
	float m_fCSketch;
	float m_fCDSketch;
	float m_fDSketch;
};
