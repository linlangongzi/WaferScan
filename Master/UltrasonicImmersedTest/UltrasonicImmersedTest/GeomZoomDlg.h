/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ͼ�����ŶԻ���
*Filename : GeomZoomDlg.h :ͷ�ļ�
*
*@author       zhaoli
*@version      1.0         Date: 2011-08-10
*/

#pragma once

class CGeomZoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeomZoomDlg)

public:
	CGeomZoomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGeomZoomDlg();

// �Ի�������
	enum { IDD = IDD_DLG_GEOZOOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float m_fXZoom;//ˮƽ�������ű���
	float m_fYZoom;//��ֱ�������ű���
};
