/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 工艺设置主界面
* Filename: TechnologySetDialog.cpp
*
* @author        DongQi
* @version       1.00         Date: 2013-04-27
*/

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QSettings>
#include <QDir>
#include "PlateInfoPanel.h"
#include "TechnologySetDialog.h"
#include "DataDefine.h"
#include "DataBaseFile.h"

QString g_strTanTouName;
float g_fDelayLength;
int g_nCeHouBuChang;
extern QString g_strGongYi;
extern QSqlDatabase *g_sqlDB;
extern QString g_strBiaoZhun;
extern QString g_strJiBie;


CTechnologySetDialog::CTechnologySetDialog(QMap<QString,int> &FlagMap,CPlateInfo&  PlateInfo,QSqlDatabase *pDatabaseConnection,QWidget *parent)
					:m_pDatabaseConnection(pDatabaseConnection),m_pPlateInfo(&PlateInfo),m_pFlagMap(&FlagMap),QDialog(parent,Qt::CustomizeWindowHint|Qt::WindowTitleHint)
{

	this->setStyleSheet(" CTechnologySetDialog{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);font-size:20px;color:rgb(255,255,255)}\
						  QLabel{color:rgb(255,255,255);font-size:20px}\
						  QPushButton{color:rgb(255,255,255);font-size:20px}\
						  QComboBox{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,255);font-size:20px;background-color: rgb(73, 73, 73)}");
   
	this->setWindowTitle(tr("工艺名称"));
	this->setMinimumSize(QSize(600,300));

	m_pMainGridLayout = new QGridLayout;
	m_pMainGridLayout->setSpacing(100);
	m_pMainGridLayout->setContentsMargins(70,70,70,70);


	QLabel *m_plblName = new QLabel(this);

	m_plblName ->setText(tr("工艺名称"));
	m_plblName ->setAlignment(Qt::AlignCenter);
    m_plblName->setMaximumWidth(150);
	m_pMainGridLayout->addWidget(m_plblName,0,0,1,1);
	

	m_pcbName = new QComboBox(this);
	m_pcbName ->setEditable(false);
	m_pcbName->setMaximumWidth(150);
	m_pMainGridLayout->addWidget(m_pcbName,0,1,1,1);

	m_pbgOk  = new QPushButton(this);
	m_pbgOk -> setText(tr("确定"));
	m_pMainGridLayout->addWidget(m_pbgOk,4,0,1,1);

	m_pbgCancel  = new QPushButton(this);
	m_pbgCancel -> setText(tr("取消"));
	m_pMainGridLayout->addWidget(m_pbgCancel,4,1,1,1);
	setLayout(m_pMainGridLayout);

	InsertDataIntoCbName();
   //连接槽函数
   connect(m_pbgOk,SIGNAL(clicked()),this,SLOT(SettingConfig()));
   connect(m_pbgCancel,SIGNAL(clicked()),this,SLOT(DestroyTechnologySettingDialog()));
}

CTechnologySetDialog::~CTechnologySetDialog(void)
{

}

void CTechnologySetDialog::DestroyTechnologySettingDialog(void)
{
  (*m_pFlagMap)["MTechnologySetFlag"] = 0;
   close();
   exit(0);
}

//chenyuzhu modify start on 2014-7-11  
void CTechnologySetDialog::InsertDataIntoCbName(void)
{
	//chenyuzhu modify start on 2014-7-11  
/*	QSqlQuery gysQuery(*g_sqlDB);
	QString strQuerry = "Select name from t_TanShangGongYi";
	gysQuery.exec(strQuerry);
	while(gysQuery.next())
	{
		m_pcbName->addItem(gysQuery.value(0).toString());
	}*/
	
	m_pcbName->addItem("黑材");
	m_pcbName->addItem("军工钢");
	m_pcbName->addItem("人工");
	//chenyuzhu modify end on 2014-7-11  
	//dong qi add start on 2013.11.29
	//读取上千工艺配置
	QString strPath = QDir::currentPath() + "/PUTS.ini" ;
	QSettings setting(strPath,QSettings::IniFormat);
	QString strArtName = setting.value("UserSet/GongYi",m_pPlateInfo->m_strArtName).toString();
	int index = m_pcbName->findText(strArtName);
	if (-1 != index)
	{
		m_pcbName->setCurrentIndex(index);
	}
	//dong qi add end on 2013.11.29
	
}
//chenyuzhu modify end on 2014-7-11  


