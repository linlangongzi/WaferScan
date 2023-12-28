/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* Cɨ��ͼ��ʾͷ�ļ�
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
enum MEASURE_TYPE{ eRECT = 0, eELLIPSE,eTRIANGLE, eLINE, eCIRCLE, ePolygon};  //yachang.huang modify start on 2012-5-31,���ePolygon

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
	CRulerWnd *m_pHRuler;  //yachang.huang add on 2012-8-14   ˮƽ���
	CRulerWnd *m_pVRuler;  //yachang.huang add on 2012-8-14   ��ֱ���
protected:
	CCScanView();           // protected constructor used by dynamic creation
	virtual ~CCScanView();
public:
	CUltrasonicImmersedTestDoc* GetDocument() const;
public:
	static int m_nObjectCount;
public:
//	eCSCanType m_eType;		// Cɨͼ����
	CSize m_layout;			// ԭʼɨ�鷶Χ
	float m_fScaleFactor;	// �Ŵ����
	CSize m_displaySize;	// ��ʾ�����С
	CRect m_rectCScan;		// Cɨ��ͼ����ʾ��
	CRect m_rectGrid;		// ����������
	CRect m_rectAngle;		// ���ο�
	BOOL m_bMeasureOn;		// �������ܿ�����־
	CPoint m_ptFrom;					// �������
	CPoint m_ptTo;						// �����յ�
	CPoint m_ptCurPos;					// ��ǰλ��
	BOOL m_bMouseLeave;
	BOOL m_nEnterCount;
	float m_SelArea;		//����ѡ�����
	CPoint m_DlbPoint;//˫��cɨ����λ������ // wangbingfu modify on 2012-12-27 private change to public
private:
	CRect m_rectScaleX;		// X����߾�����
	CRect m_rectScaleY;		// Y����߾�����
	
	BOOL m_bRota;			// ��ת������־	
	float m_fRotaAngle;		// ��ת�Ƕ�
    CPoint m_ptRotateCenter;  //��ת����  yachang.huang add on 2012-8-16
	BOOL m_bISRotateCenterExist;  //��ת�����Ƿ����  yachang.huang add on 2012-8-16
	CPoint m_ptObliqueStartPoint; //б�����  yachang.huang add on 2012-8-16
	CPoint m_ptObliqueEndPoint;  //б���յ�  yachang.huang add on 2012-8-16
	CPoint m_ptHorizontalEndPoint;	 //ˮƽ���յ�  yachang.huang add on 2012-8-16
	CSize m_page;			// ҳ��С
	CSize m_line;			// �д�С
//	float m_DefaultMax_X;   //���ȱ���������x
//	float m_DefaultMax_Y;   //���ȱ���������y
//	float m_DefaultMax_Z;   //���ȱ���������z

	BOOL m_bOverViewDisplay;			// ȫ��Cɨ����ʾ��־
	COverViewWnd* m_pOverViewWnd;		// ȫ��Cɨ�贰��ָ��
	
	CMicroscopeWnd* m_pMicroScopeWnd;	// ��΢���Ŵ󴰿�
	BOOL m_bMicroscopeDisplay;
	
	BOOL m_bTracking;					// �Ƿ񻭹��ı�־
	MEASURE_TYPE m_eMeasureType;
	CMovePathView* m_pMovePath;
	LOGICAL_POINT_t m_probePoint;//˫�����������Ӧ�Ĺ�������
	BOOL m_bIsDrawFlag;//�Ƿ�����ʮ��
	BOOL m_bIsDrawSyncFlag;	// �Ƿ�ͬ��ʮ��
public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
private:
	void PrintMessageTableOne(CDC* pDC,const CRect&rect);
	void DrawDefaultResultTable(CDC* pDC,const CRect&rect);//��̽�˽�����
	void DrawDefaultResultTable1(CDC* pDC,const CRect&rect);
	void PrintDefaultWave(CDC* pDC,const CRect&rect, const LOGICAL_POINT_t& point, CPicData*pascan);//������ͼ�����+����+���ɫ+���Σ�
	void DrawWaves(CDC* pDC, const CRect& rect, const LOGICAL_POINT_t& point, CPicData*pascan);//������ͼ
	void DrawGrids(CDC* pDC, const CRect& rect);//������
	void GetPrintMessage(void);//��printmsg.ini�ж�ȡ�ַ�����
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
	//�����������֮��Ŀհ�����
	void FillSpace(CDC* pDC,CRect& rect);

	CPicData_L* getAScanDataByPos(LOGICAL_POINT_t point);

	
