#include "StdAfx.h"
#include <pkfuncs.h>
#include <ceddk.h>
#include "ars200fpga.h"

BYTE *base;

#define  BASE_ADDR 0x18000000 //基地址 

#define  CSCAN_ARS200FPGA_SYS_REG_ADDR   0x18000000   //系统软复位
//#define  CSCAN_ARS200FPGA_SYS_REG_ADDR  0x18000800 //暂时把软复位的地址改为无用的地址，避免造成下发参数造成影响  yachang.huang modify on 2013-11-18


#define  CSCAN_ARS200FPGA_CODE_REG_ADDR   0x1800002C  //编码器使能

#define  CSCAN_ARS200FPGA_CH1_CON_REG_ADDR 0x18040000  //通道1软复位
#define  CSCAN_ARS200FPGA_CH1_CONFIG_REG_ADDR 0x18060000  //通道1检波模式
#define  CSCAN_ARS200FPGA_CH1_GATE_REG_ADDR  0x18060014   //通道1闸门1使能
#define  CSCAN_ARS200FPGA_CH1_FEATURE_REG_ADDR  0x1806004A  //通道1闸门1同步时间
#define  CSCAN_ARS200FPGA_CH1_DATE_REG_ADDR  0x18060082  //通道1监视数据复位
#define  CSCAN_ARS200FPGA_CH1_INTERVAL_ADDR  0x18060800  //通道1监视通道采样间隔SRAM
#define  CSCAN_ARS200FPGA_CH1_DATA_ADDR	0x18060C00  //通道1监视通道采样数据SRAM
#define  CSCAN_ARS200FPGA_CH1_TGC_ADDR	0x18061000 //通道1TGC SRAM
#define  CSCAN_ARS200FPGA_CH1_GATE1CURVE_ADDR   0x18014000  //通道 1 闸门1 曲线SRAM(新版未找到)
#define  CSCAN_ARS200FPGA_CH1_GATE2CURVE_ADDR   0x18016000  //通道 1 闸门2 曲线SRAM(新版未找到)
#define  CSCAN_ARS200FPGA_CH1_GATE3CURVE_ADDR   0x18018000  //通道 1 闸门2 曲线SRAM(新版未找到)
#define  CSCAN_ARS200FPGA_CH1_DAC_REG_ADDR  0x18060012  //通道1DAC使能开关
#define  CSCAN_ARS200FPGA_CH1_SAMPLING_INTERVAL_TGC_ADDR	0x18060090 //通道1TCG采样间隔


#define  CSCAN_ARS200FPGA_CH2_CON_REG_ADDR 0x18080000  //通道2软复位
#define  CSCAN_ARS200FPGA_CH2_CONFIG_REG_ADDR 0x180A0000 //通道2检波模式
#define  CSCAN_ARS200FPGA_CH2_GATE_REG_ADDR  0x180A0014  //通道2闸门1使能
#define  CSCAN_ARS200FPGA_CH2_FEATURE_REG_ADDR  0x180A004A  //通道2闸门1同步时间
#define  CSCAN_ARS200FPGA_CH2_DATE_REG_ADDR  0x180A0082  //通道2监视数据复位
#define  CSCAN_ARS200FPGA_CH2_INTERVAL_ADDR  0x180A0800  //通道2监视通道采样间隔SRAM
#define  CSCAN_ARS200FPGA_CH2_DATA_ADDR	0x180A0C00  //通道2监视通道采样数据SRAM
#define  CSCAN_ARS200FPGA_CH2_TGC_ADDR	0x180A1000  //通道2TGC SRAM
#define  CSCAN_ARS200FPGA_CH2_DAC_REG_ADDR  0x180A0012  //通道2DAC使能开关
#define  CSCAN_ARS200FPGA_CH2_SAMPLING_INTERVAL_TGC_ADDR	0x180A0090 //通道2TCG采样间隔

#define CSCAN_ARS200HARDWARE_VER_ADDR		0x18000024  //硬件版本信息
#define CSCAN_ARS200FUNCTION_DEFINE_ADDR	0x18000026  //软件功能定义1

#define CSCAN_ARS200FPGA_DEVICE_TYPE_ADDR		0x1800002A  //型号识别关键字

#define  CSCAN_ARM_SELFTEST_ADDR  0x18000400 //ARM自检RAM

#define  CSCAN_CH1_FEATURE_BUFFER_FLAG_ADDR  0x18068000	//通道1特征数据缓冲区标志位
#define  CSCAN_CH2_FEATURE_BUFFER_FLAG_ADDR  0x180A8000	//通道2特征数据缓冲区标志位

