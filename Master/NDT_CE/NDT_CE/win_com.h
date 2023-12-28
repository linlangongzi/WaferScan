#ifndef __WINCOM_H__
#define __WINCOM_H__

#define E_TBL_END -1
#define DEFAULT_LABLE_TEXT_HEIGHT 30
#define STATUSBAR_HEIGHT 48
#define LABEL_START_X	24
#define LABEL_START_Y	50
#define LABEL_3_START_Y	80
#define LABEL_4_START_Y	54
#define LABEL_5_START_Y	37
#define LABEL_6_START_Y	40
#define LABEL_7_START_Y	30
#define LABEL_HEIGHT	25
#define LABEL_HEIGHT2	45
#define LABEL_WIDTH		73
#define MENU_START_X	(640 - MENU_WIDTH)
#define MENU_START_Y	STATUSBAR_HEIGHT
#define MENU_WIDTH		116
#define MENU_HEIGHT		(480 - STATUSBAR_HEIGHT)
#define BUTTON_2_EACH_HEIGHT	(MENU_HEIGHT/2)
#define BUTTON_3_EACH_HEIGHT	(MENU_HEIGHT/3)
#define BUTTON_4_EACH_HEIGHT	(MENU_HEIGHT/4)
#define BUTTON_5_EACH_HEIGHT	(MENU_HEIGHT/5)
#define BUTTON_6_EACH_HEIGHT	(MENU_HEIGHT/6)
#define BUTTON_7_EACH_HEIGHT	(MENU_HEIGHT/7)
#define BK_COLOR_WHITE RGB(255,255,255)
#define BK_COLOR_LIGHTGRAY RGB(222,222,221)
#define STATIC_TEXT_COLOR RGB(0,0,0)
#define WHITE_POINT 65535
#define RED_POINT 63488
#define BLUE_POINT 31
#define BLACK_POINT 0
#define GREEN_POINT RGB888toRGB565(0, 255, 0)
#define GRAY_POINT RGB888toRGB565(128, 128, 128)
#define CURVE_COLOR RED_POINT
#define HEAD_FREQUENCE_1M 1000000.0
#define HEAD_FREQUENCE_50M 50000000.0
#define WAVE_AREA_HEIGHT (480 - STATUSBAR_HEIGHT - SCALE_WIDTH)
#define PARAM_STRART_X 0
#define PARAM_STRART_Y 30
#define PARAM_HEIGHT 480
#define PARAM_WIDTH 640
#define EACH_ITEM_HEIGHT 30
#define EACH_ITEM_WIDTH (640 - 20)
#define STRING_START_X 0
#define STRING_START_Y 0
#define STRING_HEIGHT 30
#define STRING_WIDTH  250
#define ALLCFG_LABEL_START_X	350
#define ALLCFG_LABEL_START_Y	3
#define ALLCFG_LABEL_WIDTH		150
#define ALLCFG_LABEL_HEIGHT		25
#define DANWEI_START_X 520
#define DANWEI_WIDTH  	50
#define CONFIRM_STRING_HEIGHT 50
#define CONFIRM_STRING_WIDTH  80

#define HEADV_POINT_MAX  512
#define TEMPCURVE_POINT_MAX  512
#define ON TRUE
#define OFF FALSE

#define DINGLIANGFENXI_SWITCH			0x00000001		//��������
#define SPECIALDISPLAY_SWITCH			0x00000002		//������ʾ
#define SPECIALFUNCTION_SWITCH			0x00000004		//ר�ù���
#define RECORD_SWITCH					0x00000008		//¼��
#define DGSCURVE_SWITCH					0x00000010		//DGS����
#define AUTOGAIN_SWITCH					0x00000020		//�Զ�����
#define AUTOADJUST_SWITCH				0x00000040		//�Զ�У׼
#define PEAKMEMORY_SWITCH				0x00000080		//��ֵ����
#define DYNAMICINCLUDE_SWITCH			0x00000100		//��̬����
#define FREEZE_SWITCH					0x00000200		//����
#define REFRENCEWAVE_SWITCH				0x00000400		//�ο���
#define HUIBOBIANMA_SWITCH				0x00000800		//�ز�����
#define MENNEIZHANKUAN_SWITCH			0x00001000		//����չ��
#define DISPLAY110_SWITCH				0x00002000		//110%��ʾ
#define BSCAN_SWITCH					0x00004000		//Bɨ��
#define ZHONGHOUBANTANSHANG_SWITCH		0x00008000		//�к��̽��
#define HANFENGTANSHANG_SWITCH			0x00010000		//����̽��
#define FUHECAILIAOJIANCE_SWITCH		0x00020000		//���ϲ��ϼ��
#define ZHUJIANTANSHANG_SWITCH			0x00040000		//����̽��
#define WORKMODE1OR2_SWITCH				0x00080000		//��ͨ��
#define WORKMODE1PLUS2_SWITCH			0x00100000		//˫ͨ��
#define WORKMODE1TO2_SWITCH				0x00200000		//һ��һ��
#define WORKMODEDANJING_SWITCH			0x00400000		//��������
#define WORKMODESHUANGJING_SWITCH		0x00800000		//˫������
#define DEVICETYPECHANGE_SWITCH			0x01000000		//�豸����ת��
#define RAILROAD_SWITCH					0x02000000		//��·ר��
#define LIEWENCEGAO_SWITCH				0x04000000		//���Ʋ��

