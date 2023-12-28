#pragma once
#include "afxcmn.h"


// CSurfaceDetailDlg dialog
class CCScanParamPanel;
class CMultiColumnSortListCtrl;
class CMessageDlg;
struct ColumnInfo
{
	LPCTSTR     name;
	SORT_TYPE   sortType;
	SORT_STATE  sortState;
	int         lengthExtension;
	int         fmt;
};

#define countof(arr)  (sizeof(arr)/sizeof(arr[0]))

class CSurfaceDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CSurfaceDetailDlg)

public:
	CSurfaceDetailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSurfaceDetailDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SURFACE_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnTransExcel();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CCScanParamPanel* m_pParentWnd;
private:
//	CMultiColumnSortListCtrl m_lstSurfaceResult;
	CListCtrlEditable m_lstSurfaceResult;
private:
	void InitResultList(CMultiColumnSortListCtrl* pListCtrl);
	void InitResultList(CListCtrlEditable* pListCtrl);
	CString		GetExcelDriver();
	BOOL GetDefaultXlsFileName(CString& sExcelFile);
	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded);
	afx_msg void OnBnClickedButtonMoveToFlaw();
	void setList();
public:
	void DrawColorRate();
	void DrawHistogram();
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnBnClickedButtonDateSave();

public:
	CButton datasave;
public:
	void ExportListToExcel(CListCtrl* pList, CString strTitle);
public:
	afx_msg void OnBnClickedButtonBinarysave();
protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
public:
	virtual BOOL DestroyWindow();
public:
	afx_msg void OnBnClickedButtonTable();
	afx_msg void OnBnClickedButtonGetascan();
public:
//	afx_msg void OnHdnItemchangedListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult);
public:
//	afx_msg void OnNMSetfocusListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMClickListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult);

	// songchenguang add start o 2012-05-12
	// 查找指定范围内伤点最大幅值
	float SearchMaxAmp(const float& fXMin, const float& fXMax, const float& fYMin, const float& fYMax,
		const int& nGateIndex, const int& nFeatureIndex);
	// songchenguang add end o 2012-05-12
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void TableCreate();
	void TableCreateZhuZhouShiDai();	//machao add on 2013-07-31
	void GetAScan();
	void InitDialog();
	CMessageDlg* m_pMessageDlg;
	LOGICAL_POINT_t m_Offset_Coordinate; // wangbingfu add at 2012-11-12
public:
	afx_msg void OnLvnKeydownListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CString VariantToCString(VARIANT var);
public:
	void TableCreateBeiFangZhongGong();	//yachang.huang add on 2014-06-24
	void TableCreateBeiFangZhongGong1(); // wangbingf add on 2014-08-27
	bool ReadExcelValue(float &fAmp,					// 试块孔波高
		float &fSurfaceComp);			// 表面补偿 wangbingf add on 2014-08-28
	void TableCreateHaFei();	//yachang.huang add on 2013-06-24
	void TableCreateHaFei1();	// wangbingfu add on 2014-11-03
private:
	PlaneFlawHeader m_combineDefect;	// wangbingfu add on 2014-11-04

};
