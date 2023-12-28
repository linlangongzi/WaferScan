/**
* Copyright(c) 2014 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 
* Filename: BiaoZhun_ASTM435_435M.cpp
*
* @author        Chen Yuzhu
* @version       1.00         Date: 2014-08-05
*/

#include <QFile>
#include <QAxWidget>
#include <QString>
#include <QPainter>
#include <QByteArray>
#include "const.h"
#include "DataDefine.h"
#include "mainwindow.h"
#include "BiaoZhun.h"
#include "BiaoZhun_ASTM435_435M.h"


extern QAxWidget* g_pPlateDraw;
extern PLATE_INFO g_Info[1];
extern int nPlate;

extern char *g_pBuf4;//用于存储点伤数据的内存区域
extern unsigned int g_nBuf4Size;//点伤大小

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiaoZhun_ASTM435_435M::CBiaoZhun_ASTM435_435M()
{
	SetParameter();
	m_dBodyMaxFlawZhiShiLength = 0;
	m_dEdgeMaxFlawZhiShiLength = 0;
}

CBiaoZhun_ASTM435_435M::~CBiaoZhun_ASTM435_435M()
{

}

void CBiaoZhun_ASTM435_435M::DrawScanResult(QPainter* painter,const QRect& rect)
{
	QPen pen(QColor(0,0,0),1,Qt::SolidLine);
	QBrush brush(QColor(255,255,255));
	painter->setPen(pen);
	painter->setBrush(brush);
	// 画框
	QRect ResultRect = rect;
	int nPerX = rect.width();
	int nperY = rect.height()/4;

	painter->drawRect(ResultRect);

	// 画表格
	QPoint ptStart, ptEnd;
	for (int i =1; i<4; i++)
	{
		ptStart.setX(ResultRect.left());
		ptStart.setY(ResultRect.top() + i*nperY);
		ptEnd.setX(ResultRect.right());
		ptEnd.setY(ptStart.y());

		painter->drawLine(ptStart,ptEnd);
	}

	// 写标题
//	pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);

	// 行1
	painter->drawText(ResultRect.center().x(), ResultRect.top()+nperY, QObject::tr("探伤结果"));
	// 行3
	QString str = QObject::tr("面伤最大直径");
	QPoint ptText;

	ptText.setX(ResultRect.left() + 0.5* nPerX);
	ptText.setY(ResultRect.center().y() + nperY);

	painter->drawText(ptText.x(), ptText.y(), str);

	// 从OCX文件里获取数据填写结果

	// 从控件里得最大的伤的直径
	if (m_bReCalc)
	{
		GetResultValues();
	}
	double dBodyMaxFlawZhiShiLength = m_dBodyMaxFlawZhiShiLength;  
	double dEdgeMaxFlawZhiShiLength = m_dEdgeMaxFlawZhiShiLength; 
	double dMaxDiameter = dBodyMaxFlawZhiShiLength > dEdgeMaxFlawZhiShiLength ? dBodyMaxFlawZhiShiLength : dEdgeMaxFlawZhiShiLength;
	//	double dMaxDiameter = g_pPlateDraw->GetMaxDuijiaoLength();

//	str.Format(_T("%.2f  mm"), dMaxDiameter);
	str = QString("%1").arg(dMaxDiameter, 0, 'f', 2);
	ptText.setX(ResultRect.left() + 0.5 * nPerX);
	ptText.setY(ResultRect.bottom());
	//判断那个标准参数超标，置颜色
	if(dMaxDiameter > 75.0f)
	{	
		painter->setPen(QColor(255, 0, 0));
	}
	else
	{
		painter->setPen(QColor(0, 0, 0));
	}
	painter->drawText(ptText.x(), ptText.y(), str);

	// 行2（钢板的实际级别和检测标志比较，写合格或不合格，
	// 合格用绿色，不合格用红色，而且不合格的数值也要用红色标出来）
	/*	if (dMaxDiameter < 75.0f && dMaxDiameter < (g_Info[nPlate].height/2))
	{
	str = _T("合格");
	}
	else
	str = _T("不合格");
	*/
	/*str = GetResult();

	if (str == _T("合格"))
	{
	pDC->SetTextColor(RGB(0,255,0));
	}
	else
	pDC->SetTextColor(RGB(255,0,0));

	pDC->TextOut(ResultRect.CenterPoint().x, ResultRect.CenterPoint().y, str);*/

	m_rectResultPos.setLeft(ResultRect.left());
	m_rectResultPos.setRight(ResultRect.right());
	m_rectResultPos.setTop(ResultRect.top() + nperY);
	m_rectResultPos.setBottom(ResultRect.bottom() - nperY*2);


	m_bReCalc = FALSE; 

}

