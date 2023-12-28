/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���ղ�������ȷ�ϴ���Դ�ļ�
 * Filename: allParamConfirmWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "craftParamWin.h"

HWND hWndAllParamConfirm = NULL;//���ղ�������ȷ�ϴ��ھ��
HWND hBtnAllParamConfirmsave = NULL;//���水ť���
HWND hBtnAllParamConfirmNoSave = NULL;//�����水ť���
HWND hBtnAllParamConfirmCancel = NULL;//ȡ����ť���
HWND allParamConfirmBtnList[3] ={NULL,NULL,NULL};//���ղ�������ȷ�ϴ��ڵİ�ť��
WNDPROC OldBtnAllParamConfirmsaveProc = NULL;//���水ťĬ�ϴ������
WNDPROC OldBtnAllParamConfirmNoSaveProc = NULL;//�����水ťĬ�ϴ������
WNDPROC OldBtnAllParamConfirmCancelProc = NULL;//ȡ����ťĬ�ϴ������
HWND currentAllParamConfirmBtn = NULL;//���ղ�������ȷ�ϴ��е�ǰѡ�еİ�ť
static const int buttonNum = 3;//��ť�ؼ���

ATOM registerAllParamConfirmChild(HINSTANCE hInstance);
static int allParamConfirm_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK allParamConfirmChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAllParamConfirmsaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamConfirmNoSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamConfirmCancelProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HWND hWndMain;
extern HINSTANCE hInst;//����ʵ�����
extern HWND hStatusBar;
extern HWND hWndCraftParam;
extern HWND currentCraftParamBtn;
extern HWND hWndAllParamCfgCaption;	//���ղ������ñ��ⴰ�ھ��
extern HWND hWndAllParamCfg;		//���ղ������ô��ھ��
extern HWND gCurrentWnd;			//��ǰ��ʾ���Ӵ���
extern HWND currentAllParamCfgBtn;
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���
extern SEND_PARAM_DATA SendParam_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern SEND_PARAM_DATA Send_AllParamConfigWith_ad[2];
extern RECEIVE_PARAM_DATA Receive_AllParamConfigWith_ad[2];
extern DISPLAY_SET_DLG_DATA Display_AllParamConfigWith_ad[2];
extern TCHAR ParamFileName[MAX_PATH];
extern WORD gnParamNo;					//�������
extern DWORD currentUsedParamNo;		//��ǰʹ�õĹ��ղ�����
extern SYSTEMTIME st;//ϵͳʱ��
//extern allParamConfigWithTime AllParam_Time;//���ղ���ʱ��

//zhaoli add in 2010 -9-16
//extern float fBScanGangBanHouDu;//������Ȳ���
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

extern float gfPingDingGain[CHANNEL_NUM];				//��������
extern float gfDingLiangGain[CHANNEL_NUM];				//��������
extern float gfPanHuiGain[CHANNEL_NUM];				//�з�����
extern float gfParamPingDing[CHANNEL_NUM];				
extern float gfParamDingLiang[CHANNEL_NUM];	
extern float gfParamPanFei[CHANNEL_NUM];


WORD wTanTouLeiXing[CHANNEL_NUM]= {1, 1};//̽ͷ����

extern WORD nTanTouType[CHANNEL_NUM];		//̽ͷ����

extern SYS_INFO gSysInfo;
extern SYS_INFO gAllcfgSysInfo;
extern HWND hWndMain;

extern float gwQuXianGuiGe1;
extern float gwQuXianGuiGe2;
extern float gwQuXianGuiGe3;
extern float gfDefectZhi1[CHANNEL_NUM];
extern float gfDefectZhi2[CHANNEL_NUM];
extern float gfDefectZhi3[CHANNEL_NUM];