#define HEAD_NUM		10
#define MATERIAL_NUM	1	//�¶Ȳ�����������
#define VIDEO_NUM		10
#define VIDEO_MEMORY_NUM 3600
#define SCREEN_DOT_NUM	640*480
typedef struct btn_info{
	int nBtnID;
	int nBtnNoFocID;
	int nBtnFocID;
}BTN_BMP_INFO;

typedef struct DGS_CURVE_tag{
	BOOL bSwitch[CHANNEL_NUM][GATE_NUM];
	float fOldRang[CHANNEL_NUM];
	WORD wOldSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];
	WORD selectedDgsPointDataNum[CHANNEL_NUM][GATE_NUM];
	CURVEPOINT_DATA selectedDgsPointData[CHANNEL_NUM][GATE_NUM][MAX_CURVE_POINT_NUM];
}DGS_CURVE;

typedef struct measure_info{
	float fValue;		//ֵ
	BOOL bSwitch;		//����
}MEASURE_INFO;

typedef struct measure_display_all{
	WORD wHeadType;				//0:���� 1:˫��
	WORD wMethod;				//0;��������-�ز��� 1:����-����ز��� 2:���ڵ���ز��� 3:��͸Ϳ�����
	WORD wChaZhiMode;			//��ֵģʽ 0:����ֵ 1�����ֵ
	float fBiaoChengZhi;		//���ֵ
	float fTuCengHouDu;			//Ϳ����
	MEASURE_INFO stMax;			//���ֵ
	MEASURE_INFO stMin;			//��Сֵ
	MEASURE_INFO stAverage;		//ƽ��ֵ
	MEASURE_INFO stCurrent;		//��ǰֵ
	MEASURE_INFO stChaZhi;		//��ֵ
}MEASURE_DISPLAY_ALL;

typedef struct measure_alarm{
	WORD wAlarmMode;		//����ģʽ 0;���ֵ���� 1;��Сֵ���� 2:��ֵ����
	float fChaoChaYuZhi;	//������ֵ
	BOOL bSwitch;			//����
}MEASURE_ALARM;

typedef struct headVPoint_data{
	float fMeasureHouDu;
	float fActualHouDu;
}HEADVPOINT_DATA;

typedef struct tempCurvePoint_data{
	float fTemperature;
	WORD wWaveSpeed;
}TEMPCURVEPOINT_DATA;

typedef struct video_data{
	WORD wPicData[CHANNEL_NUM][DATA_SIZE];
}VIDEO_DATA;
typedef struct video_info{
	WORD wWorkMode;				//����ģʽ
	WORD wCurrentChannel;			//��ǰͨ��
	float fBasePlus;				//��׼����
	float fRefPlus;					//�ο�����
	float fRangeStart;				//�������
	float fRangeEnd;				//�����յ�
	VIDEO_DATA stData;//[VIDEO_MEMORY_NUM];
//	DWORD dwDataNum;
}VIDEO_INFO;
typedef struct qumian_xiuzheng_info{
	float fShenDu;
	float fLength;
}QUMIANXIUZHENG_INFO;
//enum ENUM_MATERIAL{ziDingYi = 1, gang, tei, tong, taoCi, buXiuGang, lv, huangTong,
//				xin, huanYangShuZhi, juLvYiXi};

int WinDrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis,const BTN_BMP_INFO *pBtnBmpInfo);
int com_GetButtonBitmapID(const BTN_BMP_INFO *p,int Index,int ButtonState);
void registerAll(HINSTANCE hInstance);
//HBITMAP ScreenCapture(LPCWSTR fileName, WORD bitCount, LPRECT lpRect);
int saveDataToFile(LPCTSTR szFilePath,LPVOID lpData,DWORD dwDataSize);

void DrawLableText(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart);
void DrawLableText1(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight);
void DrawLableText2(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight);
void DrawLableText3(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight);
void DrawWaveAreaText(HDC hDC, RECT rect, LPCTSTR lpString, HFONT hFontInfo, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight);
void drawSrting(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart, int nWidth, int nHeight);

BOOL ScreenCapture(LPCWSTR fileName, WORD bitCount, LPRECT lpRect);

POINT dataToPoint(CURVEPOINT_DATA dac_point_data);
POINT dataToPoint1(CURVEPOINT_DATA curve_point_data, RECT RctWave);
void getGain2TgcSram(CURVEPOINT_DATA tempPointData[], DWORD dataNum, WORD channelNumber);
//void drawCurve(CURVEPOINT_DATA dataToDraw[], DWORD dataNum, BOOL drawFlag, WORD wColor);
void drawCurve(RECT RctWave, CURVEPOINT_DATA selectPointData[], DWORD dwDataNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag);
void drawLine(POINT point1, POINT point2, WORD wColor);
void drawLine1(POINT point1, POINT point2, WORD wColor, RECT RctWave);
void sortSelectedPointDataBySerial(CURVEPOINT_DATA queueToSort[], DWORD selectedDacPointDataNum);

