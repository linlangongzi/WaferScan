// mdsocx.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CMdsOcx

class CMdsOcx : public CWnd
{
protected:
	DECLARE_DYNCREATE(CMdsOcx)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x6C76CED1, 0xDB0D, 0x45E0, { 0xB1, 0xDC, 0xC2, 0x22, 0xE3, 0x9A, 0x90, 0xA8 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:

// Operations
public:

	void OpenFileByPath(LPCTSTR p_path)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_path);
	}
	BOOL SaveFileByPath(LPCTSTR p_path)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, p_path);
		return result;
	}
	void TrioStart()
	{
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short GetBorderStyle()
	{
		short result;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetBorderStyle(short p_val)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL GetShowStandardBar()
	{
		BOOL result;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetShowStandardBar(BOOL p_val)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL GetShowGraphBar()
	{
		BOOL result;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetShowGraphBar(BOOL p_val)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL GetShowOtherBar()
	{
		BOOL result;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetShowOtherBar(BOOL p_val)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL GetCaptureStatus()
	{
		BOOL result;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetCaptureStatus(BOOL p_val)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL GetScaleStatus()
	{
		BOOL result;
		InvokeHelper(0x60030010, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetScaleStatus(BOOL p_val)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030011, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL GetPathStatus()
	{
		BOOL result;
		InvokeHelper(0x60030012, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetPathStatus(BOOL p_val)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL GetUseStatus()
	{
		BOOL result;
		InvokeHelper(0x60030014, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetUseStatus(BOOL p_val)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL ReSet()
	{
		BOOL result;
		InvokeHelper(0x60030016, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL PtpMove(double p_x, double p_y, double p_z, double p_ab, double p_w, double p_r, long p_flag)
	{
		BOOL result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_I4 ;
		InvokeHelper(0x60030017, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, p_x, p_y, p_z, p_ab, p_w, p_r, p_flag);
		return result;
	}
	BOOL AxisJog(short p_type)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030018, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, p_type);
		return result;
	}
	BOOL AxisStop()
	{
		BOOL result;
		InvokeHelper(0x60030019, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	double GetPipR()
	{
		double result;
		InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	BOOL GetPos(double * p_val)
	{
		BOOL result;
		static BYTE parms[] = VTS_PR8 ;
		InvokeHelper(0x6003001b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, p_val);
		return result;
	}
	BOOL PathStart(long * p_val)
	{
		BOOL result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, p_val);
		return result;
	}
	void PathTracing(double * p_val)
	{
		static BYTE parms[] = VTS_PR8 ;
		InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	BOOL MoveCheck(long * p_val, short * p_val2)
	{
		BOOL result;
		static BYTE parms[] = VTS_PI4 VTS_PI2 ;
		InvokeHelper(0x6003001e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, p_val, p_val2);
		return result;
	}
	void NewFile()
	{
		InvokeHelper(0x6003001f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OpenFile(LPCTSTR p_val)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030020, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	void SaveFile()
	{
		InvokeHelper(0x60030021, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SaveAsFile(LPCTSTR p_val)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030022, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	void DrawArc()
	{
		InvokeHelper(0x60030023, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawThreeArc()
	{
		InvokeHelper(0x60030024, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawArcLine()
	{
		InvokeHelper(0x60030025, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawCircle()
	{
		InvokeHelper(0x60030026, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawThreeCircle()
	{
		InvokeHelper(0x60030027, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawEllipse()
	{
		InvokeHelper(0x60030028, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawPoint()
	{
		InvokeHelper(0x60030029, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawLine()
	{
		InvokeHelper(0x6003002a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawPolygon()
	{
		InvokeHelper(0x6003002b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawRect()
	{
		InvokeHelper(0x6003002c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawRunway()
	{
		InvokeHelper(0x6003002d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Nevigation()
	{
		InvokeHelper(0x6003002e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void nSelect()
	{
		InvokeHelper(0x6003002f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Delete()
	{
		InvokeHelper(0x60030030, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Copy()
	{
		InvokeHelper(0x60030031, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Cut()
	{
		InvokeHelper(0x60030032, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Reverse()
	{
		InvokeHelper(0x60030033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Find()
	{
		InvokeHelper(0x60030034, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Mirror()
	{
		InvokeHelper(0x60030035, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Paste()
	{
		InvokeHelper(0x60030036, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ReDraw()
	{
		InvokeHelper(0x60030037, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Redo()
	{
		InvokeHelper(0x60030038, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Rotate()
	{
		InvokeHelper(0x60030039, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SelectAll()
	{
		InvokeHelper(0x6003003a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Filter()
	{
		InvokeHelper(0x6003003b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Undo()
	{
		InvokeHelper(0x6003003c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LayerMove()
	{
		InvokeHelper(0x6003003d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LayerShow()
	{
		InvokeHelper(0x6003003e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	double Ver()
	{
		double result;
		InvokeHelper(0x6003003f, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	void ViewGraph()
	{
		InvokeHelper(0x60030040, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewWindow()
	{
		InvokeHelper(0x60030041, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewRegion()
	{
		InvokeHelper(0x60030042, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LayerColor()
	{
		InvokeHelper(0x60030043, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EditDlg()
	{
		InvokeHelper(0x60030044, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CancelCmd()
	{
		InvokeHelper(0x60030045, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void quit()
	{
		InvokeHelper(0x60030046, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SysPara()
	{
		InvokeHelper(0x60030047, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long AboutBox()
	{
		long result;
		InvokeHelper(0x60030048, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	short GetWorkStatus()
	{
		short result;
		InvokeHelper(0x60030049, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void GetPara(double * p_val1, long * p_val2)
	{
		static BYTE parms[] = VTS_PR8 VTS_PI4 ;
		InvokeHelper(0x6003004a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val1, p_val2);
	}
	void SetPara(double * p_val1, long * p_val2)
	{
		static BYTE parms[] = VTS_PR8 VTS_PI4 ;
		InvokeHelper(0x6003004b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val1, p_val2);
	}
	short GetProprety(short * p_val1, double * p_val2)
	{
		short result;
		static BYTE parms[] = VTS_PI2 VTS_PR8 ;
		InvokeHelper(0x6003004c, DISPATCH_METHOD, VT_I2, (void*)&result, parms, p_val1, p_val2);
		return result;
	}
	void Decompose()
	{
		InvokeHelper(0x6003004d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short GetInputStatus()
	{
		short result;
		InvokeHelper(0x6003004e, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void ConFan(short p_val)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6003004f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	VARIANT InputControl(short p_val, short p_val2)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x60030050, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, p_val, p_val2);
		return result;
	}
	void SetIPDlg()
	{
		InvokeHelper(0x60030051, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AutoAddRect2(short p_val, double * p_val1, double * p_val2, double * p_val3)
	{
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x60030052, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val, p_val1, p_val2, p_val3);
	}
	void AutoAddRect(short p_val, double * p_val1, double * p_val2, double * p_val3)
	{
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x60030053, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val, p_val1, p_val2, p_val3);
	}
	void SetJogPara(double p_val, long p_val2)
	{
		static BYTE parms[] = VTS_R8 VTS_I4 ;
		InvokeHelper(0x60030054, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val, p_val2);
	}
	void SetJogDlg()
	{
		InvokeHelper(0x60030055, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GraphStartPoint(double * p_x, double * p_y, double * p_z, double * p_ab, double * p_w, double * p_r)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x60030056, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_x, p_y, p_z, p_ab, p_w, p_r);
	}
	void GraphEndPoint(double * p_x, double * p_y, double * p_z, double * p_ab, double * p_w, double * p_r)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x60030057, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_x, p_y, p_z, p_ab, p_w, p_r);
	}
	void SetAxisSelect(short p_val)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030058, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	short GetAxisSelect()
	{
		short result;
		InvokeHelper(0x60030059, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void fnsetPip()
	{
		InvokeHelper(0x6003005b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void fnsetRing()
	{
		InvokeHelper(0x6003005c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void fnsetWall()
	{
		InvokeHelper(0x6003005d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void cmdC12Show()
	{
		InvokeHelper(0x6003006d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void cmdC34Show()
	{
		InvokeHelper(0x6003006e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void sEnd()
	{
		InvokeHelper(0x60030075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void InputRef()
	{
		InvokeHelper(0x60030082, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Button2()
	{
		InvokeHelper(0x60030084, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void mydxf(BOOL p_bool)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6003008b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_bool);
	}
	CString fnshort(LPCTSTR p_str)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003008c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, p_str);
		return result;
	}
	VARIANT fngetlen(VARIANT p_x, VARIANT p_y, VARIANT p_sx, VARIANT p_sy)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6003008d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &p_x, &p_y, &p_sx, &p_sy);
		return result;
	}
	void GRun(BSTR * p_text)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003008e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_text);
	}
	void setkp(BOOL * p_bool)
	{
		static BYTE parms[] = VTS_PBOOL ;
		InvokeHelper(0x60030099, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_bool);
	}
	void echo(BSTR * p_str)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003009a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_str);
	}
	void pop(VARIANT * p_obj)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x6003009b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_obj);
	}
	void readfileok()
	{
		InvokeHelper(0x6003009e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AutoFill(long * n, float * X, float * Y)
	{
		static BYTE parms[] = VTS_PI4 VTS_PR4 VTS_PR4 ;
		InvokeHelper(0x600300a0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, n, X, Y);
	}
	void SetMdsxPath(LPCTSTR p_path)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x600300a1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_path);
	}
	void SetMdsxName(LPCTSTR p_sn1, LPCTSTR p_sn2, LPCTSTR p_sn3, LPCTSTR p_sn4, LPCTSTR p_sn5)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x600300a2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_sn1, p_sn2, p_sn3, p_sn4, p_sn5);
	}
	void SetMdsxN(double n1, double n2, double n3)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x600300a3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, n1, n2, n3);
	}
	BOOL CubeShow()
	{
		BOOL result;
		InvokeHelper(0x600300a4, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	double Getab()
	{
		double result;
		InvokeHelper(0x600300a5, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetW()
	{
		double result;
		InvokeHelper(0x600300a6, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	void SetBreakPoint(long p_val)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x600300e0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_val);
	}
	double Get_WallDiameter()
	{
		double result;
		InvokeHelper(0x6003011a, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}


};
