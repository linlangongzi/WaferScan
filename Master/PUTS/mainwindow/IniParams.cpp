// IniParams.cpp: implementation of the CIniParams class.
//
//////////////////////////////////////////////////////////////////////
#include <QSettings>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include "DataDefine.h"
#include "IniParams.h"
#include "elepopular.h"
#include "mainwindow.h"

//#include "GlobalFunction.h"



//extern unsigned long m_ElepHandle;
const WORD VERSION = 100;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniParams::CIniParams()
{
	m_pMainWindow = NULL;
	m_nCoder1PerRingNum = 0;
	m_fCoder1Diameter = 0.0f;
	m_nCoder2PerRingNum = 0;
	m_fCoder2Diameter = 0.0f;
	m_nCoder3PerRingNum = 0;
	m_fCoder3Diameter = 0.0f;
	m_nCoder4PerRingNum = 0;
	m_fCoder4Diameter = 0.0f;
	m_nCoder5PerRingNum = 0;
	m_fCoder5Diameter = 0.0f;
	m_fNeitan1_RefEdge_Distance = 0.0f;
	m_fNeitan2_RefEdge_Distance = 0.0f;
	m_fCar1_RefEdge_Distance = 0.0f;
	m_fCar2_FreeEdge_Distance = 0.0f;
	m_fNeitan1_Coder1_Distance = 0.0f;
	m_fNeitan2_Coder1_Distance = 0.0f;
	m_fNeitan3_Coder1_Distance = 0.0f;
	m_fNeitan4_Coder1_Distance = 0.0f;
	m_fCar1_Coder1_Distance = 0.0f;
	m_fCar2_Coder1_Distance = 0.0f;
	m_fDisplayWndLength = 0.0f;
	m_fDisplayWndWidth = 0.0f;
	
	// YiQi Params
	m_nLSMP2_OR_DPC31 = 0;
	m_fGAIN_MAX = 0.0f;
	m_fAEC_AMP_LINEAR_PARA_A = 0.0f;
	m_fAEC_AMP_LINEAR_PARA_B = 0.0f;

	m_minXDistance = 0.0f;
	m_maxXDistance = 0.0f;
	m_minYDistance = 0.0f;
	m_maxYDistance = 0.0f;
	m_zDistance = 3;
	m_fDistanceInBox = 0.0f;
	m_fTouTan_RefEdge_Distance = 0.0f;
	m_fWeiTan_FreeEdge_Distance = 0.0f;

	m_IsDiBoHide = 0;
	m_IsWizard = 0;
	m_fminZ = 0;
	m_nDACOpen = 0;
	m_nDisplayShiBo = 0;
	m_nPlateStateSingle = 0;
	m_nDisplaySaveButton = 0;

	m_fFrontMinZ = 0.0f;
	m_fFrontLength = 0.0f;
	m_fBackMinZ = 0.0f;
	m_fBackLength = 0.0f;
	m_fEdgeMinZ = 0.0f;
	m_fEdgeLength = 0.0f;
	m_IsDisplayRegionSumArea = 0;
	m_IsDisplayLog = 0;
	m_IsDisplayLiuLanAndSetOneChannel = 0;
	m_IsThickWindowShow = 0;
//	m_IsDisplayZhaMenAndDelay = 0;
	m_IsDisplayDelay = 0;
	m_IsDisplayZhaMenFront = 0;
	m_IsDisplayZhaMenBack = 0;
	m_IsDisplayDiBoZhaMenBack = 0;
	m_IsDisplayZhaMenLine = 0;
	m_fInitShangMangQu = 0.0f;	
	m_fInitXiaMangQu = 0.0f;
	m_fMaxDiShiPlus = 0.0f;

	m_bBiaoDingEn = FALSE;	
	m_bNewGuoLvEn = FALSE;
	m_bReadPlateLenthEn = FALSE;

	m_fOKDelayBottomLine = 0.0f;	// 合理的延迟下限
	m_fOKDelayUpLine = 0.0f;		// 合理的延迟上限
	m_fZengYiBottomLine = 0.0f;	// 合理的增益下限
	m_fZengYiUpLine = 0.0f;		// 合理的增益上限

	m_fNeiFrontLength = 0.0f;	
 	m_fNeiRefEdgeLength = 0.0f;	
 	m_fNeiTailLength = 0.0f;		
 	m_fNeiUnRefEdgeLength = 0.0f;

	m_DelKuoXMin = 0.0f;
	m_DelKuoXMax = 0.0f;
	m_DelKuoYMin = 0.0f;
	m_DelKuoYMax = 0.0f;

	m_nVersion = VERSION;
	m_nAuthorization = 0;
	m_fLianMaxDistance = 3;		// 失波连接最大间距
	m_fZhongduanMinDistance = 50; // 失波连接最大间距
	m_fDiboGuolv = 10.0f;

	m_nSoftDAC = 128;
	m_bBaoLiu = TRUE;
	m_fYuZhi = 25.0f;
}

