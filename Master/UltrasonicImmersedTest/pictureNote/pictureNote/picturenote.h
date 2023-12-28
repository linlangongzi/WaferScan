// pictureNote.h : main header file for the pictureNote DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef _PICTURE_NOTE_DLL
	#ifndef DLL_DECL
		#define DLL_DECL __declspec(dllexport)
	#endif
#else
	#ifndef DLL_DECL
		#define DLL_DECL __declspec(dllimport)
	#endif
#endif

#include "resource.h"		// main symbols

class CEditNoteDlg;
// CpictureNoteApp
// See pictureNote.cpp for the implementation of this class
//
#ifdef _PICTURE_NOTE_DLL
	struct LOGICAL_POINT_t 
	{
		float x;
		float y;
	};
#endif

typedef struct tagNOTE_DATA{
	int nType;
	int nObjectNum;
//	CPoint ptFrom;
//	CPoint ptTo;
	CPoint ptArray[30];
	int nArrayNum;
	int nStrLength;
	CString strNote;
	
}NOTE_DATA_t;

typedef struct tagNOTE_NOSTRING_DATA{
	int nType;
	int nObjectNum;
	//	CPoint ptFrom;
	//	CPoint ptTo;
	CPoint ptArray[30];
	int nArrayNum;
	int nStrLength;
}NOTE_NOSTRING_DATA_t;

class DLL_DECL CpictureNoteApp : public CWinApp
{
public:
	CpictureNoteApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
public:
	void PopUpMenu(CWnd* pParentWnd, CPoint point);
	CString ShowEditDlg(int nType, CPoint ptForm, CPoint ptTo);
public:
	CEditNoteDlg* m_pEditDlg;
//	CList<NOTE_DATA_t, NOTE_DATA_t&> m_lstPictureNote;
	NOTE_DATA_t m_stNoteData;
	int m_nType;
	CPoint m_ptFrom;
	CPoint m_ptTo;
	CString m_strNote;
public:
	afx_msg void OnEdit();
public:
	afx_msg void OnDelete();
};

class DLL_DECL CpictureNote
{
public:
	CpictureNote()
	{
		
	}
	~CpictureNote()
	{
	}
	inline CpictureNote& operator=(const CpictureNote& rhs)
	{
		if(this == &rhs)
			return * this;
//		m_lstPictureNote = rhs.m_lstPictureNote;
		m_stNoteData = rhs.m_stNoteData;
		m_strFileName = rhs.m_strFileName;
		m_ptCurrent = rhs.m_ptCurrent;
		m_pParent = rhs.m_pParent;

		return *this;
	}
public:
	CList<NOTE_DATA_t, NOTE_DATA_t&> m_lstPictureNote;	// 数据链表
	NOTE_DATA_t m_stNoteData;							// 链表节点数据
	CString m_strFileName;								// 文件名
	CPoint m_ptCurrent;									// 当前点
	CWnd* m_pParent;
};

//DLL_DECL extern CList<NOTE_DATA_t, NOTE_DATA_t&> g_lstPictureNote;
DLL_DECL extern CpictureNote g_pictureNote;
void Note_GetMaxAndMin(CPoint ptFrom, CPoint ptTo, int& nXMin, int& nXMax, int& nYMin, int& nYMax);
BOOL DLL_DECL DllPreTranslateMessage(MSG* pMsg);
void DLL_DECL Note_SetFilePath(CString strFilePath);
int DLL_DECL Note_ShowEditDlg(CWnd* pParent, int nType, CPoint ptArray[], 
							  int nArrayNum, CPoint ptCurrent,CString strFileName, int nObjectNum);
int DLL_DECL Note_InNoteListSerial(CPoint ptCurrent, int nObjectNum);
BOOL DLL_DECL Note_IsInRegion(NOTE_DATA_t stNoteData, CPoint ptCurrent);
BOOL DLL_DECL Note_IsInRect(CPoint ptFrom, CPoint ptTo, CPoint ptCurrent);
BOOL DLL_DECL Note_IsInCircle(CPoint ptFrom, CPoint ptTo, CPoint ptCurrent);
BOOL DLL_DECL Note_IsInEllipse(CPoint ptFrom, CPoint ptTo, CPoint ptCurrent);
BOOL DLL_DECL Note_IsInPolygon(CPoint ptArray[], int nCount, CPoint ptCurrent);
void DLL_DECL Note_DrawDCRegion(CDC *pDC, NOTE_DATA_t stNoteData, BOOL bFillFlag);
void DLL_DECL Note_DrawAllNote(CDC *pDC, BOOL bFillFlag, int nObjectNum);
void DLL_DECL Note_DeleteSelectNote(CPoint ptCurrent, int nObjectNum);
void DLL_DECL Note_GetNoteList(CList<NOTE_DATA_t, NOTE_DATA_t&>* pLstPictureNote);
void DLL_DECL Note_SaveFile(CString strFileName);
void DLL_DECL Note_ReadFile(CString strFileName);