#define  CSCAN_CH1_FEATURE_BUFFER1_ADDR  0x1806A000	//通道1特征数据缓冲区1
#define  CSCAN_CH1_FEATURE_BUFFER2_ADDR  0x1806C000	//通道1特征数据缓冲区2
#define  CSCAN_CH2_FEATURE_BUFFER1_ADDR  0x180AA000	//通道2特征数据缓冲区1
#define  CSCAN_CH2_FEATURE_BUFFER2_ADDR  0x180AC000	//通道2特征数据缓冲区2

#define  CSCAN_CH1_MONITOR_FLAG_ADDR  0x18060088	//通道1监视数据有效标志位
#define  CSCAN_CH2_MONITOR_FLAG_ADDR  0x180A0088	//通道2监视数据有效标志位

#define  CSCAN_CH1_ASCAN_DATA_STORE_FLAG_ADDR 0x1806008C //通道1 A扫波形存储缓冲区可读标志
#define  CSCAN_CH2_ASCAN_DATA_STORE_FLAG_ADDR 0x180A008C //通道2 A扫波形存储缓冲区可读标志

#define  CSCAN_CH1_ASCAN_DATA_STORE_RESET_ADDR 0x1806008E //通道1 A扫波形存储缓冲区可读标志复位
#define  CSCAN_CH2_ASCAN_DATA_STORE_RESET_ADDR 0x180A008E //通道2 A扫波形存储缓冲区可读标志复位

#define  CSCAN_CH1_ASCAN_DATA_STORE_BUFFER_FLAG_ADDR 0x18370000 //通道1 A扫波形存储缓冲区标志位
#define  CSCAN_CH2_ASCAN_DATA_STORE_BUFFER_FLAG_ADDR 0x183B0000 //通道1 A扫波形存储缓冲区标志位

#define  CSCAN_CH1_ASCAN_DATA_STORE_BUFFER1_ADDR  0x18170000	//通道1A扫波形存储缓冲区1
#define  CSCAN_CH1_ASCAN_DATA_STORE_BUFFER2_ADDR  0x18270000	//通道1A扫波形存储缓冲区2
#define  CSCAN_CH2_ASCAN_DATA_STORE_BUFFER1_ADDR  0x181B0000	//通道2A扫波形存储缓冲区1
#define  CSCAN_CH2_ASCAN_DATA_STORE_BUFFER2_ADDR  0x182B0000	//通道2A扫波形存储缓冲区2


ARS200FPGA_SYS_REG* cscan_sys_reg;
CSCAN_ARS200FPGA_CODE_REG* cscan_code_reg;

CSCAN_ARS200FPGA_CH_CON_REG *cscan_ch1_con_reg;
CSCAN_ARS200FPGA_CH_CON_REG *cscan_ch2_con_reg;

CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch1_config_reg;
CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch2_config_reg;

CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch1_gate_reg;
CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch2_gate_reg;

CSCAN_ARS200FPGA_CH_FEATURE_REG *cscan_ch1_feature_reg;
CSCAN_ARS200FPGA_CH_FEATURE_REG *cscan_ch2_feature_reg;

CSCAN_ARS200FPGA_CH_DATA_REG *cscan_ch1_data_reg;
CSCAN_ARS200FPGA_CH_DATA_REG *cscan_ch2_data_reg;
WORD *cscan_ch1_interval;
WORD *cscan_ch2_interval;
WORD *cscan_ch1_data;
WORD *cscan_ch2_data;
WORD *cscan_ch1_tgc;
WORD *cscan_ch2_tgc;
WORD *cscan_ch1_dac_reg;
WORD *cscan_ch2_dac_reg;

WORD *cscan_ch1_sampling_interval_tgc;
WORD *cscan_ch2_sampling_interval_tgc;


WORD *cscan_arm_selftest;
WORD *cscan_ch1_feature_buffer_flag;
WORD *cscan_ch2_feature_buffer_flag;
WORD *cscan_ch1_feature_buffer1;
WORD *cscan_ch1_feature_buffer2;
WORD *cscan_ch2_feature_buffer1;
WORD *cscan_ch2_feature_buffer2;
// songchenguang add start 2010-10-13
ARS200VER* cscan_Sys_Hardware_Ver = NULL;
ARS200FUN_DEFINE* cscan_Sys_Function_Def = NULL;
// songchenguang add end 2010-10-13
// wangbingfu add start 2010-11-11
WORD* cscan_Sys_device_type = NULL;
// wangbingfu add end 2010-11-11

