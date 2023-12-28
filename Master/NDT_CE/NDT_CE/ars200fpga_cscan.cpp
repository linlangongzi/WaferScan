#include "StdAfx.h"
#include <pkfuncs.h>
#include <ceddk.h>
#include "ars200fpga.h"

BYTE *base;

#define  BASE_ADDR 0x18000000 //����ַ 

#define  CSCAN_ARS200FPGA_SYS_REG_ADDR   0x18000000   //ϵͳ��λ
//#define  CSCAN_ARS200FPGA_SYS_REG_ADDR  0x18000800 //��ʱ����λ�ĵ�ַ��Ϊ���õĵ�ַ����������·��������Ӱ��  yachang.huang modify on 2013-11-18


#define  CSCAN_ARS200FPGA_CODE_REG_ADDR   0x1800002C  //������ʹ��

#define  CSCAN_ARS200FPGA_CH1_CON_REG_ADDR 0x18040000  //ͨ��1��λ
#define  CSCAN_ARS200FPGA_CH1_CONFIG_REG_ADDR 0x18060000  //ͨ��1�첨ģʽ
#define  CSCAN_ARS200FPGA_CH1_GATE_REG_ADDR  0x18060014   //ͨ��1բ��1ʹ��
#define  CSCAN_ARS200FPGA_CH1_FEATURE_REG_ADDR  0x1806004A  //ͨ��1բ��1ͬ��ʱ��
#define  CSCAN_ARS200FPGA_CH1_DATE_REG_ADDR  0x18060082  //ͨ��1�������ݸ�λ
#define  CSCAN_ARS200FPGA_CH1_INTERVAL_ADDR  0x18060800  //ͨ��1����ͨ���������SRAM
#define  CSCAN_ARS200FPGA_CH1_DATA_ADDR	0x18060C00  //ͨ��1����ͨ����������SRAM
#define  CSCAN_ARS200FPGA_CH1_TGC_ADDR	0x18061000 //ͨ��1TGC SRAM
#define  CSCAN_ARS200FPGA_CH1_GATE1CURVE_ADDR   0x18014000  //ͨ�� 1 բ��1 ����SRAM(�°�δ�ҵ�)
#define  CSCAN_ARS200FPGA_CH1_GATE2CURVE_ADDR   0x18016000  //ͨ�� 1 բ��2 ����SRAM(�°�δ�ҵ�)
#define  CSCAN_ARS200FPGA_CH1_GATE3CURVE_ADDR   0x18018000  //ͨ�� 1 բ��2 ����SRAM(�°�δ�ҵ�)
#define  CSCAN_ARS200FPGA_CH1_DAC_REG_ADDR  0x18060012  //ͨ��1DACʹ�ܿ���
#define  CSCAN_ARS200FPGA_CH1_SAMPLING_INTERVAL_TGC_ADDR	0x18060090 //ͨ��1TCG�������


#define  CSCAN_ARS200FPGA_CH2_CON_REG_ADDR 0x18080000  //ͨ��2��λ
#define  CSCAN_ARS200FPGA_CH2_CONFIG_REG_ADDR 0x180A0000 //ͨ��2�첨ģʽ
#define  CSCAN_ARS200FPGA_CH2_GATE_REG_ADDR  0x180A0014  //ͨ��2բ��1ʹ��
#define  CSCAN_ARS200FPGA_CH2_FEATURE_REG_ADDR  0x180A004A  //ͨ��2բ��1ͬ��ʱ��
#define  CSCAN_ARS200FPGA_CH2_DATE_REG_ADDR  0x180A0082  //ͨ��2�������ݸ�λ
#define  CSCAN_ARS200FPGA_CH2_INTERVAL_ADDR  0x180A0800  //ͨ��2����ͨ���������SRAM
#define  CSCAN_ARS200FPGA_CH2_DATA_ADDR	0x180A0C00  //ͨ��2����ͨ����������SRAM
#define  CSCAN_ARS200FPGA_CH2_TGC_ADDR	0x180A1000  //ͨ��2TGC SRAM
#define  CSCAN_ARS200FPGA_CH2_DAC_REG_ADDR  0x180A0012  //ͨ��2DACʹ�ܿ���
#define  CSCAN_ARS200FPGA_CH2_SAMPLING_INTERVAL_TGC_ADDR	0x180A0090 //ͨ��2TCG�������

#define CSCAN_ARS200HARDWARE_VER_ADDR		0x18000024  //Ӳ���汾��Ϣ
#define CSCAN_ARS200FUNCTION_DEFINE_ADDR	0x18000026  //������ܶ���1

#define CSCAN_ARS200FPGA_DEVICE_TYPE_ADDR		0x1800002A  //�ͺ�ʶ��ؼ���

#define  CSCAN_ARM_SELFTEST_ADDR  0x18000400 //ARM�Լ�RAM

#define  CSCAN_CH1_FEATURE_BUFFER_FLAG_ADDR  0x18068000	//ͨ��1�������ݻ�������־λ
#define  CSCAN_CH2_FEATURE_BUFFER_FLAG_ADDR  0x180A8000	//ͨ��2�������ݻ�������־λ

#define  CSCAN_CH1_FEATURE_BUFFER1_ADDR  0x1806A000	//ͨ��1�������ݻ�����1
#define  CSCAN_CH1_FEATURE_BUFFER2_ADDR  0x1806C000	//ͨ��1�������ݻ�����2
#define  CSCAN_CH2_FEATURE_BUFFER1_ADDR  0x180AA000	//ͨ��2�������ݻ�����1
#define  CSCAN_CH2_FEATURE_BUFFER2_ADDR  0x180AC000	//ͨ��2�������ݻ�����2

#define  CSCAN_CH1_MONITOR_FLAG_ADDR  0x18060088	//ͨ��1����������Ч��־λ
#define  CSCAN_CH2_MONITOR_FLAG_ADDR  0x180A0088	//ͨ��2����������Ч��־λ

#define  CSCAN_CH1_ASCAN_DATA_STORE_FLAG_ADDR 0x1806008C //ͨ��1 Aɨ���δ洢�������ɶ���־
#define  CSCAN_CH2_ASCAN_DATA_STORE_FLAG_ADDR 0x180A008C //ͨ��2 Aɨ���δ洢�������ɶ���־

#define  CSCAN_CH1_ASCAN_DATA_STORE_RESET_ADDR 0x1806008E //ͨ��1 Aɨ���δ洢�������ɶ���־��λ
#define  CSCAN_CH2_ASCAN_DATA_STORE_RESET_ADDR 0x180A008E //ͨ��2 Aɨ���δ洢�������ɶ���־��λ

#define  CSCAN_CH1_ASCAN_DATA_STORE_BUFFER_FLAG_ADDR 0x18370000 //ͨ��1 Aɨ���δ洢��������־λ
#define  CSCAN_CH2_ASCAN_DATA_STORE_BUFFER_FLAG_ADDR 0x183B0000 //ͨ��1 Aɨ���δ洢��������־λ

#define  CSCAN_CH1_ASCAN_DATA_STORE_BUFFER1_ADDR  0x18170000	//ͨ��1Aɨ���δ洢������1
#define  CSCAN_CH1_ASCAN_DATA_STORE_BUFFER2_ADDR  0x18270000	//ͨ��1Aɨ���δ洢������2
#define  CSCAN_CH2_ASCAN_DATA_STORE_BUFFER1_ADDR  0x181B0000	//ͨ��2Aɨ���δ洢������1
#define  CSCAN_CH2_ASCAN_DATA_STORE_BUFFER2_ADDR  0x182B0000	//ͨ��2Aɨ���δ洢������2


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

