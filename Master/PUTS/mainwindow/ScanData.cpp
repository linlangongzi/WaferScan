/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved.
*
* 数据查询界面
* Filename: ScanData.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-10-09
*/


#include <QSqlDatabase>
#include <QMessageBox>
#include <QString>
#include <QModelIndex>
#include <QList>
#include "ScanData.h"
#include "DataDefine.h"


long gTestMode = 0;
extern QSqlDatabase* g_sqlDB;
extern PLATE_INFO g_Info[1];	//chenyuzhu add on 2014-5-28
extern int nPlate;			//chenyuzhu add on 2014-5-28
extern QString g_strBiaoZhun;//chenyuzhu add on 2014-5-28
extern QString g_strJiBie;	//chenyuzhu add on 2014-5-28
extern bool g_bQieHuan;

ScanData::ScanData(QWidget * parent):QDialog(parent,Qt::CustomizeWindowHint|Qt::WindowTitleHint)
{
    setStyleSheet("QGroupBox{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px}\
                  QLabel{color:rgb(255,255,0);font-size:18px}\
                  QLineEdit{border-width: 1px; border-style: solid;border-color: rgb(255, 255, 255);color:rgb(255,255,0);font-size:18px}\
                  QRadioButton{background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px}\
                  QPushButton{color:rgb(255,255,0);font-size:18px}}");
    InitDialog();
    translate();

    //chenyuzhu add start on 2014-11-2
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    m_cbEndTime->setDate(date);
    m_spEndTime->setTime(time);
    //chenyuzhu add end on 2014-11-2

    connect(m_rbOrderCheck,SIGNAL(toggled(bool)),this,SLOT(OnRadioSwitch(bool)));
    connect(m_rbPlateNCheck,SIGNAL(toggled(bool)),this,SLOT(OnRadioSwitch(bool)));
    connect(m_rbDateCheck,SIGNAL(toggled(bool)),this,SLOT(OnRadioSwitch(bool)));
    connect(m_pbCheck,SIGNAL(clicked()),this,SLOT(OnSearchButtom()));
    connect(m_tvDetial,SIGNAL(doubleClicked(const QModelIndex&)),this,SLOT(OnDoubleClicked(const QModelIndex&)));
    connect(m_pbExit,SIGNAL(clicked()),this,SLOT(OnExit()));
	connect(m_rbGongyiCheck, SIGNAL(toggled(bool)), this, SLOT(OnRadioSwitch(bool)));

    //chenyuzhu add start on 2014-10-13
    Qt::WindowFlags flags=Qt::Dialog;

    flags |=Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags);

    //chenyuzhu add end on 2014-10-13
}

ScanData::~ScanData(void)
{
    if (NULL != m_tvDetial)
    {
        delete m_tvDetial;
        m_tvDetial = NULL;
    }
    if (NULL != m_pDetialInfoModel)
    {
        delete m_pDetialInfoModel;
        m_pDetialInfoModel = NULL;
    }

}

void ScanData::OnRadioSwitch(bool flag)
{
    if (sender() == m_rbOrderCheck) //序号
    {
        if (!flag)
        {
            m_leOrder->setDisabled(true);
            m_gOrder->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_leOrder->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
        }else
        {
            m_leOrder->setEnabled(true);
            m_gOrder->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_leOrder->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
        }
    }
    if (sender() == m_rbPlateNCheck)  //钢板号
    {
        if (!flag)
        {
            m_lePlateNM->setDisabled(true);
            m_lePlateNM->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_gPlateNM->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
        }else
        {
            m_lePlateNM->setEnabled(true);
            m_lePlateNM->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_gPlateNM->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
        }
    }
    if (sender() == m_rbDateCheck)  //日期
    {
        if (!flag)
        {
            m_cbStartTime->setDisabled(true);
            m_cbEndTime->setDisabled(true);
            m_spStartTime->setDisabled(true);
            m_spEndTime->setDisabled(true);
            m_gDate->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_lbStartTime->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_lbEndTime->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_cbStartTime->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_cbEndTime->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_spStartTime->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
            m_spEndTime->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
        }else
        {
            m_cbStartTime->setEnabled(true);
            m_cbEndTime->setEnabled(true);
            m_spStartTime->setEnabled(true);
            m_spEndTime->setEnabled(true);
            m_gDate->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_lbStartTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_lbEndTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_cbStartTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_cbEndTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_spStartTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
            m_spEndTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
        }
    }
	if (sender() == m_rbGongyiCheck)  //工艺切换
	{
		if (flag)
		{
			g_bQieHuan = true;
		}
		else
		{
			g_bQieHuan = false;
		}
	}
}


