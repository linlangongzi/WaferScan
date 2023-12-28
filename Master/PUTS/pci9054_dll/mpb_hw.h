#ifndef MPB_HWW_H
#define MPB_HWW_H

#define CHANNEL_OFFSET                     0x6 
#define CHANNEL_CARD_NO                    32
#define CHANNEL_SOFT                       8

#define CLOCK_OFFSET                       0x4

#define CLOCK_500                          0
#define CLOCK_1000                         1
#define CLOCK_2000                         2
#define CLOCK_4000                         4
#define CLOCK_8000                         8

#define BAR2                               2
#define BAR1                               1
#define BAR0                               0
#define HPI_RC_ADDR_L                      0x10
#define HPI_RC_ADDR_H                      0x12

#define HPI_RC_INIT_VALUE                  0x1
#define HPI_HINT_RESET_VALUE               0x4

#define HPI_RA_ADDR_L					   0x14
#define HPI_RA_ADDR_H					   0x16

#define HPI_R_CD_ADDR_L					   0x18   /* CONTINUED DATA*/
#define HPI_R_CD_ADDR_H					   0x1A

#define HPI_R_SD_ADDR_L					   0x1C   /* SINGLE DATA */
#define HPI_R_SD_ADDR_H					   0x1E

#define MONITOR_CHANNEL_1_BASE			   0x80000800
#define MONITOR_CHANNEL_2_BASE			   0x80010800
#define MONITOR_CHANNEL_ADDR_MASK		   0xFFFF
#define MONITOR_CHANNEL_ADDR_SHF		   0x10

#define MONITOR_DATA_SIZE				   1024
#define PSUTHW_BUSWIDTH					   0x2   /* 16 bit */

#define HPI_BUSWIDTH					   0x2
#define DSP_EMIF_BUSWIDTH				   0x4

#define AD_SIZE                            0x2

#define PCI_SAMPLE_BASE						0x10400
#define PCI_SAMPLE_SIZE						0x400

/* SUB_DEVICE_ID */
#define SUB_DEVICE_ID_H                    0x2e
#define SUB_DEVICE_ID_L                    0x2c
#define MUTLI_FUNCTION_CARD_ID             0x101

/* self-test */
#define SELF_TEST_ADDR_L                   0x16800
#define SELF_TEST_ADDR_H                   0x1687f
#define SELF_TEST_HPI_H                    0xa000
#define SELF_TEST_HPI_L_BEGIN              0x8400
#define SELF_TEST_HPI_L_END                0x847f
#define SELF_TEST_VALUE                    0xaaaa

// Song Chenguang add start on 2008-10-30
// HARWARE ADDR
// SYS DATA
#define AEC_SOFTCHANNEL_SWITCH_ADDR			0x8000000C		// 通道1-4 软通道号
#define AEC_SOFTCHANNEL_SWITCH_CH0          0
#define AEC_SOFTCHANNEL_SWITCH_CH1			1
#define AEC_SOFTCHANNEL_SWITCH_CH2          2
#define AEC_SOFTCHANNEL_SWITCH_CH3          3
#define AEC_SOFTCHANNEL_SWITCH_CH4          4
#define AEC_SOFTCHANNEL_SWITCH_CH5          5
#define AEC_SOFTCHANNEL_SWITCH_CH6          6
#define AEC_SOFTCHANNEL_SWITCH_CH7			7

#define AEC_LOCALBUS_BOARDNO_ADDR			0x8000000E		// 局部总线板号

#define AEC_SYS_BOARD_STATUS_ADDR			0x80000010		// 系统故障指示灯
#define AEC_SYS_BOARD_STATUS_ADDR_H			0x8000
#define AEC_SYS_BOARD_STATUS_ADDR_L			0x0010
#define AEC_SYS_BOARD_STATUS_OK				0x1
#define AEC_SYS_BOARD_STATUS_ERROR			0x0

