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
#define AEC_SOFTCHANNEL_SWITCH_ADDR			0x8000000C		// ͨ��1-4 ��ͨ����
#define AEC_SOFTCHANNEL_SWITCH_CH0          0
#define AEC_SOFTCHANNEL_SWITCH_CH1			1
#define AEC_SOFTCHANNEL_SWITCH_CH2          2
#define AEC_SOFTCHANNEL_SWITCH_CH3          3
#define AEC_SOFTCHANNEL_SWITCH_CH4          4
#define AEC_SOFTCHANNEL_SWITCH_CH5          5
#define AEC_SOFTCHANNEL_SWITCH_CH6          6
#define AEC_SOFTCHANNEL_SWITCH_CH7			7

#define AEC_LOCALBUS_BOARDNO_ADDR			0x8000000E		// �ֲ����߰��

#define AEC_SYS_BOARD_STATUS_ADDR			0x80000010		// ϵͳ����ָʾ��
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

////////////////// CHANNEL 1 & 2 �� 3 & 4 SHARED DATA /////////////////////////////////////////////////////
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
	WORD CH_DEM_MODE;   // �첨ģʽ  ȫ��0�����벨1�����벨2
	WORD MIDDLE_VALUE;  // AD��ֵ
	WORD CH_FRONTTIME;	//բ��ǰ��	mm			
	WORD CH_REARTIME;   //բ�ź���  mm          
	WORD CH_MAXBACKTIME;	//���ײ�λ�� mm
	WORD CH_BEMINDATA;		//�ײ���ֵ
//	WORD CH_TGC_DA_START;	
	WORD CH_GAIN_DIFF;		// �˲�ͨ����ײ�ͨ�������ֵ,��ֵ66
	WORD CH_GAIN_A;			
	WORD CH_GAIN_B;
	WORD CH_GAIN_C;
	WORD CH_GAIN_D;			//����ֵ		dB
	WORD CH_REJECT_VALUE;	//����
	WORD CH_ZEROCORRECT;	//�ӳ�		mm  //chenyuzhu modify on 2014-10-10(���Ϊʱ��)
	//DWORD CH_ZEROCORRECT;	//�ӳ�		mm  //chenyuzhu modify on 2014-10-10(���Ϊʱ��)
	WORD CH_FLAW_GAIN_MUX;	//�˲�˥����	0dB��0��-6dB��1��-12dB:2��....-42dB��7
	WORD CH_BE_GAIN_MUX;	//�ײ�˥����	0dB��0��-6dB��1��-12dB:2��....-42dB��7
	WORD CH_TGC_DAC_EN;		//DAC����		�رգ�0��������1	
	WORD CH_FLAW_SURF_DISTANCE;				//��ä��		mm
	WORD CH_FLAW_BE_DISTANCE;				//��ä��		mm
	WORD CH_EN;								//ͨ������ �رգ�0��������1
	WORD AUTO_GAIN_EN;						//�Զ����� �رգ�0��������1
	WORD AUTO_GATE_EN;						//�Զ�բ�� �رգ�0��������1
	WORD ALARM_EN;							//�������� �رգ�0��������1
	WORD CH_REF_NO;							//�߼�ͨ����
	WORD CH_FLAW_DEF;						// ������
	WORD CH_SELFCHECK;						//�Լ쿪�� �رգ�0��������1
	WORD CH_MONITOR_RANGE;					
	WORD CH_NO;								//�ο�ͨ����
	DWORD id;
} CH_CONFIG;				/*�����´����ݽṹ,setall��FPGAӲͨ�� �͵���ʱ setone ���������ݽṹ*/

