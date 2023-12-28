/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 钢板参数ERP配置界面
* Filename: CPlateParaSetERPDialog.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-17
*/

#include <QFile>
#include <QDir>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include<QDir>
#include "PlateInfoPanel.h"
#include "DataDefine.h"
#include "PlateParaSetERPDialog.h"

extern QSqlDatabase* g_sqlDB;
extern PLATE_INFO g_Info[1];
extern int nPlate;
extern QString g_strBiaoZhun;
extern QString g_strJiBie;
extern QString g_strProbeType;
extern QString g_Banji;
extern QString g_Renyuan;


CPlateParaSetERPDialog::CPlateParaSetERPDialog(QMap<QString,int> &FlagMap,CPlateInfo& PalateInfo,QSqlDatabase *pDatabaseConnection,QWidget *parent )
                                               :QDialog(parent,Qt::CustomizeWindowHint|Qt::WindowTitleHint),m_pDatabaseConnection(pDatabaseConnection),m_pPalateInfo(&PalateInfo),m_pFlagMap(&FlagMap)
{
	m_pTempInfo = new CPlateInfo;

    initDialog();
	translate();
	initDataBase();  //初始化探伤标准
	connect(m_ppbPlateDPINumber,SIGNAL(clicked()),this,SLOT(insertData()));
	connect(m_pcbDStandard,SIGNAL(activated(int)),this,SLOT(OnSelChangeComboStandard(int)));

	connect(m_ppbOk, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(m_ppbExit, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(m_ppbSwitch, SIGNAL(clicked()), this, SLOT(OnSwitch()));
}

CPlateParaSetERPDialog::~CPlateParaSetERPDialog(void)
{
	if (NULL != m_pTempInfo)
	{
		delete m_pTempInfo;
		m_pTempInfo =NULL;
	}
}

void CPlateParaSetERPDialog::translate(void)
{
	this->setWindowTitle(tr("钢板参数设置"));
	m_pSteelNum->setText(tr("钢板号"));
	m_pgroupBox->setTitle(QString());
	m_plSPNumber->setText(tr("钢板号"));
	m_plTime->setText(tr("时间"));
	m_plPGrade->setText(tr("钢种"));
	m_plANumber->setText(tr("合同号"));
	m_plHNumber->setText(tr("炉号"));
	m_pRightgroupBox->setTitle(QString());
	m_plLength->setText(tr("长度"));
	m_plDStandard->setText(tr("探伤标准"));
	m_plWidth->setText(tr("宽度"));
	m_plBLevel->setText(tr("本体级别"));
	m_plThickness->setText(tr("厚度"));
	m_plELevel->setText(tr("边缘级别"));
	m_ppbExit->setText(tr("退出"));
	m_ppbOk->setText(tr("确定"));
	m_ppbSwitch->setText(tr("手动模式"));
	m_ppbPlateDPINumber->setText(tr("确定"));
	m_pRenYuan->setText(tr("探伤人员"));
	m_pBanJi->setText(tr("探伤班级"));
}


void CPlateParaSetERPDialog::initDialog(void)
{
	m_bIsOK = false;                 //标志点确定还是取消
	m_bIsSwitch = false;
	m_IniFlieName = QDir::currentPath() += "/PUTS.ini";
	m_settings = new QSettings(m_IniFlieName, QSettings::IniFormat);  //初始化QSettings对象

	//初始化Comment标示
	QString strTempFlag = m_settings->value("PlateParaSetERPDialog/IsLeftCommentERPShow","").toString();
	if("" == strTempFlag)
	{
		m_settings->setValue("PlateParaSetERPDialog/IsLeftCommentERPShow",0);
	}

	strTempFlag = m_settings->value("PlateParaSetERPDialog/IsRightCommentERPShow","").toString();
	if("" == strTempFlag)
	{
		m_settings->setValue("PlateParaSetERPDialog/IsRightCommentERPShow",0);
	}

	this->resize(1200, 600);
	this->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);font-size:20px"));
	QFont font;
	font.setPointSize(16);
	font.setKerning(true);
	this->setFont(font);

	m_pLeftVBoxLayOut = new QVBoxLayout;    //左边的
	m_pLeftVBoxLayOut->setSpacing(6);

	m_pGroup1GridLayOut = new QGridLayout;          //Group1    
	m_pGroup1GridLayOut->setSpacing(20);
	m_pGroup1GridLayOut->setContentsMargins(11, 11, 11, 11);

	m_pGroup2GridLayOut = new QGridLayout;
	m_pGroup2GridLayOut->setSpacing(20);
	m_pGroup2GridLayOut->setContentsMargins(11, 11, 11, 11);

	m_pButtonHBoxLayOut = new QHBoxLayout;
	m_pButtonHBoxLayOut ->setSpacing(10);

	m_pMainGridLayout = new QGridLayout(this);        //总体布局
	m_pMainGridLayout->setSpacing(15);
	m_pMainGridLayout->setContentsMargins(11, 11, 11, 11);

    m_pTopLeftGroupBox = new QGroupBox(this);
	m_pTopLeftGroupBox->setStyleSheet("QGroupBox{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,255)}");
	m_pSteelNum = new QLabel(m_pTopLeftGroupBox);
	QFont font1;
	font1.setPointSize(16);
	m_pSteelNum->setFont(font1);
	m_pSteelNum->setAlignment(Qt::AlignCenter);
    m_pSteelNum->setMaximumWidth(150);
	m_pSteelNum->setText(tr("钢板号"));
	m_pSteelNum->setStyleSheet("color: rgb(255,255,255)");

	m_plePlateDPINumber = new QLineEdit(m_pTopLeftGroupBox);   //钢板号编辑框
	m_plePlateDPINumber->setStyleSheet("border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,0)");

	m_ppbPlateDPINumber = new QPushButton(m_pTopLeftGroupBox); //钢板号确认按钮
	m_ppbPlateDPINumber->setStyleSheet("color:rgb(255,255,255)");

	m_pTopLeftLayout = new QHBoxLayout;
	m_pTopLeftLayout->setSpacing(8);            //左上角的标签和组合框
    m_pTopLeftLayout ->setContentsMargins(3,3,3,3);
	m_pTopLeftLayout->addWidget(m_pSteelNum);
//	m_pTopLeftLayout ->addWidget(m_pcmbPlateDPINumber);
	m_pTopLeftLayout ->addWidget(m_plePlateDPINumber);
	m_pTopLeftLayout ->addWidget(m_ppbPlateDPINumber);

	m_pTopLeftGroupBox->setLayout(m_pTopLeftLayout);

	m_pteLeft = new QTextEdit(this);
	m_pteLeft->setReadOnly(true);
	m_pteLeft->setStyleSheet("color:rgb(255,255,0)");
	m_pLeftVBoxLayOut->addWidget(m_pTopLeftGroupBox);
	m_pLeftVBoxLayOut->addWidget(m_pteLeft);                       //左边的布局

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pgroupBox = new QGroupBox(this);
    m_pgroupBox->setMinimumHeight(300);
	m_pgroupBox->setStyleSheet("QGroupBox{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);}\
							QLabel{color:rgb(255,255,255)}\
							QLineEdit{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,0)}");

	m_plSPNumber = new QLabel(m_pgroupBox);
	m_pGroup1GridLayOut->addWidget(m_plSPNumber, 0, 0, 1, 1);

	m_pleSPNumber = new QLineEdit(m_pgroupBox);
	m_pleSPNumber->setReadOnly(true);
	m_pleSPNumber->setMaximumWidth(220);
	m_pGroup1GridLayOut->addWidget(m_pleSPNumber, 0, 1, 1, 1);

	m_plTime = new QLabel(m_pgroupBox);
    m_pGroup1GridLayOut->addWidget(m_plTime, 3, 0, 1, 1);
	m_pleTime = new QLineEdit(m_pgroupBox);
	m_pleTime->setReadOnly(true);
	m_pleTime->setMaximumWidth(220);
	m_pGroup1GridLayOut->addWidget(m_pleTime, 3, 1, 1, 1);

	m_plPGrade = new QLabel(m_pgroupBox);
	m_pGroup1GridLayOut->addWidget(m_plPGrade, 1, 0, 1, 1);
	m_pleSGrade = new QLineEdit(m_pgroupBox);
	m_pleSGrade->setReadOnly(true);
	m_pleSGrade->setMaximumWidth(220);
	m_pGroup1GridLayOut->addWidget(m_pleSGrade, 1,1, 1, 1);

	m_plANumber = new QLabel(m_pgroupBox);
    m_pGroup1GridLayOut->addWidget(m_plANumber, 2, 0, 1, 1);
	m_pleANumber = new QLineEdit(m_pgroupBox);
	m_pleANumber->setReadOnly(true);
	m_pleANumber->setMaximumWidth(220);
	m_pGroup1GridLayOut->addWidget(m_pleANumber, 2, 1, 1, 1);

	m_plHNumber = new QLabel(m_pgroupBox);
	m_pGroup1GridLayOut->addWidget(m_plHNumber, 4, 0, 1, 1);
	m_pleHNumber = new QLineEdit(m_pgroupBox);
	m_pleHNumber->setMaximumWidth(220);
	m_pleHNumber->setReadOnly(true);
	m_pGroup1GridLayOut->addWidget(m_pleHNumber, 4, 1, 1, 1);


	m_pRenYuan = new QLabel(m_pgroupBox);
	m_pGroup1GridLayOut->addWidget(m_pRenYuan,5,0,1,1);

	m_pRenYuanEdit = new QLineEdit(m_pgroupBox);
	m_pRenYuanEdit->setMaximumSize(QSize(220, 16777215));
	m_pGroup1GridLayOut->addWidget(m_pRenYuanEdit,5,1,1,1);

	m_pBanJi = new QLabel(m_pgroupBox);
	m_pGroup1GridLayOut->addWidget(m_pBanJi,6,0,1,1);

	m_pBanJiEdit = new QLineEdit(m_pgroupBox);
	m_pBanJiEdit->setMaximumSize(QSize(220, 16777215));
	m_pGroup1GridLayOut->addWidget(m_pBanJiEdit,6,1,1,1);


	m_pgroupBox->setLayout(m_pGroup1GridLayOut);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pRightgroupBox = new QGroupBox(this);
	m_pRightgroupBox->setMinimumHeight(300);
	m_pRightgroupBox->setStyleSheet("QGroupBox{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);}\
							QLabel{color:rgb(255,255,255)}\
							QLineEdit{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,0)}\
							QComboBox{background-color: rgb(255, 255, 255);font-size:20px}");

	m_plLength = new QLabel(m_pRightgroupBox);
	m_pGroup2GridLayOut->addWidget(m_plLength, 0, 0, 1, 1);
	m_pleLength = new QLineEdit(m_pRightgroupBox);
	m_pleLength ->setMaximumWidth(220);
	m_pleLength->setReadOnly(true);
	m_pGroup2GridLayOut->addWidget(m_pleLength, 0, 1, 1, 1);

	m_plWidth = new QLabel(m_pRightgroupBox);
    m_pGroup2GridLayOut->addWidget(m_plWidth, 1, 0, 1, 1);

	m_pleWidth = new QLineEdit(m_pRightgroupBox);
	m_pleWidth ->setMaximumWidth(220);
	m_pleWidth->setReadOnly(true);
	m_pGroup2GridLayOut->addWidget(m_pleWidth, 1, 1, 1, 1);

	m_plThickness = new QLabel(m_pRightgroupBox);
	m_pGroup2GridLayOut->addWidget(m_plThickness, 2, 0, 1, 1);

	m_pleThickness = new QLineEdit(m_pRightgroupBox);
	m_pleThickness ->setMaximumWidth(220);
	m_pleThickness->setReadOnly(true);
	m_pGroup2GridLayOut->addWidget(m_pleThickness, 2, 1, 1, 1);

	m_plDStandard = new QLabel(m_pRightgroupBox);
	m_pGroup2GridLayOut->addWidget(m_plDStandard, 3, 0, 1, 1);

	m_plBLevel = new QLabel(m_pRightgroupBox);
	m_pGroup2GridLayOut->addWidget(m_plBLevel, 4, 0, 1, 1);

	m_plELevel = new QLabel(m_pRightgroupBox);
	m_pGroup2GridLayOut->addWidget(m_plELevel, 5, 0, 1, 1);

	m_pcbDStandard = new QComboBox(m_pRightgroupBox);
	m_pcbDStandard->setMaximumWidth(220);
	m_pcbDStandard->setStyleSheet("background-color: rgb(255, 255, 255);font-size:20px");
	m_pGroup2GridLayOut->addWidget(m_pcbDStandard, 3, 1, 1, 1);

	m_pcbBLevel = new QComboBox(m_pRightgroupBox);
	m_pcbBLevel->setMaximumWidth(220);
	m_pcbBLevel->setStyleSheet("background-color: rgb(255, 255, 255);font-size:20px");
	m_pGroup2GridLayOut->addWidget(m_pcbBLevel, 4, 1, 1, 1);

	m_pcbELevel = new QComboBox(m_pRightgroupBox);
	m_pcbELevel->setMaximumWidth(220);
	m_pcbELevel->setStyleSheet("background-color: rgb(255, 255, 255);font-size:20px");
	m_pGroup2GridLayOut->addWidget(m_pcbELevel, 5, 1, 1, 1);
	m_pRightgroupBox->setLayout(m_pGroup2GridLayOut);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pCommentleftLayOut = new QVBoxLayout;
	m_pCommentleftLayOut->setSpacing(10);
	m_pCommentRightLayOut = new QVBoxLayout;
	m_pCommentRightLayOut->setSpacing(10);

	m_plbCommentleft =new QLabel(this);
	m_plbCommentleft->setText(tr("备注1"));
	m_plbCommentleft->setMaximumWidth(100);
	m_plbCommentleft->setStyleSheet("color:rgb(255,255,255)");
	m_pCommentleftLayOut->addWidget(m_plbCommentleft);

	m_pteComment1 = new QTextEdit(this);
	m_pteComment1 ->setStyleSheet("border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);\
							   color:rgb(255,255,0)");
	m_pteComment1->setReadOnly(true);
    m_pCommentleftLayOut->addWidget(m_pteComment1);
	m_plbCommentRight = new QLabel;
	m_plbCommentRight->setText(tr("备注1"));
	m_plbCommentRight->setStyleSheet("color:rgb(255,255,255)");
	m_plbCommentRight->setMaximumWidth(100);
	m_pCommentRightLayOut->addWidget(m_plbCommentRight);

	m_pteComment2 = new QTextEdit(this);
	m_pteComment2 ->setStyleSheet("border-width: 1px; border-style: solid;border-color: rgb(255, 255,255);color:rgb(255,255,0)");
	m_pteComment2->setReadOnly(true);
	m_pCommentRightLayOut->addWidget(m_pteComment2);

	m_ppbOk = new QPushButton(this);
	m_ppbOk->setMaximumWidth(100);
	m_ppbOk->setStyleSheet("color:rgb(255,255,255)");
	m_pButtonHBoxLayOut->addWidget(m_ppbOk);

	m_ppbExit = new QPushButton(this);
	m_ppbExit->setMaximumWidth(100);
	m_ppbExit->setStyleSheet("color:rgb(255,255,255)");
	m_pButtonHBoxLayOut->addWidget(m_ppbExit);	


	m_ppbSwitch = new QPushButton(this);
	m_ppbSwitch->setMaximumWidth(100);
	m_ppbSwitch->setStyleSheet("color:rgb(255,255,255)");
	m_pButtonHBoxLayOut->addWidget(m_ppbSwitch);

	m_pMainGridLayout->addLayout(m_pLeftVBoxLayOut,0,0,3,1);
	m_pMainGridLayout->addLayout(m_pButtonHBoxLayOut,3,2,1,1);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nleftCommentFlag = m_settings->value("PlateParaSetERPDialog/IsLeftCommentERPShow","").toInt();  
	int nRightCommentFlag = m_settings->value("PlateParaSetERPDialog/IsRightCommentERPShow","").toInt();

	if ((0 != nleftCommentFlag&& (0 != nRightCommentFlag)))
	{
		m_pMainGridLayout->addLayout(m_pCommentleftLayOut,2,1,1,1);
		m_pMainGridLayout->addLayout(m_pCommentRightLayOut,2,2,1,1);
		m_pMainGridLayout->addWidget(m_pgroupBox,0,1,2,1);
		m_pMainGridLayout->addWidget(m_pRightgroupBox,0,2,2,1);
	}else if ((0 != nleftCommentFlag&& (0 == nRightCommentFlag)))
	{
		m_pMainGridLayout->addWidget(m_pgroupBox,0,1,2,1);
		m_pMainGridLayout->addWidget(m_pRightgroupBox,0,2,2,1);
	    m_pMainGridLayout->addLayout(m_pCommentleftLayOut,2,1,1,2);

		m_pCommentRightLayOut->setEnabled(false);
		m_plbCommentRight->setVisible(false);
		m_pteComment2->setVisible(false);

	}else if ((0 == nleftCommentFlag&& (0 != nRightCommentFlag)))
	{
		m_pMainGridLayout->addLayout(m_pCommentRightLayOut,2,1,1,2);
		m_pMainGridLayout->addWidget(m_pgroupBox,0,1,2,1);
		m_pMainGridLayout->addWidget(m_pRightgroupBox,0,2,2,1);

	    m_plbCommentleft->setVisible(false);
	    m_pteComment1->setVisible(false);
	}else
	{
		m_pMainGridLayout->addWidget(m_pgroupBox,0,1,3,1);
		m_pMainGridLayout->addWidget(m_pRightgroupBox,0,2,3,1);
		m_plbCommentRight->setVisible(false);
		m_pteComment2->setVisible(false);
		m_plbCommentleft->setVisible(false);
		m_pteComment1->setVisible(false);
	}
}

void CPlateParaSetERPDialog::initDataBase(void)
{	
#if HAVE_DATA_BASE
 	//插入钢板号信息
 	QSqlQuery query(*m_pDatabaseConnection);
// 	query.exec("select * from TBDL01");
// 	while(query.next())
// 	{
// 		m_pcmbPlateDPINumber->addItem(query.value(0).toString());  
// 	}
// 	m_pcmbPlateDPINumber->setCurrentIndex(-1);
 	//初始化探伤标准
 	query.exec("Select Standard_Name from T_TanShangBiaoZhun");
 	while (query.next())
 	{
 		m_pcbDStandard->addItem(query.value(0).toString());
 	}
 //	OnSelChangeComboStandard(m_pcbDStandard->currentIndex());
#endif
}

//插入相应数据

void CPlateParaSetERPDialog::insertData()
{   
	QSqlQuery query(*m_pDatabaseConnection);
	QString strNumber = m_plePlateDPINumber->text();
	QString strSql = QString("%1%2%3%4").arg("select * from TEMP02 where PlateNo =").arg("'").arg(strNumber).arg("'");
	
    query.exec(strSql);
	query.next();
	if (""== query.value(0).toString())
	{
		QString temp(tr("无此记录，请重核对数据序号再查询！！"));
		QMessageBox::information(this,"insertData",temp);
		return;
	}
	//钢板号
	QString strPlateNo;
	strPlateNo = query.value(1).toString();
	m_pTempInfo->m_strSteelNumber = strPlateNo;
	m_pleSPNumber->setText(strPlateNo);
	//排程号
	QString strpaichenghao;
	strpaichenghao = query.value(2).toString();
	//轧制序号
	QString strzhazhixuhao;
	strzhazhixuhao = query.value(3).toString();
	//计划排程日期
	QString strjihuapaichengdate;
	strjihuapaichengdate = query.value(4).toString();
	strjihuapaichengdate = strjihuapaichengdate.trimmed();
	QString strTempjihuapaichengdate(strjihuapaichengdate);
	strjihuapaichengdate.clear();
	strjihuapaichengdate = QString("%1%2%3%4%5").arg(strTempjihuapaichengdate.mid(0,4)).arg("-").arg(strTempjihuapaichengdate.mid(6,2)).arg("-").arg(strTempjihuapaichengdate.mid(4,2));
	//计划排程时间
	QString strjihuapaichengtime;
	strjihuapaichengtime = query.value(5).toString();
	strjihuapaichengtime = strjihuapaichengtime.trimmed();
	QString strTempjihuapaichengtime(strjihuapaichengtime);
	strjihuapaichengtime.clear();
	strjihuapaichengtime = QString("%1%2%3%4%5").arg(strTempjihuapaichengtime.mid(0,2)).arg(":").arg(strTempjihuapaichengtime.mid(2,2)).arg(":").arg(strTempjihuapaichengtime.mid(4,2));
	//二切板坯号
	QString strpeihao;
	strpeihao = query.value(6).toString();
	//钢种
	QString strGangZhong;
	strGangZhong = query.value(7).toString();
	//炉号
	QString strLuHao;
	strLuHao = query.value(8).toString();
	m_pTempInfo->m_strFurnNumber = strLuHao;
	m_pleHNumber->setText(strLuHao);
	//钢板厚度
	QString strThick;
	strThick = query.value(9).toString();
	m_pTempInfo->m_nPlateThickness = strThick.toFloat();
	m_pleThickness->setText(strThick);

	//钢板宽度
	QString strWidth;
	strWidth = query.value(10).toString();
	m_pTempInfo->m_nPlateWidth = strWidth.toFloat();
	m_pleWidth->setText(strWidth);
	//钢板长度
	QString strLength;
	strLength = query.value(11).toString();
	m_pTempInfo->m_nPlateLength = strLength.toFloat();
	m_pleLength->setText(strLength);
	//钢板重量
	QString strWeight;
	strWeight = query.value(12).toString();
	//订单号
	QString strDingdanHao;
	strDingdanHao = query.value(13).toString();
	m_pTempInfo->m_strContractNum = strDingdanHao;
	m_pleANumber->setText(strDingdanHao);
	//订单牌号
	QString strdingdanpaihao;
	strdingdanpaihao = query.value(14).toString();
	m_pTempInfo->m_strSteelGrade = strdingdanpaihao;
	m_pleSGrade->setText(strdingdanpaihao);
	//订单厚
	QString strdingdanThick;
	strdingdanThick = query.value(15).toString();
	//订单宽
	QString strdingdanWidth;
	strdingdanWidth = query.value(16).toString();
	//宽度限制
	QString strWidthLimit;
	strWidthLimit = query.value(17).toString();
	//订单长
	QString strdingdanLength;
	strdingdanLength = query.value(18).toString();
	m_pleLength->setText(strdingdanLength);
	//长度限制
	QString strLengthLimit;
	strLengthLimit = query.value(19).toString();
	strLengthLimit = strLengthLimit.trimmed();
	//探伤等级
	QString strdingdanLevel;
	strdingdanLevel = query.value(20).toString();
	m_pTempInfo->m_strBodyLevel = strdingdanLevel;
	m_pTempInfo->m_strBodyLevel = m_pTempInfo->m_strBodyLevel.trimmed();
	//技术标准
	QString strTanShangBiaoZhun;
	strTanShangBiaoZhun = query.value(21).toString();
	m_pTempInfo->m_strDetectStandard = strTanShangBiaoZhun;
	m_pTempInfo->m_strDetectStandard = m_pTempInfo->m_strDetectStandard.trimmed();
	//订单类别
	QString strdingdanType;
	strdingdanType = query.value(22).toString();
	//交库状态
	QString strjiaokuStatus;
	strjiaokuStatus = query.value(23).toString();
	//去向工艺1
	QString strquxiangongyi1;
	strquxiangongyi1 = query.value(24).toString();
	//去向工艺2
	QString strquxiangongyi2;
	strquxiangongyi2 = query.value(25).toString();
	//去向工艺3
	QString strquxiangongyi3;
	strquxiangongyi3 = query.value(26).toString();
	//去向工艺4
	QString strquxiangongyi4;
	strquxiangongyi4 = query.value(27).toString();
	//去向工艺5
	QString strquxiangongyi5;
	strquxiangongyi5 = query.value(28).toString();
	QString strText;
	strText = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29%30%31%32%33%34%35%36%37%38%39%40%41%42%43%44%45%46%47%48%49%50%51%52%53%54%55%56%57%58%59%60%61%62%63%64%65%66%67%68%69%70%71%72%73%74%75%76%77%78%79%80%81%82%83%84")
		                 .arg(tr("钢板号: ")).arg(strPlateNo).arg("\r\n")
		                 .arg(tr("排程号: ")).arg(strpaichenghao).arg("\r\n")
						 .arg(tr("轧制序号:")).arg(strzhazhixuhao).arg("\r\n")
						 .arg(tr("计划排程日期:")).arg(strjihuapaichengdate).arg("\r\n")
						 .arg(tr("计划排程时间:")).arg(strjihuapaichengtime).arg("\r\n")
						 .arg(tr("二切板坯号:")).arg(strpeihao).arg("\r\n")
						 .arg(tr("钢种:")).arg(strGangZhong).arg("\r\n")
						 .arg(tr("炉号:")).arg(strLuHao).arg("\r\n")
						 .arg(tr("钢板厚度:")).arg(strThick).arg("\r\n")
						 .arg(tr("钢板宽度:")).arg(strWidth).arg("\r\n")
						 .arg(tr("钢板长度:")).arg(strLength).arg("\r\n")
						 .arg(tr("钢板重量:")).arg(strWeight).arg("\r\n")
						 .arg(tr("订单号:")).arg(strDingdanHao).arg("\r\n")
						 .arg(tr("订单牌号:")).arg(strdingdanpaihao).arg("\r\n")
						 .arg(tr("订单厚:")).arg(strdingdanThick).arg("\r\n")
						 .arg(tr("订单宽:")).arg(strdingdanWidth).arg("\r\n")
						 .arg(tr("宽度限制:")).arg(strWidthLimit).arg("\r\n")
						 .arg(tr("订单长:")).arg(strdingdanLength).arg("\r\n")
						 .arg(tr("长度限制:")).arg(strLengthLimit).arg("\r\n")
						 .arg(tr("探伤等级:")).arg(strdingdanLevel).arg("\r\n")
						 .arg(tr("技术标准:")).arg(strTanShangBiaoZhun).arg("\r\n")
						 .arg(tr("订单类别:")).arg(strdingdanType).arg("\r\n")
						 .arg(tr("交库状态:")).arg(strjiaokuStatus).arg("\r\n")
						 .arg(tr("去向工艺1:")).arg(strquxiangongyi1).arg("\r\n")
						 .arg(tr("去向工艺2:")).arg(strquxiangongyi2).arg("\r\n")
						 .arg(tr("去向工艺3:")).arg(strquxiangongyi3).arg("\r\n")
						 .arg(tr("去向工艺4:")).arg(strquxiangongyi4).arg("\r\n")
						 .arg(tr("去向工艺5:")).arg(strquxiangongyi5).arg("\r\n");
	m_pteLeft->setText(strText);
	//获取系统当前时间并填入编辑框
	QDateTime time=QDateTime::currentDateTime();
	//获取系统当前时间
	QString str=time.toString("yyyy-MM-dd hh:mm:ss");
	m_pleTime->setText(str);
	m_pTempInfo->m_strTime = str;	
}
/*
void CPlateParaSetERPDialog::InterpreteDPIData(QString& strData)
{
	QString strText;
	int nIndex = 0;
	strText = QString("%1%2%3").arg(tr("FROM-ID: ")).arg(strData.mid(nIndex,6)).arg("\r\n");
	nIndex += 6;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("LOG回拋日期: ")).arg(strData.mid(nIndex,8)).arg("\r\n");

	nIndex += 8;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("LOG回拋时间: ")).arg(strData.mid(nIndex, 6)).arg("\r\n");

	nIndex += 6;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("资料传送来源计算机代号: ")).arg(strData.mid(nIndex, 2)).arg("\r\n");

	nIndex += 2;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("资料传送目的计算机代号: ")).arg(strData.mid(nIndex, 2)).arg("\r\n");

	nIndex += 2;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("功能代号: ")).arg(strData.mid(nIndex, 1)).arg("\r\n");

	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("钢板号: ")).arg(strData.mid(nIndex, 9)).arg("\r\n");

    m_pleSPNumber->setText(strData.mid(nIndex, 9));
	m_pTempInfo->m_strSteelNumber = strData.mid(nIndex, 9);

	nIndex += 9;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("排程号: ")).arg(strData.mid(nIndex, 3)).arg("\r\n");

	nIndex += 3;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("轧制序号: ")).arg(strData.mid(nIndex, 7)).arg("\r\n");

	nIndex += 7;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("计划排程日期: ")).arg(strData.mid(nIndex, 8)).arg("\r\n");

	nIndex += 8;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("计划排程时间: ")).arg(strData.mid(nIndex, 6)).arg("\r\n");

	nIndex += 6;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("二切板坯号: ")).arg(strData.mid(nIndex, 12)).arg("\r\n");

	nIndex += 12;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("钢种: ")).arg(strData.mid(nIndex, 20)).arg("\r\n");
    m_pTempInfo->m_strSteelGrade = strData.mid(nIndex, 20);

	nIndex += 20;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("炉号: ")).arg(strData.mid(nIndex, 8)).arg("\r\n");
	m_pleHNumber->setText(strData.mid(nIndex, 8));
	m_pTempInfo->m_strFurnNumber = strData.mid(nIndex, 8);

	nIndex += 8;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("钢板厚度: ")).arg(strData.mid(nIndex, 6)).arg("\r\n");
	m_pleThickness->setText(strData.mid(nIndex, 6));
	m_pTempInfo->m_nPlateThickness = (strData.mid(nIndex, 6)).toInt();

	nIndex += 6;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("钢板宽度: ")).arg(strData.mid(nIndex, 4)).arg("\r\n");
	m_pleWidth->setText(strData.mid(nIndex, 4));
	m_pTempInfo->m_nPlateWidth = (strData.mid(nIndex, 4)).toInt();

	nIndex += 4;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("钢板长度: ")).arg(strData.mid(nIndex, 5)).arg("\r\n");
	m_pleLength->setText(strData.mid(nIndex, 5));
	m_pTempInfo->m_nPlateLength = (strData.mid(nIndex, 5)).toInt();

	nIndex += 5;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("钢板重量: ")).arg(strData.mid(nIndex, 5)).arg("\r\n");

	nIndex += 5;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("订单号: ")).arg(strData.mid(nIndex, 11)).arg("\r\n");
	m_pleANumber->setText(strData.mid(nIndex, 11));
	m_pTempInfo->m_strContractNum = strData.mid(nIndex, 11);

	nIndex += 11;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("订单牌号: ")).arg(strData.mid(nIndex, 20)).arg("\r\n");

	nIndex += 20;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("订单厚: ")).arg(strData.mid(nIndex, 6)).arg("\r\n");

	nIndex += 6;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("订单宽: ")).arg(strData.mid(nIndex, 4)).arg("\r\n");

	nIndex += 4;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("宽度限制: ")).arg(strData.mid(nIndex, 1)).arg("\r\n");

	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("订单长: ")).arg(strData.mid(nIndex, 5)).arg("\r\n");

	nIndex += 5;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("长度限制: ")).arg(strData.mid(nIndex, 1)).arg("\r\n");

	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("探伤等级: ")).arg(strData.mid(nIndex, 5)).arg("\r\n");
	QString strBLevel = strData.mid(nIndex, 5); //探伤级别
    strBLevel = strBLevel.trimmed();
	m_pTempInfo->m_strBodyLevel = strBLevel;
	
	nIndex += 5;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("技术标准: ")).arg(strData.mid(nIndex, 15)).arg("\r\n");
	QString strDStandard = strData.mid(nIndex, 15); //探伤标准
	strDStandard = strDStandard.trimmed();
	int nDStandard = m_pcbDStandard->findText(strDStandard);
	m_pcbDStandard->setCurrentIndex(nDStandard);
	m_pTempInfo->m_strDetectStandard = strDStandard;

	int nBLevel = m_pcbBLevel->findText(strBLevel);
	m_pcbBLevel->setCurrentIndex(nBLevel);

	nIndex += 15;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("订单类别: ")).arg(strData.mid(nIndex, 5)).arg("\r\n");
	nIndex += 5;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("交库状态: ")).arg(strData.mid(nIndex, 1)).arg("\r\n");
	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("去向工艺1:" )).arg(strData.mid(nIndex, 1)).arg("\r\n");

	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("去向工艺2:" )).arg(strData.mid(nIndex, 1)).arg("\r\n");

	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("去向工艺3:" )).arg(strData.mid(nIndex, 1)).arg("\r\n");

	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("去向工艺4:" )).arg(strData.mid(nIndex, 1)).arg("\r\n");

	nIndex += 1;
	strText = QString("%1%2%3%4").arg(strText).arg(tr("去向工艺5:" )).arg(strData.mid(nIndex, 1)).arg("\r\n");

    m_pteLeft->setText(strText);
	//获取系统当前时间并填入编辑框
	QDateTime time=QDateTime::currentDateTime();

	//获取系统当前时间
    QString str=time.toString("yyyy-MM-dd hh:mm:ss");
	m_pleTime->setText(str);
	m_pTempInfo->m_strTime = str;
}
*/

void CPlateParaSetERPDialog::OnSelChangeComboStandard(int Index)
{
	m_pcbBLevel->clear();
	m_pcbELevel->clear();

	QString strText = m_pcbDStandard->currentText();
	if (strText == "GBT2970" || !(QString::compare(strText,tr("JBT4730"),Qt::CaseInsensitive)))
	{
		
		m_pcbBLevel->addItem("I");
		m_pcbBLevel->addItem("II");
		m_pcbBLevel->addItem("III");
		m_pcbBLevel->addItem("IV");
	}

	if (strText == "ASTM 435" || !(QString::compare(strText,tr("ASTM 435"),Qt::CaseInsensitive)))
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
		m_pcbELevel ->hide();
		m_plBLevel->setText(tr("探伤级别"));
	} 
	else if (strText == "JIS G0801"|| strText  == "JIS G0901"
		|| strText == "ASTM 435" || strText == "ASTM 435M"
		|| subStandard == tr("自定义"))
	{
		m_pcbBLevel->hide();
		m_plBLevel->hide();
		m_plELevel->hide();
		m_pcbELevel ->hide();
	}
	else
	{
		m_pcbBLevel->show();
		m_plBLevel->show();
		m_plELevel->show();
		m_pcbELevel ->show();
	}
}