CIniParams::~CIniParams()
{

}

BOOL CIniParams::ReadIniData()
{
	QSettings *pMachineSettings = NULL;
	QString  strMachineFile = QDir::currentPath() += "/MachineParams.ini";
	pMachineSettings = new QSettings(strMachineFile, QSettings::IniFormat); 

	//QSettings *pYiQiSettings = NULL;
	//QString strYiQiFile = QDir::currentPath() + "/InstrumentParams.ini";
	//pYiQiSettings = new QSettings(strYiQiFile, QSettings::IniFormat); 

	// 读MachineParams.ini
	m_nCoder1PerRingNum = pMachineSettings->value("Coder1PerRingNum","").toInt();
	m_fCoder1Diameter = pMachineSettings->value("Coder1Diameter","").toFloat();
	m_nCoder2PerRingNum = pMachineSettings->value("Coder2PerRingNum","").toInt();
	m_fCoder2Diameter = pMachineSettings->value("Coder2Diameter","").toFloat();
	m_nCoder3PerRingNum = pMachineSettings->value("Coder3PerRingNum","").toInt();
	m_fCoder3Diameter = pMachineSettings->value("Coder3Diameter","").toFloat();
	m_nCoder4PerRingNum = pMachineSettings->value("Coder4PerRingNum","").toInt();
	m_fCoder4Diameter = pMachineSettings->value("Coder4Diameter","").toFloat();
	m_nCoder5PerRingNum = pMachineSettings->value("Coder5PerRingNum","").toInt();
	m_fCoder5Diameter = pMachineSettings->value("Coder5Diameter","").toFloat();
	m_fNeitan1_RefEdge_Distance = pMachineSettings->value("Neitan1_RefEdge_Distance","").toFloat();
	m_fNeitan2_RefEdge_Distance = pMachineSettings->value("Neitan2_RefEdge_Distance","").toFloat();
	m_fCar1_RefEdge_Distance = pMachineSettings->value("Car1_RefEdge_Distance","").toFloat();
	m_fCar2_FreeEdge_Distance = pMachineSettings->value("Car2_FreeEdge_Distance","").toFloat();
	m_fNeitan1_Coder1_Distance = pMachineSettings->value("Neitan1_Coder1_Distance","").toFloat();
	m_fNeitan2_Coder1_Distance = pMachineSettings->value("Neitan2_Coder1_Distance","").toFloat();
	m_fNeitan3_Coder1_Distance = pMachineSettings->value("Neitan3_Coder1_Distance","").toFloat();
	m_fNeitan4_Coder1_Distance = pMachineSettings->value("Neitan4_Coder1_Distance","").toFloat();
	m_fCar1_Coder1_Distance = pMachineSettings->value("Car1_Coder1_Distance","").toFloat();
	m_fCar2_Coder1_Distance = pMachineSettings->value("Car2_Coder1_Distance","").toFloat();		
	m_fDisplayWndLength = pMachineSettings->value("PlateWndDisplayLength","").toFloat();
	m_fDisplayWndWidth = pMachineSettings->value("PlateWndDisplayWidth","").toFloat();	
	m_fDistanceInBox = pMachineSettings->value("DistanceInBox","").toFloat();
	m_fTouTan_RefEdge_Distance = pMachineSettings->value("TouTan_RefEdge_Distance","").toFloat();	
	m_fWeiTan_FreeEdge_Distance = pMachineSettings->value("WeiTan_FreeEdge_Distance","").toFloat();
	//如果读取失败，提示
	if (0 == m_nCoder1PerRingNum)
	{
		MessageBox(NULL,L"读取MachineParams.ini文件失败，查看文件后重新运行程序",L"CIniParams::ReadIniData",MB_OK);
	}

#if HAVE_DOG
	unsigned short ReadDatOffset=0x480;	//0x80为可读写数据区
	unsigned short ReadDatLength=sizeof(DogInfo);
	DogInfo info2;
	::ZeroMemory(&info2, sizeof(DogInfo));

	// 读取unsigned long 数据
	unsigned long RetVal = ElepReadMemory(m_pMainWindow->m_ElepHandle, ReadDatOffset, (unsigned char*)&info2, ReadDatLength);
	if(RetVal == ELEP_SUCCESS)
	{
		// 比较版本号
		//float fVersionZongZhan = m_nVersion / 100.0f;
		//float fVersionDog = info2.m_nVersion / 100.0f;

		m_nLSMP2_OR_DPC31 = info2.m_nLSMP2_OR_DPC31;
		m_fGAIN_MAX = info2.m_fGAIN_MAX;
		m_fAEC_AMP_LINEAR_PARA_A = info2.m_fAEC_AMP_LINEAR_PARA_A;
		m_fAEC_AMP_LINEAR_PARA_B = info2.m_fAEC_AMP_LINEAR_PARA_B;

		//调试修改 chenyuzhu start on 2014-10-9; 
		m_minXDistance = info2.m_minXDistance;
		m_maxXDistance = info2.m_maxXDistance;
		m_minYDistance = info2.m_minYDistance;
		m_maxYDistance = info2.m_maxYDistance;
		

		/////chenyuzhu add start on 2014-5-22 调试用
		//m_minXDistance = static_cast<double>(5);
		//m_maxXDistance = static_cast<double>(5);
		//m_minYDistance = static_cast<double>(20);
		//m_maxYDistance = static_cast<double>(20);
		////chenyuzhu add end on 2014-5-22 
		//调试修改 chenyuzhu end on 2014-10-9; 
		m_zDistance = info2.m_zDistance;
		m_IsDiBoHide = info2.m_IsDiBoHide;
		m_IsWizard = info2.m_IsWizard;
		m_fminZ = info2.m_fminZ;			// 主体Z方向过滤
		m_nDACOpen = info2.m_nDACOpen;
		m_nDisplayShiBo = info2.m_nDisplayShiBo;
		m_nPlateStateSingle = info2.m_nPlateStateSingle;
		m_nDisplaySaveButton = info2.m_nDisplaySaveButton;
		m_fFrontMinZ = info2.m_fFrontMinZ;	// 头部Z方向过滤
		m_fFrontLength = info2.m_fFrontLength;
		m_fBackMinZ = info2.m_fBackMinZ;	// 尾部Z方向过滤
		m_fBackLength = info2.m_fBackLength;
		m_fEdgeMinZ = info2.m_fEdgeMinZ;	// 边缘Z方向过滤
		m_fEdgeLength = info2.m_fEdgeLength;
		m_IsDisplayRegionSumArea = info2.m_IsDisplayRegionSumArea;
		m_IsDisplayLog = info2.m_IsDisplayLog;
		m_IsDisplayLiuLanAndSetOneChannel = info2.m_IsDisplayLiuLanAndSetOneChannel;
		m_IsThickWindowShow = info2.m_IsThickWindowShow;
		//		m_IsDisplayZhaMenAndDelay = info2.m_IsDisplayZhaMenAndDelay;
		m_IsDisplayDelay = info2.m_IsDisplayDelay;
		m_IsDisplayZhaMenFront = info2.m_IsDisplayZhaMenFront;
		m_IsDisplayZhaMenBack = info2.m_IsDisplayZhaMenBack;
		m_IsDisplayDiBoZhaMenBack = info2.m_IsDisplayDiBoZhaMenBack;
		m_IsDisplayZhaMenLine = info2.m_IsDisplayZhaMenLine;
		m_fInitShangMangQu = info2.m_fInitShangMangQu;	
		m_fInitXiaMangQu = info2.m_fInitXiaMangQu;
		m_fMaxDiShiPlus = info2.m_fMaxDiShiPlus;
		m_fDiShiJudgeValue = info2.m_fDiShiJudgeValue;
		m_bBiaoDingEn = info2.m_bBiaoDingEn;	
		m_bNewGuoLvEn = info2.m_bNewGuoLvEn;
		m_bReadPlateLenthEn = info2.m_bReadPlateLenthEn;
		m_fBiaoDingZengYiSubtract = info2.m_fBiaoDingZengYiSubtract;

		m_fOKDelayBottomLine = info2.m_fOKDelayBottomLine;		// 合理的延迟下限
		m_fOKDelayUpLine = info2.m_fOKDelayUpLine;				// 合理的延迟上限
		m_fZengYiBottomLine = info2.m_fZengYiBottomLine;		// 合理的增益下限
		m_fZengYiUpLine = info2.m_fZengYiUpLine;				// 合理的增益上限

		m_fNeiFrontLength = info2.m_fNeiFrontLength;			// 内探检测范围和板头的距离
		m_fNeiRefEdgeLength = info2.m_fNeiRefEdgeLength;		// 内探检测范围和参考边的距离
		m_fNeiTailLength = info2.m_fNeiTailLength;				// 内探检测范围和板尾的距离
		m_fNeiUnRefEdgeLength = info2.m_fNeiUnRefEdgeLength;	// 内探检测范围和非参考边的距离

		//调试先注释掉加密狗无数据
		m_DelKuoXMin = info2.m_DelKuoXMin;
		m_DelKuoXMax = info2.m_DelKuoXMax;
		m_DelKuoYMin = info2.m_DelKuoYMin;
		m_DelKuoYMax = info2.m_DelKuoYMax;

		//wangwei add start on 2016-09-01调试伤点删除功能
// 		m_DelKuoXMin = 0; // 加密狗中设置的删除伤点的时候的区域放大系数
// 		m_DelKuoXMax = 0;
// 		m_DelKuoYMin = 0;
// 		m_DelKuoYMax = 0;
		//wangwei add end on 2016-09-01 调试伤点删除功能

		m_bDelShang = info2.m_bDelShang;
		m_nAuthorization = info2.m_nAuthorization;
		m_fLianMaxDistance = info2.m_fLianMaxDistance;		// 失波连接最大间距
		m_fZhongduanMinDistance = info2.m_fZhongduanMinDistance; // 失波连接最大间距
		m_fDiboGuolv = info2.m_fDiboGuolv;			// 失波（假失波）过滤阈值

		// 提取加密狗里扫查间隔
		//		Info2Text(info2);
		m_nSoftDAC = info2.m_nSoftDAC;
		m_bBaoLiu = info2.m_bBaoLiu;
		m_fYuZhi = info2.m_fYuZhi;
		m_fRefDelayTime = info2.m_fRefDelayTime;
		m_fRefDiboZengyi = info2.m_fRefDiboZengyi;
		m_fRefLingmindu = info2.m_fRefLingmindu;
		m_fDeltaDelayTime = info2.m_fDeltaDelayTime;
		m_fDeltaDiboZengyi = info2.m_fDeltaDiboZengyi;
		m_fDeltaLingmindu = info2.m_fDeltaLingmindu;

		return TRUE;
	}
	else
	{
		QMessageBox::information(NULL, "Error", "读取加密狗数据失败!", QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}
#else 
	return TRUE;
#endif
}