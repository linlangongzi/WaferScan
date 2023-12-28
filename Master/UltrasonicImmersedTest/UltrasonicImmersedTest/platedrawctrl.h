// platedrawctrl.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPlatedrawctrl

class CPlatedrawctrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CPlatedrawctrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x60705132, 0x106E, 0x462D, { 0x9B, 0x63, 0x81, 0x65, 0x32, 0x34, 0xB9, 0xC4 } };
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

	void AddPoint(double x, double y, double z, short type)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_I2 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, z, type);
	}
	void go()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void clear()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetDistanceX(double DistanceX, double MaxDistanceX)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DistanceX, MaxDistanceX);
	}
	void SetDistanceY(double DistanceY, double MaxDistanceY)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DistanceY, MaxDistanceY);
	}
	void SetPlateInfo(double Width, double Height, double Thick)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Width, Height, Thick);
	}
	void ShowNet(BOOL bShow)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bShow);
	}
	void Zoom(double fFactor)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fFactor);
	}
	void BeginWaitor()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EndWaitor()
	{
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawBar(short line)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, line);
	}
	void AddCeHouWuCha(double PointX, double PointY, double PlateThick)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, PointX, PointY, PlateThick);
	}
	void DrawCeHouWuCha()
	{
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString GetXYFlawCount(double MinX, double MaxX, double MinY, double MaxY)
	{
		CString result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, MinX, MaxX, MinY, MaxY);
		return result;
	}
	void SetThickOffsetColor(unsigned long maxValue, unsigned long midValue, unsigned long minValue)
	{
		static BYTE parms[] = VTS_UI4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, maxValue, midValue, minValue);
	}
	CString GetPlaneFlawPoints()
	{
		CString result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetMaxCha()
	{
		CString result;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetShangsLength_gb2970()
	{
		CString result;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	double GetMaxZhiShiArea_gb2970()
	{
		double result;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double Get50MMMaxShangZhiShiLength()
	{
		double result;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	long WriteResultToDB(LPCTSTR strConnectString)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strConnectString);
		return result;
	}
	double GetPointZhiShiArea(double x, double y, double z, short type)
	{
		double result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_I2 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_R8, (void*)&result, parms, x, y, z, type);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetZhiShiLengthType(short ZhiShiLengthType)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ZhiShiLengthType);
	}
	void SetShangWidthType(short ShangWidthType)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ShangWidthType);
	}
	void SetAreaType(short AreaType)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, AreaType);
	}
	void SetMergeShangType(short MergeType, double Distance)
	{
		static BYTE parms[] = VTS_I2 VTS_R8 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MergeType, Distance);
	}
	void SetEdgeWidth(double EdgeWidth)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeWidth);
	}
	void SetMaxZDistance(double MaxZDistance)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MaxZDistance);
	}
	void SetNotUseBodyMinArea(double BodyMinArea)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyMinArea);
	}
	void SetNotUseBodyMaxArea(double BodyMaxArea)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyMaxArea);
	}
	void SetNotUseBodyMinShangZhiShiLength(double BodyMinShangZhiShiLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyMinShangZhiShiLength);
	}
	void SetNotUseBodyMaxShangZhiShiLength(double BodyMaxShangZhiShiLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyMaxShangZhiShiLength);
	}
	void SetNotUseBodyMinShangLength(double BodyMinShangLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyMinShangLength);
	}
	void SetNotUseBodyMaxShangLength(double BodyShangMaxLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyShangMaxLength);
	}
	void SetNotUseBodyMinShangWidth(double BodyMinShangWidth)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyMinShangWidth);
	}
	void SetNotUseBodyMaxShangWidth(double BodyMaxShangWidth)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BodyMaxShangWidth);
	}
	void SetNotUseEdgeMinArea(double EdgeMinArea)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMinArea);
	}
	void SetNotUseEdgeMaxArea(double EdgeMaxArea)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMaxArea);
	}
	void SetNotUseEdgeMinShangZhiShiLength(double EdgeMinShangZhiShiLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMinShangZhiShiLength);
	}
	void SetNotUseEdgeMaxShangZhiShiLength(double EdgeMaxShangZhiShiLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMaxShangZhiShiLength);
	}
	void SetNotUseEdgeMinShangLength(double EdgeMinShangLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMinShangLength);
	}
	void SetNotUseEdgeMaxShangLength(double EdgeMaxShangLength)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMaxShangLength);
	}
	void SetNotUseEdgeMinShangWidth(double EdgeMinShangWidth)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMinShangWidth);
	}
	void SetNotUseEdgeMaxShangWidth(double EdgeMaxShangWidth)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EdgeMaxShangWidth);
	}
	double GetBodyAvgQueXianLv()
	{
		double result;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeAvgQueXianLv()
	{
		double result;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetAllAvgQueXianLv()
	{
		double result;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetBodyMaxQueXianLv()
	{
		double result;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeMaxQueXianLv()
	{
		double result;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetAllMaxQueXianLv()
	{
		double result;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetBodyMaxShangCountPerSquareMeter()
	{
		double result;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeMaxShangCountPerMeter()
	{
		double result;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetAllMaxShangCountPerSquareMeter()
	{
		double result;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetBodyMaxQueXianArea()
	{
		double result;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeMaxQueXianArea()
	{
		double result;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetBodyMaxShangZhiShiLength()
	{
		double result;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeMaxShangZhiShiLength()
	{
		double result;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeMaxWidth()
	{
		double result;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeMaxLength()
	{
		double result;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	CString GetSettingInfo()
	{
		CString result;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetPointsInfo()
	{
		CString result;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long PlateType_OutPut(LPCTSTR strConnectString)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strConnectString);
		return result;
	}
	long PlateType_InPut(LPCTSTR strConnectString)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strConnectString);
		return result;
	}
	void DrawDAC(LPCTSTR strParam)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strParam);
	}
	long ZipFile(LPCTSTR strInFileName, LPCTSTR strOutFileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strInFileName, strOutFileName);
		return result;
	}
	long UnZipFile(LPCTSTR strInFileName, LPCTSTR strOutFileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strInFileName, strOutFileName);
		return result;
	}
	long DianShangDB2MEM(LPCTSTR strConnectString, long DianID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strConnectString, DianID);
		return result;
	}
	CString DianShangMEM2String(long Page, long PageSize)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Page, PageSize);
		return result;
	}
	long DianShangMEM2Draw()
	{
		long result;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetDianShang(unsigned long * DianShangDian, unsigned long * DianShangSize)
	{
		long result;
		static BYTE parms[] = VTS_PUI4 VTS_PUI4 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DianShangDian, DianShangSize);
		return result;
	}
	void DrawDianShangBarX(unsigned long DianShangData, unsigned long DianShangSize)
	{
		static BYTE parms[] = VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DianShangData, DianShangSize);
	}
	void DrawDianShangBarY(unsigned long DianShangData, unsigned long DianShangSize)
	{
		static BYTE parms[] = VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DianShangData, DianShangSize);
	}
	unsigned long GetDianShangData()
	{
		unsigned long result;
		InvokeHelper(0x49, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	unsigned long GetDianShangSize()
	{
		unsigned long result;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	long ReEvaluate(LPCTSTR strConnectionString, LPCTSTR strNewBiaoZhunName, LPCTSTR strNewBodyLevelName, LPCTSTR strNewEdgeLevelName, long IDDian)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strConnectionString, strNewBiaoZhunName, strNewBodyLevelName, strNewEdgeLevelName, IDDian);
		return result;
	}
	VARIANT GetPointsInfo2()
	{
		VARIANT result;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT GetPlaneFlawPoints2()
	{
		VARIANT result;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long DayReport_Out(LPCTSTR strConnectionString, LPCTSTR strBeginTime, LPCTSTR strEndTime)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strConnectionString, strBeginTime, strEndTime);
		return result;
	}
	long ClassReport_Out(LPCTSTR strConnectionString, LPCTSTR strBeginTime, LPCTSTR strEndTime, LPCTSTR strClassName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strConnectionString, strBeginTime, strEndTime, strClassName);
		return result;
	}
	long DianShangMEM2Draw2()
	{
		long result;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddPoint2(double x, double y, double z, short type, double p1, double p2, double p3, double p4, double p5)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_I2 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, y, z, type, p1, p2, p3, p4, p5);
		return result;
	}
	long SetDayReportData(LPCTSTR strData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strData);
		return result;
	}
	long SetClassReportData(LPCTSTR strData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x53, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strData);
		return result;
	}
	VARIANT ThreeCalculation()
	{
		VARIANT result;
		InvokeHelper(0x54, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void SetDBConnection(LPCTSTR strConnection)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strConnection);
	}
	void SetCheckID(unsigned long nCheckID)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x56, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nCheckID);
	}
	void ShowMianShang()
	{
		InvokeHelper(0x57, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	double GetBodyMaxLength()
	{
		double result;
		InvokeHelper(0x58, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetBodyMaxWidth()
	{
		double result;
		InvokeHelper(0x59, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetMaxDuijiaoLength()
	{
		double result;
		InvokeHelper(0x5a, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetBodyMaxDuijiaoLength()
	{
		double result;
		InvokeHelper(0x5b, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetEdgeMaxDuijiaoLength()
	{
		double result;
		InvokeHelper(0x5c, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetJISBodyMaxQueXianNumSquareMeter()
	{
		double result;
		InvokeHelper(0x5d, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetJISEdgeMaxQueXianNumThreeMeters()
	{
		double result;
		InvokeHelper(0x5e, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetJISBodyAvgQueXianLv()
	{
		double result;
		InvokeHelper(0x5f, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetJISEdgeAvgQueXianLv()
	{
		double result;
		InvokeHelper(0x60, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetJISBodyMaxQueXianNumSquareMeter2()
	{
		double result;
		InvokeHelper(0x61, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetJISEdgeMaxQueXianNumThreeMeters2()
	{
		double result;
		InvokeHelper(0x62, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetJISBodyAvgQueXianNumSquarMeter()
	{
		double result;
		InvokeHelper(0x63, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	void SetShangBoFuZhiBiaoZhun(BOOL bBaoliu, float fShangBoFuZhiBiaoZhun)
	{
		static BYTE parms[] = VTS_BOOL VTS_R4 ;
		InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bBaoliu, fShangBoFuZhiBiaoZhun);
	}
	double GetEdgeMaxShangZhiShiLength2(double dMinX, double dMaxX, double dMinY, double dMaxY)
	{
		double result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x65, DISPATCH_METHOD, VT_R8, (void*)&result, parms, dMinX, dMaxX, dMinY, dMaxY);
		return result;
	}
	double GetEdgeMaxShangZhiShiLengthX(double * dMinX, double * dMaxX, double * dMinY, double * dMaxY, double * dArea)
	{
		double result;
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x66, DISPATCH_METHOD, VT_R8, (void*)&result, parms, dMinX, dMaxX, dMinY, dMaxY, dArea);
		return result;
	}
	void SetAreaBiaoZhun(double fAreaBiaoZhunMin, double fAreaBiaoZhunMax)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 ;
		InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fAreaBiaoZhunMin, fAreaBiaoZhunMax);
	}


};