public:
	// �����
	// direction 1, _|_,  2,|-, 3Ϊ1������, 4Ϊ2������
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
	CPen m_cspen[22];	//����
	COLORREF m_clrPlatte[21];
	CSize m_CScanSize;
public:
	// ����X���п�������
//	void QuickSortX(FeaturesData* SeqList, int low, int high);
	// ����Y���п�������
//	void QuickSortY(FeaturesData* SeqList, int low, int high);
	// ����X�����㷨
//	int PartitionX(FeaturesData* SeqList, int low, int high);
	// ��X��������ٶ�X��ͬ�ĵ����Y��С�����������
//	void SortYAfterSortX(FeaturesData* SeqList, int low, int high);
	// ����Yֵ�����㷨
//	int PartitionY(FeaturesData* SeqList, int low, int high);
	float getScaleFactor();
	CSize getLayout();
	CSize getDisplaySize();
	CSize setDisplaySize(CSize size);
public:
	void RotateImage(CDC* pDC,double angle,CPoint centerPt);  // yachang.huang modify on 2012-8-16   ��������������Ϊ���ĵ�
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
	float m_fXMove;		// �����ƶ�����	
	float m_fYMove;		// �����ƶ�����
	float m_fYMoveForZuoBiao;
	float m_fGateThreshold[GATE_NUM];
	int m_nColorMode;	// ��ɫ 0����ɫ,1:��ɫ
	int m_nValueMode;	// ȡֵģʽ
	int m_nGateIndex;	// ѡ���բ��
	int m_nAmpFeatureIndex;	//��ֵ����ѡ��
	int m_nTimeFeatureIndex;	//ʱ������ѡ��
	int m_nDrawType;	// ����ģʽ 0:���ȳ��� 1:��ȳ��� 2:���ٳ��� 3:���迹���� 4:��˥������ 5:�ܶȳ���
	float m_fBoundMin;	// ��ʾ��Χ����	
	float m_fBoundMax;	// ��ʾ��Χ����
	BOOL m_bDrawSurfaceFlag;	//�����˱�־
	float m_fSurfacePixeRate[21];	//���˸���ɫ���ص����
	float m_fHanHeRate[21];			//�����ʸ���ɫ����
	int  m_nMemXLENGTH;		//����X���ص㳤��
	int m_nMemYLENGTH;    //����Y���ص㳤��
	float **m_pPixel;		//�������ص�ɫֵ����
	int **m_pPixelCount;	//�������ص�����������
	float m_fXLength;		// ����X����
	float m_fYLength;		// ����Y����
	float m_fGridX;				// X������
	float m_fGridY;				// Y������
	double m_dXStart;		//X���˶����
	double m_dYStart;		//Y���˶����
	double m_dZStart;		//Z���˶����
	double m_dWStart;		// W�˶���� Song Chenguang add on 2013-05-04
	double m_dXEnd;			//X���˶��յ�
	double m_dYEnd;			//Y���˶��յ�	
	double m_dZEnd;			//Z���˶��յ�

	float m_fZuobiaoXmin;	// Cɨ����X��С
	float m_fZuobiaoXmax;	// ����X���
	float m_fZuobiaoYmax;	// Cɨ����Y���
	float m_fZuobiaoYmin;	// Cɨ����Y��С
public:
	// ����
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
	void RepositionProcess(CPoint point, LOGICAL_POINT_t stLogicalPoint);	// ��λ����
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void DrawDlbFlg(CDC* pDC,CPoint point);
	void DrawSyncDlbFlg(CDC* pDC,CPoint point);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
public:
	int m_nOnSizeCount;	// �ڵ�һ��OnSize����m_MemDC
	int m_nObjectNum;	//������������������Cɨ1��Cɨ2
private:
	ReportItem m_S1_ReportItems[30];//printmsg.ini Section �ַ�����Ա
	ReportItem m_S2_ReportItems[30];
	ReportItem m_S3_ReportItems[30];
	//ReportItem m_S3_ReportItems[30];//Aɨ����ȱ��ͼ���
	CString m_csTotalStrArea;
	int m_nINI_S1_NUM;//printmsg.ini Section 1 ��Ա����
	int m_nINI_S2_NUM;
	int m_nINI_S3_NUM;
	int m_nS1_NUM;
	int m_nS2_NUM;
	int m_nS3_NUM;
	int m_nYPos;
	TCHAR m_tcBuf_S1[200];//printmsg.ini section 3
	TCHAR m_tcBuf_S2[200];//printmsg.ini section 3
	TCHAR m_tcBuf_S3[200];//printmsg.ini section 3
	int m_nFlawPrintRow_NUM; //�����������
	UINT m_uMaxPageNum;//��ӡ���ҳ��
	UINT m_uCScanLength;//cɨͼ�߶�
