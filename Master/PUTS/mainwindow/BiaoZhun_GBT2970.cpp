#include <QFile>
#include <QAxWidget>
#include <QString>
#include <QPainter>
#include <QByteArray>
#include "const.h"
#include "DataDefine.h"
#include "mainwindow.h"
#include "BiaoZhun.h"
#include "BiaoZhun_GBT2970.h"

//dong qi add start on 2013-11-22 
bool g_bLengthFlag;
double g_dMinDefectVal; //临时定义
//dong qi add end on 2013-11-22 

extern QAxWidget* g_pPlateDraw;
const double EDGE_WIDTH_GBT2970 = 50.0f;
extern PLATE_INFO g_Info[1];
extern int nPlate;
extern QString g_strJiBie;
extern CIniParams g_IniParams;
extern char *g_pBuf4;//用于存储点伤数据的内存区域
extern unsigned int g_nBuf4Size;//点伤大小
extern bool g_bLengthFlag;
extern double g_dMinDefectVal; //临时定义
extern QString g_strBiaoZhun;//chenyuzhu add on 2014-9-29

CBiaoZhun_GBT2970::CBiaoZhun_GBT2970()
{
	m_dPreDistance = 1e-7;
	m_dAllMaxQueXianLv = 0;	
}

CBiaoZhun_GBT2970::CBiaoZhun_GBT2970(const QString& strJiBie)
{
	SetParameter(strJiBie);
	m_dAllMaxQueXianLv = 0;	
	m_dEdgeMaxLength = 0;
	m_dMaxLengthValue = 0;
}

CBiaoZhun_GBT2970::~CBiaoZhun_GBT2970()
{

}

void CBiaoZhun_GBT2970::DrawScanResult(QPainter* painter,const QRect& rect)
{
	QPen pen(QColor(0,0,0),1,Qt::SolidLine);
	QBrush brush(QColor(255,255,255));
	painter->setPen(pen);
	painter->setBrush(brush);

	// 画框
	QRect ResultRect = rect;
	int nPerX = rect.width()/4;
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

	for (int i=1; i<4; i++)
	{
		ptStart.setX(ResultRect.left() + i * nPerX);
		ptStart.setY(ResultRect.center().y());
		ptEnd.setX(ptStart.x());
		ptEnd.setY(ResultRect.bottom());
		painter->drawLine(ptStart,ptEnd);
	}

	// 写标题
//	pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);

	// 行1
	painter->drawText(ResultRect.center().x(), ResultRect.top()+nperY, QObject::tr("探伤结果"));

	// 设字体
	painter->setFont(QFont(("Times New Roman"),11,4));

	// 行3
	QString str[4] = {QObject::tr("指示长度 mm"), QObject::tr("指示面积Max cm2"), QObject::tr("缺陷率 %"), QObject::tr("边界长度Max mm")};
	QPoint ptText;

	for (int i=0; i<4; i++)
	{
		ptText.setX(ResultRect.left() + (i+0.5) * nPerX);
		ptText.setY(ResultRect.center().y() + nperY);

		painter->drawText(ptText.x(), ptText.y(), str[i]);
	}

	// 从OCX文件里获取数据填写结果
	// 从OCX文件里获取面伤数据，根据标准对指示长度和面积进行再计算
	double dMaxFlawZhiShiArea2 = 0.0;
	ZhiShiAreaRegion zhishiAreaRegion;
	CalcZhishiArea(dMaxFlawZhiShiArea2, zhishiAreaRegion);

	// 指示长度
	double dEdgeMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLength()").toDouble();
	// 从指示长度字符串里取出最大值
	double dMaxFlawZhiShiLength = m_dMaxLengthValue;

	str[0] = QString("%1").arg(dMaxFlawZhiShiLength, 0, 'f', 0);

	// 指示面积Max(mm2)
	double dMaxFlawZhiShiArea = dMaxFlawZhiShiArea2 / 100.0f;	// cm2

	str[1] = QString("%1").arg(dMaxFlawZhiShiArea, 0, 'f', 0);

	// 缺陷率
	// 分两种情况决定是否重新算
	double dQuexianlv = 0.0f;
	if (m_bReCalc)
	{
		m_dAllMaxQueXianLv = g_pPlateDraw->dynamicCall("GetAllMaxQueXianLv()").toDouble() * 100;
		m_bReCalc = FALSE;
	}

	dQuexianlv = m_dAllMaxQueXianLv;

	str[2] = QString("%1").arg(dQuexianlv, 0, 'f', 2);

	// 边界长度Max
	str[3] = QString("%1").arg(m_dEdgeMaxLength, 0, 'f', 2);

	BOOL bRed[4];
	::ZeroMemory(bRed, sizeof(bRed));
	QString strResult = JudgeResult(dMaxFlawZhiShiLength, dMaxFlawZhiShiArea * 100, 
		dQuexianlv, m_dEdgeMaxLength, bRed, 4);
	for (int i=0; i<4; i++)
	{
		ptText.setX(ResultRect.left() + (i+0.5) * nPerX);
		ptText.setY(ResultRect.bottom());
		if(bRed[i])
		{
			painter->setPen(QColor(255, 0, 0));
		}
		else
		{
			painter->setPen(QColor(0, 0, 0));
		}
		painter->drawText(ptText.x(), ptText.y(), str[i]);
	}

	painter->setPen(QColor(0, 0, 0));
	// 行2（钢板的实际级别和检测标志比较，写合格或不合格，
	// 合格用绿色，不合格用红色，而且不合格的数值也要用红色标出来）
	m_rectResultPos.setLeft(ResultRect.left());
	m_rectResultPos.setRight(ResultRect.right());
	m_rectResultPos.setTop(ResultRect.top() + nperY);
	m_rectResultPos.setBottom(ResultRect.bottom() - nperY*2);	
}

