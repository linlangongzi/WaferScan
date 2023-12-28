// pictureNote.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "pictureNote.h"
#include "EditNoteDlg.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CpictureNote g_pictureNote;
CList<NOTE_DATA_t, NOTE_DATA_t&> g_lstPictureNote;
//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CpictureNoteApp

BEGIN_MESSAGE_MAP(CpictureNoteApp, CWinApp)
	ON_COMMAND(ID_EDIT, &CpictureNoteApp::OnEdit)
	ON_COMMAND(ID_DELETE, &CpictureNoteApp::OnDelete)
END_MESSAGE_MAP()


// CpictureNoteApp construction

CpictureNoteApp::CpictureNoteApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_nType = 0;
}


// The one and only CpictureNoteApp object

CpictureNoteApp theApp;


// CpictureNoteApp initialization

BOOL CpictureNoteApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void CpictureNoteApp::PopUpMenu(CWnd* pParentWnd, CPoint point)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMenu menu;
	menu.LoadMenu(IDR_MENU_NOTE);
	CMenu *pMenu = menu.GetSubMenu(0);
	pParentWnd->ClientToScreen(&point);	
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,pParentWnd);
	
/*	CEditNoteDlg dlg;
	dlg.SetParent(pParentWnd);
	dlg.DoModal();*/
#if 0
	HINSTANCE exe_hInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_NOTE);
	CMenu *pMenu = menu.GetSubMenu(0);
	pParentWnd->ClientToScreen(&point);	
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,pParentWnd);	

	AfxSetResourceHandle(exe_hInstance);
#endif
}

void CpictureNoteApp::OnEdit()
{
	// TODO: Add your command handler code here
	HINSTANCE exe_hInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	
/*	m_pEditDlg = new CEditNoteDlg;
	if(m_pEditDlg->GetSafeHwnd() == NULL)
	{
		m_pEditDlg->Create(IDD_DIALOG_EDIT, this);
		m_pEditDlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}*/
	AfxMessageBox(_T("向服务器发送数据失败！"), MB_OK);
//	m_pEditDlg->DoModal();

	AfxSetResourceHandle(exe_hInstance);
}

void CpictureNoteApp::OnDelete()
{
	// TODO: Add your command handler code here
	HINSTANCE exe_hInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	CEditNoteDlg dlg;
	dlg.DoModal();
	AfxSetResourceHandle(exe_hInstance);
}
CString CpictureNoteApp::ShowEditDlg(int nType, CPoint ptForm, CPoint ptTo)
{
	// TODO: Add your command handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

/*	m_stNoteData.nType = nType;
	m_stNoteData.ptFrom = ptForm;
	m_stNoteData.ptTo = ptTo;

	m_nType = nType;
	m_ptFrom = ptForm;
	m_ptTo = ptTo;*/
	m_nType = nType;

	CEditNoteDlg dlg;
/*	dlg.m_pParent = this;
	dlg.m_stNoteData.nType = nType;
	dlg.m_stNoteData.ptFrom = ptForm;
	dlg.m_stNoteData.ptTo = ptTo;*/
	dlg.DoModal();
/*
	NOTE_DATA_t stNoteData;
	stNoteData.nType = nType;
	stNoteData.ptFrom = ptForm;
	stNoteData.ptTo = ptTo;
	stNoteData.strNote = dlg.strPictureNote;
//	m_lstPictureNote.AddTail(stNoteData);	
	g_lstPictureNote.AddTail(stNoteData);
*/

	return dlg.strPictureNote;
}
void Note_SetFilePath(CString strFilePath)
{
	g_pictureNote.m_strFileName = strFilePath;
}
int Note_ShowEditDlg(CWnd* pParent, int nType, CPoint ptArray[], int nArrayNum, CPoint ptCurrent,CString strFileName, int nObjectNum)
{
	// TODO: Add your command handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	g_pictureNote.m_pParent = pParent;
	g_pictureNote.m_ptCurrent = ptCurrent;
	g_pictureNote.m_stNoteData.nType = nType;
	memcpy(g_pictureNote.m_stNoteData.ptArray, ptArray, sizeof(CPoint) * nArrayNum);
	g_pictureNote.m_stNoteData.nArrayNum = nArrayNum;
	g_pictureNote.m_strFileName = strFileName;
	g_pictureNote.m_stNoteData.nObjectNum = nObjectNum;
	CEditNoteDlg dlg;
	dlg.m_pParent = pParent;
	if(IDOK ==dlg.DoModal())
	{
		
		if(g_pictureNote.m_lstPictureNote.IsEmpty() ==  TRUE)
		{
			if(nArrayNum > 1)
			{
				if((ptArray[0].x != 0 && ptArray[0].y != 0) && (ptArray[1].x != 0 && ptArray[1].y != 0)) // 没框范围不加点
				{
					g_pictureNote.m_lstPictureNote.AddTail(g_pictureNote.m_stNoteData);
				}
			}
		}
		else
		{
			int nSerial = Note_InNoteListSerial(ptCurrent, nObjectNum);
			if(nSerial == -1)
			{
				if((ptArray[0].x !=0 && ptArray[0].y !=0) && (ptArray[1].x !=0 && ptArray[1].y !=0))
				{
					g_pictureNote.m_lstPictureNote.AddTail(g_pictureNote.m_stNoteData);
				}
			}
			else
			{
				POSITION pos = g_pictureNote.m_lstPictureNote.FindIndex(nSerial);
				NOTE_DATA_t stNoteData = g_pictureNote.m_lstPictureNote.GetAt(pos);
				stNoteData.nStrLength = g_pictureNote.m_stNoteData.nStrLength;
				stNoteData.strNote = g_pictureNote.m_stNoteData.strNote;
				g_pictureNote.m_lstPictureNote.RemoveAt(pos);
				g_pictureNote.m_lstPictureNote.AddTail(stNoteData);
			}

		}
		
		Note_SaveFile(strFileName);

		return IDOK;
	}
	else
	{
		return IDCANCEL;
	}
}

