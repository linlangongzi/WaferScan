#ifndef _BIAOZHUN_H
#define _BIAOZHUN_H

#include <QRect>
#include <QPainter>
#include <QString>
#include "DataDefine.h"

class CBiaoZhun  
{
public:
	CBiaoZhun();
	virtual ~CBiaoZhun();
public:
	// 显示计算结果
	virtual void DrawScanResult(QPainter* painter, const QRect& rect) = 0;	
	// 获取评判结果
	virtual QString GetResult() = 0;
	
	// 获取合并后评判结果
	virtual QString GetResult2(double dMaxLengthValue, double dMaxFlawZhiShiArea) = 0;//chenyuzhu add on 2014-7-24

	// 根据标准计算最大指示面积
	virtual void CalcZhishiArea(double& dbZhiShiArea, ZhiShiAreaRegion& ZhiShiRegion) = 0;

	// 输出计算结果的详细信息
	DetailInfo GetDetailInfo(const QString& strBiaoZhun);

	//评判伤是否符合标准
	//取得需要画虚框的伤点坐标列表
	virtual QVector<ZhiShiAreaRegionAndType> GetDrawSquarePosList() = 0;
protected:	
	// 设定参数
	virtual void SetParameter() = 0;
public:
	QString m_strJiBie;
	bool m_bReCalc;		// 是否重新运算标志
	QRect m_rectResultPos;

	QVector<ResultRectAndType> m_VectorResultRectAndType; 
	QVector<ResultRectAndType> m_VectorResultInfoRect;
	QVector<ResultRectAndType> m_VectorResultItemRect;
	virtual QVector<ZhiShiAreaRegion> GetMaxVector(StandarConditiondType eType) = 0;
	virtual void initFromBuf4() = 0;
	virtual void SetMaxVector() = 0;
	virtual void GetResultValues() = 0;
	double m_dtmpEdgeZhishiLength;
	double m_dtmpEdgeMinX;
	double m_dtmpEdgeMaxX;
	double m_dtmpEdgeMinY;
	double m_dtmpEdgeMaxY;
	double m_dtmpEdgeArea;
	float m_fBiaoZhunZhiShiLength;
	float m_fBiaoZhunBodyZhiShiLength;
	float m_fBiaoZhunEdgeZhiShiLength;

	float m_fBiaoZhunZhiShiArea;
	float m_fBiaoZhunBodyZhiShiArea;
	float m_fBiaoZhunEdgeZhiShiArea;

	float m_fBiaoZhunQueXianNum;
	float m_fBiaoZhunBodyQueXianNum;
	float m_fBiaoZhunEdgeQueXianNum;

	float m_fBiaoZhunQueXianLv;
	float m_fBiaoZhunBodyGeneralQueXianLv;
	float m_fBiaoZhunBodyMaxQueXianLv;
	float m_fBiaoZhunEdgeMaxQueXianLv;

	float m_fBiaoZhunBodyMinArea;
	float m_fBiaoZhunBodyMaxArea;
	// 获取标准项对应的标准值
	virtual void GetBiaoZhunItemInfo(const StandarConditiondType& eType, QRect& RctInfo, QString& strInfo) = 0;
	virtual void InitBiaoZhunItemRect(const QRect& rctResult) = 0;	// 初始化结果矩形区域
};

#endif // _BIAOZHUN_H
