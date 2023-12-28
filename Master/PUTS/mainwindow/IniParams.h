// IniParams.h: interface for the CIniParams class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _INIPARAMS_H_
#define _INIPARAMS_H_

#include <windows.h>
#include <QObject>

class MainWindow;
//#if _MSC_VER > 1000
//#pragma once
//#endif

struct DogInfo
{
	// YiQi Params
	WORD m_nLSMP2_OR_DPC31;
	float m_fGAIN_MAX;
	float m_fAEC_AMP_LINEAR_PARA_A;
	float m_fAEC_AMP_LINEAR_PARA_B;
	float m_minXDistance;
	float m_maxXDistance;
	float m_minYDistance;
	float m_maxYDistance;
	float m_zDistance;
	int m_IsDiBoHide;
	int m_IsWizard;
	float m_fminZ;
	WORD m_nDACOpen;
	WORD m_nDisplayShiBo;
	WORD m_nPlateStateSingle;
	WORD m_nDisplaySaveButton;
	float m_fFrontMinZ;
	float m_fFrontLength;
	float m_fBackMinZ;
	float m_fBackLength;
	float m_fEdgeMinZ;
	float m_fEdgeLength;
	WORD m_IsDisplayRegionSumArea;
	WORD m_IsDisplayLog;
	WORD m_IsDisplayLiuLanAndSetOneChannel;
	WORD m_IsThickWindowShow;
//	WORD m_IsDisplayZhaMenAndDelay;
	// Song Chenguang add start on 2009-12-07
	// m_IsDisplayZhaMenAndDelay分解为5个
	WORD m_IsDisplayDelay;
	WORD m_IsDisplayZhaMenFront;
	WORD m_IsDisplayZhaMenBack;
	WORD m_IsDisplayDiBoZhaMenBack;
	WORD m_IsDisplayZhaMenLine;
	float m_fInitShangMangQu;	// 初始上盲区
	float m_fInitXiaMangQu;		// 初始下盲区
	float m_fMaxDiShiPlus;		// 最大底时增量
	float m_fDiShiJudgeValue;	// 采用底时过滤所需值
	BOOL m_bBiaoDingEn;			// 标定使能
	BOOL m_bNewGuoLvEn;			// 新过滤功能使能
	BOOL m_bReadPlateLenthEn;	// 读板长使能
	float m_fBiaoDingZengYiSubtract;	// 标定后增益调整的值（要减掉的）
	float m_fOKDelayBottomLine;	// 合理的延迟下限
	float m_fOKDelayUpLine;		// 合理的延迟上限
	float m_fZengYiBottomLine;	// 合理的增益下限
	float m_fZengYiUpLine;		// 合理的增益上限

 	float m_fNeiFrontLength;	// 内探检测范围和板头的距离
 	float m_fNeiRefEdgeLength;	// 内探检测范围和参考边的距离
 	float m_fNeiTailLength;		// 内探检测范围和板尾的距离
 	float m_fNeiUnRefEdgeLength;	// 内探检测范围和非参考边的距离

	float m_DelKuoXMin;			// 删除时minX扩边值
	float m_DelKuoXMax;			// 删除时maxX扩边值
	float m_DelKuoYMin;			// 删除时minY扩边值
	float m_DelKuoYMax;			// 删除时maxY扩边值

	BOOL m_bDelShang;
	WORD m_nVersion;

	int		m_nAuthorization;
	float   m_fLianMaxDistance;		// 失波连接最大间距
	float   m_fZhongduanMinDistance; // 失波连接最大间距
	float   m_fDiboGuolv;			// 失波（假失波）过滤阈值
	int m_nSoftDAC;
	BOOL m_bBaoLiu;
	float m_fYuZhi;
	float	m_fRefDelayTime;
	float	m_fRefDiboZengyi;
	float	m_fRefLingmindu;
	float	m_fDeltaDelayTime;
	float	m_fDeltaDiboZengyi;
	float	m_fDeltaLingmindu;
};

//class CLogOnDialog;
//class CPlateInfo;
//class CCscanPanel;
//class CChannelPanel;
//class CPlateInfoPanel;
//class CInspectUser;
//class CToolsDialog;
//class CPlateParaSetERPDialog;
//class CPlateParaSetDialog;
//class CTechnologySetDialog;
//class MainWindow;

