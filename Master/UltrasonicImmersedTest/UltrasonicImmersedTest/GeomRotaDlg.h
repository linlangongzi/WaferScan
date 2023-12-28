#pragma once


// CGeomRotaDlg 对话框

class CGeomRotaDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeomRotaDlg)

public:
	CGeomRotaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGeomRotaDlg();

// 对话框数据
	enum { IDD = IDD_DIG_GEOMROTA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nRotateAngle;//旋转角度
};
