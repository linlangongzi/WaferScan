#pragma once


// CGeomRotaDlg �Ի���

class CGeomRotaDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeomRotaDlg)

public:
	CGeomRotaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGeomRotaDlg();

// �Ի�������
	enum { IDD = IDD_DIG_GEOMROTA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nRotateAngle;//��ת�Ƕ�
};
