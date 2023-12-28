#include "StdAfx.h"
#include <pkfuncs.h>
#include <ceddk.h>
#include "ars200fpga.h"

extern BYTE *base;
#define  BASE_ADDR 0x18000000 //����ַ 


//#define  ARS200FPGA_ALL_CONTROL_REG_ADDR 0x18000002  //���м̵������ƼĴ���,��ʼ�� д0x6c
//#define  ARS200FPGA_WORK_MODE_REG_ADDR 0x18000022   //����ģʽ  ��ʼ�� д3


#define  ARS200FPGA_CH1_OPA3692_SIMULATE_SWITCH_ADDR 0x18040006 //ͨ�� 1 OPA3692ģ�⿪����ѡһ,��ʼ�� д4
#define  ARS200FPGA_CH1_LMH6720_SD_REG_ADDR 0x18040008    //ͨ�� 1 LMH6720_SDʹ�� ��ʼ�� д1
#define  ARS200FPGA_CH1_ASSOCIATE_DETECTION_REG_ADDR 0x1804000A  //ͨ�� 1  �������ʹ��,��ʼ�� �ر�
#define  ARS200FPGA_CH1_WAVE_FILTER_REG_ADDR 0x1804000C   //ͨ�� 1 �˲���ʹ��,��ʼ��  ��
#define  ARS200FPGA_CH1_WAVE_FILTER_RESET_REG_ADDR 0x1804000E //ͨ�� 1 �˲�����λ д1
#define  ARS200FPGA_CH1_WAVE_FILTER_CONTROL_REG_ADDR 0x18040010 //ͨ�� 1 �˲�����λ д5

#define  ARS200FPGA_CH2_OPA3692_SIMULATE_SWITCH_ADDR 0x18080006 //ͨ�� 2 OPA3692ģ�⿪����ѡһ,��ʼ�� д4
#define  ARS200FPGA_CH2_LMH6720_SD_REG_ADDR 0x18080008    //ͨ�� 2 LMH6720_SDʹ�� ��ʼ�� д1
#define  ARS200FPGA_CH2_ASSOCIATE_DETECTION_REG_ADDR 0x1808000A  //ͨ�� 2  �������ʹ��,��ʼ�� �ر�
#define  ARS200FPGA_CH2_WAVE_FILTER_REG_ADDR 0x1808000C   //ͨ�� 2 �˲���ʹ��,��ʼ��  ��
#define  ARS200FPGA_CH2_WAVE_FILTER_RESET_REG_ADDR 0x1808000E //ͨ�� 2 �˲�����λ д1
#define  ARS200FPGA_CH2_WAVE_FILTER_CONTROL_REG_ADDR 0x18080010 //ͨ�� 2 �˲�����λ д5



#define  ARS200FPGA_SYS_REG_ADDR   0x18000000  //ϵͳ��λ

//#define  ARS200FPGA_SYS_REG_ADDR   0x18000800 //��ʱ����λ�ĵ�ַ��Ϊ���õĵ�ַ����������·��������Ӱ��  yachang.huang modify on 2013-11-18


#define  ARS200FPGA_CH1_CON_REG_ADDR 0x18040000 //ͨ��1��λ

#define  ARS200FPGA_CH1_PLUS_REG_ADDR  0x18060004 //ͨ�� 1 ����ת������
#define  ARS200FPGA_CH1_DAC_REG_ADDR  0x18060012 //ͨ�� 1DACʹ�ܿ���

#define  ARS200FPGA_CH1_GATE_REG_ADDR  0x18060014 //ͨ��1բ��1 ʹ��
#define  ARS200FPGA_CH1_DATE_REG_ADDR  0x18060082 //ͨ�� 1 ����ͨ����λ
#define  ARS200FPGA_CH1_INTERVAL_ADDR  0x18060800 //ͨ�� 1 ����ͨ���������SRAM
#define  ARS200FPGA_CH1_DATA_ADDR	0x18060C00 //ͨ��1����ͨ����������SRAM
#define  ARS200FPGA_CH1_TGC_ADDR	0x18061000 //ͨ��1TGC SRAM
#define  ARS200FPGA_CH1_GATE1CURVE_ADDR   0x18014000  //ͨ�� 1 բ��1 ����SRAM(�°�δ�ҵ�)
#define  ARS200FPGA_CH1_GATE2CURVE_ADDR   0x18016000  //ͨ�� 1 բ��2 ����SRAM(�°�δ�ҵ�)
#define  ARS200FPGA_CH1_GATE3CURVE_ADDR   0x18018000  //ͨ�� 1 բ��3 ����SRAM(�°�δ�ҵ�)

#define  ARS200FPGA_CH2_CON_REG_ADDR 0x18080000  //ͨ�� 2 ��λ

#define  ARS200FPGA_CH2_PLUS_REG_ADDR  0x180A0004 //ͨ��2����ת������
#define  ARS200FPGA_CH2_DAC_REG_ADDR   0x180A0012 //ͨ��2DACʹ�ܿ���

#define  ARS200FPGA_CH2_GATE_REG_ADDR  0x180A0014 //ͨ��2բ��1ʹ��
#define  ARS200FPGA_CH2_DATE_REG_ADDR  0x180A0082 //ͨ��2����ͨ����λ
#define  ARS200FPGA_CH2_INTERVAL_ADDR  0x180A0800 //ͨ��2����ͨ���������SRAM
#define  ARS200FPGA_CH2_DATA_ADDR	0x180A0C00 //ͨ��2����ͨ����������SRAM
#define  ARS200FPGA_CH2_TGC_ADDR	0x180A1000 //ͨ��2TGC SRAM
#define  ARS200FPGA_CH2_GATE1CURVE_ADDR   0x18024000 //ͨ�� 2 բ��1 ����SRAM(�°�δ�ҵ�)
#define  ARS200FPGA_CH2_GATE2CURVE_ADDR   0x18026000 //ͨ�� 2 բ��2 ����SRAM(�°�δ�ҵ�)
#define  ARS200FPGA_CH2_GATE3CURVE_ADDR   0x18028000 //ͨ�� 2 բ��3 ����SRAM(�°�δ�ҵ�)


#define ARS200HARDWARE_VER_ADDR		0x18000024 //Ӳ���汾��Ϣ
#define ARS200FUNCTION_DEFINE_ADDR	0x18000026 //������ܶ���1



#define ARS200FPGA_DEVICE_TYPE_ADDR		0x1800002A //�ͺ�ʶ��ؼ���
#define ARM_SELFTEST 0x18000400


#define  LAMP_ADDR   0x18000050  //��


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