#define AEC_LOCALBUS_EXCHANGE_BASE			0x80000200
#define AEC_LOCALBUS_EXCHANGE_SIZE			256


///////// CHANNEL 1 & 2 SHARED DATA

#define AEC_CH1_2_RESET_ADDR				0x80000000
///////// CHANNEL 3 & 4 SHARED DATA

#define AEC_CH3_4_RESET_ADDR				0x80010000

////////////////// CHANNEL 1 & 2 或 3 & 4 SHARED DATA /////////////////////////////////////////////////////
#define AEC_CHX1_X2_MONITOR_SWITCH_OFFSET		0x2
#define AEC_CHX1_X2_MONITOR_SWITCH_CHX1_FLAW	0
#define AEC_CHX1_X2_MONITOR_SWITCH_CHX1_BE		1
#define AEC_CHX1_X2_MONITOR_SWITCH_CHX2_FLAW	2
#define AEC_CHX1_X2_MONITOR_SWITCH_CHX2_BE		3

#define AEC_CHX1_X2_MONITOR_DELAY_OFFSET		0x4

#define AEC_CHX1_X2_MONITOR_SAMPLE_CONTROLER_OFFSET	0x400
#define AEC_CHX1_X2_MONITRO_SAMPLE_CONTROLER_SIZE	512

#define AEC_CHX1_X2_MONITRO_SAMPLE_DATA_OFFSET		0x800
#define AEC_CHX1_X2_MONITRO_SAMPLE_DATA_SIZE		512

//////////////// channel 1 only

#define AEC_CH1_JIANBOMOSHI_ADDR			0x80004000
//////////////// channel 2 only

#define AEC_CH2_JIANBOMOSHI_ADDR			0x80008000
//////////////// channel 3 only

#define AEC_CH3_JIANBOMOSHI_ADDR			0x80014000
//////////////// channel 4 only

#define AEC_CH4_JIANBOMOSHI_ADDR			0x80018000

////////////////////// offset value /////////////////////////////
#define AEC_CHX_JIANBOMOSHI_QUANBO			0
#define AEC_CHX_JIANBOMOSHI_ZHENGBANBO		1
#define AEC_CHX_JIANBOMOSHI_FUBANBO			2
#define AEC_CHX_MIDDLE_VALUE_OFFSET			0x2
#define AEC_CHX_FRONT_GATE_OFFSET			0x4
#define AEC_CHX_BACK_GATE_OFFSET			0x6				
#define AEC_CHX_MAX_PLATE_HEIGHT_OFFSET		0x8
#define AEC_CHX_MIN_DIBO_PEAK_VALUE_OFFSET	0xA
#define AEC_CHX_ZENGYI_CHANGE_TURN_OFFSET	0xC
#define AEC_CHX_ZENGYI_CHANGE_TURN_OFF		0
#define AEC_CHX_ZENGYI_CHANGE_TURN_ON		1
#define AEC_CHX_ZENGYI_CONTROL_A_OFFSET		0xE
#define AEC_CHX_ZENGYI_CONTROL_B_OFFSET		0x10
#define AEC_CHX_ZENGYI_CONTROL_C_OFFSET		0x12
#define AEC_CHX_ZENGYI_CONTROL_D_OFFSET		0x14
#define AEC_CHX_RESTRAIN_OFFSET				0x16
#define AEC_CHX_ZERO_DELAY_OFFSET			0x18

#define AEC_CHX_FLAW_MONI_TURN_OFFSET		0x1A
#define AEC_CHX_FLAW_MONI_TURN_ZERO			0
#define AEC_CHX_FLAW_MONI_TURN_N6			1
#define AEC_CHX_FLAW_MONI_TURN_N12			2
#define AEC_CHX_FLAW_MONI_TURN_N18			3
#define AEC_CHX_FLAW_MONI_TURN_N24			4  
#define AEC_CHX_FLAW_MONI_TURN_N30			5  
#define AEC_CHX_FLAW_MONI_TURN_N36			6  
#define AEC_CHX_FLAW_MONI_TURN_N42			7