BOOL Note_IsInRegion(NOTE_DATA_t stNoteData, CPoint ptCurrent)
{
	BOOL bRet = FALSE;

	if(stNoteData.nType == 0)
	{
		bRet = Note_IsInRect(stNoteData.ptArray[0], stNoteData.ptArray[1], ptCurrent);
	}
	else if(stNoteData.nType == 1)
	{
		bRet = Note_IsInCircle(stNoteData.ptArray[0], stNoteData.ptArray[1], ptCurrent);
	}
	else if(stNoteData.nType == 2)
	{
		bRet = Note_IsInEllipse(stNoteData.ptArray[0], stNoteData.ptArray[1], ptCurrent);
	}
	else if(stNoteData.nType == 3)
	{
		bRet = Note_IsInPolygon(stNoteData.ptArray, stNoteData.nArrayNum, ptCurrent);
	}
	else
	{
		;//
	}

	return bRet;	
}

int Note_InNoteListSerial(CPoint ptCurrent, int nObjectNum)
{
	int nSerial = 0;
	NOTE_DATA_t stNoteData;
	for(POSITION pos = g_pictureNote.m_lstPictureNote.GetHeadPosition(); pos; )
	{
		//获取数据点，并自动进行到下一个点
		stNoteData = g_pictureNote.m_lstPictureNote.GetNext(pos);
		if(stNoteData.nObjectNum == nObjectNum)
		{
			BOOL bRet = Note_IsInRegion(stNoteData, ptCurrent);
			if(bRet)
			{
				return nSerial;
			}
		}
		nSerial++;
	}
	return -1;
}

void Note_GetMaxAndMin(CPoint ptFrom, CPoint ptTo, int& nXMin, int& nXMax, int& nYMin, int& nYMax)
{
	if(ptFrom.x < ptTo.x)
	{
		nXMin = ptFrom.x;
		nXMax = ptTo.x;
	}
	else
	{
		nXMin = ptTo.x;
		nXMax = ptFrom.x;
	}

	if(ptFrom.y < ptTo.y)
	{
		nYMin = ptFrom.y;
		nYMax = ptTo.y;
	}
	else
	{
		nYMin = ptTo.y;
		nYMax = ptFrom.y;
	}
}