void CPlateParaSetERPDialog::OnOK(void)
{
	m_bIsSwitch = false;

	if (g_sqlDB) {
		QSqlQuery query(*g_sqlDB);
		QString strSQL = QString("%1%2%3").arg("select * from T_PlateCheckInfo where Part_ID = '").arg(m_pleSPNumber->text()).arg("'");
		query.exec(strSQL);
		query.next();
		if (!query.value(0).isNull()) {
			int ret = QMessageBox::information(this,tr("CPlateParaSetDialog::OnOK"),tr("数据库中已经存在此钢板号，是否继续?"),QMessageBox::Yes | QMessageBox::No);
			if(0 == ret) {
				OnCancel();  //相当于点击了取消按钮
			}
		}
	}
	
	m_bIsOK = true;                 //标志点确定还是取消
	m_pPalateInfo->m_Memo1 = m_pteComment1->toPlainText();
	m_pPalateInfo->m_Memo2 = m_pteComment2->toPlainText();
    m_pPalateInfo->m_nPlateLength = m_pTempInfo->m_nPlateLength;
    m_pPalateInfo->m_nPlateThickness = m_pTempInfo->m_nPlateThickness;
	m_pPalateInfo->m_nPlateWidth = m_pTempInfo->m_nPlateWidth;
	m_pPalateInfo->m_strContractNum = m_pTempInfo->m_strContractNum;
    m_pPalateInfo->m_strFurnNumber = m_pTempInfo->m_strFurnNumber;
    m_pPalateInfo->m_strSteelGrade = m_pTempInfo->m_strSteelGrade;
    m_pPalateInfo->m_strSteelNumber = m_pTempInfo->m_strSteelNumber;
	m_pPalateInfo->m_strTime = m_pTempInfo->m_strTime;
	m_pPalateInfo->m_strBodyLevel = m_pcbBLevel->currentText();
    m_pPalateInfo->m_strDetectStandard = m_pcbDStandard->currentText();
	m_pPalateInfo->m_strEdgeLevel = m_pcbELevel->currentText();
	if (m_pPalateInfo->m_strDetectStandard == "BS_EN_ISO_10893" && m_pPalateInfo->m_strEdgeLevel == "E2(6.0)") {
		m_pPalateInfo->m_strEdgeLevel = "E2";
	}

	int TempIndex = m_pcbDStandard->currentIndex();
	if (-1 == TempIndex) {
		QMessageBox::information(this,tr("错误提示"),tr("未选择探伤标准"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}

	QString strBodyLevel;    //本体级别
	m_pPalateInfo->m_strDetectStandard = m_pcbDStandard->currentText();          

	if (m_pPalateInfo->m_strDetectStandard =="JIS G0801" || m_pPalateInfo->m_strDetectStandard =="JIS G0901"
		||m_pPalateInfo->m_strDetectStandard =="ASTM 435"||m_pPalateInfo->m_strDetectStandard =="ASTM 435M"
		||m_pPalateInfo->m_strDetectStandard.contains(tr("自定义"),Qt::CaseInsensitive)) {
		m_pPalateInfo->m_strEdgeLevel="";
		m_settings->setValue("PlateParaSetDialog/EdgeLevel",m_pPalateInfo->m_strEdgeLevel);  //边缘级别
		m_pPalateInfo->m_strBodyLevel="";
		m_settings->setValue("PlateParaSetDialog/BodyLevel",m_pPalateInfo->m_strBodyLevel);  //级别
	} else {
		TempIndex = m_pcbBLevel->currentIndex();
		if (-1 == TempIndex) {
			QMessageBox::information(this,tr("错误提示"),tr("未选择级别"),QMessageBox::Yes, QMessageBox::Yes);
			return;
		}	

		m_pPalateInfo ->m_strBodyLevel = m_pcbBLevel->currentText();
		m_settings->setValue("PlateParaSetDialog/BodyLevel",m_pPalateInfo ->m_strBodyLevel);     //级别
		strBodyLevel= m_pPalateInfo ->m_strBodyLevel;
	}

	m_pPalateInfo->flag = 0;	// wangwei add on 20151117 试板模式标志修改

	if (m_pPalateInfo->m_strDetectStandard =="ISO12094" 
		|| m_pPalateInfo->m_strDetectStandard =="PrEn10160" 
		|| m_pPalateInfo->m_strDetectStandard == "BS_EN_ISO_10893"
		|| m_pPalateInfo->m_strDetectStandard == "NBT_47013_2015")
	{
		QString strEdgeLevel;
		int TempIndex = m_pcbELevel->currentIndex();
		if (-1 == TempIndex)
		{
			QMessageBox::information(this,tr("错误提示"),tr("未选择边缘级别"),QMessageBox::Yes, QMessageBox::Yes);
			return;
		}
		m_pPalateInfo->m_strEdgeLevel = m_pcbELevel->currentText();
		if (m_pPalateInfo->m_strDetectStandard == "BS_EN_ISO_10893" && m_pPalateInfo->m_strEdgeLevel == "E2(6.0)")
		{
			m_pPalateInfo->m_strEdgeLevel = "E2";
		}
		m_settings->setValue("PlateParaSetDialog/EdgeLevel",m_pPalateInfo->m_strEdgeLevel);    //边缘级别
		strEdgeLevel = m_pPalateInfo->m_strEdgeLevel;
		g_strJiBie = strBodyLevel+tr(" / ") + strEdgeLevel;    //给全局变量g_strJiBie赋值
	}
	else
	{ 
		g_strJiBie = strBodyLevel;                            //给全局变量g_strJiBie赋值
	}

	if (m_pPalateInfo->m_strDetectStandard =="NBT_47013_2015")
	{
		QString strProbeType;
		int TempIndex = m_pcbProbeType->currentIndex();
		if (-1 == TempIndex)
		{
			QMessageBox::information(this,tr("错误提示"),tr("未选择探头类型"),QMessageBox::Yes, QMessageBox::Yes);
			return;
		}
		m_pPalateInfo->m_strProbeType = m_pcbProbeType->currentText();
		m_settings->setValue("PlateParaSetDialog/ProbeType",m_pPalateInfo->m_strProbeType);    // 探头类型
		g_strProbeType = m_pPalateInfo->m_strEdgeLevel;
	}
	else 
	{
		g_strProbeType = "";
	}

	if (m_pPalateInfo->m_strSteelNumber =="")
	{
		QMessageBox::information(this,tr("错误提示"),tr("配置参数不能为空"),QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	(*m_pFlagMap)["MPlateParaFlag"] = 1;
	close();


	//将信息保存到全局变量中
	// 钢板长、宽、高
	g_Info[nPlate].length = m_pPalateInfo->m_nPlateLength;
	g_Info[nPlate].fRealLength = 0.0f;          //探完之后的钢板长度
	g_Info[nPlate].width = m_pPalateInfo->m_nPlateWidth;
	g_Info[nPlate].height = m_pPalateInfo->m_nPlateThickness;
	g_Info[nPlate].fReportHeight = m_pPalateInfo->m_nPlateThickness;
	g_strBiaoZhun = m_pPalateInfo->m_strDetectStandard;
	g_strProbeType = m_pPalateInfo->m_strProbeType;
	//g_strJiBie在上面应经给赋值
	g_Info[nPlate].plateno = m_pPalateInfo ->m_strSteelNumber;

	g_Renyuan = m_pRenYuanEdit->text();

	g_Banji = m_pBanJiEdit->text(); 
	
}

void CPlateParaSetERPDialog::OnCancel(void)
{
	m_bIsOK = false;                 //标志点确定还是取消
	m_bIsSwitch = false;
	(*m_pFlagMap)["MPlateParaFlag"] = 0;
	close();
}
void CPlateParaSetERPDialog::keyPressEvent(QKeyEvent *e)    //过滤ESC
{
	if (e->key() == Qt::Key_Home)
	{
		return;
	}else
	{
		QWidget::keyPressEvent(e);
	}

}

void CPlateParaSetERPDialog::OnSwitch()                   //切换到手动模式 
{
	m_bIsSwitch = true;
	close();
}
