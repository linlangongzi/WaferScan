#include "StdAfx.h"
#include <pkfuncs.h>
#include <ceddk.h>
#include "ars200fpga.h"

extern BYTE *base;
#define  BASE_ADDR 0x18000000 //基地址 


//#define  ARS200FPGA_ALL_CONTROL_REG_ADDR 0x18000002  //所有继电器控制寄存器,初始化 写0x6c
//#define  ARS200FPGA_WORK_MODE_REG_ADDR 0x18000022   //工作模式  初始化 写3


#define  ARS200FPGA_CH1_OPA3692_SIMULATE_SWITCH_ADDR 0x18040006 //通道 1 OPA3692模拟开关三选一,初始化 写4
#define  ARS200FPGA_CH1_LMH6720_SD_REG_ADDR 0x18040008    //通道 1 LMH6720_SD使能 初始化 写1
#define  ARS200FPGA_CH1_ASSOCIATE_DETECTION_REG_ADDR 0x1804000A  //通道 1  关联检测使能,初始化 关闭
#define  ARS200FPGA_CH1_WAVE_FILTER_REG_ADDR 0x1804000C   //通道 1 滤波器使能,初始化  打开
#define  ARS200FPGA_CH1_WAVE_FILTER_RESET_REG_ADDR 0x1804000E //通道 1 滤波器复位 写1
#define  ARS200FPGA_CH1_WAVE_FILTER_CONTROL_REG_ADDR 0x18040010 //通道 1 滤波器复位 写5

#define  ARS200FPGA_CH2_OPA3692_SIMULATE_SWITCH_ADDR 0x18080006 //通道 2 OPA3692模拟开关三选一,初始化 写4
#define  ARS200FPGA_CH2_LMH6720_SD_REG_ADDR 0x18080008    //通道 2 LMH6720_SD使能 初始化 写1
#define  ARS200FPGA_CH2_ASSOCIATE_DETECTION_REG_ADDR 0x1808000A  //通道 2  关联检测使能,初始化 关闭
#define  ARS200FPGA_CH2_WAVE_FILTER_REG_ADDR 0x1808000C   //通道 2 滤波器使能,初始化  打开
#define  ARS200FPGA_CH2_WAVE_FILTER_RESET_REG_ADDR 0x1808000E //通道 2 滤波器复位 写1
#define  ARS200FPGA_CH2_WAVE_FILTER_CONTROL_REG_ADDR 0x18080010 //通道 2 滤波器复位 写5



#define  ARS200FPGA_SYS_REG_ADDR   0x18000000  //系统软复位

//#define  ARS200FPGA_SYS_REG_ADDR   0x18000800 //暂时把软复位的地址改为无用的地址，避免造成下发参数造成影响  yachang.huang modify on 2013-11-18


#define  ARS200FPGA_CH1_CON_REG_ADDR 0x18040000 //通道1软复位

#define  ARS200FPGA_CH1_PLUS_REG_ADDR  0x18060004 //通道 1 增益转换开关
#define  ARS200FPGA_CH1_DAC_REG_ADDR  0x18060012 //通道 1DAC使能开关

#define  ARS200FPGA_CH1_GATE_REG_ADDR  0x18060014 //通道1闸门1 使能
#define  ARS200FPGA_CH1_DATE_REG_ADDR  0x18060082 //通道 1 监视通道复位
#define  ARS200FPGA_CH1_INTERVAL_ADDR  0x18060800 //通道 1 监视通道采样间隔SRAM
#define  ARS200FPGA_CH1_DATA_ADDR	0x18060C00 //通道1监视通道采样数据SRAM
#define  ARS200FPGA_CH1_TGC_ADDR	0x18061000 //通道1TGC SRAM
#define  ARS200FPGA_CH1_GATE1CURVE_ADDR   0x18014000  //通道 1 闸门1 曲线SRAM(新版未找到)
#define  ARS200FPGA_CH1_GATE2CURVE_ADDR   0x18016000  //通道 1 闸门2 曲线SRAM(新版未找到)
#define  ARS200FPGA_CH1_GATE3CURVE_ADDR   0x18018000  //通道 1 闸门3 曲线SRAM(新版未找到)

#define  ARS200FPGA_CH2_CON_REG_ADDR 0x18080000  //通道 2 软复位

