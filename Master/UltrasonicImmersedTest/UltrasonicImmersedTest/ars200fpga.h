#ifndef _ARS200FPGA_H_
#define _ARS200FPGA_H_

//#pragma pack(1)

typedef struct _ARS200FPGA_SYS_REG
{

	WORD wSoftReset;	//ϵͳ��λ
	WORD wRelay;		//���м̵�������bit10-0
	WORD wSPIReset;     //SPI��λ
	WORD wSPIChannel;	//SPIѡ��
	WORD wSPIStart;		//SPI���俪ʼ
	WORD wSPIModeSelect;	//SPIģʽѡ��
	WORD wSPIAddr;	//SPI��ַ
	WORD wSPIData;   //SPI����
	WORD wNULL1;   //
	WORD wNULL2;   //
	WORD wNULL3;   //
	WORD wNULL4;   //
	WORD wNULL5;   //	
	WORD wSPIReady;		//SPI����
	WORD wLedAlarm;		//LED����
	WORD wLedBrightness;	//LED��������
	WORD wPowerSupply;	// ��Դָʾ��
	WORD wWorkMode;		// ����ģʽ
} ARS200FPGA_SYS_REG;

// songchenguang add start 2010-10-13
// Ӳ���汾
typedef struct _ARS200VER
{
	WORD wHardwareVer;
}ARS200VER;

// ������ܶ���
typedef struct _ARS200FUN_DEFINE
{
	WORD wLowFunctionDefine;
	WORD wHighFunctionDefine;
}ARS200FUN_DEFINE;
// songchenguang add end 2010-10-13
/*
// wangbingfu add start 2010-11-11
//�豸�ͺ�
typedef struct _ARS200DEVICETYPE
{
	WORD wDeviceType;
}ARS200DEVICETYPE;
// wangbingfu add start 2010-11-11
*/

/*  ����û�ô���ע�� yachang.huang add on 2013-12-2
typedef struct _ARS200FPGA_CH_CON_REG
{
	WORD wReset;		//ͨ����λ
	WORD wEnable;		//ʹ��
	WORD wSuspend;		//����
	WORD wRepeatCounter;	//�ظ�Ƶ�ʼ�����
	WORD wDetectMode;	//�첨ģʽ
	WORD wValueMiddle;	//��ֵ
	WORD wMaiKuan;		//����
} ARS200FPGA_CH_CON_REG;
*/

typedef struct _ARS200FPGA_CH_PLUS_REG
{
	WORD wdBControl;	//����ת������
	WORD wdBRef;		//������ƻ�׼
	WORD wdBLevel1;		//1������ AD603
	WORD wdBLevel2;		//2������ AD600-1
	WORD wdBLevel3;		//3������ AD600-2
} ARS200FPGA_CH_PLUS_REG;

typedef struct _ARS200FPGA_CH_DAC_REG
{
	WORD wDACEnable;	//DACʹ�ܿ���
	WORD wDACFront;		//DACǰբ��
	WORD wDACRear;		//DAC��բ��
} ARS200FPGA_CH_DAC_REG;

typedef struct _ARS200FPGA_CH_GATE_REG
{
	WORD wGate1Enable;	//բ��1ʹ��
	WORD wGate1Alarm;	//բ��1������������
	WORD wGate1Front;	//բ��1ǰ��
	WORD wGate1Rear;	//բ��1����
	WORD wGate2Enable;	//բ��2ʹ��
	WORD wGate2Alarm;	//բ��2������������
	WORD wGate2Front;	//բ��2ǰ��
	WORD wGate2Rear;	//բ��2����
	WORD wGate3Enable;	//բ��3ʹ��
	WORD wGate3Alarm;	//բ��3������������
	WORD wGate3Front;	//բ��3ǰ��
	WORD wGate3Rear;	//բ��3����
	WORD wAlarmFlag;	//������־λ
	WORD wAlarmReg;		//��������
	WORD wGateDone;		//բ�Ŵ������

	WORD wGate1DefectAmp;	//բ��1����˷�
	WORD wGate1DefectTime;	//բ��1�����ʱ��

	WORD wGate2DefectAmp;	//բ��2����˷�
	WORD wGate2DefectTime;	//բ��2�����ʱ��

	WORD wGate3DefectAmp;	//բ��3����˷�
	WORD wGate3DefectTime;	//բ��3�����ʱ��

	WORD wGate1Peak;		//բ��1����ֵ
	WORD wGate2Peak;		//բ��2����ֵ
} ARS200FPGA_CH_GATE_REG;