#define AEC_CHX_BE_MONI_TURN_OFFSET			0x1C
#define AEC_CHX_BE_MONI_TURN_ZERO			0
#define AEC_CHX_BE_MONI_TURN_N6				1
#define AEC_CHX_BE_MONI_TURN_N12			2
#define AEC_CHX_BE_MONI_TURN_N18			3
#define AEC_CHX_BE_MONI_TURN_N24			4  
#define AEC_CHX_BE_MONI_TURN_N30			5  
#define AEC_CHX_BE_MONI_TURN_N36			6  
#define AEC_CHX_BE_MONI_TURN_N42			7

#define AEC_CHX_AUTO_ZENGYI_TURN_OFFSET		0x1E
#define AEC_CHX_AUTO_ZENGYI_TURN_OFF		0
#define AEC_CHX_AUTO_ZENGYI_TURN_ON			1

#define AEC_CHX_FLAW_BE_DIFF_OFFSET			0x20
#define AEC_CHX_SOFT_CHANNEL_NUM_OFFSET		0x80
#define AEC_CHX_FLAW_TIME_OFFSET			0x82
#define AEC_CHX_FLAW_PEAK_OFFSET			0x84
#define AEC_CHX_BE_TIME_OFFSET				0x86
#define AEC_CHX_BE_PEAK_OFFSET				0x88
#define AEC_CHX_THICK_OFFSET				0x8A
#define AEC_CHX_PHY_DAC_OFFSET				0x800	
#define AEC_CHX_SOFT_DAC_OFFSET				0x2000
// Song Chenguang add end on 2008-10-30
// Song Chenguang add start on 2008-11-17
#define MPB_SYSTEM_START_ADDR				0x11c
// Song Chenguang add start on 2009-03-13
#define MPB_SYSTEM_BOUND_PROBE_STATE_ADDR	0x11e
// Song Chenguang add end on 2009-03-13
	
#define MPB_LSMP2_DPC31_SWITCH_ADDR			0x0c
#define MPB_CODER1_L_ADDR					0x30
#define MPB_CODER1_H_ADDR					0x32
#define MPB_CODER2_L_ADDR					0x34
#define MPB_CODER2_H_ADDR					0x36
#define MPB_CODER3_L_ADDR					0x38
#define MPB_CODER3_H_ADDR					0x3a
#define MPB_CODER4_L_ADDR					0x3c
#define MPB_CODER4_H_ADDR					0x3e
#define MPB_CODER5_L_ADDR					0x40
#define MPB_CODER5_H_ADDR					0x42

#define MPB_CODER1_NUM_PER_RING_ADDR		0x13c
#define MPB_CODER1_NUM_PER_RING				1200
#define MPB_CODER2_NUM_PER_RING_ADDR		0x132
#define MPB_CODER2_NUM_PER_RING				1200
#define MPB_CODER3_NUM_PER_RING_ADDR		0x134
#define MPB_CODER3_NUM_PER_RING				1200
#define MPB_CODER4_NUM_PER_RING_ADDR		0x136
#define MPB_CODER4_NUM_PER_RING				1200
#define MPB_CODER5_NUM_PER_RING_ADDR		0x138
#define MPB_CODER5_NUM_PER_RING				1200

#define MPB_CARD_READY_ADDR					0x02
#define MPB_CARD_READY_OK					0x1
#define MPB_CARD_READY_ERROR				0x0