BOOL Note_IsInRect(CPoint ptFrom, CPoint ptTo, CPoint ptCurrent)
{
	BOOL bRet = FALSE;
	int nXMin = 0;
	int nXMax = 0;
	int nYMin = 0;
	int nYMax = 0;
	Note_GetMaxAndMin(ptFrom, ptTo, nXMin, nXMax, nYMin, nYMax);

	if((ptCurrent.x >= nXMin && ptCurrent.x <= nXMax) &&
		(ptCurrent.y >= nYMin && ptCurrent.y <= nYMax))
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}
BOOL Note_IsInCircle(CPoint ptFrom, CPoint ptTo, CPoint ptCurrent)
{
	BOOL bRet = FALSE;
	int nXMin = 0;
	int nXMax = 0;
	int nYMin = 0;
	int nYMax = 0;
	Note_GetMaxAndMin(ptFrom, ptTo, nXMin, nXMax, nYMin, nYMax);

	float fCenterX = (nXMin + nXMax) / 2.0f;
	float fCenterY = (nYMin + nYMax) / 2.0f;
	float fRadius = (nXMax - nXMin) / 2.0f;

	float fDistance = sqrt((ptCurrent.x - fCenterX)*(ptCurrent.x - fCenterX) + (ptCurrent.y - fCenterY)*(ptCurrent.y - fCenterY));

	if (fDistance < fRadius)
		return TRUE;
	else
		return FALSE;
}
BOOL Note_IsInEllipse(CPoint ptFrom, CPoint ptTo, CPoint ptCurrent)
{
	BOOL bRet = FALSE;
	int nXMin = 0;
	int nXMax = 0;
	int nYMin = 0;
	int nYMax = 0;
	Note_GetMaxAndMin(ptFrom, ptTo, nXMin, nXMax, nYMin, nYMax);

	float fXCenter = (nXMax + nXMin) / 2.0f;
	float fYCenter = (nYMax + nYMin) / 2.0f;

	float fA = (nXMax - nXMin) / 2.0f;
	float fB = (nYMax - nYMin) / 2.0f;
	float fValue = pow((ptCurrent.x - fXCenter)/fA, 2) + pow((ptCurrent.y - fYCenter)/fB, 2);

	if (fValue < 1)
		return TRUE;
	else
		return FALSE;
}

BOOL Note_IsInPolygon(CPoint ptArray[], int nCount, CPoint ptCurrent)
{
	int nCross = 0; 

	for (int i = 0; i < nCount; i++) 
	{
		CPoint p1;
		p1.x = ptArray[i].x;
		p1.y = ptArray[i].y;

		CPoint p2;
		p2.x = ptArray[(i + 1) % nCount].x;
		p2.y = ptArray[(i + 1) % nCount].y;

		// 求解 y=p.y 与 p1p2 的交点 

		if ( fabs(float(p1.y - p2.y)) < 1E-3 ) // p1p2 与 y=p0.y平行 
			continue; 

		if ( ptCurrent.y < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
			continue; 

		if ( ptCurrent.y >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
			continue; 

		// 求交点的 X 坐标 -------------------------------------------------------------- 
		double x = (double)(ptCurrent.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

		if ( x > ptCurrent.x ) 
			nCross++; // 只统计单边交点 
	} 

	// 单边交点为偶数，点在多边形之外 --- 
	return (nCross % 2 == 1); 
}

/** 
* 在DC上画图形
* @param     pDC		[in]: DC
* @param     stNoteData	[in]: 数据
* @param     bFillFlag	[in]: 是否填充
* @return    void
* @author    wangbingfu
* @since     1.0
*/
void Note_DrawDCRegion(CDC *pDC, NOTE_DATA_t stNoteData, BOOL bFillFlag)
{
	int nArrayNum = stNoteData.nArrayNum;
	CPoint ptArray[30];
	memcpy(ptArray, stNoteData.ptArray, sizeof(CPoint) * nArrayNum);
	int nType = stNoteData.nType;
	CString strNote = stNoteData.strNote;
	CPen pen;
	CPen *pOldPen;
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));


	CBrush br;
	CBrush *pOldBr;
	br.CreateStockObject(NULL_BRUSH);
	if(!bFillFlag)
	{
		pOldPen = pDC->SelectObject(&pen);
		pOldBr = pDC->SelectObject(&br);
	}

//	int nOldMode = pDC->SetROP2(R2_NOTXORPEN);  //反色

	if (nType == 0)		// 矩形
	{
		pDC->Rectangle(CRect(ptArray[0], ptArray[1]));
	}
	else if (nType == 1)	// 圆
	{
		int nLeft = ptArray[0].x < ptArray[1].x ? ptArray[0].x : ptArray[1].x;
		int nTop = (ptArray[1].y + ptArray[0].y)/2 - abs(ptArray[1].x - ptArray[0].x)/2;
		int nRight = ptArray[0].x < ptArray[1].x ? ptArray[1].x : ptArray[0].x;
		int nBottom = (ptArray[1].y + ptArray[0].y)/2 + abs(ptArray[1].x - ptArray[0].x)/2;

		pDC->Ellipse(CRect(nLeft, nTop, nRight,nBottom));
	}
	else if(nType == 2) // 椭圆
	{
		pDC->Ellipse(CRect(ptArray[0], ptArray[1]));
	}	
	else if (nType == 3)	// 多边形
	{	
		if(bFillFlag)
		{
			for (int i = 0; i < nArrayNum; i++)
			{
				if(i == 0)
				{
					pDC->BeginPath();
					pDC->MoveTo(ptArray[i]);
				}
				else
				{
					pDC->LineTo(ptArray[i]);
				}

				if(i == nArrayNum - 1)
				{
					pDC->EndPath();
					pDC->FillPath();
				}		
			}
		}
		else
		{
			for (int i = 0; i < nArrayNum; i++)
			{
				pDC->MoveTo(ptArray[i]);
				if(i == nArrayNum - 1)
				{
					pDC->LineTo(ptArray[0]);
				}
				else
				{
					pDC->LineTo(ptArray[i + 1]);
				}			
			}
		}
	}
	else
	{
		;//
	}

	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(RGB(255, 255, 255));

	int nXMin = 0;
	int nXMax = 0;
	int nYMin = 0;
	int nYMax = 0;
	Note_GetMaxAndMin(ptArray[0], ptArray[1], nXMin, nXMax, nYMin, nYMax);
	pDC->TextOut(nXMax, nYMax, strNote, strNote.GetLength());

	if(!bFillFlag)
	{
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBr);
	}

	pen.DeleteObject();
	br.DeleteObject();
}

