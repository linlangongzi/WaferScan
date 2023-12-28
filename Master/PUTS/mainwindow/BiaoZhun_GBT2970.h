#ifndef _BIAOZHUN_GBT2970_H
#define _BIAOZHUN_GBT2970_H

#include <QRect>
#include <QString>
#include "DataDefine.h"

struct PlaneFlawHeader2;

class CBiaoZhun_GBT2970 : public CBiaoZhun
{
public:
	CBiaoZhun_GBT2970();
	CBiaoZhun_GBT2970(const QString& strJiBie);
	virtual ~CBiaoZhun_GBT2970();
public:
	void DrawScanResult(QPainter* painter, const QRect& rect);
	QString GetResult();
	void CalcZhishiArea(double& dbZhiShiArea, ZhiShiAreaRegion& ZhiShiRegion);
	void CalcZhishiLength(double& dbZhiShiLength, ZhiShiAreaRegion& ZhiShiRegion);
	BOOL IsEdgeDetect(PlaneFlawHeader& PFH);
	BOOL UpdateAreaDrawColor(PlaneFlawHeader& PFH);
	BOOL UpdateLengthDrawColor(PlaneFlawHeader& PFH);
	QVector<ZhiShiAreaRegionAndType> GetDrawSquarePosList();
	QVector<ZhiShiAreaRegion> GetMaxVector(StandarConditiondType eType);
	void initFromBuf4();
	void SetMaxVector();
	void GetResultValues();	
	QString GetResult2(double dMaxLengthValue, double dMaxFlawZhiShiArea);//chenyuzhu add on 2014-7-24
private:
	void SetParameter(const QString& strJiBie);
	void SetParameter();
	QString JudgeResult(const double& dRealZhiShiLength,
		const double& dRealZhiShiArea,
		const double& dRealQueXianLv);
	void GetLevelValue(const QString& strLevel,
		int& nMaxZhiShiLength,
		int& nMaxZhiShiArea,
		int& nMaxQueXianLv);

	void FindNearArea(QVector<PlaneFlawHeader2>& MianshangArray, 
		int nIndex, double& dArea, ZhiShiAreaRegion& rtRegion);
	void FindNearLength(QVector<PlaneFlawHeader2>& MianshangArray, 
		int nIndex, double& dLength, ZhiShiAreaRegion& rtRegion);

	void initArrayFromBuf4(QString csflag);
public:
	void GetBiaoZhunItemInfo(const StandarConditiondType& eType, QRect& RctInfo, QString& strInfo);
	void InitBiaoZhunItemRect(const QRect& rctResult);
private:
	QString m_strJiBie;
	double m_dPreDistance;
	double m_dPreArea;

	int m_nZhiShiLength;
	double m_dDivLengthValue;  

	int m_nZhiShiArea;
	int m_nDivAreaValue;  
public:
	double m_dEdgeMaxLength;
	double m_dMaxLengthValue;
private:
	double m_dAllMaxQueXianLv;	// 缺陷率 	
	QVector<ZhiShiAreaRegion> m_vMaxLength;//存储指示长度最大的伤点坐标
	QVector<ZhiShiAreaRegionAndType> m_vDrawSquareList;//存储需要画虚框面伤伤点坐标
	QVector<int> m_vnMemeryPos;
 	QVector<PlaneFlawHeader2> m_MianshangArray;
	QVector<ZhiShiAreaRegion> m_vecMaxArea;
	QVector<ZhiShiAreaRegion> m_vecMaxLength;
	QVector<ZhiShiAreaRegion> m_vecMaxDefectRate;
	QVector<ZhiShiAreaRegion> m_vecMaxEdgeLength;
	QString JudgeResult(const double& dRealZhiShiLength,
		const double& dRealZhiShiArea,
		const double& dRealQueXianLv,
		const double& dRealEdgeZhiShiLength,
		BOOL bRed[], const int& nSize);
};

#endif // _BIAOZHUN_GBT2970_H