private:
		// WangQianfeng add start on 2012-11-21
		COLORREF m_defectFontColor;
		int m_nlfUnderline;//�»��ߡ�����Ϊ���ز���̫�࣬���Զ���ɳ�Ա����
		int m_nlfDeleteline;//ɾ���ߡ�����Ϊ���ز���̫�࣬���Զ���ɳ�Ա����

		int m_nSymbol1Enable;
		int m_nSymbol2Enable;
		int m_nSymbol3Enable;
		// WangQianfeng add end on 2012-11-21
private:
	void InitReportItems();

	void PrintPageHeader(CDC   *pDC,   CPrintInfo   *pInfo);
	void PrintPageFooter(CDC   *pDC);
public:
	double m_dPipeDiameter;		// �ܲ�ֱ��
public:
	int m_nPipeXMode;	//�ܰ���X����ģʽ
	int m_nAxisSelect; //ƽ��ѡ��
public:
	BOOL m_bFileFeatureFlag[7];	//���ļ����������ر�־
	BOOL m_bFileGateFlag[CHANNEL_NUM][GATE_NUM];		//���ļ���բ�ſ��ر�־
	int m_nFileGateNum[CHANNEL_NUM];	//���ļ���բ�Ŵ�����
	int m_nFileFeatureNum;				//���ļ�������������
	void PictureModeText(int nIndex,CString strText);
	void PictureGateText(int nIndex,CString strText);
	CString m_strPictureMode;	//����ģʽ�����ַ���
	CString m_strPictureGate;	//����բ�ű����ַ���
	
	BOOL MallocPictureMemory();// ���뻭ͼ����ڴ�  Song Chenguang modify on 2013-07-18
	void FreePictureMemory();// �����ͼ����ڴ�
	BOOL bFreePictureMemoy;//�Ƿ������ͼ����ڴ�  yachang.huang add on 2014-04-24
public:
	float m_fThick;			//�Կ���
	int m_nColorDefine;		//��ɫ���� 0=�Զ��壻1=Ĭ����ɫ
	//Machao modify start on 2013-07-3
//	COLORREF m_clrDefine[193];	//��ɫֵ����
	int GetColorNumber(float fValue, float fBoundMin, float fBoundMax);
	//Machao modify end on 2013-07-3
	COLORREF m_grayDefine[193];		//��ɫֵ����
public:
	void CaculateVelocity();
	void CalcuateShuaijian();
	void CaculateSzk();
	void CaculateMidu();
	void CalculatePolygonArea(); // �����������   yachang.huang add start on 2012-6-1
public:
	int m_nProbeNum;			// ̽ͷ����
	int m_nScanType;			// ɨ�鷽ʽ�����л����
	int m_nFillType;			// ��䷽ʽ����������
	int m_nTwoProbeIndex;		// ����̽ͷ��Ϸ�ʽ
	float m_fProbeDistance;		// ̽ͷ���
	float m_fChongDie;			// �ص�����
	CToolTipCtrl m_ToolTip;	// ������ʾ��
	CPoint m_Poly[POLY_VERTEX_NUM];  //����ζ�������  yachang.huang add start on 2012-6-1
	int m_nPolyNum;    //����ζ�������   yachang.huang add start on 2012-6-1
    float m_fPolygonArea;  //��������   yachang.huang add start on 2012-6-1
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);   //yachang.huang add on 2012-8-14   ���Oncreate��Ϣ������
public:
	LOGICAL_POINT_t m_stLogicalFlaw;	//�˵�����
	void DrawProbePosition(double dX, double dY, double dZ, double dW);
	CPoint m_ptProbeCurPos;					// ��ǰλ��
	BOOL m_bEnterFlag;						// ����ʾ����
	CCScanParamPanel* m_pCScanParamView;
	double m_dX;		//��ǰX��ֵ
	double m_dY;		//��ǰY��ֵ
	double m_dZ;		//��ǰZ��ֵ
	double m_dR;		//��ǰR��ֵ
	BOOL m_bDblClk;		//���˫����־
	void DrawMemDCRegion(CDC* pDC, CPoint ptFrom, CPoint ptTo, const MEASURE_TYPE& eType, BOOL bFillFlag);
	COLORREF m_cscan_back_color;		//Cɨ���ڻ�ͼ������ɫ
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
	BOOL m_bGuanLianFlag;	//����ʱȷ�����Ƿ����,�����޸�ģʽ���仯