typedef struct {
	WORD DETECT_STANDARD;			// ̽�˱�׼
	WORD ALARM_VALUE;				// �˲�������ƽ
	// Song Chenguang add start on 2008-11-11
	WORD BE_ALARM_VALUE;			// �ײ�������ƽ
	// Song Chenguang add end on 2008-11-11
	WORD K_VALUE;					// Kֵ
	WORD COMPENSATE_VALUE;			// ��񲹳�ϵ��
	WORD SOUND_VELOCITY;			// ����( ��λm/s )
//	WORD THICKNESS_DIFF_RANGE;
	// Song Chenguang add start on 2008-11-11
	WORD THICKNESS_POS_DIFF_RANGE;		// ���������
	WORD THICKNESS_NEG_DIFF_RANGE;		// ��ȸ�����
	// Song Chenguang add end on 2008-11-11
	WORD THICKNESS_STANDARD;			// ��ƺ��
	// Song Chenguang add start on 2008-11-11
	WORD AUTO_GAIN_RANGE_MIN;			// �Զ�������ʼֵ
	WORD AUTO_GAIN_RANGE_MAX;			// �Զ�����ֵֹͣ
	WORD BE_MIDDLE_VALUE;				// �ײ��ȶ�ֵ
	WORD AUTO_GAIN_Threshold;			// �ײ��������޵Ĵ���
	WORD AUTO_GATE_RANGE_MIN;			// �Զ�բ����ʼֵ
	WORD AUTO_GATE_RANGE_MAX;			// �Զ�բ��ֵֹͣ
	WORD AUTO_GATE_Threshold;			// �Զ�բ���������޴���
	WORD FLAW_JUDGE_Threshold;			// ����������
	WORD START_DETECT_TIME;				// �����Զ�̽�˵�ʱ��
	// Song Chenguang add end on 2008-11-11
	// Song Chenguang add start on 2008-11-24
	WORD AUTO_GAIN_RANGE_NEG_MIN;		// �Զ�����ײ����ȸ���ʼֵ 10������ֵ255��0.3db��
	WORD AUTO_GAIN_RANGE_NEG_MAX;		// �Զ�����ײ����ȸ�����ֵ ��255-��С�ײ�ֵ��
	WORD AUTO_GAIN_DB_RANGE_SUB;		// �Զ�����db������С�ķ�Χ 10 >GAIN_C
	WORD AUTO_GAIN_DB_RANGE_ADD;		// �Զ�����db�������ӵķ�Χ 20(��ʾ�ײ���Сʱ���ۼ����������6dB)
	WORD AD_SAMPLE_VALUE_MAX;			// AD�����ı���ֵ  512
	WORD AD_SAMPLE_MIDDLE_VALUE;		// AD��������ֵ��һ���ٷֱȣ���ǰ��Ϊ50%�����˺��б�ʧ����  256
	WORD FLAWTIME_RANGE;				// ��ֵ�ͱ��������У��ж����������˵�����ʱ������ȵĲ�ֵ  20��1.2mm�ڣ�
	WORD BE_JUDGE_Threshold;			// ʧ�����˵������� 1
	WORD HEIGHT_JUDGE_Threshold;		// ��ȳ����б�������� 1
	// Song Chenguang add end on 2008-11-24
	WORD CHIP1_SAMPLE_SELECT;			//����Ӳͨ��1��2ѡ��
	WORD CHIP1_SAMPLE_ZEROCORRECT;      //����Ӳͨ��1��2���
	WORD CHIP1_SYN_SIG_SELECT;          //Ӳ�����Ա���
	WORD CHIP1_SYN_4K;                  //Ӳ�����Ա��� 
	WORD CHIP1_SYN_500;                 //Ӳ�����Ա���
	WORD CHIP2_SAMPLE_SELECT;           //����Ӳͨ��3��4ѡ��
	WORD CHIP2_SAMPLE_ZEROCORRECT;      //����Ӳͨ��3��4���
	WORD CHIP2_SYN_SIG_SELECT;          //Ӳ�����Ա���  
	WORD CHIP2_SYN_4K;                  //Ӳ�����Ա���
	WORD CHIP2_SYN_500;	                //Ӳ�����Ա��� 
	WORD LOCAL_BUS_CARD_NO;             //AEC�忨���
} CH_CONFIG_GLOBAL;				/*�����´���ͨ������ȫ�����õĽṹ��*/


#define UST_RAM_ADDR  0x28000			/* DSP�з����PCI��FPGA�����ĵ�ַ�ռ����ַ*/
/**********************************************************/
/*32ͨ���������ݲ�����ַ */
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
	WORD CH_FLAW_SURF_DISTANCE;					/*PCI��ʼ�´�ʱΪ��ä��*/
	WORD CH_FLAW_BE_DISTANCE;				/*��ä��*/
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
/* �Ŵ��������޸�ϵ��*/
#define AEC_AMP_LINEAR_PARA_A               0.90
#define AEC_AMP_LINEAR_PARA_B               0.032
// Song Chenguang add end on 2009-01-13


// �汾��Ϣ
#define DSP_VER_ADDRESS_H					0xa000
#define DSP_VER_ADDRESS_L					0x000e
#define FPGA1_VER_ADDRESS_H					0x8000	// DSP
#define FPGA1_VER_ADDRESS_L					0x0012
#define FPGA2_VER_ADDRESS_H					0x8001	// DSP
#define FPGA2_VER_ADDRESS_L					0x0012
#define FPGA3_VER_ADDRESS					0x10008		// PCI Bar2�ռ���
#endif