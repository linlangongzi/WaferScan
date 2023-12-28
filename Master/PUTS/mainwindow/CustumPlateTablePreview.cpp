/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 通道切换面板
* Filename: CCustumPlateTablePreview.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-16
*/
#include <QMap>
#include <QString>
#include <QPrinter>
#include <QPainter>
#include <QFont>
#include <QVector>
#include <QVariant>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include "CustumPlateTablePreview.h"
extern QSqlDatabase *g_sqlDB;


CCustumPlateTablePreview::CCustumPlateTablePreview(void)
{
	InitTitleMap();
//	UpdateTitleMap();
	m_iTinggi_100 = 0;
	m_iLebar_100 = 0;
	m_iTinggi = 0;
	m_iLebar = 0;
	m_nViewWidth_100 = 0;
	m_nViewHeight_100 = 0;
// 	m_ntitleFontHeight = 0;
// 	m_nSectionFontHeight = 0;
	m_ntextHeight = 0;
//	m_nRecordNum = 0;
	// Song Chenguang add end on 2009-05-05
//	m_bCalRecordNum = FALSE;
	// 	::ZeroMemory(&m_prtInfo, sizeof(CPrintInfo));
//	m_nCurPage = 0;
//	m_nMaxPage = 0;

	m_strTitle = tr("中厚板自动探伤报表--钢板报表");
//	m_nPlateLength = 0;
//	m_nPlateWidth = 0;
//	m_nPlateHeight = 0;



}

CCustumPlateTablePreview::~CCustumPlateTablePreview(void)
{

}