void CBiaoZhun_ASTM435_435M::SetParameter()
{
	
	// *设置指示长度类型
	g_pPlateDraw->dynamicCall("SetZhiShiLengthType(short)",ZHISHI_LENGTH_DIAGONAL);

	g_pPlateDraw->dynamicCall("SetShangWidthType(short)",SHANG_WIDTH_Y);
	g_pPlateDraw->dynamicCall("SetAreaType(short)",AREA_REAL);
	g_pPlateDraw->dynamicCall("SetMergeShangType(short,double)",SHANG_MERGE_MAX_DISANCE,1.0f);
	g_pPlateDraw->dynamicCall("SetEdgeWidth(double)",0);
	//	g_pPlateDraw->SetMaxZDistance(double(nMaxZDistance));
	g_pPlateDraw->dynamicCall("SetNotUseBodyMinArea(double)",0);
	//	g_pPlateDraw->SetNotUseBodyMaxArea((double)nNotUseBodyMaxArea);
	g_pPlateDraw->dynamicCall("SetNotUseBodyMinShangZhiShiLength(double)",0);
	//	g_pPlateDraw->SetNotUseBodyMaxShangZhiShiLength((double)nNotUseBodyMaxShangZhiShiLength);
	g_pPlateDraw->dynamicCall("SetNotUseBodyMinShangLength(double)",0);
	//	g_pPlateDraw->SetNotUseBodyMaxShangLength((double)nNotUseBodyMaxShangLength);
	g_pPlateDraw->dynamicCall("SetNotUseBodyMinShangWidth(double)",0);
	//	g_pPlateDraw->SetNotUseBodyMaxShangWidth((double)nNotUseBodyMaxShangWidth);
	g_pPlateDraw->dynamicCall("SetNotUseEdgeMinArea(double)",0);
	//	g_pPlateDraw->SetNotUseEdgeMaxArea((double)nNotUseEdgeMaxArea);
	g_pPlateDraw->dynamicCall("SetNotUseEdgeMinShangZhiShiLength(double)",0);
	//	g_pPlateDraw->SetNotUseEdgeMaxShangZhiShiLength((double)nNotUseEdgeMaxShangZhiShiLength);
	g_pPlateDraw->dynamicCall("SetNotUseEdgeMinShangLength(double)",0);
	//	g_pPlateDraw->SetNotUseEdgeMaxShangLength((double)nNotUseEdgeMaxShangLength);
	g_pPlateDraw->dynamicCall("SetNotUseEdgeMinShangWidth(double)",0);
	//	g_pPlateDraw->SetNotUseEdgeMaxShangWidth((double)nNotUseEdgeMaxShangWidth);

	// *设置指示长度类型
	//	g_pPlateDraw->SetZhiShiLengthType(ZHISHI_LENGTH_DIAGONAL);

	//? *设置伤宽度类型
	// 1 = 以伤X方向值为伤宽度
	// 2 = 以伤Y方向值为伤宽度
	// 3 = x和y之间的较小值
	//	void SetShangWidthType(short ShangWidthType)

	//? *设置指示面积类型
	// g_pPlateDraw->SetAreaType(AREA_REAL);

	// *设置相邻伤合并时，判断距离的类型
	//	g_pPlateDraw->SetMergeShangType(SHANG_MERGE_MAX_DISANCE, 1.0);
}

