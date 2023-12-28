#ifndef _HARD_WARE_ADDRESS_H_
#define _HARD_WARE_ADDRESS_H_

const DWORD PROBEBOX_UP_DOWN = 0x110;		//̽ͷ�������־  wangwei add on 2013-12-9
const DWORD START_FLAG_ADDRESS = 0x11c;  //̽�˿�ʼ/������ʶλ
const DWORD HARDWARE_VERSION_ADDRESS = 0x0;	// �汾��Ϣ
const DWORD SYS_RESET_ADDRESS = 0x2;		// ϵͳ��λ�ź�
const DWORD FEATURE_BUFFER_RESET_ADDRESS = 0x4;	// �������������ݸ�λ����
const DWORD MONITOR_DATA_RESET_ADDRESS = 0x6;	// �������ݸ�λ����
const DWORD MONITOR_CHANNEL_H_ADDRESS = 0x8;		// ��������Ӳͨ��ѡ��Ĵ���
const DWORD MONITOR_CHANNEL_S_ADDRESS = 0xA;		// ����������ͨ��ѡ��Ĵ���
const DWORD MONITOR_CHANNEL_END_ADDRESS = 0xB;				// ���ڵ�����ͨ��ѡ��Ĵ���
const DWORD MONITOR_CHANNEL_DATA_EN = 0xC;		// ����ͨ��������Ч��־�Ĵ���
const DWORD MONITOR_FEATURE_DATA_ADDRESS = 0xE;		// ����ͨ�������׵�ַ
const DWORD MONITOR_FEATURE_DATA_END_ADDRESS = 0x1D;	// ����ͨ������β��ַ
const DWORD AEC_CARD_SELFTEST_RESULT_ADDRESS = 0x1E;		// ���Լ���
const DWORD MONITOR_RANGE_ADDRESS = 0x00400;		// ����ͨ���������RAM��512��
const DWORD MONITOR_WAVE_DATA_ADDRESS = 0x00800;	// ����ͨ����������RAM��512��
const DWORD SYS_SELF_TEST_ADDRESS = 0x00C00;	// ϵͳ�Լ�SRAM��512��
const DWORD SYS_SELF_TEST_ADDRESS_END = 0x00FFF;	// ϵͳ�Լ�SRAMβ��ַ
const DWORD LOCAL_BUS_ADDRESS = 0x00200;		// LocalBus��������RAM��256��
const DWORD FEATURE_BUFFER_RESET_FLAG_ADDRESS = 0x10000;// �������ݻ��������ݱ�־λ��λ�Ĵ���
const DWORD FEATURE_BUFFER_EN_FLAG_ADDRESS = 0x10002;	// �������ݻ�����������Ч��־�Ĵ���
const DWORD FEATURE_BUFFER_INDEX_ADDRESS = 0x10004;		// �������ݻ���������buffer��ű�־�Ĵ���
const DWORD FEATURE_BUFFER1_ADDRESS = 0x20000;	// �������ݻ�����1��32K��
const DWORD FEATURE_BUFFER2_ADDRESS = 0x30000;	// �������ݻ�����2��32K��
// Ӳͨ��1����ַ
// ��ͨ��1����ַ
const DWORD SOFT_CHANNEL_CONFIG_PARARM_ADDRESS = 0x01200;	// ��ͨ��1���ò�������ַ(64)
const DWORD SOFT_CHANNEL_CONFIG_PARARM_END_ADDRESS = 0x0125B;	// ��ͨ��1���ò���β��ַ(64)
// ��ͨ��1���ò���β��ַ
const DWORD SOFT_CHANNEL1_TCG_DATA_ADDRESS = 0x01400;		// ��ͨ��1 TCG���߼Ĵ���(256)
const DWORD SOFT_CHANNEL1_TCG_DATA_END_ADDRESS = 0x015FF;

// �๦�ܿ�
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