void CCustumPlateTablePreview::InitTitleMap()    // 初始化名称和数据库字段的映射
{
	m_titleMap.clear();
	// 钢板基本信息
	m_titleMap[QString::fromLocal8Bit("report_id")] =QString::fromLocal8Bit("报表流水号");
	m_titleMap[QString::fromLocal8Bit("check_id")] =QString::fromLocal8Bit("探伤流水号");
	m_titleMap[QString::fromLocal8Bit("plate_id")] = QString::fromLocal8Bit("钢板号");
	m_titleMap[QString::fromLocal8Bit("lu_id")] =QString::fromLocal8Bit("炉号");
	m_titleMap[QString::fromLocal8Bit("muban_id")] =QString::fromLocal8Bit("母板号（钢坯号）");
	m_titleMap[QString::fromLocal8Bit("platetype")] =QString::fromLocal8Bit("钢种");
	m_titleMap[QString::fromLocal8Bit("shengchanhao")] =QString::fromLocal8Bit("生产号（批号）");
	m_titleMap[QString::fromLocal8Bit("hetonghao")]=QString::fromLocal8Bit("合同号");
	m_titleMap[QString::fromLocal8Bit("gangji")]=QString::fromLocal8Bit("钢级");
	m_titleMap[QString::fromLocal8Bit("plate_length")]=QString::fromLocal8Bit("长度(mm)");
	m_titleMap[QString::fromLocal8Bit("plate_width")]=QString::fromLocal8Bit("宽度(mm)");
	m_titleMap[QString::fromLocal8Bit("plate_thick")]=QString::fromLocal8Bit("厚度(mm)");
	m_titleMap[QString::fromLocal8Bit("plate_weight")]=QString::fromLocal8Bit("重量(吨)");
	m_titleMap[QString::fromLocal8Bit("jiaohuostatus")]=QString::fromLocal8Bit("交货状态");
	// 探伤基本信息            
	m_titleMap[QString::fromLocal8Bit("yiqi")]=QString::fromLocal8Bit("仪器型号");
	m_titleMap[QString::fromLocal8Bit("detector")]=QString::fromLocal8Bit("探头型号");
	m_titleMap[QString::fromLocal8Bit("tanshangmethod")]=QString::fromLocal8Bit("探伤方法");
	m_titleMap[QString::fromLocal8Bit("tanshangbiaozhun")]=QString::fromLocal8Bit("探伤标准");
	m_titleMap[QString::fromLocal8Bit("tanshangtime")]=QString::fromLocal8Bit("检测时间");
	m_titleMap[QString::fromLocal8Bit("tanshangclass")]=QString::fromLocal8Bit("检测班次");
	m_titleMap[QString::fromLocal8Bit("tanshangrenyuan")]=QString::fromLocal8Bit("探伤人员");
	m_titleMap[QString::fromLocal8Bit("tanshanglinmindu")]=QString::fromLocal8Bit("探伤灵敏度");
	m_titleMap[QString::fromLocal8Bit("tanshangjibie")]=QString::fromLocal8Bit("探伤级别");
	m_titleMap[QString::fromLocal8Bit("ouheji")]=QString::fromLocal8Bit("耦合剂");
	m_titleMap[QString::fromLocal8Bit("name")]=QString::fromLocal8Bit("探伤模式");

	// 探伤结果
	m_titleMap[QString::fromLocal8Bit("result")]=QString::fromLocal8Bit("探伤结果");
	m_titleMap[QString::fromLocal8Bit("quexiansum")]=QString::fromLocal8Bit("缺陷总数");
	m_titleMap[QString::fromLocal8Bit("quexiansumarea")]=QString::fromLocal8Bit("缺陷总面积(cm^2)");
	m_titleMap[QString::fromLocal8Bit("quexianzhanjilv")]=QString::fromLocal8Bit("缺陷占积率(%)");
	m_titleMap[QString::fromLocal8Bit("quexianmaxlength")]=QString::fromLocal8Bit("指示长度(mm)");

	m_titleMap[QString::fromLocal8Bit("quexianmaxwidth")]=QString::fromLocal8Bit("缺陷最大指示宽度(mm)");
	m_titleMap[QString::fromLocal8Bit("QueXianMaxArea")]=QString::fromLocal8Bit("指示面积(cm^2)");
	m_titleMap[QString::fromLocal8Bit("quexiandetail")]=QString::fromLocal8Bit("缺陷详细信息");
	m_titleMap[QString::fromLocal8Bit("AllMaxQueXianLv")] = QString::fromLocal8Bit("缺陷率(%)");
	//////////////////////////////////////////////////////////////////////////
	m_titleMap[QString::fromLocal8Bit("BodyMaxZhiShiLength")]=QString::fromLocal8Bit("本体最大指示长度(mm)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxQueXianArea")]=QString::fromLocal8Bit("本体最大缺陷面积(mm^2)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxLength")]=QString::fromLocal8Bit("本体最大长度(mm)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxWidth")]=QString::fromLocal8Bit("本体最大宽度(mm)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxZhiShiLength")]=QString::fromLocal8Bit("边界长度(mm)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxQueXianArea")]=QString::fromLocal8Bit("边缘最大缺陷面积(mm^2)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxLength")]=QString::fromLocal8Bit("边缘最大长度(mm)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxWidth")]=QString::fromLocal8Bit("边缘最大宽度(mm)");
	m_titleMap[QString::fromLocal8Bit("BodyAvgQueXianLv")]=QString::fromLocal8Bit("本体平均缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("EdgeAvgQueXianLv")]=QString::fromLocal8Bit("边缘平均缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxQueXianLv")]=QString::fromLocal8Bit("本体最大缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxQueXianLv")]=QString::fromLocal8Bit("边缘最大缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxCountSquarMeter")]=QString::fromLocal8Bit("本体缺陷数/m^2");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxCountPerMeter")]=QString::fromLocal8Bit("边缘缺陷数/m");
	m_titleMap[QString::fromLocal8Bit("AllMaxCountSquarMeter")]=QString::fromLocal8Bit("缺陷数/m^2");
	//////////////////////////////////////////////////////////////////////////
	m_titleMap[QString::fromLocal8Bit("info")]=QString::fromLocal8Bit("备注");
	// 统计
	m_titleMap[QString::fromLocal8Bit("lu_id2")]=QString::fromLocal8Bit("炉号");
	m_titleMap[QString::fromLocal8Bit("gangzhong2")]=QString::fromLocal8Bit("钢种");
	m_titleMap[QString::fromLocal8Bit("guige")]=QString::fromLocal8Bit("规格");
	m_titleMap[QString::fromLocal8Bit("tanshangjibie2")]=QString::fromLocal8Bit("探伤级别");
	m_titleMap[QString::fromLocal8Bit("tanshangbiaozhun2")]=QString::fromLocal8Bit("探伤标准");
	m_titleMap[QString::fromLocal8Bit("okcount")]=QString::fromLocal8Bit("合格块数");
	m_titleMap[QString::fromLocal8Bit("tanshangrenyuan2")]=QString::fromLocal8Bit("探伤人员");

	m_titleMapReverse.clear();
	// 钢板基本信息
	m_titleMapReverse[QString::fromLocal8Bit("报表流水号")] =QString::fromLocal8Bit("");
	m_titleMapReverse[QString::fromLocal8Bit("探伤流水号")] =QString::fromLocal8Bit("id");
	m_titleMapReverse[QString::fromLocal8Bit("钢板号")] = QString::fromLocal8Bit("name");
	m_titleMapReverse[QString::fromLocal8Bit("炉号")] =QString::fromLocal8Bit("Lu");
	m_titleMapReverse[QString::fromLocal8Bit("母板号（钢坯号）")] =QString::fromLocal8Bit("MuBan");
	m_titleMapReverse[QString::fromLocal8Bit("钢种")] =QString::fromLocal8Bit("GangZhong");
	m_titleMapReverse[QString::fromLocal8Bit("生产号（批号）")] =QString::fromLocal8Bit("ShenChan");
	m_titleMapReverse[QString::fromLocal8Bit("合同号")]=QString::fromLocal8Bit("HeTong");

	m_titleMapReverse[QString::fromLocal8Bit("长度(mm)")]=QString::fromLocal8Bit("Length");
	m_titleMapReverse[QString::fromLocal8Bit("宽度(mm)")]=QString::fromLocal8Bit("Width");
	m_titleMapReverse[QString::fromLocal8Bit("厚度(mm)")]=QString::fromLocal8Bit("Thick");
	m_titleMapReverse[QString::fromLocal8Bit("重量(吨)")]=QString::fromLocal8Bit("Weight");
	m_titleMapReverse[QString::fromLocal8Bit("交货状态")]=QString::fromLocal8Bit("JiaoHuoStatus");
	// 探伤基本信息            
	m_titleMapReverse[QString::fromLocal8Bit("仪器型号")]=QString::fromLocal8Bit("YiQiXingHao");
	m_titleMapReverse[QString::fromLocal8Bit("探头型号")]=QString::fromLocal8Bit("Detector");
	m_titleMapReverse[QString::fromLocal8Bit("探伤方法")]=QString::fromLocal8Bit("TanShangMethod");
	m_titleMapReverse[QString::fromLocal8Bit("探伤标准")]=QString::fromLocal8Bit("TanShangBiaoZhun");
	m_titleMapReverse[QString::fromLocal8Bit("检测时间")]=QString::fromLocal8Bit("CheckTime");
	m_titleMapReverse[QString::fromLocal8Bit("检测班次")]=QString::fromLocal8Bit("CheckClass");
	m_titleMapReverse[QString::fromLocal8Bit("探伤人员")]=QString::fromLocal8Bit("CheckName");
	m_titleMapReverse[QString::fromLocal8Bit("探伤灵敏度")]=QString::fromLocal8Bit("TanShangLinMingDu");
	m_titleMapReverse[QString::fromLocal8Bit("探伤级别")]=QString::fromLocal8Bit("TanShangJiBieBody");
	m_titleMapReverse[QString::fromLocal8Bit("耦合剂")]=QString::fromLocal8Bit("OuHeJi");
	m_titleMapReverse[QString::fromLocal8Bit("探伤模式")]=QString::fromLocal8Bit("TestMode");
	// 探伤结果
	m_titleMapReverse[QString::fromLocal8Bit("探伤结果")]=QString::fromLocal8Bit("Result");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷总数")]=QString::fromLocal8Bit("MianShangCount");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷总面积(mm^2)")]=QString::fromLocal8Bit("MianShangAllMianJi");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷占积率(%)")]=QString::fromLocal8Bit("QueXianPercent");
	m_titleMapReverse[QString::fromLocal8Bit("指示长度(mm)")]=QString::fromLocal8Bit("QueXianMaxLength");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷最大指示宽度(mm)")]=QString::fromLocal8Bit("QueXianMaxWidth");
	m_titleMapReverse[QString::fromLocal8Bit("指示面积(cm^2)")]=QString::fromLocal8Bit("QueXianMaxArea");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷率(%)")]=QString::fromLocal8Bit("AllMaxQueXianLv");

	m_titleMapReverse[QString::fromLocal8Bit("本体最大指示长度(mm)")]=QString::fromLocal8Bit("BodyMaxZhiShiLength");
	m_titleMapReverse[QString::fromLocal8Bit("本体最大缺陷面积(mm^2)")]=QString::fromLocal8Bit("BodyMaxQueXianArea");
	m_titleMapReverse[QString::fromLocal8Bit("本体最大长度(mm)")]=QString::fromLocal8Bit("BodyMaxLength");
	m_titleMapReverse[QString::fromLocal8Bit("本体最大宽度(mm)")]=QString::fromLocal8Bit("BodyMaxWidth");
	m_titleMapReverse[QString::fromLocal8Bit("边界长度(mm)")]=QString::fromLocal8Bit("EdgeMaxZhiShiLength");
	m_titleMapReverse[QString::fromLocal8Bit("边缘最大缺陷面积(mm^2)")]=QString::fromLocal8Bit("EdgeMaxArea");
	m_titleMapReverse[QString::fromLocal8Bit("边缘最大长度(mm)")]=QString::fromLocal8Bit("EdgeMaxLength");
	m_titleMapReverse[QString::fromLocal8Bit("边缘最大宽度(mm)")]=QString::fromLocal8Bit("EdgeMaxWidth");
	m_titleMapReverse[QString::fromLocal8Bit("本体平均缺陷率(%)")]=QString::fromLocal8Bit("BodyAvgQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("边缘平均缺陷率(%)")]=QString::fromLocal8Bit("EdgeAvgQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("本体最大缺陷率(%)")]=QString::fromLocal8Bit("BodyMaxQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("边缘最大缺陷率(%)")]=QString::fromLocal8Bit("EdgeMaxQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("本体缺陷数/m^2")]=QString::fromLocal8Bit("BodyMaxCountSquarMeter");
	m_titleMapReverse[QString::fromLocal8Bit("边缘缺陷数/m")]=QString::fromLocal8Bit("EdgeMaxCountPerMeter");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷数/m^2")]=QString::fromLocal8Bit("AllMaxCountSquarMeter");
	//////////////////////////////////////////////////////////////////////////
	m_titleMapReverse[QString::fromLocal8Bit("缺陷详细信息")]=QString::fromLocal8Bit("");
	m_titleMapReverse[QString::fromLocal8Bit("备注")]=QString::fromLocal8Bit("");
	//////////////////////////////////////////////////////////////////////////////////////////////

	m_titleToNumber.clear();
	//vwPlate_Stat视图的映射
	m_titleToNumber[QString::fromLocal8Bit("id")]=0;
	m_titleToNumber[QString::fromLocal8Bit("name")]=1;
	m_titleToNumber[QString::fromLocal8Bit("Lu")]=2;
	m_titleToNumber[QString::fromLocal8Bit("MuBan")]=3;
	m_titleToNumber[QString::fromLocal8Bit("ShenChan")]=4;
	m_titleToNumber[QString::fromLocal8Bit("HeTong")]=5;
	m_titleToNumber[QString::fromLocal8Bit("GangJi")]=6;
	m_titleToNumber[QString::fromLocal8Bit("JiaoHuoStatus")]=7;
	m_titleToNumber[QString::fromLocal8Bit("PlateType")]=8;
	m_titleToNumber[QString::fromLocal8Bit("Length")]=9;
	m_titleToNumber[QString::fromLocal8Bit("Width")]=10;
	m_titleToNumber[QString::fromLocal8Bit("Thick")]=11;
	m_titleToNumber[QString::fromLocal8Bit("Weight")]=12;
	m_titleToNumber[QString::fromLocal8Bit("Result")]=13;
	m_titleToNumber[QString::fromLocal8Bit("CheckTime")]=14;
	m_titleToNumber[QString::fromLocal8Bit("CheckClass")]=15;
	m_titleToNumber[QString::fromLocal8Bit("DianShangCount")]=16;
	m_titleToNumber[QString::fromLocal8Bit("MianShangCount")]=17;
	m_titleToNumber[QString::fromLocal8Bit("MianShangAllMianJi")]=18;
	m_titleToNumber[QString::fromLocal8Bit("QueXianPercent")]=19;
	m_titleToNumber[QString::fromLocal8Bit("QueXianMaxLength")]=20;
	m_titleToNumber[QString::fromLocal8Bit("QueXianMaxWidth")]=21;
	m_titleToNumber[QString::fromLocal8Bit("CheckName")]=22;
	m_titleToNumber[QString::fromLocal8Bit("TestMode")]=23;
	m_titleToNumber[QString::fromLocal8Bit("CheckMethod")]=24;
	m_titleToNumber[QString::fromLocal8Bit("YiQiXingHao")]=25;
	m_titleToNumber[QString::fromLocal8Bit("Detector")]=26;
	m_titleToNumber[QString::fromLocal8Bit("TanShangMethod")]=27;
	m_titleToNumber[QString::fromLocal8Bit("TanTouZhongXinJuLi")]=28;
	m_titleToNumber[QString::fromLocal8Bit("TanShangLinMingDu")]=29;
	m_titleToNumber[QString::fromLocal8Bit("SaoMiaoJianGe")]=30;
	m_titleToNumber[QString::fromLocal8Bit("OuHeJi")]=31;
	m_titleToNumber[QString::fromLocal8Bit("IsReEvaluate")]=32;
	m_titleToNumber[QString::fromLocal8Bit("TanShangJiBieBody")]=33;
	m_titleToNumber[QString::fromLocal8Bit("TanShangBiaoZhun")]=34;
	m_titleToNumber[QString::fromLocal8Bit("AllMaxQueXianLv")]=35;
	m_titleToNumber[QString::fromLocal8Bit("GangZhong")]=36;
	m_titleToNumber[QString::fromLocal8Bit("QueXianMaxArea")]=37;
	m_titleToNumber[QString::fromLocal8Bit("info")]=38;
	m_titleToNumber[QString::fromLocal8Bit("TanShangJiBieEdge")]=39;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxZhiShiLength")]=40;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxQueXianArea")]=41;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxLength")]=42;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxWidth")]=43;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxZhiShiLength")]=44;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxArea")]=45;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxLength")]=46;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxWidth")]=47;
	m_titleToNumber[QString::fromLocal8Bit("BodyAvgQueXianLv")]=48;
	m_titleToNumber[QString::fromLocal8Bit("EdgeAvgQueXianLv")]=49;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxQueXianLv")]=50;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxQueXianLv")]=51;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxCountSquarMeter")]=52;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxCountPerMeter")]=53;
	m_titleToNumber[QString::fromLocal8Bit("AllMaxCountSquarMeter")]=54;
	m_titleToNumber[QString::fromLocal8Bit("flag")]=55;
	m_titleToNumber[QString::fromLocal8Bit("CheckBodyLevel")]=56;
	m_titleToNumber[QString::fromLocal8Bit("CheckBiaoZhun")]=57;
	m_titleToNumber[QString::fromLocal8Bit("CheckEdgeLevel")]=58;
}