void Note_DrawAllNote(CDC *pDC, BOOL bFillFlag, int nObjectNum)
{
	NOTE_DATA_t stNoteData;
	for(POSITION pos = g_pictureNote.m_lstPictureNote.GetHeadPosition(); pos; )
	{
		//获取数据点，并自动进行到下一个点
		stNoteData = g_pictureNote.m_lstPictureNote.GetNext(pos);
		if(nObjectNum == stNoteData.nObjectNum)
		{
			Note_DrawDCRegion(pDC, stNoteData, bFillFlag);
		}		
	}
//	m_pParent->Invalidate(TRUE);
}

void Note_DeleteSelectNote(CPoint ptCurrent, int nObjectNum)
{
	int nSerial = Note_InNoteListSerial(ptCurrent, nObjectNum);
	if(nSerial != -1)
	{
		POSITION pos = g_pictureNote.m_lstPictureNote.FindIndex(nSerial);
		g_pictureNote.m_lstPictureNote.RemoveAt(pos);
	}

	Note_SaveFile(g_pictureNote.m_strFileName);
}

void Note_GetNoteList(CList<NOTE_DATA_t, NOTE_DATA_t&>* pLstPictureNote)
{
	pLstPictureNote->RemoveAll();
	NOTE_DATA_t stNoteData;
	for(POSITION pos = g_pictureNote.m_lstPictureNote.GetHeadPosition(); pos; )
	{
		//获取数据点，并自动进行到下一个点
		stNoteData = g_pictureNote.m_lstPictureNote.GetNext(pos);
		pLstPictureNote->AddTail(stNoteData);
	}
}

void Note_SaveFile(CString strFileName)
{
#if 0	//		读出文件时stNoteData.strNote字符串后几位是乱码
	CFile file;
	CFileException e;
	if(file.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite, &e))
	{
		INT_PTR nCount = 0;
		nCount = g_pictureNote.m_lstPictureNote.GetCount();
		file.Write(&nCount, sizeof(nCount));

		
		for(POSITION pos = g_pictureNote.m_lstPictureNote.GetHeadPosition(); pos; )
		{
			//获取数据点，并自动进行到下一个点
			NOTE_DATA_t stNoteData;
			stNoteData.strNote = _T("");
			stNoteData = g_pictureNote.m_lstPictureNote.GetNext(pos);
		//	file.Write(&stNoteData, sizeof(NOTE_DATA_t));// - sizeof(CString));
			file.Write(&stNoteData, sizeof(NOTE_NOSTRING_DATA_t));
			file.Write(stNoteData.strNote.GetBuffer(), stNoteData.nStrLength);
		}
	}
	else
	{
		e.ReportError();
	}
	file.Close();
