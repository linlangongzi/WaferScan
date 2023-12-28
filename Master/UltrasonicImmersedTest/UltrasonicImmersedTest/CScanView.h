/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* C扫视图显示头文件
* Filename : CScanView.h
*
* @author       Song Chenguang
* @version      1.0         Date: 2011-08-01 
*/

#pragma once

//class CDibImage;
// CCScanView view
//#include "DibImage.h"
//enum eCSCanType{ NORMAL = 0, FEATURE };
using namespace std;
class CUltrasonicImmersedTestDoc;
class COverViewWnd;
class CMicroscopeWnd;
class CFeatureInfo;
class CMovePathView;
class CCScanParamPanel;
class CRulerWnd;
enum MEASURE_TYPE{ eRECT = 0, eELLIPSE,eTRIANGLE, eLINE, eCIRCLE, ePolygon};  //yachang.huang modify start on 2012-5-31,添加ePolygon

struct ReportItem
{
	CString strKey;
	CString strItem;
};

struct MPoint
{
	float x;
	float y;
	float z;
	float a;
	float b;
	float w1;
	float w2;
};


class CCScanView : public CScrollView
{
	DECLARE_DYNCREATE(CCScanView)
public:
	CRulerWnd *m_pHRuler;  //yachang.huang add on 2012-8-14   水平标尺
	CRulerWnd *m_pVRuler;  //yachang.huang add on 2012-8-14   竖直标尺
protected:
	CCScanView();           // protected constructor used by dynamic creation
	virtual ~CCScanView();
public:
	CUltrasonicImmersedTestDoc* GetDocument() const;
public:
	static int m_nObjectCount;
public:
//	eCSCanType m_eType;		// C扫图类型
	CSize m_layout;			// 原始扫查范围
	float m_fScaleFactor;	// 放大比例
	CSize m_displaySize;	// 显示区域大小
	CRect m_rectCScan;		// C扫描图形显示区
	CRect m_rectGrid;		// 画网格区域
	CRect m_rectAngle;		// 矩形框
	BOOL m_bMeasureOn;		// 测量功能开启标志
	CPoint m_ptFrom;					// 测量起点
	CPoint m_ptTo;						// 测量终点
	CPoint m_ptCurPos;					// 当前位置
	BOOL m_bMouseLeave;
	BOOL m_nEnterCount;
	float m_SelArea;		//面伤选择面积
	CPoint m_DlbPoint;//双击c扫区域位置坐标 // wangbingfu modify on 2012-12-27 private change to public
private:
	CRect m_rectScaleX;		// X坐标尺矩形区
	CRect m_rectScaleY;		// Y坐标尺矩形区
	
	BOOL m_bRota;			// 旋转开启标志	
	float m_fRotaAngle;		// 旋转角度
    CPoint m_ptRotateCenter;  //旋转中心  yachang.huang add on 2012-8-16
	BOOL m_bISRotateCenterExist;  //旋转中心是否存在  yachang.huang add on 2012-8-16
	CPoint m_ptObliqueStartPoint; //斜线起点  yachang.huang add on 2012-8-16
	CPoint m_ptObliqueEndPoint;  //斜线终点  yachang.huang add on 2012-8-16
	CPoint m_ptHorizontalEndPoint;	 //水平线终点  yachang.huang add on 2012-8-16
	CSize m_page;			// 页大小
	CSize m_line;			// 行大小
//	float m_DefaultMax_X;   //最大缺陷面积坐标x
//	float m_DefaultMax_Y;   //最大缺陷面积坐标y
//	float m_DefaultMax_Z;   //最大缺陷面积坐标z

	BOOL m_bOverViewDisplay;			// 全局C扫描显示标志
	COverViewWnd* m_pOverViewWnd;		// 全局C扫描窗口指针
	
	CMicroscopeWnd* m_pMicroScopeWnd;	// 显微镜放大窗口
	BOOL m_bMicroscopeDisplay;
	
