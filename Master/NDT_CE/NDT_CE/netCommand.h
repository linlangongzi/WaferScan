#pragma once
#ifndef __COMMAND_H__

/**
 * Copyright(c) 2011 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved.
 *
 * 通信指令集.
 * Filename: Command.h
 *
 *
 * @author       Wang Bingfu
 * @version      1.2         Date: 2011-12-23
 */

#define COMMAND_CURRENT_CHANNEL			0x0001	//当前通道

#define COMMAND_WORKMODE				0x0011	//工作模式

#define COMMAND_SEND_PARAMS				0x0101	//发射参数
#define COMMAND_SEND_VOLTAGE			0x0102	//电压
#define COMMAND_SEND_MAIKUAN			0x0103	//脉宽
#define COMMAND_SEND_REPEAT_FREQ		0x0104	//重复频率
#define COMMAND_SEND_ZERO				0x0105	//零点
#define COMMAND_SEND_QIANYAN			0x0106	//前沿

#define COMMAND_RECV_PARAMS				0x0201	//接收参数
#define COMMAND_RECV_JIANBO_MODE		0x0202	//检波模式
#define COMMAND_RECV_GAIN				0x0203	//增益
#define COMMAND_RECV_REF_GAIN			0x0204	//参考增益
#define COMMAND_RECV_DELAY				0x0205	//延迟
#define COMMAND_RECV_WORK_FREQ			0x0206	//工作频率
#define COMMAND_RECV_ZUKANG				0x0207	//阻抗

#define COMMAND_DISP_PARAMS				0x0301	//显示参数
#define COMMAND_DISP_SX					0x0302	//S/X转换
#define COMMAND_DISP_RANGE				0x0303	//声程
#define COMMAND_DISP_VELOCITY			0x0304	//角度
#define COMMAND_DISP_ANGLE				0x0305	//声速
#define COMMAND_DISP_RESTRAIN			0x0306	//抑制


#define COMMAND_GATE_PARAMS				0x0401	//闸门参数
#define COMMAND_GATE_ALARM_MODE			0x0402	//闸门报警模式
#define COMMAND_GATE_SWITCH				0x0403	//闸门开关
#define COMMAND_GATE_QIDIAN				0x0404	//闸门起点
#define COMMAND_GATE_WIDTH				0x0405	//闸门宽度
#define COMMAND_GATE_PEAK				0x0406	//闸门幅值

#define COMMAND_OTHER_PARAMS			0x0501	//颜色配置
#define COMMAND_OTHER_COLOR				0x0502	//颜色配置
#define COMMAND_OTHER_BACK_GRUOND		0x0503	//背光
#define COMMAND_OTHER_VOLUMN			0x0504	//音量
#define COMMAND_OTHER_STORE_MODE		0x0505	//存储介质

#define COMMAND_ALL_PARAM_SET		0x0601	//上位机设置全部参数

#define COMMAND_RESET				0x0701	//上位机复位请求

#define COMMAND_ALL_PARAM_REQ		0x1001	//上位机获取全部参数请求
#define COMMAND_WAVE_REQ			0x1002	//通道波形数据请求
#define COMMAND_VERSION_REQ			0x1003	//版本号请求
#define COMMAND_WORK_MODE			0x1004	// 设置工作模式
#define COMMAND_TCG_SWICH			0x1005	// 设置TCG曲线
#define COMMAND_ASCAN_DATA_STROE_REQ 0x1006  //A扫数据存储请求  yachang.huang add on 2013-12-19

#define COMMAND_WAVE_DATA		0x2001	//下位机发送波形数据
#define COMMAND_ALL_PARAM_DATA		0x2002	//下位机发送全部参数
#define COMMAND_DEFECT_DATA		0x2003	//下位机发送伤信息
#define COMMAND_ASCAN_DATA_STROE   0x2006 //A扫存储数据  yachang.huang add on 2013-12-19

#define COMMAND_DATA_FAULT		0xFF01	//数值不在范围之内

#define __COMMAND_H__
#endif

