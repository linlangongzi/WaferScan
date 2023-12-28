/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 常量定义
* Filename: const.h
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-05-20
*/

#ifndef _CONST_H
#define _CONST_H
#include "windows.h"

//常量定义处
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
#define MAIN2AECTHREADCOMMAND_NONE			9    //应该是空命令 by chenyuzhu 2014-4-23 
#define MAIN2AECTHREADCOMMAND_SET_CHANNEL_SETONE 10		// zhen dui tong shi qie huan tongdao he can shu xia fa --wangwei 2016-1-1

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
const int PER_CARD_CHANNEL_NUM = 32;
const float MAX_HEIGHT = 505.0f; //490.0f;	//512.0f;
const float WAVE_ZOOM_SCALE = 1.00;

const WORD MIN_MAIKUAN = 50;
const WORD MAX_MAIKUAN = 500;
const WORD MIN_RE_FREQ = 50;
const WORD MAX_RE_FREQ = 200;
const float MAX_LVBO = 1000.0f;
const float MIN_LVBO = 100.0f;
const WORD PER_CH_DATA_NUM = 60;
const WORD DMA_COUNT = 5060;

// Speed
const WORD WAVESPEED_STEEL = 5980;
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
const float CODER_PER_RING_XY = 500.0f;	//10000 * 25.0f / 264.0f / 4.0f;
const float CODER_PER_RING_Z = 500.0f;	//10000 * 25.0f / 155.0f / 4.0f;


#define GATE_NUM 4


const int CSCAN_SCALE_X_LENGTH = 45;
const int CSCAN_SCALE_Y_LENGTH = 23;


#define DAC_CURVE_POINT_NUM 20
#define DAC_CURVE_NUM 4
#define TIMER_AUTO_GAIN 4
#define TIMER_DEFECT_INFO 5

#define ASCAN_DATA_SIZE 512

const float MIN_PLUS = 0.0f;
const float MAX_PLUS = 80.0f;

///////////////////////////////////
const DWORD MAX_FLAW_COUNT = 200000;
const WORD TYPE_LOST_WAVE = 0;
const WORD TYPE_FLAW = 1;
const WORD TYPE_HOUDU = 2;


#endif
