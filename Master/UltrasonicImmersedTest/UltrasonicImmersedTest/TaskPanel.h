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
	CListCtrl m_lstTaskItems;		// ���������б�
private:	
	CString m_strTaskPath;			// �����ļ�Ŀ¼·��
	CString m_strListFilePath;		// �����б��ļ�·��
	CStringArray m_arrayMovePathItems;	// ������ɨ���ļ�·������
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButtonCreateItem();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonSaveItem();
	afx_msg void OnBnClickedButtonDelItem();
	afx_msg void OnBnClickedButtonCreateCube();
};


