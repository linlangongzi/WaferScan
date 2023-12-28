/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 钢板参数手动配置界面
* Filename: PlateParaSetDialog.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-17
*/


#include "DataDefine.h"
#include "PlateParaSetDialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include<QDir>


extern QSqlDatabase* g_sqlDB;
extern PLATE_INFO g_Info[1];
extern int nPlate;
extern QString g_strBiaoZhun;
extern QString g_strJiBie;
extern QString g_strProbeType;
extern QString g_Banji;
extern QString g_Renyuan;
extern GuoLvQueXian g_GuoLvQueXian;

CPlateParaSetDialog::CPlateParaSetDialog(QMap<QString,int> &FlagMap,CPlateInfo& PalateInfo,QSqlDatabase *pDatabaseConnection,QWidget *parent)
                                                       :QDialog(parent,Qt::CustomizeWindowHint|Qt::WindowTitleHint),m_pDatabaseConnection(pDatabaseConnection),m_pPlateInfo(&PalateInfo),m_pFlagMap(&FlagMap)
{	
		initDialog();
		retranslate();
		insertData();
		connect(m_ppbSwitch, SIGNAL(clicked()), this, SLOT(OnSwitch()));
		connect(m_ppbOk, SIGNAL(clicked()), this, SLOT(OnOK()));
		connect(m_ppbCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
		connect(m_pbReput, SIGNAL(clicked()), this, SLOT(reset()));
		connect(m_pcbDStandard,SIGNAL(activated(int)),this,SLOT(OnSelChangeComboStandard(int)));
}


CPlateParaSetDialog::~CPlateParaSetDialog(void)
{
}


void CPlateParaSetDialog::retranslate()
{
	setWindowTitle(tr("钢板输入参数"));
	m_pgbUp->setTitle(QString());
	m_plSPNumber->setText(tr("钢板号"));
	m_plTime->setText(tr("时间"));
	m_plPGrade->setText(tr("钢种"));
	m_plFNumber->setText(tr("炉号"));
	m_plHNumber->setText(tr("合同号"));
	m_plComment1->setText(tr("备注1"));
	m_pgbDown->setTitle(QString());
	m_plThickness->setText(tr("厚度"));
	m_plDStandard->setText(tr("探伤标准"));
	m_plWidth->setText(tr("宽度"));
	m_plELevel->setText(tr("边缘级别"));
	m_plBLevel->setText(tr("本体级别"));
	m_plProbeType->setText(tr("探头类型"));
	m_plLength->setText(tr("长度"));
	m_plComment2->setText(tr("备注2"));
	m_ppbCancel->setText(tr("退出"));
	m_ppbSwitch->setText(tr("ERP模式"));
	m_pbReput->setText(tr("重新输入"));
	m_ppbOk->setText(tr("确定"));
	m_pShiban->setText(tr("试板勾选"));
	m_pRenYuan->setText(tr("探伤人员"));
	m_pBanJi->setText(tr("探伤班级"));

} 

void CPlateParaSetDialog::initDialog(void)
{
	double StandardLength = 0;
	double StandardWidth = 0;
	double StandardHeight = 0;

	m_bIsOK = false;                 //标志点确定还是取消
	m_bIsSwitch = false;
	m_IniFlieName = QDir::currentPath() += "/PUTS.ini";
	m_settings = new QSettings(m_IniFlieName, QSettings::IniFormat);  //初始化QSettings对象

    //初始化Comment标示
	QString strTempFlag = m_settings->value("PlateParaSetDialog/IsLeftCommentHShow","").toString();
	if("" == strTempFlag) {
		m_settings->setValue("PlateParaSetDialog/IsLeftCommentHShow",0);
	}

	strTempFlag = m_settings->value("PlateParaSetDialog/IsRightCommentHShow","").toString();
	if("" == strTempFlag) {
		m_settings->setValue("PlateParaSetDialog/IsRightCommentHShow",0);
	}

	resize(1000, 600); 
	setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);font-size:20px"));

	m_pGroupLeftGridLayout = new QGridLayout;
	m_pGroupLeftGridLayout->setSpacing(6);
	m_pGroupLeftGridLayout->setContentsMargins(11, 11, 11, 11);

	m_pGroupRightGridLayout = new QGridLayout;
	m_pGroupLeftGridLayout->setSpacing(6);
	m_pGroupLeftGridLayout->setContentsMargins(11, 11, 11, 11);

	m_pLeftCommentLayout = new QVBoxLayout;
	m_pRightCommentLayout = new QVBoxLayout;
	m_pButtonLayout = new QHBoxLayout;
	m_pMainLayout = new QGridLayout(this);

//////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pgbUp = new QGroupBox(this);
	m_pgbUp->setEnabled(true);
	m_pgbUp->setMinimumHeight(300);
	m_pgbUp->setStyleSheet("QGroupBox{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);}\
							QLabel{color:rgb(255,255,255)}\
							QLineEdit{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,0)}");

	m_plSPNumber = new QLabel(m_pgbUp);
	QFont font;
	font.setPointSize(16);
	m_plSPNumber->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_plSPNumber,0,0,1,1);

	m_plTime = new QLabel(m_pgbUp);
	m_plTime->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_plTime,3,0,1,1);

	m_pleTime = new QLineEdit(m_pgbUp);
	m_pleTime->setMinimumSize(QSize(0, 30));
	m_pleTime->setMaximumSize(QSize(220, 16777215));
	m_pleTime->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_pleTime,3,1,1,1);

	m_pleSPNumber = new QLineEdit(m_pgbUp);
	m_pleSPNumber->setMinimumSize(QSize(0, 30));
	m_pleSPNumber->setMaximumSize(QSize(220, 16777215));
	m_pleSPNumber->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_pleSPNumber,0,1,1,1);

	m_plPGrade = new QLabel(m_pgbUp);
	m_plPGrade->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_plPGrade,1,0,1,1);

	m_pleHNumber = new QLineEdit(m_pgbUp);
	m_pleHNumber->setMinimumSize(QSize(0, 30));
	m_pleHNumber->setMaximumSize(QSize(220, 16777215));
	m_pleHNumber->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_pleHNumber,4,1,1,1);
	
	m_plFNumber = new QLabel(m_pgbUp);
	m_plFNumber->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_plFNumber,2,0,1,1);
	
	m_pleFNumber = new QLineEdit(m_pgbUp);
	m_pleFNumber->setMinimumSize(QSize(0, 30));
	m_pleFNumber->setMaximumSize(QSize(220, 16777215));
	m_pleFNumber->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_pleFNumber,2,1,1,1);
	
	m_plHNumber = new QLabel(m_pgbUp);
	m_plHNumber->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_plHNumber,4,0,1,1);
	
	m_plePGrade = new QLineEdit(m_pgbUp);
	m_plePGrade->setMinimumSize(QSize(0, 30));
	m_plePGrade->setMaximumSize(QSize(220, 16777215));
	m_plePGrade->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_plePGrade,1,1,1,1);
	m_pgbUp->setLayout(m_pGroupLeftGridLayout);


	m_pRenYuan = new QLabel(m_pgbUp);
	m_pRenYuan->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_pRenYuan,5,0,1,1);

	m_pRenYuanEdit = new QLineEdit(m_pgbUp);
	m_pRenYuanEdit->setMinimumSize(QSize(0, 30));
	m_pRenYuanEdit->setMaximumSize(QSize(220, 16777215));
	m_pGroupLeftGridLayout->addWidget(m_pRenYuanEdit,5,1,1,1);

	m_pBanJi = new QLabel(m_pgbUp);
	m_pBanJi->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_pBanJi,6,0,1,1);

	m_pBanJiEdit = new QLineEdit(m_pgbUp);
	m_pBanJiEdit->setMinimumSize(QSize(0, 30));
	m_pBanJiEdit->setMaximumSize(QSize(220, 16777215));
	m_pGroupLeftGridLayout->addWidget(m_pBanJiEdit,6,1,1,1);

	//chenyuzhu add start on 2014.5.13
	m_pShiban = new QLabel(m_pgbUp);
	m_pShiban->setFont(font);
	m_pGroupLeftGridLayout->addWidget(m_pShiban,7,0,1,1);

	m_pShibanCheck = new QCheckBox(m_pgbUp);
	m_pShibanCheck->setMinimumSize(QSize(0, 30));
	m_pShibanCheck->setMaximumSize(QSize(220, 16777215));
	m_pGroupLeftGridLayout->addWidget(m_pShibanCheck,7,1,1,1);
	//chenyuzhu add end on 2014.5.13

	m_plComment1 = new QLabel(this);
	m_plComment1->setFont(font);
	m_plComment1->setStyleSheet("color:rgb(255,255,255)");
	m_pLeftCommentLayout->addWidget(m_plComment1);
	
	m_pteComment1 = new QTextEdit(this);
	m_pteComment1 ->setStyleSheet("border-width: 1px; border-style: solid;border-color: rgb(0, 0, 0);");
	m_pteComment1->setFont(font);
	m_pteComment1 ->setStyleSheet("border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);\
							  color:rgb(255,255,0)");
	m_pLeftCommentLayout->addWidget(m_pteComment1);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_pgbDown = new QGroupBox(this);
	m_pgbDown->setMinimumHeight(300);
	
	m_pgbDown->setStyleSheet("QGroupBox{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);}\
									QLabel{color:rgb(255,255,255)}\
									QLineEdit{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,0)}");
    
	m_plThickness = new QLabel(m_pgbDown);
	m_plThickness->setFont(font);
	m_pGroupRightGridLayout->addWidget(m_plThickness,2,0,1,1);
	
	m_pleLength = new QLineEdit(m_pgbDown);
	m_pleLength->setMinimumSize(QSize(0, 30));
	m_pleLength->setMaximumSize(QSize(220, 16777215));
	m_pleLength->setFont(font);
	m_pleLength->setText(m_settings->value("PlateParaSetDialog/PlateLength","").toString());
	StandardLength = m_settings->value("PlateParaSetDialog/PlateLength","").toDouble();
	m_pGroupRightGridLayout->addWidget(m_pleLength,0,1,1,1);

	m_pleThickness = new QLineEdit(m_pgbDown);
	m_pleThickness->setMinimumSize(QSize(0, 30));
	m_pleThickness->setMaximumSize(QSize(220, 16777215));
	m_pleThickness->setFont(font);
	m_pleThickness->setText(m_settings->value("PlateParaSetDialog/PlateThickness","").toString());
	StandardHeight = m_settings->value("PlateParaSetDialog/PlateThickness","").toDouble();
	m_pGroupRightGridLayout->addWidget(m_pleThickness,2,1,1,1);
	
	m_plDStandard = new QLabel(m_pgbDown);
	m_plDStandard->setFont(font);
	m_pGroupRightGridLayout->addWidget(m_plDStandard,3,0,1,1);
	
	m_plWidth = new QLabel(m_pgbDown);
	m_plWidth->setFont(font);
	m_pGroupRightGridLayout->addWidget(m_plWidth,1,0,1,1);
	
	m_pleWidth = new QLineEdit(m_pgbDown);
	m_pleWidth->setMinimumSize(QSize(0, 30));
	m_pleWidth->setMaximumSize(QSize(220, 16777215));
	m_pleWidth->setFont(font);
	m_pleWidth->setText(m_settings->value("PlateParaSetDialog/PlateWidth","").toString());
	StandardWidth = m_settings->value("PlateParaSetDialog/PlateWidth","").toDouble();
	m_pGroupRightGridLayout->addWidget(m_pleWidth,1,1,1,1);

	// wangwei add start on 2016-05-13
	if ( (StandardLength >= (g_GuoLvQueXian.StandardLength - g_GuoLvQueXian.StandardOffsetLength)) 
		&& (StandardLength <= (g_GuoLvQueXian.StandardLength + g_GuoLvQueXian.StandardOffsetLength)) 
		&& (StandardWidth >= (g_GuoLvQueXian.StandardWidth - g_GuoLvQueXian.StandardOffsetLength))
		&& (StandardWidth <= (g_GuoLvQueXian.StandardWidth + g_GuoLvQueXian.StandardOffsetLength))
		&& (StandardHeight >= (g_GuoLvQueXian.StandardHeight - g_GuoLvQueXian.StandardOffsetHeight))
		&& (StandardHeight <= (g_GuoLvQueXian.StandardHeight + g_GuoLvQueXian.StandardOffsetHeight))) {
			g_GuoLvQueXian.IsConformStandard = TRUE;
	} else {
		g_GuoLvQueXian.IsConformStandard = FALSE;
	}

	if (g_GuoLvQueXian.IsTestPlate && g_GuoLvQueXian.IsConformStandard) {
		g_GuoLvQueXian.IsFilter = TRUE;
	} else {
		g_GuoLvQueXian.IsFilter = FALSE;
	}
	// wangwei add end
	
	m_plELevel = new QLabel(m_pgbDown);
	m_plELevel->setFont(font);
	m_pGroupRightGridLayout->addWidget(m_plELevel,5,0,1,1);
	
	m_plBLevel = new QLabel(m_pgbDown);
	m_plBLevel->setFont(font);
	m_pGroupRightGridLayout->addWidget(m_plBLevel,4,0,1,1);

	m_plProbeType = new QLabel(m_pgbDown);
	m_plProbeType->setFont(font);
	m_pGroupRightGridLayout->addWidget(m_plProbeType,6,0,1,1);
	
	m_pcbDStandard = new QComboBox(m_pgbDown);
	m_pcbDStandard->setMinimumSize(QSize(130, 25));
	m_pcbDStandard->setFont(font);
	m_pcbDStandard->setMaximumWidth(220);
	m_pGroupRightGridLayout->addWidget(m_pcbDStandard,3,1,1,1);
	
	m_pcbBLevel = new QComboBox(m_pgbDown);
	m_pcbBLevel->setMinimumSize(QSize(130, 25));
	m_pcbBLevel->setFont(font);
	m_pcbBLevel->setMaximumWidth(220);
	m_pcbBLevel->setStyleSheet("background-color: rgb(255, 255, 255);font-size:20px");
	m_pGroupRightGridLayout->addWidget(m_pcbBLevel,4,1,1,1);
	
	m_pcbELevel = new QComboBox(m_pgbDown);
	m_pcbELevel->setMinimumSize(QSize(130, 25));
	m_pcbELevel->setFont(font);
	m_pcbELevel->setMaximumWidth(220);
	m_pcbELevel->setStyleSheet("background-color: rgb(255, 255, 255);font-size:20px");
	m_pGroupRightGridLayout->addWidget(m_pcbELevel,5,1,1,1);

	m_pcbProbeType = new QComboBox(m_pgbDown);
	m_pcbProbeType->setMinimumSize(QSize(130, 25));
	m_pcbProbeType->setFont(font);
	m_pcbProbeType->setMaximumWidth(220);
	m_pcbProbeType->setStyleSheet("background-color: rgb(255, 255, 255);font-size:20px");
	m_pGroupRightGridLayout->addWidget(m_pcbProbeType,6,1,1,1);

	m_plLength = new QLabel(m_pgbDown);
	m_plLength->setFont(font);
	m_pGroupRightGridLayout->addWidget(m_plLength,0,0,1,1);
	m_pgbDown->setLayout(m_pGroupRightGridLayout);

	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_plComment2 = new QLabel(this);
	m_plComment2->setFont(font);
	m_plComment2->setStyleSheet("color:rgb(255,255,255)");
	m_pRightCommentLayout->addWidget(m_plComment2);
	m_pteComment2 = new QTextEdit(this);
	m_pteComment2 ->setStyleSheet("border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);\
							   color:rgb(255,255,0)");
	m_pteComment2->setFont(font);
	m_pRightCommentLayout->addWidget(m_pteComment2);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_ppbCancel = new QPushButton(this);
	m_ppbCancel->setFont(font);
	m_ppbCancel->setMaximumWidth(100);
	m_ppbCancel->setStyleSheet("color:rgb(255,255,255)");
	
	m_pbReput = new QPushButton(this);
	m_pbReput->setFont(font);
	m_pbReput->setMaximumWidth(100);
	m_pbReput->setStyleSheet("color:rgb(255,255,255)");
	
	m_ppbOk = new QPushButton(this);
	m_ppbOk->setFont(font);
	m_ppbOk->setMaximumWidth(100);
	m_ppbOk->setStyleSheet("color:rgb(255,255,255)");

	m_ppbSwitch = new QPushButton(this);
	m_ppbSwitch->setFont(font);
	m_ppbSwitch->setMaximumWidth(100);
	m_ppbSwitch->setStyleSheet("color:rgb(255,255,255)");

	m_pButtonLayout->addWidget(m_ppbOk);
	m_pButtonLayout->addWidget(m_ppbCancel);
	m_pButtonLayout->addWidget(m_ppbSwitch);


	m_pMainLayout->addWidget(m_pgbUp,0,0,2,1);
	m_pMainLayout->addWidget(m_pgbDown,0,1,2,1);
	m_pMainLayout->addWidget(m_pbReput,3,0,1,1);
	m_pMainLayout->addLayout(m_pButtonLayout,3,1,1,1);
    int nleftCommentFlag = m_settings->value("PlateParaSetDialog/IsLeftCommentHShow","").toInt();  
    int nRightCommentFlag = m_settings->value("PlateParaSetDialog/IsRightCommentHShow","").toInt();
	if ((0 != nleftCommentFlag&& (0 != nRightCommentFlag)))
	{
		m_pMainLayout->addLayout(m_pLeftCommentLayout,2,0,1,1);
		m_pMainLayout->addLayout(m_pRightCommentLayout,2,1,1,1);
	}else if ((0 != nleftCommentFlag&& (0 == nRightCommentFlag)))
	{
		m_pMainLayout->addLayout(m_pLeftCommentLayout,2,0,1,2);
		m_plComment2->setVisible(false);
		m_pteComment2->setVisible(false);

	}else if ((0 == nleftCommentFlag&& (0 != nRightCommentFlag)))
	{
		m_pMainLayout->addLayout(m_pRightCommentLayout,2,0,1,2);
		m_plComment1->setVisible(false);
		m_pteComment1->setVisible(false);
	}else
	{
		m_plComment1->setVisible(false);
		m_pteComment1->setVisible(false);

		m_plComment2->setVisible(false);
		m_pteComment2->setVisible(false);
	}
      
}