void CBiaoZhun_GBT2970::SetParameter(const QString& strJiBie)
{
	m_strJiBie = strJiBie;
	g_pPlateDraw->dynamicCall("SetShangWidthType(short)",SHANG_WIDTH_Y);
	g_pPlateDraw->dynamicCall("SetNotUseBodyMinShangLength(double)",0);
	g_pPlateDraw->dynamicCall("SetNotUseBodyMinShangWidth(double)",0);
	g_pPlateDraw->dynamicCall("SetNotUseEdgeMinShangLength(double)",0);
	g_pPlateDraw->dynamicCall("SetNotUseEdgeMinShangWidth(double)",0);

	// 设置指示长度
	g_pPlateDraw->dynamicCall("SetZhiShiLengthType(short)",ZHISHI_LENGTH_DIAGONAL);
	/*
	*设置伤宽度类型
	// 1 = 以伤X方向值为伤宽度
	// 2 = 以伤Y方向值为伤宽度
	// 3 = x和y之间的较小值
	void SetShangWidthType(short ShangWidthType)
	*/
	// 指示面积计算方式
	g_pPlateDraw->dynamicCall("SetAreaType(short)",AREA_REAL);

	// 伤合并规则
	g_pPlateDraw->dynamicCall("SetMergeShangType(short,double)",SHANG_MERGE_MAX_DISANCE,1.0f);
	g_pPlateDraw->dynamicCall("SetMergeShangType(short,double)",SHANG_MERGE_SET_DISANCE, 100.0f);

	// *设定不记录的本体伤的最小指示长度
	g_pPlateDraw->dynamicCall("SetNotUseBodyMinShangZhiShiLength(double)",40.0);

	// *设定不记录的边缘最小伤指示长度
	g_pPlateDraw->dynamicCall("SetNotUseEdgeMinShangZhiShiLength(double)",40.0);

	// 设置边缘的宽带
	if (g_Info[nPlate].height <100.0f)
	{
		g_pPlateDraw->dynamicCall("SetEdgeWidth(double)",EDGE_WIDTH_GBT2970);
	}
	else
	{
		double temp = g_Info[nPlate].height / 2.0;
		g_pPlateDraw->dynamicCall("SetEdgeWidth(double)",temp);
	}

	if (strJiBie == ("I"))
	{	  
		g_pPlateDraw->dynamicCall("SetNotUseBodyMinArea(double)",0);
		g_pPlateDraw->dynamicCall("SetNotUseEdgeMinArea(double)",0);
	}
	else if (strJiBie == ("II"))
	{
		g_pPlateDraw->dynamicCall("SetNotUseBodyMinArea(double)",0);
		g_pPlateDraw->dynamicCall("SetNotUseEdgeMinArea(double)",0);
	}
	else if (strJiBie == ("III"))
	{
		g_pPlateDraw->dynamicCall("SetNotUseBodyMinArea(double)",0);
		g_pPlateDraw->dynamicCall("SetNotUseEdgeMinArea(double)",0);
	}
	else if (strJiBie == ("IV"))
	{
		g_pPlateDraw->dynamicCall("SetNotUseBodyMinArea(double)",0);
		g_pPlateDraw->dynamicCall("SetNotUseEdgeMinArea(double)",0);
	}
	else
	{
		QMessageBox::about(NULL,NULL,"探伤级别错误!");
		return;
	}
}

void CBiaoZhun_GBT2970::SetParameter()
{

}

BOOL CBiaoZhun_GBT2970::IsEdgeDetect(PlaneFlawHeader& PFH)
{
	BOOL bRet = FALSE;
	double x1 = PFH.min_x;
	double x2 = PFH.max_x;
	double y1 = PFH.min_y;
	double y2 = PFH.max_y;

	double dDistance = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

	//是否是边缘上的伤
	if (x1 <= 50 || x2 >= g_Info[nPlate].fRealLength - 50 || y1 <=50 || y2 >= g_Info[nPlate].width - 50)
	{
		bRet = TRUE;
	}
	else
	{
		return bRet;
	}

	if(dDistance < 50/2)
	{
		PFH.nColorType = 1;	
	}
	else if(dDistance >= 50/2 && dDistance < 50)
	{
		PFH.nColorType = 2;			
	}
	else
	{
		PFH.nColorType = 3;			
	}
	return bRet;
}

