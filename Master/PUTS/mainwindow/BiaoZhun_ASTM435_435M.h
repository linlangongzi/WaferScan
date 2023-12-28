#ifndef _BIAOZHUN_ASTM435_H
#define _BIAOZHUN_ASTM435_H

#include <QRect>
#include <QString>
#include "DataDefine.h"

class CBiaoZhun_ASTM435_435M : public CBiaoZhun  
{
public:
	CBiaoZhun_ASTM435_435M();
	virtual ~CBiaoZhun_ASTM435_435M();
public:
	void DrawScanResult(QPainter* painter, const QRect& rect);
	QString GetResult();
	void CalcZhishiArea(double& dbZhiShiArea, ZhiShiAreaRegion& ZhiShiRegion);
	QVector<ZhiShiAreaRegionAndType> GetDrawSquarePosList();
	QVector<ZhiShiAreaRegion> GetMaxVector(StandarConditiondType eType);  
	BOOL UpdateLengthDrawColor(PlaneFlawHeader& PFH);  
	void initArrayFromBuf4();  
	void CalcZhishiLength(double& dbZhiShiLength, ZhiShiAreaRegion& ZhiShiRegion);  
	void initFromBuf4(); 
	void SetMaxVector(); 
	void GetResultValues();
	// 获取合并后评判结果
	QString GetResult2(double dMaxLengthValue, double dMaxFlawZhiShiArea);//chenyuzhu add on 2014-7-24

private:
	void SetParameter();
	QVector<PlaneFlawHeader2> m_MianshangArray;  
	int m_nZhiShiLength; 
	double m_dDivLengthValue;   
	double m_dMaxLengthValue;   
	QVector<ZhiShiAreaRegion> m_vMaxLength;
	QVector<ZhiShiAreaRegionAndType> m_vDrawSquareList;
	QVector<ZhiShiAreaRegion> m_vecMaxLength;  
	QVector<int> m_vnMemeryPos;  
	double m_dBodyMaxFlawZhiShiLength;   
	double m_dEdgeMaxFlawZhiShiLength;   
public:
	void GetBiaoZhunItemInfo(const StandarConditiondType& eType, QRect& RctInfo, QString& strInfo);	
	void InitBiaoZhunItemRect(const QRect& rctResult);
};

#endif