#else
	CFile file;
	CFileException e;
	if(file.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite, &e))
	{
		INT_PTR nCount = 0;
		nCount = g_pictureNote.m_lstPictureNote.GetCount();
		file.Write(&nCount, sizeof(nCount));
		file.Close();
	}
	else
	{
		e.ReportError();
		return;
	}

	for(POSITION pos = g_pictureNote.m_lstPictureNote.GetHeadPosition(); pos; )
	{
		//获取数据点，并自动进行到下一个点
		CFile file;
		file.Open(strFileName, CFile::modeReadWrite);
		NOTE_DATA_t stNoteData;
		stNoteData.strNote = _T("");
		stNoteData = g_pictureNote.m_lstPictureNote.GetNext(pos);
		file.SeekToEnd();
		file.Write(&stNoteData, sizeof(NOTE_NOSTRING_DATA_t));
		file.Close();

		CStdioFile stdioFile;
		stdioFile.Open(strFileName, CFile::modeReadWrite);
		stdioFile.SeekToEnd();
		stdioFile.WriteString(stNoteData.strNote.GetBuffer());
		stdioFile.Close();
	}
#endif
}

void Note_ReadFile(CString strFileName)
{
	g_pictureNote.m_lstPictureNote.RemoveAll();

#if 0		//		stNoteData.strNote字符串后几位是乱码
	CFile file;
	CFileException e;
	if(file.Open(strFileName, CFile::modeRead, &e))
	{
		INT_PTR nCount = 0;
		file.Read(&nCount, sizeof(nCount));

		for(int i = 0; i < nCount; i++)
		{
			NOTE_DATA_t stNoteData;
		//	file.Read(&stNoteData, sizeof(NOTE_DATA_t));// - sizeof(CString));
			file.Read(&stNoteData, sizeof(NOTE_NOSTRING_DATA_t));
			stNoteData.strNote.GetBufferSetLength(stNoteData.nStrLength);
			file.Read(stNoteData.strNote.GetBuffer(), stNoteData.nStrLength);
			g_pictureNote.m_lstPictureNote.AddTail(stNoteData);
		}
	}
	else
	{
//		e.ReportError();
	}
	file.Close();
#else
	int nOffset = 0;
	INT_PTR nCount = 0;
	CFile file;
	CFileException e;
	if(file.Open(strFileName, CFile::modeRead, &e))
	{		
		file.Read(&nCount, sizeof(nCount));
		nOffset = sizeof(nCount);
		file.Close();
	}
	else
	{
		return;
	}

	for(int i = 0; i < nCount; i++)
	{
		CFile file;
		file.Open(strFileName, CFile::modeRead);
		file.Seek(nOffset, CFile::begin);
		NOTE_DATA_t stNoteData;
		stNoteData.strNote = _T("");
		//	file.Read(&stNoteData, sizeof(NOTE_DATA_t));// - sizeof(CString));
		file.Read(&stNoteData, sizeof(NOTE_NOSTRING_DATA_t));
		nOffset = nOffset + sizeof(NOTE_NOSTRING_DATA_t);
		file.Close();

		CStdioFile stdioFile;
		stdioFile.Open(strFileName, CFile::modeRead);
		stdioFile.Seek(nOffset, CFile::begin);
	//	stdioFile.ReadString(stNoteData.strNote); //读不对
		stNoteData.strNote.GetBufferSetLength(stNoteData.nStrLength);
		LPTSTR pStr = stNoteData.strNote.GetBuffer();
		stdioFile.ReadString(pStr, stNoteData.nStrLength + 1);
		stdioFile.Close();
		//nOffset = nOffset + stNoteData.strNote.GetLength();
		nOffset = nOffset + stNoteData.nStrLength;
		g_pictureNote.m_lstPictureNote.AddTail(stNoteData);
	}
#endif
}

//响应按键消息
BOOL DllPreTranslateMessage(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.PreTranslateMessage(pMsg);

}