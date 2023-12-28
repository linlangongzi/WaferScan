#ifndef _HARD_WARE_ADDRESS_H_
#define _HARD_WARE_ADDRESS_H_

const unsigned int VERSIONS = 0x0;                              // 版本信息
const unsigned int SYS_RESET_ADDRESS = 0x2;                     // 系统复位信号
const unsigned int FEATURE_BUFFER_RESET_ADDRESS = 0x4;          // 特征缓冲区数据复位命令 AEC卡地址
const unsigned int MONITOR_DATA_RESET_ADDRESS = 0x6;            // 监视数据复位命令 低7-0位分别代表 7-0号缓冲区
const unsigned int MONITOR_CHANNEL_H_ADDRESS = 0x8;             // 监视数据伤波与界面波切换寄存器 7-0位有效
const unsigned int MONITOR_CHANNEL_S_ADDRESS = 0xA;             // 监视数据软通道选择寄存器 1-0位有效
const unsigned int MONITOR_CHANNEL_DATA_EN = 0xC;               // 8个监视通道数据有效标志寄存器 7-0位有效

// 监视通道闸门特征值缓冲区1~8
const unsigned int MONITOR_FEATURE_DATA1_ADDRESS = 0xE;         // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA1_END_ADDRESS = 0x1F;    // 监视通道数据尾地址
const unsigned int MONITOR_FEATURE_DATA2_ADDRESS = 0x38;        // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA2_END_ADDRESS = 0x49;    // 监视通道数据尾地址
const unsigned int MONITOR_FEATURE_DATA3_ADDRESS = 0x4A;        // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA3_END_ADDRESS = 0x5B;    // 监视通道数据尾地址
const unsigned int MONITOR_FEATURE_DATA4_ADDRESS = 0x5C;        // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA4_END_ADDRESS = 0x6D;    // 监视通道数据尾地址
const unsigned int MONITOR_FEATURE_DATA5_ADDRESS = 0x6E;        // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA5_END_ADDRESS = 0x7F;    // 监视通道数据尾地址
const unsigned int MONITOR_FEATURE_DATA6_ADDRESS = 0x80;        // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA6_END_ADDRESS = 0x91;    // 监视通道数据尾地址
const unsigned int MONITOR_FEATURE_DATA7_ADDRESS = 0x92;        // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA7_END_ADDRESS = 0xA3;    // 监视通道数据尾地址
const unsigned int MONITOR_FEATURE_DATA8_ADDRESS = 0xA4;        // 监视通道数据首地址
const unsigned int MONITOR_FEATURE_DATA8_END_ADDRESS = 0xB5;    // 监视通道数据尾地址

// 1-0位硬通道·  1软通道号     3-2位硬通道   2软通道号
// 5-4位硬通道·  3软通道号     7-6位硬通道   4软通道号
// 9-8位硬通道·  5软通道号     11-10位硬通道 6软通道号
// 13-12位硬通道 7软通道号     15-14位硬通道 8软通道号
const unsigned int MONITOR_SOFT_NUMBER_ADDRESS = 0xB6;

const unsigned int CODER_X_LOW_16 = 0x22;                       // 编码器X低16位
const unsigned int CODER_X_HIGH_16 = 0x24;                      // 编码器X高16位
const unsigned int CODER_R_LOW_16 = 0x34;                       // 编码器R低16位
const unsigned int CODER_R_HIGH = 0x36;                         // 编码器R高16位
const unsigned int PROBE_BOX_STATUS = 0x26;                     // 探头盒起落状态标识
const unsigned int PROBE_BOX_DEBUG_ENABLE = 0x28;               // 探头盒离线调试使能 开启
const unsigned int PROBE_BOX_DEBUG_DISENABLE = 0x2A;            // 探头盒离线调试使能 关闭

// 8个监视数据缓冲区
const unsigned int MONITOR_RANGE_ADDRESS_1 = 0x400;             // 监视通道1采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_1 = 0x800;         // 监视通道1采样数据RAM（512）

// 后添加的地址，故与前面地址有所差别
const unsigned int MONITOR_RANGE_ADDRESS_2 = 0x10400;           // 监视通道2采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_2 = 0x10800;       // 监视通道2采样数据RAM（512）
const unsigned int MONITOR_RANGE_ADDRESS_3 = 0x10C00;           // 监视通道3采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_3 = 0x11000;       // 监视通道3采样数据RAM（512）
const unsigned int MONITOR_RANGE_ADDRESS_4 = 0x11400;           // 监视通道4采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_4 = 0x11800;       // 监视通道4采样数据RAM（512）
const unsigned int MONITOR_RANGE_ADDRESS_5 = 0x11C00;           // 监视通道5采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_5 = 0x12000;       // 监视通道5采样数据RAM（512）
const unsigned int MONITOR_RANGE_ADDRESS_6 = 0x12400;           // 监视通道6采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_6 = 0x12800;       // 监视通道6采样数据RAM（512）
const unsigned int MONITOR_RANGE_ADDRESS_7 = 0x12C00;           // 监视通道7采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_7 = 0x13000;       // 监视通道7采样数据RAM（512）
const unsigned int MONITOR_RANGE_ADDRESS_8 = 0x13400;           // 监视通道8采样间隔RAM（512）
const unsigned int MONITOR_WAVE_DATA_ADDRESS_8 = 0x13800;       // 监视通道8采样数据RAM（512）