void ScanData::InitDialog()
{
    m_nCurrentIndex = -1;              //当前选中行号
    setMaximumSize(QSize(1003, 579));
    setMinimumSize(QSize(1003, 579));
    m_gTopGroup = new QGroupBox(this);
    m_gTopGroup->setGeometry(QRect(40, 20, 450, 41));
    m_rbOrderCheck = new QRadioButton(m_gTopGroup);
    m_rbOrderCheck->setGeometry(QRect(20, 10, 110, 18));
    m_rbPlateNCheck = new QRadioButton(m_gTopGroup);
    m_rbPlateNCheck->setGeometry(QRect(170, 10, 130, 18));
    m_rbDateCheck = new QRadioButton(m_gTopGroup);
    m_rbDateCheck->setGeometry(QRect(320, 10, 110, 18));

    m_gGongyiGroup = new QGroupBox(this);
    m_gGongyiGroup->setGeometry(QRect(500, 20, 160, 41));
    m_rbGongyiCheck = new QRadioButton(m_gGongyiGroup);
    m_rbGongyiCheck->setGeometry(QRect(30, 10, 110, 18));

    //chenyuzhu add start on 2014.5.13
    m_gShibanGroup = new QGroupBox(this);
    m_gShibanGroup->setGeometry(QRect(670, 20, 290, 41));
    m_rbShibanCheck = new QRadioButton(m_gShibanGroup);
    m_rbShibanCheck->setGeometry(QRect(30, 10, 110, 18));
    m_rbUnShibanCheck = new QRadioButton(m_gShibanGroup);
    m_rbUnShibanCheck->setGeometry(QRect(160, 10, 110, 18));
    //chenyuzhu add end on 2014.5.13

    m_gOrder = new QGroupBox(this);
    m_gOrder->setGeometry(QRect(40, 70, 151, 101));
    m_leOrder = new QLineEdit(m_gOrder);
    m_leOrder->setGeometry(QRect(20, 39, 113, 25));
    m_gPlateNM = new QGroupBox(this);
    m_gPlateNM->setGeometry(QRect(209, 69, 201, 101));
    m_lePlateNM = new QLineEdit(m_gPlateNM);
    m_lePlateNM->setGeometry(QRect(32, 40, 131, 25));
    m_pbCheck = new QPushButton(this);
    m_pbCheck->setGeometry(QRect(430, 80, 91, 81));
    m_gDate = new QGroupBox(this);
    m_gDate->setGeometry(QRect(540, 70, 412, 101));
    m_lbStartTime = new QLabel(m_gDate);
    m_lbStartTime->setGeometry(QRect(25, 20, 80, 21));
    m_lbEndTime = new QLabel(m_gDate);
    m_lbEndTime->setGeometry(QRect(25, 60, 80, 21));
    m_cbStartTime = new QDateEdit(m_gDate);
    m_cbStartTime->setGeometry(QRect(130, 20, 131, 22));
    m_cbEndTime = new QDateEdit(m_gDate);
    m_cbEndTime->setGeometry(QRect(130, 60, 131, 22));
    m_spStartTime = new QTimeEdit(m_gDate);
    m_spStartTime->setGeometry(QRect(280, 20, 91, 22));
    m_spEndTime = new QTimeEdit(m_gDate);
    m_spEndTime->setGeometry(QRect(280, 60, 91, 22));
    m_cbStartTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
    m_cbEndTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
    m_spStartTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
    m_spEndTime->setStyleSheet("border-color: rgb(255, 255, 255);background-color:rgb(73, 73, 73);color:rgb(255,255,0);font-size:18px");
    m_lbCurrentSelect = new QLabel(this);
    m_lbCurrentSelect->setGeometry(QRect(50, 180, 141, 21));
    m_pbExit = new QPushButton(this);
    m_pbExit->setGeometry(QRect(850, 530, 91, 31));
    m_pDetialInfoModel = new QStandardItemModel();
    m_pDetialInfoModel->setColumnCount(17);
    m_pDetialInfoModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));
    m_pDetialInfoModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("编号"));
    m_pDetialInfoModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("探伤工艺"));
    m_pDetialInfoModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("钢板编号"));
    m_pDetialInfoModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("检测时间"));
    m_pDetialInfoModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("检测班次"));
    m_pDetialInfoModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("检测员"));
    m_pDetialInfoModel->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("探伤结果"));
    m_pDetialInfoModel->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("面伤总面积"));
    m_pDetialInfoModel->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("探伤标准"));
    m_pDetialInfoModel->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("探伤级别"));
    m_pDetialInfoModel->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("长"));
    m_pDetialInfoModel->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("宽"));
    m_pDetialInfoModel->setHeaderData(13,Qt::Horizontal,QString::fromLocal8Bit("厚"));
    m_pDetialInfoModel->setHeaderData(14,Qt::Horizontal,QString::fromLocal8Bit("探伤模式"));
    m_pDetialInfoModel->setHeaderData(15,Qt::Horizontal,QString::fromLocal8Bit("班次"));
    m_pDetialInfoModel->setHeaderData(16,Qt::Horizontal,QString::fromLocal8Bit("操作员"));
    m_tvDetial = new QTableView(this);
    m_tvDetial->setGeometry(QRect(40, 200, 911, 321));
    m_tvDetial->setModel(m_pDetialInfoModel);
    m_tvDetial->verticalHeader()->setHidden(true);  //隐藏序号列
    m_tvDetial->setColumnWidth(0,50);  //设置列宽度
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    m_tvDetial->horizontalHeader()->setSectionsClickable(false);
#else
    m_tvDetial->horizontalHeader()->setClickable(false); //设置表头不可点击