void CTechnologySetDialog::SettingConfig(void)  //响应OK的槽
{
	m_pPlateInfo->m_strArtName = m_pcbName->currentText();  //保存工艺名称
	(*m_pFlagMap)["MTechnologySetFlag"] = 1;							//记录已配置

	//dong qi add start on 2013.11.29
	//保存当前工艺配置到配置文件中
	QString strPath = QDir::currentPath() + "/PUTS.ini" ;
	QSettings setting(strPath,QSettings::IniFormat);
	setting.setValue("UserSet/GongYi",m_pPlateInfo->m_strArtName);
	//dong qi add end in 2013.11.29
	
	g_strGongYi = m_pcbName->currentText();
	QString strTanShangJiBie;
	QString strTanShangJiBieEdge;
	QString strTanTouNumber;
	
	//chenyuzhu modify start on 2014-7-11
// 	if (g_sqlDB) {
// 		QSqlQuery query(*g_sqlDB);
// 	}
// 	QString strSql = QString("%1%2%3").arg("select * from t_TanShangGongYi where name = '")
// 									  .arg(g_strGongYi)
// 									  .arg("'");
//	query.exec(strSql);
//	query.next();
//	select top 1 id,Material from dbo.T_PlateCheckInfo where Material='黑才' order by id desc
	
		
	//chenyuzhu add start on 2014-7-11
/*	QString strSql = QString("%1%2%3").arg("select top 1 id,Material from T_PlateCheckInfo where Material='")
		.arg(g_strGongYi).arg("'").arg(" order by id desc");
	query.exec(strSql);
	query.next();
	//读取并解压SheBeiFile文件*/

	QString strFileName = QDir::currentPath()+ "/SheBeiFile.dbfl";
	QFile file(strFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
		return ;
	}
	SHEBEICANSHU SheBeiCanShu;

	file.seek(0);

	quint64 nBytesRead = file.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));//读文件到结构体中
	file.close();


	strTanTouNumber = SheBeiCanShu.TanShangGongYi.strDetector; //Detector
	g_strBiaoZhun = SheBeiCanShu.TanShangGongYi.strTangShangBiaoZhun; //TanShangBiaoZhun
	strTanShangJiBie = SheBeiCanShu.TanShangGongYi.strTanShangJiBieBody; //TanShangJiBieBody
	strTanShangJiBieEdge = SheBeiCanShu.TanShangGongYi.strTanShangJiBieEdge; //TanShangJiBieEdge
	strTanShangJiBie = strTanShangJiBie.trimmed();
	strTanShangJiBieEdge = strTanShangJiBieEdge.trimmed();
	if (strTanShangJiBieEdge != (""))
	{
		g_strJiBie = strTanShangJiBie +(" / ") + strTanShangJiBieEdge;
	}
	else
		g_strJiBie = strTanShangJiBie;	
	strTanTouNumber = strTanTouNumber.trimmed();
	//strSql = QString("%1%2%3").arg("Select * from t_Detector where name = '").arg(strTanTouNumber).arg("'");
	//query.clear();
	//query.exec(strSql);
	//query.next();
	float fDelayLength  = SheBeiCanShu.DetectorParam.fDelay;
	int nCeHouBuChang = SheBeiCanShu.DetectorParam.fCeHouBuChang;
	g_strTanTouName = SheBeiCanShu.DetectorParam.strName;
	g_fDelayLength = fDelayLength;
	g_nCeHouBuChang = nCeHouBuChang;	  
	close();
	//chenyuzhu add end on 2014-7-11
	/*
	strTanTouNumber = query.value(4).toString(); //Detector
	g_strBiaoZhun = query.value(6).toString(); //TanShangBiaoZhun
	strTanShangJiBie = query.value(7).toString(); //TanShangJiBieBody
	strTanShangJiBieEdge = query.value(8).toString(); //TanShangJiBieEdge
	strTanShangJiBie = strTanShangJiBie.trimmed();
	strTanShangJiBieEdge = strTanShangJiBieEdge.trimmed();
	if (strTanShangJiBieEdge != (""))
	{
		g_strJiBie = strTanShangJiBie +(" / ") + strTanShangJiBieEdge;
	}
	else
		g_strJiBie = strTanShangJiBie;	
	strTanTouNumber = strTanTouNumber.trimmed();
	strSql = QString("%1%2%3").arg("Select * from t_Detector where name = '").arg(strTanTouNumber).arg("'");
	query.clear();
	query.exec(strSql);
	query.next();
	QString strDelayLength  = query.value(11).toString();
	QString strCeHouBuChang = query.value(9).toString();
	g_strTanTouName = query.value(1).toString();
	g_fDelayLength = strDelayLength.toFloat();
	g_nCeHouBuChang = strCeHouBuChang.toInt();	  
	close();
	*/
}

void CTechnologySetDialog::keyPressEvent(QKeyEvent *e)    //过滤ESC
{
	if (e->key() == Qt::Key_Home)
	{
		return;
	}else
	{
		QWidget::keyPressEvent(e);
	}

}