//根据指示长度判断单个伤,从而改变伤的成像颜色
BOOL CBiaoZhun_GBT2970::UpdateLengthDrawColor(PlaneFlawHeader& PFH)
{
	try
	{
		if (IsEdgeDetect(PFH))
		{
			return TRUE;
		}
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
	//catch (CException* e)
	catch (...)
	{
		//throw e;
		return FALSE;
	}
	return TRUE;
}

//根据面积判断单个伤,从而改变伤的成像颜色
BOOL CBiaoZhun_GBT2970::UpdateAreaDrawColor(PlaneFlawHeader& PFH)
{
	try
	{
		if (IsEdgeDetect(PFH))
		{
			return TRUE;
		}
		ZhiShiAreaRegion addPoint_t;
		double dArea = PFH.area;

		if (dArea < m_nDivAreaValue) 	//绿色
		{
			if (PFH.nColorType <= 1)
			{
				PFH.nColorType = 1;
			}
		}
		else if (dArea >= m_nDivAreaValue && dArea < m_nZhiShiArea) //黄色
		{
			if (PFH.nColorType <= 2)
			{
				PFH.nColorType = 2;
			}
		}
		else if (dArea >= m_nZhiShiArea) //红色
		{
			if (PFH.nColorType <= 3)
			{
				PFH.nColorType = 3;
			}
		}
	}
	//catch (CException* e)
	catch(...)
	{
		//throw e;
		return FALSE;
	}
	return TRUE;
}

QVector<ZhiShiAreaRegionAndType> CBiaoZhun_GBT2970::GetDrawSquarePosList()
{
	int nZhiShiLength = 0;
	int nZhiShiArea = 0;
	int nMaxQueXianLv = 0;//没有实装此条件

	GetLevelValue(m_strJiBie, nZhiShiLength, nZhiShiArea, nMaxQueXianLv);
	m_nZhiShiArea = nZhiShiArea;
	m_nDivAreaValue = static_cast<double>(m_nZhiShiArea/2);  

	m_nZhiShiLength = nZhiShiLength;
	m_dDivLengthValue = static_cast<double>(nZhiShiLength/2);  
	double dmin_x = 0.0f;
	double dmax_x = 0.0f;
	double dmin_y = 0.0f;
	double dmax_y = 0.0f;
	double dArea = 0.0f;
	m_vecMaxEdgeLength.clear();
	double dEdgeMaxZhiShiLength = 0;
/*	double dEdgeMaxZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLengthX(double&,double&,double&,double&,double&)",dmin_x,dmax_x,dmin_y,dmax_y,dArea).toDouble();*/

//	MessageBox(NULL,L"GetEdgeMaxShangZhiShiLengthXQt",L"GetEdgeMaxShangZhiShiLengthXQt",MB_OK);
	QByteArray temp = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLengthXQt()").toByteArray();  //6个double
	//chenyuzhu modify start on 2014-10-31
	if (temp.size() != 0)
	{
		int tempSize = temp.size();
		char* tempBuf = temp.data();
		char* tempBuf2;
		tempBuf2 = new char[tempSize];
		memcpy(tempBuf2,tempBuf,tempSize);

		int tempPos = 0;
		memcpy(&(dEdgeMaxZhiShiLength),tempBuf2,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmin_x),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmax_x),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmin_y),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmax_y),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dArea),tempBuf2+tempPos,sizeof(double));
		delete[] tempBuf2;
	}
	//chenyuzhu add end on 2014-10-31

	ZhiShiAreaRegion Region;
	if (dEdgeMaxZhiShiLength > m_dtmpEdgeZhishiLength && ( dmin_x > g_IniParams.m_fNeiFrontLength && dmax_x <  (g_Info[nPlate].length - g_IniParams.m_fNeiTailLength)))
	{
		m_dEdgeMaxLength = dEdgeMaxZhiShiLength; 
		Region.min_x = dmin_x;
		Region.min_y = dmin_y;
		Region.max_x = dmax_x;
		Region.max_y = dmax_y;
	}
	else
	{
		m_dEdgeMaxLength = m_dtmpEdgeZhishiLength;
		Region.min_x = m_dtmpEdgeMinX;
		Region.min_y = m_dtmpEdgeMinY;
		Region.max_x = m_dtmpEdgeMaxX;
		Region.max_y = m_dtmpEdgeMaxY;
	}

	m_vecMaxEdgeLength.push_back(Region);//双击返回时使用
	m_vDrawSquareList.clear();
	double dMaxAreaValue = 0.0;
	ZhiShiAreaRegion maxAreaRegion_t;

	CalcZhishiArea(dMaxAreaValue, maxAreaRegion_t);//取得伤的列表

	double dMaxLengthValue = 0.0;
	ZhiShiAreaRegion maxLengthRegion_t;

	//崩溃测试先注释掉
	CalcZhishiLength(dMaxLengthValue, maxLengthRegion_t);//返回结果为指示长度最大的面伤，同时会得到伤点颜色
	m_dMaxLengthValue = dMaxLengthValue;

	return m_vDrawSquareList;
}

QString CBiaoZhun_GBT2970::JudgeResult(const double& dRealZhiShiLength,
									   const double& dRealZhiShiArea,
									   const double& dRealQueXianLv)
{
	int nZhiShiLength = 0;
	int nZhiShiArea = 0;
	int nMaxQueXianLv = 0;

	GetLevelValue(m_strJiBie, nZhiShiLength, nZhiShiArea, nMaxQueXianLv);

	if ((dRealZhiShiLength >= nZhiShiLength) || 
		(dRealZhiShiArea >= nZhiShiArea) ||
		(dRealQueXianLv > nMaxQueXianLv))
	{
		return QObject::tr("不合格");
	}
	else
	{
		return QObject::tr("合格");
	}
}

void CBiaoZhun_GBT2970::GetLevelValue(const QString& strLevel,
									  int& nMaxZhiShiLength,
									  int& nMaxZhiShiArea,
									  int& nMaxQueXianLv)
{
	if (strLevel == ("I"))
	{
		nMaxZhiShiLength = 80;
		nMaxZhiShiArea = 2500;
		nMaxQueXianLv = 3;
		m_fBiaoZhunZhiShiLength = 80;
		m_fBiaoZhunZhiShiArea = 25;
		m_fBiaoZhunQueXianLv = 3;
	}
	else if (strLevel == ("II"))
	{
		nMaxZhiShiLength = 100;
		nMaxZhiShiArea = 5000;
		nMaxQueXianLv = 5;
		m_fBiaoZhunZhiShiLength = 100;
		m_fBiaoZhunZhiShiArea = 50;
		m_fBiaoZhunQueXianLv = 5;
	}
	else if (strLevel == ("III"))
	{
		nMaxZhiShiLength = 120;
		nMaxZhiShiArea = 10000;
		nMaxQueXianLv = 10;
		m_fBiaoZhunZhiShiLength = 120;
		m_fBiaoZhunZhiShiArea = 100;
		m_fBiaoZhunQueXianLv = 10;
	}
	else if (strLevel == ("IV"))
	{
		nMaxZhiShiLength = 150;
		nMaxZhiShiArea = 10000;
		nMaxQueXianLv = 10;
		m_fBiaoZhunZhiShiLength = 150;
		m_fBiaoZhunZhiShiArea = 100;
		m_fBiaoZhunQueXianLv = 10;
	}
	else
	{
		QMessageBox::about(NULL,NULL,"探伤级别标准错误");
	}
}

