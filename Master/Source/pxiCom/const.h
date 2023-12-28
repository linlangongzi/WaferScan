#ifndef _CONST_H_
#define _CONST_H_

//#include <windows.h>
#include <qt_windows.h>
#include <minwindef.h>

//#define GAIN_10_BIT

const int MONITOR_SIZE = 512;
const int MONITOR_SIZE_DMA = MONITOR_SIZE * 2;
const int GATE_NUM = 4;

#ifdef PIPE_DETECT
const float MAX_HEIGHT = 511.0f;    // 管探
const int MAX_CARD_NO = 2;
#else
const float MAX_HEIGHT = 1023.0f;   // 南车
const int MAX_CARD_NO = 16;
#endif

const WORD WAVESPEED_STEEL = 5980;
const WORD SELF_TEST_VALUE = 0xaaaa;
const int ADFREQUENCY = 100;
const int FLAW_DOT_MAX_NO = 512;
#ifdef GAIN_10_BIT
const int GAIN_DATA_MAX = 1023;     // 8位时255,10位时1023
const int GAIN_AB_CONST = 511;      // 8位时128,10位时511 A,B增益下发固定值
#else
const int GAIN_DATA_MAX = 255;      // 8位时255,10位时1023
const int GAIN_AB_CONST = 128;      // 8位时128,10位时511 A,B增益下发固定值
#endif
const int GAIN_DATA_MIN = 0;
const float GAIN_MAX = 40.0f;
const float GAIN_MIN = 0.0f;
const float GAIN_OFFSET = 0.7f; // 增益计算所用的矫正值
/* 放大器线性修改系数*/

const float AEC_AMP_LINEAR_PARA_A = 0.90f;
const float AEC_AMP_LINEAR_PARA_B = 0.032f;
const int DELAY_TIME = 500;
const int PCI_SAMPLE_RESET_ON = 1;
const int PCI_SAMPLE_RESET_OFF = 0;

#endif