#endif
    m_tvDetial->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    m_tvDetial->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tvDetial->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tvDetial->setSelectionMode(QAbstractItemView::SingleSelection);
    //设置TableView样式
    m_tvDetial->horizontalHeader()->setStyleSheet("QHeaderView::section{font-size:18px;color:rgb(255,255,0);background:rgb(73, 73, 73);}");
    m_tvDetial->setStyleSheet("QTableView{background-color:rgb(73, 73, 73);font-size:18px;color:rgb(255,255,0)}");
    //设置初始状态
    m_leOrder->setDisabled(true);
    m_gOrder->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
    m_leOrder->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
    m_lePlateNM->setDisabled(true);
    m_lePlateNM->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
    m_gPlateNM->setStyleSheet("border-color: rgb(0, 0, 0);background-color:rgb(73, 73, 73);color:rgb(0,0,0);font-size:18px");
    m_rbDateCheck->setChecked(true);

    m_rbUnShibanCheck->setChecked(true);//chenyuzhu add on 2014.5.14
}

void ScanData::translate()
{
    setWindowTitle(tr("数据浏览"));
    m_gTopGroup->setTitle(QString());
    m_rbOrderCheck->setText(tr("按序号查询"));
    m_rbPlateNCheck->setText(tr("按钢板号查询"));
    m_rbDateCheck->setText(tr("按日期查询"));
    m_gOrder->setTitle(tr("请输入序号"));
    m_gPlateNM->setTitle(tr("请输入钢板号"));
    m_pbCheck->setText(tr("查询"));
    m_gDate->setTitle(tr("日期时间查询"));
    m_lbStartTime->setText(tr("开始日期"));
    m_lbEndTime->setText(tr("结束日期"));
    m_lbCurrentSelect->setText(tr("当前选择序号为"));
    m_pbExit->setText(tr("退出"));

    m_rbShibanCheck->setText(tr("试板查询"));
    m_rbUnShibanCheck->setText(tr("非试板查询"));
    m_rbGongyiCheck->setText(tr("切换工艺"));

}
//响应查询按钮
void ScanData::OnSearchButtom()
{
    if (m_rbOrderCheck->isChecked()) //按序号查询
    {
        PlateNoQuery();

    }else if (m_rbPlateNCheck->isChecked())//按钢板号查询
    {
        DataPlateNumQuery();

    }else if (m_rbDateCheck->isChecked())//按日期查询
    {
        DataTimeQuery();
    }else
    {

    }


}