QString CBiaoZhun_ASTM435_435M::GetResult()
{
	QString strResult;
	// 从控件里得最大的伤的直径
	//yachang.huang modify start on 2013-01-31
	if (m_bReCalc)
	{
		GetResultValues();
	}
	double dBodyMaxFlawZhiShiLength = m_dBodyMaxFlawZhiShiLength;  //yachang.huang modify on 2012-12-24
	double dEdgeMaxFlawZhiShiLength = m_dEdgeMaxFlawZhiShiLength;  //yachang.huang modify on 2012-12-24
	//yachang.huang modify end on 2013-01-31
	double dMaxDiameter = dBodyMaxFlawZhiShiLength > dEdgeMaxFlawZhiShiLength ? dBodyMaxFlawZhiShiLength : dEdgeMaxFlawZhiShiLength;
	//	double dMaxDiameter = g_pPlateDraw->GetMaxDuijiaoLength();
	m_fBiaoZhunZhiShiLength = 75; //yachang.huang add on 2013-02-18
	if (dMaxDiameter < 75.0f || dMaxDiameter < (g_Info[nPlate].height/2))
	{
		strResult = QObject::tr("合格");
	}
	else
		strResult = QObject::tr("不合格");

	return strResult;
}

void CBiaoZhun_ASTM435_435M::CalcZhishiArea(double& dbZhiShiArea, ZhiShiAreaRegion& ZhiShiRegion)
{
	/*
	double dMaxBodyArea =  g_pPlateDraw->GetBodyMaxQueXianArea();
	double dMaxEdgeArea =  g_pPlateDraw->GetEdgeMaxQueXianArea();

	dbZhiShiArea = dMaxBodyArea > dMaxEdgeArea ? dMaxBodyArea : dMaxEdgeArea;
	*/
	// 从buf4里查询最大指示面积
//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CZongZhanView* pView = (CZongZhanView*)pFrame->GetActiveView();

	double dmaxArea = 0.0;
//	char* buffer =  pView->buf4;
//	UINT nSize = pView->nsize4;
	char* buffer = g_pBuf4;
	UINT nSize = g_nBuf4Size;
	if (buffer != NULL)
	{
		PlaneFlawHeader PFH = {0};
		UINT Pos = 0;
		while(Pos+sizeof(PlaneFlawHeader) <= nSize)
		{		
			memcpy(&PFH, buffer+Pos, sizeof(PlaneFlawHeader));
			Pos += sizeof(PlaneFlawHeader);

			if (PFH.area > dmaxArea)
			{
				dmaxArea = PFH.area;
				ZhiShiRegion.min_x = PFH.min_x;
				ZhiShiRegion.min_y = PFH.min_y;
				ZhiShiRegion.max_x = PFH.max_x;
				ZhiShiRegion.max_y = PFH.max_y;
			}

			Pos += sizeof(HPOINT)*PFH.PointsCount;			 		
		}		
	}

	dbZhiShiArea = dmaxArea;
}

QVector<ZhiShiAreaRegionAndType> CBiaoZhun_ASTM435_435M::GetDrawSquarePosList()
{	
	// 	m_nZhiShiLength = 75;
	// 	m_dDivLengthValue = static_cast<double>(75/2);
	// 	m_vDrawSquareList.clear();
	// 	double dMaxLengthValue = 0.0;
	// 	ZhiShiAreaRegion maxLengthRegion_t;
	// 	CalcZhishiLength(dMaxLengthValue, maxLengthRegion_t);//返回结果为指示长度最大的面伤，同时会得到伤点颜色
	// 	m_dMaxLengthValue = dMaxLengthValue;
	// 	return m_vDrawSquareList;
	//return NULL;
	return m_vDrawSquareList;
}

QVector<ZhiShiAreaRegion> CBiaoZhun_ASTM435_435M::GetMaxVector(StandarConditiondType eType) 
{
	return m_vecMaxLength;
}