#define  ARS200FPGA_CH2_PLUS_REG_ADDR  0x180A0004 //通道2增益转换开关
#define  ARS200FPGA_CH2_DAC_REG_ADDR   0x180A0012 //通道2DAC使能开关

#define  ARS200FPGA_CH2_GATE_REG_ADDR  0x180A0014 //通道2闸门1使能
#define  ARS200FPGA_CH2_DATE_REG_ADDR  0x180A0082 //通道2监视通道复位
#define  ARS200FPGA_CH2_INTERVAL_ADDR  0x180A0800 //通道2监视通道采样间隔SRAM
#define  ARS200FPGA_CH2_DATA_ADDR	0x180A0C00 //通道2监视通道采样数据SRAM
#define  ARS200FPGA_CH2_TGC_ADDR	0x180A1000 //通道2TGC SRAM
#define  ARS200FPGA_CH2_GATE1CURVE_ADDR   0x18024000 //通道 2 闸门1 曲线SRAM(新版未找到)
#define  ARS200FPGA_CH2_GATE2CURVE_ADDR   0x18026000 //通道 2 闸门2 曲线SRAM(新版未找到)
#define  ARS200FPGA_CH2_GATE3CURVE_ADDR   0x18028000 //通道 2 闸门3 曲线SRAM(新版未找到)


#define ARS200HARDWARE_VER_ADDR		0x18000024 //硬件版本信息
#define ARS200FUNCTION_DEFINE_ADDR	0x18000026 //软件功能定义1



#define ARS200FPGA_DEVICE_TYPE_ADDR		0x1800002A //型号识别关键字
#define ARM_SELFTEST 0x18000400


#define  LAMP_ADDR   0x18000050  //灯


WORD *lamp;






WORD*  ch1_opa3692_simulate_switch;
WORD*  ch1_lmh6720_sd;
WORD*  ch1_associate_detection;
WORD*  ch1_wave_filter;
WORD*  ch1_wave_filter_reset;
WORD*  ch1_wave_filter_control;

WORD*  ch2_opa3692_simulate_switch;
WORD*  ch2_lmh6720_sd;
WORD*  ch2_associate_detection;
WORD*  ch2_wave_filter;
WORD*  ch2_wave_filter_reset;
WORD*  ch2_wave_filter_control;





ARS200FPGA_SYS_REG* sys_reg;
ARS200FPGA_CH_CON_REG *ch1_con_reg;
ARS200FPGA_CH_CON_REG *ch2_con_reg;
// songchenguang add start 2010-05-26
ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;
ARS200FPGA_CH_PLUS_REG* ch2_plus_reg;
ARS200FPGA_CH_DAC_REG* ch1_dac_reg;
ARS200FPGA_CH_DAC_REG* ch2_dac_reg;
// songchenguang add end 2010-05-26
ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
ARS200FPGA_CH_DATA_REG *ch1_data_reg;
ARS200FPGA_CH_DATA_REG *ch2_data_reg;
WORD *ch1_interval;
WORD *ch2_interval;
WORD *ch1_data;
WORD *ch2_data;
WORD *ch1_tgc;
WORD *ch2_tgc;
WORD *ch1_gate1curve;
WORD *ch2_gate1curve;
WORD *ch1_gate2curve;
WORD *ch2_gate2curve;
WORD *ch1_gate3curve;
WORD *ch2_gate3curve;
WORD *arm_selftest;
// songchenguang add start 2010-10-13
ARS200VER* Sys_Hardware_Ver = NULL;
ARS200FUN_DEFINE* Sys_Function_Def = NULL;
// songchenguang add end 2010-10-13
// wangbingfu add start 2010-11-11
WORD* Sys_device_type = NULL;
// wangbingfu add end 2010-11-11