#define MPB_SYS_READY_ADDR					0x08
#define MPB_SYS_READY_OK					0x1
#define MPB_SYS_READY_ERROR					0x0			
// Song Chenguang add end on 2008-11-17
// Song Chenguang add start on 2009-03-02
#define MPB_REF_PROBE_UPDOWN_STATE_ADDR			0x118
#define MPB_NOREF_PROBE_UPDOWN_STATE_ADDR		0x11a
#define MPB_NEITAN1_PROBE_UPDOWN_STATE_ADDR		0x110
#define MPB_NEITAN2_PROBE_UPDOWN_STATE_ADDR		0x112
#define MPB_NEITAN3_PROBE_UPDOWN_STATE_ADDR		0x114
#define MPB_NEITAN4_PROBE_UPDOWN_STATE_ADDR		0x116
// Song Chenguang add end on 2009-03-02

/* hardware configure Initialize */
typedef struct {
	WORD CH_DEM_MODE;   // 检波模式  全波0，正半波1，负半波2
	WORD MIDDLE_VALUE;  // AD中值
	WORD CH_FRONTTIME;	//闸门前沿	mm			
	WORD CH_REARTIME;   //闸门后沿  mm          
	WORD CH_MAXBACKTIME;	//最大底波位置 mm
	WORD CH_BEMINDATA;		//底波阈值
//	WORD CH_TGC_DA_START;	
	WORD CH_GAIN_DIFF;		// 伤波通道与底波通道增益差值,初值66
	WORD CH_GAIN_A;			
	WORD CH_GAIN_B;
	WORD CH_GAIN_C;
	WORD CH_GAIN_D;			//增益值		dB
	WORD CH_REJECT_VALUE;	//抑制
	WORD CH_ZEROCORRECT;	//延迟		mm  //chenyuzhu modify on 2014-10-10(后改为时间)
	//DWORD CH_ZEROCORRECT;	//延迟		mm  //chenyuzhu modify on 2014-10-10(后改为时间)
	WORD CH_FLAW_GAIN_MUX;	//伤波衰减档	0dB：0；-6dB：1；-12dB:2，....-42dB：7
	WORD CH_BE_GAIN_MUX;	//底波衰减档	0dB：0；-6dB：1；-12dB:2，....-42dB：7
	WORD CH_TGC_DAC_EN;		//DAC开启		关闭：0；开启：1	
	WORD CH_FLAW_SURF_DISTANCE;				//上盲区		mm
	WORD CH_FLAW_BE_DISTANCE;				//下盲区		mm
	WORD CH_EN;								//通道开启 关闭：0；开启：1
	WORD AUTO_GAIN_EN;						//自动增益 关闭：0；开启：1
	WORD AUTO_GATE_EN;						//自动闸门 关闭：0；开启：1
	WORD ALARM_EN;							//报警开关 关闭：0；开启：1
	WORD CH_REF_NO;							//逻辑通道号
	WORD CH_FLAW_DEF;						// 伤属性
	WORD CH_SELFCHECK;						//自检开关 关闭：0；开启：1
	WORD CH_MONITOR_RANGE;					
	WORD CH_NO;								//参考通道号
	DWORD id;
} CH_CONFIG;				/*主机下传数据结构,setall、FPGA硬通道 和调试时 setone 的配置数据结构*/