QString CBiaoZhun_GBT2970::GetResult()
{
	QString strResult;
	// 从OCX文件里获取数据填写结果
	// 指示长度
	double dBodyMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetBodyMaxShangZhiShiLength()").toDouble();
	double dEdgeMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLength()").toDouble();

	if(m_dEdgeMaxLength >= 50.0f)
	{
		strResult = QObject::tr("不合格");
	}
	else
	{
		// 从指示长度字符串里取出最大值
		double dMaxFlawZhiShiLength = dBodyMaxFlawZhiShiLength > dEdgeMaxFlawZhiShiLength ? dBodyMaxFlawZhiShiLength : dEdgeMaxFlawZhiShiLength;

		// 指示面积Max
		double dBodyMaxFlawZhiShiArea = g_pPlateDraw->dynamicCall("GetBodyMaxQueXianArea()").toDouble();
		double dEdgeMaxFlawZhiShiArea = g_pPlateDraw->dynamicCall("GetEdgeMaxQueXianArea()").toDouble();

		// 从OCX文件里获取面伤数据，根据标准对指示长度和面积进行再计算
		double dMaxFlawZhiShiArea = 0.0;
		ZhiShiAreaRegion zhishiAreaRegion;

		CalcZhishiArea(dMaxFlawZhiShiArea, zhishiAreaRegion);
		double dMaxLengthValue = 0.0;
		ZhiShiAreaRegion maxLengthRegion_t;
		CalcZhishiLength(dMaxLengthValue, maxLengthRegion_t);//返回结果为指示长度最大的面伤，同时会得到伤点颜色

		// 缺陷率

		double dQuexianlv = m_dAllMaxQueXianLv;

		// 行2（钢板的实际级别和检测标志比较，写合格或不合格，
		// 合格用绿色，不合格用红色，而且不合格的数值也要用红色标出来）
		// ...
		BOOL bRed[4];
		::ZeroMemory(bRed, sizeof(bRed));
		strResult = JudgeResult(dMaxLengthValue, dMaxFlawZhiShiArea,
			dQuexianlv, m_dEdgeMaxLength, bRed, 4);		
	}

	return strResult;
}

//chenyuzhu add start on 2014-7-24
QString CBiaoZhun_GBT2970::GetResult2(double dMaxLengthValue, double dMaxFlawZhiShiArea)
{
	QString strResult;
	// 从OCX文件里获取数据填写结果
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
	}
	return strResult;
}
//chenyuzhu add end on 2014-7-24

