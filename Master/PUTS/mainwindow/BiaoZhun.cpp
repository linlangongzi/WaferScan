#include <QAxWidget>
#include <QSqlDatabase>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "DataDefine.h"
#include "BiaoZhun.h"

extern QAxWidget* g_pPlateDraw;
extern PLATE_INFO g_Info[1];
extern int nPlate;
extern QSqlDatabase* g_sqlDB;

CBiaoZhun::CBiaoZhun()
{	
	m_VectorResultRectAndType.clear();
	m_VectorResultInfoRect.clear();
	m_bReCalc = FALSE;
	m_dtmpEdgeZhishiLength = 0.0f;
	m_dtmpEdgeMinX = 0.0f;
	m_dtmpEdgeMaxX = 0.0f;
	m_dtmpEdgeMinY = 0.0f;
	m_dtmpEdgeMaxY = 0.0f;
	m_dtmpEdgeArea = 0.0f;
	m_fBiaoZhunZhiShiLength = 0;
	m_fBiaoZhunBodyZhiShiLength = 0;
	m_fBiaoZhunEdgeZhiShiLength = 0;

	m_fBiaoZhunZhiShiArea = 0;
	m_fBiaoZhunBodyZhiShiArea = 0;
	m_fBiaoZhunEdgeZhiShiArea = 0;

	m_fBiaoZhunQueXianNum = 0;
	m_fBiaoZhunBodyQueXianNum = 0;
	m_fBiaoZhunEdgeQueXianNum = 0;

	m_fBiaoZhunQueXianLv = 0;
	m_fBiaoZhunBodyGeneralQueXianLv = 0;
	m_fBiaoZhunBodyMaxQueXianLv = 0;
	m_fBiaoZhunEdgeMaxQueXianLv = 0;

	m_fBiaoZhunBodyMinArea = 0;
	m_fBiaoZhunBodyMaxArea = 0;
}

CBiaoZhun::~CBiaoZhun()
{

}

DetailInfo CBiaoZhun::GetDetailInfo(const QString& strBiaoZhun)
{
	DetailInfo info;

	bool bAllMaxQueXianLv = FALSE;
	bool bBodyAvgQueXianLv = FALSE;
	bool bBodyMaxQueXianLv = FALSE;
	bool bEdgeAvgQueXianLv = FALSE;
	bool bEdgeMaxQueXianLv = FALSE;
	bool bBodyMaxShangCountPerSquareMeter = FALSE;
	bool bEdgeMaxShangCountPerMeter = FALSE;
	bool bAllMaxShangCountPerSquareMeter = FALSE;
	bool bBodyMaxShangZhiShiLength = FALSE;
	bool bBodyMaxLength = FALSE;
	bool bBodyMaxWidth = FALSE;
	bool bBodyMaxQueXianArea = FALSE;
	bool bEdgeMaxShangZhiShiLength = FALSE;
	bool bEdgeMaxQueXianArea = FALSE;
	bool bEdgeMaxShangLength = FALSE;
	bool bEdgeMaxShangWidth = FALSE;

	QString strSQL;
	strSQL = QString("select * from t_PlatePrintSet where name = '%1'").arg(strBiaoZhun);
	QSqlQuery query(*g_sqlDB);
	query.exec(strSQL);
	query.next();

	// 获取字段名
	bBodyMaxShangZhiShiLength = query.value(33).toBool();
	bBodyMaxQueXianArea = query.value(34).toBool();
	bBodyMaxLength = query.value(35).toBool();
	bBodyMaxWidth = query.value(36).toBool();
	bEdgeMaxQueXianArea = query.value(37).toBool();
	bEdgeMaxShangZhiShiLength = query.value(38).toBool();
	bEdgeMaxShangLength = query.value(39).toBool();
	bEdgeMaxShangWidth = query.value(40).toBool();
	bBodyAvgQueXianLv = query.value(41).toBool();
	bEdgeAvgQueXianLv = query.value(42).toBool();
	bBodyMaxQueXianLv = query.value(43).toBool();
	bEdgeMaxQueXianLv = query.value(44).toBool();
	bAllMaxQueXianLv = query.value(45).toBool();
	bBodyMaxShangCountPerSquareMeter = query.value(46).toBool();
	bEdgeMaxShangCountPerMeter = query.value(47).toBool();
	bAllMaxShangCountPerSquareMeter = query.value(48).toBool();

	if(bAllMaxQueXianLv)
	{
		info.m_AllMaxQueXianLv = g_pPlateDraw->dynamicCall("GetAllMaxQueXianLv()").toDouble();
	}
	if(bBodyAvgQueXianLv)
	{
		info.m_BodyAvgQueXianLv = g_pPlateDraw->dynamicCall("GetBodyAvgQueXianLv()").toDouble();
	}
	if(bBodyMaxQueXianLv)
	{
		info.m_BodyMaxQueXianLv = g_pPlateDraw->dynamicCall("GetBodyMaxQueXianLv()").toDouble();
	}
	if(bEdgeAvgQueXianLv)
	{
		info.m_EdgeAvgQueXianLv = g_pPlateDraw->dynamicCall("GetEdgeAvgQueXianLv()").toDouble();
	}
	if(bEdgeMaxQueXianLv)
	{
		info.m_EdgeMaxQueXianLv = g_pPlateDraw->dynamicCall("GetEdgeMaxQueXianLv()").toDouble();
	}
	if(bBodyMaxShangCountPerSquareMeter)
	{
		info.m_perBodyMaxQueXianCount = g_pPlateDraw->dynamicCall("GetBodyMaxShangCountPerSquareMeter()").toDouble();
	}
	if(bEdgeMaxShangCountPerMeter)
	{
		info.m_perEdgeMaxQueXianCount = g_pPlateDraw->dynamicCall("GetEdgeMaxShangCountPerMeter()").toDouble();
	}
	if(bAllMaxShangCountPerSquareMeter)
	{
		info.m_perAllMaxQueXianCount = g_pPlateDraw->dynamicCall("GetAllMaxShangCountPerSquareMeter()").toDouble();
	}
	if(bBodyMaxShangZhiShiLength)
	{
		info.m_BodyMaxZhiShiLength = g_pPlateDraw->dynamicCall("GetBodyMaxShangZhiShiLength()").toDouble();
	}
	if(bBodyMaxLength)
	{
		info.m_BodyMaxLength = g_pPlateDraw->dynamicCall("GetBodyMaxLength()").toDouble();
	}
	if(bBodyMaxWidth)
	{
		info.m_BodyMaxWidth = g_pPlateDraw->dynamicCall("GetBodyMaxWidth()").toDouble();
	}
	if(bBodyMaxQueXianArea)
	{
		info.m_BodyMaxArea = g_pPlateDraw->dynamicCall("GetBodyMaxQueXianArea()").toDouble();
	}
	if(bEdgeMaxShangZhiShiLength)
	{
		info.m_EdgeMaxZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLength()").toDouble();
	}
	if(bEdgeMaxQueXianArea)
	{
		info.m_EdgeMaxArea = g_pPlateDraw->dynamicCall("GetEdgeMaxQueXianArea()").toDouble();
	}
	if(bEdgeMaxShangLength)
	{
		info.m_EdgeMaxLength = g_pPlateDraw->dynamicCall("GetEdgeMaxLength()").toDouble();
	}
	if(bEdgeMaxShangWidth)
	{
		info.m_EdgeMaxWidth = g_pPlateDraw->dynamicCall("GetEdgeMaxWidth()").toDouble();
	}
	return info;
}