	BOOL m_bTracking;					// 是否画过的标志
	MEASURE_TYPE m_eMeasureType;
	CMovePathView* m_pMovePath;
	LOGICAL_POINT_t m_probePoint;//双击鼠标坐标点对应的工件坐标
	BOOL m_bIsDrawFlag;//是否画坐标十字
	BOOL m_bIsDrawSyncFlag;	// 是否画同步十字
public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
private:
	void PrintMessageTableOne(CDC* pDC,const CRect&rect);
	void DrawDefaultResultTable(CDC* pDC,const CRect&rect);//画探伤结果表格
	void DrawDefaultResultTable1(CDC* pDC,const CRect&rect);
	void PrintDefaultWave(CDC* pDC,const CRect&rect, const LOGICAL_POINT_t& point, CPicData*pascan);//画波形图（标尺+虚线+填充色+波形）
	void DrawWaves(CDC* pDC, const CRect& rect, const LOGICAL_POINT_t& point, CPicData*pascan);//画波形图
	void DrawGrids(CDC* pDC, const CRect& rect);//画虚线
	void GetPrintMessage(void);//从printmsg.ini中读取字符串。
	void PrintDraw(CDC* pDC,const CRect& rect);
	void DistributeRegion(CDC* pDC,
						  const CRect& rectAll, 
						  CRect& rectAngle,
						  CRect& rectCScan,
						  CRect& rectScaleX,
						  CRect& rectScaleY,
						  CRect& rectGrid);
	
	void DrawFlawPoints(CDC* pDC, const CRect& rect, CList<CFeatureInfo, CFeatureInfo&>& lstFlawData);
	void DrawGrid(CDC* pDC, const CRect& rect);
	//填充两坐标轴之间的空白区间
	void FillSpace(CDC* pDC,CRect& rect);

	CPicData_L* getAScanDataByPos(LOGICAL_POINT_t point);

	
public:
	// 画标尺
	// direction 1, _|_,  2,|-, 3为1反方向, 4为2反方向
	void DrawScale(CDC* pDC, const CRect& rect, float fStart, float fEnd, float fStep, int direction, BOOL bReverse);
	void DrawCScanBackGround(CDC* pDC, const CRect& rect, CList<CFeatureInfo, CFeatureInfo&>& lstFlawData);
	LRESULT DrawCScanRealTime(WPARAM wParam, LPARAM lParam);
	void InvertRegion(CDC* pDC, CPoint ptFrom, CPoint ptTo, const MEASURE_TYPE& eType, BOOL bErase = TRUE);
	void InvertPolygonRegion(CDC* pDC, CPoint* ptArray, const int& nSize);