WORD* cscan_ch1_monitor_flag;
WORD* cscan_ch2_monitor_flag;


WORD *cscan_ch1_ascan_data_store_monitor_flag;
WORD *cscan_ch2_ascan_data_store_monitor_flag;

WORD *cscan_ch1_ascan_data_store_reset;
WORD *cscan_ch2_ascan_data_store_reset;

WORD *cscan_ch1_ascan_data_store_buffer_flag;
WORD *cscan_ch2_ascan_data_store_buffer_flag;
WORD *cscan_ch1_ascan_data_store_buffer1;
WORD *cscan_ch1_ascan_data_store_buffer2;
WORD *cscan_ch2_ascan_data_store_buffer1;
WORD *cscan_ch2_ascan_data_store_buffer2;

BOOL cscan_ars200fpga_alloc_io()
{
    
	cscan_sys_reg = (ARS200FPGA_SYS_REG *)(base + (CSCAN_ARS200FPGA_SYS_REG_ADDR - BASE_ADDR));
	cscan_code_reg = (CSCAN_ARS200FPGA_CODE_REG *)(base + (CSCAN_ARS200FPGA_CODE_REG_ADDR - BASE_ADDR));
	cscan_ch1_con_reg=(CSCAN_ARS200FPGA_CH_CON_REG *)(base + (CSCAN_ARS200FPGA_CH1_CON_REG_ADDR - BASE_ADDR));
	cscan_ch2_con_reg=(CSCAN_ARS200FPGA_CH_CON_REG *)(base + (CSCAN_ARS200FPGA_CH2_CON_REG_ADDR - BASE_ADDR));
	cscan_ch1_config_reg =(CSCAN_ARS200FPGA_CH_CONFIG_REG *)(base + (CSCAN_ARS200FPGA_CH1_CONFIG_REG_ADDR - BASE_ADDR));
	cscan_ch2_config_reg =(CSCAN_ARS200FPGA_CH_CONFIG_REG *)(base + (CSCAN_ARS200FPGA_CH2_CONFIG_REG_ADDR - BASE_ADDR));

	cscan_Sys_Hardware_Ver = (ARS200VER*)(base + (CSCAN_ARS200HARDWARE_VER_ADDR - BASE_ADDR));
	cscan_Sys_Function_Def = (ARS200FUN_DEFINE*)(base + (CSCAN_ARS200FUNCTION_DEFINE_ADDR - BASE_ADDR));

	cscan_Sys_device_type = (WORD*)(base + (CSCAN_ARS200FPGA_DEVICE_TYPE_ADDR - BASE_ADDR));

	cscan_ch1_gate_reg=(CSCAN_ARS200FPGA_CH_GATE_REG *)(base + (CSCAN_ARS200FPGA_CH1_GATE_REG_ADDR - BASE_ADDR));
	cscan_ch2_gate_reg=(CSCAN_ARS200FPGA_CH_GATE_REG *)(base + (CSCAN_ARS200FPGA_CH2_GATE_REG_ADDR - BASE_ADDR));

	cscan_ch1_feature_reg = (CSCAN_ARS200FPGA_CH_FEATURE_REG *)(base + (CSCAN_ARS200FPGA_CH1_FEATURE_REG_ADDR - BASE_ADDR));
	cscan_ch2_feature_reg = (CSCAN_ARS200FPGA_CH_FEATURE_REG *)(base + (CSCAN_ARS200FPGA_CH2_FEATURE_REG_ADDR - BASE_ADDR));

	
	cscan_ch1_interval=(WORD *)(base + (CSCAN_ARS200FPGA_CH1_INTERVAL_ADDR - BASE_ADDR));
	cscan_ch2_interval=(WORD *)(base + (CSCAN_ARS200FPGA_CH2_INTERVAL_ADDR - BASE_ADDR));

	cscan_ch1_tgc=(WORD *)(base + (CSCAN_ARS200FPGA_CH1_TGC_ADDR - BASE_ADDR));
	cscan_ch2_tgc=(WORD *)(base + (CSCAN_ARS200FPGA_CH2_TGC_ADDR - BASE_ADDR));
	cscan_ch1_dac_reg = (WORD*)(base + (CSCAN_ARS200FPGA_CH1_DAC_REG_ADDR - BASE_ADDR));
	cscan_ch2_dac_reg = (WORD*)(base + (CSCAN_ARS200FPGA_CH2_DAC_REG_ADDR - BASE_ADDR));

	cscan_ch1_sampling_interval_tgc = (WORD *)(base + (CSCAN_ARS200FPGA_CH1_SAMPLING_INTERVAL_TGC_ADDR - BASE_ADDR));
	cscan_ch2_sampling_interval_tgc = (WORD *)(base + (CSCAN_ARS200FPGA_CH2_SAMPLING_INTERVAL_TGC_ADDR - BASE_ADDR));

	cscan_arm_selftest=(WORD *)(base + (CSCAN_ARM_SELFTEST_ADDR - BASE_ADDR));
	cscan_ch1_feature_buffer_flag = (WORD*)(base + (CSCAN_CH1_FEATURE_BUFFER_FLAG_ADDR - BASE_ADDR)); 
	cscan_ch2_feature_buffer_flag = (WORD*)(base + (CSCAN_CH2_FEATURE_BUFFER_FLAG_ADDR - BASE_ADDR));
	cscan_ch1_feature_buffer1 = (WORD *)(base + (CSCAN_CH1_FEATURE_BUFFER1_ADDR - BASE_ADDR));
	cscan_ch1_feature_buffer2 = (WORD *)(base + (CSCAN_CH1_FEATURE_BUFFER2_ADDR - BASE_ADDR));
	cscan_ch2_feature_buffer1 = (WORD *)(base + (CSCAN_CH2_FEATURE_BUFFER1_ADDR - BASE_ADDR));
	cscan_ch2_feature_buffer2 = (WORD *)(base + (CSCAN_CH2_FEATURE_BUFFER2_ADDR - BASE_ADDR));

	cscan_ch1_monitor_flag = (WORD*)(base + (CSCAN_CH1_MONITOR_FLAG_ADDR - BASE_ADDR));
	cscan_ch2_monitor_flag = (WORD*)(base + (CSCAN_CH2_MONITOR_FLAG_ADDR - BASE_ADDR));
    
    
	cscan_ch1_data_reg = (CSCAN_ARS200FPGA_CH_DATA_REG *)(base + (CSCAN_ARS200FPGA_CH1_DATE_REG_ADDR - BASE_ADDR));
	cscan_ch2_data_reg = (CSCAN_ARS200FPGA_CH_DATA_REG *)(base + (CSCAN_ARS200FPGA_CH2_DATE_REG_ADDR - BASE_ADDR));
	cscan_ch1_data =(WORD*)(base + (CSCAN_ARS200FPGA_CH1_DATA_ADDR - BASE_ADDR));
	cscan_ch2_data = (WORD*)(base + (CSCAN_ARS200FPGA_CH2_DATA_ADDR - BASE_ADDR));

	cscan_ch1_ascan_data_store_monitor_flag = (WORD *)(base + (CSCAN_CH1_ASCAN_DATA_STORE_FLAG_ADDR - BASE_ADDR));
	cscan_ch2_ascan_data_store_monitor_flag = (WORD *)(base + (CSCAN_CH2_ASCAN_DATA_STORE_FLAG_ADDR - BASE_ADDR));
	cscan_ch1_ascan_data_store_reset = (WORD *)(base + (CSCAN_CH1_ASCAN_DATA_STORE_RESET_ADDR - BASE_ADDR));
	cscan_ch2_ascan_data_store_reset = (WORD *)(base + (CSCAN_CH2_ASCAN_DATA_STORE_RESET_ADDR - BASE_ADDR));
	cscan_ch1_ascan_data_store_buffer_flag = (WORD *)(base + (CSCAN_CH1_ASCAN_DATA_STORE_BUFFER_FLAG_ADDR - BASE_ADDR));
	cscan_ch2_ascan_data_store_buffer_flag = (WORD *)(base + (CSCAN_CH2_ASCAN_DATA_STORE_BUFFER_FLAG_ADDR - BASE_ADDR));
	cscan_ch1_ascan_data_store_buffer1 = (WORD *)(base + (CSCAN_CH1_ASCAN_DATA_STORE_BUFFER1_ADDR- BASE_ADDR));
	cscan_ch1_ascan_data_store_buffer2 = (WORD *)(base + (CSCAN_CH1_ASCAN_DATA_STORE_BUFFER2_ADDR - BASE_ADDR));
	cscan_ch2_ascan_data_store_buffer1 = (WORD *)(base + (CSCAN_CH2_ASCAN_DATA_STORE_BUFFER1_ADDR - BASE_ADDR));
	cscan_ch2_ascan_data_store_buffer2 = (WORD *)(base + (CSCAN_CH2_ASCAN_DATA_STORE_BUFFER2_ADDR - BASE_ADDR));

	return TRUE;
}

BOOL cscan_ars200fpga_unalloc_io()
{

	return TRUE;
}