void ScanData::PlateNoQuery() //按序号查询
{
    QString strNo = m_leOrder->text();
    if (strNo.isEmpty())
    {
        m_pDetialInfoModel->clear();
        m_pDetialInfoModel->setColumnCount(17);
        m_pDetialInfoModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));
        m_pDetialInfoModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("编号"));
        m_pDetialInfoModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("探伤工艺"));
        m_pDetialInfoModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("钢板编号"));
        m_pDetialInfoModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("检测时间"));
        m_pDetialInfoModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("检测班次"));
        m_pDetialInfoModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("检测员"));
        m_pDetialInfoModel->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("探伤结果"));
        m_pDetialInfoModel->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("面伤总面积"));
        m_pDetialInfoModel->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("探伤标准"));
        m_pDetialInfoModel->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("探伤级别"));
        m_pDetialInfoModel->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("长"));
        m_pDetialInfoModel->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("宽"));
        m_pDetialInfoModel->setHeaderData(13,Qt::Horizontal,QString::fromLocal8Bit("厚"));
        m_pDetialInfoModel->setHeaderData(14,Qt::Horizontal,QString::fromLocal8Bit("探伤模式"));
        m_pDetialInfoModel->setHeaderData(15,Qt::Horizontal,QString::fromLocal8Bit("班次"));
        m_pDetialInfoModel->setHeaderData(16,Qt::Horizontal,QString::fromLocal8Bit("操作员"));
        m_tvDetial->setModel(m_pDetialInfoModel);
        return;
    }
    QSqlQuery query(*g_sqlDB);
    QString strSql = QString("%1%2").arg("select * from T_PlateCheckInfo where ID =").arg(strNo);//chenyuzhu modify on 2014-7-15
    query.exec(strSql);
    UpdateTableView(query);
}

void ScanData::DataPlateNumQuery()//按钢板号查询
{
    QString strPlateNum = m_lePlateNM->text();
    if (strPlateNum.isEmpty())
    {
        m_pDetialInfoModel->clear();
        m_pDetialInfoModel->setColumnCount(17);
        m_pDetialInfoModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));
        m_pDetialInfoModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("编号"));
        m_pDetialInfoModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("探伤工艺"));
        m_pDetialInfoModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("钢板编号"));
        m_pDetialInfoModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("检测时间"));
        m_pDetialInfoModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("检测班次"));
        m_pDetialInfoModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("检测员"));
        m_pDetialInfoModel->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("探伤结果"));
        m_pDetialInfoModel->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("面伤总面积"));
        m_pDetialInfoModel->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("探伤标准"));
        m_pDetialInfoModel->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("探伤级别"));
        m_pDetialInfoModel->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("长"));
        m_pDetialInfoModel->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("宽"));
        m_pDetialInfoModel->setHeaderData(13,Qt::Horizontal,QString::fromLocal8Bit("厚"));
        m_pDetialInfoModel->setHeaderData(14,Qt::Horizontal,QString::fromLocal8Bit("探伤模式"));
        m_pDetialInfoModel->setHeaderData(15,Qt::Horizontal,QString::fromLocal8Bit("班次"));
        m_pDetialInfoModel->setHeaderData(16,Qt::Horizontal,QString::fromLocal8Bit("操作员"));
        m_tvDetial->setModel(m_pDetialInfoModel);
        return;
    }

#if HAVE_DATA_BASE
    QSqlQuery query(*g_sqlDB);
    QString strSql = QString("%1%2%3").arg("select * from T_PlateCheckInfo where Part_ID ='").arg(strPlateNum).arg("'");//chenyuzhu modify on 2014-7-15
    query.exec(strSql);
    UpdateTableView(query);
#else 
	QMessageBox::information(this, "ScanData::DataTimeQuery", tr("未连接数据库"));
#endif
}


