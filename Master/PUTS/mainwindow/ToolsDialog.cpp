/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 工具面板
* Filename: ToolsDialog.cpp
*
* @author        DongQi
* @version       1.00         Date: 2013-05-30
*/

#include "ToolsDialog.h"
#include <QStringListModel>
#include <QStringList>
#include <QString>
#include "mainwindow.h"
#include <QMessageBox>
#include <QTextEdit>

//chenyuzhu add start on 2014-6-16
bool *g_bInjPointFilter = NULL;
bool *g_bFilterBiger = NULL;
bool *g_bFilterSmaller = NULL;
int *g_nFilterBigerNum = NULL;
int *g_nFilterSmallerNum = NULL;
//chenyuzhu add end on 2014-6-16
bool *g_bInjPointPath = NULL;//chenyuzhu add start on 2014-11-4
extern bool g_DepthFilterSwitch;
extern CIniParams g_IniParams;
 
CToolsDialog::CToolsDialog(CToolsDialogInfo *pToolsDialogInfo,QWidget *parent)
									:QDialog(parent,Qt::CustomizeWindowHint|Qt::WindowTitleHint),m_pTToolsDialogInfo(pToolsDialogInfo)
{
	InitDialog();
	ReadToolDialogInfo();
	translate();
//连接信号和槽
    connect(m_plistCategory,SIGNAL(currentRowChanged(int)),this,SLOT(OnListWidgetChange(int)));
	connect(m_pOK,SIGNAL(clicked()),this,SLOT(OnOK()));
	connect(m_pCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
	connect(m_pLogViewPage->m_pbVersionLog,SIGNAL(clicked()),this,SLOT(OnDetectingLogFile()));
	connect(m_pLogViewPage->m_pbEventLog,SIGNAL(clicked()),this,SLOT(OnCidentLogFile()));//chenyuzhu add on 2014-6-24
	
	//chenyuzhu add start on 2014-6-16
	 g_bInjPointFilter = &m_pTToolsDialogInfo->m_bInjPointFilter;
	 g_bFilterBiger = &m_pTToolsDialogInfo->m_bFilterBiger;
	 g_bFilterSmaller = &m_pTToolsDialogInfo->m_bFilterSmaller;
	 g_nFilterBigerNum = &m_pTToolsDialogInfo->m_nFilterBigerNum;
	 g_nFilterSmallerNum = &m_pTToolsDialogInfo->m_nFilterSmallerNum;
	 
	 //chenyuzhu add end on 2014-6-16
	 g_bInjPointPath = &m_pTToolsDialogInfo->m_bInjPointPath;
}


CToolsDialog::~CToolsDialog(void)
{
	if (NULL != m_pDetectTextEdit) {
		delete m_pDetectTextEdit;
		m_pDetectTextEdit = NULL;
	}

	//chenyuzhu add start on 2014-6-24
	if (NULL != m_pCidentLogTextEdit) {
		delete m_pCidentLogTextEdit;
		m_pCidentLogTextEdit = NULL;
	}
	//chenyuzhu add end on 2014-6-24
}

void CToolsDialog::InitDialog(void)
{
	setWindowOpacity(0.9);
	m_IniFlieName = QDir::currentPath() += "/PUTS.ini";
	m_pSettings = new QSettings(m_IniFlieName, QSettings::IniFormat); //初始化配置对象

	setStyleSheet("CToolsDialog{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);font-size:20px;color:rgb(255,255,255)}\
				  QPushButton{font-size:20px;color:rgb(255,255,255)}\
				  QLabel{font-size:20px;color:rgb(255,255,255)}\
				  QListWidget{border: 1px solid rgb(255,255,255);font-size:20px;color: rgb(255,255,255)}");
    
	resize(800, 600);
	m_pMainGridLayout = new QGridLayout(this);
	m_pMainGridLayout->setSpacing(11);
	m_pMainGridLayout->setContentsMargins(11, 11, 11, 11);

	m_pButtonLayout = new QHBoxLayout();
	m_pButtonLayout->setSpacing(6);

	m_pOK = new QPushButton(this);
    m_pOK->setMaximumWidth(130);
	m_pButtonLayout->addWidget(m_pOK);

	m_pCancel = new QPushButton(this);
    m_pCancel->setMaximumWidth(130);
	m_pButtonLayout->addWidget(m_pCancel);

    m_pMainGridLayout->addLayout(m_pButtonLayout, 3, 1, 1, 1);

	
	m_pButtonHLayout = new QHBoxLayout();
	m_pButtonHLayout->setSpacing(6);
	
	m_pRestoreDefault = new QPushButton(this);
	m_pRestoreDefault->setMaximumWidth(130);
	m_pButtonHLayout->addWidget(m_pRestoreDefault);

    m_pLilunBiaoding = new QPushButton(this);
    m_pLilunBiaoding->setMaximumWidth(130);
    m_pButtonHLayout->addWidget(m_pLilunBiaoding);

	m_pMainGridLayout->addLayout(m_pButtonHLayout, 3, 0, 1, 1);

	QFont font;
	font.setPointSize(20);

	m_plblCategory = new QLabel(this);
	m_plblCategory->setFont(font);
	m_pMainGridLayout->addWidget(m_plblCategory, 0, 0, 2, 1);

    m_pDetailedPanel = new QLabel(this);
	m_pDetailedPanel->setFont(font);
	m_pMainGridLayout->addWidget(m_pDetailedPanel, 0, 1, 1, 1);

	m_pSWDetailedPanel = new QStackedWidget(this);
	
	m_pInConPointPage = new CInjPointControl(this);
	m_pSWDetailedPanel->addWidget(m_pInConPointPage);

	m_pLogViewPage   = new CLogViewWidget(this);
	m_pSWDetailedPanel->addWidget(m_pLogViewPage);

	m_pPrintControlPage = new CPrintReportControl(this);  //打印报表控制
	m_pSWDetailedPanel->addWidget(m_pPrintControlPage);

	m_pOtherControlPage = new COtherControlWidget(this);  //其他控制
	m_pSWDetailedPanel->addWidget(m_pOtherControlPage);

	m_pUserManager = new UserManager(this);  //其他控制
	m_pSWDetailedPanel->addWidget(m_pUserManager);
	m_pMainGridLayout->addWidget(m_pSWDetailedPanel, 2, 1, 1, 1);

	m_plistCategory = new QListWidget(this);  //列表控件
	m_pItemInConPoint = new QListWidgetItem(tr("伤点控制"),m_plistCategory);  //伤点控制Item
	m_pItemLogView= new QListWidgetItem(tr("日志查看"),m_plistCategory);     //日志查看Item
	m_pItemPrintControl = new QListWidgetItem(tr("打印报表控制"),m_plistCategory); //打印报表控制Item
	m_pItemOtherContro = new QListWidgetItem(tr("其他控制"),m_plistCategory); //其他控制Item
//	m_pItemHelp= new QListWidgetItem(tr("帮助"),m_plistCategory);//帮助Item
	m_pItemUserManage= new QListWidgetItem(tr("用户账号信息"),m_plistCategory);//帮助Item
	m_pMainGridLayout->addWidget(m_plistCategory, 2, 0, 1, 1);

	m_pDetectTextEdit = new QTextEdit(tr("监测日志")); //显示检测日志
	m_pDetectTextEdit->setWindowFlags(Qt::WindowStaysOnTopHint);
	m_pDetectTextEdit->setReadOnly(true);
	m_pDetectTextEdit->resize(400,600);
	m_pDetectTextEdit->setStyleSheet("QTextEdit{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,0)}");

	//chenyuzhu add start on 2014-6-24
	m_pCidentLogTextEdit = new QTextEdit(tr("事件日志")); //显示事件日志
	m_pCidentLogTextEdit->setWindowFlags(Qt::WindowStaysOnTopHint);
	m_pCidentLogTextEdit->setReadOnly(true);
	m_pCidentLogTextEdit->resize(400,600);
	m_pCidentLogTextEdit->setStyleSheet("QTextEdit{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,0)}");
	//chenyuzhu add end on 2014-6-24
}

void CToolsDialog::OnDetectingLogFile()             //打开探测日志
{
	//m_pDetectTextEdit->setta
	QString curPath = QDir::currentPath() + QString("\\RunningDetect.txt");
	QFile file(curPath);
	bool bRet = file.open(QIODevice::ReadOnly);
	if (!bRet)
	{
		QMessageBox::information(this,"CLogViewWidget::OnDetectingLogFile",tr("RunningDetect.txt打开失败"));
		return;
	}
	m_pDetectTextEdit->setText(QString(file.readAll()));
	m_pDetectTextEdit->show();
	m_pOK->click();
}

//chenyuzhu add start on 2014-6-24
void CToolsDialog::OnCidentLogFile()
{
	QString curPath = QDir::currentPath() + QString("/CidentLog.txt");
	
	emit SignalOnCidentLog();


	QFile file(curPath);
	bool bRet = file.open(QIODevice::ReadOnly);
	if (!bRet)
	{
		QMessageBox::information(this,"CLogViewWidget::OnCidentLogFile",tr("CidentLog.txt打开失败"));
		return;
	}
	m_pCidentLogTextEdit->setText(QString(file.readAll()));
	m_pCidentLogTextEdit->show();
	m_pOK->click();
}
//chenyuzhu add end on 2014-6-24

void CToolsDialog::translate(void)
{
	setWindowTitle(tr("工具面板"));
	m_plblCategory->setText(tr("类别"));
	m_pDetailedPanel->setText(tr("详细面板"));
	m_pCancel->setText(tr("取消"));
	m_pOK->setText(tr("确定"));
	m_pRestoreDefault->setText(tr("恢复默认"));
    m_pLilunBiaoding->setText(tr("全部标定"));
}


//重置按钮的显示
void CToolsDialog::OnListWidgetChange(int Index)
{
	m_pSWDetailedPanel->setCurrentIndex(Index);
    if (0 == Index)
    {
		m_pRestoreDefault->show();
    }else
	{
		m_pRestoreDefault->hide();
	}
}


void CToolsDialog::OnOK(void)
{
	m_pTToolsDialogInfo->m_bBottonEdgePicture = m_pInConPointPage->m_pBottonCheck->isChecked();
    m_pTToolsDialogInfo->m_bTopEdgePicture = m_pInConPointPage->m_pTopCheck->isChecked();
	m_pTToolsDialogInfo->m_bLeftEdgePicture = m_pInConPointPage->m_pLeftCheck->isChecked();
	m_pTToolsDialogInfo->m_bRightEdgePicture = m_pInConPointPage->m_pRightCheck->isChecked();
	m_pTToolsDialogInfo->m_bThickNessPicture = m_pInConPointPage->m_pCThicknessView->isChecked();
	m_pTToolsDialogInfo->m_bFrontPicture = m_pInConPointPage->m_pCFrontView->isChecked();
	m_pTToolsDialogInfo->m_bSidePicture = m_pInConPointPage->m_pCSideView->isChecked();
	m_pTToolsDialogInfo->m_bWholePicture = m_pInConPointPage->m_pCOverView->isChecked();
	m_pTToolsDialogInfo->m_bCentreCrosingLine = m_pInConPointPage->m_pReticleCheck->isChecked();
	m_pTToolsDialogInfo->m_bInjPointRangeLine = m_pInConPointPage->m_pPointRangeCheck->isChecked();
	m_pTToolsDialogInfo->m_bInjPointPath = m_pInConPointPage->m_pWoundPathCheck->isChecked();
	m_pTToolsDialogInfo->m_bInjPointFilter = m_pInConPointPage->m_pWondFilterCheck->isChecked();
	m_pTToolsDialogInfo->m_bFilterBiger = m_pInConPointPage->m_pWFBigerCheck->isChecked();
	m_pTToolsDialogInfo->m_bFilterSmaller = m_pInConPointPage->m_pWFSmallerCheck->isChecked();

	m_pTToolsDialogInfo->m_nFilterBigerNum = m_pInConPointPage->m_pleWFBigger->text().toInt() * 100;	// chenyuzhu modify on 2014-10-30
	m_pTToolsDialogInfo->m_nFilterSmallerNum = m_pInConPointPage->m_pleWFSmaller->text().toInt() * 100; // chenyuzhu modify on 2014-10-30
	m_pTToolsDialogInfo->m_bNextDefectDistant = m_pInConPointPage->m_pDefectDistantCheck->isChecked();
	m_pTToolsDialogInfo->m_nDefectDistaneSmaller = m_pInConPointPage->m_pleNextDistant->text().toInt();

	g_DepthFilterSwitch = m_pInConPointPage->m_pDepthFilterCheck->isChecked();
	g_IniParams.m_fFrontMinZ = m_pInConPointPage->m_HeadDepthFilterBox->text().toDouble();
	g_IniParams.m_fBackMinZ = m_pInConPointPage->m_EndDepthFilterBox->text().toDouble();
	g_IniParams.m_fEdgeMinZ = m_pInConPointPage->m_EdgeDepthFilterBox->text().toDouble();
	g_IniParams.m_fminZ = m_pInConPointPage->m_BodyDepthFilterBox->text().toDouble();
	g_IniParams.m_fFrontMinZEnd = m_pInConPointPage->m_HeadDepthFilterBox2->text().toDouble();
	g_IniParams.m_fBackMinZEnd = m_pInConPointPage->m_EndDepthFilterBox2->text().toDouble();
	g_IniParams.m_fEdgeMinZEnd = m_pInConPointPage->m_EdgeDepthFilterBox2->text().toDouble();
	g_IniParams.m_fminZEnd = m_pInConPointPage->m_BodyDepthFilterBox2->text().toDouble();

	m_pSettings->setValue("ToolsDialogInfo/g_DepthFilterSwitch", QString::number(g_DepthFilterSwitch));
	m_pSettings->setValue("ToolsDialogInfo/m_fFrontMinZ", QString::number(g_IniParams.m_fFrontMinZ));
	m_pSettings->setValue("ToolsDialogInfo/m_fBackMinZ", QString::number(g_IniParams.m_fBackMinZ));
	m_pSettings->setValue("ToolsDialogInfo/m_fEdgeMinZ", QString::number(g_IniParams.m_fEdgeMinZ));
	m_pSettings->setValue("ToolsDialogInfo/m_fminZ", QString::number(g_IniParams.m_fminZ));
	m_pSettings->setValue("ToolsDialogInfo/m_fFrontMinZEnd", QString::number(g_IniParams.m_fFrontMinZEnd));
	m_pSettings->setValue("ToolsDialogInfo/m_fBackMinZEnd", QString::number(g_IniParams.m_fBackMinZEnd));
	m_pSettings->setValue("ToolsDialogInfo/m_fEdgeMinZEnd", QString::number(g_IniParams.m_fEdgeMinZEnd));
	m_pSettings->setValue("ToolsDialogInfo/m_fminZEnd", QString::number(g_IniParams.m_fminZEnd));

    m_pTToolsDialogInfo->m_bDependOnMInput = m_pPrintControlPage->m_pCDependOnMInput->isChecked();
	m_pTToolsDialogInfo->m_bPrintInjPointDetailPic = m_pPrintControlPage->m_pCPrintPointPicture->isChecked();

	m_pTToolsDialogInfo->m_strAddress = m_pOtherControlPage->m_pleAddress->text();
	m_pTToolsDialogInfo->m_strUserName = m_pOtherControlPage->m_pleUserName->text();
	m_pTToolsDialogInfo->m_strPwd=m_pOtherControlPage->m_plePWD->text();

	//保存数据库配置到.cfg文件
	QFile fileCfg(QString(QDir::currentPath() + QString("/database.cfg"))) ;
	bool bRet = fileCfg.open(QIODevice::ReadWrite);
	if (!bRet) {
		QMessageBox::information(this,"ReadToolDialogInfo",tr("数据库文件打开失败"));
	}

	QString textCfg(fileCfg.readAll().data());
	fileCfg.close();
	QStringList tempList = textCfg.split(";");
	QString strTemp;
	for (int i = 0;i<tempList.size();i++) {//控制面板确定后 总是保存不上数据库密码原来为for (int i = 0;i<tempList.size()-1;i++) chenyuzhu modify on 2014-6-17
		if (1 == i) {
			strTemp = strTemp + QString("Server=") + m_pTToolsDialogInfo->m_strAddress + QString(";");
		} else if (4 == i) {
			strTemp = strTemp + QString("Uid=") + m_pTToolsDialogInfo->m_strUserName + QString(";");
		} else if(5 == i) {
			strTemp = strTemp + QString("Pwd=") + m_pTToolsDialogInfo->m_strPwd + QString(";");
		} else {
			strTemp = strTemp + tempList.at(i) + QString(";");
		}
	}

	fileCfg.open(QIODevice::WriteOnly | QIODevice::Truncate);	
	fileCfg.write(strTemp.toLocal8Bit().data());
	fileCfg.close();	

	emit SignalToUpdateCscan();// chenyuzhu add on 2014-7-24
	close();
}



void CToolsDialog::ReadToolDialogInfo(void)
{
	m_pTToolsDialogInfo->m_bTopEdgePicture = m_pSettings->value("ToolsDialogInfo/TopEdgePicture","").toBool();
	m_pTToolsDialogInfo->m_bBottonEdgePicture = m_pSettings->value("ToolsDialogInfo/BottonEdgePicture","").toBool();
	m_pTToolsDialogInfo->m_bLeftEdgePicture = m_pSettings->value("ToolsDialogInfo/LeftEdgePicture","").toBool();
	m_pTToolsDialogInfo->m_bRightEdgePicture = m_pSettings->value("ToolsDialogInfo/RightEdgePicture","").toBool();
	m_pTToolsDialogInfo->m_bThickNessPicture= m_pSettings->value("ToolsDialogInfo/ThickNessPicture","").toBool();
	m_pTToolsDialogInfo->m_bFrontPicture= m_pSettings->value("ToolsDialogInfo/FrontPicture","").toBool();
	m_pTToolsDialogInfo->m_bSidePicture= m_pSettings->value("ToolsDialogInfo/SidePicture","").toBool();
	m_pTToolsDialogInfo->m_bWholePicture= m_pSettings->value("ToolsDialogInfo/WholePicture","").toBool();
	m_pTToolsDialogInfo->m_bCentreCrosingLine= m_pSettings->value("ToolsDialogInfo/CentreCrosingLine","").toBool();
	m_pTToolsDialogInfo->m_bInjPointRangeLine= m_pSettings->value("ToolsDialogInfo/InjPointRangeLine","").toBool();
	m_pTToolsDialogInfo->m_bInjPointPath= m_pSettings->value("ToolsDialogInfo/InjPointPath","").toBool();
	m_pTToolsDialogInfo->m_bInjPointFilter= m_pSettings->value("ToolsDialogInfo/InjPointFilter","").toBool();
	m_pTToolsDialogInfo->m_bFilterBiger= m_pSettings->value("ToolsDialogInfo/FilterBiger","").toBool();
	m_pTToolsDialogInfo->m_bFilterSmaller= m_pSettings->value("ToolsDialogInfo/FilterSmaller","").toBool();
	m_pTToolsDialogInfo->m_nFilterBigerNum= m_pSettings->value("ToolsDialogInfo/FilterBigerNum","").toInt();
	m_pTToolsDialogInfo->m_nFilterSmallerNum= m_pSettings->value("ToolsDialogInfo/FilterSmallerNum","").toInt();
	m_pTToolsDialogInfo->m_bNextDefectDistant= m_pSettings->value("ToolsDialogInfo/NextDefectDistant","").toBool();
	m_pTToolsDialogInfo->m_nDefectDistaneSmaller= m_pSettings->value("ToolsDialogInfo/DefectDistaneSmaller","").toInt();
	m_pTToolsDialogInfo->m_bDependOnMInput= m_pSettings->value("ToolsDialogInfo/DependOnMInput","").toBool();
	m_pTToolsDialogInfo->m_bPrintInjPointDetailPic= m_pSettings->value("ToolsDialogInfo/PrintInjPointDetailPic","").toBool();

	g_DepthFilterSwitch = m_pSettings->value("ToolsDialogInfo/g_DepthFilterSwitch").toBool();
	g_IniParams.m_fFrontMinZ = m_pSettings->value("ToolsDialogInfo/m_fFrontMinZ").toDouble();
	g_IniParams.m_fBackMinZ = m_pSettings->value("ToolsDialogInfo/m_fBackMinZ").toDouble();
	g_IniParams.m_fEdgeMinZ = m_pSettings->value("ToolsDialogInfo/m_fEdgeMinZ").toDouble();
	g_IniParams.m_fminZ = m_pSettings->value("ToolsDialogInfo/m_fminZ").toDouble();
	g_IniParams.m_fFrontMinZEnd = m_pSettings->value("ToolsDialogInfo/m_fFrontMinZEnd").toDouble();
	g_IniParams.m_fBackMinZEnd = m_pSettings->value("ToolsDialogInfo/m_fBackMinZEnd").toDouble();
	g_IniParams.m_fEdgeMinZEnd = m_pSettings->value("ToolsDialogInfo/m_fEdgeMinZEnd").toDouble();
	g_IniParams.m_fminZEnd = m_pSettings->value("ToolsDialogInfo/m_fminZEnd").toDouble();

	//读.CFG配置文件数据库信息
	QFile fileCfg(QString(QDir::currentPath() + QString("/database.cfg"))) ;
	bool bRet = fileCfg.open(QIODevice::ReadOnly);
	if (!bRet)
	{
		QMessageBox::information(this,"ReadToolDialogInfo",tr("数据库文件打开失败"));
	}
	QString textCfg(fileCfg.readAll().data());
	QStringList FieldList = textCfg.split(QString(";"));
	QString StrAddress = FieldList.at(1);
	m_pTToolsDialogInfo->m_strAddress = StrAddress.right(StrAddress.size() - StrAddress.indexOf("=")-1);	
	QString StrUserName= FieldList.at(4);
	m_pTToolsDialogInfo->m_strUserName = StrUserName.right(StrUserName.size() - StrUserName.indexOf("=")-1);
	QString StrPwd= FieldList.at(5);
	m_pTToolsDialogInfo->m_strPwd = StrPwd.right(StrPwd.size() - StrPwd.indexOf("=")-1);
    //显示到界面上
	m_pOtherControlPage->m_pleAddress->setText(m_pTToolsDialogInfo->m_strAddress);
	m_pOtherControlPage->m_pleUserName->setText(m_pTToolsDialogInfo->m_strUserName);
	m_pOtherControlPage->m_plePWD->setText(m_pTToolsDialogInfo->m_strPwd);
	fileCfg.close();

	m_pInConPointPage->m_pBottonCheck->setChecked(m_pTToolsDialogInfo->m_bBottonEdgePicture);
	m_pInConPointPage->m_pTopCheck->setChecked(m_pTToolsDialogInfo->m_bTopEdgePicture);
	m_pInConPointPage->m_pLeftCheck->setChecked(m_pTToolsDialogInfo->m_bLeftEdgePicture);
	m_pInConPointPage->m_pRightCheck->setChecked(m_pTToolsDialogInfo->m_bRightEdgePicture);
	m_pInConPointPage->m_pCThicknessView->setChecked(m_pTToolsDialogInfo->m_bThickNessPicture);
	m_pInConPointPage->m_pCFrontView->setChecked(m_pTToolsDialogInfo->m_bFrontPicture);
	m_pInConPointPage->m_pCSideView->setChecked(m_pTToolsDialogInfo->m_bSidePicture);
	m_pInConPointPage->m_pCOverView->setChecked(m_pTToolsDialogInfo->m_bWholePicture);
	m_pInConPointPage->m_pReticleCheck->setChecked(m_pTToolsDialogInfo->m_bCentreCrosingLine);
	m_pInConPointPage->m_pPointRangeCheck->setChecked(m_pTToolsDialogInfo->m_bInjPointRangeLine);
	m_pInConPointPage->m_pWoundPathCheck->setChecked(m_pTToolsDialogInfo->m_bInjPointPath);
	m_pInConPointPage->m_pWondFilterCheck->setChecked(m_pTToolsDialogInfo->m_bInjPointFilter);
	m_pInConPointPage->m_pWFBigerCheck->setChecked(m_pTToolsDialogInfo->m_bFilterBiger);
	m_pInConPointPage->m_pWFSmallerCheck->setChecked(m_pTToolsDialogInfo->m_bFilterSmaller);
	QString temp = QString("%1").arg(m_pTToolsDialogInfo->m_nFilterBigerNum);
	m_pInConPointPage->m_pleWFBigger->setText(temp);
	temp = QString("%1").arg(m_pTToolsDialogInfo->m_nFilterSmallerNum);
	m_pInConPointPage->m_pleWFSmaller->setText(temp);
	m_pInConPointPage->m_pDefectDistantCheck->setChecked(m_pTToolsDialogInfo->m_bNextDefectDistant);
	temp = QString("%1").arg(m_pTToolsDialogInfo->m_nDefectDistaneSmaller);
	m_pInConPointPage->m_pleNextDistant->setText(temp);
	m_pPrintControlPage->m_pCDependOnMInput->setChecked(m_pTToolsDialogInfo->m_bDependOnMInput);
	m_pPrintControlPage->m_pCPrintPointPicture->setChecked(m_pTToolsDialogInfo->m_bPrintInjPointDetailPic);
	m_pOtherControlPage->m_pleAddress->setText(m_pTToolsDialogInfo->m_strAddress);
	m_pOtherControlPage->m_pleUserName->setText(m_pTToolsDialogInfo->m_strUserName);
	m_pOtherControlPage->m_plePWD->setText(m_pTToolsDialogInfo->m_strPwd);

	m_pInConPointPage->m_pDepthFilterCheck->setChecked(g_DepthFilterSwitch);
	m_pInConPointPage->m_HeadDepthFilterBox->setText(QString("%1").arg(g_IniParams.m_fFrontMinZ));
	m_pInConPointPage->m_EndDepthFilterBox->setText(QString("%1").arg(g_IniParams.m_fBackMinZ));
	m_pInConPointPage->m_EdgeDepthFilterBox->setText(QString("%1").arg(g_IniParams.m_fEdgeMinZ));
	m_pInConPointPage->m_BodyDepthFilterBox->setText(QString("%1").arg(g_IniParams.m_fminZ));
	m_pInConPointPage->m_HeadDepthFilterBox2->setText(QString("%1").arg(g_IniParams.m_fFrontMinZEnd));
	m_pInConPointPage->m_EndDepthFilterBox2->setText(QString("%1").arg(g_IniParams.m_fBackMinZEnd));
	m_pInConPointPage->m_EdgeDepthFilterBox2->setText(QString("%1").arg(g_IniParams.m_fEdgeMinZEnd));
	m_pInConPointPage->m_BodyDepthFilterBox2->setText(QString("%1").arg(g_IniParams.m_fminZEnd));

	if (m_pInConPointPage->m_pWondFilterCheck->isChecked()) {	
		m_pInConPointPage->m_pWFBigerCheck->setEnabled(true);            
		m_pInConPointPage->m_pWFSmallerCheck->setEnabled(true);
		m_pInConPointPage->m_pleWFBigger->setEnabled(true);
		m_pInConPointPage->m_pleWFSmaller->setEnabled(true);
	} else {
		m_pInConPointPage->m_pWFBigerCheck->setEnabled(false);            
		m_pInConPointPage->m_pWFSmallerCheck->setEnabled(false);
		m_pInConPointPage->m_pleWFBigger->setEnabled(false);
		m_pInConPointPage->m_pleWFSmaller->setEnabled(false);
	}
}

void CToolsDialog::OnCancel(void)
{
	emit UpdateMainWindowOnCancel();
	close();
}



