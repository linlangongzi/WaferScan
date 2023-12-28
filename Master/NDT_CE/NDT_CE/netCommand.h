#pragma once
#ifndef __COMMAND_H__

/**
 * Copyright(c) 2011 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved.
 *
 * ͨ��ָ�.
 * Filename: Command.h
 *
 *
 * @author       Wang Bingfu
 * @version      1.2         Date: 2011-12-23
 */

#define COMMAND_CURRENT_CHANNEL			0x0001	//��ǰͨ��

#define COMMAND_WORKMODE				0x0011	//����ģʽ

#define COMMAND_SEND_PARAMS				0x0101	//�������
#define COMMAND_SEND_VOLTAGE			0x0102	//��ѹ
#define COMMAND_SEND_MAIKUAN			0x0103	//����
#define COMMAND_SEND_REPEAT_FREQ		0x0104	//�ظ�Ƶ��
#define COMMAND_SEND_ZERO				0x0105	//���
#define COMMAND_SEND_QIANYAN			0x0106	//ǰ��

#define COMMAND_RECV_PARAMS				0x0201	//���ղ���
#define COMMAND_RECV_JIANBO_MODE		0x0202	//�첨ģʽ
#define COMMAND_RECV_GAIN				0x0203	//����
#define COMMAND_RECV_REF_GAIN			0x0204	//�ο�����
#define COMMAND_RECV_DELAY				0x0205	//�ӳ�
#define COMMAND_RECV_WORK_FREQ			0x0206	//����Ƶ��
#define COMMAND_RECV_ZUKANG				0x0207	//�迹

#define COMMAND_DISP_PARAMS				0x0301	//��ʾ����
#define COMMAND_DISP_SX					0x0302	//S/Xת��
#define COMMAND_DISP_RANGE				0x0303	//����
#define COMMAND_DISP_VELOCITY			0x0304	//�Ƕ�
#define COMMAND_DISP_ANGLE				0x0305	//����
#define COMMAND_DISP_RESTRAIN			0x0306	//����


#define COMMAND_GATE_PARAMS				0x0401	//բ�Ų���
#define COMMAND_GATE_ALARM_MODE			0x0402	//բ�ű���ģʽ
#define COMMAND_GATE_SWITCH				0x0403	//բ�ſ���
#define COMMAND_GATE_QIDIAN				0x0404	//բ�����
#define COMMAND_GATE_WIDTH				0x0405	//բ�ſ��
#define COMMAND_GATE_PEAK				0x0406	//բ�ŷ�ֵ

#define COMMAND_OTHER_PARAMS			0x0501	//��ɫ����
#define COMMAND_OTHER_COLOR				0x0502	//��ɫ����
#define COMMAND_OTHER_BACK_GRUOND		0x0503	//����
#define COMMAND_OTHER_VOLUMN			0x0504	//����
#define COMMAND_OTHER_STORE_MODE		0x0505	//�洢����

#define COMMAND_ALL_PARAM_SET		0x0601	//��λ������ȫ������

#define COMMAND_RESET				0x0701	//��λ����λ����

#define COMMAND_ALL_PARAM_REQ		0x1001	//��λ����ȡȫ����������
#define COMMAND_WAVE_REQ			0x1002	//ͨ��������������
#define COMMAND_VERSION_REQ			0x1003	//�汾������
#define COMMAND_WORK_MODE			0x1004	// ���ù���ģʽ
#define COMMAND_TCG_SWICH			0x1005	// ����TCG����
#define COMMAND_ASCAN_DATA_STROE_REQ 0x1006  //Aɨ���ݴ洢����  yachang.huang add on 2013-12-19

#define COMMAND_WAVE_DATA		0x2001	//��λ�����Ͳ�������
#define COMMAND_ALL_PARAM_DATA		0x2002	//��λ������ȫ������
#define COMMAND_DEFECT_DATA		0x2003	//��λ����������Ϣ
#define COMMAND_ASCAN_DATA_STROE   0x2006 //Aɨ�洢����  yachang.huang add on 2013-12-19

#define COMMAND_DATA_FAULT		0xFF01	//��ֵ���ڷ�Χ֮��

#define __COMMAND_H__
#endif