typedef struct _ARS200FPGA_CH_DATA_REG
{
	WORD wMonitorReset;	//����ͨ����λ
	WORD wMonitorZeroCounter;	//�������������
	WORD wMonitorFlag;	//������Ч��־
	WORD wMonitorDelayCounter;	// ����ͨ���ӳټ�����
} ARS200FPGA_CH_DATA_REG;

//	cscan wangbingfu add start at 2012-1-13
//������
typedef struct _CSCAN_ARS200FPGA_CODE_REG
{
	WORD wEnable;		//������ʹ��
	WORD wReset;		//��������λ
	WORD wXLow;			//X���������16λ
	WORD wXHigh;		//X���������16λ
	WORD wYLow;			//Y���������16λ
	WORD wYHigh;		//Y���������16λ
	WORD wZLow;			//Z���������16λ
	WORD wZHigh;		//Z���������16λ
	WORD wW1Low;		//W1���������16λ
	WORD wW1High;		//W1���������16λ
	WORD wALow;			//A���������16λ
	WORD wAHigh;		//A���������16λ
	WORD wBLow;			//B���������16λ
	WORD wBHigh;		//B���������16λ
	WORD wW2Low;		//W2���������16λ
	WORD wW2High;		//W2���������16λ
} CSCAN_ARS200FPGA_CODE_REG;

typedef struct _CSCAN_ARS200FPGA_CH_CON_REG
{
	WORD wReset;		//ͨ����λ
	WORD wRepeatCounter;	//�ظ�Ƶ�ʼ�����
	WORD wMaiKuan;		//����
} CSCAN_ARS200FPGA_CH_CON_REG;

typedef struct _CSCAN_ARS200FPGA_CH_CONFIG_REG
{
	WORD wDetectMode;	//�첨ģʽ
	WORD wValueMiddle;	//��ֵ
	WORD wdBControl;	//����ת������
	WORD wdBRef;		//������ƻ�׼
	WORD wdBLevel1;		//1������ AD603
	WORD wdBLevel2;		//2������ AD600-1
	WORD wdBLevel3;		//3������ AD600-2
	WORD wSuspend;		//����
	WORD wMonitorZeroCounter;	//�������������
	WORD wDACEnable;	//DACʹ�ܿ���
} CSCAN_ARS200FPGA_CH_CONFIG_REG;

typedef struct _CSCAN_ARS200FPGA_CH_GATE_REG
{
	WORD wGate1Enable;					//բ��1ʹ��
	WORD wGate1SyncMode;				//բ��1ͬ��ģʽ
   	WORD wGate1SyncThreshold;			//բ��1ͬ����ֵ
   	WORD wGate1Start;					//բ��1���			
  	WORD wGate1Width;              		//բ��1��� 
   	WORD wGate1Amp;						//բ��1��ֵ
   	WORD wGate2Enable;					//բ��2ʹ��
   	WORD wGate2SyncMode;				//բ��2ͬ��ģʽ
   	WORD wGate2SyncThreshold;			//բ��2ͬ����ֵ
   	WORD wGate2Start;					//բ��2���			
   	WORD wGate2Width;              		//բ��2��� 
   	WORD wGate2Amp;						//բ��2��ֵ
   	WORD wGate3Enable;					//բ��3ʹ��
   	WORD wGate3SyncMode;				//բ��3ͬ��ģʽ
   	WORD wGate3SyncThreshold;			//բ��3ͬ����ֵ
   	WORD wGate3Start;					//բ��3���			
   	WORD wGate3Width;              		//բ��3��� 
   	WORD wGate3Amp;						//բ��3��ֵ
   	WORD wGate4Enable;					//բ��4ʹ��
   	WORD wGate4SyncMode;				//բ��4ͬ��ģʽ
   	WORD wGate4SyncThreshold;			//բ��4ͬ����ֵ
   	WORD wGate4Start;					//բ��4���			
   	WORD wGate4Width;              		//բ��4��� 
   	WORD wGate4Amp;						//բ��4��ֵ
   	WORD wSimpleInterval;				//ͨ���������
   	WORD wSimpleAverage;				//ͨ������ƽ����
   	WORD wDelay;						//ͨ���ӳ�ʱ��
}CSCAN_ARS200FPGA_CH_GATE_REG;

