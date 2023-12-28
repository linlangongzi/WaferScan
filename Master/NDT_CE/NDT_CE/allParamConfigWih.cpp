/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���ղ������ô���Դ�ļ�
 * Filename: allParamConfigWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"

#include "resource.h"
#include "interfaceCmdID.h"

#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"

HWND hWndAllParamCfgCaption = NULL;//���ղ������ñ��ⴰ�ھ��
HWND hWndAllParamCfg = NULL;//���ղ������ô��ھ��



//�����ܰ�ť�������
static HWND hBtnAllParamCfgGongYiCanShu = NULL;//���ղ�����ť���
static HWND hBtnAllParamCfgVoltage = NULL;//��ѹ��ť���
static HWND hBtnAllParamCfgMaiKuan = NULL;//����ť���
static HWND hBtnAllParamCfgRate = NULL;//�ظ�Ƶ��ť���
static HWND hBtnAllParamCfgAmend = NULL;//������ť���
static HWND hBtnAllParamCfgQianYan = NULL;//̽ͷǰ�ؾ��
static HWND hBtnAllParamCfgJianBo = NULL;//�첨��ʽ��ť���
static HWND hBtnAllParamCfgPlus = NULL;//���水ť���
static HWND hBtnAllParamCfgRefPlus = NULL;//�ο����水ť���
static HWND hBtnAllParamCfgDelay = NULL;//�ӳٰ�ť���
static HWND hBtnAllParamCfgLvBo = NULL;//����Ƶ����ť���
static HWND hBtnAllParamCfgRefractMode = NULL;//S/Xת����ť���
static HWND hBtnAllParamCfgSpeedType = NULL;//���ٲ��ϰ�ť���
static HWND hBtnAllParamCfgSpeed = NULL;//���ٰ�ť���
static HWND hBtnAllParamCfgRange = NULL;//���̰�ť���
static HWND hBtnAllParamCfgAngle = NULL;//����ǰ�ť���
static HWND hBtnAllParamCfgKValue = NULL;//Kֵ��ť���
static HWND hBtnAllParamCfgBate = NULL;//���ư�ť���
static HWND hBtnAllParamCfgTimeYear = NULL;//��ʱ�䰴ť���
static HWND hBtnAllParamCfgTimeMonth = NULL;//��ʱ�䰴ť���
static HWND hBtnAllParamCfgTimeDay = NULL;//��ʱ�䰴ť���
static HWND hBtnAllParamCfgTimeHour = NULL;//Сʱʱ�䰴ť���
static HWND hBtnAllParamCfgTimeMinute = NULL;//����ʱ�䰴ť���
static HWND hBtnAllParamCfgTimeSecond = NULL;//��ʱʱ�䰴ť���
static HWND hBtnAllParamCfgGongJianHouDu = NULL;//Bɨ������Ȱ�ť���
static HWND hBtnAllParamCfgScanMode = NULL;//Bɨɨ�跽ʽ��ť���
static HWND hBtnAllParamCfgBScanOnOrOff = NULL;//Bɨ���ذ�ť���
static HWND hBtnAllParamCfgPlateHeight = NULL;//�к��̽�˹�����Ȱ�ť���
static HWND hBtnAllParamCfgProbeType = NULL;//�к��̽��̽ͷ�ͺŰ�ť���
static HWND hBtnAllParamCfgUpBlindValue = NULL;//�к��̽����ä����ť���
static HWND hBtnAllParamCfgDownBlindValue = NULL;//�к��̽����ä����ť���
static HWND hBtnAllParamCfgZhongHouBanTanShangSensitive = NULL;//�к��̽�˼�������Ⱦ��
static HWND hBtnAllParamCfgPoKouType = NULL;//����̽���¿����;��
static HWND hBtnAllParamCfgPoKouAngle = NULL;//����̽���¿ڽǶȾ��
static HWND hBtnAllParamCfgPoKouDeep = NULL;//����̽���¿���Ⱦ��
static HWND hBtnAllParamCfgHanFengTanShangHouDu = NULL;//����̽�˹�����Ⱦ��
static HWND hBtnAllParamCfgHanFengTanShangDistance = NULL;//����̽��̽ͷ���¿ڵľ�����
static HWND hBtnAllParamCfgHanFengTanShangOnOrOff = NULL;//����̽�˿��ؾ��
static HWND hBtnAllParamCfgLiangDu = NULL;//���Ȱ�ť���
static HWND hBtnAllParamCfgZuKangPiPei = NULL;//�迹ƥ����
static HWND hBtnAllParamCfgQuXianFenDuan = NULL;//���߷ֶο��ؾ��
static HWND hBtnAllParamCfgPinPuFenXi = NULL;//Ƶ�׷������
static HWND hBtnAllParamCfgXingNengJiaoYan = NULL;//����У����
static HWND hBtnAllParamCfgJuLiZuoBiao = NULL;//����������
static HWND hBtnAllParamCfgGuanCaiWaiJing = NULL;//�ܲ��⾶���
static HWND hBtnAllParamCfgGuanCaiNeiJing = NULL;//�ܲ��ھ����
static HWND hBtnAllParamCfgXiangYingSpeed = NULL;//��Ӧ�ٶȾ��
static HWND hBtnAllParamCfgBiaoMianBuChang = NULL;//���油�����
static HWND hBtnAllParamCfgPrintType = NULL;//��ӡ���;��
static HWND hBtnAllParamCfgLieWenQiBo = NULL;//�����𲨾��
static HWND hBtnAllParamCfgPingDing = NULL;//�������
static HWND hBtnAllParamCfgDingLing = NULL;//�������
static HWND hBtnAllParamCfgPanFei = NULL;//�зϾ��
static HWND hBtnAllParamCfgTanTouType = NULL;//̽ͷ���;��
static HWND hBtnAllParamCfgCanShuQinLing = NULL;//����������
static HWND hBtnAllParamCfgTongDaoQinLing = NULL;//ͨ��������
static HWND hBtnAllParamCfgCanShuSuoDing = NULL;//�����������
static HWND hBtnAllParamCfgQuXianGuiGe1 = NULL;//���ߦ�1ֵ���
static HWND hBtnAllParamCfgQuXianGuiGe2 = NULL;//���ߦ�2ֵ���
static HWND hBtnAllParamCfgQuXianGuiGe3 = NULL;//���ߦ�3ֵ���
static HWND hBtnAllParamCfgZiDongBoGao = NULL;//�Զ����߾��
static HWND hBtnAllParamCfgPlusType = NULL;// = NULL���淽ʽ���
static HWND hBtnAllParamCfgWorkType = NULL;//������ʽ���
static HWND hBtnAllParamCfgHardWare = NULL;//Ӳ���汾��
static HWND hBtnAllParamCfgSoftWare = NULL;//����汾��
static HWND hBtnAllParamCfgEquipmentModel = NULL;//�豸�ͺ�
static HWND hBtnAllParamCfgNextAdjustTime = NULL;//�´�У׼ʱ����
static HWND hBtnAllParamCfgThisAdjustTime = NULL;//����У׼ʱ����


//�����ܰ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgGongYiCanShuProc = NULL;//���ղ�����ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgVoltageProc = NULL;//��ѹ��ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgMaiKuanProc = NULL;//����ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgRateProc = NULL;//�ظ�Ƶ�ʰ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgAmendProc = NULL;//������ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgQianYanProc = NULL;//̽ͷǰ��Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgJianBoProc = NULL;//�첨��ʽ��ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgPlusProc = NULL;//���水ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgRefPlusProc = NULL;//�ο����水ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgDelayProc = NULL;//�ӳٰ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgLvBoProc = NULL;//�˲�Ƶ����ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgRefractModeProc = NULL;//S/Xת����ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgSpeedTypeProc = NULL;//���ٲ��ϰ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgSpeedProc = NULL;//���ٰ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgRangeProc = NULL;//���̰�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgAngleProc = NULL;//����ǰ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgKValueProc = NULL;//Kֵ��ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgBateProc = NULL;//���ư�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgTimeYearProc = NULL;//��ʱ�䰴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgTimeMonthProc = NULL;//��ʱ�䰴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgTimeDayProc = NULL;//��ʱ�䰴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgTimeHourProc = NULL;//Сʱʱ�䰴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgTimeMinuteProc = NULL;//����ʱ�䰴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgTimeSecondProc = NULL;//��ʱ�䰴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgGongJianHouDuProc= NULL;//Bɨ������Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgScanModeProc= NULL;//Bɨɨ�谴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgBScanOnOrOffProc= NULL;//Bɨɨ�谴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgPlateHeightProc= NULL;//�к��̽�˹�����Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgProbeTypeProc= NULL;//�к��̽��̽ͷ���Ͱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgUpBlindValueProc= NULL;//�к��̽����ä����ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgDownBlindValueProc= NULL;//�к��̽��ä�����Ͱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgZhongHouBanTanShangSensitiveProc= NULL;//�к��̽��������Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgPoKouTypeProc= NULL;//����̽���¿����Ͱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgPoKouAngleProc= NULL;//����̽���¿ڽǶȰ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgPoKouDeepProc= NULL;//����̽���¿���Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgHanFengTanShangHouDuProc= NULL;//����̽�˹�����Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgHanFengTanShangDistanceProc= NULL;//����̽��̽ͷ���¿ڵľ��밴ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgHanFengTanShangOnOrOffProc= NULL;//����̽�˿��ذ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgLiangDuProc= NULL;//���Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnAllParamCfgZuKangPiPeiProc= NULL;//�迹ƥ��Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgQuXianFenDuanProc= NULL;//�迹ƥ��Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgPinPuFenXiProc= NULL;//Ƶ�׷���Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgXingNengJiaoYanProc= NULL;//Ƶ�׷���Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgJuLiZuoBiaoProc= NULL;//��������Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgGuanCaiWaiJingProc= NULL;//�ܲ��⾶Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgGuanCaiNeiJingProc= NULL;//�ܲ��ھ�Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgXiangYingSpeedProc= NULL;//��Ӧ�ٶ�Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgBiaoMianBuChangProc= NULL;//��Ӧ�ٶ�Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgPrintTypeProc= NULL;//��ӡ����Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgLieWenQiBoProc = NULL;//������Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgPingDingProc = NULL;//����Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgDingLiangProc = NULL;//����Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgPanFeiProc = NULL;//�з�Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgTanTouTypeProc = NULL;//̽ͷ����Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgCanShuQinLingProc = NULL;//��������Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgTongDaoQinLingProc = NULL;//ͨ������Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgCanShuSuoDingProc = NULL;//��������Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgQuXianGuiGe1Proc = NULL;//���ߦ�1ֵĬ�ϴ������
static WNDPROC OldBtnAllParamCfgQuXianGuiGe2Proc = NULL;//���ߦ�2ֵĬ�ϴ������
static WNDPROC OldBtnAllParamCfgQuXianGuiGe3Proc = NULL;//���ߦ�3ֵĬ�ϴ������
static WNDPROC OldBtnAllParamCfgZiDongBoGaoProc = NULL;//�Զ�����Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgPlusTypeProc = NULL;//��������Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgWorkTypeProc = NULL;//������ʽĬ�ϴ������
static WNDPROC OldBtnAllParamCfgHardWareProc = NULL;//Ӳ���汾��Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgSoftWareProc = NULL;//Ӳ���汾��Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgEquipmentModelProc = NULL;//�豸�ͺ�Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgNextAdjustTimeProc = NULL;//�´�У׼ʱ��Ĭ�ϴ������
static WNDPROC OldBtnAllParamCfgThisAdjustTimeProc = NULL;//����У׼ʱ��Ĭ�ϴ������


static HWND allParamCfgBtnList[49] ={NULL};//���ղ������ô��ڵİ�ť��
WORD wChannelNo = 0; 	//Ҫ���õ�ͨ����
HWND currentAllParamCfgBtn  = NULL;//���ղ������ô����е�ǰѡ�еİ�ť

//�������Զ��崰�ڹ��̺���

static LRESULT CALLBACK allParamCfgCaptionChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK allParamCfgChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int allParamCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnAllParamCfgGongYiCanShuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgVoltageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgMaiKuanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgRateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgAmendProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgQianYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgJianBoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgPlusProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgRefPlusProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgDelayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgLvBoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgRefractModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgSpeedTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgRangeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgAngleProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgKValueProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgBateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamCfgTimeYearProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgTimeMonthProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgTimeDayProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgTimeHourProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgTimeMinuteProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgTimeSecondProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);

static LRESULT CALLBACK btnAllParamCfgGongJianHouDuProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgScanModeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgBScanOnOrOffProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPlateHeightProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgProbeTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgUpBlindValueProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgDownBlindValueProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgZhongHouBanTanShangSensitiveProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);

static LRESULT CALLBACK btnAllParamCfgPoKouTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPoKouAngleProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPoKouDeepProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgHanFengTanShangHouDuProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgHanFengTanShangDistanceProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgHanFengTanShangOnOrOffProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgLiangDuProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgZuKangPiPeiProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgQuXianFenDuanProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPinPuFenXiProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgXingNengJiaoYanProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgJuLiZuoBiaoProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgGuanCaiWaiJingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgGuanCaiNeiJingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgXiangYingSpeedProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgBiaoMianBuChangProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPrintTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgLieWenQiBoProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPingDingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgDingLiangProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPanFeiProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgTanTouTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgCanShuQinLingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgTongDaoQinLingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgCanShuSuoDingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgQuXianGuiGe1Proc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgQuXianGuiGe2Proc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgQuXianGuiGe3Proc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgZiDongBoGaoProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgPlusTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgWorkTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgHardWareProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgSoftWareProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgEquipmentModelProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgNextAdjustTimeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK btnAllParamCfgThisAdjustTimeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam);


static int buttonNum = 50;//��ť�ؼ���
static float fStep = 0;			//����
//songchenguang add start 2010-08-14
static int nButtonIndex = 0;
//songchenguang add end 2010-08-14

extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;//�����ھ��

//extern HWND hWndCraftParam;
//extern HWND currentCraftParamBtn;
extern HWND hStatusBar;
extern WORD CurrentChannelNo;//��ǰͨ��
extern  CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���
extern WORD gnParamNo;							//���ղ������
extern SEND_PARAM_DATA SendParam_ad[2];//��������ṹ��
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];//���ղ����ṹ��
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];//��ʾ�����ṹ��
extern float MIN_DELAY[2];
extern float MAX_DELAY;
extern SYS_INFO gSysInfo;
// songchenguang add start 2010-12-14
SYS_INFO gAllcfgSysInfo;
// songchenguang add end 2010-12-14
extern ARS200FPGA_SYS_REG* sys_reg;
extern ARS200FPGA_SYS_REG* sys_reg;

extern TCHAR FileDirectory[MAX_PATH];//�ļ�����·��
//extern WORD nParamNo;//�ļ����
extern BOOL gbHotCraftParamView;//���ּ����Ʒ�
extern BOOL ReadParamFile(LPCTSTR lpszPathName);//�ļ���ȡ����



//zhaoli add 2010- 9- 3
extern DWORD gFunctionDefine;//���ܶ��忪��

extern float fBScanGangBanHouDu;//������Ȳ���
extern WORD wScanMode;//Bɨɨ�跽ʽ
extern BOOL gbBscanSwitch;//Bɨ����
extern float gfPlateHeight;//�к��̽�˹������
extern WORD nProbeType;//�к��̽��̽ͷ����
extern float gfUpBlindValue;//�к��̽����ä��
extern float gfDownBlindValue;//�к��̽����ä��
extern WORD gwPoKouLeiXing;//����̽���¿���������
extern float  gfPoKouJiaoDu;//����̽���¿ڽǶ�����
extern float  gfPoKouShenDu;//����̽���¿��������
extern BOOL bHanFengTanShangSwitch;//����̽�˿���
extern float fHanFengTanShangGangBanHouDu;//����̽�˹������
extern float fHead2HanFengDistance;//̽ͷ���¿ڵľ���
extern BOOL nLCDValue;//��������

extern float gfWaiJing;						//�ܲ��⾶
extern float gfNeiJing;					//�ܲ��ھ�
extern float gfPingDingGain[CHANNEL_NUM];				//��������
extern float gfDingLiangGain[CHANNEL_NUM];				//��������
extern float gfPanHuiGain[CHANNEL_NUM];				//�з�����
extern float gfDefectZhi1[CHANNEL_NUM];			//����׾���1
extern float gfDefectZhi2[CHANNEL_NUM];			//��2
extern float gfDefectZhi3[CHANNEL_NUM];			//��3

extern float HARDWARE_VER;//Ӳ���汾��
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22
//zhaoli add in 2010-9-16
float allParamBScanGongJianHouDu = 0.0f;//Bɨ�������
WORD allParamScanMode = 0;//Bɨɨ�跽ʽ
BOOL allParamBscanSwitch = FALSE;//Bɨ����
float allParamPlateHeight = 1.0f;//�к��̽�˹������
WORD allParamProbeType = 0;//�к��̽��̽ͷ���Ͳ�
float allParamUpBlindValue = 0.0f;//�к��̽����ä��
float allParamDownBlindValue = 0.0f;//�к��̽����ä��
WORD allParamPoKouLeiXing = 0;//����̽���¿���������
float allParamPoKouJiaoDu = 1.0f;//����̽���¿ڽǶ�����
float  allParamPoKouShenDu = 1.0f;//����̽���¿��������
BOOL allParamHanFengTanShangSwitch = FALSE;//����̽�˿���
float allParamHanFengTanShangGangBanHouDu = 0.0f;//����̽�˹������
float allParamHead2HanFengDistance = 1.0f;//̽ͷ���¿ڵľ���
int allParamLCDValue = 7;//��������


//zhaoli add in 2010-9-16



SYSTEMTIME st;//ϵͳʱ��

//allParamConfigWithTime AllParam_Time;//���ղ���ʱ��


SEND_PARAM_DATA Send_AllParamConfigWith_ad[2];
RECEIVE_PARAM_DATA Receive_AllParamConfigWith_ad[2];
DISPLAY_SET_DLG_DATA Display_AllParamConfigWith_ad[2];

static WORD wAllCfgVoltageType = 0;
static WORD wMaterialType = 1;				//��������
static float fKValue = 0;					//Kֵ
static WORD wUserDefineWaveSpeed = 6000;  		//�Զ�������
//static WORD wAllCfgQianYan = 0;	//̽ͷǰ��

//���ؼ�������ʼ������
const WORD nMaiKuanStep = 10;
const WORD nRepeatFreqStep = 10;
static float fAmendStep = 10;
static float fParamQianYanStep = 10;
static float fPlusStep = 0.1;//PLUSSTEPS[0];
static float fDelayStep = 0.1;//AMENDSTEPS[0];
static WORD nLvBoStep = 1;//LVBOLITTLEMODIFY[0];
static float fRangeStep = 1;
static WORD nWaveSpeedStep = 100;
static float fZheSheAngleStep = 1;
static float fKValueStep = 1;
static const WORD nBateStep = 1;
//zhaoli add 9-3
static float fGongJianHouDuStep = 1.0;//Bɨ�������Ĭ�ϲ���
static float PlateHeightStep = 0.1;//�к��̽�˹������Ĭ�ϲ���
static float BlindValueStep = 0.1;//�к��̽������ä��Ĭ�ϲ���
static float PoKouAngleStep = 0.1;//����̽���¿ڽǶ�Ĭ�ϲ���
static float PoKouDeepStep = 0.1;//����̽���¿����Ĭ�ϲ���
static float HanFengTanShangHouDuStep = 0.1;//����̽�˹������Ĭ�ϲ���
static float HanFengTanShangDistanceStep = 0.1;//����̽��̽ͷ���¿ڵľ���Ĭ�ϲ���
//zhaoli add 9-7
static float fNeiJingStep = 1.0f;				//�ܲ��ھ�����
static float fWaiJingStep = 1.0f;				//�ܲ��⾶����
static float fParamCurveGainStep = 1.0f;			//����/����/�зϲ���
static float fParamFaiStep = 1.0f; 				//��ֵ����


static WORD nQianYanStepIndex = 0;				// ��ǰǰ�ز������
static WORD nAmendStepIndex = 0;				// ��ǰ�����������
static WORD nPlusStepIndex = 0;					// ��ǰ���沽�����
static WORD nDelayStepIndex = 0;				// ��ǰ��ʱ�������
static WORD nRangeStepIndex = 0;				// ��ǰ���̲������
static WORD nWaveSpeedStepIndex = 0;			// ��ǰ���ٲ������
static WORD nZheSheAngleStepIndex = 0;			// ��ǰ����ǲ������
static WORD nKValueStepIndex = 0;				// ��ǰKֵ�������

//zhaoli add 9-3
static WORD fGongJianHouDuIndex = 0;			// ��ǰBɨ������Ȳ������
static WORD PlateHeightIndex = 0;				// ��ǰ�к��̽�˹�����Ȳ������
static WORD PoKouAngleIndex = 0;				//��ǰ����̽���¿ڽǶȲ������
static WORD PoKouDeepIndex = 0;					//��ǰ����̽���¿���Ȳ������
static WORD HanFengTanShangHouDuIndex = 0;		//��ǰ����̽�˹�����Ȳ������
static WORD HanFengTanShangDistanceIndex = 0;	//��ǰ����̽��̽ͷ���¿ڵľ��벽�����
static WORD wBlindAreaStepIndex = 0;			//ä����������
//zhaoli add 9-3
static WORD wWaiJingStepIndex = 1;				//�ܲ��⾶��������
static WORD wNeiJingStepIndex = 1;

//zhaoli add 9-8
//WORD gwZuKangPiPei = 100;//�迹ƥ������
WORD gwGongYiCanShu = 1;
BOOL gbZuKangPiPei = FALSE;
BOOL gbQuXianFenDuan = FALSE;//���߷ֶ�
BOOL gbPinPuFenXi = FALSE;//Ƶ�׷���
BOOL gbXingNengJiaoYan = FALSE;//����У��
WORD gwJuLiZuoBiao = 0;//��������
float gfParamGuanCaiWaiJing = 0.0;//�ܲ��⾶
float gfGuanCaiNeiJing = 0.0;//�ܲ��ھ�
int gnXiangYingSpeed = 0;//��Ӧ�ٶ�
WORD gwBiaoMianBuChang = 0;//���油��
int gnPrintType = 0;//��ӡ����
WORD gwLieWenQiBo = 0;//������
float gfParamPingDing[CHANNEL_NUM] = {0, 0}; //��������
float gfParamDingLiang[CHANNEL_NUM] = {0, 0};//��������
float gfParamPanFei[CHANNEL_NUM] = {0, 0};//�з�����
WORD nTanTouType[CHANNEL_NUM]= {1, 1};//̽ͷ����
// songchenguang add start 2010-12-09
CRITICAL_SECTION csProbeType;
// songchenguang add end 2010-12-09

BOOL gbCanShuQinLing = FALSE;//�������㰴ťֵ
BOOL gbTongDaoQinLing = FALSE;//ͨ�����㰴ťֵ
BOOL gbCanShuSuoDing = FALSE;//��������
float gfParamFaiZhi1[CHANNEL_NUM] = {2, 2};//���ߦ�1ֵ
float gfParamFaiZhi2[CHANNEL_NUM] = {0, 0};//���ߦ�2ֵ
float gfParamFaiZhi3[CHANNEL_NUM] = {0, 0};//���ߦ�3ֵ
WORD gwZiDongBoGao =0;//�Զ����߲���
WORD gwPlusType = 0;//�������ͱ���
int gnWorkType = 1;//������ʽ����
//zhaoli add 9-8

extern HWND hWndAllParamConfirm;//���ղ�������ȷ�ϴ��ھ��
extern HWND currentAllParamConfirmBtn; //���ղ�������ȷ�ϴ��е�ǰѡ�еİ�ť

extern TCHAR ParamFileName[MAX_PATH];
extern CRITICAL_SECTION csSysInfo;
extern GATE_INFO GateInfos[2][3];
extern CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//��ӵ��������ݶ���
extern DWORD gSelectedDacPointDataNum[CHANNEL_NUM];					//��ѡ�е�DAC������
extern float gfTcgFirstPointGain[CHANNEL_NUM];		//��ӵĵ��һ�����Ӧ��Dbֵ
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];				//DGSͨ������
extern float gfDacFirstPointGain[CHANNEL_NUM];			//DAC��һ������ֵ
extern float gfOldDacRange[CHANNEL_NUM];				//�������DACʱ������
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DGS������
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//��ѡ�е�AVG������
extern float gfAvgFirstPointGain[CHANNEL_NUM]; 	//��ӵĵ��һ�����Ӧ��Dbֵ
extern float gfOldAvgRange[CHANNEL_NUM];				//�������AVGʱ������
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������AVGʱ��ÿ�����λ��

//extern DWORD gSelectedDgsPointDataNum[CHANNEL_NUM][GATE_NUM];//��ѡ�е�DGS������
//extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][GATE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
BOOL gAllCfgDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{0,0},{0,0}};				//DGSͨ������
float gfAllCfgOldRange[CHANNEL_NUM] = {0,0};				//�������DACʱ������
WORD gwAllCfgOldSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM] = {{0,0},{0,0}};	//�������DACʱ��ÿ�����λ��
float fAllCfgOldRange[CHANNEL_NUM] = {0,0};			//�ն�ȡʱ������
float fAllCfgOldPlus[CHANNEL_NUM] = {0,0};			//�ն�ȡʱ������
float fAllCfgOldRefPlus[CHANNEL_NUM] = {0,0};		//�ն�ȡʱ�Ĳο�����
float fAllCfgOldDelay[CHANNEL_NUM] = {0.0f, 0.0f};		//�ն�ȡʱ���ӳ�ֵ

float gfAllOldRangeStart[CHANNEL_NUM] = {0.0f, 0.0f};		//���湤�ղ���֮ǰ����ʼ����
float gfAllOldRangeEnd[CHANNEL_NUM] = {0.0f, 0.0f};			//���湤�ղ���֮ǰ���յ�����

extern float fRangeStart[2];
extern float fRangeEnd[2];

extern DWORD currentUsedParamNo;		//��ǰʹ�õĹ��ղ�����

DWORD gAllCfgSelectedDgsPointDataNum[CHANNEL_NUM][GATE_NUM] = {{0,0},{0,0}};//��ѡ�е�DGS������
CURVEPOINT_DATA gAllCfgSelectedDgsPointData[CHANNEL_NUM][GATE_NUM][MAX_CURVE_POINT_NUM] = {{0,0,0},{0,0,0}};//��ӵ��������ݶ���

extern wchar_t gszTextStep[10];
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern WORD gwDeviceTypeValue;
extern DWORD gFunctionDefine;

extern float gfOldTcgRange[CHANNEL_NUM];				//�������TCGʱ������
extern short gwOldTcgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������TCGʱ��ÿ�����λ��

extern WORD gwDeviceType;	//�豸���� 0:̽����	1:����� 2:��·ר��

extern void CheckChannelParam(CRAFT_PARAM& chParam, const WORD& chNo);
extern WORD typeToVoltage(WORD wType);
extern WORD VoltageToType(WORD wVoltage);
extern void SetPressLableAndExecute(HWND hWnd);