extern wchar_t gszTextChannel[10];
extern int gnWorkType;
extern WORD wChannelNo;
extern float allParamBScanGongJianHouDu;//Bɨ�������
extern WORD allParamScanMode;//Bɨɨ�跽ʽ
extern BOOL allParamBscanSwitch;//Bɨ����
extern float allParamPlateHeight;//�к��̽�˹������
extern WORD allParamProbeType;//�к��̽��̽ͷ����
extern float allParamUpBlindValue;//�к��̽����ä��
extern float allParamDownBlindValue;//�к��̽����ä��
extern WORD allParamPoKouLeiXing;//����̽���¿���������
extern float allParamPoKouJiaoDu;//����̽���¿ڽǶ�����
extern float  allParamPoKouShenDu;//����̽���¿��������
extern BOOL allParamHanFengTanShangSwitch;//����̽�˿���
extern float allParamHanFengTanShangGangBanHouDu;//����̽�˹������
extern float allParamHead2HanFengDistance;//̽ͷ���¿ڵľ���
extern BOOL allParamLCDValue;//��������
extern HWND gCurrentFocusBtn;

//zhaoli add in 2010 -9-16
// songchenguang add start 2010-12-08
extern BOOL gbHotCraftParamView;
extern WORD gwGongYiCanShu;
// songchenguang add end 2010-12-08
extern BOOL gAllCfgDgsSwitch[CHANNEL_NUM][GATE_NUM];	//DGSͨ������
extern float gfAllCfgOldRange[CHANNEL_NUM];				//�������DACʱ������
extern WORD gwAllCfgOldSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
extern float fAllCfgOldRange[CHANNEL_NUM];			//�ն�ȡʱ������
extern float fAllCfgOldPlus[CHANNEL_NUM];			//�ն�ȡʱ������
extern float fAllCfgOldRefPlus[CHANNEL_NUM];		//�ն�ȡʱ�Ĳο�����
extern float fAllCfgOldDelay[CHANNEL_NUM];			//�ն�ȡʱ���ӳ�ֵ


extern wchar_t lpDefaultParamFile[MAX_PATH];
extern DWORD gAllCfgSelectedDgsPointDataNum[CHANNEL_NUM][GATE_NUM];//��ѡ�е�DGS������
extern CURVEPOINT_DATA gAllCfgSelectedDgsPointData[CHANNEL_NUM][GATE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD CurrentChannelNo;				// ��ǰͨ����
extern ARS200FPGA_SYS_REG* sys_reg;
extern float gfParamGuanCaiWaiJing;//�ܲ��⾶
extern float gfGuanCaiNeiJing;//�ܲ��ھ�
extern float gfNeiJing;//�ܲ��ھ�
extern float gfWaiJing;//�ܲ��⾶
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//��ѡ�е�AVG������
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���

extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DGS������
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���

extern float gfParamFaiZhi1[CHANNEL_NUM];//���ߦ�1ֵ
extern float gfParamFaiZhi2[CHANNEL_NUM];//���ߦ�2ֵ
extern float gfParamFaiZhi3[CHANNEL_NUM];//���ߦ�3ֵ

extern BOOL  gbHotGateFlag;	//�Ƿ��բ�ſ�ݷ�ʽ wangbingfu add at 2010-8-9
extern BOOL  gbHotAutoGainFlag;	//�Ƿ���Զ������ݷ�ʽ 
extern BOOL  gbHotAutoAdjustFlag;	//�Ƿ���Զ�У׼��ݷ�ʽ 
extern HWND hWndGate;				// բ���趨���ھ��
extern HWND currentGateBtn;			// բ���趨���е�ǰѡ�еİ�ť
extern HWND hWndAutoGainPlus;//�Զ����洰�ھ��
extern HWND currentAutoGainPlusBtn;//�Զ����洰�е�ǰѡ�еİ�ť
extern HWND hWndAdjust;						//У׼���ھ��
extern HWND currentAdjustBtn;					//У׼���е�ǰѡ�еİ�ť

extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];		//��ӵ��������ݶ���
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];											//��ѡ�е�DGS������
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];	//��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];													//��ѡ�е�AVG������
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������AVGʱ��ÿ�����λ��
extern float gfOldDacRange[CHANNEL_NUM];						//�������DACʱ������
extern float gfOldAvgRange[CHANNEL_NUM];						//�������AVGʱ������

extern float gfAllOldRangeStart[CHANNEL_NUM];		//���湤�ղ���֮ǰ����ʼ����
extern float gfAllOldRangeEnd[CHANNEL_NUM];			//���湤�ղ���֮ǰ���յ�����
extern float fRangeStart[2];

extern BOOL allCfgSaveParamFile(LPCTSTR lpszPathName);
extern BOOL ReadParamFile(LPCTSTR lpszPathName);

