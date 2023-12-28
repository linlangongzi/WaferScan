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
const int CHANNEL_NUM = 8;
const int GATE_NUM = 4;		// Song Chenguang add on 2012-07-31
const int PER_CARD_CHANNEL_NUM = 2;
const float MAX_HEIGHT = 1023.0f;
const float WAVE_ZOOM_SCALE = 1.00;

const WORD MIN_MAIKUAN = 50;
const WORD MAX_MAIKUAN = 500;
const WORD MIN_RE_FREQ = 50;
const float MAX_LVBO = 1000.0f;
const float MIN_LVBO = 100.0f;
const WORD PER_CH_DATA_NUM = 60;
const WORD DMA_COUNT = 5060;
const float PI = 3.1415926f;
// WangQianfeng add start on 2013-06-28
const int SPLITTER_VIEW_NUM = 5;
// WangQianfeng add end on 2013-06-28
// Speed
const WORD WAVESPEED_STEEL = 5980;
//X与Y像素比例
//const float X_Y_SCALE = (float)6 / 4;
// Timer
#define TIMER_THREAD_PROCESS		1
#define TIMER_SAVE_DMA_DATA			2
#define MECHANICAL_TIMER			3
#define TIMER_SAVE_PICTURE			4
#define TIMER_NET_JUDGE				5	//网络通讯失败判断
#define TIMER_CREATE_TABLE			6	
#define TIMER_GET_ASCAN				7	
#define TIMER_ASYNC_SAVE_SCAN		11
#define TIMER_DRAW_PROBE_POSITTION	12

#define TIMER_WAITT_FOR_DETAIL		50
#define TIMER_WAITT_FOR_READ		51
#define TIMER_WAITT_FOR_PICTURE		52
#define TIMER_WAITT_FOR_FLAW_CALC	53
#define TIMER_WAITT_FOR_HANHE_CALC	54

#define TIMER_AUTO_GAIN				55
#define TIMER_DEFECT_INFO			56
#define TIMER_TIME_COUNT			57

// message
#define WM_A_SCAN_DATA		WM_USER+0x100
#define WM_PLATE_SCAN_START WM_USER+0x101
#define WM_PLATE_SCAN_END	WM_USER+0x102
#define WM_FEATURE_INFO		WM_USER+0x103
#define WM_HIDE_FEATURE		WM_USER+0x104
#define WM_MOVE_OVERVIEW    WM_USER+0x105
#define WM_CSCAN_DRAW		WM_USER+0x106
#define WM_BSCAN_DRAW		WM_USER+0x107
#define WM_USER_DIALOG_DESTROYED	WM_USER+0x108	//machao add on 2013-08-16

const int OVERVIEW_WIDTH = 400;
const int OVERVIEW_HEIGHT = 200;
//const float CODER_PER_RING = 500.0f;
const float CODER_PER_RING_XY = 10000.0f * 25 / 264;//131072.0f * 25 / 264; //500.0f;	//10000 * 25.0f / 264.0f / 4.0f;
const float CODER_PER_RING_Z = 10000.0f * 25 / 155;//131072.0f * 25 / 155;//500.0f;	//10000 * 25.0f / 155.0f / 4.0f;
const float CODER_PER_RING_W1 = 10000.0f * 35 / 360;//131072.0f * 25 / 360;//500.0f;	//10000 * 25.0f / 155.0f / 4.0f;
//const float CODER_PER_RING_XY = 10000 * 25.0f / 264.0f / 4.0f;
//const float CODER_PER_RING_Z = 10000 * 25.0f / 155.0f / 4.0f;
// wangbingfu add start on 2013-01-09
const float CODER_PER_RING_A = 10000.0f * 8 / 360;
const float CODER_PER_RING_B = 10000.0f * 10 / 360;
// wangbingfu add end on 2013-01-09
#define GATE_NUM 4

const CString g_strDBConnection = _T("Provider=sqloledb;Data Source=192.168.1.55;Initial Catalog=scj;User Id=sa;Password=");
//const CString g_strDBConnection = _T("Driver={SQL Server};Server=Localhost;Trusted_Connection=no;Database=ms;Uid=sa;Pwd=");

const int CSCAN_SCALE_X_LENGTH = 50;  //yachang.huang modify on 2012-8-8 调高Y轴标尺的宽度
const int CSCAN_SCALE_Y_LENGTH = 36;  //yachang.huang modify on 2012-8-8 调高X轴标尺的高度

#define TCG_CURVE_POINT_NUM 20
#define DAC_CURVE_POINT_NUM 20
#define DAC_CURVE_NUM 4

const float MIN_PLUS = 0.0f;
const float MAX_PLUS = 80.0f;

const int DATA_SIZE = 512;
const WORD MIN_PRESSURE = 50;
const WORD MAX_PRESSURE = 400;
const float MIN_AMEND = 0;
const float MAX_AMEND = 99;
const float MIN_RANGE = 30.72f;//Dot2Range(VSOUND, MONITOR_SIZE);
const float MAX_RANGE = 10000.0f;

const float YIQI_ADJUST = 0.925f;

const float DRIVE_WHEEL_DIAMETER = 100.0f;	//驱动轮直径

const long MAX_POINTS_NUM = 3000000;//500000;wanbingfu modify on 2015-07-09

const int RE_FREQ_MIN = 100;		// 重复频率最小值
const int RE_FREQ_MAX = 8000;		// 重复频率最大值 ADFREQUENCY成400，最大值为16000 ；；ADFREQUENCY成200，最大值为8000 ； ADFREQUENCY成100，最大值为4000 yachang.huang modify on 2013-11-26

const int TCG_SRAM_NUM = 1024;		//TCG SRAM
#define RF_REDUCE_DB	14			// 射频时须要减小的DB wangbingfu add at 2012-11-21

// wangbingfu add start at 2012-12-5
const int ADD_MEM_X_LENGTH = 200;
const int ADD_MEM_Y_LENGTH = 200;
// wangbingfu add end at 2012-12-5

const int CSCAN_VIEW_NUM = 2;	// wangbingfu add on 2013-01-09
const int WATER_SPEED = 1500;	// 水中声速 m/s
const int PLATTE_NUM = 256;		// 调色板颜色最大个数 或 存储自定义颜色信息数组的大小
const int NET_DATA_SIZE = 128;  // 单通道单次数据传输个数 Song Chenguang add on 2013-07-17
//machao add start on 2013-07-25
// machao modify start on 2013-08-13
//const float MAXFVALUE = 127.5;	//幅值上限大小
const float MAXFVALUE = 100;	//幅值上限大小
// machao modify end on 2013-08-13
//machao add end on 2013-07-25
const UINT SYSPARAM1_NUM = 45;	// GraphMove控件GetPara方法参数1个数
const UINT NET_JUDGE_TIME = 60000;
const UINT POLY_VERTEX_NUM = 60;	// 顶点数量 Song Chenguang add on 2013-12-23
#endif