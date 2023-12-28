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

#define DINGLIANGFENXI_SWITCH			0x00000001		//定量分析
#define SPECIALDISPLAY_SWITCH			0x00000002		//特殊显示
#define SPECIALFUNCTION_SWITCH			0x00000004		//专用功能
#define RECORD_SWITCH					0x00000008		//录像
#define DGSCURVE_SWITCH					0x00000010		//DGS曲线
#define AUTOGAIN_SWITCH					0x00000020		//自动增益
#define AUTOADJUST_SWITCH				0x00000040		//自动校准
#define PEAKMEMORY_SWITCH				0x00000080		//峰值记忆
#define DYNAMICINCLUDE_SWITCH			0x00000100		//动态包络
#define FREEZE_SWITCH					0x00000200		//冻结
#define REFRENCEWAVE_SWITCH				0x00000400		//参考波
#define HUIBOBIANMA_SWITCH				0x00000800		//回波编码
#define MENNEIZHANKUAN_SWITCH			0x00001000		//门内展宽
#define DISPLAY110_SWITCH				0x00002000		//110%显示
#define BSCAN_SWITCH					0x00004000		//B扫描
#define ZHONGHOUBANTANSHANG_SWITCH		0x00008000		//中厚板探伤
#define HANFENGTANSHANG_SWITCH			0x00010000		//焊缝探伤
#define FUHECAILIAOJIANCE_SWITCH		0x00020000		//复合材料检测
#define ZHUJIANTANSHANG_SWITCH			0x00040000		//铸件探伤
#define WORKMODE1OR2_SWITCH				0x00080000		//单通道
#define WORKMODE1PLUS2_SWITCH			0x00100000		//双通道
#define WORKMODE1TO2_SWITCH				0x00200000		//一发一收
#define WORKMODEDANJING_SWITCH			0x00400000		//单晶关联
#define WORKMODESHUANGJING_SWITCH		0x00800000		//双晶关联
#define DEVICETYPECHANGE_SWITCH			0x01000000		//设备类型转换
#define RAILROAD_SWITCH					0x02000000		//铁路专用
#define LIEWENCEGAO_SWITCH				0x04000000		//裂纹测高

#define HEAD_NUM		10
#define MATERIAL_NUM	1	//温度补偿材料种类
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
	float fValue;		//值
	BOOL bSwitch;		//开关
}MEASURE_INFO;

typedef struct measure_display_all{
	WORD wHeadType;				//0:单晶 1:双晶
	WORD wMethod;				//0;激励脉冲-回波法 1:界面-底面回波法 2:相邻底面回波法 3:穿透涂层测量
	WORD wChaZhiMode;			//差值模式 0:绝对值 1：相对值
	float fBiaoChengZhi;		//标称值
	float fTuCengHouDu;			//涂层厚度
	MEASURE_INFO stMax;			//最大值
	MEASURE_INFO stMin;			//最小值
	MEASURE_INFO stAverage;		//平均值
	MEASURE_INFO stCurrent;		//当前值
	MEASURE_INFO stChaZhi;		//差值
}MEASURE_DISPLAY_ALL;

typedef struct measure_alarm{
	WORD wAlarmMode;		//报警模式 0;最大值超差 1;最小值超差 2:差值超差
	float fChaoChaYuZhi;	//超差阈值
	BOOL bSwitch;			//开关
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
	WORD wWorkMode;				//工作模式
	WORD wCurrentChannel;			//当前通道
	float fBasePlus;				//基准增益
	float fRefPlus;					//参考增益
	float fRangeStart;				//声程起点
	float fRangeEnd;				//声程终点
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
void waveAreaDrawText(LPCTSTR szText, RECT RctInfo);//波形显示区内写文字
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

float curveChangeByDelay(float fDataToChange, float fRangeStartChange, float fRange);//曲线数据随延迟改变
#endif//__WINCOM_H__