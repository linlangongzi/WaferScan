#ifndef _COMMONDEFINE_H_
#define _COMMONDEFINE_H
/**
* Copyright(c) 2012 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 常数定义头文件
* Filename: CommonDefine.h
* 
* @author        Wang Qianfeng
* @version       1.00         Date: 2012-08-16
*/

#define	GAIN_DATA_MAX						255
#define GAIN_MAX							35.0
#define GAIN_MIN							0
/* 放大器线性修改系数*/
#define AEC_AMP_LINEAR_PARA_A				0.90
#define AEC_AMP_LINEAR_PARA_B				0.032
#define	GAIN_DATA_MIN						0

#define ADFREQUENCY							400             // MHz//由于下位机硬件升级，由100改成200  yachang.huang modify on 2013-12-2 
#define MONITOR_SIZE						512

#endif