void CPlateParaSetDialog::insertData(void)
{   
	//初始化探伤标准
	//chenyuzhu modify on 2014-7-11
#if HAVE_DATA_BASE
	QSqlQuery query(*m_pDatabaseConnection);
	query.exec("Select Standard_Name from T_TanShangBiaoZhun");
	while (query.next())
	{
		m_pcbDStandard->addItem(query.value(0).toString());
	}
#endif
	
/*	m_pcbDStandard->addItem("GBT2970");
	m_pcbDStandard->addItem("ASTM 435");
	m_pcbDStandard->addItem("ASTM 578");
	m_pcbDStandard->addItem("ISO12094");
	m_pcbDStandard->addItem("PrEn10160");
	m_pcbDStandard->addItem("自定义_2970_1_5级");
	m_pcbDStandard->addItem("ASTM 435M");
	m_pcbDStandard->addItem("ASTM 578M");
	m_pcbDStandard->addItem("JIS G0801");
	m_pcbDStandard->addItem("JIS G0901");
	m_pcbDStandard->addItem("JBT4730");
	m_pcbDStandard->addItem("自定义_10160S2_5_E4");
	m_pcbDStandard->addItem("自定义_astm578_C_");
	m_pcbDStandard->addItem("自定义_12094_2_B1_5E1_5");*/
	//chenyuzhu modify on 2014-7-11
	QString Temp = m_settings->value("PlateParaSetDialog/DetectStandard","").toString();
    if (0 == Temp)
    {
		m_pcbDStandard->setCurrentIndex(-1);
    }else
	{
		int nTemp  = m_pcbDStandard->findText(Temp);
		m_pcbDStandard->setCurrentIndex(nTemp);
	}
	OnSelChangeComboStandard(m_pcbDStandard->currentIndex());
	Temp = m_settings->value("PlateParaSetDialog/BodyLevel","").toString();
	int nTemp  = m_pcbBLevel->findText(Temp);
	m_pcbBLevel->setCurrentIndex(nTemp);

	Temp = m_settings->value("PlateParaSetDialog/EdgeLevel","").toString();
	nTemp  = m_pcbELevel->findText(Temp);
	m_pcbELevel->setCurrentIndex(nTemp);

	Temp = m_settings->value("PlateParaSetDialog/ProbeType","").toString();
	nTemp  = m_pcbProbeType->findText(Temp);
	m_pcbProbeType->setCurrentIndex(nTemp);
	
	//dong qi add start on 2013.11.29 
	//将上次配置的显示到界面上
	
	QString PlateWidth = m_settings->value("PlateParaSetDialog/PlateWidth").toString();   //钢板宽度
	m_pleWidth->setText(PlateWidth);
	QString PlateLength = m_settings->value("PlateParaSetDialog/PlateLength").toString();  //钢板长
	m_pleLength->setText(PlateLength);
	QString PlateThickness = m_settings->value("PlateParaSetDialog/PlateThickness").toString();  //钢板厚
	m_pleThickness->setText(PlateThickness);
	QString ContractNum = m_settings->value("PlateParaSetDialog/ContractNum").toString();  //合同号
	m_pleHNumber->setText(ContractNum);//合同号
	
	QString FurnNumber = m_settings->value("PlateParaSetDialog/FurnNumber").toString();    //炉号
	m_pleFNumber->setText(FurnNumber);
	QString SteelGrade = m_settings->value("PlateParaSetDialog/SteelGrade").toString();    //钢种
	m_plePGrade->setText(SteelGrade);
	QString SteelNumber = m_settings->value("PlateParaSetDialog/SteelNumber").toString();    //钢板号
	m_pleSPNumber->setText(SteelNumber);
	QString Time = m_settings->value("PlateParaSetDialog/Time").toString();    //时间
	m_pleTime->setText(Time);

	//chenyuzhu add start on 2014.5.14
	int Flag = m_settings->value("PlateParaSetDialog/Flag").toInt();    //试板标志
	if(1 == Flag)
	{
		g_GuoLvQueXian.IsTestPlate = TRUE;
		m_pShibanCheck->setChecked(true);
	}
	else
	{
		g_GuoLvQueXian.IsTestPlate = FALSE;
		m_pShibanCheck->setChecked(false);
	}

	if (g_GuoLvQueXian.IsTestPlate && g_GuoLvQueXian.IsConformStandard) {
		g_GuoLvQueXian.IsFilter = TRUE;
	} else {
		g_GuoLvQueXian.IsFilter = FALSE;
	}

	//chenyuzhu add start on 2014.5.14

	QString DetectStandard = m_settings->value("PlateParaSetDialog/DetectStandard").toString();  //探伤标准
	int index = m_pcbDStandard->findText(DetectStandard);
	if (-1 != index)
	{
		m_pcbDStandard->setCurrentIndex(index);
	}
	else
	{
		m_pcbDStandard->setCurrentIndex(1);
	}

	QString EdgeLevel = m_settings->value("PlateParaSetDialog/EdgeLevel").toString();  //边缘级别
	index = m_pcbBLevel->findText(EdgeLevel);
	if (-1 != index)
	{
		m_pcbELevel->setCurrentIndex(index);
	}
	else
	{
		m_pcbELevel->setCurrentIndex(1);
	}

	QString BodyLevel = m_settings->value("PlateParaSetDialog/BodyLevel").toString();  //级别
	index = m_pcbBLevel->findText(BodyLevel);
	if (-1 != index)
	{
		m_pcbBLevel->setCurrentIndex(index);
	}
	else
	{
		m_pcbBLevel->setCurrentIndex(1);
	}

	QString ProbeType = m_settings->value("PlateParaSetDialog/ProbeType").toString();  //探头类型
	index = m_pcbProbeType->findText(ProbeType);
	if (-1 != index)
	{
		m_pcbProbeType->setCurrentIndex(index);
	}
	else
	{
		m_pcbProbeType->setCurrentIndex(0);		// 默认5M 双晶
	}
	//dong qi add end on 2013.11.29 
}

