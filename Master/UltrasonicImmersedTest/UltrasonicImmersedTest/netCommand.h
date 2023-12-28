#pragma once

#ifndef __NET_COMMAND_H__
#define __NET_COMMAND_H__
/**
 * Copyright(c) 2011 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved.
 *
 * 通信指令集.
 * Filename: netCommand.h
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

#define COMMAND_ALL_PARAM_REQ		0x1001	//上位机获取全部参数请求
#define COMMAND_WAVE_REQ		0x1002	//通道波形数据请求
//#define COMMAND_PEAK_REQ		0x1003	//通道伤时伤幅数据请求


#define COMMAND_WAVE_DATA		0x2001	//下位机发送波形数据
#define COMMAND_ALL_PARAM_DATA		0x2002	//下位机发送全部参数
#define COMMAND_DEFECT_DATA		0x2003	//下位机发送伤信息

#define COMMAND_DATA_FAULT		0xFF01	//数值不在范围之内
#define COMMAND_RESET				0x0701	//上位机复位请求

#define COMMAND_WORK_MODE			0x1004	// 设置工作模式
#define COMMAND_TCG_SWICH			0x1005	// 设置TCG曲线


const DWORD NETHEAD_SIZE = sizeof(PACKET_HEAD_t);

const WORD CMD_DQTD = 0x0001;
const DWORD PARAM_DQTD_LEN = sizeof(WORD);

const WORD CMD_GZMS = 0x0011;
const DWORD PARAM_GZMS_LEN = sizeof( WORKMODE);

const WORD CMD_FSCS = 0x0101;
//const DWORD PARAM_FSCS_LEN = sizeof( SEND_PARAM_DATA);
const WORD CMD_FSCS_DY = 0x0102;
const DWORD PARAM_FSCS_DY_LEN = sizeof(WORD);
const WORD CMD_FSCS_MK = 0x0103;
const DWORD PARAM_FSCS_MK_LEN = sizeof(WORD);
const WORD CMD_FSCS_CFPL = 0x0104;
const DWORD PARAM_FSCS_CFPL_LEN = sizeof(DWORD);
const WORD CMD_FSCS_LD = 0x0105;
const DWORD PARAM_FSCS_LD_LEN = sizeof(float);
const WORD CMD_FSCS_QY = 0x0106;
const DWORD PARAM_FSCS_QY_LEN = sizeof(float);

const WORD CMD_JSCS = 0x0201;
//const DWORD PARAM_JSCS_LEN = sizeof( RECEIVE_PARAM_DATA);
const WORD CMD_JSCS_JBFS = 0x0202;
const DWORD PARAM_JSCS_JBFS_LEN = sizeof(WORD);
const WORD CMD_JSCS_ZY = 0x0203;
const DWORD PARAM_JSCS_ZY_LEN = sizeof(float);
const WORD CMD_JSCS_CKZY = 0x0204;
const DWORD PARAM_JSCS_CKZY_LEN = sizeof(float);
const WORD CMD_JSCS_YC = 0x0205;
const DWORD PARAM_JSCS_YC_LEN = sizeof(float);
const WORD CMD_JSCS_GZPL = 0x0206;
const DWORD PARAM_JSCS_GZPL_LEN = sizeof(WORD);
const WORD CMD_JSCS_ZK = 0x0207;
const DWORD PARAM_JSCS_ZK_LEN = sizeof(WORD);

const WORD CMD_XSCS = 0x0301;
//const DWORD PARAM_XSCS_LEN = sizeof( DISPLAY_SET_DLG_DATA);
const WORD CMD_XSCS_SX = 0x0302;
const DWORD PARAM_XSCS_SX_LEN = sizeof( BOOL);
const WORD CMD_XSCS_SC = 0x0303;
const DWORD PARAM_XSCS_SC_LEN = sizeof( float);
const WORD CMD_XSCS_SS = 0x0304;
const DWORD PARAM_XSCS_SS_LEN = sizeof( WORD);
const WORD CMD_XSCS_ZSJ = 0x0305;
const DWORD PARAM_XSCS_ZSJ_LEN = sizeof( float);
const WORD CMD_XSCS_YZ = 0x0306;
const DWORD PARAM_XSCS_YZ_LEN = sizeof( WORD);

const WORD CMD_ZM = 0x0401;
//const DWORD PARAM_ZM_LEN = sizeof( GATE_INFO);
const WORD CMD_ZM_BJMS = 0x0402;
//const DWORD PARAM_ZM_BJMS_LEN = sizeof( alarm_modes);
const WORD CMD_ZM_ZMKG = 0x0403;
const DWORD PARAM_ZM_ZMKG_LEN = sizeof( BOOL);
const WORD CMD_ZM_QD = 0x0404;
const DWORD PARAM_ZM_QD_LEN = sizeof( float);
const WORD CMD_ZM_KD = 0x0405;
const DWORD PARAM_ZM_KD_LEN = sizeof( float);
const WORD CMD_ZM_FZ = 0x0406;
const DWORD PARAM_ZM_FZ_LEN = sizeof( float);

const WORD CMD_QTCS = 0x0501;
//const DWORD PARAM_QTCS_LEN = sizeof( OTHER_PARAM_t);
const WORD CMD_QTCS_YS = 0x0502;
const DWORD PARAM_QTCS_YS_LEN = sizeof( int);
const WORD CMD_QTCS_BG = 0x0503;
const DWORD PARAM_QTCS_BG_LEN = sizeof( int);
const WORD CMD_QTCS_YL = 0x0504;
const DWORD PARAM_QTCS_YL_LEN = sizeof( int);
const WORD CMD_QTCS_CCJZ = 0x0505;
const DWORD PARAM_QTCS_CCJZ_LEN = sizeof( int);

const WORD CMD_SET_ALL_PARAM = 0x0601;
//const DWORD PARAM_SET_ALL_PARAM_lEN = sizeof( WIRELESS_ALL_PARAM_t);
const WORD CMD_GET_ALL_PARAM = 0x1001;
const WORD CMD_GET_ALL_PARAM_RET = 0x2002;

const WORD CMD_ASCAN = 0x1002;
const WORD CMD_ASCAN_DATA_STORE = 0x1006;	// Song Chenguang add on 2013-12-19
const WORD CDM_ASCAN_RET_WAVE = 0x2001;
const WORD CDM_ASCAN_RET_PEAK = 0x2003;
const WORD CMD_ASCAN_RET_DATA_STORE = 0x2006;	// Song Chenguang add on 2013-12-19

//const DWORD DATA_ASCAN_RET_LEN = sizeof( REAL_SEND_DATA_t);
//const DWORD DATA_PEAK_RET_LEN = sizeof( PEAK_INFO);

const DWORD MAX_BUF_LEN = 1024;
const DWORD MIN_BUF_LEN = 6;

const DWORD CMD_SET_PARAM_RET = 0xff01;

#define COMMAND_RESET				0x0701	//上位机复位请求
#define COMMAND_VERSION_REQ			0x1003	//版本号请求

#endif