void  ScanData::DataTimeQuery() // 按日期查询
{
    QString strStartDate = m_cbStartTime->date().toString("yyyy/MM/dd");
    QString strStartTime = m_spStartTime->time().toString("hh:mm:ss");

    QString strEndDate = m_cbEndTime->date().toString("yyyy/MM/dd");
    QString strEndTime = m_spEndTime->time().toString("hh:mm:ss");
    if ( m_cbStartTime->date()> m_cbEndTime->date())
    {
        QMessageBox::information(this,"ScanData::DataTimeQuery",tr("结束日期不能小于开始日期，请重新输入再查询!!!"));
        return;
    }
    if (m_spStartTime->time() > m_spEndTime->time())
    {
        QMessageBox::information(this,"ScanData::DataTimeQuery",tr("结束时间不能小于开始时间，请重新输入再查询!!!"));
        return;
    }
    //转化成数据库时间类型
    QString strDataStartTime = QString("%1%2%3%4").arg(strStartDate.left(10)).arg(" ").arg(strStartTime.left(8)).arg(".000");
    QString strDataEndTime = QString("%1%2%3%4").arg(strEndDate.left(10)).arg(" ").arg(strEndTime.left(8)).arg(".000");

#if HAVE_DATA_BASE
    QSqlQuery query(*g_sqlDB);
    QString strSql = QString("%1%2%3%4%5").arg("select top 1000 * from T_PlateCheckInfo where Check_Time >='").arg(strDataStartTime).arg("' and Check_Time <= '").arg(strDataEndTime).arg("'");//chenyuzhu modify on 2014-7-15
    query.exec(strSql);
    UpdateTableView(query);
#else 
	QMessageBox::information(this, "ScanData::DataTimeQuery", tr("未连接数据库"));
#endif
}

void ScanData::UpdateTableView(QSqlQuery &query) //更新视图
{
    m_pDetialInfoModel->clear();
    m_pDetialInfoModel->setColumnCount(17);
    m_pDetialInfoModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));
    m_pDetialInfoModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("编号"));
    m_pDetialInfoModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("探伤工艺"));
    m_pDetialInfoModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("钢板编号"));
    m_pDetialInfoModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("检测时间"));
    m_pDetialInfoModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("检测班次"));
    m_pDetialInfoModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("检测员"));
    m_pDetialInfoModel->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("探伤结果"));
    m_pDetialInfoModel->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("面伤总面积"));
    m_pDetialInfoModel->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("探伤标准"));
    m_pDetialInfoModel->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("探伤级别"));
    m_pDetialInfoModel->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("长"));
    m_pDetialInfoModel->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("宽"));
    m_pDetialInfoModel->setHeaderData(13,Qt::Horizontal,QString::fromLocal8Bit("厚"));
    m_pDetialInfoModel->setHeaderData(14,Qt::Horizontal,QString::fromLocal8Bit("探伤模式"));
    m_pDetialInfoModel->setHeaderData(15,Qt::Horizontal,QString::fromLocal8Bit("班次"));
    m_pDetialInfoModel->setHeaderData(16,Qt::Horizontal,QString::fromLocal8Bit("操作员"));
    m_tvDetial->setModel(m_pDetialInfoModel);
    int nRow = 0;
    while (query.next())
    {
        //chenyuzhu add start on 2014.5.13
        if(m_rbShibanCheck->isChecked())//判断是否是试板查询
        {
                if (0 == query.value(4).toInt())//根据从数据库中取出来的数判断是否是试板
                {
                    continue;
                }
        }
        if(m_rbUnShibanCheck->isChecked())//判断是否是非试板查询
        {
                if (1 == query.value(4).toInt())//根据从数据库中取出来的数判断是否是试板
                {
                    continue;
                }
        }
        //chenyuzhu add end on 2014.5.13
        //chenyuzhu modify start on 2014-7-15
        m_pDetialInfoModel->setItem(nRow,0,new QStandardItem(QString::number(nRow+1)));//序号
        m_pDetialInfoModel->setItem(nRow,1,new QStandardItem(query.value(0).toString().trimmed()));//编号
        m_pDetialInfoModel->setItem(nRow,2,new QStandardItem(query.value(13).toString().trimmed()));//探伤工艺
        m_pDetialInfoModel->setItem(nRow,3,new QStandardItem(query.value(2).toString().trimmed()));//钢板编号
        //chenyuzhu modify start on 2014-9-29
        QString strTime = query.value(5).toString().trimmed();
        int  n = strTime.indexOf("T");
        strTime = strTime.replace(n, 1, " ");
        m_pDetialInfoModel->setItem(nRow,4,new QStandardItem(strTime));//检测时间
        //chenyuzhu modify end on 2014-9-29
        m_pDetialInfoModel->setItem(nRow,5,new QStandardItem(query.value(6).toString().trimmed()));//检测班次
        m_pDetialInfoModel->setItem(nRow,6,new QStandardItem(query.value(7).toString().trimmed()));//检测员

        int ntemp = query.value(10).toInt();
        QString strTemp;
        if (1 == ntemp)
        {
            strTemp = "合格";
        }
        else
        {
            strTemp = "不合格";
        }
        m_pDetialInfoModel->setItem(nRow,7,new QStandardItem(strTemp));//探伤结果
        m_pDetialInfoModel->setItem(nRow,8,new QStandardItem(query.value(14).toString().trimmed()));//面伤总面积
        m_pDetialInfoModel->setItem(nRow,9,new QStandardItem(query.value(8).toString().trimmed()));//探伤标准
        //探伤级别
        QString strTanShangJiBie;
        QString temp = query.value(16).toString();
        temp = temp.trimmed();
        if ("" == temp)
        {
            strTanShangJiBie = query.value(15).toString().trimmed();

        }else
        {
            strTanShangJiBie = 	query.value(15).toString().trimmed() + "/" + temp;
        }
        m_pDetialInfoModel->setItem(nRow,10,new QStandardItem(strTanShangJiBie));//探伤级别

        QString strLWh =  query.value(11).toString().trimmed();
        if(!strLWh.isEmpty())
        {
            QStringList strlist = strLWh.split("*");
            m_pDetialInfoModel->setItem(nRow,11,new QStandardItem(strlist.at(0)));//长
            m_pDetialInfoModel->setItem(nRow,12,new QStandardItem(strlist.at(1)));//宽
            m_pDetialInfoModel->setItem(nRow,13,new QStandardItem(strlist.at(2)));//厚
        }
        m_pDetialInfoModel->setItem(nRow,14,new QStandardItem(query.value(23).toString().trimmed()));//探伤模式
        m_pDetialInfoModel->setItem(nRow,15,new QStandardItem(query.value(6).toString().trimmed()));//班次
        m_pDetialInfoModel->setItem(nRow,16,new QStandardItem(query.value(7).toString().trimmed()));//操作员
        nRow++;
        //chenyuzhu modify end on 2014-7-15
    }
    if (!nRow)
    {
        QMessageBox::information(this,"ScanData::PlateNoQuery",tr("无对应记录！！！"));
    }

}