class CIniParams : public QObject
{
	  Q_OBJECT
public:
	CIniParams();
	virtual ~CIniParams();
	BOOL ReadIniData();
public:
	MainWindow* m_pMainWindow;
	// machine params
	WORD m_nCoder1PerRingNum;
	float m_fCoder1Diameter;
	WORD m_nCoder2PerRingNum;
	float m_fCoder2Diameter;
	WORD m_nCoder3PerRingNum;
	float m_fCoder3Diameter;
	WORD m_nCoder4PerRingNum;
	float m_fCoder4Diameter;
	WORD m_nCoder5PerRingNum;
	float m_fCoder5Diameter;
//	float m_fNeitan12_RefEdge_Distance;
//	float m_fNeitan34_FreeEdge_Distance;
	float m_fNeitan1_RefEdge_Distance;
	float m_fNeitan2_RefEdge_Distance;
	float m_fCar1_RefEdge_Distance;
	float m_fCar2_FreeEdge_Distance;
	float m_fNeitan1_Coder1_Distance;
	float m_fNeitan2_Coder1_Distance;
	float m_fNeitan3_Coder1_Distance;
	float m_fNeitan4_Coder1_Distance;
	float m_fCar1_Coder1_Distance;
	float m_fCar2_Coder1_Distance;
	float m_fDisplayWndLength;
	float m_fDisplayWndWidth;
	float m_fDistanceInBox;
	float m_fTouTan_RefEdge_Distance;
	float m_fWeiTan_FreeEdge_Distance;

	// YiQi Params
	WORD m_nLSMP2_OR_DPC31;
	float m_fGAIN_MAX;
	float m_fAEC_AMP_LINEAR_PARA_A;
	float m_fAEC_AMP_LINEAR_PARA_B;

	float m_minXDistance;
	float m_maxXDistance;
	float m_minYDistance;
	float m_maxYDistance;
	float m_zDistance;
	int m_IsDiBoHide;
	int m_IsWizard;
	float m_fminZ;
	float m_fminZEnd;
	WORD m_nDACOpen;
	WORD m_nDisplayShiBo;
	WORD m_nPlateStateSingle;
	WORD m_nDisplaySaveButton;
	float m_fFrontMinZ;
	float m_fFrontMinZEnd;
	float m_fFrontLength;
	float m_fBackMinZ;
	float m_fBackMinZEnd;
	float m_fBackLength;
	float m_fEdgeMinZ;
	float m_fEdgeMinZEnd;
	float m_fEdgeLength;
	WORD m_IsDisplayRegionSumArea;
	WORD m_IsDisplayLog;
	WORD m_IsDisplayLiuLanAndSetOneChannel;
	WORD m_IsThickWindowShow;
//	WORD m_IsDisplayZhaMenAndDelay;
	// m_IsDisplayZhaMenAndDelay分解为5个
	WORD m_IsDisplayDelay;
	WORD m_IsDisplayZhaMenFront;
	WORD m_IsDisplayZhaMenBack;
	WORD m_IsDisplayDiBoZhaMenBack;
	WORD m_IsDisplayZhaMenLine;
	float m_fInitShangMangQu;	// 初始上盲区
	float m_fInitXiaMangQu;		// 初始下盲区
	float m_fMaxDiShiPlus;		// 最大底时增量
	float m_fDiShiJudgeValue;	// 采用底时过滤所需值
	BOOL m_bBiaoDingEn;			// 标定使能
	BOOL m_bNewGuoLvEn;			// 新过滤功能使能
	BOOL m_bReadPlateLenthEn;	// 读板长使能
	float m_fBiaoDingZengYiSubtract;	// 标定后增益调整的值（要减掉的）
	float m_fOKDelayBottomLine;	// 合理的延迟下限
	float m_fOKDelayUpLine;		// 合理的延迟上限
	float m_fZengYiBottomLine;	// 合理的增益下限
	float m_fZengYiUpLine;		// 合理的增益上限

 	float m_fNeiFrontLength;	// 内探检测范围和板头的距离
 	float m_fNeiRefEdgeLength;	// 内探检测范围和参考边的距离
 	float m_fNeiTailLength;		// 内探检测范围和板尾的距离
 	float m_fNeiUnRefEdgeLength;	// 内探检测范围和非参考边的距离

	float m_DelKuoXMin;			// 删除时minX扩边值
	float m_DelKuoXMax;			// 删除时maxX扩边值
	float m_DelKuoYMin;			// 删除时minY扩边值
	float m_DelKuoYMax;			// 删除时maxY扩边值

	BOOL m_bDelShang;

	WORD m_nVersion;
	
	int		m_nAuthorization;
	float   m_fLianMaxDistance;		// 失波连接最大间距
	float   m_fZhongduanMinDistance; // 失波连接最大间距
	float   m_fDiboGuolv;			// 失波（假失波）过滤阈值
	int m_nSoftDAC;
	BOOL m_bBaoLiu;
	int m_fYuZhi;
	float	m_fRefDelayTime;
	float	m_fRefDiboZengyi;
	float	m_fRefLingmindu;
	float	m_fDeltaDelayTime;
	float	m_fDeltaDiboZengyi;
	float	m_fDeltaLingmindu;

};

#endif // _INIPARAMS_H_