//根据指示长度判断单个伤,从而改变伤的成像颜色
BOOL CBiaoZhun_ASTM435_435M::UpdateLengthDrawColor(PlaneFlawHeader& PFH)
{
	try
	{
		ZhiShiAreaRegion addPoint_t;
		double x1 = PFH.min_x;
		double x2 = PFH.max_x;
		double y1 = PFH.min_y;
		double y2 = PFH.max_y;

		double dDistance = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

		if (dDistance < m_dDivLengthValue) 	//绿色
		{
			if (PFH.nColorType <= 1)
			{
				PFH.nColorType = 1;
			}
		}
		else if (dDistance >= m_dDivLengthValue && dDistance < m_nZhiShiLength) //黄色
		{
			if (PFH.nColorType <= 2)
			{
				PFH.nColorType = 2;
			}
		}
		else if (dDistance >= m_nZhiShiLength) //红色
		{
			if (PFH.nColorType <= 3)
			{
				PFH.nColorType = 3;
			}
		}
	}
//	catch (CException* e)
	catch (...)
	{
		//throw e;
		return FALSE;
	}
	return TRUE;
}
void CBiaoZhun_ASTM435_435M::initArrayFromBuf4()
{
	m_MianshangArray.clear();
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//CZongZhanView* pView = (CZongZhanView*)pFrame->GetActiveView();
	//char* buf =  pView->buf4;
	//UINT Size = pView->nsize4;
	char* buf = g_pBuf4;
	UINT Size = g_nBuf4Size;
	int Pos = 0;
	PlaneFlawHeader PFH = {0};

	int i=0;
	while(Pos+sizeof(PlaneFlawHeader) <= Size)
	{	
		memcpy(&PFH,buf+Pos,sizeof(PlaneFlawHeader));
		UpdateLengthDrawColor(PFH);
		PFH.bDisplay = 	TRUE;
		memcpy(buf+Pos, &PFH, sizeof(PlaneFlawHeader));

		PlaneFlawHeader2 tmp;
		tmp.min_x = PFH.min_x;
		tmp.min_y = PFH.min_y;
		tmp.max_x = PFH.max_x;
		tmp.max_y = PFH.max_y;
		tmp.z = PFH.z;
		tmp.type = PFH.type;
		tmp.area = PFH.area;
		tmp.EdgeOrBody = PFH.EdgeOrBody;
		tmp.PointsCount = PFH.PointsCount;
		tmp.nMemeryPos = Pos;
		tmp.bDisPlay = PFH.bDisplay;

		//m_MianshangArray.SetAtGrow(i, tmp);
		m_MianshangArray.push_back(tmp);

		Pos += sizeof(PlaneFlawHeader)+(sizeof(HPOINT)*PFH.PointsCount);
		i++;
	}
}