BOOL ars200fpga_alloc_io()
{

	base = (BYTE *)GetVirtual(BASE_ADDR, 0x400000);
	

	
    sys_reg = (ARS200FPGA_SYS_REG*)(base + (ARS200FPGA_SYS_REG_ADDR - BASE_ADDR));
    ch1_con_reg = (ARS200FPGA_CH_CON_REG*)(base + (ARS200FPGA_CH1_CON_REG_ADDR - BASE_ADDR));
    ch2_con_reg = (ARS200FPGA_CH_CON_REG*)(base + (ARS200FPGA_CH2_CON_REG_ADDR - BASE_ADDR));

	ch1_plus_reg = (ARS200FPGA_CH_PLUS_REG*)(base + (ARS200FPGA_CH1_PLUS_REG_ADDR - BASE_ADDR));
	ch2_plus_reg = (ARS200FPGA_CH_PLUS_REG*)(base + (ARS200FPGA_CH2_PLUS_REG_ADDR - BASE_ADDR));
	ch1_dac_reg = (ARS200FPGA_CH_DAC_REG*)(base + (ARS200FPGA_CH1_DAC_REG_ADDR - BASE_ADDR));
	ch2_dac_reg = (ARS200FPGA_CH_DAC_REG*)(base + (ARS200FPGA_CH2_DAC_REG_ADDR - BASE_ADDR));
    Sys_Hardware_Ver = (ARS200VER*)(base + (ARS200HARDWARE_VER_ADDR - BASE_ADDR));
    Sys_Function_Def = (ARS200FUN_DEFINE*)(base + (ARS200HARDWARE_VER_ADDR - BASE_ADDR));

	Sys_device_type = (WORD*)(base + (ARS200FPGA_DEVICE_TYPE_ADDR - BASE_ADDR));

	ch1_gate_reg=(ARS200FPGA_CH_GATE_REG *)(base + (ARS200FPGA_CH1_GATE_REG_ADDR - BASE_ADDR));
	ch2_gate_reg=(ARS200FPGA_CH_GATE_REG *)(base + (ARS200FPGA_CH2_GATE_REG_ADDR - BASE_ADDR));


    //yachang.huang 2013-10-25
	ch1_data_reg = (ARS200FPGA_CH_DATA_REG *)(base + (ARS200FPGA_CH1_DATA_ADDR - BASE_ADDR));
	ch2_data_reg = (ARS200FPGA_CH_DATA_REG *)(base + (ARS200FPGA_CH2_DATA_ADDR - BASE_ADDR));

	ch1_interval=(WORD *)(base + (ARS200FPGA_CH1_INTERVAL_ADDR - BASE_ADDR));
	ch2_interval=(WORD *)(base + (ARS200FPGA_CH2_INTERVAL_ADDR - BASE_ADDR));
	ch1_data=(WORD *)(base + (ARS200FPGA_CH1_DATA_ADDR - BASE_ADDR));
	ch2_data=(WORD *)(base + (ARS200FPGA_CH2_DATA_ADDR - BASE_ADDR));
	ch1_tgc=(WORD *)(base + (ARS200FPGA_CH1_TGC_ADDR - BASE_ADDR));
	ch2_tgc=(WORD *)(base + (ARS200FPGA_CH2_TGC_ADDR - BASE_ADDR));
	ch1_gate1curve=(WORD *)(base + (ARS200FPGA_CH1_GATE1CURVE_ADDR - BASE_ADDR));
	ch2_gate1curve=(WORD *)(base + (ARS200FPGA_CH2_GATE1CURVE_ADDR - BASE_ADDR));
	ch1_gate2curve=(WORD *)(base + (ARS200FPGA_CH1_GATE2CURVE_ADDR - BASE_ADDR));
	ch2_gate2curve=(WORD *)(base + (ARS200FPGA_CH2_GATE2CURVE_ADDR - BASE_ADDR));
	ch1_gate3curve=(WORD *)(base + (ARS200FPGA_CH1_GATE3CURVE_ADDR - BASE_ADDR));
	ch2_gate3curve=(WORD *)(base + (ARS200FPGA_CH2_GATE3CURVE_ADDR - BASE_ADDR));

    
	arm_selftest=(WORD *)(base + (ARM_SELFTEST - BASE_ADDR));
	Sys_Hardware_Ver = (ARS200VER*)(base + (ARS200HARDWARE_VER_ADDR - BASE_ADDR));
	lamp = (WORD *)(base + (LAMP_ADDR - BASE_ADDR));


	ch1_opa3692_simulate_switch = (WORD *)(base + (ARS200FPGA_CH1_OPA3692_SIMULATE_SWITCH_ADDR - BASE_ADDR));
	ch1_lmh6720_sd = (WORD *)(base + (ARS200FPGA_CH1_LMH6720_SD_REG_ADDR - BASE_ADDR));
	ch1_associate_detection = (WORD *)(base + (ARS200FPGA_CH1_ASSOCIATE_DETECTION_REG_ADDR - BASE_ADDR));
	ch1_wave_filter = (WORD *)(base + (ARS200FPGA_CH1_WAVE_FILTER_REG_ADDR - BASE_ADDR));
	ch1_wave_filter_reset = (WORD *)(base + (ARS200FPGA_CH1_WAVE_FILTER_RESET_REG_ADDR - BASE_ADDR));
	ch1_wave_filter_control = (WORD *)(base + (ARS200FPGA_CH1_WAVE_FILTER_CONTROL_REG_ADDR - BASE_ADDR));

	ch2_opa3692_simulate_switch = (WORD *)(base + (ARS200FPGA_CH2_OPA3692_SIMULATE_SWITCH_ADDR - BASE_ADDR));
	ch2_lmh6720_sd = (WORD *)(base + (ARS200FPGA_CH2_LMH6720_SD_REG_ADDR - BASE_ADDR));
	ch2_associate_detection = (WORD *)(base + (ARS200FPGA_CH2_ASSOCIATE_DETECTION_REG_ADDR - BASE_ADDR));
	ch2_wave_filter = (WORD *)(base + (ARS200FPGA_CH2_WAVE_FILTER_REG_ADDR - BASE_ADDR));
	ch2_wave_filter_reset = (WORD *)(base + (ARS200FPGA_CH2_WAVE_FILTER_RESET_REG_ADDR - BASE_ADDR));
	ch2_wave_filter_control = (WORD *)(base + (ARS200FPGA_CH2_WAVE_FILTER_CONTROL_REG_ADDR - BASE_ADDR));



	return TRUE;
}

