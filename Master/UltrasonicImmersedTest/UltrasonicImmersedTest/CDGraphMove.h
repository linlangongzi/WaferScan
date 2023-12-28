// CDGraphMove.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDGraphMove

class CDGraphMove : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDGraphMove)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x3EA0465A, 0x993D, 0x491B, { 0xB5, 0x43, 0xAA, 0x10, 0x68, 0xB5, 0x8B, 0xE } };
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
	short GetBorderStyle()
	{
		short result;
		GetProperty(DISPID_BORDERSTYLE, VT_I2, (void*)&result);
		return result;
	}

	void SetBorderStyle(short propVal)
	{
		SetProperty(DISPID_BORDERSTYLE, VT_I2, propVal);
	}

	BOOL GetShowStandardBar()
	{
		BOOL result;
		GetProperty(0x1, VT_BOOL, (void*)&result);
		return result;
	}

	void SetShowStandardBar(BOOL propVal)
	{
		SetProperty(0x1, VT_BOOL, propVal);
	}

	BOOL GetShowGraphBar()
	{
		BOOL result;
		GetProperty(0x2, VT_BOOL, (void*)&result);
		return result;
	}

	void SetShowGraphBar(BOOL propVal)
	{
		SetProperty(0x2, VT_BOOL, propVal);
	}

	BOOL GetShowOtherBar()
	{
		BOOL result;
		GetProperty(0x3, VT_BOOL, (void*)&result);
		return result;
	}

	void SetShowOtherBar(BOOL propVal)
	{
		SetProperty(0x3, VT_BOOL, propVal);
	}

	BOOL GetCaptureStatus()
	{
		BOOL result;
		GetProperty(0x4, VT_BOOL, (void*)&result);
		return result;
	}

	void SetCaptureStatus(BOOL propVal)
	{
		SetProperty(0x4, VT_BOOL, propVal);
	}

	BOOL GetScaleStatus()
	{
		BOOL result;
		GetProperty(0x5, VT_BOOL, (void*)&result);
		return result;
	}

	void SetScaleStatus(BOOL propVal)
	{
		SetProperty(0x5, VT_BOOL, propVal);
	}

	BOOL GetPathStatus()
	{
		BOOL result;
		GetProperty(0x6, VT_BOOL, (void*)&result);
		return result;
	}

	void SetPathStatus(BOOL propVal)
	{
		SetProperty(0x6, VT_BOOL, propVal);
	}

	BOOL GetUseStatus()
	{
		BOOL result;
		GetProperty(0x7, VT_BOOL, (void*)&result);
		return result;
	}

	void SetUseStatus(BOOL propVal)
	{
		SetProperty(0x7, VT_BOOL, propVal);
	}
// Operations
public:

	BOOL ReSet()
	{
		BOOL result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL AxisJog(short type)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, type);
		return result;
	}
	BOOL AxisStop()
	{
		BOOL result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL GetPos(double * Coordinate)
	{
		BOOL result;
		static BYTE parms[] = VTS_PR8 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Coordinate);
		return result;
	}
	BOOL PathStart(long * Zindex)
	{
		BOOL result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Zindex);
		return result;
	}
	void NewFile()
	{
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OpenFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveFile()
	{
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SaveAsFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void DrawArc()
	{
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawThreeArc()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawArcLine()
	{
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawCircle()
	{
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawThreeCircle()
	{
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawEllipse()
	{
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawPoint()
	{
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawLine()
	{
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawPolygon()
	{
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawRect()
	{
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawRunway()
	{
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Nevigation()
	{
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Select()
	{
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Delete()
	{
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Copy()
	{
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Cut()
	{
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Reverse()
	{
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Find()
	{
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Mirror()
	{
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Paste()
	{
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Redo()
	{
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Rotate()
	{
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SelectAll()
	{
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Filter()
	{
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Undo()
	{
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LayerMove()
	{
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LayerShow()
	{
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewGraph()
	{
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewWindow()
	{
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewRegion()
	{
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SysPara()
	{
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LayerColor()
	{
		InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EditDlg()
	{
		InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CancelCmd()
	{
		InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Quit()
	{
		InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short GetWorkStatus()
	{
		short result;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	long GetDrawCmd()
	{
		long result;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void GetPara(double * douPara, long * logPara)
	{
		static BYTE parms[] = VTS_PR8 VTS_PI4 ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, parms, douPara, logPara);
	}
	void SetPara(double * douPara, long * logPara)
	{
		static BYTE parms[] = VTS_PR8 VTS_PI4 ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms, douPara, logPara);
	}
	short GetProprety(short * type, double * pos)
	{
		short result;
		static BYTE parms[] = VTS_PI2 VTS_PR8 ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_I2, (void*)&result, parms, type, pos);
		return result;
	}
	void Decompose()
	{
		InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL MoveCheck(long * Dindex, short * Empty)
	{
		BOOL result;
		static BYTE parms[] = VTS_PI4 VTS_PI2 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Dindex, Empty);
		return result;
	}
	short GetInputStatus()
	{
		short result;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void ConFan(short on)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, on);
	}
	void InputControl(short m_InputControl, short m_InputFlag)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, m_InputControl, m_InputFlag);
	}
	short Servo(short status)
	{
		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_I2, (void*)&result, parms, status);
		return result;
	}
	long SetIPDlg()
	{
		long result;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL PtpMove(double Coordinatex, double Coordinatey, double Coordinatez, long CaiFlag)
	{
		BOOL result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Coordinatex, Coordinatey, Coordinatez, CaiFlag);
		return result;
	}
	void SetJogPara(double * douPara, long * logPara)
	{
		static BYTE parms[] = VTS_PR8 VTS_PI4 ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_EMPTY, NULL, parms, douPara, logPara);
	}
	void SetJogDlg()
	{
		InvokeHelper(0x42, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetJogPara(double * douPara, long * logPara)
	{
		static BYTE parms[] = VTS_PR8 VTS_PI4 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_EMPTY, NULL, parms, douPara, logPara);
	}
	void GraphStartPoint(double * x, double * y, double * z)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, z);
	}
	void GraphEndPoint(double * x, double * y, double * z)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, z);
	}
	void AutoAddRect(short type, double * x, double * y, double * z)
	{
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_EMPTY, NULL, parms, type, x, y, z);
	}
	void SetAxisSelect(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	long GetAxisSelect()
	{
		long result;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}


};