public:
	CPoint m_ptNoteCurrent;
	CPoint Logical2MemDC(LOGICAL_POINT_t ptLogical);
//	int m_nSavedDC;   // rem by Song Chenguang on 2013-07-17 ������Դй©
	BOOL m_bNotEditFlag;  //ע�ͱ༭,ΪTRUEʱ���������������ɫ
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
	float m_fPipeProbeAngle;	// �ܲ�̽ͷ�Ƕ�
	int GetPipeGuanLianCard();
	int bRealTimeDrawMode;
	// wangbingfu add end on 2013-01-09
	// Song Chenguang add start on 2013-06-27
	CList<LOGICAL_REGION, LOGICAL_REGION&> m_lstCustumSurfaceRegion;	// �Զ����˷�Χ����	
	// Song Chenguang add end on 2013-06-27
public:
	afx_msg void OnSelSurfaceCenter();
public:
	afx_msg void OnSelSurfaceEdge();
	// Song Chenguang add start on 2013-06-27
private:
	BOOL m_bSelSurfaceCenter;	// ѡ�����ĵ��־
	BOOL m_bSurfaceCenterSelected;	// ���ĵ���ѡ��־
	BOOL m_bSelSurfaceEdge;	// ѡ���Ե���־
	void CalcOneCustumFlaw(LOGICAL_REGION& stRegion, const float& fThreshold);	// ����ָ����ֵ����Χ����
public:
	char* m_pcCustumFlawBuffer;			// �Զ�������ϢBuffer
	long m_lCustumFlawBufferSize;		// Buffer��С
	// machao add start on 2013-09-17
	CList<long, long&> m_lstCustumFlawBufferSize;
	BOOL m_bEdgePoint;		// �Ƿ���ӱ�Ե��
	// machao add end on 2013-09-17
	LOGICAL_POINT_t m_stCurPoint;
	// Song Chenguang add end on 2013-06-27
	// Song Chenguang add start on 2013-07-02
	float m_fCenterDB;		// ��ѡ�������ĵ�DB
	float m_fEdgeDB;		// ��ѡ���˱�Ե��DB
	// Song Chenguang add end on 2013-07-02
public:
	afx_msg void OnCustumSurfaceDel();
	afx_msg void OnUpdateSelSurfaceEdge(CCmdUI *pCmdUI);
#if _HAFEI
	float CalcKongXiLv(const float& fdB, const float& fThickness);	// �����϶�� Song Chenguang add on 2013-07-11
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
	PLATTE m_arrayPlatte[PLATTE_NUM];	// ɫ����ɫ
	int m_nPlatteSize;	// ɫ��ʵ����ɫ����
	// Song Chenguang add end on 2013-07-12
	//Machao add start on 2013-07-12	
	float **m_pfSrcData;	//���봦���ķ�ֵ��λָ��
	BOOL m_bNoiseReduction;		//�Ƿ���н��������TRUE���н��봦��FLASE�����н��봦��
	BOOL m_bImage;			//�Ƿ�ѡ��Cɨ1����Cɨ2������
	afx_msg void OnNoiseReduction();	//����DFClass12.dll���н������ݴ���
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
	BOOL m_bCompensateRange;		// �Ƿ񻭾ֲ����油����Χ
	CPartGainCompensateDlg *m_pPartGainCompensateDlg;
	void DrawDBCompensatesite();
	LRESULT OnDialogDestroyed(WPARAM wParam, LPARAM lParam);
	// machao add end on 2013-08-16
	CPoint Logical2MemDC(float fPointX, float fPointY);	// machao add on 2013-08-20
	// machao add start on 2013-08-23
	BOOL m_bMouseGainRangeSelect;
	BOOL m_bDrawGainRange;
	// machao add end on 2013-08-23
	int m_nAllCount; // ��Χ�����еĵ���	// machao add on 2013-09-3
	float *m_pfCustomHanheRate;	// �Զ�����ɫ�����ʸ���ɫ����	// machao add on 2013-09-6
	int m_nDblClickSelect;	// ˫��ѡ�� wangbingfu add on 2014-09-10
	typedef struct _LINE
	{
		CPoint p1;
		CPoint p2;
	}LINE_T;
	vector<LINE_T> m_vecLine;
};

#ifndef _DEBUG  // UltrasonicImmersedTestView.cpp �еĵ��԰汾
inline CUltrasonicImmersedTestDoc* CCScanView::GetDocument() const
{ return reinterpret_cast<CUltrasonicImmersedTestDoc*>(m_pDocument); }
#endif