BOOL ars200fpga_unalloc_io()
{
	return TRUE;
}

void send_max500(WORD cword)
{
	while (sys_reg->wSPIReady)
	{
//		__asm{nop};
	}
	sys_reg->wSPIChannel=1;
	sys_reg->wSPIStart=1;
//	__asm{nop};
	sys_reg->wSPIStart=0;
}


void send_mcp41010(WORD cword)
{
	while (sys_reg->wSPIReady)
	{
//		__asm{nop};
	}
	sys_reg->wSPIChannel=2;
//	sys_reg->wSPICh2Reg=cword;
	sys_reg->wSPIStart=1;
//	__asm{nop};
	sys_reg->wSPIStart=0;
}

typedef struct {
    void*    pvDestMem;
    DWORD    dwPhysAddr;
    DWORD    dwSize;
} VIRTUAL_COPY_EX_DATA;

#define IOCTL_VIRTUAL_COPY_EX CTL_CODE (FILE_DEVICE_UNKNOWN,3333,METHOD_BUFFERED,FILE_ANY_ACCESS)

volatile LPVOID GetVirtual(DWORD dwPhyBaseAddress, DWORD dwSize)
{
	volatile LPVOID pVirtual;
	VIRTUAL_COPY_EX_DATA vced;

	// 	if(dwPhyBaseAddress&0xFFF)
	// 	{
	// 		return NULL;
	// 	}
	vced.dwPhysAddr = dwPhyBaseAddress>>8;
	HANDLE hSrc = (HANDLE)GetCurrentProcessId();
	pVirtual = VirtualAllocEx(hSrc,0,dwSize,MEM_RESERVE,PAGE_NOACCESS);
	vced.pvDestMem = pVirtual;
	vced.dwSize = dwSize;
	KernelIoControl(IOCTL_VIRTUAL_COPY_EX,&vced, sizeof(vced), NULL, NULL, NULL);
	return pVirtual;
}

void Process_Reg_Init()
{
	sys_reg->wRelay = 0x6c;
	sys_reg->wWorkMode = 3;
	sys_reg->wSPIReset = 1;


	*ch1_opa3692_simulate_switch = 4;
	*ch1_lmh6720_sd = 1;
	*ch1_associate_detection = 0;
	*ch1_wave_filter = 1;
	*ch1_wave_filter_reset = 1;
	*ch1_wave_filter_control = 5;

	*ch2_opa3692_simulate_switch = 4;
	*ch2_lmh6720_sd = 1;
	*ch2_associate_detection = 0;
	*ch2_wave_filter = 1;
	*ch2_wave_filter_reset = 1;
	*ch2_wave_filter_control = 5;
}