DWORD drawShiZiPoint(CURVEPOINT_DATA unDrawPointData, CURVEPOINT_DATA drawPointData,
				   CURVEPOINT_DATA DrawSelectedPointData[],DWORD selectedPointNum, 
				   BOOL drawWaveFlag, BOOL drawDacPoinFlag);

CURVEPOINT_DATA getHeightestPoinData(CURVEPOINT_DATA dataForSearch[], DWORD dataNum);
BOOL buDian(CURVEPOINT_DATA queueToBuDian[],CURVEPOINT_DATA queueAfterBuDian[], DWORD dataNum);
void sortSelectedPointDataBySerial(CURVEPOINT_DATA queueToSort[], DWORD selectedDacPointDataNum);
void write2GateSram(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dataNum,WORD channelNumber,WORD gateNumber);


void drawShiZhi(POINT point, WORD wColor, BOOL bDrawFlag, RECT RctWave);
void DisplayClear(BOOL bMenuShown);
void closeAllGate();

BOOL readDgsFile(LPCTSTR lpszPathName);
BOOL saveDgsFile(LPCTSTR lpszPathName);

BOOL ReadWaveFile(LPCTSTR szFilePath,LPVOID lpFileDataBuffer);//test
void drawWaveFromWaveFile(LPCTSTR szFilePath);//test
void clearWave();
BOOL saveWaveFile(LPCTSTR lpszFilePath, WORD wWaveData[]);//test

void eraseDrawLine(POINT point1, POINT point2, WORD nBackColor, WORD wColor);
void eraseDrawCurve(CURVEPOINT_DATA selectDacPointData[], DWORD dataNum, WORD nBackColor,WORD wColor);

void drawDashLine(POINT point1, POINT point2, WORD wColor);
void drawDashLine1(POINT point1, POINT point2, WORD wColor, RECT RctWave);
void drawDashCurve(RECT RctWave, CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dwDataNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag);

//void drawDacCurve(CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dataNum, BOOL drawFlag, WORD wColor);
void drawDacDashCurve(CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dataNum, BOOL drawFlag, WORD wColor);

void FreezeUp(HWND hWnd, const BOOL& bOpen);
void FreezeLeftMove();
void FreezeRightMove();
void DrawPeakInfo();

float firstHuiBoDistance();
float secondHuiBoDistance();
float zeroAdjust(float fThickness);
WORD waveSpeedAdjust(float fThickness);

POINT headVDataToPoint(HEADVPOINT_DATA headVPoint_data);
void sortSelectedPointDataByHoudu(HEADVPOINT_DATA queueToSort[], DWORD selectedPointNum);
void drawCurve1(POINT selectPoint[], DWORD dwPointNum, BOOL bDrawFlag);
void drawDashCurve1(POINT selectPoint[], DWORD dwPointNum, BOOL bDrawFlag);
void drawShiZhi1(POINT point, WORD wColor, BOOL bDrawFlag);
POINT tempCurveDataToPoint(TEMPCURVEPOINT_DATA stTempCurvePoint_data);
void sortSelectedPointDataByTemp(TEMPCURVEPOINT_DATA queueToSort[], DWORD selectedPointNum);
void waveAreaDrawText(LPCTSTR szText, RECT RctInfo);//������ʾ����д����
void drawAfaCurve(RECT RctWave, CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag);
void drawDashAfaCurve(RECT RctWave, CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag);
void drawCurve2(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,WORD wColor, BOOL bDrawFlag);
void drawCurve3(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,const WORD& wColor, BOOL bDrawFlag, RECT RctWave);
void drawDashCurve2(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,WORD wColor, BOOL bDrawFlag);
void drawDashCurve3(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,WORD wColor, BOOL bDrawFlag, RECT RctWave);
QUMIANXIUZHENG_INFO quMianXiuZheng(DWORD dwGateDefectTime, float fWaiBanJing, float fNeiBanJing, float fZheSheJiao, float fQianYan);
float dacDeltaDB(WORD wGateDefectAmp, DWORD dwGateDefectTime, CURVEPOINT_DATA stDacDingLiang[MAX_CURVE_POINT_NUM], WORD wPointNum);
float calculateFaiZhi(WORD wGateDefectAmp, DWORD dwGateDefectTime, CURVEPOINT_DATA stCurveData[MAX_CURVE_POINT_NUM], WORD wPointNum);
float curveDataChangeByGain(float dwDataToChange, float fGain);

float curveChangeByDelay(float fDataToChange, float fRangeStartChange, float fRange);//�����������ӳٸı�
#endif//__WINCOM_H__