extern void SetJianBoModeLableAndExecute(HWND hWnd);
extern void SetMaiKuanLableAndExecute(HWND hWnd);
extern void SetReFreqLableAndExecute(HWND hWnd);
extern void SetAmendLableAndExecute(HWND hWnd);
extern void SetDelayLableAndExecute(HWND hWnd, const WORD& nChNo);
extern void SetLvBoLableAndExecute(HWND hWnd, const WORD& chNo);
extern WORD typeToWaveSpeed(WORD wType);
extern WORD waveSpeedToType(WORD wWaveSpeed);

WORD allCfgwaveSpeedToType(WORD wWaveSpeed);//��������

//��·
extern WORD gwStandard[CHANNEL_NUM];	//����׼
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//��ⲿλ
extern float gfTlGainBuChang[CHANNEL_NUM];//���油��
extern BOOL gbTlSheDingFlag[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ�Ƿ����趨��־
extern float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//����׼����λ����
extern float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//����׼����λ����
extern float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ��������

//�ļ���ȡ
BOOL allCfgReadParamFile(LPCTSTR lpszPathName)
{
	CRAFT_PARAM ch1Param;
	CRAFT_PARAM ch2Param;					

	HANDLE hFileWrite;	//�ļ����
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��PAR�ļ�ʧ��"));
		return FALSE;
	}

	
	if(!ReadFile(hFileWrite,&gAllcfgSysInfo, sizeof(SYS_INFO),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT,
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	if(!ReadFile(hFileWrite,&ch1Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));
		
		CloseHandle(hFileWrite);
		return FALSE;
	}

	if(!ReadFile(hFileWrite,&ch2Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

//	Send_AllParamConfigWith_ad[0] = ch1Param.SendParam;
//	Receive_AllParamConfigWith_ad[0] = ch1Param.ReceiveParam;
//	Display_AllParamConfigWith_ad[0] = ch1Param.DisplayParam;

	CheckChannelParam(ch1Param, 0);
	CheckChannelParam(ch2Param, 1);

	// ��ֵ�����
	//memcpy(&Send_AllParamConfigWith_ad[0], &(ch1Param.SendParam), sizeof(SEND_PARAM_DATA));	
	//memcpy(&Receive_AllParamConfigWith_ad[0], &(ch1Param.ReceiveParam), sizeof(RECEIVE_PARAM_DATA));
	//memcpy(&Display_AllParamConfigWith_ad[0], &(ch1Param.DisplayParam), sizeof(DISPLAY_SET_DLG_DATA));
	
	Send_AllParamConfigWith_ad[0] = ch1Param.SendParam;
	Receive_AllParamConfigWith_ad[0] = ch1Param.ReceiveParam;
	Display_AllParamConfigWith_ad[0] = ch1Param.DisplayParam;

	//memcpy(GateInfos[0], ch1Param.GateInfos, sizeof(GATE_INFO)*3);
	
	if(gAllcfgSysInfo.wWorkMode == SingleChannelA || gAllcfgSysInfo.wWorkMode == SingleChannelB)
	{
		gnWorkType = 1;
	}
	else
	{
		gnWorkType = (int)gAllcfgSysInfo.wWorkMode;
		if(gnWorkType == 4 || gnWorkType == 5 || gnWorkType == 6)
				wChannelNo = 1;
		
	}
	
	//TCG
	gSelectedDacPointDataNum[0] = ch1Param.SelectedDacPointDataNum;
	gfTcgFirstPointGain[0] = ch1Param.fTcgFirstGain;
	memcpy(gSelectedDacPointData[0], ch1Param.SelectedDacPointData, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	gfOldTcgRange[1]  = ch2Param.fOldTcgRang;
	for(WORD j = 0; j < ch2Param.SelectedDacPointDataNum; j++)
	{
		gwOldTcgSerial[1][j] = ch2Param.wOldTcgSerial[j];
	}
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		gDgsSwitch[0][j] = ch1Param.dgsCurve.bSwitch[j];
	}

	gfPingDingGain[0] = ch1Param.dgsCurve.fPingDing;
	gfDingLiangGain[0] = ch1Param.dgsCurve.fDingLiang;
	gfPanHuiGain[0] = ch1Param.dgsCurve.fPanFei;
	
	gfDacFirstPointGain[0] = ch1Param.dgsCurve.fFirstGain;
	gfOldDacRange[0] = ch1Param.dgsCurve.fOldRang;
	for(WORD j = 0; j < ch1Param.dgsCurve.selectedDgsPointDataNum[1]; j++)
	{
		gwOldDacSerial[0][j] = ch1Param.dgsCurve.wOldSerial[j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		gSelectedDgsPointDataNum[0][j] = ch1Param.dgsCurve.selectedDgsPointDataNum[j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(gSelectedDgsPointData[0][j], ch1Param.dgsCurve.selectedDgsPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	gbAvgCurveFlag[0] = ch1Param.avgCurve.bSwitch;
	gfDefectZhi1[0] = ch1Param.avgCurve.fFai1 ;
	gfDefectZhi2[0] = ch1Param.avgCurve.fFai2;
	gfDefectZhi3[0] = ch1Param.avgCurve.fFai3;
	gfAvgFirstPointGain[0] = ch1Param.avgCurve.fFirstGain;
	gfOldAvgRange[0] = ch1Param.avgCurve.fOldRange;
	for(WORD j = 0; j < ch1Param.avgCurve.selectedPointDataNum; j++)
	{
		gwOldAvgSerial[0][j] = ch1Param.avgCurve.wOldSerial[j];
	}
	gSelectedAvgPointDataNum[0] = ch1Param.avgCurve.selectedPointDataNum;
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(gSelectedAvgPointData[0][j], ch1Param.avgCurve.selectedPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}


	Send_AllParamConfigWith_ad[1] = ch2Param.SendParam;
	Receive_AllParamConfigWith_ad[1] = ch2Param.ReceiveParam;
	Display_AllParamConfigWith_ad[1] = ch2Param.DisplayParam;
	//memcpy(&Send_AllParamConfigWith_ad[1], &(ch2Param.SendParam), sizeof(SEND_PARAM_DATA));
	//memcpy(&Receive_AllParamConfigWith_ad[1], &(ch2Param.ReceiveParam), sizeof(RECEIVE_PARAM_DATA));
	//memcpy(&Display_AllParamConfigWith_ad[1], &(ch2Param.DisplayParam), sizeof(DISPLAY_SET_DLG_DATA));
	//memcpy(GateInfos[1], ch2Param.GateInfos, sizeof(GATE_INFO)*3);
	//TCG
	gSelectedDacPointDataNum[1] = ch2Param.SelectedDacPointDataNum;
	gfTcgFirstPointGain[1] = ch2Param.fTcgFirstGain;
	memcpy(gSelectedDacPointData[1], ch2Param.SelectedDacPointData, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	gfOldTcgRange[1]  = ch2Param.fOldTcgRang;
	for(WORD j = 0; j < ch2Param.SelectedDacPointDataNum; j++)
	{
		gwOldTcgSerial[1][j] = ch2Param.wOldTcgSerial[j];
	}
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		gDgsSwitch[1][j] = ch2Param.dgsCurve.bSwitch[j];
	}
	
	gfPingDingGain[1] = ch2Param.dgsCurve.fPingDing;
	gfDingLiangGain[1] = ch2Param.dgsCurve.fDingLiang;
	gfPanHuiGain[1] = ch2Param.dgsCurve.fPanFei;
	
	gfDacFirstPointGain[1] = ch2Param.dgsCurve.fFirstGain;
	gfOldDacRange[1] = ch2Param.dgsCurve.fOldRang;
	for(WORD j = 0; j < ch2Param.dgsCurve.selectedDgsPointDataNum[1]; j++)
	{
		gwOldDacSerial[1][j] = ch2Param.dgsCurve.wOldSerial[j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		gSelectedDgsPointDataNum[1][j] = ch2Param.dgsCurve.selectedDgsPointDataNum[j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(gSelectedDgsPointData[1][j], ch2Param.dgsCurve.selectedDgsPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	gbAvgCurveFlag[1] = ch2Param.avgCurve.bSwitch;
	gfDefectZhi1[1] = ch2Param.avgCurve.fFai1 ;
	gfDefectZhi2[1] = ch2Param.avgCurve.fFai2;
	gfDefectZhi3[1] = ch2Param.avgCurve.fFai3;
	gfAvgFirstPointGain[1] = ch2Param.avgCurve.fFirstGain;
	gfOldAvgRange[1] = ch2Param.avgCurve.fOldRange;
	for(WORD j = 0; j < ch2Param.avgCurve.selectedPointDataNum; j++)
	{
		gwOldAvgSerial[1][j] = ch2Param.avgCurve.wOldSerial[j];
	}
	gSelectedAvgPointDataNum[1] = ch2Param.avgCurve.selectedPointDataNum;
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(gSelectedAvgPointData[1][j], ch2Param.avgCurve.selectedPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}

	if(fabs(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao) > 1E-6)
	{
		nTanTouType[wChannelNo] = 0;
		wMaterialType = allCfgwaveSpeedToType(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);
	}	
	else 
	{
		nTanTouType[wChannelNo] = 1;
		wMaterialType = allCfgwaveSpeedToType(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);

	}
		// ���ݽǶ��Ƿ�Ϊ�㣬����ֱ��б̽ͷ��־
/*	InitializeCriticalSection(&csProbeType);

	for(int i=0; i<2; i++)
	{
		EnterCriticalSection(&csProbeType);
		if(fabs(DisplaySet_ad[i].fZheSheJiao) < 1E-6)
			nTanTouType[i] = 1;
		else
			nTanTouType[i] = 0;
		LeaveCriticalSection(&csProbeType);
	}

	DeleteCriticalSection(&csProbeType);*/	


	//��ȡע�����ʾ����У׼ʱ��
	
	return TRUE;
}


BOOL allCfgSaveParamFile(LPCTSTR lpszPathName)
{
	CRAFT_PARAM ch1Param;
	CRAFT_PARAM ch2Param;

	ch1Param.SendParam = Send_AllParamConfigWith_ad[0];
	ch1Param.ReceiveParam = Receive_AllParamConfigWith_ad[0];
	ch1Param.DisplayParam = Display_AllParamConfigWith_ad[0];
//	sys_reg->wLedBrightness = allParamLCDValue;
	
	//memcpy(ch1Param.GateInfos, GateInfos[0], sizeof(GATE_INFO)*3);
	for(int i = 0; i < GATE_NUM; i++)
	{
		ch1Param.GateInfos[i] = GateInfos[0][i];
	}
	//TCG
	ch1Param.SelectedDacPointDataNum = gSelectedDacPointDataNum[0];
	ch1Param.fTcgFirstGain = gfTcgFirstPointGain[0];
	memcpy(ch1Param.SelectedDacPointData, gSelectedDacPointData[0], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	ch1Param.fOldTcgRang = gfOldTcgRange[0];
	for(WORD j = 0; j < gSelectedDacPointDataNum[0]; j++)
	{
		ch1Param.wOldTcgSerial[j] = gwOldTcgSerial[0][j];
	}
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		ch1Param.dgsCurve.bSwitch[j] = gDgsSwitch[0][j];
	}

	ch1Param.dgsCurve.fPingDing = gfPingDingGain[0];
	ch1Param.dgsCurve.fDingLiang = gfDingLiangGain[0];
	ch1Param.dgsCurve.fPanFei = gfPanHuiGain[0];
	
	ch1Param.dgsCurve.fFirstGain= gfDacFirstPointGain[0];
	ch1Param.dgsCurve.fOldRang = gfOldDacRange[0];
	for(int j = 0; j < gSelectedDgsPointDataNum[0][1]; j++)
	{
		ch1Param.dgsCurve.wOldSerial[j] = gwOldDacSerial[0][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		ch1Param.dgsCurve.selectedDgsPointDataNum[j] = gSelectedDgsPointDataNum[0][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(ch1Param.dgsCurve.selectedDgsPointData[j], gSelectedDgsPointData[0][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	ch1Param.avgCurve.bSwitch = gbAvgCurveFlag[0];
	
	ch1Param.avgCurve.fFai1 = gfDefectZhi1[0];
	ch1Param.avgCurve.fFai2 = gfDefectZhi2[0];
	ch1Param.avgCurve.fFai3 = gfDefectZhi3[0];
	
	ch1Param.avgCurve.fFirstGain = gfAvgFirstPointGain[0];
	ch1Param.avgCurve.fOldRange = gfOldAvgRange[0];
	for(int j = 0; j < gSelectedAvgPointDataNum[0]; j++)
	{
		ch1Param.avgCurve.wOldSerial[j] = gwOldAvgSerial[0][j];
	}
	ch1Param.avgCurve.selectedPointDataNum = gSelectedAvgPointDataNum[0];
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(ch1Param.avgCurve.selectedPointData[j], gSelectedAvgPointData[0][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//��·
	ch1Param.RailRoadParam.wStandard = gwStandard[0];
	ch1Param.RailRoadParam.wPosition = gwPosition[0][gwStandard[0]];
	memcpy(ch1Param.RailRoadParam.fBuChangGain, gfTlBuChangPlus[0], sizeof(ch1Param.RailRoadParam.fBuChangGain));
	memcpy(ch1Param.RailRoadParam.fRange, gfTlRange[0], sizeof(ch1Param.RailRoadParam.fRange));
	memcpy(ch1Param.RailRoadParam.fPlus, gfTlPlus[0], sizeof(ch1Param.RailRoadParam.fPlus));
	memcpy(ch1Param.RailRoadParam.bFlag, gbTlSheDingFlag[0], sizeof(ch1Param.RailRoadParam.bFlag));
	
	ch2Param.SendParam = Send_AllParamConfigWith_ad[1];
	ch2Param.ReceiveParam = Receive_AllParamConfigWith_ad[1];
	ch2Param.DisplayParam = Display_AllParamConfigWith_ad[1];
	//memcpy(ch2Param.GateInfos, GateInfos[1], sizeof(GATE_INFO)*3);
	//gAllcfgSysInfo.wWorkMode = (WORKMODE)gnWorkType;
	
	for(int i = 0; i < GATE_NUM; i++)
	{
		ch2Param.GateInfos[i] = GateInfos[1][i];
	}
	//TCG
	ch2Param.SelectedDacPointDataNum = gSelectedDacPointDataNum[1];
	ch2Param.fTcgFirstGain = gfTcgFirstPointGain[1];
	memcpy(ch2Param.SelectedDacPointData, gSelectedDacPointData[1], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	ch2Param.fOldTcgRang = gfOldTcgRange[1];
	for(WORD j = 0; j < gSelectedDacPointDataNum[1]; j++)
	{
		ch2Param.wOldTcgSerial[j] = gwOldTcgSerial[1][j];
	}
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		ch2Param.dgsCurve.bSwitch[j] = gDgsSwitch[1][j];
	}

	ch2Param.dgsCurve.fPingDing = gfPingDingGain[1];
	ch2Param.dgsCurve.fDingLiang = gfDingLiangGain[1];
	ch2Param.dgsCurve.fPanFei = gfPanHuiGain[1];
	
	ch2Param.dgsCurve.fFirstGain= gfDacFirstPointGain[1];
	ch2Param.dgsCurve.fOldRang = gfOldDacRange[1];
	for(int j = 0; j < gSelectedDgsPointDataNum[1][1]; j++)
	{
		ch2Param.dgsCurve.wOldSerial[j] = gwOldDacSerial[1][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		ch2Param.dgsCurve.selectedDgsPointDataNum[j] = gSelectedDgsPointDataNum[1][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(ch2Param.dgsCurve.selectedDgsPointData[j], gSelectedDgsPointData[1][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	ch2Param.avgCurve.bSwitch = gbAvgCurveFlag[1];

	ch2Param.avgCurve.fFai1 = gfDefectZhi1[1];
	ch2Param.avgCurve.fFai2 = gfDefectZhi2[1];
	ch2Param.avgCurve.fFai3 = gfDefectZhi3[1];
	
	ch2Param.avgCurve.fFirstGain = gfAvgFirstPointGain[1];
	ch2Param.avgCurve.fOldRange = gfOldAvgRange[1];
	for(int j = 0; j < gSelectedAvgPointDataNum[1]; j++)
	{
		ch2Param.avgCurve.wOldSerial[j] = gwOldAvgSerial[1][j];
	}
	ch2Param.avgCurve.selectedPointDataNum = gSelectedAvgPointDataNum[1];
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(ch2Param.avgCurve.selectedPointData[j], gSelectedAvgPointData[1][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//��·
	ch2Param.RailRoadParam.wStandard = gwStandard[1];
	ch2Param.RailRoadParam.wPosition = gwPosition[1][gwStandard[1]];
	memcpy(ch2Param.RailRoadParam.fBuChangGain, gfTlBuChangPlus[1], sizeof(ch2Param.RailRoadParam.fBuChangGain));
	memcpy(ch2Param.RailRoadParam.fRange, gfTlRange[1], sizeof(ch2Param.RailRoadParam.fRange));
	memcpy(ch2Param.RailRoadParam.fPlus, gfTlPlus[1], sizeof(ch2Param.RailRoadParam.fPlus));
	memcpy(ch2Param.RailRoadParam.bFlag, gbTlSheDingFlag[1], sizeof(ch2Param.RailRoadParam.bFlag));
	
	HANDLE hFileWrite;	//�ļ����
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�����ļ�ʧ��"));
		return -1;
	}

	if(!WriteFile(hFileWrite,&gSysInfo, sizeof(SYS_INFO),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));
		
		CloseHandle(hFileWrite);
		return FALSE;
	}

	if(!WriteFile(hFileWrite,&ch1Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	if(!WriteFile(hFileWrite,&ch2Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

	int nRet = wcscmp(lpszPathName, lpDefaultParamFile);
	if(nRet != 0)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�ļ�����ɹ�"));
	}

	return TRUE;	
}


//ͨ������ʾ���ڵ�˫ͨ����
static void allCfgDisplayChannelNo(HWND hWnd, WORD wChannelNo)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%d"), wChannelNo + 1);
	DrawLableText1(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 80, ALLCFG_LABEL_START_Y, 50, ALLCFG_LABEL_HEIGHT);
}
//��һ��һ�ա�����������˫������ģʽ��ͨ������ʾΪ��
static void allCfgDisplayChNo(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	DrawLableText1(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 80, ALLCFG_LABEL_START_Y, 50, ALLCFG_LABEL_HEIGHT);
}

//�����������͵Ĳ�����ʾ
static void allCfgDisplayStepFloat2(HWND hWnd, float fStep)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.2f"), fStep);
	DrawLableText1(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 510, ALLCFG_LABEL_START_Y, 100, ALLCFG_LABEL_HEIGHT);
}

static void allCfgDisplayStepFloat(HWND hWnd, float fStep)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fStep);
	DrawLableText1(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 510, ALLCFG_LABEL_START_Y, 100, ALLCFG_LABEL_HEIGHT);
}
static void allCfgDisplayStepInt(HWND hWnd, WORD wStep)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%d"), wStep);
	DrawLableText1(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 510, ALLCFG_LABEL_START_Y, 100, ALLCFG_LABEL_HEIGHT);
}

/**************************
*ע�Ṥ�ղ������ñ��ⴰ��
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAllParamCfgCaptionChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(0, 0, 255));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC) allParamCfgCaptionChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAllParamCfgCaptionChild;

	return RegisterClass(&wc);
}
/**************************
*���ղ������ñ��ⴰ�ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK allParamCfgCaptionChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message) 
	{
		case WM_CREATE:
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			wchar_t strText[15];
			memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("ͨ��"));
			drawSrting(hWnd,strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								20, STRING_START_Y,50, STRING_HEIGHT);
			if(gnWorkType == 1 || gnWorkType == 3)
				allCfgDisplayChannelNo(hWnd, wChannelNo);
			else
				allCfgDisplayChNo(hWnd);
			/*memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("���ղ��� %d"), gnParamNo);
			
			drawSrting(hWnd,strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								220, STRING_START_Y,150, STRING_HEIGHT);*/

			drawSrting(hWnd,_T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								450, STRING_START_Y,50, STRING_HEIGHT);

			//allCfgDisplayStepFloat(hWnd, fStep);
			
			EndPaint(hWnd, &ps);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/**************************
*ע�Ṥ�ղ������ô���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAllParamCfgChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC) allParamCfgChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAllParamCfgChild;

	return RegisterClass(&wc);
}
/*static void allCfgDisplayGongYiCanShu(HWND hWnd)
{	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%d"), gwGongYiCanShu);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}*/



// ��ѹ
static void allCfgDisplayVoltage(HWND hWnd, WORD wVoltage)
{
	wAllCfgVoltageType = VoltageToType(wVoltage);
		
	if(wAllCfgVoltageType == 0)
		DrawLableText1(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wAllCfgVoltageType == 1)
		DrawLableText1(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wAllCfgVoltageType == 2)
		DrawLableText1(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wAllCfgVoltageType == 3)
		DrawLableText1(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}

// �첨ģʽ
static void allCfgDisplayJianBo(HWND hWnd,WORD wJianBoType)
{
	if(wJianBoType == 0)
		DrawLableText1(hWnd, _T("���벨"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wJianBoType == 1)
		DrawLableText1(hWnd, _T("���벨"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wJianBoType ==2)
		DrawLableText1(hWnd, _T("ȫ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wJianBoType == 3)
		DrawLableText1(hWnd, _T("��Ƶ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Receive_AllParamConfigWith_ad[0].nJianBoMode = Receive_AllParamConfigWith_ad[1].nJianBoMode;
	}
}

// ����
static void allCfgDisplayMaiKuan(HWND hWnd)
{	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%d"), Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Send_AllParamConfigWith_ad[0].nMaiKuan = Send_AllParamConfigWith_ad[1].nMaiKuan;
	}
}
//ֱ̽ͷʱǰ��������ʾ
static void allCfgDisplayQianYan(HWND hWnd,float fQianYan)
{	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"), fQianYan);
	
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, RGB(219,207,202), ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Send_AllParamConfigWith_ad[0].fQianYan= Send_AllParamConfigWith_ad[1].fQianYan;
	}

}

// �ظ�Ƶ�� 
static void allCfgDisplayRate(HWND hWnd)
{
	
	/*DWORD nReFreq = Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency;

	if(nReFreq != MIN_RE_FREQ)
	{
		nReFreq = DWORD(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed*1000/(6*Display_AllParamConfigWith_ad[wChannelNo].fRange));

		if(nReFreq > 1000)
			nReFreq = 1000;
		else if(nReFreq < 50)
			nReFreq = 50;
	}
	// ���ñ�ǩ
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		if(nReFreq == MIN_RE_FREQ)
			swprintf(strText, _T("%d"), nReFreq);
		else
		{
			swprintf(strText, _T("�Զ�"));
		}

		DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	}*/
	DrawLableText1(hWnd,_T("�Զ�"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}

// ���
static void allCfgDisplayAmend(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.2f"), Send_AllParamConfigWith_ad[wChannelNo].nAmend);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Send_AllParamConfigWith_ad[0].nAmend = Send_AllParamConfigWith_ad[1].nAmend;
	}

}

// ��׼����
static void allCfgDisplayPlus(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"), Receive_AllParamConfigWith_ad[wChannelNo].fPlus);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Receive_AllParamConfigWith_ad[0].fPlus = Receive_AllParamConfigWith_ad[1].fPlus;
	}

}

// �ο�����
static void allCfgDisplayRefPlus(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"), Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Receive_AllParamConfigWith_ad[0].fRefPlus = Receive_AllParamConfigWith_ad[1].fRefPlus;
	}

}


// ��ʱ
static void allCfgDisplayDelay(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.2f"), Receive_AllParamConfigWith_ad[wChannelNo].fDelay);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Receive_AllParamConfigWith_ad[0].fDelay = Receive_AllParamConfigWith_ad[1].fDelay;
	}


}

// �˲�Ƶ��
static void allCfgDisplayLvBo(HWND hWnd,WORD wLvBoType)
{
	if(wLvBoType ==0)
		DrawLableText1(hWnd,_T("1M"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wLvBoType ==1)
		DrawLableText1(hWnd,_T("0.5M"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wLvBoType ==2)
		DrawLableText1(hWnd,_T("2M"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wLvBoType ==3)
		DrawLableText1(hWnd,_T("��Ƶ��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wLvBoType ==4)
		DrawLableText1(hWnd,_T("5M"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(wLvBoType ==5)
		DrawLableText1(hWnd,_T("2.5M"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Receive_AllParamConfigWith_ad[0].nProbeFreq= Receive_AllParamConfigWith_ad[1].nProbeFreq;
	}

}

// S/Xģʽ
static void allCfgDisplayRefractMode(HWND hWnd)
{
	if(Display_AllParamConfigWith_ad[wChannelNo].bSFlag)	
		DrawLableText1(hWnd,_T("S"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else
		DrawLableText1(hWnd,_T("X"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4||gnWorkType == 5||gnWorkType == 6)
	{
		Display_AllParamConfigWith_ad[0].bSFlag = Display_AllParamConfigWith_ad[1].bSFlag;
	}

}
WORD allCfgwaveSpeedToType(WORD wWaveSpeed)
{
    WORD wMaterialType = 0;

	InitializeCriticalSection(&csProbeType);
	EnterCriticalSection(&csProbeType);

	if(nTanTouType[wChannelNo] == 0)	//б̽ͷ
	{
		if(wWaveSpeed == 3230)
		{
			wMaterialType = 2;	//��
		}
		else if(wWaveSpeed == 3230)
		{
			wMaterialType = 3;	//
		}
		else if(wWaveSpeed == 2260)
		{
			wMaterialType = 4;	//
		}
		else if(wWaveSpeed == 3150)
		{
			wMaterialType = 5;	//
		}
		else if(wWaveSpeed == 3110)
		{
			wMaterialType = 6;	//
		}
		else if(wWaveSpeed == 3080)
		{
			wMaterialType = 7;	//
		}
		else if(wWaveSpeed == 2110)
		{
			wMaterialType = 8;	//
		}
		else if(wWaveSpeed == 2410)
		{
			wMaterialType = 9;	//
		}
		else if(wWaveSpeed == 1100)
		{
			wMaterialType = 10;	//
		}
		else if(wWaveSpeed == 980)
		{
			wMaterialType = 11;	//
		}
		else
		{
			wMaterialType = 1;	//�Զ���
		}
	}
	else if(nTanTouType[wChannelNo] == 1)	//ֱ̽ͷ
	{
		if(wWaveSpeed == 5920)
		{
			wMaterialType = 2;	//��
		}
		else if(wWaveSpeed == 5930)
		{
			wMaterialType = 3;	//
		}
		else if(wWaveSpeed == 4720)
		{
			wMaterialType = 4;	//
		}
		else if(wWaveSpeed == 5842)
		{
			wMaterialType = 5;	//
		}
		else if(wWaveSpeed == 5740)
		{
			wMaterialType = 6;	//
		}
		else if(wWaveSpeed == 6260)
		{
			wMaterialType = 7;	//
		}
		else if(wWaveSpeed == 4399)
		{
			wMaterialType = 8;	//
		}
		else if(wWaveSpeed == 4170)
		{
			wMaterialType = 9;	//
		}
		else if(wWaveSpeed == 2540)
		{
			wMaterialType = 10;	//
		}
		else if(wWaveSpeed == 2388)
		{
			wMaterialType = 11;	//
		}
		else
		{
			wMaterialType = 1;	//�Զ���
		}
	}

	LeaveCriticalSection(&csProbeType);
	DeleteCriticalSection(&csProbeType);
	return wMaterialType;
}

WORD allCfgtypeToWaveSpeed(WORD wType)
{
	WORD wWaveSpeed = 0;
	InitializeCriticalSection(&csProbeType);
	EnterCriticalSection(&csProbeType);

	if(nTanTouType[wChannelNo] == 0)	//б̽ͷ
	{
		if(wType == 2)//��
		{
			wWaveSpeed = 3230;			
		}
		else if(wType == 3)
		{
			wWaveSpeed = 3230;
		}
		else if(wType == 4)
		{
			wWaveSpeed = 2260;
		}
		else if(wType == 5)
		{
			wWaveSpeed = 3150;
		}
		else if(wType == 6)
		{
			wWaveSpeed = 3110;
		}
		else if(wType == 7)
		{
			wWaveSpeed = 3080;
		}
		else if(wType == 8)
		{
			wWaveSpeed = 2110;
		}
		else if(wType == 9)
		{
			wWaveSpeed = 2410;
		}
		else if(wType == 10)
		{
			wWaveSpeed = 1100;
		}
		else if(wType == 11)
		{
			wWaveSpeed = 980;
		}
		else if(wType == 1)
		{
			wWaveSpeed = wUserDefineWaveSpeed;
		}
	}
	else if(nTanTouType[wChannelNo] == 1)	//ֱ̽ͷ
	{
		if(wType == 2)//��
		{
			wWaveSpeed = 5920;			
		}
		else if(wType == 3)
		{
			wWaveSpeed = 5930;
		}
		else if(wType == 4)
		{
			wWaveSpeed = 4720;
		}
		else if(wType == 5)
		{
			wWaveSpeed = 5842;
		}
		else if(wType == 6)
		{
			wWaveSpeed = 5740;
		}
		else if(wType == 7)
		{
			wWaveSpeed = 6260;
		}
		else if(wType == 8)
		{
			wWaveSpeed = 4399;
		}
		else if(wType == 9)
		{
			wWaveSpeed = 4170;
		}
		else if(wType == 10)
		{
			wWaveSpeed = 2540;
		}
		else if(wType == 11)
		{
			wWaveSpeed = 2388;
		}
		else if(wType == 1)
		{
			wWaveSpeed = wUserDefineWaveSpeed;
		}
	}

	LeaveCriticalSection(&csProbeType);
	DeleteCriticalSection(&csProbeType);
	return wWaveSpeed;
}

// ����ģʽ
static void allCfgDisplaySpeedType(HWND hWnd, WORD wMaterial)
{
	//wMaterialType = waveSpeedToType(wWaveSpeed);
	
	switch(wMaterial)
	{
		case 1:
			DrawLableText1(hWnd,_T("�Զ���"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 2:
			DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 3:
			DrawLableText1(hWnd, _T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 4:
			DrawLableText1(hWnd,_T("ͭ"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 5:
			DrawLableText1(hWnd,_T("�մ�"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 6:
			DrawLableText1(hWnd,_T("�����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 7:
			DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 8:
			DrawLableText1(hWnd,_T("��ͭ"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 9:
			DrawLableText1(hWnd,_T("п"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 10:
			DrawLableText1(hWnd, _T("������֬"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		case 11:
			DrawLableText1(hWnd, _T("������ϩ"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			break;
		default:
			break;						
	}
}

//����
static void allCfgDisplaySpeed(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%d"), Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);
	
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4|| gnWorkType == 5|| gnWorkType == 6)
	{
		Display_AllParamConfigWith_ad[0].nWaveSpeed = Display_AllParamConfigWith_ad[1].nWaveSpeed;

	}
}
//����
static void allCfgDisplayRange(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"), Display_AllParamConfigWith_ad[wChannelNo].fRange);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4|| gnWorkType == 5|| gnWorkType == 6)
	{
		Display_AllParamConfigWith_ad[0].fRange = Display_AllParamConfigWith_ad[1].fRange;

	}

}
//�����
static void allCfgDisplayAngle(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"), Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4|| gnWorkType == 5|| gnWorkType == 6)
	{
		Display_AllParamConfigWith_ad[0].fZheSheJiao = Display_AllParamConfigWith_ad[1].fZheSheJiao;

	}

}
//Kֵ
static void allCfgDisplayKValue(HWND hWnd, float  fZheSheJiao)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	double fAngle = fZheSheJiao/180.0f * PI;
	fKValue = (float)tan(fAngle);
			
	wsprintf(strText, _T("%.1f"), fKValue);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//����
static void allCfgDisplayBate(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%d"), Display_AllParamConfigWith_ad[wChannelNo].nBate);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	if(gnWorkType == 4|| gnWorkType == 5|| gnWorkType == 6)
	{
		Display_AllParamConfigWith_ad[0].nBate = Display_AllParamConfigWith_ad[1].nBate;

	}

}
//ʱ����ʾ ��
static void allCfgDisplayTimeYear(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),st.wYear);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//��
static void allCfgDisplayTimeMonth(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),st.wMonth);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//��
static void allCfgDisplayTimeDay(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),st.wDay);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//Сʱ
static void allCfgDisplayTimeHour(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),st.wHour);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//����
static void allCfgDisplayTimeMinute(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),st.wMinute);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//��
static void allCfgDisplayTimeSecond(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),st.wSecond);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//ʱ���붨ʱ����Ӧ����
void CALLBACK allCfgParamDisplayProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
		::GetLocalTime(&st);

		 allCfgDisplayTimeHour(hBtnAllParamCfgTimeHour);
		 allCfgDisplayTimeMinute(hBtnAllParamCfgTimeMinute);
		 allCfgDisplayTimeSecond(hBtnAllParamCfgTimeSecond);	 
}

static void allCfgDisplayGongJianHouDu(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"),allParamBScanGongJianHouDu);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//Bɨģʽת��
static void allCfgDisplayScanMode(HWND hWnd, WORD wScanModeType)
{
	
		
	if(wScanModeType== 0)
		DrawLableText1(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	if(wScanModeType== 1)
		DrawLableText1(hWnd, _T("ѭ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	
}
static void allCfgDisplayBScanOnOrOff(HWND hWnd, WORD ScanswitchType)
{
	
		
	if(ScanswitchType == 0)
		DrawLableText1(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	if(ScanswitchType == 1)
		DrawLableText1(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	
}
static void allCfgDisplayPlateHeight(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"),allParamPlateHeight);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
static void allCfgDisplayProbeType(HWND hWnd, const WORD& nProbeNo)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));
	if(nProbeNo != 10)
		wsprintf(strText, _T("ARS-P0%d"), nProbeNo);
	else
		wsprintf(strText, _T("ARS-P10"));
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//��ä��
static void allCfgDisplayUpBlindValue(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"),allParamUpBlindValue);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//��ä��
static void allCfgDisplayDownBlindValue(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"),allParamDownBlindValue);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
static void allCfgDisplayZhongHouBanTanShangSensitive(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"),Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//�¿ڽǶ�
static void allCfgDisplayPoKouAngle(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%.1f"),allParamPoKouJiaoDu);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//�¿����
static void allCfgDisplayPoKouDeep(HWND hWnd ,WORD wPoKouType)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(wPoKouType == 0)
	{ 
		wsprintf(strText, _T("�������"));
		DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	}
	else if(wPoKouType == 1)
	{
		wsprintf(strText, _T("%.1f"), allParamPoKouShenDu);
		DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	}
	else if(wPoKouType == 2)
	{
		wsprintf(strText, _T("%.1f"), allParamPoKouShenDu);
		DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	}
}
//�¿�����
static void allCfgDisplayPoKouType(HWND hWnd ,WORD wPoKouType)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(wPoKouType == 0)
		wsprintf(strText, _T("V�Ϳ�"));
	else if(wPoKouType == 1)
		wsprintf(strText, _T("Y�Ϳ�"));
	else if(wPoKouType == 2)
		wsprintf(strText, _T("˫Y�Ϳ�"));
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	allCfgDisplayPoKouAngle(hBtnAllParamCfgPoKouAngle);
	allCfgDisplayPoKouDeep(hBtnAllParamCfgPoKouDeep,allParamPoKouLeiXing);
}
static void allCfgDisplayHanFengTanShangHouDu(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"),allParamHanFengTanShangGangBanHouDu);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
static void allCfgDisplayHanFengTanShangDistance(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"),allParamHead2HanFengDistance);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
static void allCfgDisplayHanFengTanShangOnOrOff(HWND hWnd, WORD HanFengTanShangswitchType)
{
	
		
	if(HanFengTanShangswitchType == 0)
		DrawLableText1(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(HanFengTanShangswitchType == 1)
		DrawLableText1(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	
}
//����
static void allCfgDisplayLiangDu(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText,0,sizeof(strText));
	wsprintf(strText,_T("%d"),allParamLCDValue);
	
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//�迹��ʾ
static void allCfgdisplayImpedance(HWND hWnd,WORD wChannelNo, WORD wImpedance)
{
	if(gnWorkType == 1)
	{
		if(wChannelNo == 0)
		{
			if(wImpedance == 0)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			else if(wImpedance == 1)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
		}
		else if(wChannelNo == 1)
		{
			if(wImpedance == 2)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			else if(wImpedance == 0)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
		}
	}
	else
	{
		if(wChannelNo == 0)
		{
			if(wImpedance == 0)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT,  RGB(219,207,202), ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			else if(wImpedance == 1)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT,  RGB(219,207,202), ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
		}
		else if(wChannelNo == 1)
		{
			if(wImpedance == 2)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT,  RGB(219,207,202), ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
			else if(wImpedance == 0)
				DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT,  RGB(219,207,202), ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
		}
	}
}

static void allCfgDisplayQuXianFenDuan(HWND hWnd)
{
	if(gbQuXianFenDuan ==0)		
		DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gbQuXianFenDuan == 1)
		DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);	
}
static void allCfgDisplayPinPuFenXi(HWND hWnd)
{
	if(gbPinPuFenXi==0)		
		DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gbPinPuFenXi== 1)
		DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);	
}
static void allCfgDisplayXingNengJiaoYan(HWND hWnd)
{
	if(gbXingNengJiaoYan == 0)		
		DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gbXingNengJiaoYan == 1)
		DrawLableText1(hWnd,_T("��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);	
}
static void allCfgDisplayJuLiZuoBiao(HWND hWnd)
{
	if(gwJuLiZuoBiao== 0)		
		DrawLableText1(hWnd,_T("H"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwJuLiZuoBiao== 1)
		DrawLableText1(hWnd,_T("S"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);	
	else if(gwJuLiZuoBiao== 2)
		DrawLableText1(hWnd,_T("L"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT); 

}
//�ܲ��⾶
static void allCfgDisplayGuanCaiWaiJing(HWND hWnd, float fWaiJing)
{
	wchar_t strText[20];
	memset(strText,0,sizeof(strText));

	wsprintf(strText,_T("%.1f"),fWaiJing);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
//�ܲ��ھ�
static void allCfgDisplayGuanCaiNeiJing(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText,0,sizeof(strText));

	wsprintf(strText,_T("%.1f"),gfGuanCaiNeiJing);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
static void allCfgDisplayXiangYingSpeed(HWND hWnd)
{
	if(gnXiangYingSpeed== 0)
		
		DrawLableText1(hWnd,_T("����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gnXiangYingSpeed== 1)
		DrawLableText1(hWnd,_T("����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);	
	else if(gnXiangYingSpeed== 2)
		DrawLableText1(hWnd,_T("����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT); 

}
//���油��
static void allCfgDisplayBiaoMianBuChang(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),gwBiaoMianBuChang);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}

static void allCfgDisplayPrintType(HWND hWnd)
{
	if(gnPrintType== 0)
		
		DrawLableText1(hWnd,_T("HP"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gnPrintType== 1)
		DrawLableText1(hWnd,_T("EPSON"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);	
	else if(gnPrintType== 2)
		DrawLableText1(hWnd,_T("CANON"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT); 
	else if(gnPrintType== 3)
		DrawLableText1(hWnd,_T("��ʽ��ӡ��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT); 

}
static void allCfgDisplayLieWenQiBo(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),gwLieWenQiBo);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//����
static void allCfgDisplayPingDing(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%.f"),gfParamPingDing[wChannelNo]);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//����
static void allCfgDisplayDingLiang(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%.f"),gfParamDingLiang[wChannelNo]);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//�з�
static void allCfgDisplayPanFei(HWND hWnd)
{	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%.f"),gfParamPanFei[wChannelNo]);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//̽ͷ����
static void allCfgDisplayTanTouType(HWND hWnd)
{
	InitializeCriticalSection(&csProbeType);
	EnterCriticalSection(&csProbeType);

	if(nTanTouType[wChannelNo] == 0)		
		DrawLableText1(hWnd,_T("б̽ͷ"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(nTanTouType[wChannelNo] == 1)
		DrawLableText1(hWnd,_T("ֱ̽ͷ"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	LeaveCriticalSection(&csProbeType);
	DeleteCriticalSection(&csProbeType);
}
static void allCfgDisplayCanShuQinLing(HWND hWnd)
{
	if(gbCanShuQinLing== 0)		
		DrawLableText1(hWnd,_T("�ر�"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gbCanShuQinLing== 1)
		DrawLableText1(hWnd,_T("����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
static void allCfgDisplayTongDaoQinLing(HWND hWnd)
{
	if(gbTongDaoQinLing== 0)		
		DrawLableText1(hWnd,_T("�ر�"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	if(gbTongDaoQinLing== 1)
		DrawLableText1(hWnd,_T("����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
//��������

static void allCfgDisplayCanShuSuoDing(HWND hWnd)
{
	if(gbCanShuSuoDing== 0)		
		DrawLableText1(hWnd,_T("�ر�"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	if(gbCanShuSuoDing== 1)
		DrawLableText1(hWnd,_T("����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}

static void allCfgDisplayZiDongBoGao(HWND hWnd)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),gwZiDongBoGao);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
static void allCfgDisplayPlusType(HWND hWnd)
{
	if(gwPlusType== 0)
		
		DrawLableText1(hWnd,_T("�ֶ�����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	if(gwPlusType== 1)
		DrawLableText1(hWnd,_T("�Զ�����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
//����ģʽ

static void allCfgDisplayWorkType(HWND hWnd, int nWorkType)
{
	if(nWorkType == 1 && (gFunctionDefine & WORKMODE1OR2_SWITCH) != 0)
		DrawLableText1(hWnd,_T("��ͨ��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	
	else if(nWorkType == 3 && (gFunctionDefine & WORKMODE1PLUS2_SWITCH) != 0)
		DrawLableText1(hWnd,_T("˫ͨ��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	else if(nWorkType == 4 && (gFunctionDefine & WORKMODE1TO2_SWITCH) != 0)
		DrawLableText1(hWnd,_T("һ��һ��"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	else if(nWorkType == 5 && (gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
		DrawLableText1(hWnd,_T("��������"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

	else if(nWorkType == 6 && (gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
		DrawLableText1(hWnd,_T("˫������"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
static void allCfgDisplayHardWare(HWND hWnd)//��ʾӲ���汾�ź���
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%.2f"),HARDWARE_VER);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
static void allCfgDisplaySoftWare(HWND hWnd)//��ʾ����汾�ź���
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%.2fR"),SOFTWARE_VER);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
static void allCfgDisplayNextAdjustTime(HWND hWnd)//��ʾ�´�У׼ʱ��
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	HKEY HKeyNum;
	LONG ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\UpperDataManager\\Adjust"), 0, NULL, &HKeyNum);
/*	if(ret != ERROR_SUCCESS)
	{
		MessageBox (hWnd, TEXT ("��עУ׼ʱ����ʧ��"), TEXT("NDT_CE"), 0) ;
		
		return;
	}*/
	DWORD nSize = sizeof(wchar_t)*20;
	DWORD type = 0;

	RegQueryValueEx(HKeyNum,_T("NextAdjust"), 0, &type, (LPBYTE)strText, &nSize);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	RegCloseKey(HKeyNum);
}
static void allCfgDisplayThisAdjustTime(HWND hWnd)//��ʾ����У׼ʱ��
{

	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	HKEY HKeyNum;
	LONG ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\UpperDataManager\\Adjust"), 0, NULL, &HKeyNum);
/*	if(ret != ERROR_SUCCESS)
	{
		MessageBox (hWnd, TEXT ("��עУ׼ʱ����ʧ��"), TEXT("NDT_CE"), 0) ;
		
		return;
	}*/
	DWORD nSize = sizeof(wchar_t)*20;
	DWORD type = 0;

	RegQueryValueEx(HKeyNum,_T("CurrentAdjust"), 0, &type, (LPBYTE)strText, &nSize);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	RegCloseKey(HKeyNum);
}
static void allCfgDisplayEquipmentModel(HWND hWnd)//��ʾ�豸�ͺź���
{
	if(gwDeviceTypeValue == 0x2010)
		DrawLableText1(hWnd,_T("ARS201"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2011)
		DrawLableText1(hWnd,_T("ARS201E"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2020)
		DrawLableText1(hWnd,_T("ARS202"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2021)
		DrawLableText1(hWnd,_T("ARS202E"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2022)
		DrawLableText1(hWnd,_T("ARS202M"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2023)
		DrawLableText1(hWnd,_T("ARS202P"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2024)
		DrawLableText1(hWnd,_T("ARS202R"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2025)
		DrawLableText1(hWnd,_T("ARS202RA"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2026)
		DrawLableText1(hWnd,_T("ARS202RB"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2027)
		DrawLableText1(hWnd,_T("ARS202RC"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2070)
		DrawLableText1(hWnd,_T("ARS207"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x2080)
		DrawLableText1(hWnd,_T("ARS208"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x3010)
		DrawLableText1(hWnd,_T("ARS301"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else if(gwDeviceTypeValue == 0x5010)
		DrawLableText1(hWnd,_T("ARS501"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//������������ʾͨ�ú���
static void allCfgDisplayFloat1(HWND hWnd, float fValue)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%.1f"),fValue);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//BOOL�ͱ���������ʾͨ�ú���

static void allCfgDisplaySwitch(HWND hWnd, BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText1(hWnd,_T("����"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
	else
		DrawLableText1(hWnd,_T("�ر�"),DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}
//int�ͱ���������ʾͨ�ú���

static void allCfgDisplayInt(HWND hWnd, int nValue)
{
	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText,_T("%d"),nValue);
	DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
}
//���ղ�������ʾ

static void allCfgDisplayGongYiCanShu(HWND hWnd, DWORD dwNum)
{
	
		wchar_t strText[10];
		memset(strText, 0, sizeof(strText));

		wchar_t strFileName[10];
		memset(strFileName, 0, sizeof(strFileName));

		wsprintf(strFileName, _T("%d.par"), gwGongYiCanShu);

		memcpy(ParamFileName, FileDirectory, sizeof(ParamFileName));
		StringCchCat(ParamFileName, MAX_PATH, strFileName);

		if(GetFileAttributes(ParamFileName)  == 0xFFFFFFFF)
		{
			wsprintf(strText, _T("*%d"), gwGongYiCanShu);
			
		}
		
		else
		{
			wsprintf(strText, _T("%d"), gwGongYiCanShu);

		}
		DrawLableText1(hWnd,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);

}


void allCfgCh1Data2Ch2Data()
{
	// �������
	memcpy(&Send_AllParamConfigWith_ad[1], &Send_AllParamConfigWith_ad[0], sizeof(SEND_PARAM_DATA));
	// ���ղ���
//	memcpy(&ReceiveParam_ad[1], &ReceiveParam_ad[0], sizeof(RECEIVE_PARAM_DATA));//�迹������wangbingfu modify at 2010-11-19
	Receive_AllParamConfigWith_ad[1].fDelay = Receive_AllParamConfigWith_ad[0].fDelay;
	Receive_AllParamConfigWith_ad[1].fPlus = Receive_AllParamConfigWith_ad[0].fPlus;
	Receive_AllParamConfigWith_ad[1].fRefPlus = Receive_AllParamConfigWith_ad[0].fRefPlus;
	Receive_AllParamConfigWith_ad[1].nJianBoMode = Receive_AllParamConfigWith_ad[0].nJianBoMode;
	Receive_AllParamConfigWith_ad[1].nLvBo = Receive_AllParamConfigWith_ad[0].nLvBo;
	Receive_AllParamConfigWith_ad[1].nProbeFreq = Receive_AllParamConfigWith_ad[0].nProbeFreq;
	// ��ʾ����
	memcpy(&Display_AllParamConfigWith_ad[1], &Display_AllParamConfigWith_ad[0], sizeof(DISPLAY_SET_DLG_DATA));

}



//�л�ͨ����Ӧ����

static void refreshAllCarftParam()
{
	//allCfgDisplayGongYiCanShu(hBtnAllParamCfgGongYiCanShu,gwGongYiCanShu);

		allCfgDisplayChannelNo(hWndAllParamCfgCaption, wChannelNo);
		//һͨ���Ͷ�ͨ����ѹֵ���
		//allCfgDisplayVoltage(hBtnAllParamCfgVoltage, Send_AllParamConfigWith_ad[wChannelNo].nPress);
		if(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao > 0.0f)
		{
			allCfgDisplayFloat1(hBtnAllParamCfgQianYan, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
			nTanTouType[wChannelNo] = 0;
			allCfgDisplayTanTouType(hBtnAllParamCfgTanTouType);
		}
		else
		{
			allCfgDisplayQianYan(hBtnAllParamCfgQianYan, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
			nTanTouType[wChannelNo] = 1;
			allCfgDisplayTanTouType(hBtnAllParamCfgTanTouType);
		}
		allCfgDisplayMaiKuan(hBtnAllParamCfgMaiKuan);
		allCfgDisplayRate(hBtnAllParamCfgRate);
		allCfgDisplayAmend(hBtnAllParamCfgAmend);
		allCfgDisplayJianBo(hBtnAllParamCfgJianBo, Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode);
		allCfgDisplayPlus(hBtnAllParamCfgPlus);
		allCfgDisplayRefPlus(hBtnAllParamCfgRefPlus);
		allCfgDisplayDelay(hBtnAllParamCfgDelay);
		allCfgDisplayLvBo(hBtnAllParamCfgLvBo, Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq);
		allCfgDisplayRefractMode(hBtnAllParamCfgRefractMode);

		allCfgDisplaySpeed(hBtnAllParamCfgSpeed);
		wMaterialType = allCfgwaveSpeedToType(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);
		allCfgDisplaySpeedType(hBtnAllParamCfgSpeedType, wMaterialType);
		
		allCfgDisplayRange(hBtnAllParamCfgRange);
		allCfgDisplayAngle(hBtnAllParamCfgAngle);
		allCfgDisplayKValue(hBtnAllParamCfgKValue, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
		allCfgDisplayBate(hBtnAllParamCfgBate);
		allCfgdisplayImpedance(hBtnAllParamCfgZuKangPiPei,wChannelNo, Receive_AllParamConfigWith_ad[wChannelNo].wImpedance);
	
		
		allCfgDisplayWorkType(hBtnAllParamCfgWorkType,gnWorkType);
	
	
		allCfgDisplayFloat1(hBtnAllParamCfgPingDing,gfParamPingDing[wChannelNo]);
		allCfgDisplayFloat1(hBtnAllParamCfgDingLing,gfParamDingLiang[wChannelNo]);
		allCfgDisplayFloat1(hBtnAllParamCfgPanFei, gfParamPanFei[wChannelNo]);
		allCfgDisplayFloat1(hBtnAllParamCfgQuXianGuiGe1,gfParamFaiZhi1[wChannelNo]);
		allCfgDisplayFloat1(hBtnAllParamCfgQuXianGuiGe2,gfParamFaiZhi2[wChannelNo]);
		allCfgDisplayFloat1(hBtnAllParamCfgQuXianGuiGe3,gfParamFaiZhi3[wChannelNo]);
		//allCfgDisplayFloat1(hBtnAllParamCfgGongJianHouDu, allParamPlateHeight);
		allCfgDisplayFloat1(hBtnAllParamCfgZhongHouBanTanShangSensitive, Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus);
	

}

/**************************
*���ղ������ô��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK allParamCfgChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	// songchenguang add start 2010-08-14
	SCROLLINFO  si;
	int  iVertPos = 0;
	int nVScrollPos = 0;
	HWND hChildFocus = NULL;
	 RECT RectChild;
	 RECT RectParent;
	 int cha = 0xFF;
	 int cha2 = 0xFF;
	// songchenguang add end 2010-08-14
	switch (message) 
	{
		case WM_CREATE:
			
			wChannelNo = CurrentChannelNo;
			

			//ʱ���ʼ��

			::GetLocalTime(&st);
	 		
			 SetTimer(hWnd, TIMER_TIME_DISPLAY, 1000, allCfgParamDisplayProc);
			 //zhaoli add in 2010-9-16
			 
			 allParamBScanGongJianHouDu = fBScanGangBanHouDu;//Bɨ������Ȳ���ʼ��
			 allParamScanMode = wScanMode;//Bɨɨ�跽ʽ����ʼ��
			 allParamBscanSwitch = gbBscanSwitch;//Bɨ���ز���ʼ��
			 allParamPlateHeight = gfPlateHeight;//�к��̽�˹�����Ȳ���ʼ��
			 allParamProbeType = nProbeType;//�к��̽��̽ͷ���Ͳ���ʼ��
			 allParamUpBlindValue = gfUpBlindValue;//�к��̽����ä������ʼ��
			 allParamDownBlindValue = gfDownBlindValue;//�к��̽����ä������ʼ��
			 allParamPoKouLeiXing = gwPoKouLeiXing;//����̽���¿��������ò���ʼ��
			 allParamPoKouJiaoDu = gfPoKouJiaoDu;//����̽���¿ڽǶ����ò���ʼ��
			 allParamPoKouShenDu = gfPoKouShenDu;//����̽���¿�������ò���ʼ��
			 allParamHanFengTanShangSwitch = bHanFengTanShangSwitch;//����̽�˿��ز���ʼ��
			 allParamHanFengTanShangGangBanHouDu = fHanFengTanShangGangBanHouDu;//����̽�˹�����Ȳ���ʼ��
			 allParamHead2HanFengDistance = fHead2HanFengDistance;//̽ͷ���¿ڵľ��벢��ʼ��
			 gfParamGuanCaiWaiJing = gfWaiJing;//�ܲ��⾶��ʼ��
			 gfGuanCaiNeiJing = gfNeiJing;
			 
			 if(sys_reg->wLedBrightness == 0)
					nLCDValue = 6;
				else
					nLCDValue = sys_reg->wLedBrightness;
				
			 allParamLCDValue = nLCDValue;//�������Ȳ���ʼ��

		
		//zhaoli add in 2010-9-16
		/*	for(int i = 0; i < 2; i++)
			{
				Send_AllParamConfigWith_ad[i] = SendParam_ad[i];
				Receive_AllParamConfigWith_ad[i] = ReceiveParam_ad[i];
				Display_AllParamConfigWith_ad[i] = DisplaySet_ad[i];
			
			}*/
			
			if(gbHotCraftParamView)
			{
				wchar_t strText[10];
				memset(strText, 0, sizeof(strText));
				HKEY HKeyNum;
				LONG ret = RegOpenKeyEx(HKEY_CURRENT_USER, _T("ParNum"), 0, NULL, &HKeyNum);
				//if(ret != ERROR_SUCCESS)
				//	return false;
				DWORD nSize = sizeof(DWORD);
				DWORD type = 0;
				WORD currentUsedParamNo1 = 0;	
				RegQueryValueEx(HKeyNum, _T("ParNum"), 0, &type, (LPBYTE)&currentUsedParamNo1, &nSize);
				gwGongYiCanShu = currentUsedParamNo1;
				gnParamNo = currentUsedParamNo1;
				wsprintf(strText, _T("%d"), gwGongYiCanShu);
				DrawLableText1(hBtnAllParamCfgGongYiCanShu,strText,DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, ALLCFG_LABEL_START_X, ALLCFG_LABEL_START_Y, ALLCFG_LABEL_WIDTH, ALLCFG_LABEL_HEIGHT);
				RegCloseKey(HKeyNum);	
				allCfgReadParamFile(lpDefaultParamFile);//���ȼ����빤�ղ���Ԥ��ʱ��ȡ��ǰ�ļ�
			}
			else
			{
				//gwGongYiCanShu = (WORD)currentUsedParamNo;//��ȡ�ļ���Ÿ�ֵ�����ղ�����
				gwGongYiCanShu = (WORD)gnParamNo;
				allCfgReadParamFile(ParamFileName);//�Ӳ˵�����Ԥ������ȡ������趨���ļ�
			}
			//allCfgReadParamFile(lpDefaultParamFile);
			//allCfgReadParamFile(ParamFileName);

			for(int i = 0; i < CHANNEL_NUM; i++)
			{
				gfParamPingDing[i] = gfPingDingGain[i];				
				gfParamDingLiang[i] = gfDingLiangGain[i];	
				gfParamPanFei[i] = gfPanHuiGain[i];
			}

			for(int i = 0; i < CHANNEL_NUM; i++)
			{
				gfParamFaiZhi1[i] = gfDefectZhi1[i];
				gfParamFaiZhi2[i] = gfDefectZhi2[i];
				gfParamFaiZhi3[i] = gfDefectZhi3[i];
			}
			
			//allCfgReadParamFile(_T("\\ResidentFlash\\1.par"));
			for(int i = 0; i < 2; i++)
			{
				fAllCfgOldRange[i] = Display_AllParamConfigWith_ad[i].fRange;
				fAllCfgOldPlus[i] = Receive_AllParamConfigWith_ad[i].fPlus;
				fAllCfgOldRefPlus[i] = Receive_AllParamConfigWith_ad[i].fRefPlus;
				fAllCfgOldDelay[i] = Receive_AllParamConfigWith_ad[i].fDelay;
				gfAllOldRangeStart[i] = fRangeStart[i];
				gfAllOldRangeEnd[i] = fRangeEnd[i];
			}


			//����������ť
			if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
			{
				buttonNum = 50;
				hBtnAllParamCfgGongYiCanShu= CreateWindow(_T("BUTTON"),_T("AllParamCfgGongYiCanShu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 ,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_GONGYICANSHU,hInst,NULL);

				//hBtnAllParamCfgCanShuQinLing= CreateWindow(_T("BUTTON"),_T("AllParamCfgCanShuQinLing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				//		0 + EACH_ITEM_HEIGHT ,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_CANSHUQINLING,hInst,NULL);

				//hBtnAllParamCfgCanShuSuoDing = CreateWindow(_T("BUTTON"),_T("AllParamCfgCanShuSuoDing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				//		0 + 2*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_CANSHUSUODING,hInst,NULL);

				hBtnAllParamCfgWorkType = CreateWindow(_T("BUTTON"),_T("AllParamCfgWorkType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 1*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_WORKTYPE,hInst,NULL);

				hBtnAllParamCfgTanTouType= CreateWindow(_T("BUTTON"),_T("AllParamCfgTanTouType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TANTOUTYPE,hInst,NULL);

				//hBtnAllParamCfgProbeType= CreateWindow(_T("BUTTON"),_T("AllParamCfgProbeType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				//		0 + 5*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PROBETYPE,hInst,NULL);
				hBtnAllParamCfgAngle = CreateWindow(_T("BUTTON"),_T("AllParamCfgAngle"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_ANGLE,hInst,NULL);

				hBtnAllParamCfgKValue = CreateWindow(_T("BUTTON"),_T("AllParamCfgKValue"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_KVALUE,hInst,NULL);

				hBtnAllParamCfgVoltage = CreateWindow(_T("BUTTON"),_T("AllParamCfgVoltage"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 5*EACH_ITEM_HEIGHT, EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_VOLTAGE,hInst,NULL);
			
				hBtnAllParamCfgMaiKuan = CreateWindow(_T("BUTTON"),_T("AllParamCfgMaiKuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 6*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_MAIKUAN,hInst,NULL);
			
				hBtnAllParamCfgRate = CreateWindow(_T("BUTTON"),_T("AllParamCfgRate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 7*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_RATE,hInst,NULL);
			
				hBtnAllParamCfgAmend = CreateWindow(_T("BUTTON"),_T("AllParamCfgAmend"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 8*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_AMEND,hInst,NULL);

				hBtnAllParamCfgQianYan= CreateWindow(_T("BUTTON"),_T("AllParamCfgQianYan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 9*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_QIANYAN,hInst,NULL);
			
				hBtnAllParamCfgJianBo= CreateWindow(_T("BUTTON"),_T("AllParamCfgJianBo"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 10*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_JIANBO,hInst,NULL);
			
				hBtnAllParamCfgPlus = CreateWindow(_T("BUTTON"),_T("AllParamCfgPlus"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 11*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PLUS,hInst,NULL);

				hBtnAllParamCfgRefPlus = CreateWindow(_T("BUTTON"),_T("AllParamCfgRefPlus"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 12*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_REFPLUS,hInst,NULL);
			
				hBtnAllParamCfgDelay = CreateWindow(_T("BUTTON"),_T("AllParamCfgDelay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 13*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_DELAY,hInst,NULL);
				
				hBtnAllParamCfgLvBo = CreateWindow(_T("BUTTON"),_T("AllParamCfgLvBo"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 14*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_LVBO,hInst,NULL);

				hBtnAllParamCfgZuKangPiPei= CreateWindow(_T("BUTTON"),_T("AllParamCfgZuKangPiPei"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 15*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_ZUKANGPIPEI,hInst,NULL);

				hBtnAllParamCfgRefractMode = CreateWindow(_T("BUTTON"),_T("AllParamCfgRefractMode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 16*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_REFRACTMODE,hInst,NULL);

				hBtnAllParamCfgSpeedType = CreateWindow(_T("BUTTON"),_T("AllParamCfgSpeedType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 17*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SPEEDTYPE,hInst,NULL);
				
				hBtnAllParamCfgSpeed = CreateWindow(_T("BUTTON"),_T("AllParamCfgSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 18*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SPEED,hInst,NULL);
			
				hBtnAllParamCfgRange = CreateWindow(_T("BUTTON"),_T("AllParamCfgRange"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 19*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_RANGE,hInst,NULL);

				hBtnAllParamCfgBate = CreateWindow(_T("BUTTON"),_T("AllParamCfgBate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 20*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_BATE,hInst,NULL);
		
				hBtnAllParamCfgGuanCaiWaiJing = CreateWindow(_T("BUTTON"),_T("AllParamCfgGuanCaiWaiJing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 21*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_GUANCAIWAIJING,hInst,NULL);

				hBtnAllParamCfgGuanCaiNeiJing = CreateWindow(_T("BUTTON"),_T("AllParamCfgGuanCaiNeiJing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 22*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_GUANCAINEIJING,hInst,NULL);


				hBtnAllParamCfgPingDing= CreateWindow(_T("BUTTON"),_T("AllParamCfgPingDing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 23*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PINGDING,hInst,NULL);


				hBtnAllParamCfgDingLing= CreateWindow(_T("BUTTON"),_T("AllParamCfgDingLiang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 24*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_DINGLIANG,hInst,NULL);

				
				hBtnAllParamCfgPanFei= CreateWindow(_T("BUTTON"),_T("AllParamCfgPanFei"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 25*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PANFEI,hInst,NULL);

				
				//hBtnAllParamCfgBiaoMianBuChang= CreateWindow(_T("BUTTON"),_T("AllParamCfgBiaoMianBuChang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				//		0 + 29*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_BIAOMIANBUCHANG,hInst,NULL);

				
				hBtnAllParamCfgQuXianGuiGe1= CreateWindow(_T("BUTTON"),_T("AllParamCfgQuXianGuiGe1"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 26*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_QUXIANGUIGE,hInst,NULL);
							
				hBtnAllParamCfgQuXianGuiGe2 = CreateWindow(_T("BUTTON"),_T("AllParamCfgQuXianGuiGe2"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 27*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_QUXIANGUIGE,hInst,NULL);
									
				hBtnAllParamCfgQuXianGuiGe3= CreateWindow(_T("BUTTON"),_T("AllParamCfgQuXianGuiGe3"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 28*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_QUXIANGUIGE,hInst,NULL);

				//hBtnAllParamCfgXiangYingSpeed = CreateWindow(_T("BUTTON"),_T("AllParamCfgXiangYingSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					//	0 + 31*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_XIANGYINGSPEED,hInst,NULL);

				hBtnAllParamCfgScanMode= CreateWindow(_T("BUTTON"),_T("AllParamCfgScanMode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 29*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SCANMODE,hInst,NULL);

				//hBtnAllParamCfgBScanOnOrOff= CreateWindow(_T("BUTTON"),_T("AllParamCfgBScanOnOrOff"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				//		0 + 34*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_BSCANONOROFF,hInst,NULL);
				
				//hBtnAllParamCfgPlateHeight= CreateWindow(_T("BUTTON"),_T("AllParamCfgPlateHeight"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				//		0 + 33*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PLATEHEIGHT,hInst,NULL);

				
				hBtnAllParamCfgGongJianHouDu= CreateWindow(_T("BUTTON"),_T("AllParamCfgGongJianHouDu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 30*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_GONGJIANHOUDU,hInst,NULL);

				hBtnAllParamCfgUpBlindValue= CreateWindow(_T("BUTTON"),_T("AllParamCfgUpBlindValue"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 31*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_UPBLINDVALUE,hInst,NULL);

				hBtnAllParamCfgDownBlindValue= CreateWindow(_T("BUTTON"),_T("AllParamCfgDownBlindValue"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 32*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_DOWNBLINDVALUE,hInst,NULL);

				hBtnAllParamCfgZhongHouBanTanShangSensitive= CreateWindow(_T("BUTTON"),_T("AllParamCfgZhongHouBanTanShangSensitive"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 33*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SENSITIVE,hInst,NULL);

				hBtnAllParamCfgPoKouType= CreateWindow(_T("BUTTON"),_T("AllParamCfgPoKouType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 34*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_POKOUTYPE,hInst,NULL);

				hBtnAllParamCfgPoKouAngle= CreateWindow(_T("BUTTON"),_T("AllParamCfgPoKouAngle"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 35*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_POKOUANGLE,hInst,NULL);

				hBtnAllParamCfgPoKouDeep= CreateWindow(_T("BUTTON"),_T("AllParamCfgPoKouDeep"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 36*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_POKOUDEEP,hInst,NULL);
				
				hBtnAllParamCfgHanFengTanShangDistance= CreateWindow(_T("BUTTON"),_T("AllParamCfgHanFengTanShangDistance"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 37*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_HANFENGTANSHANGDISTANCE,hInst,NULL);

				//hBtnAllParamCfgHanFengTanShangOnOrOff= CreateWindow(_T("BUTTON"),_T("AllParamCfgHanFengTanShangOnOrOff"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				//		0 + 42*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_HANFENGTANSHANGONOROFF,hInst,NULL);

		
				hBtnAllParamCfgTimeYear = CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeYear"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 38*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEYEAR,hInst,NULL);
				
				hBtnAllParamCfgTimeMonth= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeMonth"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 39*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEMONTH,hInst,NULL);

				hBtnAllParamCfgTimeDay= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeDay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 40*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEDAY,hInst,NULL);
				
				hBtnAllParamCfgTimeHour= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeHour"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 41*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEHOUR,hInst,NULL);

				hBtnAllParamCfgTimeMinute= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeMinute"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 42*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEMINUTE,hInst,NULL);
				
				hBtnAllParamCfgTimeSecond= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeSecond"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 43*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMESECOND,hInst,NULL);
				
				hBtnAllParamCfgLiangDu= CreateWindow(_T("BUTTON"),_T("AllParamCfgLiangDu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 44*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_LIANGDU,hInst,NULL);

				hBtnAllParamCfgEquipmentModel = CreateWindow(_T("BUTTON"),_T("AllParamCfgSoftWare"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 45*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_EQUIPMENTMODEL,hInst,NULL);
				
				hBtnAllParamCfgHardWare= CreateWindow(_T("BUTTON"),_T("AllParamCfgHardWare"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 46*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_HARDWARE,hInst,NULL);

				hBtnAllParamCfgSoftWare= CreateWindow(_T("BUTTON"),_T("AllParamCfgSoftWare"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 47*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SOFTWARE,hInst,NULL);


				hBtnAllParamCfgNextAdjustTime = CreateWindow(_T("BUTTON"),_T("AllParamCfgNextAdjustTime"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 48*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_NEXTADJUSTTIME,hInst,NULL);

				hBtnAllParamCfgThisAdjustTime = CreateWindow(_T("BUTTON"),_T("AllParamCfgThisAdjustTime"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 49*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_THISADJUSTTIME,hInst,NULL);

				/*
				hBtnAllParamCfgZiDongBoGao = CreateWindow(_T("BUTTON"),_T("AllParamCfgZiDongBoGao"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 58*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_ZIDONGBOGAO,hInst,NULL);
				
				hBtnAllParamCfgQuXianFenDuan= CreateWindow(_T("BUTTON"),_T("AllParamCfgQuXianFenDuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 39*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_QUXIANFENDUAN,hInst,NULL);
				
				hBtnAllParamCfgPinPuFenXi= CreateWindow(_T("BUTTON"),_T("AllParamCfgPinPuFenXi"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 40*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PINPUFENXI,hInst,NULL);
				
				hBtnAllParamCfgXingNengJiaoYan= CreateWindow(_T("BUTTON"),_T("AllParamCfgXingNengJiaoYan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 41*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_XINGNENGJIAOYAN,hInst,NULL);
						
				hBtnAllParamCfgJuLiZuoBiao= CreateWindow(_T("BUTTON"),_T("AllParamCfgJuLiZuoBiao"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 42*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_JULIZUOBIAO,hInst,NULL);
				
				hBtnAllParamCfgLieWenQiBo= CreateWindow(_T("BUTTON"),_T("AllParamCfgLieWenQiBo"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 48*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_LIEWENQIBO,hInst,NULL);

				hBtnAllParamCfgPrintType= CreateWindow(_T("BUTTON"),_T("AllParamCfgPrintType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 47*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PRINTTYPE,hInst,NULL);

				
				hBtnAllParamCfgTongDaoQinLing= CreateWindow(_T("BUTTON"),_T("AllParamCfgTongDaoQinLing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 53*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TONGDAOQINLING,hInst,NULL);

				hBtnAllParamCfgPlusType = CreateWindow(_T("BUTTON"),_T("AllParamCfgPlusType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 59*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PLUSTYPE,hInst,NULL);*/

				allParamCfgBtnList[0] = hBtnAllParamCfgGongYiCanShu;
				//allParamCfgBtnList[1] = hBtnAllParamCfgCanShuQinLing;
				//allParamCfgBtnList[2] = hBtnAllParamCfgCanShuSuoDing;
				allParamCfgBtnList[1] = hBtnAllParamCfgWorkType;
				allParamCfgBtnList[2] = hBtnAllParamCfgTanTouType;
				//allParamCfgBtnList[5] = hBtnAllParamCfgProbeType;
				allParamCfgBtnList[3] = hBtnAllParamCfgAngle;
				allParamCfgBtnList[4] = hBtnAllParamCfgKValue;
				allParamCfgBtnList[5] = hBtnAllParamCfgVoltage;
				allParamCfgBtnList[6] = hBtnAllParamCfgMaiKuan;
				allParamCfgBtnList[7] = hBtnAllParamCfgRate;
				allParamCfgBtnList[8] = hBtnAllParamCfgAmend;
				allParamCfgBtnList[9] = hBtnAllParamCfgQianYan;
				allParamCfgBtnList[10] = hBtnAllParamCfgJianBo;
				allParamCfgBtnList[11] = hBtnAllParamCfgPlus;
				allParamCfgBtnList[12] = hBtnAllParamCfgRefPlus;
				allParamCfgBtnList[13] = hBtnAllParamCfgDelay;
				allParamCfgBtnList[14] = hBtnAllParamCfgLvBo;
				allParamCfgBtnList[15] = hBtnAllParamCfgZuKangPiPei;
				allParamCfgBtnList[16] = hBtnAllParamCfgRefractMode;
				allParamCfgBtnList[17] = hBtnAllParamCfgSpeedType;
				allParamCfgBtnList[18] = hBtnAllParamCfgSpeed;
				allParamCfgBtnList[19] = hBtnAllParamCfgRange;
				allParamCfgBtnList[20] = hBtnAllParamCfgBate;
				allParamCfgBtnList[21] = hBtnAllParamCfgGuanCaiWaiJing;
				allParamCfgBtnList[22] = hBtnAllParamCfgGuanCaiNeiJing;
				allParamCfgBtnList[23] = hBtnAllParamCfgPingDing;
				allParamCfgBtnList[24] = hBtnAllParamCfgDingLing;
				allParamCfgBtnList[25] = hBtnAllParamCfgPanFei;
				//allParamCfgBtnList[24] = hBtnAllParamCfgBiaoMianBuChang;
				allParamCfgBtnList[26] = hBtnAllParamCfgQuXianGuiGe1;
				allParamCfgBtnList[27] = hBtnAllParamCfgQuXianGuiGe2;
				allParamCfgBtnList[28] = hBtnAllParamCfgQuXianGuiGe3;
				allParamCfgBtnList[29] = hBtnAllParamCfgScanMode;
				//allParamCfgBtnList[34] = hBtnAllParamCfgBScanOnOrOff;
			//	allParamCfgBtnList[33] = hBtnAllParamCfgPlateHeight;
				allParamCfgBtnList[30] = hBtnAllParamCfgGongJianHouDu;
				allParamCfgBtnList[31] = hBtnAllParamCfgUpBlindValue;
				allParamCfgBtnList[32] = hBtnAllParamCfgDownBlindValue;
				allParamCfgBtnList[33] = hBtnAllParamCfgZhongHouBanTanShangSensitive;
				allParamCfgBtnList[34] = hBtnAllParamCfgPoKouType;
				allParamCfgBtnList[35] = hBtnAllParamCfgPoKouAngle;
				allParamCfgBtnList[36] = hBtnAllParamCfgPoKouDeep;
				allParamCfgBtnList[37] = hBtnAllParamCfgHanFengTanShangDistance;
				//allParamCfgBtnList[42] = hBtnAllParamCfgHanFengTanShangOnOrOff;
				allParamCfgBtnList[38] = hBtnAllParamCfgTimeYear;
				allParamCfgBtnList[39] = hBtnAllParamCfgTimeMonth;
				allParamCfgBtnList[40] = hBtnAllParamCfgTimeDay;
				allParamCfgBtnList[41] = hBtnAllParamCfgTimeHour;
				allParamCfgBtnList[42] = hBtnAllParamCfgTimeMinute;
				allParamCfgBtnList[43] = hBtnAllParamCfgTimeSecond;
				allParamCfgBtnList[44] = hBtnAllParamCfgLiangDu;
				allParamCfgBtnList[45] = hBtnAllParamCfgEquipmentModel;
				allParamCfgBtnList[46] = hBtnAllParamCfgHardWare;
				allParamCfgBtnList[47] = hBtnAllParamCfgSoftWare;
				allParamCfgBtnList[48] = hBtnAllParamCfgNextAdjustTime;
				allParamCfgBtnList[49] = hBtnAllParamCfgThisAdjustTime;
				
				/*
				allParamCfgBtnList[39] = hBtnAllParamCfgQuXianFenDuan;
				allParamCfgBtnList[40] = hBtnAllParamCfgPinPuFenXi;
				allParamCfgBtnList[41] = hBtnAllParamCfgXingNengJiaoYan;
				allParamCfgBtnList[42] = hBtnAllParamCfgJuLiZuoBiao;
				
				allParamCfgBtnList[45] = hBtnAllParamCfgXiangYingSpeed;
				
				allParamCfgBtnList[47] = hBtnAllParamCfgPrintType;
				allParamCfgBtnList[48] = hBtnAllParamCfgLieWenQiBo;

				allParamCfgBtnList[52] = hBtnAllParamCfgCanShuQinLing;
				allParamCfgBtnList[53] = hBtnAllParamCfgTongDaoQinLing;
				allParamCfgBtnList[54] = hBtnAllParamCfgCanShuSuoDing;

				allParamCfgBtnList[58] = hBtnAllParamCfgZiDongBoGao;
				allParamCfgBtnList[59] = hBtnAllParamCfgPlusType;
				allParamCfgBtnList[60] = hBtnAllParamCfgWorkType;*/


				//��ť�ؼ����Զ��崦�����

				OldBtnAllParamCfgGongYiCanShuProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgGongYiCanShu,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgGongYiCanShu,GWL_WNDPROC,(LONG)btnAllParamCfgGongYiCanShuProc);
				
				OldBtnAllParamCfgVoltageProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgVoltage,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgVoltage,GWL_WNDPROC,(LONG)btnAllParamCfgVoltageProc);
				
				OldBtnAllParamCfgMaiKuanProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgMaiKuan,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgMaiKuan,GWL_WNDPROC,(LONG)btnAllParamCfgMaiKuanProc);
				
				OldBtnAllParamCfgRateProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgRate,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgRate,GWL_WNDPROC,(LONG)btnAllParamCfgRateProc);

				OldBtnAllParamCfgQianYanProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgQianYan,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgQianYan,GWL_WNDPROC,(LONG)btnAllParamCfgQianYanProc);
				
				OldBtnAllParamCfgAmendProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgAmend,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgAmend,GWL_WNDPROC,(LONG)btnAllParamCfgAmendProc);
				
				OldBtnAllParamCfgJianBoProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgJianBo,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgJianBo,GWL_WNDPROC,(LONG)btnAllParamCfgJianBoProc);
				
				OldBtnAllParamCfgPlusProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPlus,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgPlus,GWL_WNDPROC,(LONG)btnAllParamCfgPlusProc);

				OldBtnAllParamCfgRefPlusProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgRefPlus,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgRefPlus,GWL_WNDPROC,(LONG)btnAllParamCfgRefPlusProc);
				
				OldBtnAllParamCfgDelayProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgDelay,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgDelay,GWL_WNDPROC,(LONG)btnAllParamCfgDelayProc);
				
				OldBtnAllParamCfgLvBoProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgLvBo,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgLvBo,GWL_WNDPROC,(LONG)btnAllParamCfgLvBoProc);

				OldBtnAllParamCfgRefractModeProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgRefractMode,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgRefractMode,GWL_WNDPROC,(LONG)btnAllParamCfgRefractModeProc);

				OldBtnAllParamCfgSpeedTypeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgSpeedType,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgSpeedType,GWL_WNDPROC,(LONG)btnAllParamCfgSpeedTypeProc);
				
				OldBtnAllParamCfgSpeedProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgSpeed,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgSpeed,GWL_WNDPROC,(LONG)btnAllParamCfgSpeedProc);
				
				OldBtnAllParamCfgRangeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgRange,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgRange,GWL_WNDPROC,(LONG)btnAllParamCfgRangeProc);
				
				OldBtnAllParamCfgAngleProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgAngle,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgAngle,GWL_WNDPROC,(LONG)btnAllParamCfgAngleProc);
				
				OldBtnAllParamCfgKValueProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgKValue,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgKValue,GWL_WNDPROC,(LONG)btnAllParamCfgKValueProc);
				
				
				OldBtnAllParamCfgBateProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgBate,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgBate,GWL_WNDPROC,(LONG)btnAllParamCfgBateProc);

				OldBtnAllParamCfgTimeYearProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeYear,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeYear,GWL_WNDPROC,(LONG)btnAllParamCfgTimeYearProc);

				OldBtnAllParamCfgTimeMonthProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeMonth,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeMonth,GWL_WNDPROC,(LONG)btnAllParamCfgTimeMonthProc);

				OldBtnAllParamCfgTimeDayProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeDay,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeDay,GWL_WNDPROC,(LONG)btnAllParamCfgTimeDayProc);

				OldBtnAllParamCfgTimeHourProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeHour,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeHour,GWL_WNDPROC,(LONG)btnAllParamCfgTimeHourProc);

				OldBtnAllParamCfgTimeMinuteProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeMinute,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeMinute,GWL_WNDPROC,(LONG)btnAllParamCfgTimeMinuteProc);

				OldBtnAllParamCfgTimeSecondProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeSecond,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeSecond,GWL_WNDPROC,(LONG)btnAllParamCfgTimeSecondProc);

				
				OldBtnAllParamCfgGongJianHouDuProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgGongJianHouDu,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgGongJianHouDu,GWL_WNDPROC,(LONG)btnAllParamCfgGongJianHouDuProc);

				
				OldBtnAllParamCfgScanModeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgScanMode,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgScanMode,GWL_WNDPROC,(LONG)btnAllParamCfgScanModeProc);

				OldBtnAllParamCfgBScanOnOrOffProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgBScanOnOrOff,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgBScanOnOrOff,GWL_WNDPROC,(LONG)btnAllParamCfgBScanOnOrOffProc);

			//	OldBtnAllParamCfgPlateHeightProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPlateHeight,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgPlateHeight,GWL_WNDPROC,(LONG)btnAllParamCfgPlateHeightProc);

				//OldBtnAllParamCfgProbeTypeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgProbeType,GWL_WNDPROC);
				//SetWindowLong(hBtnAllParamCfgProbeType,GWL_WNDPROC,(LONG)btnAllParamCfgProbeTypeProc);

				OldBtnAllParamCfgUpBlindValueProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgUpBlindValue,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgUpBlindValue,GWL_WNDPROC,(LONG)btnAllParamCfgUpBlindValueProc);

				OldBtnAllParamCfgDownBlindValueProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgDownBlindValue,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgDownBlindValue,GWL_WNDPROC,(LONG)btnAllParamCfgDownBlindValueProc);

				
				OldBtnAllParamCfgZhongHouBanTanShangSensitiveProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgZhongHouBanTanShangSensitive,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgZhongHouBanTanShangSensitive,GWL_WNDPROC,(LONG)btnAllParamCfgZhongHouBanTanShangSensitiveProc);

				OldBtnAllParamCfgPoKouTypeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPoKouType,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgPoKouType,GWL_WNDPROC,(LONG)btnAllParamCfgPoKouTypeProc);

				
				OldBtnAllParamCfgPoKouAngleProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPoKouAngle,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgPoKouAngle,GWL_WNDPROC,(LONG)btnAllParamCfgPoKouAngleProc);

				OldBtnAllParamCfgPoKouDeepProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPoKouDeep,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgPoKouDeep,GWL_WNDPROC,(LONG)btnAllParamCfgPoKouDeepProc);

				//OldBtnAllParamCfgHanFengTanShangHouDuProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgHanFengTanShangHouDu,GWL_WNDPROC);
				//SetWindowLong(hBtnAllParamCfgHanFengTanShangHouDu,GWL_WNDPROC,(LONG)btnAllParamCfgHanFengTanShangHouDuProc);

				OldBtnAllParamCfgHanFengTanShangDistanceProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgHanFengTanShangDistance,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgHanFengTanShangDistance,GWL_WNDPROC,(LONG)btnAllParamCfgHanFengTanShangDistanceProc);

				//OldBtnAllParamCfgHanFengTanShangOnOrOffProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgHanFengTanShangOnOrOff,GWL_WNDPROC);
				//SetWindowLong(hBtnAllParamCfgHanFengTanShangOnOrOff,GWL_WNDPROC,(LONG)btnAllParamCfgHanFengTanShangOnOrOffProc);
				
				OldBtnAllParamCfgLiangDuProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgLiangDu,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgLiangDu,GWL_WNDPROC,(LONG)btnAllParamCfgLiangDuProc);

				OldBtnAllParamCfgZuKangPiPeiProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgZuKangPiPei,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgZuKangPiPei,GWL_WNDPROC,(LONG)btnAllParamCfgZuKangPiPeiProc);

				//OldBtnAllParamCfgQuXianFenDuanProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgQuXianFenDuan,GWL_WNDPROC);
				//SetWindowLong(hBtnAllParamCfgQuXianFenDuan,GWL_WNDPROC,(LONG)btnAllParamCfgQuXianFenDuanProc);


				//OldBtnAllParamCfgPinPuFenXiProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPinPuFenXi,GWL_WNDPROC);
				//SetWindowLong(hBtnAllParamCfgPinPuFenXi,GWL_WNDPROC,(LONG)btnAllParamCfgPinPuFenXiProc);

				//OldBtnAllParamCfgXingNengJiaoYanProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgXingNengJiaoYan,GWL_WNDPROC);
				//SetWindowLong(hBtnAllParamCfgXingNengJiaoYan,GWL_WNDPROC,(LONG)btnAllParamCfgXingNengJiaoYanProc);

				

				//OldBtnAllParamCfgJuLiZuoBiaoProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgJuLiZuoBiao,GWL_WNDPROC);
				//SetWindowLong(hBtnAllParamCfgJuLiZuoBiao,GWL_WNDPROC,(LONG)btnAllParamCfgJuLiZuoBiaoProc);

				OldBtnAllParamCfgGuanCaiWaiJingProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgGuanCaiWaiJing,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgGuanCaiWaiJing,GWL_WNDPROC,(LONG)btnAllParamCfgGuanCaiWaiJingProc);

				OldBtnAllParamCfgGuanCaiNeiJingProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgGuanCaiNeiJing,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgGuanCaiNeiJing,GWL_WNDPROC,(LONG)btnAllParamCfgGuanCaiNeiJingProc);

				
			//	OldBtnAllParamCfgXiangYingSpeedProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgXiangYingSpeed,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgXiangYingSpeed,GWL_WNDPROC,(LONG)btnAllParamCfgXiangYingSpeedProc);

				
			//	OldBtnAllParamCfgBiaoMianBuChangProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgBiaoMianBuChang,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgBiaoMianBuChang,GWL_WNDPROC,(LONG)btnAllParamCfgBiaoMianBuChangProc);


			//	OldBtnAllParamCfgPrintTypeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPrintType,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgPrintType,GWL_WNDPROC,(LONG)btnAllParamCfgPrintTypeProc);

			//	OldBtnAllParamCfgLieWenQiBoProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgLieWenQiBo,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgLieWenQiBo,GWL_WNDPROC,(LONG)btnAllParamCfgLieWenQiBoProc);


				OldBtnAllParamCfgPingDingProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPingDing,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgPingDing,GWL_WNDPROC,(LONG)btnAllParamCfgPingDingProc);

				
				OldBtnAllParamCfgDingLiangProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgDingLing,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgDingLing,GWL_WNDPROC,(LONG)btnAllParamCfgDingLiangProc);

				OldBtnAllParamCfgPanFeiProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgPanFei,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgPanFei,GWL_WNDPROC,(LONG)btnAllParamCfgPanFeiProc);

				
				OldBtnAllParamCfgTanTouTypeProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgTanTouType,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTanTouType,GWL_WNDPROC,(LONG)btnAllParamCfgTanTouTypeProc);

				
				OldBtnAllParamCfgCanShuQinLingProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgCanShuQinLing,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgCanShuQinLing,GWL_WNDPROC,(LONG)btnAllParamCfgCanShuQinLingProc);

			//	OldBtnAllParamCfgTongDaoQinLingProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgTongDaoQinLing,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgTongDaoQinLing,GWL_WNDPROC,(LONG)btnAllParamCfgTongDaoQinLingProc);

			//	OldBtnAllParamCfgCanShuSuoDingProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgCanShuSuoDing,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgCanShuSuoDing,GWL_WNDPROC,(LONG)btnAllParamCfgCanShuSuoDingProc);

				
				OldBtnAllParamCfgQuXianGuiGe1Proc = (WNDPROC) GetWindowLong(hBtnAllParamCfgQuXianGuiGe1,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgQuXianGuiGe1,GWL_WNDPROC,(LONG)btnAllParamCfgQuXianGuiGe1Proc);

				OldBtnAllParamCfgQuXianGuiGe2Proc = (WNDPROC) GetWindowLong(hBtnAllParamCfgQuXianGuiGe2,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgQuXianGuiGe2,GWL_WNDPROC,(LONG)btnAllParamCfgQuXianGuiGe2Proc);

				OldBtnAllParamCfgQuXianGuiGe3Proc = (WNDPROC) GetWindowLong(hBtnAllParamCfgQuXianGuiGe3,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgQuXianGuiGe3,GWL_WNDPROC,(LONG)btnAllParamCfgQuXianGuiGe3Proc);

				
			//	OldBtnAllParamCfgZiDongBoGaoProc= (WNDPROC) GetWindowLong(hBtnAllParamCfgZiDongBoGao,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgZiDongBoGao,GWL_WNDPROC,(LONG)btnAllParamCfgZiDongBoGaoProc);

										
			//	OldBtnAllParamCfgPlusTypeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPlusType,GWL_WNDPROC);
			//	SetWindowLong(hBtnAllParamCfgPlusType,GWL_WNDPROC,(LONG)btnAllParamCfgPlusTypeProc);

				OldBtnAllParamCfgWorkTypeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgWorkType,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgWorkType,GWL_WNDPROC,(LONG)btnAllParamCfgWorkTypeProc);

				OldBtnAllParamCfgHardWareProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgHardWare,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgHardWare,GWL_WNDPROC,(LONG)btnAllParamCfgHardWareProc);

				OldBtnAllParamCfgSoftWareProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgSoftWare,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgSoftWare,GWL_WNDPROC,(LONG)btnAllParamCfgSoftWareProc);

				OldBtnAllParamCfgEquipmentModelProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgEquipmentModel,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgEquipmentModel,GWL_WNDPROC,(LONG)btnAllParamCfgEquipmentModelProc);

				OldBtnAllParamCfgNextAdjustTimeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgNextAdjustTime,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgNextAdjustTime,GWL_WNDPROC,(LONG)btnAllParamCfgNextAdjustTimeProc);

				OldBtnAllParamCfgThisAdjustTimeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgThisAdjustTime,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgThisAdjustTime,GWL_WNDPROC,(LONG)btnAllParamCfgThisAdjustTimeProc);

				
			}
			else if(gwDeviceType == 1)
			{
				buttonNum = 28;
				hBtnAllParamCfgGongYiCanShu= CreateWindow(_T("BUTTON"),_T("AllParamCfgGongYiCanShu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 ,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_GONGYICANSHU,hInst,NULL);

				hBtnAllParamCfgVoltage = CreateWindow(_T("BUTTON"),_T("AllParamCfgVoltage"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 1*EACH_ITEM_HEIGHT, EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_VOLTAGE,hInst,NULL);
			
				hBtnAllParamCfgMaiKuan = CreateWindow(_T("BUTTON"),_T("AllParamCfgMaiKuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_MAIKUAN,hInst,NULL);
			
				hBtnAllParamCfgRate = CreateWindow(_T("BUTTON"),_T("AllParamCfgRate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_RATE,hInst,NULL);
			
				hBtnAllParamCfgAmend = CreateWindow(_T("BUTTON"),_T("AllParamCfgAmend"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_AMEND,hInst,NULL);
			
				hBtnAllParamCfgJianBo= CreateWindow(_T("BUTTON"),_T("AllParamCfgJianBo"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 5*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_JIANBO,hInst,NULL);
			
				hBtnAllParamCfgPlus = CreateWindow(_T("BUTTON"),_T("AllParamCfgPlus"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 6*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_PLUS,hInst,NULL);

				hBtnAllParamCfgRefPlus = CreateWindow(_T("BUTTON"),_T("AllParamCfgRefPlus"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 7*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_REFPLUS,hInst,NULL);
			
				hBtnAllParamCfgDelay = CreateWindow(_T("BUTTON"),_T("AllParamCfgDelay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 8*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_DELAY,hInst,NULL);
				
				hBtnAllParamCfgLvBo = CreateWindow(_T("BUTTON"),_T("AllParamCfgLvBo"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 9*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_LVBO,hInst,NULL);

				hBtnAllParamCfgZuKangPiPei= CreateWindow(_T("BUTTON"),_T("AllParamCfgZuKangPiPei"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 10*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_ZUKANGPIPEI,hInst,NULL);

				hBtnAllParamCfgSpeedType = CreateWindow(_T("BUTTON"),_T("AllParamCfgSpeedType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 11*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SPEEDTYPE,hInst,NULL);
				
				hBtnAllParamCfgSpeed = CreateWindow(_T("BUTTON"),_T("AllParamCfgSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 12*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SPEED,hInst,NULL);
			
				hBtnAllParamCfgRange = CreateWindow(_T("BUTTON"),_T("AllParamCfgRange"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 13*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_RANGE,hInst,NULL);

				hBtnAllParamCfgBate = CreateWindow(_T("BUTTON"),_T("AllParamCfgBate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 14*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_BATE,hInst,NULL);
		
				hBtnAllParamCfgScanMode= CreateWindow(_T("BUTTON"),_T("AllParamCfgScanMode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 15*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SCANMODE,hInst,NULL);
		
				hBtnAllParamCfgTimeYear = CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeYear"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 16*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEYEAR,hInst,NULL);
				
				hBtnAllParamCfgTimeMonth= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeMonth"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 17*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEMONTH,hInst,NULL);

				hBtnAllParamCfgTimeDay= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeDay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 18*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEDAY,hInst,NULL);
				
				hBtnAllParamCfgTimeHour= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeHour"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 19*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEHOUR,hInst,NULL);

				hBtnAllParamCfgTimeMinute= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeMinute"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 20*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMEMINUTE,hInst,NULL);
				
				hBtnAllParamCfgTimeSecond= CreateWindow(_T("BUTTON"),_T("AllParamCfgTimeSecond"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 21*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_TIMESECOND,hInst,NULL);
				
				hBtnAllParamCfgLiangDu= CreateWindow(_T("BUTTON"),_T("AllParamCfgLiangDu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 22*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_LIANGDU,hInst,NULL);

				hBtnAllParamCfgEquipmentModel = CreateWindow(_T("BUTTON"),_T("AllParamCfgSoftWare"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 23*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_EQUIPMENTMODEL,hInst,NULL);
				
				hBtnAllParamCfgHardWare= CreateWindow(_T("BUTTON"),_T("AllParamCfgHardWare"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 24*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_HARDWARE,hInst,NULL);

				hBtnAllParamCfgSoftWare= CreateWindow(_T("BUTTON"),_T("AllParamCfgSoftWare"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 25*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_SOFTWARE,hInst,NULL);

				hBtnAllParamCfgNextAdjustTime = CreateWindow(_T("BUTTON"),_T("AllParamCfgNextAdjustTime"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 26*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_NEXTADJUSTTIME,hInst,NULL);

				hBtnAllParamCfgThisAdjustTime = CreateWindow(_T("BUTTON"),_T("AllParamCfgThisAdjustTime"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 27*EACH_ITEM_HEIGHT,EACH_ITEM_WIDTH,EACH_ITEM_HEIGHT,hWnd,(HMENU)IDC_ALLPARAMCFG_THISADJUSTTIME,hInst,NULL);

				allParamCfgBtnList[0] = hBtnAllParamCfgGongYiCanShu;
				allParamCfgBtnList[1] = hBtnAllParamCfgVoltage;
				allParamCfgBtnList[2] = hBtnAllParamCfgMaiKuan;
				allParamCfgBtnList[3] = hBtnAllParamCfgRate;
				allParamCfgBtnList[4] = hBtnAllParamCfgAmend;
				allParamCfgBtnList[5] = hBtnAllParamCfgJianBo;
				allParamCfgBtnList[6] = hBtnAllParamCfgPlus;
				allParamCfgBtnList[7] = hBtnAllParamCfgRefPlus;
				allParamCfgBtnList[8] = hBtnAllParamCfgDelay;
				allParamCfgBtnList[9] = hBtnAllParamCfgLvBo;
				allParamCfgBtnList[10] = hBtnAllParamCfgZuKangPiPei;
				allParamCfgBtnList[11] = hBtnAllParamCfgSpeedType;
				allParamCfgBtnList[12] = hBtnAllParamCfgSpeed;
				allParamCfgBtnList[13] = hBtnAllParamCfgRange;
				allParamCfgBtnList[14] = hBtnAllParamCfgBate;
				allParamCfgBtnList[15] = hBtnAllParamCfgScanMode;
				allParamCfgBtnList[16] = hBtnAllParamCfgTimeYear;
				allParamCfgBtnList[17] = hBtnAllParamCfgTimeMonth;
				allParamCfgBtnList[18] = hBtnAllParamCfgTimeDay;
				allParamCfgBtnList[19] = hBtnAllParamCfgTimeHour;
				allParamCfgBtnList[20] = hBtnAllParamCfgTimeMinute;
				allParamCfgBtnList[21] = hBtnAllParamCfgTimeSecond;
				allParamCfgBtnList[22] = hBtnAllParamCfgLiangDu;
				allParamCfgBtnList[23] = hBtnAllParamCfgEquipmentModel;
				allParamCfgBtnList[24] = hBtnAllParamCfgHardWare;
				allParamCfgBtnList[25] = hBtnAllParamCfgSoftWare;
				allParamCfgBtnList[26] = hBtnAllParamCfgNextAdjustTime;
				allParamCfgBtnList[27] = hBtnAllParamCfgThisAdjustTime;

				//��ť�ؼ����Զ��崦�����
				OldBtnAllParamCfgGongYiCanShuProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgGongYiCanShu,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgGongYiCanShu,GWL_WNDPROC,(LONG)btnAllParamCfgGongYiCanShuProc);
				
				OldBtnAllParamCfgVoltageProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgVoltage,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgVoltage,GWL_WNDPROC,(LONG)btnAllParamCfgVoltageProc);
				
				OldBtnAllParamCfgMaiKuanProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgMaiKuan,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgMaiKuan,GWL_WNDPROC,(LONG)btnAllParamCfgMaiKuanProc);
				
				OldBtnAllParamCfgRateProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgRate,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgRate,GWL_WNDPROC,(LONG)btnAllParamCfgRateProc);
				
				OldBtnAllParamCfgAmendProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgAmend,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgAmend,GWL_WNDPROC,(LONG)btnAllParamCfgAmendProc);
				
				OldBtnAllParamCfgJianBoProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgJianBo,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgJianBo,GWL_WNDPROC,(LONG)btnAllParamCfgJianBoProc);
				
				OldBtnAllParamCfgPlusProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgPlus,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgPlus,GWL_WNDPROC,(LONG)btnAllParamCfgPlusProc);

				OldBtnAllParamCfgRefPlusProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgRefPlus,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgRefPlus,GWL_WNDPROC,(LONG)btnAllParamCfgRefPlusProc);
				
				OldBtnAllParamCfgDelayProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgDelay,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgDelay,GWL_WNDPROC,(LONG)btnAllParamCfgDelayProc);
				
				OldBtnAllParamCfgLvBoProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgLvBo,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgLvBo,GWL_WNDPROC,(LONG)btnAllParamCfgLvBoProc);

				OldBtnAllParamCfgSpeedTypeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgSpeedType,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgSpeedType,GWL_WNDPROC,(LONG)btnAllParamCfgSpeedTypeProc);
				
				OldBtnAllParamCfgSpeedProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgSpeed,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgSpeed,GWL_WNDPROC,(LONG)btnAllParamCfgSpeedProc);
				
				OldBtnAllParamCfgRangeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgRange,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgRange,GWL_WNDPROC,(LONG)btnAllParamCfgRangeProc);				
				
				OldBtnAllParamCfgBateProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgBate,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgBate,GWL_WNDPROC,(LONG)btnAllParamCfgBateProc);

				OldBtnAllParamCfgTimeYearProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeYear,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeYear,GWL_WNDPROC,(LONG)btnAllParamCfgTimeYearProc);

				OldBtnAllParamCfgTimeMonthProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeMonth,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeMonth,GWL_WNDPROC,(LONG)btnAllParamCfgTimeMonthProc);

				OldBtnAllParamCfgTimeDayProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeDay,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeDay,GWL_WNDPROC,(LONG)btnAllParamCfgTimeDayProc);

				OldBtnAllParamCfgTimeHourProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeHour,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeHour,GWL_WNDPROC,(LONG)btnAllParamCfgTimeHourProc);

				OldBtnAllParamCfgTimeMinuteProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeMinute,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeMinute,GWL_WNDPROC,(LONG)btnAllParamCfgTimeMinuteProc);

				OldBtnAllParamCfgTimeSecondProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgTimeSecond,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgTimeSecond,GWL_WNDPROC,(LONG)btnAllParamCfgTimeSecondProc);
				
				OldBtnAllParamCfgScanModeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgScanMode,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgScanMode,GWL_WNDPROC,(LONG)btnAllParamCfgScanModeProc);
				
				OldBtnAllParamCfgLiangDuProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgLiangDu,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgLiangDu,GWL_WNDPROC,(LONG)btnAllParamCfgLiangDuProc);

				OldBtnAllParamCfgZuKangPiPeiProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgZuKangPiPei,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgZuKangPiPei,GWL_WNDPROC,(LONG)btnAllParamCfgZuKangPiPeiProc);

				OldBtnAllParamCfgHardWareProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgHardWare,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgHardWare,GWL_WNDPROC,(LONG)btnAllParamCfgHardWareProc);

				OldBtnAllParamCfgSoftWareProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgSoftWare,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgSoftWare,GWL_WNDPROC,(LONG)btnAllParamCfgSoftWareProc);

				OldBtnAllParamCfgEquipmentModelProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgEquipmentModel,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgEquipmentModel,GWL_WNDPROC,(LONG)btnAllParamCfgEquipmentModelProc);

				OldBtnAllParamCfgNextAdjustTimeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgNextAdjustTime,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgNextAdjustTime,GWL_WNDPROC,(LONG)btnAllParamCfgNextAdjustTimeProc);

				OldBtnAllParamCfgThisAdjustTimeProc = (WNDPROC) GetWindowLong(hBtnAllParamCfgThisAdjustTime,GWL_WNDPROC);
				SetWindowLong(hBtnAllParamCfgThisAdjustTime,GWL_WNDPROC,(LONG)btnAllParamCfgThisAdjustTimeProc);

			}
			else
			{
				;//
			}
			currentAllParamCfgBtn = allParamCfgBtnList[0];//Ĭ�ϵ�ǰѡ�й��ղ�����ť
			SetFocus(currentAllParamCfgBtn);//ѡ�а�ť������뽹��
			// songchenguang add start 2010-12-20
			nButtonIndex = 0;
			// songchenguang add end 2010-12-20

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
		//	RECT rt;
		//	GetClientRect(hWnd, &rt);
		//	DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_CENTER);
			//����ť�ؼ�
		/*	wchar_t strText[15];
			memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("���ղ��� %d"), nParamNo);
			
			drawSrting(hWnd,strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								220, STRING_START_Y,150, STRING_HEIGHT);

			drawSrting(hWnd,_T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								450, STRING_START_Y,50, STRING_HEIGHT);

			allCfgDisplayStep(hWnd, fStep);
			*/
			EndPaint(hWnd, &ps);
			break;
		case WM_DRAWITEM://�ػ���ť
			pdis=(LPDRAWITEMSTRUCT)lParam;
			allParamCfg_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnAllParamCfgGongYiCanShu)
			{
				drawSrting(pdis->hwndItem, _T("�������"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
							STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayGongYiCanShu(pdis->hwndItem,gwGongYiCanShu);
			
			}			
			else if(pdis->hwndItem == hBtnAllParamCfgVoltage)
			{
				drawSrting(pdis->hwndItem, _T("�������           ��ѹ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayVoltage(pdis->hwndItem, Send_AllParamConfigWith_ad[wChannelNo].nPress);
				//drawSrting(pdis->hwndItem, _T("V"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
				//				DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);

			}
			else if(pdis->hwndItem == hBtnAllParamCfgQianYan)
			{
				drawSrting(pdis->hwndItem, _T("ǰ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				if(nTanTouType[wChannelNo] == 0)
				{
					allCfgDisplayFloat1(pdis->hwndItem, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
				}
				else
				{
					allCfgDisplayQianYan(pdis->hwndItem, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
				}
				
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgMaiKuan)
			{
				drawSrting(pdis->hwndItem,  _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayMaiKuan(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("ns"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgRate)
			{
				drawSrting(pdis->hwndItem,  _T("�ظ�Ƶ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayRate(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("Hz"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgAmend)
			{
				drawSrting(pdis->hwndItem,  _T("���"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayAmend(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��s"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgJianBo)
			{
				drawSrting(pdis->hwndItem,  _T("���ղ���     �첨��ʽ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayJianBo(pdis->hwndItem,Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgPlus)
			{
				drawSrting(pdis->hwndItem,  _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayPlus(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgRefPlus)
			{
				drawSrting(pdis->hwndItem,  _T("�ο�����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayRefPlus(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgDelay)
			{
				drawSrting(pdis->hwndItem,  _T("�ӳ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayDelay(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��s"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgLvBo)
			{
				drawSrting(pdis->hwndItem,  _T("�˲�Ƶ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayLvBo(pdis->hwndItem, Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq);
				drawSrting(pdis->hwndItem, _T("Hz"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgRefractMode)
			{
				drawSrting(pdis->hwndItem,  _T("��ʾ����      S/Xת��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayRefractMode(pdis->hwndItem);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgSpeedType)
			{
				drawSrting(pdis->hwndItem,  _T("���ٲ���"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);				
				allCfgDisplaySpeedType(pdis->hwndItem, wMaterialType);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgSpeed)
			{
				drawSrting(pdis->hwndItem,  _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplaySpeed(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("m/s"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgRange)
			{
				drawSrting(pdis->hwndItem,  _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayRange(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgAngle)
			{
				drawSrting(pdis->hwndItem,  _T("�����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayAngle(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgKValue)
			{
				drawSrting(pdis->hwndItem,  _T("Kֵ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayKValue(pdis->hwndItem, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgBate)
			{
				drawSrting(pdis->hwndItem,  _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayBate(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("%"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgTimeYear)
			{
				drawSrting(pdis->hwndItem, _T("ʱ������                "), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayTimeYear(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgTimeMonth)
			{

				allCfgDisplayTimeMonth(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgTimeDay)
			{
				/*drawSrting(pdis->hwndItem, _T(""), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);*/
				allCfgDisplayTimeDay(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgTimeHour)
			{
				/*drawSrting(pdis->hwndItem, _T(""), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);*/
				allCfgDisplayTimeHour(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("ʱ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgTimeMinute)
			{
				/*drawSrting(pdis->hwndItem, _T(""), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);*/
				allCfgDisplayTimeMinute(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgTimeSecond)
			{
				/*drawSrting(pdis->hwndItem, _T(""), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);*/
				allCfgDisplayTimeSecond(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgGongJianHouDu)
			{
				drawSrting(pdis->hwndItem, _T("�������"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayFloat1(pdis->hwndItem, allParamPlateHeight);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgScanMode)
			{
				drawSrting(pdis->hwndItem, _T("ɨ�跽ʽ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayScanMode(pdis->hwndItem,allParamScanMode);
				/*drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);*/
			}

		/*	if(pdis->hwndItem == hBtnAllParamCfgBScanOnOrOff)
			{
				drawSrting(pdis->hwndItem, _T("ɨ�迪��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayBScanOnOrOff(pdis->hwndItem,allParamBscanSwitch);
				/*drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}*/

		//	if(pdis->hwndItem == hBtnAllParamCfgPlateHeight)
		//	{
		//		drawSrting(pdis->hwndItem, _T("�к��̽��   �������"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
		//						STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
		//		allCfgDisplayPlateHeight(pdis->hwndItem);
		//		drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
		//						DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
		//	}
		/*	if(pdis->hwndItem == hBtnAllParamCfgProbeType)
			{
				drawSrting(pdis->hwndItem, _T("̽ͷ�ͺ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayProbeType(pdis->hwndItem,allParamProbeType);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}*/

			else if(pdis->hwndItem == hBtnAllParamCfgUpBlindValue)
			{
				drawSrting(pdis->hwndItem, _T("��ä��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayUpBlindValue(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgDownBlindValue)
			{
				drawSrting(pdis->hwndItem, _T("��ä��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayDownBlindValue(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}
			else if(pdis->hwndItem == hBtnAllParamCfgZhongHouBanTanShangSensitive)
			{
				drawSrting(pdis->hwndItem, _T("�����ȼ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayZhongHouBanTanShangSensitive(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgPoKouType)
			{
				drawSrting(pdis->hwndItem, _T("����̽��    �¿�����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayPoKouType(pdis->hwndItem,allParamPoKouLeiXing);
				
			}
			else if(pdis->hwndItem == hBtnAllParamCfgPoKouAngle)
			{
				drawSrting(pdis->hwndItem, _T("�¿ڽǶ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayPoKouAngle(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}
			else if(pdis->hwndItem == hBtnAllParamCfgPoKouDeep)
			{
				drawSrting(pdis->hwndItem, _T("�¿����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayPoKouDeep(pdis->hwndItem,allParamPoKouLeiXing);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}
			/*if(pdis->hwndItem == hBtnAllParamCfgHanFengTanShangHouDu)
			{
				drawSrting(pdis->hwndItem, _T("���칤�����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayHanFengTanShangHouDu(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}*/
			else if(pdis->hwndItem == hBtnAllParamCfgHanFengTanShangDistance)
			{
				drawSrting(pdis->hwndItem, _T("̽ͷ<->�¿�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayHanFengTanShangDistance(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}

		/*	if(pdis->hwndItem == hBtnAllParamCfgHanFengTanShangOnOrOff)
			{
				drawSrting(pdis->hwndItem, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayHanFengTanShangOnOrOff(pdis->hwndItem,allParamHanFengTanShangSwitch);
				
				
			}*/
			else if(pdis->hwndItem == hBtnAllParamCfgLiangDu)
			{
				drawSrting(pdis->hwndItem, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayLiangDu(pdis->hwndItem);				
			}
			else if(pdis->hwndItem == hBtnAllParamCfgZuKangPiPei)
			{
				drawSrting(pdis->hwndItem, _T("�迹ƥ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
											STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgdisplayImpedance(pdis->hwndItem,wChannelNo, Receive_AllParamConfigWith_ad[wChannelNo].wImpedance);
				
			}

		/*	if(pdis->hwndItem == hBtnAllParamCfgQuXianFenDuan)
			{
				drawSrting(pdis->hwndItem, _T("���߷ֶ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayQuXianFenDuan(pdis->hwndItem);				
			}
			if(pdis->hwndItem == hBtnAllParamCfgPinPuFenXi)
			{
				drawSrting(pdis->hwndItem, _T("Ƶ�׷���"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayPinPuFenXi(pdis->hwndItem);				
			}
			if(pdis->hwndItem == hBtnAllParamCfgXingNengJiaoYan)
			{
				drawSrting(pdis->hwndItem, _T("����У��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayXingNengJiaoYan(pdis->hwndItem);				
			}
			if(pdis->hwndItem == hBtnAllParamCfgJuLiZuoBiao)
			{
				drawSrting(pdis->hwndItem, _T("��������"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayJuLiZuoBiao(pdis->hwndItem);				
			}*/
			else if(pdis->hwndItem == hBtnAllParamCfgGuanCaiWaiJing)
			{
				drawSrting(pdis->hwndItem, _T("�ܲ��⾶"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayGuanCaiWaiJing(pdis->hwndItem, gfParamGuanCaiWaiJing);	
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			if(pdis->hwndItem == hBtnAllParamCfgGuanCaiNeiJing)
			{
				drawSrting(pdis->hwndItem, _T("�ܲ��ھ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayGuanCaiNeiJing(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}

			/*if(pdis->hwndItem == hBtnAllParamCfgXiangYingSpeed)
			{
				drawSrting(pdis->hwndItem, _T("��Ӧ�ٶ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayXiangYingSpeed(pdis->hwndItem);
				
			}*/
			
		/*	if(pdis->hwndItem == hBtnAllParamCfgBiaoMianBuChang)
			{
				drawSrting(pdis->hwndItem, _T("���油��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayBiaoMianBuChang(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}*/
		/*	if(pdis->hwndItem == hBtnAllParamCfgPrintType)
			{
				drawSrting(pdis->hwndItem, _T("��ӡ����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayPrintType(pdis->hwndItem);
				
			}
			if(pdis->hwndItem == hBtnAllParamCfgLieWenQiBo)
			{
				drawSrting(pdis->hwndItem, _T("������"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayLieWenQiBo(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}*/

			else if(pdis->hwndItem == hBtnAllParamCfgPingDing)
			{
				drawSrting(pdis->hwndItem, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayFloat1(pdis->hwndItem,gfParamPingDing[wChannelNo]);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}
			else if(pdis->hwndItem == hBtnAllParamCfgDingLing)
			{
				drawSrting(pdis->hwndItem, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayFloat1(pdis->hwndItem,gfParamDingLiang[wChannelNo]);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}
			else if(pdis->hwndItem == hBtnAllParamCfgPanFei)
			{
				drawSrting(pdis->hwndItem, _T("�з�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayFloat1(pdis->hwndItem,gfParamPanFei[wChannelNo]);
				drawSrting(pdis->hwndItem, _T("dB"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
			}
			else if(pdis->hwndItem == hBtnAllParamCfgTanTouType)
			{
				drawSrting(pdis->hwndItem, _T("̽ͷ����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayTanTouType(pdis->hwndItem);	
			}
			/*if(pdis->hwndItem == hBtnAllParamCfgCanShuQinLing)
			{
				drawSrting(pdis->hwndItem, _T("������λ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayCanShuQinLing(pdis->hwndItem);
				
				
			}*/
		/*	if(pdis->hwndItem == hBtnAllParamCfgTongDaoQinLing)
			{
				drawSrting(pdis->hwndItem, _T("ͨ������"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayTongDaoQinLing(pdis->hwndItem);
				
				
			}*/
		/*	if(pdis->hwndItem == hBtnAllParamCfgCanShuSuoDing)
			{
				drawSrting(pdis->hwndItem, _T("��������"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayCanShuSuoDing(pdis->hwndItem);
				
				
			}	*/
			else if(pdis->hwndItem == hBtnAllParamCfgQuXianGuiGe1)
			{
				drawSrting(pdis->hwndItem, _T("���ߦ�1ֵ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayFloat1(pdis->hwndItem, gfParamFaiZhi1[wChannelNo]);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgQuXianGuiGe2)
			{
				drawSrting(pdis->hwndItem, _T("���ߦ�2ֵ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayFloat1(pdis->hwndItem, gfParamFaiZhi2[wChannelNo]);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgQuXianGuiGe3)
			{
				drawSrting(pdis->hwndItem, _T("���ߦ�3ֵ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayFloat1(pdis->hwndItem,gfParamFaiZhi3[wChannelNo]);
				drawSrting(pdis->hwndItem, _T("mm"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
			}
		/*	if(pdis->hwndItem == hBtnAllParamCfgZiDongBoGao)
			{
				drawSrting(pdis->hwndItem, _T("�Զ�����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayZiDongBoGao(pdis->hwndItem);
				drawSrting(pdis->hwndItem, _T("%"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								DANWEI_START_X, STRING_START_Y,DANWEI_WIDTH, STRING_HEIGHT);
				
				
			}
			if(pdis->hwndItem == hBtnAllParamCfgPlusType)
			{
				drawSrting(pdis->hwndItem, _T("���淽ʽ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayPlusType(pdis->hwndItem);
				
				
			}*/
			else if(pdis->hwndItem == hBtnAllParamCfgWorkType)
			{
				drawSrting(pdis->hwndItem, _T("������ʽ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayWorkType(pdis->hwndItem,gnWorkType);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgHardWare)
			{
				drawSrting(pdis->hwndItem, _T("Ӳ���汾��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayHardWare(pdis->hwndItem);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgSoftWare)
			{
				drawSrting(pdis->hwndItem, _T("����汾��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplaySoftWare(pdis->hwndItem);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgNextAdjustTime)
			{
				drawSrting(pdis->hwndItem, _T("�´�У׼ʱ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayNextAdjustTime(pdis->hwndItem);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgThisAdjustTime)
			{
				drawSrting(pdis->hwndItem, _T("����У׼ʱ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayThisAdjustTime(pdis->hwndItem);
			}
			else if(pdis->hwndItem == hBtnAllParamCfgEquipmentModel)
			{
				drawSrting(pdis->hwndItem, _T("�豸�ͺ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 
								STRING_START_X, STRING_START_Y,STRING_WIDTH, STRING_HEIGHT);
				allCfgDisplayEquipmentModel(pdis->hwndItem);
			}
			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentAllParamCfgBtn)
			{
				if(pdis->hwndItem == hBtnAllParamCfgMaiKuan)
				{
					allCfgDisplayStepInt(hWndAllParamCfgCaption,nMaiKuanStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgQianYan)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fParamQianYanStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgAmend)
				{
					allCfgDisplayStepFloat2(hWndAllParamCfgCaption, fAmendStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgPlus || pdis->hwndItem == hBtnAllParamCfgRefPlus)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fPlusStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgDelay)
				{
					allCfgDisplayStepFloat2(hWndAllParamCfgCaption, fDelayStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgRange)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fRangeStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgSpeed)
				{
					allCfgDisplayStepInt(hWndAllParamCfgCaption, nWaveSpeedStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgAngle)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fZheSheAngleStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgKValue)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fKValueStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgBate)
				{
					allCfgDisplayStepInt(hWndAllParamCfgCaption, nBateStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgGongJianHouDu)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PlateHeightStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgPlateHeight)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PlateHeightStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgPoKouAngle)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PoKouAngleStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgPoKouDeep)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PoKouDeepStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgHanFengTanShangHouDu)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption,HanFengTanShangHouDuStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgHanFengTanShangDistance)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption,HanFengTanShangDistanceStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgGuanCaiWaiJing)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption,fWaiJingStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgGuanCaiNeiJing)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption,fNeiJingStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgUpBlindValue)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption,BlindValueStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgDownBlindValue)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption,BlindValueStep);
				}
				else if(pdis->hwndItem == hBtnAllParamCfgZhongHouBanTanShangSensitive)
				{
					allCfgDisplayStepFloat(hWndAllParamCfgCaption,fPlusStep);
				}
				else
				{
					DrawLableText1(hWndAllParamCfgCaption, _T(""), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 510, ALLCFG_LABEL_START_Y, 100, ALLCFG_LABEL_HEIGHT);
				}

			}
			break;
		case WM_COMMAND:
			break;
		case WM_CLOSE:
		
			break;
		case WM_SIZE:
			si.cbSize = sizeof (si) ;
			si.fMask  = SIF_RANGE;
			si.nMin   = 0 ;
			si.nMax   = buttonNum - 1;
			SetScrollInfo (hWnd, SB_VERT, &si, TRUE) ;
			return 0 ;
		case WM_KEYDOWN:
          switch (wParam)
          {
	          case VK_UP:    
				  if(nButtonIndex > 1)
					nButtonIndex -- ;
				  else
					  nButtonIndex = 0 ;
				  break ;
	          case VK_DOWN: 
				  if(nButtonIndex < buttonNum - 1)
					  nButtonIndex++;
				  else
					  nButtonIndex = buttonNum - 1;
				  break ;
	          case VK_HOME:  
				  nButtonIndex = 0 ;
				  break ;
	          case VK_END:   
				  nButtonIndex = buttonNum - 1 ;  
				  break ;
			  case '\t':			  		
			  		if(gwDeviceType != 1)//������л�ͨ�� wangbingfu modify at 2011-4-2
			  		{
						if(((gFunctionDefine & WORKMODE1PLUS2_SWITCH) != 0)&&(gnWorkType ==1 ||gnWorkType == 3))//�ڵ�ͨ��˫ͨ�������л�ͨ��
						{
							wChannelNo++;
							if(wChannelNo > 1)
								wChannelNo = 0;
						//	allCfgReadParamFile(ParamFileName);
							refreshAllCarftParam();
						}
			  		}
					break;
			case VK_ESCAPE:
					KillTimer(hWnd, TIMER_TIME_DISPLAY);
					break;
	          default:       
				  return 0 ;
          }
          
		  hChildFocus = currentAllParamCfgBtn;
		  
		  GetWindowRect(hChildFocus, &RectChild);
		  
		  GetWindowRect(hWnd, &RectParent);

		  cha = RectParent.bottom - RectChild.bottom;
		  cha2 = RectChild.top - RectParent.top;
		  
		  si.cbSize = sizeof (si) ;
		  si.fMask  = SIF_POS ;
		  GetScrollInfo (hWnd, SB_VERT, &si) ;
		  iVertPos = si.nPos;

		  si.nPos = nButtonIndex;
		  SetScrollInfo (hWnd, SB_VERT, &si, TRUE) ;

		  if(cha < 0 || cha2 < 0)
		  {
			  ScrollWindowEx(hWnd, 0, (iVertPos - nButtonIndex)*EACH_ITEM_HEIGHT, NULL, NULL,
				  NULL, NULL, SW_SCROLLCHILDREN);
		  }

          return 0 ;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*���ع��ղ������ô��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int allParamCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_allParamCfgBtnBitmap[]=
	{
		{IDC_ALLPARAMCFG_GONGYICANSHU, 	IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R	},
		{IDC_ALLPARAMCFG_VOLTAGE, 	IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_MAIKUAN,	IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_RATE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_AMEND,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_QIANYAN, 	IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_JIANBO,	IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_PLUS,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_REFPLUS,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_DELAY,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_LVBO,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_REFRACTMODE,	IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_SPEEDTYPE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_SPEED,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_RANGE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_ANGLE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_KVALUE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_BATE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_TIMEYEAR,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_TIMEMONTH,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_TIMEDAY,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_TIMEHOUR,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_TIMEMINUTE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_TIMESECOND,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_GONGJIANHOUDU,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_SCANMODE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_BSCANONOROFF,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_PLATEHEIGHT,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_PROBETYPE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_UPBLINDVALUE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_DOWNBLINDVALUE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_SENSITIVE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_POKOUTYPE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_POKOUANGLE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_POKOUDEEP,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_HANFENGTANSHANGHOUDU,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_HANFENGTANSHANGDISTANCE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_HANFENGTANSHANGONOROFF,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_LIANGDU,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_ZUKANGPIPEI,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_QUXIANFENDUAN,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_PINPUFENXI,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_XINGNENGJIAOYAN,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_JULIZUOBIAO,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_GUANCAIWAIJING,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},		
		{IDC_ALLPARAMCFG_GUANCAINEIJING,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_XIANGYINGSPEED,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_BIAOMIANBUCHANG,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_PRINTTYPE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_LIEWENQIBO,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_PINGDING,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_DINGLIANG,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_PANFEI,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_TANTOUTYPE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_CANSHUQINLING,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_TONGDAOQINLING,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_CANSHUSUODING,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_QUXIANGUIGE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_ZIDONGBOGAO,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		//{IDC_ALLPARAMCFG_PLUSTYPE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_WORKTYPE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_HARDWARE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_SOFTWARE,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_EQUIPMENTMODEL,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},

		{IDC_ALLPARAMCFG_NEXTADJUSTTIME,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{IDC_ALLPARAMCFG_THISADJUSTTIME,		IDB_BITMAP_ALLCONFIG_NOSEL_R	 ,  IDB_BITMAP_ALLCONFIG_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_allParamCfgBtnBitmap);
	return ReturnVal;
}
/**************************
*���ղ�����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgGongYiCanShuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
			
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = 0;

					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{	
						if(gwGongYiCanShu <=1)
							gwGongYiCanShu = 1000;
						else 
							gwGongYiCanShu --;
						
							allCfgDisplayGongYiCanShu(Wnd,gwGongYiCanShu);
						/*if(ReadParamFile(ParamFileName))
							refreshAllCarftParam();*/
							if(GetFileAttributes(ParamFileName) != 0xFFFFFFFF)
							{	
								allCfgReadParamFile(ParamFileName);
								refreshAllCarftParam();
							}	
				
						
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gwGongYiCanShu >= 1000)
						{
							gwGongYiCanShu = 1;
						}
						else 
						{
							gwGongYiCanShu ++;
						}	
						allCfgDisplayGongYiCanShu(Wnd,gwGongYiCanShu);
						if(GetFileAttributes(ParamFileName) != 0xFFFFFFFF)
						{	
							allCfgReadParamFile(ParamFileName);
							refreshAllCarftParam();
						}	
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgGongYiCanShuProc,Wnd,Msg,wParam,lParam);
}

/**************************
*��ѹ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgVoltageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);


				//	ShowWindow(hWndAllParamCfg,SW_HIDE);
				//	ShowWindow(hWndAllParamConfirm,SW_SHOW);
				//	UpdateWindow(hWndAllParamConfirm);
				/*	InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					
					ShowWindow(hStatusBar,SW_SHOW);
					ShowWindow(hWndCraftParam,SW_SHOW);
					SetFocus(currentCraftParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndAllParamCfg);
					hWndAllParamCfg = 0;
					DestroyWindow(hWndAllParamCfgCaption);
					hWndAllParamCfgCaption = 0;

					SetRedrawFlag(TRUE);
					*/

				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = 0;
					
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					//	if(allCfgReadParamFile(ParamFileName))
					//		DrawLableText1(hWndAllParamCfgCaption, _T("�ɹ�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 510, ALLCFG_LABEL_START_Y, 100, ALLCFG_LABEL_HEIGHT);
					//	else
					//		DrawLableText1(hWndAllParamCfgCaption, _T("ʧ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, 510, ALLCFG_LABEL_START_Y, 100, ALLCFG_LABEL_HEIGHT);

					if(!gbCanShuSuoDing)
					{	
						
							if(wAllCfgVoltageType == 0)
							{
								wAllCfgVoltageType = 3;
							}
							else
								wAllCfgVoltageType--;

							Send_AllParamConfigWith_ad[0].nPress = typeToVoltage(wAllCfgVoltageType);

							Send_AllParamConfigWith_ad[1].nPress = typeToVoltage(wAllCfgVoltageType);
							
							allCfgDisplayVoltage(Wnd, Send_AllParamConfigWith_ad[wChannelNo].nPress);
							
							
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(wAllCfgVoltageType == 3)
						{
							wAllCfgVoltageType = 0;
						}
						else
							wAllCfgVoltageType++;
						
						Send_AllParamConfigWith_ad[0].nPress = typeToVoltage(wAllCfgVoltageType);
						Send_AllParamConfigWith_ad[1].nPress = typeToVoltage(wAllCfgVoltageType);
						
						allCfgDisplayVoltage(Wnd, Send_AllParamConfigWith_ad[wChannelNo].nPress);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgVoltageProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgMaiKuanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);



				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				//z
			  	else if(wParam == VK_LEFT)
			  	{
				  if(!gbCanShuSuoDing)
				  	{
						if(Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan > MIN_MAIKUAN && 
									Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan > nMaiKuanStep)
							Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan -= nMaiKuanStep;
						else
							Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan = MIN_MAIKUAN;
						allCfgDisplayMaiKuan(Wnd);
				  	}
				//SetMaiKuanLableAndExecute(Wnd);
			
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan < MAX_MAIKUAN)
							Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan += nMaiKuanStep;
						else
							Send_AllParamConfigWith_ad[wChannelNo].nMaiKuan = MAX_MAIKUAN;
						allCfgDisplayMaiKuan(Wnd);
					}
					//SetMaiKuanLableAndExecute(Wnd);
				
				}//z
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F') // ����
				{
					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgMaiKuanProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�ظ�Ƶ�ʰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgRateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);

				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				//z
			/*	else if(wParam == VK_LEFT)
				{
				
					if(Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency > MIN_RE_FREQ && 
							Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency > nRepeatFreqStep)
						Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency -= nRepeatFreqStep;
					else
						Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency = MIN_RE_FREQ;
						allCfgDisplayRate(Wnd);

					//SetReFreqLableAndExecute(Wnd);
				
				}
			  	else if(wParam == VK_RIGHT)
			  	{
			
					if(Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency < MAX_RE_FREQ)
						Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency += nRepeatFreqStep;
					else
						Send_AllParamConfigWith_ad[wChannelNo].nRefrainFrequency = MAX_RE_FREQ;
					allCfgDisplayRate(Wnd);
				//	SetReFreqLableAndExecute(Wnd);			
			  	}//z
			  	*/
			  	else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgRateProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgAmendProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Send_AllParamConfigWith_ad[wChannelNo].nAmend > MIN_AMEND && 
								Send_AllParamConfigWith_ad[wChannelNo].nAmend > fAmendStep)
							Send_AllParamConfigWith_ad[wChannelNo].nAmend -= fAmendStep;
						else
							Send_AllParamConfigWith_ad[wChannelNo].nAmend = MIN_AMEND;
						
						allCfgDisplayAmend(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Send_AllParamConfigWith_ad[wChannelNo].nAmend < MAX_AMEND)
								Send_AllParamConfigWith_ad[wChannelNo].nAmend += fAmendStep;
						else
							Send_AllParamConfigWith_ad[wChannelNo].nAmend = MAX_AMEND;
						allCfgDisplayAmend(Wnd);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}

				else if(wParam == 'F') // ����
				{

					if(nAmendStepIndex < 2)
						nAmendStepIndex++;
					else
						nAmendStepIndex = 0;

					fAmendStep= USSTEPS[nAmendStepIndex];

					allCfgDisplayStepFloat2(hWndAllParamCfgCaption, fAmendStep);
					
					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgAmendProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ǰ�ذ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgQianYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao > 0.0f)
						{
							if(Send_AllParamConfigWith_ad[wChannelNo].fQianYan > fParamQianYanStep)
								Send_AllParamConfigWith_ad[wChannelNo].fQianYan -= fParamQianYanStep;
							else
								Send_AllParamConfigWith_ad[wChannelNo].fQianYan = 0.0f;
		
							
							allCfgDisplayFloat1(Wnd,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
						}
						else
						{
							allCfgDisplayQianYan(Wnd,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
						}
					}	
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao > 0.0f)
						{
							if(Send_AllParamConfigWith_ad[wChannelNo].fQianYan < 100)
								Send_AllParamConfigWith_ad[wChannelNo].fQianYan += fParamQianYanStep;
							else
								Send_AllParamConfigWith_ad[wChannelNo].fQianYan = 100;
							allCfgDisplayFloat1(Wnd,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
						}
						else
						{
							allCfgDisplayQianYan(Wnd,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
						}
				
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}

				else if(wParam == 'F') // ����
				{
					if(nQianYanStepIndex < 2)
						nQianYanStepIndex++;
					else
						nQianYanStepIndex = 0;

					fParamQianYanStep = AMENDSTEPS[nQianYanStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fParamQianYanStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgQianYanProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�첨��ʽ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgJianBoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)

					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode > 0)
							Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode -- ;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode = 0;

						allCfgDisplayJianBo(Wnd, Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode);
					}
					
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode < 3)
							Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode ++ ;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode = 3;

					 	allCfgDisplayJianBo(Wnd, Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode);

					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgJianBoProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���水ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPlusProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						float fOldPlus = ReceiveParam_ad[wChannelNo].fPlus;
					
						if(Receive_AllParamConfigWith_ad[wChannelNo].fPlus > MIN_PLUS && 
								Receive_AllParamConfigWith_ad[wChannelNo].fPlus > fPlusStep &&
								Receive_AllParamConfigWith_ad[wChannelNo].fPlus > (-1*Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus + fPlusStep))
							Receive_AllParamConfigWith_ad[wChannelNo].fPlus -= fPlusStep;
						else
							//Receive_AllParamConfigWith_ad[wChannelNo].fPlus = MIN_PLUS < (-1*Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus) ? MIN_PLUS : (-1*Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus);
							Receive_AllParamConfigWith_ad[wChannelNo].fPlus = MIN_PLUS;
						allCfgDisplayPlus(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].fPlus < (MAX_PLUS - Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus))
							Receive_AllParamConfigWith_ad[wChannelNo].fPlus += fPlusStep;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].fPlus = MAX_PLUS - Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus;
						
						allCfgDisplayPlus(Wnd);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					if(nPlusStepIndex < 3)
						nPlusStepIndex++;
					else
						nPlusStepIndex = 0;

					fPlusStep = PLUSSTEPS[nPlusStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fPlusStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPlusProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�ο����水ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgRefPlusProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)

					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus > (MIN_PLUS + fPlusStep) //&& 
				//			Receive_AllParamConfigWith_ad[wChannelNo] > fPlusStep
							)
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus -= fPlusStep;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus = MIN_PLUS;

						allCfgDisplayRefPlus(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus < (MAX_PLUS - Receive_AllParamConfigWith_ad[wChannelNo].fPlus))
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus += fPlusStep;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus = MAX_PLUS - Receive_AllParamConfigWith_ad[wChannelNo].fPlus;

						allCfgDisplayRefPlus(Wnd);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					if(nPlusStepIndex < 3)
						nPlusStepIndex++;
					else
						nPlusStepIndex = 0;

					fPlusStep = PLUSSTEPS[nPlusStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fPlusStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgRefPlusProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�ӳٰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgDelayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				//z
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].fDelay > MIN_DELAY[wChannelNo] && 
									Receive_AllParamConfigWith_ad[wChannelNo].fDelay > (MIN_DELAY[wChannelNo] + fDelayStep))
							Receive_AllParamConfigWith_ad[wChannelNo].fDelay -= fDelayStep;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].fDelay = MIN_DELAY[wChannelNo];

						if(fabs(Receive_AllParamConfigWith_ad[wChannelNo].fDelay)< 1E-3)
							Receive_AllParamConfigWith_ad[wChannelNo].fDelay = 0;
						allCfgDisplayDelay(Wnd);

						//SetDelayLableAndExecute(Wnd, wChannelNo);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].fDelay < MAX_DELAY)
							Receive_AllParamConfigWith_ad[wChannelNo].fDelay += fDelayStep;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].fDelay = MAX_DELAY;
						
						if(fabs(Receive_AllParamConfigWith_ad[wChannelNo].fDelay)< 1E-3)
							Receive_AllParamConfigWith_ad[wChannelNo].fDelay = 0;
						allCfgDisplayDelay(Wnd);
					//SetDelayLableAndExecute(Wnd, wChannelNo);
					}
				}
				//z
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					if(nDelayStepIndex < 2)
						nDelayStepIndex++;
					else
						nDelayStepIndex = 0;

					fDelayStep = USSTEPS[nDelayStepIndex];

					allCfgDisplayStepFloat2(hWndAllParamCfgCaption, fDelayStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgDelayProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�˲�Ƶ����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgLvBoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
				
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				//z
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq > MIN_LVBO_TYPE)
							Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq --;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq = MAX_LVBO_TYPE;
						
						allCfgDisplayLvBo(Wnd, Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq);
					}
				//	SetLvBoLableAndExecute(Wnd, wChannelNo);
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{	
						if(Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq < MAX_LVBO_TYPE)
							Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq ++;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq = MIN_LVBO_TYPE;
						
						allCfgDisplayLvBo(Wnd, Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq);
					//	SetLvBoLableAndExecute(Wnd, wChannelNo);
					}
				}
				//z
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgLvBoProc,Wnd,Msg,wParam,lParam);
}
/**************************
*B/Xת����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgRefractModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				//z
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						Display_AllParamConfigWith_ad[wChannelNo].bSFlag = !Display_AllParamConfigWith_ad[wChannelNo].bSFlag;
						allCfgDisplayRefractMode(Wnd);
					}
				}
			//z
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgRefractModeProc,Wnd,Msg,wParam,lParam);
}

/**************************
*���ٲ��ϰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgSpeedTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{	
						wMaterialType--;
						if(wMaterialType == 0)
							wMaterialType = 11;
						
						allCfgDisplaySpeedType(Wnd,wMaterialType);
						Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
						//allCfgDisplaySpeedType(Wnd, Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);
						allCfgDisplaySpeed(hBtnAllParamCfgSpeed);

						
						if(wMaterialType == 1)
							wUserDefineWaveSpeed = Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed;//�����Զ�������
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						wMaterialType++;
						if(wMaterialType == 12)
							wMaterialType = 1;
						
						allCfgDisplaySpeedType(Wnd,wMaterialType);									
						Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
						//allCfgDisplaySpeedType(Wnd, Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);
						allCfgDisplaySpeed(hBtnAllParamCfgSpeed);

						if(wMaterialType == 1)
							wUserDefineWaveSpeed = Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed;//�����Զ�������
							
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgSpeedTypeProc,Wnd,Msg,wParam,lParam);
}

/**************************
*���ٰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				//z
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed > MIN_WAVE_SPEED && 
								Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed > nWaveSpeedStep)
						{
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed -= nWaveSpeedStep;
						}
						else
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = MIN_WAVE_SPEED;
						
						allCfgDisplaySpeed(Wnd);
						wMaterialType = allCfgwaveSpeedToType(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);
						allCfgDisplaySpeedType(hBtnAllParamCfgSpeedType, wMaterialType);
		
						if(wMaterialType == 1)
							wUserDefineWaveSpeed = Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed;
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed < MAX_WAVE_SPEED)
						{
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed += nWaveSpeedStep;
						}
						else
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = MAX_WAVE_SPEED;
					
						allCfgDisplaySpeed(Wnd);
						wMaterialType = allCfgwaveSpeedToType(Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed);
						allCfgDisplaySpeedType(hBtnAllParamCfgSpeedType, wMaterialType);

						if(wMaterialType == 1)
							wUserDefineWaveSpeed = Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed;

					}
				
				}
			//z
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					
					if(nWaveSpeedStepIndex < 2)
						nWaveSpeedStepIndex++;
					else
						nWaveSpeedStepIndex = 0;
	
					nWaveSpeedStep = WAVESPEEDSTEPS[nWaveSpeedStepIndex];

					allCfgDisplayStepInt(hWndAllParamCfgCaption, nWaveSpeedStep);
					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgSpeedProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���̰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgRangeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{	
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].fRange > 1 && 
								Display_AllParamConfigWith_ad[wChannelNo].fRange > fRangeStep)
							Display_AllParamConfigWith_ad[wChannelNo].fRange -= fRangeStep;
						else
							Display_AllParamConfigWith_ad[wChannelNo].fRange = 1;
						allCfgDisplayRange(Wnd);
						//SetRangeLableAndExecute(Wnd, DisplaySet_ad[wChannelNo].fRange);
					}
				}
				else if(wParam == VK_RIGHT)
				{		
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].fRange < MAX_RANGE)
							Display_AllParamConfigWith_ad[wChannelNo].fRange += fRangeStep;
						else
							Display_AllParamConfigWith_ad[wChannelNo].fRange = MAX_RANGE;
					
						allCfgDisplayRange(Wnd);
					}
				//SetRangeLableAndExecute(Wnd, DisplaySet_ad[wChannelNo].fRange);
				}
				//z
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					if(nRangeStepIndex < 3)
						nRangeStepIndex++;
					else
						nRangeStepIndex = 0;

					fRangeStep = AMENDSTEPS[nRangeStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fRangeStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgRangeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����ǰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgAngleProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);

					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao - fZheSheAngleStep > MIN_ANGLE)
						{
							Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao -= fZheSheAngleStep;
							nTanTouType[wChannelNo] = 0;
							allCfgDisplayTanTouType(hBtnAllParamCfgTanTouType);
							
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
							allCfgDisplaySpeed(hBtnAllParamCfgSpeed);

							allCfgDisplayFloat1(hBtnAllParamCfgQianYan,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
						}
						else
						{
							Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao = MIN_ANGLE;
							nTanTouType[wChannelNo] = 1;
							allCfgDisplayTanTouType(hBtnAllParamCfgTanTouType);

							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
							allCfgDisplaySpeed(hBtnAllParamCfgSpeed);

							allCfgDisplayQianYan(hBtnAllParamCfgQianYan,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
						}
						allCfgDisplayAngle(Wnd);
						allCfgDisplayKValue(hBtnAllParamCfgKValue, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
					}				
				}
				else if(wParam == VK_RIGHT )
				{
					if(!gbCanShuSuoDing)
					{	
						if(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao + fZheSheAngleStep < MAX_ANGLE)
						{
							Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao += fZheSheAngleStep;

							
						}
						else
							Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao = MAX_ANGLE;

						nTanTouType[wChannelNo] = 0;
						
						allCfgDisplayTanTouType(hBtnAllParamCfgTanTouType);
						
						allCfgDisplayFloat1(hBtnAllParamCfgQianYan,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);

						Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
						allCfgDisplaySpeed(hBtnAllParamCfgSpeed);

						allCfgDisplayAngle(Wnd);
						allCfgDisplayKValue(hBtnAllParamCfgKValue, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}	
				else if(wParam == 'F')
				{
					if(nZheSheAngleStepIndex < 2)
						nZheSheAngleStepIndex++;
					else
						nZheSheAngleStepIndex = 0;

					fZheSheAngleStep = AMENDSTEPS[nZheSheAngleStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fZheSheAngleStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgAngleProc,Wnd,Msg,wParam,lParam);
}

/**************************
*Kֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgKValueProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
				
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);

					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if((fKValue - fKValueStep) > 0.01f)
						{
							fKValue = fKValue - fKValueStep;
						}
						else
						{
							fKValue = 0.0f;

							nTanTouType[wChannelNo] = 1;
							allCfgDisplayTanTouType(hBtnAllParamCfgTanTouType);
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
							allCfgDisplaySpeed(hBtnAllParamCfgSpeed);
							allCfgDisplayQianYan(hBtnAllParamCfgQianYan,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
						}
						Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao = (float)(atan((double)fKValue) / PI * 180);
						allCfgDisplayKValue(Wnd, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
						allCfgDisplayAngle(hBtnAllParamCfgAngle);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((fKValue + fKValueStep) < 573)
						{
							fKValue = fKValue + fKValueStep;
						}
						else
							fKValue = 573;
						
						
						Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao = (float)(atan((double)fKValue) / PI * 180);
						allCfgDisplayKValue(Wnd, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
						allCfgDisplayAngle(hBtnAllParamCfgAngle);

						nTanTouType[wChannelNo] = 0;
						allCfgDisplayTanTouType(hBtnAllParamCfgTanTouType);
						Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
						allCfgDisplaySpeed(hBtnAllParamCfgSpeed);
						allCfgDisplayFloat1(hBtnAllParamCfgQianYan,Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}	
				else if(wParam == 'F')
				{
					if(nKValueStepIndex < 2)
						nKValueStepIndex++;
					else
						nKValueStepIndex = 0;

					fKValueStep = AMENDSTEPS[nKValueStepIndex];
					
					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fKValueStep);
					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgKValueProc,Wnd,Msg,wParam,lParam);
}

/**************************
*���ư�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAllParamCfgBateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].nBate > MIN_BATE && 
								Display_AllParamConfigWith_ad[wChannelNo].nBate > nBateStep)
							Display_AllParamConfigWith_ad[wChannelNo].nBate -= nBateStep;
						else
							Display_AllParamConfigWith_ad[wChannelNo].nBate = MIN_BATE;
						allCfgDisplayBate(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Display_AllParamConfigWith_ad[wChannelNo].nBate < MAX_BATE)
							Display_AllParamConfigWith_ad[wChannelNo].nBate += nBateStep;
						else
							Display_AllParamConfigWith_ad[wChannelNo].nBate = MAX_BATE;

						allCfgDisplayBate(Wnd);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgBateProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�� ʱ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTimeYearProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	 
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{	
					
						st.wYear--;
						if(st.wYear<2000)
						{
							st.wYear = 2050;
						}
					//	st.wYear = AllParam_Time.wYear;
						::SetLocalTime(&st);
						allCfgDisplayTimeYear(Wnd);
					}
					
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wYear++;
						if(st.wYear>2050)
						{
							st.wYear=2000;
						}
					//	st.wYear = AllParam_Time.wYear;
						::SetLocalTime(&st);
						allCfgDisplayTimeYear(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTimeYearProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�� ʱ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTimeMonthProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wMonth--;
						if(st.wMonth< 1)
						{
							st.wMonth = 12;
						}
						
						if((st.wYear%400 == 0)||(st.wYear%4 ==0&&st.wYear%100!=0))
						{
							if(st.wMonth ==2  && st.wDay >29)
								st.wDay = 1;
						}
						else 
						{
							if(st.wMonth == 2 && st.wDay >28)
								st.wDay = 1;
						}
						if(st.wMonth == 4||st.wMonth == 6||st.wMonth == 9||st.wMonth == 11)
						{
							if(st.wDay>30)
								st.wDay = 1;
			 			}
							
					//	st.wMonth = AllParam_Time.wMonth;
						::SetLocalTime(&st);
						allCfgDisplayTimeDay(hBtnAllParamCfgTimeDay);
						allCfgDisplayTimeMonth(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wMonth++;
						if(st.wMonth>12)
						{
							st.wMonth = 1;
						}
						if((st.wYear%400 == 0)||(st.wYear%4 ==0&&st.wYear%100!=0))
						{
							if(st.wMonth ==2 && st.wDay >29)
								st.wDay = 1;
						}
						else
						{
							if(st.wMonth ==2&& st.wDay > 28)
								st.wDay = 1;
						}
						if((st.wMonth == 4||st.wMonth == 6||st.wMonth == 9||st.wMonth == 11)&&st.wDay>30)
						{
							st.wDay = 1;
			 			}
						//st.wMonth = AllParam_Time.wMonth;
						::SetLocalTime(&st);
						allCfgDisplayTimeDay(hBtnAllParamCfgTimeDay);////////////////////////////////////////////////
						allCfgDisplayTimeMonth(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTimeMonthProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�� ʱ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTimeDayProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wDay--;
				
						if(st.wMonth == 1||st.wMonth == 3 ||st.wMonth == 5 ||st.wMonth == 7
							|| st.wMonth == 8 ||st.wMonth ==10||st.wMonth == 12)
						{
							if(st.wDay<1)
								st.wDay = 31;
						}
						if((st.wYear%400 == 0)||(st.wYear%4 ==0&&st.wYear%100!=0))
						{
							if(st.wMonth == 2&&st.wDay <1)
								st.wDay = 29;
								
						}
						else
						{
							if(st.wMonth == 2&&st.wDay<28)
							{
								st.wDay = 28;
							}
						}
						if(st.wMonth ==4||st.wMonth ==6||st.wMonth ==9||st.wMonth ==11)
						{
								if(st.wDay < 1)
									st.wDay = 30;
						}
						
					//	st.wDay = AllParam_Time.wDay;
						::SetLocalTime(&st);

						allCfgDisplayTimeDay(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wDay++;
				
						if(st.wMonth == 1||st.wMonth == 3 ||st.wMonth == 5 ||st.wMonth == 7
							|| st.wMonth == 8 ||st.wMonth ==10||st.wMonth == 12)
						{
							if(st.wDay>31)
								st.wDay = 1;
						}
						if((st.wYear%400 == 0)||(st.wYear%4 ==0&&st.wYear%100!=0))
						{
							if(st.wMonth == 2&&st.wDay >29)
								st.wDay = 1;
								
						}
						else
						{
							if(st.wMonth == 2&&st.wDay>28)
							{
								st.wDay = 1;
							}
						}
						if(st.wMonth ==4||st.wMonth ==6||st.wMonth ==9||st.wMonth ==11)
						{
								if(st.wDay > 30)
									st.wDay = 1;
						}
							
						

						//st.wDay = AllParam_Time.wDay;
						::SetLocalTime(&st);

						allCfgDisplayTimeDay(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTimeDayProc,Wnd,Msg,wParam,lParam);
}

/**************************
*Сʱ ʱ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTimeHourProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						
						if(st.wHour == 0)
						{
							st.wHour = 24;
						}
						st.wHour--;
					//	st.wHour = AllParam_Time.wHour;
						::SetLocalTime(&st);

						allCfgDisplayTimeHour(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wHour++;
						if(st.wHour == 24)
						{
							st.wHour = 0;
						}
					//	st.wHour = AllParam_Time.wHour;
						::SetLocalTime(&st);

						allCfgDisplayTimeHour(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTimeHourProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���� ʱ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTimeMinuteProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(st.wMinute == 0)
						{
							st.wMinute = 60;
						}
						st.wMinute--;
						
						
					//	st.wMinute = AllParam_Time.wMinute;
						::SetLocalTime(&st);

						allCfgDisplayTimeMinute(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wMinute++;
						if(st.wMinute == 60)
						{
							st.wMinute = 0;
						}
					//	st.wMinute = AllParam_Time.wMinute;
						::SetLocalTime(&st);
						
						allCfgDisplayTimeMinute(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTimeMinuteProc,Wnd,Msg,wParam,lParam);
}

/**************************
*��  ʱ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTimeSecondProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						
						if(st.wSecond == 0)
						{
							st.wSecond = 60;
						}
						st.wSecond--;
					//	st.wSecond = AllParam_Time.wSecond;
						::SetLocalTime(&st);

						allCfgDisplayTimeSecond(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						st.wSecond++;
						if(st.wSecond == 60)
						{
							st.wSecond = 0;
						}
						
					//	st.wSecond = AllParam_Time.wSecond;
						::SetLocalTime(&st);

						allCfgDisplayTimeSecond(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTimeSecondProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgGongJianHouDuProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if((allParamPlateHeight - PlateHeightStep)> 5)
							allParamPlateHeight = allParamPlateHeight - PlateHeightStep;
						else
							allParamPlateHeight = 5;
						allCfgDisplayFloat1(Wnd, allParamPlateHeight);

					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((allParamPlateHeight + PlateHeightStep) < MAX_fPlateHeight)
							allParamPlateHeight = allParamPlateHeight + PlateHeightStep;
						else
							allParamPlateHeight = MAX_fPlateHeight;
						allCfgDisplayFloat1(Wnd, allParamPlateHeight);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					
					if(PlateHeightIndex<3)
						PlateHeightIndex++;
					else
						PlateHeightIndex = 0;
	
					PlateHeightStep = AMENDSTEPS[PlateHeightIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PlateHeightStep);					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgGongJianHouDuProc,Wnd,Msg,wParam,lParam);
}

/**************************
*Bɨ֮ɨ�跽ʽ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgScanModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
			
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						allParamScanMode = !allParamScanMode;
						allCfgDisplayScanMode(Wnd,allParamScanMode);
					}
				}

				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgScanModeProc,Wnd,Msg,wParam,lParam);
}

/**************************
*Bɨ֮ɨ�迪�ذ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgBScanOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
			
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{	allParamBscanSwitch= !allParamBscanSwitch;
						allCfgDisplayBScanOnOrOff(Wnd,allParamBscanSwitch);
					}
				}

				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgBScanOnOrOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�к��̽��֮����������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPlateHeightProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						allParamPlateHeight -= PlateHeightStep;
						if(allParamPlateHeight<=5)
						{
							allParamPlateHeight= 5;
						}
				
						allCfgDisplayPlateHeight(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						allParamPlateHeight += PlateHeightStep;
						
						allCfgDisplayPlateHeight(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					
					if(PlateHeightIndex<3)
						PlateHeightIndex++;
					else
						PlateHeightIndex= 0;
	
					PlateHeightStep= PLATEHEIGHT[PlateHeightIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PlateHeightStep);
					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPlateHeightProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�к��̽��֮̽ͷ�������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgProbeTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						allParamProbeType --;
						if(allParamProbeType<1)
						{
							allParamProbeType = 10;
						}
						
				
						allCfgDisplayProbeType(Wnd,allParamProbeType);
					}
					
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						allParamProbeType++;
						if(allParamProbeType >10)
						{
							allParamProbeType =1 ;
						}
						allCfgDisplayProbeType(Wnd,allParamProbeType);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgProbeTypeProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�к��̽��֮��ä�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgUpBlindValueProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(allParamUpBlindValue - BlindValueStep > 1E-6)
							allParamUpBlindValue -= BlindValueStep;
						else
							allParamUpBlindValue = 0.0;
										
						allCfgDisplayFloat1(Wnd, allParamUpBlindValue);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((allParamUpBlindValue + BlindValueStep) < (allParamPlateHeight - allParamDownBlindValue))
							allParamUpBlindValue += BlindValueStep;
						else
							allParamUpBlindValue = allParamPlateHeight - allParamDownBlindValue;
						
						allCfgDisplayFloat1(Wnd, allParamUpBlindValue);
					}
				}
				else if(wParam == 'F')
				{
					if(wBlindAreaStepIndex < 3)
						wBlindAreaStepIndex++;
					else
						wBlindAreaStepIndex = 0;

					BlindValueStep = AMENDSTEPS[wBlindAreaStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, BlindValueStep);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgUpBlindValueProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�к��̽��֮��ä�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgDownBlindValueProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(allParamDownBlindValue - BlindValueStep > 1E-6)
							allParamDownBlindValue -= BlindValueStep;
						else
							allParamDownBlindValue = 0.0;
						
				
						allCfgDisplayFloat1(Wnd, allParamDownBlindValue);
					}
					
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((allParamDownBlindValue + BlindValueStep) < (allParamPlateHeight - allParamUpBlindValue))
							allParamDownBlindValue += BlindValueStep;
						else
							allParamDownBlindValue = allParamPlateHeight - allParamUpBlindValue;
						
						allCfgDisplayFloat1(Wnd, allParamDownBlindValue);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					if(wBlindAreaStepIndex < 3)
						wBlindAreaStepIndex++;
					else
						wBlindAreaStepIndex = 0;

					BlindValueStep = AMENDSTEPS[wBlindAreaStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, BlindValueStep);
				}
			
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgDownBlindValueProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�к��̽��֮�����ȼ�ⰴť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgZhongHouBanTanShangSensitiveProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus > MIN_PLUS && 
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus > fPlusStep)
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus -= fPlusStep;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus = MIN_PLUS;
						
				
						allCfgDisplayFloat1(Wnd, Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus);
					}
					
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus < (MAX_PLUS - Receive_AllParamConfigWith_ad[wChannelNo].fPlus))
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus += fPlusStep;
						else
							Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus = MAX_PLUS - Receive_AllParamConfigWith_ad[wChannelNo].fPlus;
						
						allCfgDisplayFloat1(Wnd, Receive_AllParamConfigWith_ad[wChannelNo].fRefPlus);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					if(nPlusStepIndex < 3)
						nPlusStepIndex++;
					else
						nPlusStepIndex = 0;

					fPlusStep = PLUSSTEPS[nPlusStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fPlusStep);
				}
			
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgZhongHouBanTanShangSensitiveProc,Wnd,Msg,wParam,lParam);
}

/**************************
*����̽��֮�¿��������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPoKouTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(allParamPoKouLeiXing > 0)
							allParamPoKouLeiXing--;

						allCfgDisplayPoKouType(Wnd,allParamPoKouLeiXing);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(allParamPoKouLeiXing < 2)
							allParamPoKouLeiXing++;
							
						allCfgDisplayPoKouType(Wnd,allParamPoKouLeiXing);
					}
					
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPoKouTypeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����̽��֮�¿ڽǶ����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPoKouAngleProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if((allParamPoKouJiaoDu - PoKouAngleStep) > 1E-6)
						{
							allParamPoKouJiaoDu = allParamPoKouJiaoDu - PoKouAngleStep;

							double fAngle = allParamPoKouJiaoDu/180.0f * PI;
							allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
						}
						allCfgDisplayPoKouAngle(Wnd);	
					}
				}
				
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((allParamPoKouJiaoDu + PoKouAngleStep) < 90)
						{
							allParamPoKouJiaoDu = allParamPoKouJiaoDu + PoKouAngleStep;

							double fAngle = allParamPoKouJiaoDu/180.0f * PI;
							allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
						}
						allCfgDisplayPoKouAngle(Wnd);					
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					if(PoKouAngleIndex < 1)
						PoKouAngleIndex++;
					else
						PoKouAngleIndex = 0;

					PoKouAngleStep = ANGLESTEPS[PoKouAngleIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PoKouAngleStep);
				}
			
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPoKouAngleProc,Wnd,Msg,wParam,lParam);
}
/******************************************
*����̽��֮�¿��������
* @Param       hinstance[in]
* @Param       message[in]
* @Param       wParam[in]
* @Param       lParam[in]
* return       LRESULT
* @Param       since 1.0
*******************************************/
LRESULT CALLBACK btnAllParamCfgPoKouDeepProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
		{
			case WM_KEYDOWN:
			{
				int i =0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);	
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(allParamPoKouLeiXing == 1)
						{
							if(allParamPoKouShenDu - PoKouDeepStep > 1)
							{
								allParamPoKouShenDu = allParamPoKouShenDu - PoKouDeepStep;
								double fAngle = allParamPoKouJiaoDu/180.0f * PI;
								allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
							}
							else
							{
								allParamPoKouShenDu = 1;
								double fAngle = allParamPoKouJiaoDu/180.0f * PI;
								allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
							}
						}
						else if(allParamPoKouLeiXing == 2)
						{
							if(2 * (allParamPoKouShenDu - PoKouDeepStep) > 2)
							{
								allParamPoKouShenDu = allParamPoKouShenDu - PoKouDeepStep;
								double fAngle = allParamPoKouJiaoDu/180.0f * PI;
								allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
							}
							else
							{
								allParamPoKouShenDu = 1;
								double fAngle = allParamPoKouJiaoDu/180.0f * PI;
								allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
							}

							//gfPlateHeight = 2 * gfPoKouShenDu;
						}
						allCfgDisplayPoKouDeep(Wnd,allParamPoKouLeiXing);
						allCfgDisplayHanFengTanShangDistance(hBtnAllParamCfgHanFengTanShangDistance);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(allParamPoKouLeiXing == 1)
						{
							if(allParamPoKouShenDu + PoKouDeepStep <  allParamPlateHeight)
							{
								allParamPoKouShenDu = allParamPoKouShenDu + PoKouDeepStep;
								double fAngle = allParamPoKouJiaoDu/180.0f * PI;
								allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
							}
						}
						else if(allParamPoKouLeiXing == 2)
						{
							if(2 * (allParamPoKouShenDu + PoKouDeepStep) <  allParamPlateHeight)
							{
								allParamPoKouShenDu = allParamPoKouShenDu + PoKouDeepStep;
								double fAngle = allParamPoKouJiaoDu/180.0f * PI;
								allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
							}
						}
						allCfgDisplayPoKouDeep(Wnd,allParamPoKouLeiXing);
						allCfgDisplayHanFengTanShangDistance(hBtnAllParamCfgHanFengTanShangDistance);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
				else if(wParam == 'F')
				{
					if(PoKouDeepIndex < 1)
						PoKouDeepIndex++;
					else
						PoKouDeepIndex = 0;

					PoKouDeepStep = ANGLESTEPS[PoKouDeepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, PoKouDeepStep);
				}
			}
			break;
			
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPoKouDeepProc,Wnd,Msg,wParam,lParam);
}

/**************************
*����̽�˹���������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgHanFengTanShangHouDuProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(allParamHanFengTanShangSwitch == OFF)
						{
							if(allParamPoKouLeiXing == 0)
							{
								if((allParamHanFengTanShangGangBanHouDu -  HanFengTanShangHouDuStep) >= 0)
								{
									allParamHanFengTanShangGangBanHouDu -= HanFengTanShangHouDuStep;
									if(allParamPoKouLeiXing == 0)
									{
										double fAngle = allParamPoKouJiaoDu/180.0f * PI;
										allParamPoKouShenDu = allParamHanFengTanShangGangBanHouDu;
										allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
										allCfgDisplayHanFengTanShangDistance(hBtnAllParamCfgHanFengTanShangDistance);
									}
									allCfgDisplayHanFengTanShangHouDu(Wnd);
									
								}
							}
							else if(allParamPoKouLeiXing == 1)
							{
								if((allParamHanFengTanShangGangBanHouDu -  HanFengTanShangHouDuStep) >= allParamPoKouShenDu)
								{
									allParamHanFengTanShangGangBanHouDu -= HanFengTanShangHouDuStep;
									if(allParamPoKouLeiXing == 0)
									{
										double fAngle = allParamPoKouJiaoDu/180.0f * PI;
										allParamPoKouShenDu = allParamHanFengTanShangGangBanHouDu;
										allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
										allCfgDisplayHanFengTanShangDistance(hBtnAllParamCfgHanFengTanShangDistance);
									}
									allCfgDisplayHanFengTanShangHouDu(Wnd);
									
								}
							}
							else
							{
								if((allParamHanFengTanShangGangBanHouDu - HanFengTanShangHouDuStep) >= 2 * allParamPoKouShenDu)
								{
									allParamHanFengTanShangGangBanHouDu -= HanFengTanShangHouDuStep;
									if(allParamPoKouLeiXing == 0)
									{
										double fAngle = allParamPoKouJiaoDu/180.0f * PI;
										allParamPoKouShenDu = allParamHanFengTanShangGangBanHouDu;
										allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
										allCfgDisplayHanFengTanShangDistance(hBtnAllParamCfgHanFengTanShangDistance);
									}
										allCfgDisplayHanFengTanShangHouDu(Wnd);
									
								}
							}
						}
					}
				}
				else if(wParam == VK_RIGHT)
				{	
					if(!gbCanShuSuoDing)
					{
						if(allParamHanFengTanShangSwitch == OFF)
						{
							allParamHanFengTanShangGangBanHouDu += HanFengTanShangHouDuStep;
							if(allParamPoKouLeiXing == 0)
							{
									double fAngle = allParamPoKouJiaoDu/180.0f * PI;
									allParamPoKouShenDu = allParamHanFengTanShangGangBanHouDu;
									allParamHead2HanFengDistance = float(allParamPoKouShenDu / tan(fAngle));
									allCfgDisplayHanFengTanShangDistance(hBtnAllParamCfgHanFengTanShangDistance);
									
							}
								allCfgDisplayHanFengTanShangHouDu(Wnd);
						}
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					
					if(HanFengTanShangHouDuIndex<3)
						HanFengTanShangHouDuIndex++;
					else
						HanFengTanShangHouDuIndex= 0;
	
					HanFengTanShangHouDuStep= PLATEHEIGHT[HanFengTanShangHouDuIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, HanFengTanShangHouDuStep);
					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgHanFengTanShangHouDuProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����̽��̽ͷ���¿ڵľ������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgHanFengTanShangDistanceProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						double fAngle = allParamPoKouJiaoDu/180.0f * PI;
						if((allParamHead2HanFengDistance -  HanFengTanShangDistanceStep) >= float(allParamPoKouShenDu / tan(fAngle)))
							allParamHead2HanFengDistance -= HanFengTanShangDistanceStep;
						allCfgDisplayHanFengTanShangDistance(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					
					if(!gbCanShuSuoDing)
					{
						allParamHead2HanFengDistance += HanFengTanShangDistanceStep;
						allCfgDisplayHanFengTanShangDistance(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'F')
				{
					
					if(HanFengTanShangDistanceIndex<3)
						HanFengTanShangDistanceIndex++;
					else
						HanFengTanShangDistanceIndex= 0;
	
					HanFengTanShangDistanceStep= PLATEHEIGHT[HanFengTanShangDistanceIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, HanFengTanShangDistanceStep);
					
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgHanFengTanShangDistanceProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����̽�˿��ذ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgHanFengTanShangOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
			
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						allParamHanFengTanShangSwitch = !allParamHanFengTanShangSwitch;
						allCfgDisplayHanFengTanShangOnOrOff(Wnd,allParamHanFengTanShangSwitch);
					}
				}

				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgHanFengTanShangOnOrOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgLiangDuProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						
						if(allParamLCDValue < 0  )
							allParamLCDValue = 10;
						allParamLCDValue--;
						/*else
						{
							sys_reg->wLedBrightness = allParamLCDValue;
							InvalidateRect(hBtnOtherCfgLcd, NULL, TRUE);
							UpdateWindow(hBtnOtherCfgLcd);
						}*/
						allCfgDisplayLiangDu(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						allParamLCDValue++;
						if(allParamLCDValue >10)
							allParamLCDValue = 10;
					/*	else
						{	
							sys_reg->wLedBrightness = allParamLCDValue;
							InvalidateRect(hBtnOtherCfgLcd, NULL, TRUE);
							UpdateWindow(hBtnOtherCfgLcd);
						}
						*/
						allCfgDisplayLiangDu(Wnd);
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgLiangDuProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�迹ƥ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgZuKangPiPeiProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
			else if(wParam == VK_LEFT || wParam == VK_RIGHT)
			{
				if(!gbCanShuSuoDing && gnWorkType == 1)
				{
					if(wChannelNo == 0)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].wImpedance == 0) //A�� ����A�� ����λΪ00 �鿴�̵�������˵��
						{
							Receive_AllParamConfigWith_ad[wChannelNo].wImpedance = 1;
						}
						else if(Receive_AllParamConfigWith_ad[wChannelNo].wImpedance == 1)//A�� ����A�� ����λΪ01 �鿴�̵�������˵��
						{							
							Receive_AllParamConfigWith_ad[wChannelNo].wImpedance = 0;
						}
					}
					else if(wChannelNo == 1)
					{
						if(Receive_AllParamConfigWith_ad[wChannelNo].wImpedance == 2) //B�� ����B�� ����λΪ10 �鿴�̵�������˵��
						{							
							Receive_AllParamConfigWith_ad[wChannelNo].wImpedance = 0;
						}
						else if(Receive_AllParamConfigWith_ad[wChannelNo].wImpedance == 0)//B�� ����B�� ����λΪ00 �鿴�̵�������˵��
						{
							
							Receive_AllParamConfigWith_ad[wChannelNo].wImpedance = 2;
						}
					}
					allCfgdisplayImpedance(Wnd, wChannelNo, Receive_AllParamConfigWith_ad[wChannelNo].wImpedance);
					// �������õ�Ĭ���ļ�
					//allCfgSaveParamFile(lpDefaultParamFile);
					
				}
			}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgZuKangPiPeiProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���߷ֶ����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgQuXianFenDuanProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
				
					gbQuXianFenDuan = !gbQuXianFenDuan;
					allCfgDisplayQuXianFenDuan(Wnd);
				}
				/*else if(wParam == VK_RIGHT)
				{
				
					allCfgDisplayLiangDu(Wnd);
				}*/
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgQuXianFenDuanProc,Wnd,Msg,wParam,lParam);
}

/**************************
*Ƶ�׷������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPinPuFenXiProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
				
					gbPinPuFenXi= !gbPinPuFenXi;
					allCfgDisplayPinPuFenXi(Wnd);
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPinPuFenXiProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����У�鰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgXingNengJiaoYanProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
				
					gbXingNengJiaoYan= !gbXingNengJiaoYan;
					allCfgDisplayXingNengJiaoYan(Wnd);
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgXingNengJiaoYanProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�������갴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgJuLiZuoBiaoProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					gwJuLiZuoBiao --;
					if(gwJuLiZuoBiao<0)
						gwJuLiZuoBiao = 2;
					
					allCfgDisplayJuLiZuoBiao(Wnd);
				}
				else if(wParam == VK_RIGHT)
				{
					gwJuLiZuoBiao ++;
					if(gwJuLiZuoBiao >2)
						gwJuLiZuoBiao = 0;
					allCfgDisplayJuLiZuoBiao(Wnd);
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgJuLiZuoBiaoProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�ܲ��⾶��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgGuanCaiWaiJingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if((gfParamGuanCaiWaiJing - fWaiJingStep) >= 0)
						gfParamGuanCaiWaiJing = gfParamGuanCaiWaiJing - fWaiJingStep;
						else
						gfParamGuanCaiWaiJing =0;
					
						allCfgDisplayGuanCaiWaiJing(Wnd, gfParamGuanCaiWaiJing);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((gfParamGuanCaiWaiJing + fWaiJingStep) <= 1000)
							gfParamGuanCaiWaiJing = gfParamGuanCaiWaiJing + fWaiJingStep;
						else
							gfParamGuanCaiWaiJing = 1000;
						
						allCfgDisplayGuanCaiWaiJing(Wnd,gfParamGuanCaiWaiJing);
					}
				}
				else if(wParam == 'F')
				{
					if(wWaiJingStepIndex < 3)
						wWaiJingStepIndex++;
					else
						wWaiJingStepIndex = 0;

					fWaiJingStep = AMENDSTEPS[wWaiJingStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fWaiJingStep);
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgGuanCaiWaiJingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�ܲ��ھ���ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgGuanCaiNeiJingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
					
					if((gfGuanCaiNeiJing - fNeiJingStep)> MIN_GUANCAIWAIJING)
						gfGuanCaiNeiJing -= fNeiJingStep;
					else
						gfGuanCaiNeiJing =MIN_GUANCAIWAIJING;
					
					allCfgDisplayGuanCaiNeiJing(Wnd);
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((gfGuanCaiNeiJing + fNeiJingStep) < gfParamGuanCaiWaiJing)
							gfGuanCaiNeiJing += fNeiJingStep;
						else
							gfGuanCaiNeiJing = gfParamGuanCaiWaiJing - 0.1f;
						
						allCfgDisplayGuanCaiNeiJing(Wnd);
					}
				}
				else if(wParam == 'F')
				{
					if(wNeiJingStepIndex < 3)
						wNeiJingStepIndex++;
					else
						wNeiJingStepIndex = 0;

					fNeiJingStep = AMENDSTEPS[wNeiJingStepIndex];

					allCfgDisplayStepFloat(hWndAllParamCfgCaption, fNeiJingStep);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgGuanCaiNeiJingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��Ӧ�ٶȰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgXiangYingSpeedProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						gnXiangYingSpeed --;
						if(gnXiangYingSpeed<0)
							gnXiangYingSpeed = 2; 
					
						
						allCfgDisplayXiangYingSpeed(Wnd);
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						gnXiangYingSpeed++;
						if(gnXiangYingSpeed>2)
							gnXiangYingSpeed = 0;
						
						allCfgDisplayXiangYingSpeed(Wnd);
					}
				
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgXiangYingSpeedProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���油����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgBiaoMianBuChangProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
					if(gwBiaoMianBuChang<=0)
						gwBiaoMianBuChang= 0; 
					else
						gwBiaoMianBuChang--;
				
					
					allCfgDisplayBiaoMianBuChang(Wnd);
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
					if(gwBiaoMianBuChang>=10)
						gwBiaoMianBuChang = 10;
					else
						gwBiaoMianBuChang++;
					
					allCfgDisplayBiaoMianBuChang(Wnd);
					}
				
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgBiaoMianBuChangProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ӡ���Ͱ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPrintTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					gnPrintType--;
					if(gnPrintType <0)
						gnPrintType = 3;
				
					
				allCfgDisplayPrintType(Wnd);
				
				}
				else if(wParam == VK_RIGHT)
				{
					gnPrintType++;
					if(gnPrintType>3)
						gnPrintType =0;
				allCfgDisplayPrintType(Wnd);
				
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPrintTypeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�����𲨰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgLieWenQiBoProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					
					if(gwLieWenQiBo>=1)
						gwLieWenQiBo -=1;
					else
						gwLieWenQiBo = 0;
				
					
				allCfgDisplayLieWenQiBo(Wnd);
				
				}
				else if(wParam == VK_RIGHT)
				{
					
					if(gwLieWenQiBo>=25)
						gwLieWenQiBo=25;
					else
						gwLieWenQiBo++;
				allCfgDisplayLieWenQiBo(Wnd);
				
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgLieWenQiBoProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPingDingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						
						if(gfParamPingDing[wChannelNo] != MIN_CURVE_GAIN)
						{
							if((gfParamPingDing[wChannelNo] - fParamCurveGainStep) > MIN_CURVE_GAIN)
							{
								gfParamPingDing[wChannelNo] -= fParamCurveGainStep;
							}
							else
								gfParamPingDing[wChannelNo] = MIN_CURVE_GAIN;
							
							allCfgDisplayFloat1(Wnd, gfParamPingDing[wChannelNo]);

						}
						
					
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gfParamPingDing[wChannelNo] != MAX_CURVE_GAIN)
						{
							if((gfParamPingDing[wChannelNo] + fParamCurveGainStep) < gfParamDingLiang[wChannelNo])
							{
								gfParamPingDing[wChannelNo] += fParamCurveGainStep;
							}

							
							allCfgDisplayFloat1(Wnd,gfParamPingDing[wChannelNo]);
					
						}
					}
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPingDingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgDingLiangProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gfParamDingLiang[wChannelNo] != MIN_CURVE_GAIN)
						{
							if((gfParamDingLiang[wChannelNo] - fParamCurveGainStep) > gfParamPingDing[wChannelNo])
							{
								gfParamDingLiang[wChannelNo] -= fParamCurveGainStep;
							}
						
							allCfgDisplayFloat1(Wnd, gfParamDingLiang[wChannelNo]);
						}
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gfParamDingLiang[wChannelNo] != MAX_CURVE_GAIN)
						{
							if((gfParamDingLiang[wChannelNo] + fParamCurveGainStep) < gfParamPanFei[wChannelNo])
							{
								gfParamDingLiang[wChannelNo] += fParamCurveGainStep;
							}

							allCfgDisplayFloat1(Wnd, gfParamDingLiang[wChannelNo]);
						}
					}
				
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgDingLiangProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�зϰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPanFeiProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gfParamPanFei[wChannelNo] != MIN_CURVE_GAIN)
						{
							
								if((gfParamPanFei[wChannelNo] - fParamCurveGainStep) > gfParamDingLiang[wChannelNo])
								{
									gfParamPanFei[wChannelNo] -= fParamCurveGainStep;
								}

								allCfgDisplayFloat1(Wnd, gfParamPanFei[wChannelNo]);
						}
						
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gfParamPanFei[wChannelNo] != MAX_CURVE_GAIN)
						{
							if((gfParamPanFei[wChannelNo] + fParamCurveGainStep) < MAX_CURVE_GAIN)
							{
								gfParamPanFei[wChannelNo] += fParamCurveGainStep;
							}
							else
								gfParamPanFei[wChannelNo] = MAX_CURVE_GAIN;

							allCfgDisplayFloat1(Wnd, gfParamPanFei[wChannelNo]);
						}
					 }				
				}
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPanFeiProc,Wnd,Msg,wParam,lParam);
}
/**************************
*̽ͷ���Ͱ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTanTouTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(gbCanShuSuoDing)//ֻ��ʾ̽ͷ���ͣ������޸ģ���������ǵĴ�С�жϵ�ǰ��ֱ����б
					{
						InitializeCriticalSection(&csProbeType);
						EnterCriticalSection(&csProbeType);

						nTanTouType[wChannelNo] = !nTanTouType[wChannelNo];
						
						allCfgDisplayTanTouType(Wnd);
						if(nTanTouType[wChannelNo] == 0)
						{
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
							
							allCfgDisplaySpeed(hBtnAllParamCfgSpeed);
							Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao = 0.1f;
							double fAngle = Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao/180.0f * PI;
							fKValue = (float)tan(fAngle); 
						}
						else if(nTanTouType[wChannelNo] == 1)
						{
							
							Display_AllParamConfigWith_ad[wChannelNo].nWaveSpeed = allCfgtypeToWaveSpeed(wMaterialType);
							allCfgDisplaySpeed(hBtnAllParamCfgSpeed);
							Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao = 0.0f;
							double fAngle = Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao/180.0f * PI;
							fKValue = (float)tan(fAngle); 
							
						}
						allCfgDisplayFloat1(hBtnAllParamCfgAngle,Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
						allCfgDisplayKValue(hBtnAllParamCfgKValue,Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);

						LeaveCriticalSection(&csProbeType);
						DeleteCriticalSection(&csProbeType);
					}
				}
				
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTanTouTypeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�������㰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgCanShuQinLingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						gbCanShuQinLing= !gbCanShuQinLing;
						
						allCfgDisplayCanShuQinLing(Wnd);
					}
				}
				
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgCanShuQinLingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ͨ�����㰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgTongDaoQinLingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					
					gbTongDaoQinLing= !gbTongDaoQinLing;
					
					allCfgDisplayTongDaoQinLing(Wnd);
				
				}
				
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgTongDaoQinLingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgCanShuSuoDingProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					
					gbCanShuSuoDing= !gbCanShuSuoDing;
				
					allCfgDisplayCanShuSuoDing(Wnd);
				
				}
				
			
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgCanShuSuoDingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���ߦ�1ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgQuXianGuiGe1Proc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT )
				{
					if(!gbCanShuSuoDing)
					{
					
						if((gfParamFaiZhi1[wChannelNo] - fParamFaiStep)  > 1)
							gfParamFaiZhi1[wChannelNo] = gfParamFaiZhi1[wChannelNo] - fParamFaiStep;
						else
							gfParamFaiZhi1[wChannelNo] = 1;
						
						allCfgDisplayFloat1(Wnd, gfParamFaiZhi1[wChannelNo]);
					}				
				}
				
				else if( wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((gfParamFaiZhi1[wChannelNo] + fParamFaiStep) <= 20)
							gfParamFaiZhi1[wChannelNo] = gfParamFaiZhi1[wChannelNo] + fParamFaiStep;
						else
							gfParamFaiZhi1[wChannelNo] = 20;

						allCfgDisplayFloat1(Wnd, gfParamFaiZhi1[wChannelNo]);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgQuXianGuiGe1Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*���ߦ�2ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgQuXianGuiGe2Proc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT )
				{
					if(!gbCanShuSuoDing)
					{
					
						if((gfParamFaiZhi2[wChannelNo] - fParamFaiStep)  > 0)
							gfParamFaiZhi2[wChannelNo] = gfParamFaiZhi2[wChannelNo] - fParamFaiStep;
						else
							gfParamFaiZhi2[wChannelNo] = 0;
						
						allCfgDisplayFloat1(Wnd, gfParamFaiZhi2[wChannelNo]);
					}
				
				}
				
				else if( wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((gfParamFaiZhi2[wChannelNo] + fParamFaiStep) <= 20)
							gfParamFaiZhi2[wChannelNo] = gfParamFaiZhi2[wChannelNo] + fParamFaiStep;
						else
							gfParamFaiZhi2[wChannelNo] = 20;

						allCfgDisplayFloat1(Wnd,gfParamFaiZhi2[wChannelNo]);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgQuXianGuiGe2Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*���ߦ�3ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgQuXianGuiGe3Proc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT )
				{
					if(!gbCanShuSuoDing)
					{
					
						if((gfParamFaiZhi3[wChannelNo] - fParamFaiStep)  > 0)
							gfParamFaiZhi3[wChannelNo] = gfParamFaiZhi3[wChannelNo] - fParamFaiStep;
						else
							gfParamFaiZhi3[wChannelNo] = 0;
						
						allCfgDisplayFloat1(Wnd, gfParamFaiZhi3[wChannelNo]);
					}
				
				}
				
				else if( wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if((gfParamFaiZhi3[wChannelNo] + fParamFaiStep) <= 20)
							gfParamFaiZhi3[wChannelNo] = gfParamFaiZhi3[wChannelNo] + fParamFaiStep;
						else
							gfParamFaiZhi3[wChannelNo] = 20;

						allCfgDisplayFloat1(Wnd, gfParamFaiZhi3[wChannelNo]);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgQuXianGuiGe3Proc,Wnd,Msg,wParam,lParam);
}

/**************************
*�Զ����߰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgZiDongBoGaoProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT )
				{
					
					
					if(gwZiDongBoGao<=0)
						gwZiDongBoGao= 0;
					else
						gwZiDongBoGao--;
					
					allCfgDisplayZiDongBoGao(Wnd);
				
				}
				
				else if( wParam == VK_RIGHT)
				{
					
					if(gwZiDongBoGao>=100)
						gwZiDongBoGao= 100;
					else
						gwZiDongBoGao++;
					allCfgDisplayZiDongBoGao(Wnd);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgZiDongBoGaoProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�������Ͱ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgPlusTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					
					gwPlusType = !gwPlusType;
					
					
					allCfgDisplayPlusType(Wnd);
				
				}
				
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgPlusTypeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ʽ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgWorkTypeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam == VK_LEFT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gnWorkType <=1 )
							gnWorkType = 1;
						else 
						{
							gnWorkType --;
							if(gnWorkType == 2)
								gnWorkType = 1;
						}
						if(gnWorkType == 3)
							allCfgdisplayImpedance(hBtnAllParamCfgZuKangPiPei,wChannelNo,Receive_AllParamConfigWith_ad[wChannelNo].wImpedance);

						if(gnWorkType == 4 || gnWorkType == 5 || gnWorkType == 6)
						{
							allCfgCh1Data2Ch2Data();//��ͨ��һ�����ݸ��Ƶ�ͨ������

							wChannelNo = 1;
							allCfgDisplayChNo(hWndAllParamCfgCaption);
							//allCfgDisplayVoltage(hBtnAllParamCfgVoltage, Send_AllParamConfigWith_ad[wChannelNo].nPress);
							if(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao == 0)
							{
								allCfgDisplayQianYan(hBtnAllParamCfgQianYan, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
							}
							else
							{
								allCfgDisplayFloat1(hBtnAllParamCfgQianYan, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
							}
							allCfgDisplayMaiKuan(hBtnAllParamCfgMaiKuan);
							allCfgDisplayRate(hBtnAllParamCfgRate);
							allCfgDisplayAmend(hBtnAllParamCfgAmend);
							allCfgDisplayJianBo(hBtnAllParamCfgJianBo, Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode);
							allCfgDisplayPlus(hBtnAllParamCfgPlus);
							allCfgDisplayRefPlus(hBtnAllParamCfgRefPlus);
							allCfgDisplayDelay(hBtnAllParamCfgDelay);
							allCfgDisplayLvBo(hBtnAllParamCfgLvBo, Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq);
							allCfgDisplayRefractMode(hBtnAllParamCfgRefractMode);
							allCfgDisplaySpeedType(hBtnAllParamCfgSpeedType, wMaterialType);
							allCfgDisplaySpeed(hBtnAllParamCfgSpeed);
							allCfgDisplayRange(hBtnAllParamCfgRange);
							allCfgDisplayAngle(hBtnAllParamCfgAngle);
							allCfgDisplayKValue(hBtnAllParamCfgKValue, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
							allCfgDisplayBate(hBtnAllParamCfgBate);
							allCfgdisplayImpedance(hBtnAllParamCfgZuKangPiPei,wChannelNo,Receive_AllParamConfigWith_ad[wChannelNo].wImpedance);
							
						}	

						if(gnWorkType == 5)
						{
							if((gFunctionDefine & WORKMODEDANJING_SWITCH) == 0)
							{
								gnWorkType = 4;
								if((gFunctionDefine & WORKMODE1TO2_SWITCH) == 0)
								{
									gnWorkType = 3;
									if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) == 0)
									{
										gnWorkType = 1;
									}
								}
							}
						}
						else if (gnWorkType == 4)
						{
							if((gFunctionDefine & WORKMODE1TO2_SWITCH) == 0)
							{
								gnWorkType = 3;
								if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) == 0)
								{
									gnWorkType = 1;
								}
							}
						}
						else if(gnWorkType == 3)
						{
							if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) == 0)
							{
								gnWorkType = 1;
							}
						}
						else if(gnWorkType == 1)
						{
							if((gFunctionDefine & WORKMODE1OR2_SWITCH) == 0)
							{
								gnWorkType = 3;
							}
						}
						
						gAllcfgSysInfo.wWorkMode = (WORKMODE)gnWorkType;
						allCfgDisplayWorkType(Wnd,gnWorkType);
					}
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(!gbCanShuSuoDing)
					{
						if(gnWorkType >=6)
							gnWorkType = 6;
						else
						{
							gnWorkType ++;
							if(gnWorkType == 2)
								gnWorkType = 3;
						}
						if(gnWorkType == 3)
							allCfgdisplayImpedance(hBtnAllParamCfgZuKangPiPei,wChannelNo,Receive_AllParamConfigWith_ad[wChannelNo].wImpedance);
						if(gnWorkType == 4 || gnWorkType == 5 || gnWorkType == 6)
						{
							allCfgCh1Data2Ch2Data();//��ͨ��һ�����ݸ��Ƶ�ͨ������

							wChannelNo = 1;
							allCfgDisplayChNo(hWndAllParamCfgCaption);
							//allCfgDisplayVoltage(hBtnAllParamCfgVoltage, Send_AllParamConfigWith_ad[wChannelNo].nPress);
							if(Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao == 0)
							{
								allCfgDisplayQianYan(hBtnAllParamCfgQianYan, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
							}
							else
							{
								allCfgDisplayFloat1(hBtnAllParamCfgQianYan, Send_AllParamConfigWith_ad[wChannelNo].fQianYan);
							}
							allCfgDisplayMaiKuan(hBtnAllParamCfgMaiKuan);
							allCfgDisplayRate(hBtnAllParamCfgRate);
							allCfgDisplayAmend(hBtnAllParamCfgAmend);
							allCfgDisplayJianBo(hBtnAllParamCfgJianBo, Receive_AllParamConfigWith_ad[wChannelNo].nJianBoMode);
							allCfgDisplayPlus(hBtnAllParamCfgPlus);
							allCfgDisplayRefPlus(hBtnAllParamCfgRefPlus);
							allCfgDisplayDelay(hBtnAllParamCfgDelay);
							allCfgDisplayLvBo(hBtnAllParamCfgLvBo, Receive_AllParamConfigWith_ad[wChannelNo].nProbeFreq);
							allCfgDisplayRefractMode(hBtnAllParamCfgRefractMode);
							allCfgDisplaySpeedType(hBtnAllParamCfgSpeedType, wMaterialType);
							allCfgDisplaySpeed(hBtnAllParamCfgSpeed);
							allCfgDisplayRange(hBtnAllParamCfgRange);
							allCfgDisplayAngle(hBtnAllParamCfgAngle);
							allCfgDisplayKValue(hBtnAllParamCfgKValue, Display_AllParamConfigWith_ad[wChannelNo].fZheSheJiao);
							allCfgDisplayBate(hBtnAllParamCfgBate);
							allCfgdisplayImpedance(hBtnAllParamCfgZuKangPiPei,wChannelNo,Receive_AllParamConfigWith_ad[wChannelNo].wImpedance);
							
						}
						
						if(gnWorkType == 3 )
						{
							if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) == 0)
							{
								gnWorkType = 4;
								if((gFunctionDefine & WORKMODE1TO2_SWITCH) == 0)
								{
									gnWorkType = 5;
									if((gFunctionDefine & WORKMODEDANJING_SWITCH) == 0)
									{
										gnWorkType = 6;
									}
								}
							}
							
							
						}

						else if(gnWorkType == 4)
						{
							if((gFunctionDefine & WORKMODE1TO2_SWITCH) == 0)
							{
								gnWorkType = 5;
								if((gFunctionDefine & WORKMODEDANJING_SWITCH) == 0)
								{
									gnWorkType = 6;
								}
							}
						}

						else if(gnWorkType == 5)
						{
							if((gFunctionDefine & WORKMODEDANJING_SWITCH) == 0)
							{
								gnWorkType = 6;
							}
						}

						else if(gnWorkType == 6)
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) == 0)
							{
								gnWorkType = 5;
							}
						}
						
						gAllcfgSysInfo.wWorkMode = (WORKMODE)gnWorkType;
						allCfgDisplayWorkType(Wnd, gnWorkType);
					}
				}
				
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgWorkTypeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*Ӳ���汾�Ű�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgHardWareProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgHardWareProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����汾�Ű�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgSoftWareProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgSoftWareProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�豸�ͺŰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgEquipmentModelProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = buttonNum - 1;
					
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgEquipmentModelProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�´�У׼ʱ�䰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgNextAdjustTimeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgNextAdjustTimeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����У׼ʱ�䰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK btnAllParamCfgThisAdjustTimeProc(HWND Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					hWndAllParamConfirm = CreateWindow(szAllParamConfirmChild, TEXT(""), WS_BORDER|WS_POPUP|WS_VISIBLE,
							 200, 210, 240, 60, hWndMain, NULL, hInst, NULL);
				}
			
				else if(wParam == VK_UP)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				else if(wParam ==VK_DOWN)
				{
					while(allParamCfgBtnList[i] != currentAllParamCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = buttonNum - 1;
					currentAllParamCfgBtn = allParamCfgBtnList[i];
					SetFocus(currentAllParamCfgBtn);
					
					HWND hParent = GetParent(currentAllParamCfgBtn);

					SendMessage(hParent, Msg, wParam, lParam);
				}
				
				else if(wParam == '\t')
				{
					SendMessage(hWndAllParamCfg, WM_KEYDOWN, wParam, lParam);
				}
				
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamCfgThisAdjustTimeProc,Wnd,Msg,wParam,lParam);
}