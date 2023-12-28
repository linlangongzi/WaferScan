#pragma once


// CChannelInfoDockView
class CChannelPanel;
class CChannelInfoDockView : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CChannelInfoDockView)

public:
	CChannelInfoDockView();
	virtual ~CChannelInfoDockView();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CCreateContext* m_pContext;
public:
	CChannelPanel* m_pFormView;
	// Operations
public:
	void SetContext (CCreateContext* pContext)
	{
		m_pContext = pContext;
	}
	CChannelInfo GetChannelInfo(const WORD& wChannelNo);	// 获取通道数据
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