typedef struct _CSCAN_ARS200FPGA_SETONE
{
	CSCAN_ARS200FPGA_CH_CON_REG stCon;
	CSCAN_ARS200FPGA_CH_CONFIG_REG stConfig;
	CSCAN_ARS200FPGA_CH_GATE_REG stGate;
	float fRange;			//����
	int nVelocity;			//����
//	WORD wWorkMode;			//����ģʽ
	WORD wProbeBand;		//̽ͷƵ��
	WORD wVoltage;			//��ѹ
	WORD wImpedance;		//�迹
	float fPlus;			//����
	float fRefPlus;			//�ο�����
	WORD wEnerge;           //�ߵ������л���0Ϊ��������1Ϊ������,��ͨ��   yachang.huang add on 2013-11-22
	WORD wRevAndSendMode;   //�շ�ģʽ�л���0Ϊһ��һ��ģʽ��1Ϊ�Է�����ģʽ yachang.huang add on 2013-11-22
	WORD wSPIAddr;          //SPI��ַѡ�� ����D/A 8·ͨ��ѡ�� ����1-8����   yachang.huang add on 2013-11-22
	WORD wSPIData;          //0x0000~0x3FFF������0x0000Ϊ-6V��0x3FFFΪ+6V   yachang.huang add on 2013-11-22
	float fPlusAddX11;      //���油�� X11   yachang.huang add on 2014-04-16
	float fPlusAddX12;      //���油�� X12   yachang.huang add on 2014-04-16
	float fPlusAddX21;      //���油�� X21   yachang.huang add on 2014-04-16
	float fPlusAddX22;      //���油�� X22   yachang.huang add on 2014-04-16
}CSCAN_ARS200FPGA_SETONE;

typedef struct _ARS200FPGA_CH_FEATURE_REG{
	WORD wGate1SyncTime;			//բ��1ͬ��ʱ��
   	WORD wGate1MPFirstTime;			//բ��1����󲨷����ֵʱ��
  	WORD wGate1MPTime;				//բ��1����󲨷�ֵʱ��
    WORD wGate1MPAmp	;			//բ��1����󲨷�ֵ����
    WORD wGate1FPFirstTime;			//բ��1�ڵ�һ���������ֵʱ��
    WORD wGate1FPTime;				//բ��1�ڵ�һ�����岨��ֵʱ��
    WORD wGate1FPAmp;				//բ��1�ڵ�һ�����岨��ֵ����
    WORD wGate2SyncTime;			//բ��2ͬ��ʱ��
   	WORD wGate2MPFirstTime;			//բ��2����󲨷����ֵʱ��
  	WORD wGate2MPTime;				//բ��2����󲨷�ֵʱ��
    WORD wGate2MPAmp	;			//բ��2����󲨷�ֵ����
    WORD wGate2FPFirstTime;			//բ��2�ڵ�һ���������ֵʱ��
    WORD wGate2FPTime;				//բ��2�ڵ�һ�����岨��ֵʱ��
    WORD wGate2FPAmp;				//բ��2�ڵ�һ�����岨��ֵ����
    WORD wGate3SyncTime;			//բ��3ͬ��ʱ��
   	WORD wGate3MPFirstTime;			//բ��3����󲨷����ֵʱ��
  	WORD wGate3MPTime;				//բ��3����󲨷�ֵʱ��
    WORD wGate3MPAmp	;			//բ��3����󲨷�ֵ����
    WORD wGate3FPFirstTime;			//բ��3�ڵ�һ���������ֵʱ��
    WORD wsGate3FPTime;				//բ��3�ڵ�һ�����岨��ֵʱ��
    WORD wGate3FPAmp;				//բ��3�ڵ�һ�����岨��ֵ����
    WORD wGate4SyncTime;			//բ��4ͬ��ʱ��
   	WORD wGate4MPFirstTime;			//բ��4����󲨷����ֵʱ��
  	WORD wGate4MPTime;				//բ��4����󲨷�ֵʱ��
    WORD wGate4MPAmp	;			//բ��4����󲨷�ֵ����
    WORD wGate4FPFirstTime;			//բ��4�ڵ�һ���������ֵʱ��
    WORD wGate4FPTime;				//բ��4�ڵ�һ�����岨��ֵʱ��
    WORD wGate4FPAmp;				//բ��4�ڵ�һ�����岨��ֵ����
} CSCAN_ARS200FPGA_CH_FEATURE_REG;

typedef struct _CSCAN_ARS200FPGA_CH_DATA_REG
{
	WORD wMonitorReset;		//����ͨ����λ
	WORD wMonitorMReset;	//����ͨ���ֶ���λ
	WORD wFeatureReadReset;	//բ�����������ݻ������ɶ���־��λ
	WORD wMonitorFlag;		//������Ч��־
	WORD wFeatureReadFlag;	//բ�����������ݻ������ɶ���־
} CSCAN_ARS200FPGA_CH_DATA_REG;

//	cscan wangbingfu add end at 2012-1-13
//#pragma pack()

BOOL ars200fpga_alloc_io();
BOOL ars200fpga_unalloc_io();

BOOL cscan_ars200fpga_alloc_io();
BOOL cscan_ars200fpga_unalloc_io();

void send_max500(WORD cword);
void send_mcp41010(WORD cword);

#endif