const unsigned int LOCAL_BUS_ADDRESS = 0x200;                   // LocalBus交换数据RAM（256）
const unsigned int FEATURE_BUFFER_RESET_FLAG_ADDRESS = 0x10000; // 特征数据缓冲区数据标志位复位寄存器
const unsigned int FEATURE_BUFFER_EN_FLAG_ADDRESS = 0x10002;    // 特征数据缓冲区数据有效标志寄存器
const unsigned int FEATURE_BUFFER_INDEX_ADDRESS = 0x10004;      // 特征数据缓冲区数据buffer序号标志寄存器
const unsigned int FEATURE_BUFFER1_ADDRESS = 0x20000;           // 特征数据缓冲区1 0x20000-0x2FFFF 24000（32K）256*94（每组94个关键字，每次256组）
const unsigned int FEATURE_BUFFER2_ADDRESS = 0x30000;           // 特征数据缓冲区2 0x30000-0x3FFFF 24000（32K）256*94（每组94个关键字，每次256组）

const unsigned int SOFT_CHANNEL1_CONFIG_PARARM_ADDRESS = 0x1200;        // 软通道1配置参数基地址 0x01200-0x0127F(128)
const unsigned int SOFT_CHANNEL_CONFIG_PARARM_END_ADDRESS = 0x0125B;    // 软通道1配置参数尾地址(64)

const unsigned int SOFT_CHANNEL1_TCG_DATA_ADDRESS = 0x1800;         // 软通道1 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL2_TCG_DATA_ADDRESS = 0x1a00;         // 软通道2 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL3_TCG_DATA_ADDRESS = 0x1c00;         // 软通道3 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL4_TCG_DATA_ADDRESS = 0x1e00;         // 软通道4 TCG曲线寄存器地址

const unsigned int SOFT_CHANNEL5_TCG_DATA_ADDRESS = 0x2800;         // 软通道5 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL6_TCG_DATA_ADDRESS = 0x2a00;         // 软通道6 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL7_TCG_DATA_ADDRESS = 0x2c00;         // 软通道7 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL8_TCG_DATA_ADDRESS = 0x2e00;         // 软通道8 TCG曲线寄存器地址

const unsigned int SOFT_CHANNEL9_TCG_DATA_ADDRESS = 0x3800;         // 软通道9 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL10_TCG_DATA_ADDRESS = 0x3a00;         // 软通道10 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL11_TCG_DATA_ADDRESS = 0x3c00;         // 软通道11 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL12_TCG_DATA_ADDRESS = 0x3e00;         // 软通道12 TCG曲线寄存器地址

const unsigned int SOFT_CHANNEL13_TCG_DATA_ADDRESS = 0x4800;         // 软通道13 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL14_TCG_DATA_ADDRESS = 0x4a00;         // 软通道14 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL15_TCG_DATA_ADDRESS = 0x4c00;         // 软通道15 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL16_TCG_DATA_ADDRESS = 0x4e00;         // 软通道16 TCG曲线寄存器地址

const unsigned int SOFT_CHANNEL17_TCG_DATA_ADDRESS = 0x5800;         // 软通道17 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL18_TCG_DATA_ADDRESS = 0x5a00;         // 软通道18 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL19_TCG_DATA_ADDRESS = 0x5c00;         // 软通道19 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL20_TCG_DATA_ADDRESS = 0x5e00;         // 软通道20 TCG曲线寄存器地址

const unsigned int SOFT_CHANNEL21_TCG_DATA_ADDRESS = 0x6800;         // 软通道21 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL22_TCG_DATA_ADDRESS = 0x6a00;         // 软通道22 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL23_TCG_DATA_ADDRESS = 0x6c00;         // 软通道23 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL24_TCG_DATA_ADDRESS = 0x6e00;         // 软通道24 TCG曲线寄存器地址

const unsigned int SOFT_CHANNEL25_TCG_DATA_ADDRESS = 0x7800;         // 软通道25 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL26_TCG_DATA_ADDRESS = 0x7a00;         // 软通道26 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL27_TCG_DATA_ADDRESS = 0x7c00;         // 软通道27 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL28_TCG_DATA_ADDRESS = 0x7e00;         // 软通道28 TCG曲线寄存器地址

const unsigned int SOFT_CHANNEL29_TCG_DATA_ADDRESS = 0x8800;         // 软通道29 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL30_TCG_DATA_ADDRESS = 0x8a00;         // 软通道30 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL31_TCG_DATA_ADDRESS = 0x8c00;         // 软通道31 TCG曲线寄存器地址
const unsigned int SOFT_CHANNEL32_TCG_DATA_ADDRESS = 0x8e00;         // 软通道32 TCG曲线寄存器地址

// 多功能卡地址
const unsigned int PULSE_REPETITION_FREQUENCY_ADDRESS = 0x4;            // PRF 多功能卡地址
const unsigned int DODER_SELECT_MARK = 0x14;                            // 编码器切换标志 0：C3， 1：C2
const unsigned int TEST_START_FLAG = 0x11c;                             // 探伤开始标志
const unsigned int MPB_SIMULATE_ENABL_ADDR = 0xb0;                      // Lsmp2电气模拟使能
const unsigned int MPB_SIMULATE_DATA_ADDR = 0xb2;                       // 电气模拟数据起始地址 0xb2-0xb8

#endif
