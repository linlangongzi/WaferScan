/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 图像缩放对话框
*Filename : GeomZoomDlg.h :头文件
*
*@author       zhaoli
*@version      1.0         Date: 2011-08-10
*/

#pragma once

class CGeomZoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeomZoomDlg)

public:
	CGeomZoomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGeomZoomDlg();

// 对话框数据
	enum { IDD = IDD_DLG_GEOZOOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_fXZoom;//水平方向缩放比例
	float m_fYZoom;//垂直方向缩放比例
};