typedef struct {
	WORD DETECT_STANDARD;			// 探伤标准
	WORD ALARM_VALUE;				// 伤波报警电平
	// Song Chenguang add start on 2008-11-11
	WORD BE_ALARM_VALUE;			// 底波报警电平
	// Song Chenguang add end on 2008-11-11
	WORD K_VALUE;					// K值
	WORD COMPENSATE_VALUE;			// 测厚补偿系数
	WORD SOUND_VELOCITY;			// 声速( 单位m/s )
//	WORD THICKNESS_DIFF_RANGE;
	// Song Chenguang add start on 2008-11-11
	WORD THICKNESS_POS_DIFF_RANGE;		// 厚度正公差
	WORD THICKNESS_NEG_DIFF_RANGE;		// 厚度负公差
	// Song Chenguang add end on 2008-11-11
	WORD THICKNESS_STANDARD;			// 标称厚度
	// Song Chenguang add start on 2008-11-11
	WORD AUTO_GAIN_RANGE_MIN;			// 自动增益起始值
	WORD AUTO_GAIN_RANGE_MAX;			// 自动增益停止值
	WORD BE_MIDDLE_VALUE;				// 底波稳定值
	WORD AUTO_GAIN_Threshold;			// 底波连续超限的次数
	WORD AUTO_GATE_RANGE_MIN;			// 自动闸门起始值
	WORD AUTO_GATE_RANGE_MAX;			// 自动闸门停止值
	WORD AUTO_GATE_Threshold;			// 自动闸门连续超限次数
	WORD FLAW_JUDGE_Threshold;			// 连续伤数量
	WORD START_DETECT_TIME;				// 可以自动探伤的时刻
	// Song Chenguang add end on 2008-11-11
	// Song Chenguang add start on 2008-11-24
	WORD AUTO_GAIN_RANGE_NEG_MIN;		// 自动增益底波幅度负起始值 10（与中值255比0.3db）
	WORD AUTO_GAIN_RANGE_NEG_MAX;		// 自动增益底波幅度负结束值 （255-最小底波值）
	WORD AUTO_GAIN_DB_RANGE_SUB;		// 自动增益db点数减小的范围 10 >GAIN_C
	WORD AUTO_GAIN_DB_RANGE_ADD;		// 自动增益db点数增加的范围 20(表示底波减小时，累计增益可增加6dB)
	WORD AD_SAMPLE_VALUE_MAX;			// AD采样的饱和值  512
	WORD AD_SAMPLE_MIDDLE_VALUE;		// AD采样饱和值的一定百分比，当前设为50%，判伤和判别失波用  256
	WORD FLAWTIME_RANGE;				// 阈值和比例判伤中，判定连续两个伤点连续时，伤深度的差值  20（1.2mm内）
	WORD BE_JUDGE_Threshold;			// 失波判伤的连续数 1
	WORD HEIGHT_JUDGE_Threshold;		// 厚度超差判别的连续数 1
	// Song Chenguang add end on 2008-11-24
	WORD CHIP1_SAMPLE_SELECT;			//监视硬通道1、2选择
	WORD CHIP1_SAMPLE_ZEROCORRECT;      //监视硬通道1、2零点
	WORD CHIP1_SYN_SIG_SELECT;          //硬件调试备用
	WORD CHIP1_SYN_4K;                  //硬件调试备用 
	WORD CHIP1_SYN_500;                 //硬件调试备用
	WORD CHIP2_SAMPLE_SELECT;           //监视硬通道3、4选择
	WORD CHIP2_SAMPLE_ZEROCORRECT;      //监视硬通道3、4零点
	WORD CHIP2_SYN_SIG_SELECT;          //硬件调试备用  
	WORD CHIP2_SYN_4K;                  //硬件调试备用
	WORD CHIP2_SYN_500;	                //硬件调试备用 
	WORD LOCAL_BUS_CARD_NO;             //AEC板卡编号
} CH_CONFIG_GLOBAL;				/*主机下传对通道进行全局配置的结构体*/


#define UST_RAM_ADDR  0x28000			/* DSP中分配给PCI和FPGA操作的地址空间基地址*/
/**********************************************************/
/*32通道配置数据操作地址 */
#define CHX_CONFIG_ADDR  (UST_RAM_ADDR+0)
#define CHX_CONFIG_ADDR_LENGTH  0x6C0


#define FPGA_CH1_PHY_DAC_BASE 0x80004800
#define FPGA_CH1_SOFT_DAC_BASE 0x80006000
#define FPGA_CH2_PHY_DAC_BASE 0x80008800
#define FPGA_CH2_SOFT_DAC_BASE 0x8000a000
#define FPGA_CH3_PHY_DAC_BASE 0x80014800
#define FPGA_CH3_SOFT_DAC_BASE 0x80016000
#define FPGA_CH4_PHY_DAC_BASE 0x80018800
#define FPGA_CH4_SOFT_DAC_BASE 0x8001a000