void CBiaoZhun_ASTM435_435M::CalcZhishiLength(double& dbZhiShiLength, ZhiShiAreaRegion& ZhiShiRegion)
{
	initArrayFromBuf4();//必须调用重新初期化
	// 查询伤
	QString strFileName = ("LengthLog.txt");
	for(int j=0; j< m_MianshangArray.size(); j++)
	{
		if (m_MianshangArray[j].bDisPlay)
		{
			if (m_MianshangArray[j].bCalculated)
			{
				continue;
			}
			m_MianshangArray[j].bCalculated = TRUE;

			// 写测试文件
			UINT style = 0;
			style = QIODevice::ReadWrite;
		/*	if (j==0)
			{
				style = CFile::modeCreate|CFile::modeReadWrite;
			}
			else
			{
				style = CFile::modeReadWrite;
			}	*/	

			// 查询数组内可以和它合并面积的伤
			double x1 = m_MianshangArray[j].min_x;
			double x2 = m_MianshangArray[j].max_x;
			double y1 = m_MianshangArray[j].min_y;
			double y2 = m_MianshangArray[j].max_y;			
			double dLength = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

			QString str;
			/*str.Format(_T("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t"), 
				dLength, m_MianshangArray[j].min_x,m_MianshangArray[j].max_x,
				m_MianshangArray[j].min_y,m_MianshangArray[j].max_y,m_MianshangArray[j].z);
			*/
			str = QString("%1\t%2\t%3\t%4\t%5\t%6\t")
				.arg(dLength, 0,'f',2)
				.arg(m_MianshangArray[j].min_x, 0, 'f', 2)
				.arg(m_MianshangArray[j].max_x, 0, 'f', 2)
				.arg(m_MianshangArray[j].min_y, 0, 'f', 2)
				.arg(m_MianshangArray[j].max_y, 0, 'f', 2)
				.arg(m_MianshangArray[j].z, 0, 'f', 2);

			ZhiShiAreaRegion tmpZhiShiArea;
			tmpZhiShiArea.min_x = m_MianshangArray[j].min_x;
			tmpZhiShiArea.max_x = m_MianshangArray[j].max_x;
			tmpZhiShiArea.min_y = m_MianshangArray[j].min_y;
			tmpZhiShiArea.max_y = m_MianshangArray[j].max_y;

			m_vnMemeryPos.clear();//存储每个小伤的坐标和位置
			//FindNearLength(m_MianshangArray, j, dLength, tmpZhiShiArea);

			ZhiShiAreaRegionAndType AreaRegionAndType;
			AreaRegionAndType.AreaRegion_t = tmpZhiShiArea;
			AreaRegionAndType.type = STANDAR_ASTM435_MAXDIAMETER;
			AreaRegionAndType.nGeneralOrMaxType = 0;
			m_vDrawSquareList.push_back(AreaRegionAndType);

			//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			//CZongZhanView* pView = (CZongZhanView*)pFrame->GetActiveView();

			PlaneFlawHeader PFH = {0};
			if (dLength < m_dDivLengthValue)
			{
				for (UINT i=0;i<m_vnMemeryPos.size();++i)
				{
					char* buffer =  g_pBuf4;;
					if (buffer != NULL)
					{
						memcpy(&PFH, buffer+m_vnMemeryPos[i], sizeof(PlaneFlawHeader));	

						if (PFH.nColorType >= 1)
						{
							continue;	
						}
						else
						{
							PFH.nColorType = 1;
							memcpy(buffer+m_vnMemeryPos[i], &PFH, sizeof(PlaneFlawHeader));
						}
					}
				}
			}
			else if (dLength >= m_dDivLengthValue && dLength < m_nZhiShiLength)
			{
				for (UINT i=0;i<m_vnMemeryPos.size();++i)
				{
					char* buffer =  g_pBuf4;;
					if (buffer != NULL)
					{
						memcpy(&PFH, buffer+m_vnMemeryPos[i], sizeof(PlaneFlawHeader));	
						if (PFH.nColorType >= 2)
						{
							continue;	
						}
						else
						{
							PFH.nColorType = 2;
							memcpy(buffer+m_vnMemeryPos[i], &PFH, sizeof(PlaneFlawHeader));
						}
					}
				}
			}
			else if (dLength >= m_nZhiShiLength)
			{
				for (UINT i=0;i<m_vnMemeryPos.size();++i)
				{
					char* buffer =  g_pBuf4;;
					if (buffer != NULL)
					{
						memcpy(&PFH, buffer+m_vnMemeryPos[i], sizeof(PlaneFlawHeader));	
						if (PFH.nColorType >= 3)
						{
							continue;	
						}
						else
						{
							PFH.nColorType = 3;
							memcpy(buffer+m_vnMemeryPos[i], &PFH, sizeof(PlaneFlawHeader));
						}
					}
				}
			}

			if (dLength >= dbZhiShiLength)
			{
				if (dLength > dbZhiShiLength)
				{
					m_vecMaxLength.clear();
				}
				m_vecMaxLength.push_back(tmpZhiShiArea);
				dbZhiShiLength = dLength;
				ZhiShiRegion.min_x = tmpZhiShiArea.min_x;
				ZhiShiRegion.max_x = tmpZhiShiArea.max_x;
				ZhiShiRegion.min_y = tmpZhiShiArea.min_y;
				ZhiShiRegion.max_y = tmpZhiShiArea.max_y;
			}
		}
	}
	if (m_MianshangArray.size() == 0)
	{
		m_vecMaxLength.clear();
	}
	double x1 = ZhiShiRegion.min_x;
	double x2 = ZhiShiRegion.max_x;
	double y1 = ZhiShiRegion.min_y;
	double y2 = ZhiShiRegion.max_y;			
	dbZhiShiLength = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

void CBiaoZhun_ASTM435_435M::initFromBuf4()
{
	m_MianshangArray.clear();
	int nZhiShiLength = 0;
	m_nZhiShiLength = 75;
	m_dDivLengthValue = static_cast<double>(75/2);
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//CZongZhanView* pView = (CZongZhanView*)pFrame->GetActiveView();
	//char* buf =  pView->buf4;
	//UINT Size = pView->nsize4;
	char* buf = g_pBuf4;
	UINT Size = g_nBuf4Size;
	int Pos = 0;
	PlaneFlawHeader PFH = {0};

	int i=0;
	while(Pos+sizeof(PlaneFlawHeader) <= Size)
	{	
		memcpy(&PFH,buf+Pos,sizeof(PlaneFlawHeader));
		UpdateLengthDrawColor(PFH);
	/*	if (pView->m_bSetMaxFlag && pView->m_bSetMinFlag)
		{
			if (PFH.area <= pView->m_dDefectMinVal || PFH.area >= pView->m_dDefectMaxVal)
			{
				PFH.bDisplay = 	FALSE;
			}
		}
		else
		{
			if (pView->m_bSetMinFlag)
			{
				if (PFH.area <= pView->m_dDefectMinVal)
				{
					PFH.bDisplay = 	FALSE;
				}	
			}
			if (pView->m_bSetMaxFlag)
			{
				if (PFH.area >= pView->m_dDefectMaxVal)
				{
					PFH.bDisplay = 	FALSE;
				}
			}
		}
		memcpy(buf+Pos, &PFH, sizeof(PlaneFlawHeader));

		PlaneFlawHeader2 tmp;
		tmp.min_x = PFH.min_x;
		tmp.min_y = PFH.min_y;
		tmp.max_x = PFH.max_x;
		tmp.max_y = PFH.max_y;
		tmp.z = PFH.z;
		tmp.type = PFH.type;
		tmp.area = PFH.area;
		tmp.EdgeOrBody = PFH.EdgeOrBody;
		tmp.PointsCount = PFH.PointsCount;
		tmp.nMemeryPos = Pos;
		tmp.bDisPlay = PFH.bDisplay;	
		m_MianshangArray.SetAtGrow(i, tmp);
		Pos += sizeof(PlaneFlawHeader)+(sizeof(HPOINT)*PFH.PointsCount);
		i++;*/
	}
}

void CBiaoZhun_ASTM435_435M::SetMaxVector()
{
	double dMaxLengthValue = 0.0;
	ZhiShiAreaRegion maxLengthRegion_t;
	CalcZhishiLength(dMaxLengthValue, maxLengthRegion_t);//返回结果为指示长度最大的面伤，同时会得到伤点颜色
	m_dMaxLengthValue = dMaxLengthValue;
	return;
}

void CBiaoZhun_ASTM435_435M::GetResultValues()
{
	m_dBodyMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetBodyMaxShangZhiShiLength()").toDouble();
	m_dEdgeMaxFlawZhiShiLength  = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLength()").toDouble();
	return;
}

void CBiaoZhun_ASTM435_435M::GetBiaoZhunItemInfo(const StandarConditiondType& eType, QRect& RctInfo, QString& strInfo)
{
	if(eType != STANDAR_ASTM435_MAXDIAMETER)
		return;
	double dMaxDiameter = 0.0;

	if (g_Info[nPlate].height > 150.0f)
		dMaxDiameter = g_Info[nPlate].height/2;
	else
		dMaxDiameter = 75.0f;

	RctInfo = m_VectorResultInfoRect.begin()->rect;
	//strInfo.Format(_T("≤%.0f mm"), dMaxDiameter);
	strInfo = QString("≤%1 mm").arg(dMaxDiameter);

}
// Song Chenguang add start on 2013-01-28
void CBiaoZhun_ASTM435_435M::InitBiaoZhunItemRect(const QRect& rctResult)
{
	int nperY = rctResult.height()/4;
	QRect rectTemp;
	rectTemp.setLeft(rctResult.left());
	rectTemp.setRight(rctResult.right());
	rectTemp.setTop(rctResult.center().y() + nperY);
	rectTemp.setBottom(rctResult.bottom());
	ResultRectAndType rectAndtype;
	rectAndtype.rect = rectTemp;
	rectAndtype.type = STANDAR_ASTM435_MAXDIAMETER;
	m_VectorResultRectAndType.push_back(rectAndtype);   //yachang.huang add on 2012-12-20

	// Song Chenguang add start on 2013-01-31
	// 添加项位置 
	rectTemp.translate(0,  -1 * nperY);
	ResultRectAndType rectAndtypeItem;
	rectAndtypeItem.rect = rectTemp;
	rectAndtypeItem.type = rectAndtype.type;
	m_VectorResultItemRect.push_back(rectAndtypeItem);

	// 添加信息显示位置
	rectTemp.translate(0, nperY * 2);
	ResultRectAndType rectAndtypeInfo;
	rectAndtypeInfo.rect = rectTemp;
	rectAndtypeInfo.type = rectAndtype.type;
	m_VectorResultInfoRect.push_back(rectAndtypeInfo);
	// Song Chenguang add end on 2013-01-31

}
//chenyuzhu add start on 2014-7-24
QString CBiaoZhun_ASTM435_435M::GetResult2(double dMaxLengthValue, double dMaxFlawZhiShiArea)
{
	QString strResult;
	/*// 从OCX文件里获取数据填写结果
	// 指示长度
	//double dBodyMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetBodyMaxShangZhiShiLength()").toDouble();
	//double dEdgeMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLength()").toDouble();

	if(m_dEdgeMaxLength >= 50.0f)
	{
		strResult = QObject::tr("不合格");
	}
	else
	{
		// 从指示长度字符串里取出最大值
		//double dMaxFlawZhiShiLength = dBodyMaxFlawZhiShiLength > dEdgeMaxFlawZhiShiLength ? dBodyMaxFlawZhiShiLength : dEdgeMaxFlawZhiShiLength;

		// 指示面积Max
		//	double dBodyMaxFlawZhiShiArea = g_pPlateDraw->dynamicCall("GetBodyMaxQueXianArea()").toDouble();
		//	double dEdgeMaxFlawZhiShiArea = g_pPlateDraw->dynamicCall("GetEdgeMaxQueXianArea()").toDouble();

		// 从OCX文件里获取面伤数据，根据标准对指示长度和面积进行再计算
		//	double dMaxFlawZhiShiArea = 0.0;
		//	ZhiShiAreaRegion zhishiAreaRegion;

		//	CalcZhishiArea(dMaxFlawZhiShiArea, zhishiAreaRegion);
		//	double dMaxLengthValue = 0.0;
		//		ZhiShiAreaRegion maxLengthRegion_t;
		//		CalcZhishiLength(dMaxLengthValue, maxLengthRegion_t);//返回结果为指示长度最大的面伤，同时会得到伤点颜色

		// 缺陷率

		double dQuexianlv = m_dAllMaxQueXianLv;

		// 行2（钢板的实际级别和检测标志比较，写合格或不合格，
		// 合格用绿色，不合格用红色，而且不合格的数值也要用红色标出来）
		// ...
		BOOL bRed[4];
		::ZeroMemory(bRed, sizeof(bRed));
		strResult = JudgeResult(dMaxLengthValue, dMaxFlawZhiShiArea,
			dQuexianlv, m_dEdgeMaxLength, bRed, 4);		
	}*/
	return strResult;
}
//chenyuzhu add end on 2014-7-24