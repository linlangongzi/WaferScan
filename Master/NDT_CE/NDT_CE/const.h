#ifndef _CONST_H_
#define _CONST_H_

const WORD  REF_GAIN_DATA = 128;

const float AEC_AMP_LINEAR_PARA_A = 0.9f;
const int	GAIN_DATA_MAX = 255;
const int	GAIN_DATA_MIN = 0;
const float GAIN_MAX = 120.0f;
const float GAIN_MIN = 0.0f;
//const float REF_GAIN = 15.0f;

const int ADFREQUENCY = 400;    //吴越调试改为400  yachang.huang modify on 2014-06-16          
// MHz  //由于下位机硬件升级，由100改成200  yachang.huang modify on 2013-11-16 
const WORD MIN_STATUS_HEIGHT = 48;
const WORD SCALE_WIDTH = 32;
const WORD TEXT_HEIGHT = 32;

const WORD VSOUND = 6000;            // m/s
const WORD MONITOR_SIZE = 512;

const float MIN_RANGE = 30.72;//Dot2Range(VSOUND, MONITOR_SIZE);
const float MAX_RANGE = 10000;
//const int MIN_DELAY = -10;
//const int MAX_DELAY = 12000;
const WORD MIN_PLUS = 0;
//const WORD MAX_PLUS = 110;
const WORD MIN_MAIKUAN = 50;
const WORD MAX_MAIKUAN = 500;
const WORD MIN_PRESSURE = 50;
const WORD MAX_PRESSURE = 400;
const WORD MIN_MID_VALUE = 0;
const WORD MAX_MID_VALUE = 1023;
const WORD MIN_RE_FREQ = 50;
const WORD MAX_RE_FREQ = 200;
const float MIN_AMEND = 0;
const float MAX_AMEND = 99;
const WORD MIN_BATE = 0;
const WORD MAX_BATE = 80;
const WORD MIN_WAVE_SPEED = 950;//wangbingfu modify at 2010-7-27,zhaoli modify at 2010-12-30
const WORD MAX_WAVE_SPEED = 10000;
const float MIN_ANGLE = 0.0f;
const float MAX_ANGLE = 89.9f;

const WORD MIN_LVBO_TYPE = 0;
const WORD MAX_LVBO_TYPE = 5;

const WORD MIN_PEAK = 0;
const WORD MAX_PEAK = 100;

const WORD MIN_LCD = 1;
const WORD MAX_LCD = 11;
const WORD MIN_VOLUMN = 0;
const WORD MAX_VOLUMN = 5;
const WORD MIN_TYPE = 0;
const WORD MAX_TYPE = 2;


const float MAX_fPlateHeight = 10000.0f;
const float AMENDSTEPS[4] = {0.1, 1, 10, 100};	// 修正步长值
const float PLUSSTEPS[4] = {0.1, 2, 6, 12};		// 增益步长值
const WORD WAVESPEEDSTEPS[3] = {1,10,100};		// 声速步长值
const float ANGLESTEPS[2] = {0.1, 1};			// 角度步长值
const WORD LVBOLITTLEMODIFY[2] = {1, 10};		// 滤波微调步长值
const float CURVEGAINSTEPS[2] = {0.1, 1};		// 评定线跟判废线增益步长值
const float FAIZHISTEPS[2] = {0.1, 1};			//Φ值步长值
const float USSTEPS[5] = {0.01, 0.1, 1, 10, 100};		// us所用步长值
const float RATESTEPS[3] = {0.1, 1, 10};		//百分比所用步长值
//zhaoli add 9-3
const float GONGJIANHOUDUSTEPS[4] = {0.1,1,10,100};//B扫工件厚度步长值
const float PLATEHEIGHT[4] = {0.1,1,10,100};//中厚板探伤工件厚度步长值
//zhaoli add 9-3


const double PI = 3.1415926;
const int DATA_SIZE = 512;
const float MAX_HEIGHT = 1023.0f;

//const LPCTSTR lpDefaultParamFile = _T("\\ResidentFlash\\1.par");

