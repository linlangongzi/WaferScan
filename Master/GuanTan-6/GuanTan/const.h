#ifndef _CONST_H_
#define _CONST_H_

#define MAIN2AECTHREADCOMMAND_IDLE			0xFFFF
#define MAIN2AECTHREADCOMMAND_RESET			0
#define MAIN2AECTHREADCOMMAND_PLATE_START	1
#define MAIN2AECTHREADCOMMAND_PLATE_STOP	2
#define MAIN2AECTHREADCOMMAND_SETONE		3
#define MAIN2AECTHREADCOMMAND_INIT			4   /// SET ALL
#define MAIN2AECTHREADCOMMAND_BIAODING		5
#define MAIN2AECTHREADCOMMAND_ZIJIAN		6
#define MAIN2AECTHREADCOMMAND_THREAD_STOP	7
#define MAIN2AECTHREADCOMMAND_SEL_CHANNEL	8
#define MAIN2AECTHREADCOMMAND_NONE			9
#define MAIN2AECTHREADCOMMAND_SETALL		10 //添加所有通道参数下发命令  yachang.huang add on 2013-08-11

#define AECTHREADREURN_IDLE				0xFFFF
#define AECTHREADREURN_RESET_0K			0x0
#define AECTHREADREURN_RESET_FALSE		0x1
#define AECTHREADREURN_START_OK			0x10
#define AECTHREADREURN_START_FALSE		0x11
#define AECTHREADREURN_STOP_OK			0x20
#define AECTHREADREURN_STOP_FALSE		0x21
#define AECTHREADREURN_SETONE_OK		0x30
#define AECTHREADREURN_SETONE_FALSE		0x31
#define AECTHREADREURN_INIT_OK			0x40   /// SET ALL
#define AECTHREADREURN_INIT_FALSE		0x41   /// SET ALL
#define AECTHREADREURN_BIAODING_OK		0x50
#define AECTHREADREURN_BIAODING_FALSE	0x51
#define AECTHREADREURN_ZIJIAN_OK		0x60
#define AECTHREADREURN_ZIJIAN_FALSE		0x61
#define AEC_THREAD_SUSPEND				FALSE
#define AEC_THREAD_RUN					TRUE
#define AECTHREADREURN_THREAD_STOP_OK	0x70
#define WAVESPEED						5980
#define MONITOR_SIZE					 512

#define DEFECTTIMETIP					 514
#define DEFECTWIDTHTIP					 515
#define MINSOUNDDISTANCE				 32
#define GRADLEVEL						1

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 
const int GRID_WIDTH = 10;//网格宽度，单位mm
const int CHANNEL_NUM = 32;
const int PER_CARD_CHANNEL_NUM = 32;
const int BADCOUPLE_RANK = 77; //耦合不良判别标准 15%波幅
const float THICK_CUT_BADPOINT = 0.5f;//测厚去畸点标准
const float MAX_HEIGHT = 490.0f;	//512.0f;
const float WAVE_ZOOM_SCALE = 1.00;

const WORD MIN_MAIKUAN = 50;
const WORD MAX_MAIKUAN = 500;
const WORD MIN_RE_FREQ = 600;
const WORD MAX_RE_FREQ = 4000;
const float MAX_LVBO = 1000.0f;
const float MIN_LVBO = 100.0f;
const WORD PER_CH_DATA_NUM = 60;
const WORD DMA_COUNT = 5060;
const WORD DATA_NUM = 256;

const float PROBEBOX_DISTANCE1 = 0;		// 探头盒1与零点差值
const float PROBEBOX_DISTANCE2 = 220;	// 2
const float PROBEBOX_DISTANCE3 = 440;	// 3
const float PROBEBOX_DISTANCE4 = 660;	// 4
const float PROBEBOX_DISTANCE5 = 880;	// 5
const float PROBEBOX_DISTANCE6 = 1100;	// 6
const float X_TO_LENGTH = 44.737f;	//X值转化为钢管实际位置，转变参数  wangwei modify on 2014-05-05 编码器刚刚安好时是 45.2355
//const float X_TO_LENGTH = 10000.0f / 176.0 / 16.6 * 16 * 0.826;   //X值转化为钢管实际位置，转变参数  wangwei modify on 2014-05-05
const float PIPE_LENGTH = 1000.0f;  //钢管长度1米
const int PIPE_MIN_SHOW_LENGTH = 200;//钢管最小显示长度200毫米
const double SLIDER_LENGTH = 345.00;
// Speed
const WORD WAVESPEED_STEEL = 5980;
//X与Y像素比例
//const float X_Y_SCALE = (float)6 / 4;
// Timer
#define TIMER_THREAD_PROCESS		1
#define TIMER_SAVE_DMA_DATA			2
#define MECHANICAL_TIMER			3
#define TIMER_SAVE_PICTURE			4

// message
#define WM_A_SCAN_DATA		WM_USER+0x100
#define WM_PLATE_SCAN_START WM_USER+0x101
#define WM_PLATE_SCAN_END	WM_USER+0x102
#define WM_FEATURE_INFO		WM_USER+0x103
#define WM_HIDE_FEATURE		WM_USER+0x104
#define WM_MOVE_OVERVIEW    WM_USER+0x105
#define WM_CSCAN_DRAW		WM_USER+0x106
#define WM_BSCAN_DRAW		WM_USER+0x107

const int OVERVIEW_WIDTH = 400;
const int OVERVIEW_HEIGHT = 200;
//const float CODER_PER_RING = 500.0f;
const float CODER_PER_RING_XY = 500.0f;	//10000 * 25.0f / 264.0f / 4.0f;
const float CODER_PER_RING_Z = 500.0f;	//10000 * 25.0f / 155.0f / 4.0f;

//const float CODER_PER_RING_XY = 10000 * 25.0f / 264.0f / 4.0f;
//const float CODER_PER_RING_Z = 10000 * 25.0f / 155.0f / 4.0f;

#define GATE_NUM 4

//const CString g_strDBConnection = _T("Provider=sqloledb;Data Source=192.168.1.55;Initial Catalog=scj;User Id=sa;Password=");
//const CString g_strDBConnection = _T("Driver={SQL Server};Server=Localhost;Trusted_Connection=no;Database=ms;Uid=sa;Pwd=");

const int CSCAN_SCALE_X_LENGTH = 45;
const int CSCAN_SCALE_Y_LENGTH = 23;

#define DAC_CURVE_POINT_NUM 20
#define DAC_CURVE_NUM 4
#define TIMER_AUTO_GAIN 4
#define TIMER_DEFECT_INFO 5
const float MIN_PLUS = 0.0f;
const float MAX_PLUS = 80.0f;

const int MAX_CARD_NO = 2;

//////////////////////////////////////////////////////////////////////////
// 管探 Song Chenguang 2013-05-23
const WORD SELF_TEST_VALUE = 0xaaaa;
const int ADFREQUENCY = 100;
const int FLAW_DOT_MAX_NO = 512;
const int GAIN_DATA_MAX	= 255;
const int GAIN_DATA_MIN	= 0;
const float GAIN_MAX = 35.0f;
const float GAIN_MIN = 0.0f;
/* 放大器线性修改系数*/
const float AEC_AMP_LINEAR_PARA_A = 0.90f;
const float AEC_AMP_LINEAR_PARA_B = 0.032f;
const int DELAY_TIME = 500;
const int PCI_SAMPLE_RESET_ON = 1;
const int PCI_SAMPLE_RESET_OFF = 0;
//////////////////////////////////////////////////////////////////////////
#endif
