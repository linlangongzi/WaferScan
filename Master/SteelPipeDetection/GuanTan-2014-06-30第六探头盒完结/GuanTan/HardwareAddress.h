#ifndef _HARD_WARE_ADDRESS_H_
#define _HARD_WARE_ADDRESS_H_

const DWORD PROBEBOX_UP_DOWN = 0x110;		//探头盒起落标志  wangwei add on 2013-12-9
const DWORD START_FLAG_ADDRESS = 0x11c;  //探伤开始/结束标识位
const DWORD HARDWARE_VERSION_ADDRESS = 0x0;	// 版本信息
const DWORD SYS_RESET_ADDRESS = 0x2;		// 系统复位信号
const DWORD FEATURE_BUFFER_RESET_ADDRESS = 0x4;	// 特征缓冲区数据复位命令
const DWORD MONITOR_DATA_RESET_ADDRESS = 0x6;	// 监视数据复位命令
const DWORD MONITOR_CHANNEL_H_ADDRESS = 0x8;		// 监视数据硬通道选择寄存器
const DWORD MONITOR_CHANNEL_S_ADDRESS = 0xA;		// 监视数据软通道选择寄存器
const DWORD MONITOR_CHANNEL_END_ADDRESS = 0xB;				// 相邻的是软通道选择寄存器
const DWORD MONITOR_CHANNEL_DATA_EN = 0xC;		// 监视通道数据有效标志寄存器
const DWORD MONITOR_FEATURE_DATA_ADDRESS = 0xE;		// 监视通道数据首地址
const DWORD MONITOR_FEATURE_DATA_END_ADDRESS = 0x1D;	// 监视通道数据尾地址
const DWORD AEC_CARD_SELFTEST_RESULT_ADDRESS = 0x1E;		// 卡自检结果
const DWORD MONITOR_RANGE_ADDRESS = 0x00400;		// 监视通道采样间隔RAM（512）
const DWORD MONITOR_WAVE_DATA_ADDRESS = 0x00800;	// 监视通道采样数据RAM（512）
const DWORD SYS_SELF_TEST_ADDRESS = 0x00C00;	// 系统自检SRAM（512）
const DWORD SYS_SELF_TEST_ADDRESS_END = 0x00FFF;	// 系统自检SRAM尾地址
const DWORD LOCAL_BUS_ADDRESS = 0x00200;		// LocalBus交换数据RAM（256）
const DWORD FEATURE_BUFFER_RESET_FLAG_ADDRESS = 0x10000;// 特征数据缓冲区数据标志位复位寄存器
const DWORD FEATURE_BUFFER_EN_FLAG_ADDRESS = 0x10002;	// 特征数据缓冲区数据有效标志寄存器
const DWORD FEATURE_BUFFER_INDEX_ADDRESS = 0x10004;		// 特征数据缓冲区数据buffer序号标志寄存器
const DWORD FEATURE_BUFFER1_ADDRESS = 0x20000;	// 特征数据缓冲区1（32K）
const DWORD FEATURE_BUFFER2_ADDRESS = 0x30000;	// 特征数据缓冲区2（32K）
// 硬通道1基地址
// 软通道1基地址
const DWORD SOFT_CHANNEL_CONFIG_PARARM_ADDRESS = 0x01200;	// 软通道1配置参数基地址(64)
const DWORD SOFT_CHANNEL_CONFIG_PARARM_END_ADDRESS = 0x0125B;	// 软通道1配置参数尾地址(64)
// 软通道1配置参数尾地址
const DWORD SOFT_CHANNEL1_TCG_DATA_ADDRESS = 0x01400;		// 软通道1 TCG曲线寄存器(256)
const DWORD SOFT_CHANNEL1_TCG_DATA_END_ADDRESS = 0x015FF;

// 多功能卡
#define MPB_CARD_READY_OK					0x1
const WORD MPB_CARD_READY_ERROR = 0x0;	
const DWORD MPB_CARD_READY_ADDR =	0x02;

#define MPB_SYS_READY_ADDR					0x08
#define MPB_SYS_READY_OK					0x1
#define MPB_SYS_READY_ERROR					0x0	

#define MULTI_CARD_SELF_TEST_ADDR            0x80
#define MULTI_CARD_SELF_TEST_ADDR_SIZE       8

#define SUB_DEVICE_ID_H                    0x2e
#define SUB_DEVICE_ID_L                    0x2c
#define MUTLI_FUNCTION_CARD_ID             0x101
#endif