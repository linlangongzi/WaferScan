/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 图像缩放对话框
*Filename : GeomZoomDlg.cpp : 实现文件
*
*@author       zhaoli
*@version      1.0         Date: 2011-08-10
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "GeomZoomDlg.h"



IMPLEMENT_DYNAMIC(CGeomZoomDlg, CDialog)

CGeomZoomDlg::CGeomZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeomZoomDlg::IDD, pParent)
	, m_fXZoom(0)
	, m_fYZoom(0)
{

}

CGeomZoomDlg::~CGeomZoomDlg()
{
}

void CGeomZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_XZOOM, m_fXZoom);
	DDV_MinMaxFloat(pDX, m_fXZoom, 0.25, 4);
	DDX_Text(pDX, IDC_EDIT_YZOOM, m_fYZoom);
	DDV_MinMaxFloat(pDX, m_fYZoom, 0.25, 4);
}


BEGIN_MESSAGE_MAP(CGeomZoomDlg, CDialog)
END_MESSAGE_MAP()


// CGeomZoomDlg 消息处理程序