#define DSP_INTERRUPT_H     0x0188
#define DSP_INTERRUPT_L     0x0000


typedef struct {
	WORD CH_DEM_MODE;
	WORD MIDDLE_VALUE;
	WORD CH_FRONTTIME;					
	WORD CH_REARTIME;               
	WORD CH_MAXBACKTIME;
	WORD CH_BEMINDATA;
	WORD CH_TGC_DA_START;
	WORD CH_GAIN_A;
	WORD CH_GAIN_B;
	WORD CH_GAIN_C;
	WORD CH_GAIN_D;
	WORD CH_REJECT_VALUE;
	WORD CH_ZEROCORRECT;
	WORD CH_FLAW_GAIN_MUX;
	WORD CH_BE_GAIN_MUX;
	WORD CH_TGC_DAC_EN;
	WORD CH_FLAW_SURF_DISTANCE;					/*PCI初始下传时为上盲区*/
	WORD CH_FLAW_BE_DISTANCE;				/*下盲区*/
	WORD CH_EN;
	WORD AUTO_GAIN_EN;
	WORD AUTO_GATE_EN;
	WORD ALARM_EN;
	WORD CH_REF_NO;
	WORD CH_FLAW_DEF;
	WORD CH_SELFCHECK;
	WORD CH_MONITOR_RANGE;
	DWORD CH_NO;
}SETONE_DATA;

#define SETONE_OR_ALL         0x16000
// Song Chenguang add start on 2008-07-07
#define PCI2DSP_BASE		  0x16000
#define SIM_END_OFFSET        0
#define SIM_END_VALUE         3
#define SIM_END_VALUE_RESET   2
#define SIM_END_NO_EFFECT	  0
#define SIM_SEL_CHANNEL		  4
// Song Chenguang add end on 2008-07-07
#define SETONE_DATA_ADDR      0x16002
#define SETONE_DATA_ADDR_END  0x16038
#define SETALL_QUERY          0x143C4 // 0x14374

#define CHANNEL_SEL_ADDR      0x16038
#define MAX_CARD_NO           16
#define MAX_CARD_CHANNEL_NO   32

#define INTERRUPT_TYPE_ADDR        0x123C4 //0x12374
#define INTERRUPT_REAL_TIME_1      1
#define INTERRUPT_REAL_TIME_2      2
#define INTERRUPT_RESULT           3
// Song Chenguang add start on 2008-12-18
#define INTERRUPT_TYPE_ADDR_AUTO_ZENGYI		0x123C6
#define INTERRUPT_TYPE_ADDR_AUTO_ZENGYI_FLAW	0x123C8
#define AUTO_PLUS_FLAG				1
// Song Chenguang add end on 2008-12-18



#define DEFECT_END_FLAG_ADDR       0x10002
#define DEFECT_REAL_TIME_DATA_ADDR 0x10400


#define CH_FLAW_DATA_ADDR_1        0x12002
#define CH_FLAW_DATA_ADDR_2        0x14002

#define CH_FLAW_RESULT_ADDR        0xb0000000
#define DELAY_TIME                 500

#define PCI_DSP_INT_MASK           0x2
#define FLAW_END_ADDR              0x10004
#define PCI_SAMPLE_NO_END          0x0
#define PCI_SAMPLE_END             0x1
#define PCI_SAMPLE_ZERO_OFFSET	   0x10000
#define PCI_SAMPLE_RESET_OFFSET    0x10002