void CCustumPlateTablePreview::InitProperities()
{
	QPrinter printer;
	printer.setDocName(tr("This string will appear in Printer Queue"));
	printer.setPageSize(QPrinter::B5);
	printer.setOrientation(QPrinter::Landscape);
	printer.setPageMargins(30,30,30,30,QPrinter::DevicePixel);
	QPainter painter(&printer);
	m_PrintFont.setPointSize(9);
	m_PrintFont.setFamily("OCR A");
	painter.setFont(m_PrintFont);
	m_RectMemory = printer.pageRect();
	m_iPrnX = printer.pageRect().width();	// 实际打印的长(像素)
	m_iPrnY = printer.pageRect().height();	// 实际打印的宽(像素)
	m_ntextHeight = painter.fontMetrics().height()+painter.fontMetrics().leading();
	m_nCharsPerLine = m_iPrnX / painter.fontMetrics().width(tr("字"));		
	m_nLinesPerPage = m_iPrnY / painter.fontMetrics().lineSpacing();
	float fScale = (float)m_RectMemory.width()/(float)m_RectMemory.height();
	QRect rect = this->rect();  //对话框的矩形
	m_iTinggi_100 = int(rect.height()*0.9);
	m_iLebar_100 = int(m_iTinggi_100*fScale);
	m_iTinggi = m_iTinggi_100;
	m_iLebar = m_iLebar_100;
	m_nHScrollPos = m_nHPageSize = 0;
	m_nVScrollPos = m_nVPageSize = 0;
    m_Titlefont.setPointSize(360);
	m_Titlefont.setFamily("OCR A");
	m_SectionFont.setPointSize(150);
	m_SectionFont.setFamily("OCR A");

}