//wangbingfu add start at 2011-5-25
/*//wangbingfu delete at 2011-5-27
extern void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag); 		//��·
//extern BOOL gbTlParamFlag;	//��ǰ�Ƿ�ʹ����·����	
*/
//wangbingfu add end at 2011-5-25

//wangbingfu add start at 2011-5-27
extern WORD gwDeviceType;	//�豸���� 0:̽����	1:����� 2:��·ר��
extern WORD gwStandard[CHANNEL_NUM];	//����׼
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//��ⲿλ
extern BOOL gbTlSheDingFlag[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ�Ƿ����趨��־
extern float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];			//����׼����λ����
extern float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//����׼����λ����
extern float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ��������
//wangbingfu add end at 2011-5-27

/**************************
*ע�Ṥ�ղ�������ȷ�ϴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAllParamConfirmChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) allParamConfirmChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAllParamConfirmChild;

	return RegisterClass(&wc);
}
/**************************
*���ղ�������ȷ�ϴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK allParamConfirmChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
//	LPDRAWITEMSTRUCT pdis;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:
			//	if(hStaticBrush == NULL)	
			//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			//����ť�ؼ�
			hBtnAllParamConfirmsave = CreateWindow(_T("BUTTON"),_T("save"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,80,60,hWnd,(HMENU)IDC_ALLPARAMCONFIRM_SAVE,hInst,NULL);
			hBtnAllParamConfirmNoSave = CreateWindow(_T("BUTTON"),_T("NoSave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,80,
					0,80,60,hWnd,(HMENU)IDC_ALLPARAMCONFIRM_NOSAVE,hInst,NULL);
			hBtnAllParamConfirmCancel = CreateWindow(_T("BUTTON"),_T("Cancel"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,160,
					0,80,60,hWnd,(HMENU)IDC_ALLPARAMCONFIRM_CANCEL,hInst,NULL);
		
			allParamConfirmBtnList[0] = hBtnAllParamConfirmsave;
			allParamConfirmBtnList[1] = hBtnAllParamConfirmNoSave;
			allParamConfirmBtnList[2] = hBtnAllParamConfirmCancel;

			//��ť�ؼ����Զ��崦�����
			OldBtnAllParamConfirmsaveProc = (WNDPROC) GetWindowLong(hBtnAllParamConfirmsave,GWL_WNDPROC);
			SetWindowLong(hBtnAllParamConfirmsave,GWL_WNDPROC,(LONG)btnAllParamConfirmsaveProc);
			OldBtnAllParamConfirmNoSaveProc = (WNDPROC) GetWindowLong(hBtnAllParamConfirmNoSave,GWL_WNDPROC);
			SetWindowLong(hBtnAllParamConfirmNoSave,GWL_WNDPROC,(LONG)btnAllParamConfirmNoSaveProc);
			OldBtnAllParamConfirmCancelProc = (WNDPROC) GetWindowLong(hBtnAllParamConfirmCancel,GWL_WNDPROC);
		    SetWindowLong(hBtnAllParamConfirmCancel,GWL_WNDPROC,(LONG)btnAllParamConfirmCancelProc);
			
			currentAllParamConfirmBtn = allParamConfirmBtnList[0];//Ĭ�ϵ�ǰѡ�е����ղ�������ȷ�ϰ�ť
			SetFocus(currentAllParamConfirmBtn);//ѡ�а�ť������뽹��
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;

		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			allParamConfirm_DrawButtonBitmap(hInst,pdis);
		/*	if(pdis->hwndItem == hBtnAllParamConfirmsave)
			{
				drawSrting(pdis->hwndItem,  _T("����"), 50, STATIC_TEXT_COLOR, 
								0, 0,CONFIRM_STRING_WIDTH, CONFIRM_STRING_HEIGHT);
			}
			if(pdis->hwndItem == hBtnAllParamConfirmNoSave)
			{
				drawSrting(pdis->hwndItem,  _T("����"), 50, STATIC_TEXT_COLOR, 
								0, 0,CONFIRM_STRING_WIDTH, CONFIRM_STRING_HEIGHT);
			}
			if(pdis->hwndItem == hBtnAllParamConfirmCancel)
			{
				drawSrting(pdis->hwndItem,  _T("ȡ��"), 50, STATIC_TEXT_COLOR, 
								0, 0,CONFIRM_STRING_WIDTH, CONFIRM_STRING_HEIGHT);
			}*/
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
		//	if(hStaticBrush)
		//	{
		//		DeleteObject(hStaticBrush);
		//		hStaticBrush = NULL;
		//	}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*���ع��ղ�������ȷ�ϴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int allParamConfirm_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_allParamConfirmBtnBitmap[]=
	{
		{IDC_ALLPARAMCONFIRM_SAVE	,IDB_BITMAP_CONFIRM_BAOCUN_NOSEL_R	 ,  IDB_BITMAP_CONFIRM_BAOCUN_SELECT_R		},
		{IDC_ALLPARAMCONFIRM_NOSAVE	,IDB_BITMAP_CONFIRM_FANGQI_NOSEL_R	 ,  IDB_BITMAP_CONFIRM_FANGQI_SELECT_R		},
		{IDC_ALLPARAMCONFIRM_CANCEL	,IDB_BITMAP_CONFIRM_QUXIAO_NOSEL_R ,  IDB_BITMAP_CONFIRM_QUXIAO_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_allParamConfirmBtnBitmap);
	return ReturnVal;
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
static LRESULT CALLBACK btnAllParamConfirmsaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
			
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}
			
				else if(wParam == VK_LEFT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RIGHT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RETURN)
				{	


					for(int i = 0;i<2;i++)
					{
						wTanTouLeiXing[i] = nTanTouType[i];
					}
			
					//zhaoli add in 2010 9 16
		//			fBScanGangBanHouDu = allParamBScanGongJianHouDu;
					wScanMode = allParamScanMode;
					//gbBscanSwitch = allParamBscanSwitch;
					gfPlateHeight = allParamPlateHeight;
					//nProbeType = allParamProbeType;
					gfUpBlindValue = allParamUpBlindValue;
					gfDownBlindValue = allParamDownBlindValue;
					gwPoKouLeiXing = allParamPoKouLeiXing;
					gfPoKouJiaoDu = allParamPoKouJiaoDu;
					gfPoKouShenDu = allParamPoKouShenDu;
				//	bHanFengTanShangSwitch = allParamHanFengTanShangSwitch;
					fHanFengTanShangGangBanHouDu = allParamHanFengTanShangGangBanHouDu;
					fHead2HanFengDistance = allParamHead2HanFengDistance;
					nLCDValue = allParamLCDValue;
					gfWaiJing = gfParamGuanCaiWaiJing;
					gfNeiJing = gfGuanCaiNeiJing;
					CurrentChannelNo = wChannelNo;//�޸�ͨ��
					gAllcfgSysInfo.wCurrentChNo = wChannelNo;
					

					
					for( int j = 0; j< CHANNEL_NUM; j++)
					{
						if(gSelectedDgsPointDataNum[j][3] > 1)
						{
							for(int i = 0; i < gSelectedDgsPointDataNum[j][3]; i++)
							{	
								gSelectedDgsPointData[j][0][i].data = (WORD)(gSelectedDgsPointData[j][3][i].data 
																							/ pow(10, (double)gfParamPingDing[j] / (-20)));
								gSelectedDgsPointData[j][0][i].serial = gSelectedDgsPointData[j][3][i].serial;
								
								gSelectedDgsPointData[j][1][i].data = (WORD)(gSelectedDgsPointData[j][3][i].data 
																							/ pow(10, (double)gfParamDingLiang[j] / (-20)));
								gSelectedDgsPointData[j][1][i].serial = gSelectedDgsPointData[j][3][i].serial;
								
								gSelectedDgsPointData[j][2][i].data = (WORD)(gSelectedDgsPointData[j][3][i].data 
																							/ pow(10, (double)gfParamPanFei[j] / (-20)));
								gSelectedDgsPointData[j][2][i].serial = gSelectedDgsPointData[j][3][i].serial;							
							}
						}
					}

					for( int i = 0; i< CHANNEL_NUM; i++)
					{
						gfPingDingGain[i] = gfParamPingDing[i];				
						gfDingLiangGain[i] = gfParamDingLiang[i];	
						gfPanHuiGain[i] = gfParamPanFei[i];
					}
					
					for( int i = 0; i< CHANNEL_NUM; i++)
					{
						if(gSelectedAvgPointDataNum[i] > 1)
						{
							for(int j = 0; j < AVG_CURVE_NUM; j++)
							{
								if(j == 0)
								{
									float fDeltaDB =(float)(40 * log10((float)gfParamFaiZhi1[i] / gfDefectZhi1[i]));
									for(int k = 0; k < gSelectedAvgPointDataNum[i]; k++)
									{
										gSelectedAvgPointData[i][j][k].data = (float)(gSelectedAvgPointData[i][j][k].data 
																							/ pow(10, (double)fDeltaDB / (-20)));
									}
								}
								else if(j == 1)
								{
									float fDeltaDB = (float)(40 * log10((float)gfParamFaiZhi2[i] / gfDefectZhi2[i]));
									for(int k = 0; k < gSelectedAvgPointDataNum[i]; k++)
									{
										gSelectedAvgPointData[i][j][k].data = (float)(gSelectedAvgPointData[i][j][k].data 
																							/ pow(10, (double)fDeltaDB / (-20)));
									}
								}
								else if(j == 2)
								{
									float fDeltaDB = (float)(40 * log10((float)gfParamFaiZhi3[i] / gfDefectZhi3[i]));
									for(int k = 0; k < gSelectedAvgPointDataNum[i]; k++)
									{
										gSelectedAvgPointData[i][j][k].data = (float)(gSelectedAvgPointData[i][j][k].data 
																							/ pow(10, (double)fDeltaDB / (-20)));
									}
								}
							}
						}
					}
					
					for( int i = 0; i< CHANNEL_NUM; i++)
					{
						gfDefectZhi1[i] = gfParamFaiZhi1[i];
						gfDefectZhi2[i] = gfParamFaiZhi2[i];
						gfDefectZhi3[i] = gfParamFaiZhi3[i];

					}
					
					
					
					//��ʾģʽ�ĸı�
					if(gnWorkType == 1)//��ͨ��
					{
						if(CurrentChannelNo == 0)
						{
							swprintf(gszTextChannel, _T("1"));
							SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
							
							gAllcfgSysInfo.wWorkMode = (WORKMODE)gnWorkType;
						}
						else if(CurrentChannelNo == 1)
						{
							swprintf(gszTextChannel, _T("2"));
							SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

							gnWorkType = 2;
							gAllcfgSysInfo.wWorkMode = (WORKMODE)gnWorkType;

						}
					}
			
					else if(gnWorkType == 3)//˫ͨ��
					{
						
						if(CurrentChannelNo == 0)
						{
							swprintf(gszTextChannel, _T("1"));
						}
						else if(CurrentChannelNo == 1)
						{
							swprintf(gszTextChannel, _T("2"));
						}

						SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					else if(gnWorkType == 4)//һ��һ��
					{
						CurrentChannelNo = 1;
						memset(gszTextChannel, 0, sizeof(gszTextChannel));
						
						swprintf(gszTextChannel, _T("%d"), CurrentChannelNo+1);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					else if(gnWorkType == 5)
					{
						
						memset(gszTextChannel, 0, sizeof(gszTextChannel));

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					else if(gnWorkType == 6)
					{
						SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)ShuangJingGuanLian);	

						memset(gszTextChannel, 0, sizeof(gszTextChannel));

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					
					gSysInfo = gAllcfgSysInfo;
					sys_reg->wLedBrightness = allParamLCDValue;
					
					if(gwDeviceType == 2)//��·ר��ʱ������������ο�����һ��  wangbingfu add at 2011-05-27
					{
						for(int i = 0; i < 2; i++)
						{
							gfTlRange[i][gwStandard[i]][gwPosition[i][gwStandard[i]]] = Display_AllParamConfigWith_ad[i].fRange;
							gfTlBuChangPlus[i][gwStandard[i]][gwPosition[i][gwStandard[i]]] = Receive_AllParamConfigWith_ad[i].fRefPlus;
							gfTlPlus[i][gwStandard[i]][gwPosition[i][gwStandard[i]]] = Receive_AllParamConfigWith_ad[i].fPlus;
						}
					}
					allCfgSaveParamFile(ParamFileName);
//					if(gnParamNo == currentUsedParamNo)


					currentUsedParamNo = gwGongYiCanShu;
					HKEY HKeyNum;
					LONG ret = RegOpenKeyEx(HKEY_CURRENT_USER, _T("ParNum"), 0, 0,&HKeyNum);
				
					if(ret != ERROR_SUCCESS)
					{
							MessageBox (Wnd, TEXT ("��ע���ʧ��"), TEXT("NDT_CE"), 0) ;
						
						return -1 ;
					}
					
					RegSetValueEx(HKeyNum, _T("ParNum"), 0, REG_DWORD, (const BYTE *)&currentUsedParamNo, sizeof(DWORD));
				
					RegCloseKey(HKeyNum);
					
					swprintf(lpDefaultParamFile, _T("\\ResidentFlash\\%d.par"), currentUsedParamNo);
			
					ReadParamFile(ParamFileName);
					// �Ѳ�������ȥ
					//InitParamDetail();
					InitParam();
			
					//�ı�����
					for(int i = 0; i < 2; i++)
					{
						if(fAllCfgOldDelay[i] - Receive_AllParamConfigWith_ad[i].fDelay != 0)
						{
							for(int j = 0; j < (int)gSelectedDgsPointDataNum[i][1]; j++)
							{
								for(int k = 0; k < DAC_CURVE_NUM; k++)
								{
									gSelectedDgsPointData[i][k][j].serial = curveChangeByDelay(gSelectedDgsPointData[i][k][j].serial, 
																								(fRangeStart[i] - gfAllOldRangeStart[i]), (gfAllOldRangeEnd[i] - gfAllOldRangeStart[i]));
								}
							}
							for(int j = 0; j < (int)gSelectedAvgPointDataNum[i]; j++)
							{
								for(int k = 0; k < AVG_CURVE_NUM + 1; k++)
								{
									gSelectedAvgPointData[i][k][j].serial = curveChangeByDelay(gSelectedAvgPointData[i][k][j].serial, 
																								(fRangeStart[i] - gfAllOldRangeStart[i]), (gfAllOldRangeEnd[i] - gfAllOldRangeStart[i]));
								}
							}
						}
						
						if(fAllCfgOldRange[i] - Display_AllParamConfigWith_ad[i].fRange != 0)
						{
							for(int j = 0; j < (int)gSelectedDgsPointDataNum[i][1]; j++)
							{
								for(int k = 0; k < DAC_CURVE_NUM; k++)
								{
									gSelectedDgsPointData[i][k][j].serial = (float)(gwOldDacSerial[i][j] 
																								* (gfOldDacRange[i] / Display_AllParamConfigWith_ad[i].fRange));
								}
							}
							for(int j = 0; j < (int)gSelectedAvgPointDataNum[i]; j++)
							{
								for(int k = 0; k < AVG_CURVE_NUM + 1; k++)
								{
									gSelectedAvgPointData[i][k][j].serial = (float)(gwOldAvgSerial[i][j] 
																								* (gfOldAvgRange[i] / Display_AllParamConfigWith_ad[i].fRange));
								}
							}
						}
						
						if(fAllCfgOldPlus[i] - Receive_AllParamConfigWith_ad[i].fPlus != 0)
						{
							for(int j = 0; j < (int)gSelectedDgsPointDataNum[i][1]; j++)
							{
								for(int k = 0; k < DAC_CURVE_NUM; k++)
								{
									gSelectedDgsPointData[i][k][j].data = curveDataChangeByGain(gSelectedDgsPointData[i][k][j].data, (Receive_AllParamConfigWith_ad[i].fPlus - fAllCfgOldPlus[i]));
								}
							}
							for(int j = 0; j < (int)gSelectedAvgPointDataNum[i]; j++)
							{
								for(int k = 0; k < AVG_CURVE_NUM + 1; k++)
								{
									gSelectedAvgPointData[i][k][j].data = curveDataChangeByGain(gSelectedAvgPointData[i][k][j].data, (Receive_AllParamConfigWith_ad[i].fPlus - fAllCfgOldPlus[i]));
								}
							}
						}
					}				
					allCfgSaveParamFile(ParamFileName);

				/*	if(gwGongYiCanShu == currentUsedParamNo)
					{
						ReadParamFile(ParamFileName);
						// �Ѳ�������ȥ
						InitParam();
					}
					*/						
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					ShowWindow(hStatusBar,SW_SHOW);

					DestroyWindow(hWndAllParamCfg);
					hWndAllParamCfg = 0;
					DestroyWindow(hWndAllParamCfgCaption);
					hWndAllParamCfgCaption = 0;
					DestroyWindow(hWndAllParamConfirm);
					hWndAllParamConfirm = 0;

					if(gbHotCraftParamView)
					{
						gbHotCraftParamView = FALSE;

						if(gbHotGateFlag)
						{
							ShowWindow(hWndGate,SW_SHOW);
							SetFocus(currentGateBtn);
						}
						else if(gbHotAutoGainFlag)
						{
							ShowWindow(hWndAutoGainPlus,SW_SHOW);
							SetFocus(currentAutoGainPlusBtn);
						}
						else if(gbHotAutoAdjustFlag)
						{
							ShowWindow(hWndAdjust,SW_SHOW);
							SetFocus(currentAdjustBtn);
						}
						else
						{
							if(gCurrentWnd)//���Ӳ˵�����
							{
								ShowWindow(gCurrentWnd, SW_SHOW);
								SetFocus(gCurrentFocusBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
							}
							else
							{
								SetFocus(hWndMain);//�����ڻ�����뽹��
							}
						}
					}
					else
					{
						ShowWindow(hWndCraftParam,SW_SHOW);
						SetFocus(currentCraftParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					}

					SetRedrawFlag(TRUE);
					
					Sleep(30);
					SetRedrawFlag(TRUE);//�ٶ�ˢ�´Σ����п����Ȼ��Դ�,ϵͳ��ˢ��ʾ�����°���

					//wangbingfu add start at 2011-5-25
				/*	//wangbingfu delete at 2011-5-27
					if(gwDeviceType == 2 && gbTlParamFlag == TRUE)
					{
						setRailRoadParam(CurrentChannelNo, FALSE);//���øñ�׼�ò�λ���̣���׼���棬��������
					}*/
					//wangbingfu add end at 2011-5-25
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamConfirmsaveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�����水ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAllParamConfirmNoSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}
			
				else if(wParam == VK_LEFT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RIGHT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RETURN)
				{				
					ShowWindow(hStatusBar,SW_SHOW);

					DestroyWindow(hWndAllParamCfg);
					hWndAllParamCfg = 0;
					DestroyWindow(hWndAllParamCfgCaption);
					hWndAllParamCfgCaption = 0;
					DestroyWindow(hWndAllParamConfirm);
					hWndAllParamConfirm = 0;

					if(gbHotCraftParamView)
					{
						gbHotCraftParamView = FALSE;

						if(gbHotGateFlag)
						{
							ShowWindow(hWndGate,SW_SHOW);
							SetFocus(currentGateBtn);
						}
						else if(gbHotAutoGainFlag)
						{
							ShowWindow(hWndAutoGainPlus,SW_SHOW);
							SetFocus(currentAutoGainPlusBtn);
						}
						else if(gbHotAutoAdjustFlag)
						{
							ShowWindow(hWndAdjust,SW_SHOW);
							SetFocus(currentAdjustBtn);
						}
						else
						{
							if(gCurrentWnd)//���Ӳ˵�����
							{
								ShowWindow(gCurrentWnd, SW_SHOW);
								SetFocus(gCurrentFocusBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
							}
							else
							{
								SetFocus(hWndMain);//�����ڻ�����뽹��
							}
						}
					}
					else
					{
						ShowWindow(hWndCraftParam,SW_SHOW);
						SetFocus(currentCraftParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					}

					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetRedrawFlag(TRUE);

					Sleep(50);
					SetRedrawFlag(TRUE);//�ٶ�ˢ�´Σ����п����Ȼ��Դ�,ϵͳ��ˢ��ʾ�����°���
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamConfirmNoSaveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ȡ����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAllParamConfirmCancelProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}
			
				else if(wParam == VK_LEFT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RIGHT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RETURN)
				{
					DestroyWindow(hWndAllParamConfirm);
					hWndAllParamConfirm = 0;
					SetFocus(currentAllParamCfgBtn);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamConfirmCancelProc,Wnd,Msg,wParam,lParam);
}
