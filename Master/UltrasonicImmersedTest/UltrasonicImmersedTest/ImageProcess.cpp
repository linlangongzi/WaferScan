/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 图像处理源文件
*Filename : ImageProcess.cpp
*
*@author       zhaoli
*@version      1.0         Date: 2011-08-08 
*/


#include "StdAfx.h"
#include "CScanDlgBar.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "CScanParamDockBar.h"
#include "ImageProcess.h"
#include "ZoomView.h"
#include "DibImage.h"
#include "CScanView.h"
#include "UltrasonicImmersedTestDoc.h"
#include "MainFrm.h"
#include "SmoothDlg.h"
#include "MidFilterDlg.h"
#include "GeomRotaDlg.h"
#include "GeomZoomDlg.h"
#include "UltrasonicImmersedTestView.h"

CImageProcess::CImageProcess(void)
{
}

CImageProcess::~CImageProcess(void)
{
}
//平滑
void CImageProcess::Smooth()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = pView->GetDocument();

	//指向DIB的指针
	LPSTR lpDIB;
	//指向DIB像素的指针
	LPSTR lpDIBBits;
	//模板系数
	float fTempC;
	//模板高度
	int nTempH;
	//模板宽度
	int nTempW;
	//模板中心元素x坐标
	int nTempMX;
	//模板中心元素Y坐标
	int nTempMY;
	//模板元素
	float* aValue;
	
	//锁定DIB
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//找到DIB图像像素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);
	//现在只处理8—bpp位图
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		AfxMessageBox(_T("目前只支持256色位图的平滑"),MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		return;
	}
	CSmoothDlg smoothDlg;

	if (IDOK != smoothDlg.DoModal())
	{
		return;
	}

	aValue = smoothDlg.m_fpArray;
	nTempH = smoothDlg.m_nTempH;
	nTempW = smoothDlg.m_nTempW;
	nTempMX = smoothDlg.m_nTempMX;
	nTempMY = smoothDlg.m_nTempMY;
	fTempC = smoothDlg.m_fTempC;

	delete smoothDlg;
	
	//调用template()平滑DIB
	if (pDoc->GetDibImage()->Template(lpDIBBits,pDoc->GetDibImage()->DIBWidth(lpDIB)
				,pDoc->GetDibImage()->DIBHeight(lpDIB),nTempH,nTempW,nTempMX,nTempMY,aValue,fTempC))
	{
		pDoc->SetModifiedFlag(TRUE);//设置脏标记
		pDoc->UpdateAllViews(pView);//更新视图

	}
	else
	{
		AfxMessageBox(_T("系统分配内存失败"));
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

}
//中值滤波
void CImageProcess::MidFilter()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pView->GetDocument();

	LPSTR lpDIB;//指向DIB的指针
	LPSTR lpDIBBits;//指向DIB像素的指针
	int nFilterH;//滤波器的高度
	int nFilterW;//滤波器的宽度
	int nFilterMX;//滤波器的中心元素X坐标
	int nFilterMY;//滤波器的中心元素Y坐标
	//锁定DIB
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//找到DIB像素的起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);
	//这里只考虑8—bpp的位图的中值滤波
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB)!= 256)
	{
		AfxMessageBox(_T("目前只支持256色的位图的中值滤波！"));
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());	
		return;
	}
	CMidFilterDlg midFilterDlg;
	midFilterDlg.m_nFilterH = 3;
	midFilterDlg.m_nFilterW = 1;
	midFilterDlg.m_nFilterMX = 0;
	midFilterDlg.m_nFilterMY = 1;

	if (IDOK !=midFilterDlg.DoModal())
	{
		return;
	}
	nFilterH = midFilterDlg.m_nFilterH;
	nFilterW = midFilterDlg.m_nFilterW;
	nFilterMX = midFilterDlg.m_nFilterMX;
	nFilterMY = midFilterDlg.m_nFilterMY;

	//调用medianFilter()函数执行中值滤波
	if (pDoc->GetDibImage()->MedianFilter(lpDIBBits,pDoc->GetDibImage()->DIBWidth(lpDIB),pDoc->GetDibImage()->DIBHeight(lpDIB)
											,nFilterW,nFilterH,nFilterMX,nFilterMY))
	{
		pDoc->SetModifiedFlag(TRUE);//设置脏标记
		pDoc->UpdateAllViews(pView);//更新视图

	}
	else
	{
		AfxMessageBox(_T("内存分配失败！"));
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

}
//旋转
void CImageProcess::GeomRota()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pView->GetDocument();

	int nRotaAngle;//旋转角度
	LPSTR lpDIB;//指向DIB位图的指针
	//锁定lpDIB
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//只支持256位图的旋转
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		AfxMessageBox(_T("目前只支持256色位图的旋转！"));
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		return;
	}
	CGeomRotaDlg gemoRotaDlg;
	gemoRotaDlg.m_nRotateAngle = 90;
	if (IDOK != gemoRotaDlg.DoModal())
	{
		return;
	}
	nRotaAngle = gemoRotaDlg.m_nRotateAngle;
	delete gemoRotaDlg;

	HDIB hNewDIB = NULL;

	hNewDIB = (HDIB)pDoc->GetDibImage()->RotateDIB(lpDIB,nRotaAngle);
	if (hNewDIB != NULL)
	{
		pDoc->ReplaceHDIB(hNewDIB);//替换DIB同时释放旧的DIB
		pDoc->InitDIBData();//更新DIB大小和调色板
		pDoc->SetModifiedFlag(TRUE);//设置脏标记
		//pView->SetScrollSizes(MM_TEXT, pDoc->GetDocSize());// 重新设置滚动视图大小(现在视图不能滑动，不知道需要不)
	
		pDoc->UpdateAllViews(pView);
	}
	else
	{
		AfxMessageBox(_T("系统内存分配失败！"));
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

}
//图形缩放
void CImageProcess::GeomZoom()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pView->GetDocument();

	LPSTR lpDIB;//指向DIB的指针
	//锁定指针
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//判断是否是256色位图
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		AfxMessageBox(_T("目前只支持256色位图的缩放！"));
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		return;
	}
	float fXZoomRatio;//缩放比率
	float fYZoomRatio;

	CGeomZoomDlg geomZoomDlg;
	geomZoomDlg.m_fXZoom = 0.5;
	geomZoomDlg.m_fYZoom = 0.5;

	if (IDOK != geomZoomDlg.DoModal())
	{
		return;
	}
	fXZoomRatio = geomZoomDlg.m_fXZoom;
	fYZoomRatio = geomZoomDlg.m_fYZoom;

	delete geomZoomDlg;

	HDIB hNewDIB = NULL;

	hNewDIB = (HDIB)pDoc->GetDibImage()->ZoomDIB(lpDIB,fXZoomRatio,fYZoomRatio);
	if (hNewDIB != NULL)
	{
		pDoc->ReplaceHDIB(hNewDIB);//更新DIB
		pDoc->InitDIBData();//更新DIB大小
		pDoc->SetModifiedFlag(TRUE);//设置脏标记
		//pView->SetScrollSizes(MM_TEXT,pDoc->GetDocSize());//从新设置滚动视图的大小
		pDoc->UpdateAllViews(pView);
	}
	else
	{
		AfxMessageBox(_T("分配内存失败"));
		
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	
}