void CCustumPlateTablePreview::InitTableItem(QString strFileName) // 读数据库获取显示表项
{
 	m_plateArray.clear();
 	m_TanShangArray.clear();
 	m_ResultArray.clear();
	if (NULL == g_sqlDB)
	{
		QMessageBox::warning(this,"InitTableItem",tr("g_sqlDB未初始化"));
		return;
	}
	QString strSql = QString("%1%2%3").arg("select * from t_PlatePrintSet where name = '").arg(strFileName).arg("'");
	QSqlQuery query(*g_sqlDB);
	bool bRet = query.exec(strSql);
	// 获取字段名
	QStringList FieldList;
	int nCount = query.record().count();
	for(int i=2; i<nCount; i++)
	{
		FieldList.append(query.record().fieldName(i));
	}
	while (query.next())
	{
		// 钢板基本信息
		for(int j=0; j<14; j++)
		{
			bool Value = query.value(j+2).toBool();
			if(Value)
			{
				QString str;
				str = m_titleMap[FieldList.at(j)];
				if(!str.isEmpty() && !m_plateArray.contains(str))
				{
					m_plateArray.append(str);
				}
			}
		}

		// 探伤基本信息
		for(int j=14; j<24; j++)
		{
			bool Value = query.value(j+2).toBool();
			if(Value)
			{
				QString str = m_titleMap[FieldList.at(j)];
				if(!str.isEmpty() && !m_TanShangArray.contains(str))
				{
					m_TanShangArray.append(str);
				}
			}
		}

		// 探伤结果
		for(int j=24; j<48; j++)
		{
			bool Value = query.value(j+2).toBool();
			if(Value)
			{
				QString str = m_titleMap[FieldList.at(j)];
				if(!str.isEmpty() && !m_ResultArray.contains(str))
				{
					m_ResultArray.append(str);
				}
			}
		}
		m_strTitle = query.value(51).toString();
	}
}