void CBiaoZhun_GBT2970::initArrayFromBuf4(QString csflag)
{
	m_MianshangArray.clear();
	char* buf = g_pBuf4;
	UINT Size = g_nBuf4Size;
	int Pos = 0;
	PlaneFlawHeader PFH = {0};

	int i=0;
	while(Pos+sizeof(PlaneFlawHeader) <= Size)
	{	
		memcpy(&PFH,buf+Pos,sizeof(PlaneFlawHeader));
		if (("Length") == csflag)
		{
			UpdateLengthDrawColor(PFH);
		}
		else if (("Area") == csflag)
		{
			UpdateAreaDrawColor(PFH);
		}
		//判断是不是边界最大的伤点，如果是则为红色
		if (PFH.max_x == m_vecMaxEdgeLength[0].max_x &&
			PFH.max_y == m_vecMaxEdgeLength[0].max_y &&
			PFH.min_x == m_vecMaxEdgeLength[0].min_x &&
			PFH.min_y == m_vecMaxEdgeLength[0].min_y &&
			m_dEdgeMaxLength >= 50)
		{
			PFH.nColorType =  3;
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

		m_MianshangArray.push_back(tmp);

		Pos += sizeof(PlaneFlawHeader)+(sizeof(HPOINT)*PFH.PointsCount);
		i++;
	}
}

void CBiaoZhun_GBT2970::CalcZhishiLength(double& dbZhiShiLength, ZhiShiAreaRegion& ZhiShiRegion)
{

//	MessageBox(NULL,L"initArrayFromBuf4",L"initArrayFromBuf4",MB_OK);
	initArrayFromBuf4(("Length"));//必须调用重新初期化
//	MessageBox(NULL,L"initArrayFromBuf4END",L"initArrayFromBuf4END",MB_OK);
	// 查询伤
	QString strFileName = ("LengthLog.txt");
	for(int j=0; j< m_MianshangArray.size(); j++)
	{
		if (m_MianshangArray[j].bCalculated)
		{
			continue;
		}
		m_MianshangArray[j].bCalculated = TRUE;

		// 写测试文件
		UINT style = 0;
		style = QIODevice::ReadWrite;
// 		if (j==0)
// 		{
// 			style = CFile::modeCreate|CFile::modeReadWrite;
// 		}
// 		else
// 		{
// 			style = CFile::modeReadWrite;
// 		}

		// 查询数组内可以和它合并面积的伤
		double x1 = m_MianshangArray[j].min_x;
		double x2 = m_MianshangArray[j].max_x;
		double y1 = m_MianshangArray[j].min_y;
		double y2 = m_MianshangArray[j].max_y;			
		double dLength = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

		QString str;
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
//		MessageBox(NULL,L"FindNearLength",L"FindNearLength",MB_OK);
		FindNearLength(m_MianshangArray, j, dLength, tmpZhiShiArea);
//		MessageBox(NULL,L"FindNearLengthEND",L"FindNearLengthEND",MB_OK);

		ZhiShiAreaRegionAndType AreaRegionAndType;
		AreaRegionAndType.AreaRegion_t = tmpZhiShiArea;
		AreaRegionAndType.type = STANDAR_GBT2970_LENGTH;
		AreaRegionAndType.nGeneralOrMaxType = 0;
		m_vDrawSquareList.push_back(AreaRegionAndType);

		PlaneFlawHeader PFH = {0};
		if (dLength < m_dDivLengthValue)
		{
			for (UINT i=0;i<m_vnMemeryPos.size();++i)
			{
				char* buffer = g_pBuf4;
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
				char* buffer =  g_pBuf4;
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
				char* buffer =  g_pBuf4;
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
	if (m_MianshangArray.size() == 0)
	{
		m_vecMaxLength.clear();
		m_dEdgeMaxLength = 0;
		m_dMaxLengthValue = 0;
	}
	double x1 = ZhiShiRegion.min_x;
	double x2 = ZhiShiRegion.max_x;
	double y1 = ZhiShiRegion.min_y;
	double y2 = ZhiShiRegion.max_y;			
	dbZhiShiLength = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

void CBiaoZhun_GBT2970::FindNearLength(QVector<PlaneFlawHeader2>& MianshangArray, 
									   int nIndex, double& dLength, ZhiShiAreaRegion& rtRegion)
{
	PlaneFlawHeader2 tmp = MianshangArray[nIndex];

	for(int j=0; j< MianshangArray.size(); j++)
	{
		// 若已合并过，跳过
		if (MianshangArray[j].bCalculated)
		{
			continue;
		}

		double dMinX = tmp.min_x < MianshangArray[j].min_x ? tmp.min_x: MianshangArray[j].min_x;
		double dMaxX = tmp.max_x > MianshangArray[j].max_x ? tmp.max_x : MianshangArray[j].max_x;

		double xCha = dMaxX - dMinX;

		double dMinY = tmp.min_y < MianshangArray[j].min_y ? tmp.min_y: MianshangArray[j].min_y;
		double dMaxY = tmp.max_y > MianshangArray[j].max_y ? tmp.max_y : MianshangArray[j].max_y;

		double yCha = dMaxY - dMinY;

		double dDistance = 0.0;

		// X方向有重合
		if (xCha < (tmp.max_x - tmp.min_x + MianshangArray[j].max_x - MianshangArray[j].min_x))
		{
			if (tmp.min_y > MianshangArray[j].max_y)
			{
				dDistance = tmp.min_y - MianshangArray[j].max_y;
			}
			else
			{
				dDistance = MianshangArray[j].min_y - tmp.max_y;
			}
		}
		// Y方向有重合
		else if (yCha < (tmp.max_y - tmp.min_y + MianshangArray[j].max_y - MianshangArray[j].min_y))
		{
			if (tmp.min_x > MianshangArray[j].max_x)
			{
				dDistance = tmp.min_x - MianshangArray[j].max_x;
			}
			else
			{
				dDistance = MianshangArray[j].min_x - tmp.max_x;
			}
		}
		else	// X，Y方向都不重合
		{
			double dCenterX2 = (tmp.max_x+tmp.min_x)/2;
			double dCenterY2 = (tmp.max_y+tmp.min_y)/2;

			double dCenterX1 = (MianshangArray[j].max_x + MianshangArray[j].min_x)/2;
			double dCenterY1 = (MianshangArray[j].max_y + MianshangArray[j].min_y)/2;

			if (dCenterX1 < dCenterX2 && dCenterY1 < dCenterY2)
			{
				dDistance = sqrt((tmp.min_x - MianshangArray[j].max_x)*(tmp.min_x - MianshangArray[j].max_x)+
					(tmp.min_y - MianshangArray[j].max_y)*(tmp.min_y - MianshangArray[j].max_y));
			}
			else if (dCenterX1 < dCenterX2 && dCenterY1 > dCenterY2)
			{
				dDistance = sqrt((tmp.min_x - MianshangArray[j].max_x)*(tmp.min_x - MianshangArray[j].max_x)+
					(MianshangArray[j].min_y - tmp.max_y)*(MianshangArray[j].min_y - tmp.max_y));
			}
			else if (dCenterX1 > dCenterX2 && dCenterY1 < dCenterY2)
			{
				dDistance = sqrt((MianshangArray[j].min_x - tmp.max_x)*(MianshangArray[j].min_x - tmp.max_x)+
					(tmp.min_y - MianshangArray[j].max_y)*(tmp.min_y - MianshangArray[j].max_y));
			}
			else	// dCenterX1 > dCenterX2 && dCenterY1 < dCenterY2
			{
				dDistance = sqrt((MianshangArray[j].min_x - tmp.max_x)*(MianshangArray[j].min_x - tmp.max_x)+
					(MianshangArray[j].min_y - tmp.max_y)*(MianshangArray[j].min_y - tmp.max_y));
			}
		}

		// 若满足条件，将面积相加
		double dMinVal=0.0f;

		if (g_bLengthFlag)
		{
			dMinVal = g_dMinDefectVal;//pView->m_dMinDefectVal;
		}
		if (dDistance < dMinVal)//pView->m_DlgPlateInfo.m_dMinDefectVal最终要放到加密狗里
		{
			// 合并面积

			// 计算扩大后的范围
			double tmpMinX = rtRegion.min_x < MianshangArray[j].min_x ? rtRegion.min_x : MianshangArray[j].min_x;
			double tmpMaxX = rtRegion.max_x > MianshangArray[j].max_x ? rtRegion.max_x : MianshangArray[j].max_x;
			double tmpMinY = rtRegion.min_y < MianshangArray[j].min_y ? rtRegion.min_y : MianshangArray[j].min_y;
			double tmpMaxY = rtRegion.max_y > MianshangArray[j].max_y ? rtRegion.max_y : MianshangArray[j].max_y;

			if (m_vnMemeryPos.size() == 0)
			{
				m_vnMemeryPos.push_back(MianshangArray[nIndex].nMemeryPos);
			}
			m_vnMemeryPos.push_back(MianshangArray[j].nMemeryPos);

			rtRegion.min_x = tmpMinX;
			rtRegion.max_x = tmpMaxX;
			rtRegion.min_y = tmpMinY;
			rtRegion.max_y = tmpMaxY;

			double x1 = rtRegion.min_x;
			double x2 = rtRegion.max_x;
			double y1 = rtRegion.min_y;
			double y2 = rtRegion.max_y;	
			dLength = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

			MianshangArray[j].bCalculated = TRUE;

			QString strFileName = ("LengthLog.txt");
			QString str;
			str = QString("%1\t%2\t%3\t%4\t%5\t%6\t")
					.arg(dLength, 0, 'f', 2)
					.arg(MianshangArray[j].min_x, 0, 'f', 2)
					.arg(MianshangArray[j].max_x, 0, 'f', 2)
					.arg(MianshangArray[j].min_y, 0, 'f', 2)
					.arg(MianshangArray[j].max_y, 0, 'f', 2)
					.arg(MianshangArray[j].z, 0, 'f', 2);
			// 查询数组内可以和它合并面积的伤
			FindNearLength(MianshangArray, j,dLength, rtRegion);
		}
	}
}

QVector<ZhiShiAreaRegion> CBiaoZhun_GBT2970::GetMaxVector(StandarConditiondType eType)
{
	QVector<ZhiShiAreaRegion> vect;
	if(eType == STANDAR_GBT2970_AREA)
	{
		return m_vecMaxArea;
	}
	else if (eType == STANDAR_GBT2970_LENGTH)
	{
		return m_vecMaxLength;
	}
	else if (eType == STANDAR_GBT2970_EDGEMAXFLAW)
	{
		return m_vecMaxEdgeLength;
	}
	else if (eType == STANDAR_GBT2970_DEFECTRATE)
	{
		return vect;
	}
	else
	{
		return vect;
	}
}

void CBiaoZhun_GBT2970::CalcZhishiArea(double& dbZhiShiArea, ZhiShiAreaRegion& ZhiShiRegion)
{
	initArrayFromBuf4(("Area"));//必须调用重新初期化

	// 查询伤
	QString strFileName = "AreaLog.txt";
	for(int j=0; j< m_MianshangArray.size(); j++)
	{
		if (m_MianshangArray[j].bCalculated)
		{
			continue;
		}
		m_MianshangArray[j].bCalculated = TRUE;

		// 写测试文件
		UINT style = 0;
		style = QIODevice::ReadWrite;
// 		if (j==0)
// 		{
// 			style = CFile::modeCreate|CFile::modeReadWrite;
// 		}
// 		else
// 		{
// 			style = CFile::modeReadWrite;
// 		}

		QString str;
		str = QString("%1\t%2\t%3\t%4\t%5\t%6\t")
				.arg(m_MianshangArray[j].area, 0, 'f', 2)
				.arg(m_MianshangArray[j].min_x, 0, 'f', 2)
				.arg(m_MianshangArray[j].max_x, 0, 'f', 2)
				.arg(m_MianshangArray[j].min_y, 0, 'f', 2)
				.arg(m_MianshangArray[j].max_y, 0, 'f', 2)
				.arg(m_MianshangArray[j].z, 0, 'f', 2);

		// 查询数组内可以和它合并面积的伤
		double dArea = m_MianshangArray[j].area;
		ZhiShiAreaRegion tmpZhiShiArea;
		tmpZhiShiArea.min_x = m_MianshangArray[j].min_x;
		tmpZhiShiArea.max_x = m_MianshangArray[j].max_x;
		tmpZhiShiArea.min_y = m_MianshangArray[j].min_y;
		tmpZhiShiArea.max_y = m_MianshangArray[j].max_y;

		m_vnMemeryPos.clear();//存储每个小伤的坐标和位置
		FindNearArea(m_MianshangArray, j, dArea, tmpZhiShiArea);
		ZhiShiAreaRegionAndType AreaRegionAndType;
		AreaRegionAndType.AreaRegion_t = tmpZhiShiArea;
		AreaRegionAndType.type = STANDAR_GBT2970_AREA;
		AreaRegionAndType.nGeneralOrMaxType = 0;
		m_vDrawSquareList.push_back(AreaRegionAndType);


		PlaneFlawHeader PFH = {0};
		if (dArea < m_nDivAreaValue)
		{
			for (UINT i=0;i<m_vnMemeryPos.size();++i)
			{
				char* buffer =  g_pBuf4;
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
		else if (dArea >= m_nDivAreaValue && dArea < m_nZhiShiArea)
		{
			for (UINT i=0;i<m_vnMemeryPos.size();++i)
			{
				char* buffer =  g_pBuf4;
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
		else if (dArea >= m_nZhiShiArea)
		{
			for (UINT i=0;i<m_vnMemeryPos.size();++i)
			{
				char* buffer =  g_pBuf4;
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

		if (dArea >= dbZhiShiArea)
		{
			if (dArea > dbZhiShiArea)
			{
				m_vecMaxArea.clear();
			}
			m_vecMaxArea.push_back(tmpZhiShiArea);
			dbZhiShiArea = dArea;
			ZhiShiRegion.min_x = tmpZhiShiArea.min_x;
			ZhiShiRegion.max_x = tmpZhiShiArea.max_x;
			ZhiShiRegion.min_y = tmpZhiShiArea.min_y;
			ZhiShiRegion.max_y = tmpZhiShiArea.max_y;
		}
	}

	// 在buf4里查询最大范围内的面伤，将其面积相加作为最大指示面积
	if (m_MianshangArray.size() == 0)
	{
		m_vecMaxArea.clear();
	}
	double sumArea = 0.0;
	for(int j=0; j< m_MianshangArray.size(); j++)
	{
		// 若在范围内,面积相加
		if ((m_MianshangArray[j].min_x >= ZhiShiRegion.min_x) &&
			(m_MianshangArray[j].max_x <= ZhiShiRegion.max_x) &&
			(m_MianshangArray[j].min_y >= ZhiShiRegion.min_y) &&
			(m_MianshangArray[j].max_y <= ZhiShiRegion.max_y))
		{
			sumArea += m_MianshangArray[j].area;
		}
	}
	dbZhiShiArea = sumArea;
}

void CBiaoZhun_GBT2970::FindNearArea(QVector<PlaneFlawHeader2>& MianshangArray, 
									 int nIndex, double& dArea, ZhiShiAreaRegion& rtRegion)
{
	PlaneFlawHeader2 tmp = MianshangArray[nIndex];
	for(int j=0; j< MianshangArray.size(); j++)
	{
		// 若已合并过，跳过
		if (MianshangArray[j].bCalculated)
		{
			continue;
		}
		double dMinX = tmp.min_x < MianshangArray[j].min_x ? tmp.min_x: MianshangArray[j].min_x;
		double dMaxX = tmp.max_x > MianshangArray[j].max_x ? tmp.max_x : MianshangArray[j].max_x;

		double xCha = dMaxX - dMinX;

		double dMinY = tmp.min_y < MianshangArray[j].min_y ? tmp.min_y: MianshangArray[j].min_y;
		double dMaxY = tmp.max_y > MianshangArray[j].max_y ? tmp.max_y : MianshangArray[j].max_y;

		double yCha = dMaxY - dMinY;

		double dDistance = 0.0;

		// X方向有重合
		if (xCha < (tmp.max_x - tmp.min_x + MianshangArray[j].max_x - MianshangArray[j].min_x))
		{
			if (tmp.min_y > MianshangArray[j].max_y)
			{
				dDistance = tmp.min_y - MianshangArray[j].max_y;
			}
			else
			{
				dDistance = MianshangArray[j].min_y - tmp.max_y;
			}
		}
		// Y方向有重合
		else if (yCha < (tmp.max_y - tmp.min_y + MianshangArray[j].max_y - MianshangArray[j].min_y))
		{
			if (tmp.min_x > MianshangArray[j].max_x)
			{
				dDistance = tmp.min_x - MianshangArray[j].max_x;
			}
			else
			{
				dDistance = MianshangArray[j].min_x - tmp.max_x;
			}
		}
		else	// X，Y方向都不重合
		{
			double dCenterX2 = (tmp.max_x+tmp.min_x)/2;
			double dCenterY2 = (tmp.max_y+tmp.min_y)/2;

			double dCenterX1 = (MianshangArray[j].max_x + MianshangArray[j].min_x)/2;
			double dCenterY1 = (MianshangArray[j].max_y + MianshangArray[j].min_y)/2;

			if (dCenterX1 < dCenterX2 && dCenterY1 < dCenterY2)
			{
				dDistance = sqrt((tmp.min_x - MianshangArray[j].max_x)*(tmp.min_x - MianshangArray[j].max_x)+
					(tmp.min_y - MianshangArray[j].max_y)*(tmp.min_y - MianshangArray[j].max_y));
			}
			else if (dCenterX1 < dCenterX2 && dCenterY1 > dCenterY2)
			{
				dDistance = sqrt((tmp.min_x - MianshangArray[j].max_x)*(tmp.min_x - MianshangArray[j].max_x)+
					(MianshangArray[j].min_y - tmp.max_y)*(MianshangArray[j].min_y - tmp.max_y));
			}
			else if (dCenterX1 > dCenterX2 && dCenterY1 < dCenterY2)
			{
				dDistance = sqrt((MianshangArray[j].min_x - tmp.max_x)*(MianshangArray[j].min_x - tmp.max_x)+
					(tmp.min_y - MianshangArray[j].max_y)*(tmp.min_y - MianshangArray[j].max_y));
			}
			else	// dCenterX1 > dCenterX2 && dCenterY1 < dCenterY2
			{
				dDistance = sqrt((MianshangArray[j].min_x - tmp.max_x)*(MianshangArray[j].min_x - tmp.max_x)+
					(MianshangArray[j].min_y - tmp.max_y)*(MianshangArray[j].min_y - tmp.max_y));
			}
		}

		double dZhiShiLength1 = sqrt((tmp.max_x - tmp.min_x)*(tmp.max_x - tmp.min_x) +
			(tmp.max_y - tmp.min_y)*(tmp.max_y - tmp.min_y));
		double dZhiShiLength2 = sqrt((MianshangArray[j].max_x - MianshangArray[j].min_x)*(MianshangArray[j].max_x - MianshangArray[j].min_x) +
			(MianshangArray[j].max_y - MianshangArray[j].min_y)*(MianshangArray[j].max_y - MianshangArray[j].min_y));

		double MaxZhiShiLength = dZhiShiLength1 > dZhiShiLength2? dZhiShiLength1:dZhiShiLength2;
		// 若满足条件，将面积相加
		if (dDistance< 100.0f || dDistance < MaxZhiShiLength)
		{
			// 合并面积
			dArea +=  MianshangArray[j].area;

			// 计算扩大后的范围
			double tmpMinX = rtRegion.min_x < MianshangArray[j].min_x ? rtRegion.min_x : MianshangArray[j].min_x;
			double tmpMaxX = rtRegion.max_x > MianshangArray[j].max_x ? rtRegion.max_x : MianshangArray[j].max_x;
			double tmpMinY = rtRegion.min_y < MianshangArray[j].min_y ? rtRegion.min_y : MianshangArray[j].min_y;
			double tmpMaxY = rtRegion.max_y > MianshangArray[j].max_y ? rtRegion.max_y : MianshangArray[j].max_y;

			if (m_vnMemeryPos.size() == 0)
			{
				m_vnMemeryPos.push_back(MianshangArray[nIndex].nMemeryPos);
			}
			m_vnMemeryPos.push_back(MianshangArray[j].nMemeryPos);
			rtRegion.min_x = tmpMinX;
			rtRegion.max_x = tmpMaxX;
			rtRegion.min_y = tmpMinY;
			rtRegion.max_y = tmpMaxY;

			MianshangArray[j].bCalculated = TRUE;

			QString strFileName = QObject::tr("AreaLog.txt");
			QString str;
			str = QString("%1\t%2\t%3\t%4\t%5\t%6\t") 
					.arg(MianshangArray[j].area, 0, 'f', 2)
					.arg(MianshangArray[j].min_x, 0, 'f', 2)
					.arg(MianshangArray[j].max_x, 0, 'f', 2)
					.arg(MianshangArray[j].min_y, 0, 'f', 2)
					.arg(MianshangArray[j].max_y, 0, 'f', 2)
					.arg(MianshangArray[j].z, 0, 'f', 2);
			// 查询数组内可以和它合并面积的伤
			FindNearArea(MianshangArray, j,dArea, rtRegion);
		}
	}
}

void CBiaoZhun_GBT2970::initFromBuf4()
{
	return;
}

void CBiaoZhun_GBT2970::SetMaxVector()
{
	return;
}

void CBiaoZhun_GBT2970::GetResultValues()
{
	double m_dBodyMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetBodyMaxShangZhiShiLength()").toDouble();

	double m_dBodyMaxFlawZhiShiArea = g_pPlateDraw->dynamicCall("GetBodyMaxQueXianArea()").toDouble() / 100.0f;	// cm^2
	double m_dEdgeMaxFlawZhiShiArea = g_pPlateDraw->dynamicCall("GetEdgeMaxQueXianArea").toDouble() / 100.0f;	// cm^2

	double dmin_x = 0.0f;
	double dmax_x = 0.0f;
	double dmin_y = 0.0f;
	double dmax_y = 0.0f;
	double dArea = 0.0f;
	double m_dEdgeMaxFlawZhiShiLength = 0;
/*	double m_dEdgeMaxFlawZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLengthX(double&,double&,double&,double&,double&)",dmin_x,dmax_x,dmin_y,dmax_y,dArea).toDouble();*/


	QByteArray temp = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLengthXQt()").toByteArray();  //6个double
	//chenyuzhu add start on 2014-10-31
	if (temp.size() != 0)
	{
		int tempSize = temp.size();
		char* tempBuf = temp.data();
		char* tempBuf2;
		tempBuf2 = new char[tempSize];
		memcpy(tempBuf2,tempBuf,tempSize);

		int tempPos = 0;
		memcpy(&(m_dEdgeMaxFlawZhiShiLength),tempBuf2,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmin_x),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmax_x),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmin_y),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dmax_y),tempBuf2+tempPos,sizeof(double));
		tempPos += sizeof(double);
		memcpy(&(dArea),tempBuf2+tempPos,sizeof(double));
	}
	//chenyuzhu modify end on 2014-10-31
	m_dAllMaxQueXianLv = g_pPlateDraw->dynamicCall("GetAllMaxQueXianLv()").toDouble() * 100;
}

QString CBiaoZhun_GBT2970::JudgeResult(const double& dRealZhiShiLength,
									   const double& dRealZhiShiArea,
									   const double& dRealQueXianLv,
									   const double& dRealEdgeZhiShiLength,
									   BOOL bRed[], const int& nSize)
{
	if(bRed == NULL)
	{
		return NULL;
	}

	int nZhiShiLength = 0;
	int nZhiShiArea = 0;
	int nMaxQueXianLv = 0;
	QString strResult;
	QString strLever[4] = {"I","II","III","IV"};
	UINT i = 0;
	for ( i = 0;i < 4;++i)
	{
		if (m_strJiBie.compare(strLever[i]) < 0)
		{
			break;
		}
	}

	for ( UINT j = 0;j < i;++j)
	{
		GetLevelValue(strLever[j], nZhiShiLength, nZhiShiArea, nMaxQueXianLv);

		if (dRealZhiShiLength >= nZhiShiLength)
		{
			bRed[0] = TRUE;
		}
		else
		{
			bRed[0] = FALSE;
		}

		if (dRealZhiShiArea >= nZhiShiArea)
		{
			bRed[1] = TRUE;
		}
		else
		{
			bRed[1] = FALSE;
		}

		if (dRealQueXianLv >= nMaxQueXianLv)
		{
			bRed[2] = TRUE;
		}
		else
		{
			bRed[2] = FALSE;
		}

		if(dRealEdgeZhiShiLength >= 50.0f)
		{
			bRed[3] = TRUE;
		}
		else
		{
			bRed[3] = FALSE;
		}

		if ((dRealZhiShiLength >= nZhiShiLength) || 
			(dRealZhiShiArea >= nZhiShiArea) ||
			(dRealQueXianLv > nMaxQueXianLv))

		{
			strResult = QObject::tr("不合格");
		}
		else
		{
			strResult = QObject::tr("合格");
			g_strJiBie = strLever[j];
			m_strJiBie = strLever[j];
			break;
		}
	}
	return strResult;
}

void CBiaoZhun_GBT2970::GetBiaoZhunItemInfo(const StandarConditiondType& eType, QRect& RctInfo, QString& strInfo)
{
	QVector<ResultRectAndType>::iterator it;
	for (it = m_VectorResultInfoRect.begin(); it != m_VectorResultInfoRect.end(); it++)
	{
		if ((*it).type == eType)
		{
			// 查找矩形
			RctInfo = (*it).rect;

			// 查找对应值
			int nZhiShiLength = 0;
			int nZhiShiArea = 0;
			int nMaxQueXianLv = 0;
			GetLevelValue(m_strJiBie, nZhiShiLength, nZhiShiArea, nMaxQueXianLv);
			switch(eType)
			{
			case STANDAR_GBT2970_LENGTH:
				strInfo = QString("≤%1 mm").arg(nZhiShiLength);
				break;
			case STANDAR_GBT2970_AREA:
				strInfo = QString("≤%1 cm^2").arg(nZhiShiArea / 100);
				break;
			case STANDAR_GBT2970_DEFECTRATE:
				strInfo = QString("≤%1 %%").arg(nMaxQueXianLv);
				break;
			case STANDAR_GBT2970_EDGEMAXFLAW:
				strInfo = QObject::tr("≤50 mm");
				break;
			default:
				break;
			}

			break;
		}
	}
}

void CBiaoZhun_GBT2970::InitBiaoZhunItemRect(const QRect& rctResult)
{
	int nPerX = rctResult.width()/4;
	int nperY = rctResult.height()/4;

	for (int i=0; i<4; i++)
	{
		QRect rectTemp;
		rectTemp.setLeft(rctResult.left() + i*nPerX);
		rectTemp.setRight(rctResult.left() + (i+1)*nPerX);
		rectTemp.setTop(rctResult.top() + 3*nperY);
		rectTemp.setBottom(rctResult.bottom());
		ResultRectAndType rectAndtype;
		rectAndtype.rect = rectTemp;
		switch (i)
		{
		case 0 :
			rectAndtype.type = STANDAR_GBT2970_LENGTH;
			break;
		case 1:
			rectAndtype.type = STANDAR_GBT2970_AREA;
			break;
		case 2:
			rectAndtype.type = STANDAR_GBT2970_DEFECTRATE;
			break;
		case 3:
			rectAndtype.type = STANDAR_GBT2970_EDGEMAXFLAW;
			break;
		}
		m_VectorResultRectAndType.push_back(rectAndtype);	

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
	}
}