void CPlateParaSetDialog::OnOK(void)
{ 
	//dong qi add start on 2013.12.2
	m_bIsSwitch = false;

	if (g_sqlDB) {
		QSqlQuery query(*g_sqlDB);
		QString strSQL = QString("%1%2%3").arg("select * from T_PlateCheckInfo where Part_ID = '").arg(m_pleSPNumber->text()).arg("'");
		query.exec(strSQL);
		query.next();
		if (!query.value(0).isNull())
		{
			int ret = QMessageBox::information(this,tr("CPlateParaSetDialog::OnOK"),tr("数据库中已经存在此钢板号，是否继续?"),QMessageBox::Yes | QMessageBox::No);
			if(0 == ret)
			{
				OnCancel();  //相当于点击了取消按钮
			}
		}
	} else {
		QMessageBox::information(this, "CPlateParaSetDialog::OnOK", tr("未连接数据库"));
	}
	
	//dongqi add end on 2013.12.2 
	m_bIsOK = true;                       //标志点确定还是取消
	if (m_pleWidth->text().toFloat()<= 0)
	{
		QMessageBox::information(this,tr("错误提示"),tr("请输入合理宽度"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo->m_nPlateWidth = m_pleWidth->text().toFloat();


	if (m_pleLength->text().toFloat() <= 0)
	{
		QMessageBox::information(this,tr("错误提示"),tr("请输入合理长度"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo->m_nPlateLength = m_pleLength->text().toFloat();


	if (m_pleThickness->text().toFloat() <= 0)
	{
		QMessageBox::information(this,tr("错误提示"),tr("请输入合理厚度"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo->m_nPlateThickness = m_pleThickness->text().toFloat();


	if (m_pleHNumber->text() == NULL)
	{
		QMessageBox::information(this,tr("错误提示"),tr("请输入合理的合同号"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo->m_strContractNum = m_pleHNumber->text();             //合同号

	int TempIndex = m_pcbDStandard->currentIndex();
	if (-1 == TempIndex)
	{
		QMessageBox::information(this,tr("错误提示"),tr("未选择探伤标准"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QString strBodyLevel;    //本体级别
	m_pPlateInfo->m_strDetectStandard = m_pcbDStandard->currentText();          


	if (m_pPlateInfo->m_strDetectStandard =="JIS G0801" || m_pPlateInfo->m_strDetectStandard =="JIS G0901"
		||m_pPlateInfo->m_strDetectStandard =="ASTM 435"||m_pPlateInfo->m_strDetectStandard =="ASTM 435M"
		||m_pPlateInfo->m_strDetectStandard.contains(tr("自定义"),Qt::CaseInsensitive))
	{
		m_pPlateInfo->m_strEdgeLevel="";
		m_settings->setValue("PlateParaSetDialog/EdgeLevel",m_pPlateInfo->m_strEdgeLevel);  //边缘级别
		m_pPlateInfo->m_strBodyLevel="";
		m_settings->setValue("PlateParaSetDialog/BodyLevel",m_pPlateInfo->m_strBodyLevel);  //级别
	}
	else
	{
		TempIndex = m_pcbBLevel->currentIndex();
		if (-1 == TempIndex)
		{
			QMessageBox::information(this,tr("错误提示"),tr("未选择级别"),QMessageBox::Yes, QMessageBox::Yes);
			return;
		}	
		m_pPlateInfo ->m_strBodyLevel = m_pcbBLevel->currentText();
		m_settings->setValue("PlateParaSetDialog/BodyLevel", m_pPlateInfo->m_strBodyLevel);     //级别
		strBodyLevel= m_pPlateInfo->m_strBodyLevel;
	}

	if (m_pPlateInfo->m_strDetectStandard =="ISO12094" 
		|| m_pPlateInfo->m_strDetectStandard =="PrEn10160" 
		|| m_pPlateInfo->m_strDetectStandard == "BS_EN_ISO_10893"
		|| m_pPlateInfo->m_strDetectStandard =="NBT_47013_2015")
	{
		QString strEdgeLevel;
		int TempIndex = m_pcbELevel->currentIndex();
		if (-1 == TempIndex)
		{
			QMessageBox::information(this,tr("错误提示"),tr("未选择边缘级别"),QMessageBox::Yes, QMessageBox::Yes);
			return;
		}
	    m_pPlateInfo->m_strEdgeLevel = m_pcbELevel->currentText();
		if (m_pPlateInfo->m_strDetectStandard == "BS_EN_ISO_10893" && m_pPlateInfo->m_strEdgeLevel == "E2(6.0)")
		{
			m_pPlateInfo->m_strEdgeLevel = "E2";
		}
		m_settings->setValue("PlateParaSetDialog/EdgeLevel",m_pPlateInfo->m_strEdgeLevel);    //边缘级别
		strEdgeLevel = m_pPlateInfo->m_strEdgeLevel;
		g_strJiBie = strBodyLevel+tr(" / ") + strEdgeLevel;    //给全局变量g_strJiBie赋值
	}
	else
	{ 
		g_strJiBie = strBodyLevel;                            //给全局变量g_strJiBie赋值
	}

	if (m_pPlateInfo->m_strDetectStandard =="NBT_47013_2015")
	{
		QString strProbeType;
		int TempIndex = m_pcbProbeType->currentIndex();
		if (-1 == TempIndex)
		{
			QMessageBox::information(this,tr("错误提示"),tr("未选择探头类型"),QMessageBox::Yes, QMessageBox::Yes);
			return;
		}
		m_pPlateInfo->m_strProbeType = m_pcbProbeType->currentText();
		m_settings->setValue("PlateParaSetDialog/ProbeType",m_pPlateInfo->m_strProbeType);    // 探头类型
		g_strProbeType = m_pPlateInfo->m_strEdgeLevel;
	}
	else 
	{
		g_strProbeType = "";
	}

	if ("" == m_pleFNumber->text())
	{
		QMessageBox::information(this,tr("错误提示"),tr("炉号不能为空"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo->m_strFurnNumber = m_pleFNumber->text();
	
	if ("" == m_plePGrade->text())
	{
		QMessageBox::information(this,tr("错误提示"),tr("钢种不能为空"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo->m_strSteelGrade = m_plePGrade->text();

	if ("" == m_pleSPNumber->text())
	{
		QMessageBox::information(this,tr("错误提示"),tr("钢板号不能为空"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo ->m_strSteelNumber = m_pleSPNumber->text();
	
	if ("" == m_pleTime->text())
	{
		QMessageBox::information(this,tr("错误提示"),tr("时间不能为空"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_pPlateInfo->m_strTime = m_pleTime->text();


	//chenyuzhu add start on 2014.5.13
	if (m_pShibanCheck->isChecked())
	{
		m_pPlateInfo->flag = 1;
	}else
	{
		m_pPlateInfo->flag = 0;
	}
	//chenyuzhu add end on 2014.5.13

	(*m_pFlagMap)["MPlateParaFlag"] = 1;
	//dongqi add start on 2013.11.29
	//保存当前数据到配置文件
	m_settings->setValue("PlateParaSetDialog/PlateWidth",m_pPlateInfo->m_nPlateWidth);   //钢板宽度
	m_settings->setValue("PlateParaSetDialog/PlateLength",m_pPlateInfo->m_nPlateLength);  //钢板长
	m_settings->setValue("PlateParaSetDialog/PlateThickness",m_pPlateInfo->m_nPlateThickness);  //钢板厚
	m_settings->setValue("PlateParaSetDialog/ContractNum",m_pPlateInfo->m_strContractNum);  //合同号
	m_settings->setValue("PlateParaSetDialog/DetectStandard",m_pPlateInfo->m_strDetectStandard);  //探伤标准
	m_settings->setValue("PlateParaSetDialog/FurnNumber",m_pPlateInfo->m_strFurnNumber);    //炉号
	m_settings->setValue("PlateParaSetDialog/SteelGrade",m_pPlateInfo->m_strSteelGrade);    //钢种
	m_settings->setValue("PlateParaSetDialog/SteelNumber",m_pPlateInfo ->m_strSteelNumber);    //钢板号
	m_settings->setValue("PlateParaSetDialog/Time",m_pPlateInfo->m_strTime);    //时间

	//chenyuzhu add start on 2014.5.13	
	m_settings->setValue("PlateParaSetDialog/Flag",m_pPlateInfo->flag);
	//chenyuzhu add end on 2014.5.13

	// wangwei add start on 2016-05-13
	g_GuoLvQueXian.IsTestPlate = m_pPlateInfo->flag;

	if ( (m_pPlateInfo->m_nPlateLength >= (g_GuoLvQueXian.StandardLength - g_GuoLvQueXian.StandardOffsetLength)) 
		&& (m_pPlateInfo->m_nPlateLength <= (g_GuoLvQueXian.StandardLength + g_GuoLvQueXian.StandardOffsetLength)) 
		&& (m_pPlateInfo->m_nPlateWidth >= (g_GuoLvQueXian.StandardWidth - g_GuoLvQueXian.StandardOffsetLength))
		&& (m_pPlateInfo->m_nPlateWidth <= (g_GuoLvQueXian.StandardWidth + g_GuoLvQueXian.StandardOffsetLength))
		&& (m_pPlateInfo->m_nPlateThickness >= (g_GuoLvQueXian.StandardHeight - g_GuoLvQueXian.StandardOffsetHeight))
		&& (m_pPlateInfo->m_nPlateThickness <= (g_GuoLvQueXian.StandardHeight + g_GuoLvQueXian.StandardOffsetHeight))) {
		g_GuoLvQueXian.IsConformStandard = TRUE;
	} else {
		g_GuoLvQueXian.IsConformStandard = FALSE;
	}

	if (g_GuoLvQueXian.IsTestPlate && g_GuoLvQueXian.IsConformStandard) {
		g_GuoLvQueXian.IsFilter = TRUE;
	} else {
		g_GuoLvQueXian.IsFilter = FALSE;
	}
	// wangwei add end

	//dongqi add end on 2013.11.29
	//以上均是将变量保存到对象或写到文件中

	//dongqi add start on 2013.12.2
	//将信息保存到全局变量中
	// 钢板长、宽、高
	g_Info[nPlate].length = m_pPlateInfo->m_nPlateLength;
	g_Info[nPlate].fRealLength=0.0f;          //探完之后的钢板长度
	g_Info[nPlate].width = m_pPlateInfo->m_nPlateWidth;
	g_Info[nPlate].height = m_pPlateInfo->m_nPlateThickness;
	g_Info[nPlate].fReportHeight = m_pPlateInfo->m_nPlateThickness;
	g_strBiaoZhun = m_pPlateInfo->m_strDetectStandard;
	g_strProbeType = m_pPlateInfo->m_strProbeType;
	//g_strJiBie在上面应经给赋值
	g_Info[nPlate].plateno = m_pPlateInfo ->m_strSteelNumber;
	//dongqi add end on 2013.12.2

	g_Renyuan = m_pRenYuanEdit->text();

	g_Banji = m_pBanJiEdit->text();

    close();
}


void CPlateParaSetDialog::OnCancel(void)
{
	m_bIsOK = false;                 //标志点确定还是取消
	m_bIsSwitch = false;
	(*m_pFlagMap)["MPlateParaFlag"] = 0;
    close();
}

void CPlateParaSetDialog::OnSwitch(void)
{
	m_bIsSwitch = true;
	close();
}

void CPlateParaSetDialog::reset(void)
{
	m_pleLength->clear();
	m_pleThickness->clear();
	m_pcbBLevel->setCurrentIndex(-1);
	m_pleHNumber->clear();
	m_pcbDStandard->setCurrentIndex(-1);
	m_pcbELevel->setCurrentIndex(-1);
	m_pleFNumber->clear();
	m_plePGrade->clear();
	m_pleSPNumber->clear();
	m_pleTime->clear();
	m_pleWidth->clear();


}

void CPlateParaSetDialog::OnSelChangeComboStandard(int Index)
{
	m_pcbBLevel->clear();
	m_pcbELevel->clear();
	m_pcbProbeType->clear();

	QString strText = m_pcbDStandard->currentText();
	if (strText == "GBT2970" || !(QString::compare(strText,tr("JBT4730"),Qt::CaseInsensitive)))
	{

		m_pcbBLevel->addItem("I");
		m_pcbBLevel->addItem("II");
		m_pcbBLevel->addItem("III");
		m_pcbBLevel->addItem("IV");
	}
	else if (strText == "ASTM 435" || !(QString::compare(strText,tr("ASTM 435"),Qt::CaseInsensitive)))
	{	

	}
	else if (strText == "ASTM 578" || !(QString::compare(strText,tr("ASTM 578M"),Qt::CaseInsensitive)))
	{

		m_pcbBLevel->addItem("A");
		m_pcbBLevel->addItem("B");
		m_pcbBLevel->addItem("C");
	}
	else if (strText == "ISO12094")
	{

		m_pcbBLevel->addItem("B1");
		m_pcbBLevel->addItem("B2");
		m_pcbBLevel->addItem("B3");


		m_pcbELevel->addItem("E1");
		m_pcbELevel->addItem("E2");
		m_pcbELevel->addItem("E3");
	}
	else if (strText == "PrEn10160")
	{   

		m_pcbBLevel->addItem("S0");
		m_pcbBLevel->addItem("S1");
		m_pcbBLevel->addItem("S2");
		m_pcbBLevel->addItem("S3");

		m_pcbELevel->addItem("E0");
		m_pcbELevel->addItem("E1");
		m_pcbELevel->addItem("E2");
		m_pcbELevel->addItem("E3");
		m_pcbELevel->addItem("E4");
	}
	else if (strText == "BS_EN_ISO_10893")
	{
		m_pcbBLevel->addItem("B1");
		m_pcbBLevel->addItem("B2");
		m_pcbBLevel->addItem("B3");

		m_pcbELevel->addItem("E1");
		m_pcbELevel->addItem("E2(6.0)");
		m_pcbELevel->addItem("E3");
	}
	else if (strText == "NBT_47013_2015")
	{
		m_pcbBLevel->addItem("B-I");
		m_pcbBLevel->addItem("B-II");
		m_pcbBLevel->addItem("B-III");
		m_pcbBLevel->addItem("B-IV");
		m_pcbBLevel->addItem("B-V");

		m_pcbELevel->addItem("E-I");
		m_pcbELevel->addItem("E-II");
		m_pcbELevel->addItem("E-III");
		m_pcbELevel->addItem("E-IV");
		m_pcbELevel->addItem("E-V");

		m_pcbProbeType->addItem(tr("5M 双晶"));
		m_pcbProbeType->addItem(tr("5M 单晶"));
	}
	else
	{	

	}

	QString subStandard = strText.left(6);
	if (strText == "GBT2970" || strText == "ASTM 578" ||
		strText == "JBT4730"  || strText == "ASTM 578M")
	{
		m_pcbBLevel->show();
		m_plBLevel->show();
		m_plELevel->hide();
		m_pcbELevel->hide();
		m_plProbeType->hide();
		m_pcbProbeType->hide();
		m_plBLevel->setText(tr("探伤级别"));

	} 
	else if (strText == "BS_EN_ISO_10893")
	{
		m_pcbBLevel->show();
		m_plBLevel->show();
		m_plELevel->show();
		m_pcbELevel->show();
		m_plProbeType->hide();
		m_pcbProbeType->hide();
		m_plBLevel->setText(tr("本体级别"));
	}
	else if (strText == "JIS G0801"|| strText  == "JIS G0901"
		|| strText == "ASTM 435" || strText == "ASTM 435M"
		|| subStandard == tr("自定义"))
	{
		m_pcbBLevel->hide();
		m_plBLevel->hide();
		m_plELevel->hide();
		m_pcbELevel ->hide();
		m_plProbeType->hide();
		m_pcbProbeType->hide();
	}
	else
	{
		m_pcbBLevel->show();
		m_plBLevel->show();
		m_plELevel->show();
		m_pcbELevel ->show();
		m_plProbeType->show();
		m_pcbProbeType->show();
		m_plBLevel->setText(tr("本体级别"));
	}
}

void CPlateParaSetDialog::keyPressEvent(QKeyEvent *e)    //过滤ESC
{
	if (e->key() == Qt::Key_Home)
	{
		return;
	}else
	{
		QWidget::keyPressEvent(e);
	}

}