void ScanData::SetRowToTableView(int row)
{
    QModelIndex index = m_pDetialInfoModel->index(row,1);
    QString strNo = m_pDetialInfoModel->data(index).toString();  //编号
    index = m_pDetialInfoModel->index(row,9);
    QString strBiaoZhun = m_pDetialInfoModel->data(index).toString();  //标准
    index = m_pDetialInfoModel->index(row,10);
    QString strJiBie = m_pDetialInfoModel->data(index).toString();  //级别
    index = m_pDetialInfoModel->index(row,3);
    QString strPlateNo = m_pDetialInfoModel->data(index).toString();  //钢板编号
    index = m_pDetialInfoModel->index(row,11);
    QString strLength = m_pDetialInfoModel->data(index).toString();  //长
    index = m_pDetialInfoModel->index(row,12);
    QString strWidth = m_pDetialInfoModel->data(index).toString();  //宽
    index = m_pDetialInfoModel->index(row,13);
    QString strThick = m_pDetialInfoModel->data(index).toString();  //厚
    index = m_pDetialInfoModel->index(row,14);
    QString strTestMode = m_pDetialInfoModel->data(index).toString();  //模式
    strTestMode = strTestMode.trimmed();
    if (strTestMode == tr("A"))
    {
        gTestMode = 1;
    }
    else if (strTestMode == tr("B"))
    {
        gTestMode = 2;
    }
    else if (strTestMode == tr("C"))
    {
        gTestMode =3;
    }
    else
    {
        strTestMode = tr("未知模式");
    }

    index = m_pDetialInfoModel->index(row,15);
    QString strClass = m_pDetialInfoModel->data(index).toString();  //班次
    index = m_pDetialInfoModel->index(row,16);
    QString strOperator = m_pDetialInfoModel->data(index).toString();  //操作者

    if(NULL != strNo)
    {

        m_nCurrentIndex = row; //当前选中行号
        QList<QString> list;
        list<<strNo<<strBiaoZhun<<strJiBie<<strPlateNo<<strLength<<strWidth<<strThick;
        emit SignalDrawSelectedFile(list);     //双击TableView的发出的信号
    }
}

