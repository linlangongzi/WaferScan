/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 报表
* Filename: CCustumPlateTablePreview.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-16
*/
#ifndef CCUSTUMPLATETABLEPREVIEW_H
#define CCUSTUMPLATETABLEPREVIEW_H

#include <QWidget>
class QString;
class QFont;

class CCustumPlateTablePreview :public QWidget
{
public:
	CCustumPlateTablePreview(void);
	~CCustumPlateTablePreview(void);
public:
	void InitTitleMap();// 初始化名称和数据库字段的映射
	void InitProperities(); 	// 初始化相关参数
	void InitTableItem(QString strFileName); // 读数据库获取显示表项
	void UpdateTitleMap();
public:
	QMap<QString,QString> m_titleMap;
	QMap<QString,QString> m_titleMapReverse;
	QMap<QString,int> m_titleToNumber;
	QFont m_PrintFont;
	int m_iTinggi;	// 显示的长度
	int m_iLebar;	// 显示的宽度
	int m_iPrnX;	// 实际打印的长
	int m_iPrnY;	// 实际打印的宽
	QRect m_RectMemory;		// 打印的实际范围
	int m_ntextHeight;
	int m_nCharsPerLine;
	int m_nLinesPerPage;
	int m_iTinggi_100;	// 显示的长度100%
	int m_iLebar_100;	// 显示的宽度100%
	int m_nViewWidth_100;
	int m_nViewHeight_100;
	int m_nHScrollPos;	// 水平滚动条位置
	int m_nVScrollPos;	// 垂直滚动条位置
	int m_nHPageSize;	// 水平Page大小
	int m_nVPageSize;	// 垂直Page大小
	QFont m_Titlefont;
	QFont m_SectionFont;
	QVector<QString> m_plateArray; 	   // 钢板基本信息组要显示的字符串集合
	QVector<QString> m_TanShangArray;  // 探伤基本信息组要显示的字符串集合
	QVector<QString> m_ResultArray;    // 探伤结果组要显示的字符串集合
	QString m_strTitle;	               // 表的标题
	int m_nPlateLength;
	int m_nPlateWidth;
	int m_nPlateHeight;

};

#endif  //CCUSTUMPLATETABLEPREVIEW_H