	CPoint MPoint2SPoint( MPoint point );
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	LOGICAL_POINT_t m_Offset_Coordinate;
protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	float m_fRateX;
	float m_fRateY;
public:
	CDC	m_MemDC;
	int m_nGrayMax;
	int m_nGrayMin;
	CPen m_cspen[22];	//画笔
	COLORREF m_clrPlatte[21];
	CSize m_CScanSize;
public:
	// 根据X进行快速排序
//	void QuickSortX(FeaturesData* SeqList, int low, int high);
	// 根据Y进行快速排序
//	void QuickSortY(FeaturesData* SeqList, int low, int high);
	// 根据X划分算法
//	int PartitionX(FeaturesData* SeqList, int low, int high);
	// 按X排完序后再对X相同的点根据Y由小到大进行排序
//	void SortYAfterSortX(FeaturesData* SeqList, int low, int high);
	// 根据Y值划分算法
//	int PartitionY(FeaturesData* SeqList, int low, int high);
	float getScaleFactor();
	CSize getLayout();
	CSize getDisplaySize();
	CSize setDisplaySize(CSize size);
public:
	void RotateImage(CDC* pDC,double angle,CPoint centerPt);  // yachang.huang modify on 2012-8-16   将第三个参数改为中心点
	void SmoothCScanImage(CDC* pDC,
		int nWidth,
		int nHeight,
		int nTempW,
		int nTempH,
		int nTempMX,
		int nTempMY,
		float* fpArray,
		float fCoef);
	void MedianFilter(CDC* pDC,int nWidth,int nHeight,int nFilterH,int nFilterW,int nFilterMX,int nFilterMY);
	unsigned char CCScanView::GetMedianNum(int bArray[], int iFilterLen);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	
	afx_msg void OnRotateImage();
	afx_msg void OnDestroy();
	afx_msg void OnCScanOverview();
	afx_msg void OnUpdateCScanOverview(CCmdUI *pCmdUI);
	afx_msg LRESULT OnMoveOverView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCscanSmooth();
	afx_msg void OnCscanMf();
	afx_msg void OnCScanMicroscope();
	afx_msg void OnUpdateCScanMicroscope(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMeasureOnOff();
	afx_msg void OnUpdateMeasureOnOff(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRulerMeasure();
	afx_msg void OnRectMeasure();
	afx_msg void OnEllipseMeasure();
	afx_msg void OnPolygonMeasure(); //yachang.huang add start on 2012-5-31
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnUpdateIDMROTATE(CCmdUI* pCmdUI);  // yachang.huang add on 2012-8-16
public:
	float CalculateDBValue(CPoint point);
public:
	afx_msg void OnUpdateRulerMeasure(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateTriangleMeasure(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateEllipseMeasure(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePolygonMeasure(CCmdUI *pCmdUI); //yachang.huang add start on 2012-5-31
public:	
	float m_fXMove;		// 向左移动距离	
	float m_fYMove;		// 向上移动距离
	float m_fYMoveForZuoBiao;
	float m_fGateThreshold[GATE_NUM];
	int m_nColorMode;	// 颜色 0：灰色,1:彩色
	int m_nValueMode;	// 取值模式
	int m_nGateIndex;	// 选择的闸门
	int m_nAmpFeatureIndex;	//幅值特征选择
	int m_nTimeFeatureIndex;	//时间特征选择
	int m_nDrawType;	// 成像模式 0:幅度成像 1:深度成像 2:声速成像 3:声阻抗成像 4:声衰减成像 5:密度成像
	float m_fBoundMin;	// 显示范围上限	
	float m_fBoundMax;	// 显示范围下限
	BOOL m_bDrawSurfaceFlag;	//画面伤标志
	float m_fSurfacePixeRate[21];	//面伤各颜色像素点比率
	float m_fHanHeRate[21];			//焊合率各颜色比率
	int  m_nMemXLENGTH;		//缓存X像素点长度
	int m_nMemYLENGTH;    //缓存Y像素点长度
	float **m_pPixel;		//缓存像素点色值矩阵
	int **m_pPixelCount;	//缓存像素点描绘次数矩阵
	float m_fXLength;		// 工件X长度
	float m_fYLength;		// 工件Y长度
	float m_fGridX;				// X网格间隔
	float m_fGridY;				// Y网格间隔
	double m_dXStart;		//X轴运动起点
	double m_dYStart;		//Y轴运动起点
	double m_dZStart;		//Z轴运动起点
	double m_dWStart;		// W运动起点 Song Chenguang add on 2013-05-04
	double m_dXEnd;			//X轴运动终点
	double m_dYEnd;			//Y轴运动终点	
	double m_dZEnd;			//Z轴运动终点

	float m_fZuobiaoXmin;	// C扫坐标X最小
	float m_fZuobiaoXmax;	// 坐标X最大
	float m_fZuobiaoYmax;	// C扫坐标Y最大
	float m_fZuobiaoYmin;	// C扫坐标Y最小
public:
	// 成像
//	void DrawPicture(CDC* pDC, const CRect& rect, CList<CFeatureInfo, CFeatureInfo&>& lstFlawData);
	void DrawPicture(CDC* pDC);
	void DrawPictureRealTime(CDC* pDC, const CRect& rect, CH_FLAWDATA_CSCAN_S* pFeatureInfo, const int& nCardIndex, 
								const int& nChannelIndex, const int& nStartPoint, const int& nNum);
	void draw2DC(CDC* pDC, float fValue, int x, int y, float fXPixelLength, float fYPixelLength);
	void DrawSurfaceFlaw(CDC* pDC);
	LOGICAL_POINT_t Client2Logical(CPoint clientPoint);
	LOGICAL_POINT_t Client2Logical1(CPoint clientPoint);
	CPoint Logical2Client(LOGICAL_POINT_t logicalPoint);
	void CalcSurfaceColorRate();
	void CalcHanHeColorRate();
	void RepositionProcess(CPoint point, LOGICAL_POINT_t stLogicalPoint);	// 回位处理
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void DrawDlbFlg(CDC* pDC,CPoint point);
	void DrawSyncDlbFlg(CDC* pDC,CPoint point);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
public:
	int m_nOnSizeCount;	// 在第一次OnSize设置m_MemDC
	int m_nObjectNum;	//类对象计数，用来区分C扫1和C扫2
private:
	ReportItem m_S1_ReportItems[30];//printmsg.ini Section 字符串成员
	ReportItem m_S2_ReportItems[30];
	ReportItem m_S3_ReportItems[30];
	//ReportItem m_S3_ReportItems[30];//A扫波形缺陷图输出
	CString m_csTotalStrArea;
	int m_nINI_S1_NUM;//printmsg.ini Section 1 成员个数
	int m_nINI_S2_NUM;
	int m_nINI_S3_NUM;
	int m_nS1_NUM;
	int m_nS2_NUM;
	int m_nS3_NUM;
	int m_nYPos;
	TCHAR m_tcBuf_S1[200];//printmsg.ini section 3
	TCHAR m_tcBuf_S2[200];//printmsg.ini section 3
	TCHAR m_tcBuf_S3[200];//printmsg.ini section 3
	int m_nFlawPrintRow_NUM; //输出的伤行数
	UINT m_uMaxPageNum;//打印最高页数
	UINT m_uCScanLength;//c扫图高度
private:
		// WangQianfeng add start on 2012-11-21
		COLORREF m_defectFontColor;
		int m_nlfUnderline;//下划线··因为返回参数太多，所以定义成成员变量
		int m_nlfDeleteline;//删除线··因为返回参数太多，所以定义成成员变量

		int m_nSymbol1Enable;
		int m_nSymbol2Enable;
		int m_nSymbol3Enable;
		// WangQianfeng add end on 2012-11-21
private:
	void InitReportItems();

	void PrintPageHeader(CDC   *pDC,   CPrintInfo   *pInfo);
	void PrintPageFooter(CDC   *pDC);
public:
	double m_dPipeDiameter;		// 管材直径
public:
	int m_nPipeXMode;	//管棒材X坐标模式
	int m_nAxisSelect; //平面选择
public:
	BOOL m_bFileFeatureFlag[7];	//读文件的特征开关标志
	BOOL m_bFileGateFlag[CHANNEL_NUM][GATE_NUM];		//读文件的闸门开关标志
	int m_nFileGateNum[CHANNEL_NUM];	//读文件的闸门打开数量
	int m_nFileFeatureNum;				//读文件的特征打开数量
	void PictureModeText(int nIndex,CString strText);
	void PictureGateText(int nIndex,CString strText);
	CString m_strPictureMode;	//成像模式报错字符串
	CString m_strPictureGate;	//成像闸门报错字符串
	
	BOOL MallocPictureMemory();// 申请画图相关内存  Song Chenguang modify on 2013-07-18
	void FreePictureMemory();// 清除画图相关内存
	BOOL bFreePictureMemoy;//是否清除画图相关内存  yachang.huang add on 2014-04-24
public:
	float m_fThick;			//试块厚度
	int m_nColorDefine;		//颜色定义 0=自定义；1=默认颜色
	//Machao modify start on 2013-07-3
//	COLORREF m_clrDefine[193];	//彩色值数组
	int GetColorNumber(float fValue, float fBoundMin, float fBoundMax);
	//Machao modify end on 2013-07-3
	COLORREF m_grayDefine[193];		//灰色值数组
public:
	void CaculateVelocity();
	void CalcuateShuaijian();
	void CaculateSzk();
	void CaculateMidu();
	void CalculatePolygonArea(); // 计算多边形面积   yachang.huang add start on 2012-6-1
public:
	int m_nProbeNum;			// 探头个数
	int m_nScanType;			// 扫查方式：并行或深度
	int m_nFillType;			// 填充方式：纵向或横向
	int m_nTwoProbeIndex;		// 两个探头组合方式
	float m_fProbeDistance;		// 探头间距
	float m_fChongDie;			// 重叠长度
	CToolTipCtrl m_ToolTip;	// 坐标提示框
	CPoint m_Poly[POLY_VERTEX_NUM];  //多边形顶点坐标  yachang.huang add start on 2012-6-1
	int m_nPolyNum;    //多边形顶点数量   yachang.huang add start on 2012-6-1
    float m_fPolygonArea;  //多边形面积   yachang.huang add start on 2012-6-1
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);   //yachang.huang add on 2012-8-14   添加Oncreate消息处理函数
public:
	LOGICAL_POINT_t m_stLogicalFlaw;	//伤点坐标
	void DrawProbePosition(double dX, double dY, double dZ, double dW);
	CPoint m_ptProbeCurPos;					// 当前位置
	BOOL m_bEnterFlag;						// 在显示区上
	CCScanParamPanel* m_pCScanParamView;
	double m_dX;		//当前X轴值
	double m_dY;		//当前Y轴值
	double m_dZ;		//当前Z轴值
	double m_dR;		//当前R轴值
	BOOL m_bDblClk;		//左键双击标志
	void DrawMemDCRegion(CDC* pDC, CPoint ptFrom, CPoint ptTo, const MEASURE_TYPE& eType, BOOL bFillFlag);
	COLORREF m_cscan_back_color;		//C扫窗口画图区背景色
	double m_dbFlawArea;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void InvertPolygon();
public:
	void GetCscanBackColor();
	void DrawPictureErrorAlarm();
	void DrawCalcRegion(CDC* pDC);
	int GetPictureMode(int nCFeatureIndex);
	void Draw2DCAsValueMode(CDC *pDC, int x, int y, float fCalcValue, float fXPixelLength, float fYPixelLength);
	void CCScanView::BuDian(CFeatureInfo feature, int nGateNum, int nFeatureNum, int nChannelNum, 
		int nFillType, CDC *pDC, float fCalcValue, int x, int y, float fXPixelLength, float fYPixelLength);
	float CCScanView::CalcPictureValue(CFeatureInfo ndata, int nMode,int nGateIndexTemp,int nFeatureIndexTemp, 
		int nMaxTimeIndexTemp, int nMaxAmpIndexTemp);
	void CalcSelectArea(LOGICAL_POINT_t ptLogicalFrom, LOGICAL_POINT_t ptLogicalTo);
	BOOL DataInSelArea(CFeatureInfo ndata, LOGICAL_POINT_t* ptArray);
public:
	BOOL m_bGuanLianFlag;	//成像时确定的是否关联,不随修改模式而变化
public:
	CPoint m_ptNoteCurrent;
	CPoint Logical2MemDC(LOGICAL_POINT_t ptLogical);
//	int m_nSavedDC;   // rem by Song Chenguang on 2013-07-17 避免资源泄漏
	BOOL m_bNotEditFlag;  //注释编辑,为TRUE时点击鼠标左键不画反色
	CString m_strNotePath;
	// WangQianfeng add start on 2012-11-21
	int m_nDefectNum;
	// WangQianfeng add end on 2012-11-21
public:
// WangQianfeng add start on 2012-11-21
	void GetDataFromSetFile(int &nSymbol1,
							int &nSymbol2,
							int &nSymbol3,
							int &nAreaA,
							int &nAreaB,
							CString &csFont,
							int &nFontSize,
							int &nDefectEnable);
	 void DrawGraphical(int nSymbol,float fXpos,float fYpos,CString csFont,int nFontSize);
	 void DrawDiamond(int nXpos,int nYpos,CString csFont,int nFontSize);
	 void DrawTriangle(int nXpos,int nYpos,CString csFont,int nFontSize);
	 void DrawSquare(int nXpos,int nYpos,CString csFont,int nFontSize);
// WangQianfeng add end on 2012-11-21
public:
	afx_msg void OnNoteEdit();
public:
	afx_msg void OnNoteDelete();
	// Song Chenguang add start on 2012-11-05
	CBitmap	m_Membitmap;
	CBitmap* m_pOldBitmap;
	// Song Chenguang add start on 2012-11-05
	// wangbingfu add start on 2013-01-09
public:
	int m_nPipeChannelSelect[CHANNEL_NUM];
	int m_nCScanSelect;
	float m_fPipeProbeAngle;	// 管材探头角度
	int GetPipeGuanLianCard();
	int bRealTimeDrawMode;
	// wangbingfu add end on 2013-01-09
	// Song Chenguang add start on 2013-06-27
	CList<LOGICAL_REGION, LOGICAL_REGION&> m_lstCustumSurfaceRegion;	// 自定义伤范围链表	
	// Song Chenguang add end on 2013-06-27
public:
	afx_msg void OnSelSurfaceCenter();
public:
	afx_msg void OnSelSurfaceEdge();
	// Song Chenguang add start on 2013-06-27
private:
	BOOL m_bSelSurfaceCenter;	// 选择中心点标志
	BOOL m_bSurfaceCenterSelected;	// 中心点已选标志
	BOOL m_bSelSurfaceEdge;	// 选择边缘点标志
	void CalcOneCustumFlaw(LOGICAL_REGION& stRegion, const float& fThreshold);	// 计算指定阈值、范围的伤
public:
	char* m_pcCustumFlawBuffer;			// 自定义伤信息Buffer
	long m_lCustumFlawBufferSize;		// Buffer大小
	// machao add start on 2013-09-17
	CList<long, long&> m_lstCustumFlawBufferSize;
	BOOL m_bEdgePoint;		// 是否添加边缘点
	// machao add end on 2013-09-17
	LOGICAL_POINT_t m_stCurPoint;
	// Song Chenguang add end on 2013-06-27
	// Song Chenguang add start on 2013-07-02
	float m_fCenterDB;		// 自选面伤中心点DB
	float m_fEdgeDB;		// 自选面伤边缘点DB
	// Song Chenguang add end on 2013-07-02
public:
	afx_msg void OnCustumSurfaceDel();
	afx_msg void OnUpdateSelSurfaceEdge(CCmdUI *pCmdUI);
#if _HAFEI
	float CalcKongXiLv(const float& fdB, const float& fThickness);	// 计算孔隙率 Song Chenguang add on 2013-07-11
	// machao add start on 2013-08-8
	void DrawTailDecreasePipeUnfoldMap(CDC *pDC);	
	// machao add start on 2013-08-8
	// machao add start on 2013-08-12
	BOOL m_bTailDecreasePipe;
	afx_msg void OnTailDecreasePipeUnfold();
	afx_msg void OnUpdateTailDecreasePipeUnfold(CCmdUI *pCmdUI);
	// machao add end on 2013-08-12
#endif
	CPoint CoordinateTransform(float fPointX, float fPointY);
	//  Song Chenguang add start on 2013-07-12
	PLATTE m_arrayPlatte[PLATTE_NUM];	// 色条颜色
	int m_nPlatteSize;	// 色条实际颜色个数
	// Song Chenguang add end on 2013-07-12
	//Machao add start on 2013-07-12	
	float **m_pfSrcData;	//降噪处理后的幅值二位指针
	BOOL m_bNoiseReduction;		//是否进行降噪操作（TRUE进行降噪处理；FLASE不进行降噪处理）
	BOOL m_bImage;			//是否选中C扫1（或C扫2）区域
	afx_msg void OnNoiseReduction();	//调用DFClass12.dll进行降噪数据处理
	//Machao add end on 2013-07-12
	//machao add start on 2013-07-22
public:
	afx_msg void OnUpdateNoiseReduction(CCmdUI *pCmdUI);
	//machao add end on 2013-07-22
	
	// machao add start on 2013-08-15
public:
	float m_fWStart;
	float m_fWStop;
	float m_fZStart;
	float m_fZStop;
	afx_msg void OnPartGainCompensateSite();
	void DrawPartGainCompensateRange(CDC *pDC, float fWStart, float fZStart, float fWStop, float fZStop);
	// machao add end on 2013-08-15
	// machao add start on 2013-08-16
	BOOL m_bCompensateRange;		// 是否画局部增益补偿范围
	CPartGainCompensateDlg *m_pPartGainCompensateDlg;
	void DrawDBCompensatesite();
	LRESULT OnDialogDestroyed(WPARAM wParam, LPARAM lParam);
	// machao add end on 2013-08-16
	CPoint Logical2MemDC(float fPointX, float fPointY);	// machao add on 2013-08-20
	// machao add start on 2013-08-23
	BOOL m_bMouseGainRangeSelect;
	BOOL m_bDrawGainRange;
	// machao add end on 2013-08-23
	int m_nAllCount; // 范围内所有的点数	// machao add on 2013-09-3
	float *m_pfCustomHanheRate;	// 自定义颜色焊合率各颜色比率	// machao add on 2013-09-6
	int m_nDblClickSelect;	// 双击选中 wangbingfu add on 2014-09-10
	typedef struct _LINE
	{
		CPoint p1;
		CPoint p2;
	}LINE_T;
	vector<LINE_T> m_vecLine;
};

#ifndef _DEBUG  // UltrasonicImmersedTestView.cpp 中的调试版本
inline CUltrasonicImmersedTestDoc* CCScanView::GetDocument() const
{ return reinterpret_cast<CUltrasonicImmersedTestDoc*>(m_pDocument); }
#endif
