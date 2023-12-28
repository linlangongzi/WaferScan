#ifndef _HARD_WARE_ADDRESS_H_
#define _HARD_WARE_ADDRESS_H_

const unsigned int SYS_RESET_ADDRESS = 0x2;                     // 系统复位信号
const unsigned int FEATURE_BUFFER_RESET_ADDRESS = 0x4;          // 特征缓冲区数据复位命令
const unsigned int MONITOR_DATA_RESET_ADDRESS = 0x6;            // 监视数据复位命令
const unsigned int MONITOR_CHANNEL_H_ADDRESS = 0x8;             // 监视数据硬通道选择寄存器
const unsigned int MONITOR_CHANNEL_S_ADDRESS = 0xA;             // 监视数据软通道选择寄存器
const unsigned int MONITOR_CHANNEL_END_ADDRESS = 0xB;           // 相邻的是软通道选择寄存器
const unsigned int MONITOR_CHANNEL_DATA_EN = 0xC;               // 监视通道数据有效标志寄存器
const unsigned int MONITOR_FEATURE_DATA_ADDRESS = 0xE;          // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA_END_ADDRESS = 0x1D;     // 监视通道数据尾地址
const unsigned int MONITOR_RANGE_ADDRESS = 0x00400;             // 监视通道采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS = 0x00800;         // 监视通道采样数据RAM（512）
const unsigned int LOCAL_BUS_ADDRESS = 0x00200;                 // LocalBus交换数据RAM（256）
const unsigned int FEATURE_BUFFER_RESET_FLAG_ADDRESS = 0x10000; // 特征数据缓冲区数据标志位复位寄存器
const unsigned int FEATURE_BUFFER_EN_FLAG_ADDRESS = 0x10002;	// 特征数据缓冲区数据有效标志寄存器
const unsigned int FEATURE_BUFFER_INDEX_ADDRESS = 0x10004;		// 特征数据缓冲区数据buffer序号标志寄存器
const unsigned int FEATURE_BUFFER1_ADDRESS = 0x20000;           // 特征数据缓冲区1（32K）
const unsigned int FEATURE_BUFFER2_ADDRESS = 0x30000;           // 特征数据缓冲区2（32K）
const unsigned int SOFT_CHANNEL_SELECT_MODE_ADDRESS = 0x3A;     // 软通道号选择 0:检测模式，根据光眼自动切换 1：调试状态，切换通道切换
const unsigned int METER2_LAST_EN_ADDRESS = 0x10006;            // 车轮通过2米区域最后一个光眼的触发使能
const unsigned int METER2_LAST_FRAME_NUM_ADDRESS = 0x10008;     // 车轮通过2米区域最后一个光眼时，数据缓冲区应读走的数据帧数
const unsigned int METER2_LAST__BUFFER_INDEX_ADDRESS = 0x1000A; // 车轮通过2米区域最后一个光眼时，应读取的缓冲区选择
const unsigned int EYE_LIGHT_ADDRESS = 0x26;                    // 光眼数据
const unsigned int CARD_NO_ADDRESS = 0x38;                      // 卡编号，填写光眼区域编号（16位一个编号）
const unsigned int SIMULATE_ENABL_ADDR = 0x3E;                  // 电气模拟使能
const unsigned int SIMULATE_DATA_ADDR = 0x42;                   // 电气模拟数据起始地址（光眼信号）
const unsigned int REPEAT_FREQUENCE_ADDR = 0x52;                // 重复频率
const unsigned int ADDR_CHN_EYE_AREA_START = 0x54;              // AEC卡硬通道对应的传感器区域起始地址（0~7 16个传感器为一个区（2米））
const unsigned int SENSOR_ROLL_ENABLE_ADDR = 0x4A;              // 探伤aec卡传感器轮循使能
const unsigned int TIMESTAMP_RESET = 0x54;                      // 时间戳复位
const unsigned int WHEEL_NO_RESET = 0x56;                       // 轮号复位
const unsigned int SENSOR_TIME_STAMP_RESET = 0x4A;              // 触感器aec卡时间戳复位地址

// 硬通道1基地址
// 软通道1基地址
const unsigned int SOFT_CHANNEL_CONFIG_PARARM_ADDRESS = 0x01000;	// 软通道1配置参数基地址(128)
// 软通道1配置参数尾地址
const unsigned int CHANNEL1_TCG_DATA_ADDRESS = 0x1800;                                              // 硬通道1 TCG曲线寄存器(64)

const unsigned int MPB_SIMULATE_ENABL_ADDR = 0xb0;          // 电气模拟使能
const unsigned int MPB_SIMULATE_DATA_ADDR = 0xb2;           // 电气模拟数据起始地址（光眼信号）
const unsigned int MPB_ACTUAL_DATA_ADDR = 0x110;            // 电气实际数据起始地址（光眼信号）

const unsigned int MFC_DATA_BYTE_SIZE = 0x112;

#endif
