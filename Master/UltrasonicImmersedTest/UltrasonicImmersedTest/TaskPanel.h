#pragma once
#include "afxcmn.h"



// CTaskPanel form view
class CListCtrlEditable;

class CTaskPanel : public CFormView
{
	DECLARE_DYNCREATE(CTaskPanel)

protected:
	CTaskPanel();           // protected constructor used by dynamic creation
	virtual ~CTaskPanel();

public:
	enum { IDD = IDD_DIALOG_TASK };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL CreateView(CWnd* pParent, CCreateContext* pContext);
public:
	afx_msg void OnBnClickedButtonCreate();
private:
	void InsertItemToList(const CString& strItemName);
	BOOL SaveParaFile(const int& nIndex, const BOOL& bSaveMdsx);
	BOOL CreateTask();
	BOOL IsReady(CString& strMessage);
private:
	CString m_strTaskName;
public:
	CListCtrl m_lstTaskItems;		// 任务子项列表
private:	
	CString m_strTaskPath;			// 任务文件目录路径
	CString m_strListFilePath;		// 任务列表文件路径
	CStringArray m_arrayMovePathItems;	// 任务项扫描文件路径数组
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButtonCreateItem();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonSaveItem();
	afx_msg void OnBnClickedButtonDelItem();
	afx_msg void OnBnClickedButtonCreateCube();
};