#define PCI_SAMPLE_RESET_ON          0x1
#define PCI_SAMPLE_RESET_OFF         0x0
#define PCI_2_DSP_FLAG_OFFSET        0x16400
// Song Chenguang add start on 2009-02-09
#define DSP_2_PCI_SRAM_CONTROL1_ADDR			0x16402
#define DSP_2_PCI_SRAM_CONTROL2_ADDR			0x16404
#define PCI_2_DSP_SRAM_CONTROL_ADDR				0x16406
#define PCI_2_DSP_COMMUNICATE_FLAG_ADDR			0x16408
#define DSP_2_PCI_COMMUNICATE_FLAG_ADDR			0x1640e
#define PCI_CLEAR_COMMUNICATE_FLAG_ADDR			0x1640a
#define PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR		0x1640c
#define DSP_2_PCI_COMMUNICATE_CMD_NUM_ADDR		0x16410
// Song Chenguang add end on 2009-02-09



#define MAX_CHANNEL_NO               500
#define VSOUND                       6000            // m/s
#define ADFREQUENCY                  50              // MHz

#define PSUT_CARD_NO_BASE            1
#define MONITOR_RANGE_ADDR           0x10800
#define FLAW_DOT_MAX_NO              512
#define MONITOR_SIZE                 512
#define AVG_COUNT                    1

#define PLATE_FLAW_HEIGHT            32
#define PLATE_FLAW_WIDTH             128

#define PLATE_WIDTH                  12000

#define CODEC_OUT_STATE          0x00
#define CODEC_1_START            0x01
#define CODEC_1_END              0x11
#define CODEC_2_START            0x02
#define CODEC_2_END              0x12
#define CODEC_3_START            0x03
#define CODEC_3_END              0x13
#define CODEC_ADDR               0xA


#define CODEC_INFO_ADDR_START       0x22
#define CODEC_INFO_ADDR_END         0x2C
#define CODEC_INFO_ADDR_SIZE        12

#define PROBE_STATE_ADDR            0x14
#define PROBE_STATE_ADDR_SIZE       0x8

#define MULTI_CARD_SELF_TEST_ADDR            0x80
#define MULTI_CARD_SELF_TEST_ADDR_SIZE       8

#define PROBE_RATIO                          4
#define PROBE_STATE_SIZE                     4
#define REF_GAIN							 15
//#define REF_GAIN							 22


#define GAIN_MAX					35.0
#define GAIN_MIN                    0
#define	GAIN_DATA_MAX				255
#define	GAIN_DATA_MIN				0

#define DEFAULT_CHANNEL			    16


#define SYS_STATUS_FLAG_ADDR_H      0x8000
#define SYS_STATUS_FLAG_ADDR_L      0x0010

#define SYS_STATUS_FLAG_DATA_H_OK			0x0
#define SYS_STATUS_FLAG_DATA_H_NOT_Ok		0X0

#define SYS_STATUS_FLAG_DATA_L_OK			0x1
#define SYS_STATUS_FLAG_DATA_L_NOT_Ok		0X0

// Song Chenguang add start on 2008-11-28
#define MFC_DPC31_LSMP2_SELECTOR_ADDR       0x0c
#define MFC_DPC31_LSMP2_SELECTOR_DPC31      0x0
#define MFC_DPC31_LSMP2_SELECTOR_LSMP2      0x1
// Song Chenguang add end on 2008-11-28
// Song Chenguang add start on 2009-01-13
/* 放大器线性修改系数*/
#define AEC_AMP_LINEAR_PARA_A               0.90
#define AEC_AMP_LINEAR_PARA_B               0.032
// Song Chenguang add end on 2009-01-13


// 版本信息
#define DSP_VER_ADDRESS_H					0xa000
#define DSP_VER_ADDRESS_L					0x000e
#define FPGA1_VER_ADDRESS_H					0x8000	// DSP
#define FPGA1_VER_ADDRESS_L					0x0012
#define FPGA2_VER_ADDRESS_H					0x8001	// DSP
#define FPGA2_VER_ADDRESS_L					0x0012
#define FPGA3_VER_ADDRESS					0x10008		// PCI Bar2空间下
#endif