const WORD CHANNEL_NUM = 2;
const WORD GATE_NUM = 3;
const float YIQI_ADJUST = 0.925f;
const WORD SYS_COLOR_NUM = 5;

const float MIN_BIAOCHENGZHI = 0.1;
const float SOFTWARE_VER = 1.24f;

const TCHAR szCraftParamChild[] = TEXT("craftParamChild");
const TCHAR szCreateCurveChild[] = TEXT("createCurveChild");
const TCHAR szModifyCurveChild[] = TEXT("modifyCurveChild");
const TCHAR szCurveChild[] = TEXT("CurveChild");
const TCHAR szDisplayChild[] = TEXT("displayChild");
const TCHAR szFileManageChild[] = TEXT("FileManageChild");
const TCHAR szBScanChild[] = TEXT("BScanChild");
const TCHAR szCreateDacCurveChild[] = TEXT("createDacCurveChild");
const TCHAR szDgsCurveChild[] = TEXT("dgsCurveChild");
const TCHAR szDgsMakeChild[] = TEXT("dgsMakeChild");
const TCHAR szHanFengTanShangChild[] = TEXT("hanFengTanShangChild");
const TCHAR szHuiBoBianMaChild[] = TEXT("huiBoBianMaChild");
const TCHAR szFnMenuChild[] = TEXT("fnMenuChild");
const TCHAR szModifyDgsCurveChild[] = TEXT("modifyDgsChild");
const TCHAR szPoKouSheZhiChild[] = TEXT("poKouSheZhiChild");
const TCHAR szQuantitativeAnalysisChild[] = TEXT("quantitativeAnalysisChild");
const TCHAR szAutoGainPlusChild[] = TEXT("autoGainPlusChild");
const TCHAR szAdjustChild[] = TEXT("fnAdjustChild");

const TCHAR szReferenceWaveChild[] = TEXT("referenceWaveChild");
const TCHAR szSpecialDisplayChild[] = TEXT("specialDisplayChild");
const TCHAR szSpecialFunctionChild[] = TEXT("specialFunctionChild");
const TCHAR szVideoRecordChild[] = TEXT("videoRecordChild");
const TCHAR szZhongHouBanTanShangChild[] = TEXT("zhongHouBanTanShangChild");
const TCHAR szGateDisplayChild[] = TEXT("gateDisplayChild");
const TCHAR szGateChild[] = TEXT("gateChild");
const TCHAR szHotKeyChild[] = TEXT("hotkeyChild");
const TCHAR szMakeCurveChild[] = TEXT("makeCurveChild");
const TCHAR szModeChild[] = TEXT("modeChild");
const TCHAR szOtherCfgChild[] = TEXT("otherCfgChild");
const TCHAR szParamChild[] = TEXT("paramChild");
const TCHAR szRecvParamChild[] = TEXT("recvParamChild");
const TCHAR szReportFormChild[] = TEXT("reportFormChild");
const TCHAR szScreenCopyChild[] = TEXT("ScreenCopyChild");
const TCHAR szSendParamChild[] = TEXT("sendParamChild");
const TCHAR szTsMenuChild[] = TEXT("tsMenuChild");
const TCHAR szTcgCurveChild[] = TEXT("chTcgCurveChild");
const TCHAR szWaveSpeedChild[] = TEXT("waveSpeedChild");
const TCHAR szRefractAngleChild[] = TEXT("refractAngleChild");
const TCHAR szAllParamCfgChild[] = TEXT("allParamCfgChild");
const TCHAR szAllParamCfgCaptionChild[] = TEXT("allParamCfgCaptionChild");
const TCHAR szAllParamConfirmChild[] = TEXT("allParamConfirmChild");
const TCHAR szLogoChild[] = TEXT("logoChild");
const TCHAR szZhuJianChild[] = TEXT("zhuJianTanShangChild");
const TCHAR szReportFormViewChild[] = TEXT("reportFormViewChild");
const TCHAR szQuMianXiuZhengChild[] = TEXT("quMianXiuZhengChild");
const TCHAR szAvgCurveChild[] = TEXT("avgCurveChild");
const TCHAR szMakeAvgCurveChild[] = TEXT("makeAvgCurveChild");
const TCHAR szCreateAvgCurveChild[] = TEXT("createAvgCurveChild");
const TCHAR szModifyAvgCurveChild[] = TEXT("modifyAvgCurveChild");
const TCHAR szFuHeCaiLiaoJianCeChild[] = TEXT("fuHeCaiLiaoJianCeChild");
const TCHAR szLieWenCeGaoChild[] = TEXT("lieWenCeGaoChild");