void CCustumPlateTablePreview::UpdateTitleMap()    // 初始化名称和数据库字段的映射
{
	m_titleMap.clear();
	// 钢板基本信息
	m_titleMap[QString::fromLocal8Bit("report_id")] =QString::fromLocal8Bit("报表流水号");
	m_titleMap[QString::fromLocal8Bit("check_id")] =QString::fromLocal8Bit("探伤流水号");
	m_titleMap[QString::fromLocal8Bit("plate_id")] = QString::fromLocal8Bit("Plate number");
	m_titleMap[QString::fromLocal8Bit("lu_id")] =QString::fromLocal8Bit("Production batch");
	m_titleMap[QString::fromLocal8Bit("muban_id")] =QString::fromLocal8Bit("母板号（钢坯号）");
	m_titleMap[QString::fromLocal8Bit("platetype")] =QString::fromLocal8Bit("Steel grade");
	m_titleMap[QString::fromLocal8Bit("shengchanhao")] =QString::fromLocal8Bit("Batch number");
	m_titleMap[QString::fromLocal8Bit("hetonghao")]=QString::fromLocal8Bit("Contract number");
	m_titleMap[QString::fromLocal8Bit("gangji")]=QString::fromLocal8Bit("钢级");
	m_titleMap[QString::fromLocal8Bit("plate_length")]=QString::fromLocal8Bit("Length(mm)");
	m_titleMap[QString::fromLocal8Bit("plate_width")]=QString::fromLocal8Bit("Width(mm)");
	m_titleMap[QString::fromLocal8Bit("plate_thick")]=QString::fromLocal8Bit("Thickness(mm)");
	m_titleMap[QString::fromLocal8Bit("plate_weight")]=QString::fromLocal8Bit("Weight(t)");
	m_titleMap[QString::fromLocal8Bit("jiaohuostatus")]=QString::fromLocal8Bit("交货状态");
	// 探伤基本信息            
	m_titleMap[QString::fromLocal8Bit("yiqi")]=QString::fromLocal8Bit("Instrument model");
	m_titleMap[QString::fromLocal8Bit("detector")]=QString::fromLocal8Bit("Probe");
	m_titleMap[QString::fromLocal8Bit("tanshangmethod")]=QString::fromLocal8Bit("Test method");
	m_titleMap[QString::fromLocal8Bit("tanshangbiaozhun")]=QString::fromLocal8Bit("Test standard");
	m_titleMap[QString::fromLocal8Bit("tanshangtime")]=QString::fromLocal8Bit("Date of test");
	m_titleMap[QString::fromLocal8Bit("tanshangclass")]=QString::fromLocal8Bit("Test shift");
	m_titleMap[QString::fromLocal8Bit("tanshangrenyuan")]=QString::fromLocal8Bit("Test personnel");
	m_titleMap[QString::fromLocal8Bit("tanshanglinmindu")]=QString::fromLocal8Bit("Sensitivity");
	m_titleMap[QString::fromLocal8Bit("tanshangjibie")]=QString::fromLocal8Bit("Acceptance level");
	m_titleMap[QString::fromLocal8Bit("ouheji")]=QString::fromLocal8Bit("Couplant");
	m_titleMap[QString::fromLocal8Bit("name")]=QString::fromLocal8Bit("探伤模式");

	// 探伤结果
	m_titleMap[QString::fromLocal8Bit("result")]=QString::fromLocal8Bit("Test results");
	m_titleMap[QString::fromLocal8Bit("quexiansum")]=QString::fromLocal8Bit("缺陷总数");
	m_titleMap[QString::fromLocal8Bit("quexiansumarea")]=QString::fromLocal8Bit("缺陷总面积(cm^2)");
	m_titleMap[QString::fromLocal8Bit("quexianzhanjilv")]=QString::fromLocal8Bit("缺陷占积率(%)");
	m_titleMap[QString::fromLocal8Bit("quexianmaxlength")]=QString::fromLocal8Bit("指示长度(mm)");

	m_titleMap[QString::fromLocal8Bit("quexianmaxwidth")]=QString::fromLocal8Bit("缺陷最大指示宽度(mm)");
	m_titleMap[QString::fromLocal8Bit("QueXianMaxArea")]=QString::fromLocal8Bit("指示面积(cm^2)");
	m_titleMap[QString::fromLocal8Bit("quexiandetail")]=QString::fromLocal8Bit("缺陷详细信息");
	m_titleMap[QString::fromLocal8Bit("AllMaxQueXianLv")] = QString::fromLocal8Bit("缺陷率(%)");
	//////////////////////////////////////////////////////////////////////////
	m_titleMap[QString::fromLocal8Bit("BodyMaxZhiShiLength")]=QString::fromLocal8Bit("Max. length on body(mm)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxQueXianArea")]=QString::fromLocal8Bit("Max. area on body(mm^2)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxLength")]=QString::fromLocal8Bit("本体最大长度(mm)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxWidth")]=QString::fromLocal8Bit("本体最大宽度(mm)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxZhiShiLength")]=QString::fromLocal8Bit("Max. Length on edge(mm)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxQueXianArea")]=QString::fromLocal8Bit("Max. area on edge(mm^2)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxLength")]=QString::fromLocal8Bit("边缘最大长度(mm)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxWidth")]=QString::fromLocal8Bit("边缘最大宽度(mm)");
	m_titleMap[QString::fromLocal8Bit("BodyAvgQueXianLv")]=QString::fromLocal8Bit("本体平均缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("EdgeAvgQueXianLv")]=QString::fromLocal8Bit("边缘平均缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxQueXianLv")]=QString::fromLocal8Bit("本体最大缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxQueXianLv")]=QString::fromLocal8Bit("边缘最大缺陷率(%)");
	m_titleMap[QString::fromLocal8Bit("BodyMaxCountSquarMeter")]=QString::fromLocal8Bit("Defect count on body/m^2");
	m_titleMap[QString::fromLocal8Bit("EdgeMaxCountPerMeter")]=QString::fromLocal8Bit("Defect count on edge/m");
	m_titleMap[QString::fromLocal8Bit("AllMaxCountSquarMeter")]=QString::fromLocal8Bit("缺陷数/m^2");
	//////////////////////////////////////////////////////////////////////////
	m_titleMap[QString::fromLocal8Bit("info")]=QString::fromLocal8Bit("备注");
	// 统计
	m_titleMap[QString::fromLocal8Bit("lu_id2")]=QString::fromLocal8Bit("炉号");
	m_titleMap[QString::fromLocal8Bit("gangzhong2")]=QString::fromLocal8Bit("钢种");
	m_titleMap[QString::fromLocal8Bit("guige")]=QString::fromLocal8Bit("规格");
	m_titleMap[QString::fromLocal8Bit("tanshangjibie2")]=QString::fromLocal8Bit("探伤级别");
	m_titleMap[QString::fromLocal8Bit("tanshangbiaozhun2")]=QString::fromLocal8Bit("探伤标准");
	m_titleMap[QString::fromLocal8Bit("okcount")]=QString::fromLocal8Bit("合格块数");
	m_titleMap[QString::fromLocal8Bit("tanshangrenyuan2")]=QString::fromLocal8Bit("探伤人员");

	m_titleMapReverse.clear();
	// 钢板基本信息
	m_titleMapReverse[QString::fromLocal8Bit("报表流水号")] =QString::fromLocal8Bit("");
	m_titleMapReverse[QString::fromLocal8Bit("探伤流水号")] =QString::fromLocal8Bit("id");
	m_titleMapReverse[QString::fromLocal8Bit("Plate number")] = QString::fromLocal8Bit("name");
	m_titleMapReverse[QString::fromLocal8Bit("Production batch")] =QString::fromLocal8Bit("Lu");
	m_titleMapReverse[QString::fromLocal8Bit("母板号（钢坯号）")] =QString::fromLocal8Bit("MuBan");
	m_titleMapReverse[QString::fromLocal8Bit("Steel grade")] =QString::fromLocal8Bit("GangZhong");
	m_titleMapReverse[QString::fromLocal8Bit("Batch number")] =QString::fromLocal8Bit("ShenChan");
	m_titleMapReverse[QString::fromLocal8Bit("Contract number")]=QString::fromLocal8Bit("HeTong");

	m_titleMapReverse[QString::fromLocal8Bit("Length(mm)")]=QString::fromLocal8Bit("Length");
	m_titleMapReverse[QString::fromLocal8Bit("Width(mm)")]=QString::fromLocal8Bit("Width");
	m_titleMapReverse[QString::fromLocal8Bit("Thickness(mm)")]=QString::fromLocal8Bit("Thick");
	m_titleMapReverse[QString::fromLocal8Bit("Weight(t)")]=QString::fromLocal8Bit("Weight");
	m_titleMapReverse[QString::fromLocal8Bit("交货状态")]=QString::fromLocal8Bit("JiaoHuoStatus");
	// 探伤基本信息            
	m_titleMapReverse[QString::fromLocal8Bit("Instrument model")]=QString::fromLocal8Bit("YiQiXingHao");
	m_titleMapReverse[QString::fromLocal8Bit("Probe")]=QString::fromLocal8Bit("Detector");
	m_titleMapReverse[QString::fromLocal8Bit("Test method")]=QString::fromLocal8Bit("TanShangMethod");
	m_titleMapReverse[QString::fromLocal8Bit("Test standard")]=QString::fromLocal8Bit("TanShangBiaoZhun");
	m_titleMapReverse[QString::fromLocal8Bit("Date of test")]=QString::fromLocal8Bit("CheckTime");
	m_titleMapReverse[QString::fromLocal8Bit("Test shift")]=QString::fromLocal8Bit("CheckClass");
	m_titleMapReverse[QString::fromLocal8Bit("Test personnel")]=QString::fromLocal8Bit("CheckName");
	m_titleMapReverse[QString::fromLocal8Bit("Sensitivity")]=QString::fromLocal8Bit("TanShangLinMingDu");
	m_titleMapReverse[QString::fromLocal8Bit("Acceptance level")]=QString::fromLocal8Bit("TanShangJiBieBody");
	m_titleMapReverse[QString::fromLocal8Bit("Couplant")]=QString::fromLocal8Bit("OuHeJi");
	m_titleMapReverse[QString::fromLocal8Bit("探伤模式")]=QString::fromLocal8Bit("TestMode");
	// 探伤结果
	m_titleMapReverse[QString::fromLocal8Bit("Test results")]=QString::fromLocal8Bit("Result");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷总数")]=QString::fromLocal8Bit("MianShangCount");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷总面积(mm^2)")]=QString::fromLocal8Bit("MianShangAllMianJi");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷占积率(%)")]=QString::fromLocal8Bit("QueXianPercent");
	m_titleMapReverse[QString::fromLocal8Bit("指示长度(mm)")]=QString::fromLocal8Bit("QueXianMaxLength");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷最大指示宽度(mm)")]=QString::fromLocal8Bit("QueXianMaxWidth");
	m_titleMapReverse[QString::fromLocal8Bit("指示面积(cm^2)")]=QString::fromLocal8Bit("QueXianMaxArea");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷率(%)")]=QString::fromLocal8Bit("AllMaxQueXianLv");

	m_titleMapReverse[QString::fromLocal8Bit("Max. length on body(mm)")]=QString::fromLocal8Bit("BodyMaxZhiShiLength");
	m_titleMapReverse[QString::fromLocal8Bit("Max. area on body(mm^2)")]=QString::fromLocal8Bit("BodyMaxQueXianArea");
	m_titleMapReverse[QString::fromLocal8Bit("本体最大长度(mm)")]=QString::fromLocal8Bit("BodyMaxLength");
	m_titleMapReverse[QString::fromLocal8Bit("本体最大宽度(mm)")]=QString::fromLocal8Bit("BodyMaxWidth");
	m_titleMapReverse[QString::fromLocal8Bit("Max. Length on edge(mm)")]=QString::fromLocal8Bit("EdgeMaxZhiShiLength");
	m_titleMapReverse[QString::fromLocal8Bit("Max. area on edge(mm^2)")]=QString::fromLocal8Bit("EdgeMaxArea");
	m_titleMapReverse[QString::fromLocal8Bit("边缘最大长度(mm)")]=QString::fromLocal8Bit("EdgeMaxLength");
	m_titleMapReverse[QString::fromLocal8Bit("边缘最大宽度(mm)")]=QString::fromLocal8Bit("EdgeMaxWidth");
	m_titleMapReverse[QString::fromLocal8Bit("本体平均缺陷率(%)")]=QString::fromLocal8Bit("BodyAvgQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("边缘平均缺陷率(%)")]=QString::fromLocal8Bit("EdgeAvgQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("本体最大缺陷率(%)")]=QString::fromLocal8Bit("BodyMaxQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("边缘最大缺陷率(%)")]=QString::fromLocal8Bit("EdgeMaxQueXianLv");
	m_titleMapReverse[QString::fromLocal8Bit("Defect count on body/m^2")]=QString::fromLocal8Bit("BodyMaxCountSquarMeter");
	m_titleMapReverse[QString::fromLocal8Bit("Defect count on edge/m")]=QString::fromLocal8Bit("EdgeMaxCountPerMeter");
	m_titleMapReverse[QString::fromLocal8Bit("缺陷数/m^2")]=QString::fromLocal8Bit("AllMaxCountSquarMeter");
	//////////////////////////////////////////////////////////////////////////
	m_titleMapReverse[QString::fromLocal8Bit("缺陷详细信息")]=QString::fromLocal8Bit("");
	m_titleMapReverse[QString::fromLocal8Bit("备注")]=QString::fromLocal8Bit("");
	//////////////////////////////////////////////////////////////////////////////////////////////

	m_titleToNumber.clear();
	//vwPlate_Stat视图的映射
	m_titleToNumber[QString::fromLocal8Bit("id")]=0;
	m_titleToNumber[QString::fromLocal8Bit("name")]=1;
	m_titleToNumber[QString::fromLocal8Bit("Lu")]=2;
	m_titleToNumber[QString::fromLocal8Bit("MuBan")]=3;
	m_titleToNumber[QString::fromLocal8Bit("ShenChan")]=4;
	m_titleToNumber[QString::fromLocal8Bit("HeTong")]=5;
	m_titleToNumber[QString::fromLocal8Bit("GangJi")]=6;
	m_titleToNumber[QString::fromLocal8Bit("JiaoHuoStatus")]=7;
	m_titleToNumber[QString::fromLocal8Bit("PlateType")]=8;
	m_titleToNumber[QString::fromLocal8Bit("Length")]=9;
	m_titleToNumber[QString::fromLocal8Bit("Width")]=10;
	m_titleToNumber[QString::fromLocal8Bit("Thick")]=11;
	m_titleToNumber[QString::fromLocal8Bit("Weight")]=12;
	m_titleToNumber[QString::fromLocal8Bit("Result")]=13;
	m_titleToNumber[QString::fromLocal8Bit("CheckTime")]=14;
	m_titleToNumber[QString::fromLocal8Bit("CheckClass")]=15;
	m_titleToNumber[QString::fromLocal8Bit("DianShangCount")]=16;
	m_titleToNumber[QString::fromLocal8Bit("MianShangCount")]=17;
	m_titleToNumber[QString::fromLocal8Bit("MianShangAllMianJi")]=18;
	m_titleToNumber[QString::fromLocal8Bit("QueXianPercent")]=19;
	m_titleToNumber[QString::fromLocal8Bit("QueXianMaxLength")]=20;
	m_titleToNumber[QString::fromLocal8Bit("QueXianMaxWidth")]=21;
	m_titleToNumber[QString::fromLocal8Bit("CheckName")]=22;
	m_titleToNumber[QString::fromLocal8Bit("TestMode")]=23;
	m_titleToNumber[QString::fromLocal8Bit("CheckMethod")]=24;
	m_titleToNumber[QString::fromLocal8Bit("YiQiXingHao")]=25;
	m_titleToNumber[QString::fromLocal8Bit("Detector")]=26;
	m_titleToNumber[QString::fromLocal8Bit("TanShangMethod")]=27;
	m_titleToNumber[QString::fromLocal8Bit("TanTouZhongXinJuLi")]=28;
	m_titleToNumber[QString::fromLocal8Bit("TanShangLinMingDu")]=29;
	m_titleToNumber[QString::fromLocal8Bit("SaoMiaoJianGe")]=30;
	m_titleToNumber[QString::fromLocal8Bit("OuHeJi")]=31;
	m_titleToNumber[QString::fromLocal8Bit("IsReEvaluate")]=32;
	m_titleToNumber[QString::fromLocal8Bit("TanShangJiBieBody")]=33;
	m_titleToNumber[QString::fromLocal8Bit("TanShangBiaoZhun")]=34;
	m_titleToNumber[QString::fromLocal8Bit("AllMaxQueXianLv")]=35;
	m_titleToNumber[QString::fromLocal8Bit("GangZhong")]=36;
	m_titleToNumber[QString::fromLocal8Bit("QueXianMaxArea")]=37;
	m_titleToNumber[QString::fromLocal8Bit("info")]=38;
	m_titleToNumber[QString::fromLocal8Bit("TanShangJiBieEdge")]=39;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxZhiShiLength")]=40;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxQueXianArea")]=41;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxLength")]=42;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxWidth")]=43;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxZhiShiLength")]=44;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxArea")]=45;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxLength")]=46;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxWidth")]=47;
	m_titleToNumber[QString::fromLocal8Bit("BodyAvgQueXianLv")]=48;
	m_titleToNumber[QString::fromLocal8Bit("EdgeAvgQueXianLv")]=49;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxQueXianLv")]=50;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxQueXianLv")]=51;
	m_titleToNumber[QString::fromLocal8Bit("BodyMaxCountSquarMeter")]=52;
	m_titleToNumber[QString::fromLocal8Bit("EdgeMaxCountPerMeter")]=53;
	m_titleToNumber[QString::fromLocal8Bit("AllMaxCountSquarMeter")]=54;
	m_titleToNumber[QString::fromLocal8Bit("flag")]=55;
	m_titleToNumber[QString::fromLocal8Bit("CheckBodyLevel")]=56;
	m_titleToNumber[QString::fromLocal8Bit("CheckBiaoZhun")]=57;
	m_titleToNumber[QString::fromLocal8Bit("CheckEdgeLevel")]=58;
}
