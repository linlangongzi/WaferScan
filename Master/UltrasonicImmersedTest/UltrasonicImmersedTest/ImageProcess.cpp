/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ͼ����Դ�ļ�
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
//ƽ��
void CImageProcess::Smooth()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = pView->GetDocument();

	//ָ��DIB��ָ��
	LPSTR lpDIB;
	//ָ��DIB���ص�ָ��
	LPSTR lpDIBBits;
	//ģ��ϵ��
	float fTempC;
	//ģ��߶�
	int nTempH;
	//ģ����
	int nTempW;
	//ģ������Ԫ��x����
	int nTempMX;
	//ģ������Ԫ��Y����
	int nTempMY;
	//ģ��Ԫ��
	float* aValue;
	
	//����DIB
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//�ҵ�DIBͼ��������ʼλ��
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);
	//����ֻ����8��bppλͼ
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		AfxMessageBox(_T("Ŀǰֻ֧��256ɫλͼ��ƽ��"),MB_ICONINFORMATION | MB_OK);
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
	
	//����template()ƽ��DIB
	if (pDoc->GetDibImage()->Template(lpDIBBits,pDoc->GetDibImage()->DIBWidth(lpDIB)
				,pDoc->GetDibImage()->DIBHeight(lpDIB),nTempH,nTempW,nTempMX,nTempMY,aValue,fTempC))
	{
		pDoc->SetModifiedFlag(TRUE);//��������
		pDoc->UpdateAllViews(pView);//������ͼ

	}
	else
	{
		AfxMessageBox(_T("ϵͳ�����ڴ�ʧ��"));
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

}
//��ֵ�˲�
void CImageProcess::MidFilter()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pView->GetDocument();

	LPSTR lpDIB;//ָ��DIB��ָ��
	LPSTR lpDIBBits;//ָ��DIB���ص�ָ��
	int nFilterH;//�˲����ĸ߶�
	int nFilterW;//�˲����Ŀ��
	int nFilterMX;//�˲���������Ԫ��X����
	int nFilterMY;//�˲���������Ԫ��Y����
	//����DIB
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//�ҵ�DIB���ص���ʼλ��
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);
	//����ֻ����8��bpp��λͼ����ֵ�˲�
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB)!= 256)
	{
		AfxMessageBox(_T("Ŀǰֻ֧��256ɫ��λͼ����ֵ�˲���"));
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

	//����medianFilter()����ִ����ֵ�˲�
	if (pDoc->GetDibImage()->MedianFilter(lpDIBBits,pDoc->GetDibImage()->DIBWidth(lpDIB),pDoc->GetDibImage()->DIBHeight(lpDIB)
											,nFilterW,nFilterH,nFilterMX,nFilterMY))
	{
		pDoc->SetModifiedFlag(TRUE);//��������
		pDoc->UpdateAllViews(pView);//������ͼ

	}
	else
	{
		AfxMessageBox(_T("�ڴ����ʧ�ܣ�"));
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

}
//��ת
void CImageProcess::GeomRota()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pView->GetDocument();

	int nRotaAngle;//��ת�Ƕ�
	LPSTR lpDIB;//ָ��DIBλͼ��ָ��
	//����lpDIB
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//ֻ֧��256λͼ����ת
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		AfxMessageBox(_T("Ŀǰֻ֧��256ɫλͼ����ת��"));
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
		pDoc->ReplaceHDIB(hNewDIB);//�滻DIBͬʱ�ͷžɵ�DIB
		pDoc->InitDIBData();//����DIB��С�͵�ɫ��
		pDoc->SetModifiedFlag(TRUE);//��������
		//pView->SetScrollSizes(MM_TEXT, pDoc->GetDocSize());// �������ù�����ͼ��С(������ͼ���ܻ�������֪����Ҫ��)
	
		pDoc->UpdateAllViews(pView);
	}
	else
	{
		AfxMessageBox(_T("ϵͳ�ڴ����ʧ�ܣ�"));
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

}
//ͼ������
void CImageProcess::GeomZoom()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pView->GetDocument();

	LPSTR lpDIB;//ָ��DIB��ָ��
	//����ָ��
	lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	//�ж��Ƿ���256ɫλͼ
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		AfxMessageBox(_T("Ŀǰֻ֧��256ɫλͼ�����ţ�"));
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		return;
	}
	float fXZoomRatio;//���ű���
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
		pDoc->ReplaceHDIB(hNewDIB);//����DIB
		pDoc->InitDIBData();//����DIB��С
		pDoc->SetModifiedFlag(TRUE);//��������
		//pView->SetScrollSizes(MM_TEXT,pDoc->GetDocSize());//�������ù�����ͼ�Ĵ�С
		pDoc->UpdateAllViews(pView);
	}
	else
	{
		AfxMessageBox(_T("�����ڴ�ʧ��"));
		
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	
}