const TCHAR szChMenuChild[] = TEXT("chMenuChild");
const TCHAR szChParamChild[] = TEXT("chParamChild");
const TCHAR szChMeasureCfgChild[] = TEXT("chMeasureCfgChild");
const TCHAR szChAdjustChild[] = TEXT("chAdjustChild");
const TCHAR szChAlarmCfgChild[] = TEXT("chAlarmCfgChild");
const TCHAR szChWaveSpeedChild[] = TEXT("chWaveSpeedChild");
const TCHAR szChWaveSpeedAdjustChild[] = TEXT("chWaveSpeedAdjustChild");
const TCHAR szChZeroAdjustChild[] = TEXT("chZeroAdjustChild");
const TCHAR szChTempCompensateChild[] = TEXT("chTempCompensateChild");
const TCHAR szChTempCurveMakeChild[] = TEXT("chTempCurveMakeChild");
const TCHAR szChTempCurveCreateChild[] = TEXT("chTempCurveCreateChild");
const TCHAR szChHeadVCompensateChild[] = TEXT("chHeadVCompensateChild");
const TCHAR szChHeadVMakeChild[] = TEXT("chHeadVMakeChild");
const TCHAR szChHeadVCreateChild[] = TEXT("chHeadVCreateChild");
const TCHAR szChDisplayChild[] = TEXT("chDisplayChild");
const TCHAR szChMeasureContentChild[] = TEXT("chMeasureContentChild");
const TCHAR szChZeroSingleChild[] = TEXT("chZeroSingleChild");
const TCHAR szChZeroDoubleChild[] = TEXT("chZeroDoubleChild");
const TCHAR szChAverageChild[] = TEXT("chAverageChild");
const TCHAR szTlMenuChild[] = TEXT("tlMenuChild");
const TCHAR szTlAdjustChild[] = TEXT("szTlAdjustChild");

const int LCD_XSIZE_TFT = 640;


#define RGB888toRGB565(r,g,b) ((WORD)(((WORD(r)<<8)&0xF800)|((WORD(g)<<3)&0x7E0)|((WORD(b)>> 3))))
#define RGBtoRGB565(rgb)       ((WORD)(((((WORD)((rgb)>>3))&(0x1F))<<11)|((((WORD)((rgb)>>10))&(0x3F))<<5)|(((WORD)((rgb)>>19))&(0x1F))))
#define DAC_CURVE_NUM	4
#define AVG_CURVE_NUM	3
#define MAX_CURVE_POINT_NUM	100
#define MIN_CURVE_GAIN  -20
#define MAX_CURVE_GAIN  20
#define MIN_FAIZHI  0
#define MAX_FAIZHI  20
#define MIN_GUANCAIWAIJING 1
#define MAX_GUANCAIWAIJING 1000
#define MIN_GONGJIANHOUDU 5
#define MAX_GONGJIANHOUDU 100

#define TL_STANDARD_NUM 5
#define TL_POSITION_NUM 18

#define BUFFER_SIZE 5120
#define MAX_BUFFER_SIZE 8500
#define TCG_CURVE_POINT_NUM 20
// wangbingfu modify start on 2015-025-12
#define ASCAN_DATA_STORE_SIZE 16320
//#define ASCAN_DATA_STORE_SIZE 17280	//60帧 a扫压缩
// wangbingfu modify end on 2015-025-12
#endif