void ScanData::OnDoubleClicked(const QModelIndex& Index) //响应TableView双击消息
{
    QModelIndex index = m_pDetialInfoModel->index(Index.row(),1);
    QString strNo = m_pDetialInfoModel->data(index).toString();  //编号
    index = m_pDetialInfoModel->index(Index.row(),9);
    QString strBiaoZhun = m_pDetialInfoModel->data(index).toString();  //探伤标准
    index = m_pDetialInfoModel->index(Index.row(),10);
    QString strJiBie = m_pDetialInfoModel->data(index).toString();  //探伤级别
    index = m_pDetialInfoModel->index(Index.row(),3);
    QString strPlateNo = m_pDetialInfoModel->data(index).toString();  //钢板编号
    index = m_pDetialInfoModel->index(Index.row(),11);
    QString strLength = m_pDetialInfoModel->data(index).toString();  //长
    index = m_pDetialInfoModel->index(Index.row(),12);
    QString strWidth = m_pDetialInfoModel->data(index).toString();  //宽
    index = m_pDetialInfoModel->index(Index.row(),13);
    QString strThick = m_pDetialInfoModel->data(index).toString();  //厚
    index = m_pDetialInfoModel->index(Index.row(),14);
    QString strTestMode = m_pDetialInfoModel->data(index).toString();  //模式
    strTestMode = strTestMode.trimmed();
    index = m_pDetialInfoModel->index(Index.row(),2);
    QString strArtName = m_pDetialInfoModel->data(index).toString();  //工艺名称
    strArtName = strArtName.trimmed();
    if (strTestMode == tr("A"))
    {
        gTestMode = 1;
    }
    else if (strTestMode == tr("B"))
    {
        gTestMode = 2;
    }
    else if (strTestMode == tr("C"))
    {
        gTestMode =3;
    }
    else
    {
        strTestMode = tr("未知模式");
    }

    index = m_pDetialInfoModel->index(Index.row(),15);
    QString strClass = m_pDetialInfoModel->data(index).toString();  //班次
    index = m_pDetialInfoModel->index(Index.row(),16);
    QString strOperator = m_pDetialInfoModel->data(index).toString();  //操作者

    if(NULL != strNo)
    {
        //chenyuzhu add start on 2014-5-28 //取出数据后将钢板参数更新到g_Info中
        g_Info[nPlate].length = strLength.toFloat();
        g_Info[nPlate].fRealLength = strLength.toFloat();          //探完之后的钢板长度
        g_Info[nPlate].width = strWidth.toFloat();
        g_Info[nPlate].height = strThick.toFloat();
        g_Info[nPlate].fReportHeight = strThick.toFloat();
        g_strBiaoZhun = strBiaoZhun;
        g_strJiBie = strJiBie;
        g_Info[nPlate].plateno = strPlateNo;
        //chenyuzhu add end on 2014-5-28

        m_nCurrentIndex = Index.row(); //当前选中行号
        QList<QString> list;
        list<<strNo<<strBiaoZhun<<strJiBie<<strPlateNo<<strLength<<strWidth<<strThick<<strArtName;
        emit SignalUpdateChannelDock(list); //双击TableView的发出的信号 更新界面信息
        emit SignalDrawSelectedFile(list);     //双击TableView的发出的信号
    }
}

void ScanData::OnExit() //退出按钮
{
    close();
}


void ScanData::OnPreDrawingImage()  //响应向前按钮
{
    if (0 != m_pDetialInfoModel->rowCount()) //TableView中有记录
    {
        if (m_nCurrentIndex == 0)  //当前为最前端
        {
            m_nCurrentIndex = m_pDetialInfoModel->rowCount();
            SetRowToTableView(m_nCurrentIndex);


        }else
        {
            m_nCurrentIndex--;
            SetRowToTableView(m_nCurrentIndex);
        }

    }

}


void ScanData::OnNextDrawingImage()   //响应向后按钮
{
    if (0 != m_pDetialInfoModel->rowCount()) //TableView中有记录
    {
        if (m_nCurrentIndex == m_pDetialInfoModel->rowCount())  //当前为最末端
        {
            m_nCurrentIndex = 0;
            SetRowToTableView(m_nCurrentIndex);


        }else
        {
            m_nCurrentIndex++;
            SetRowToTableView(m_nCurrentIndex);
        }

    }
}
