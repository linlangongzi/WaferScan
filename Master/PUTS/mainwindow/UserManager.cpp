/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved.
*
* 工具面板
* Filename: UserManager.cpp
*
* @author        DongQi
* @version       1.00         Date: 2013-08-26
*/

#include <QColor>
#include <QTableWidgetItem>
#include <QColor>
#include "DataDefine.h"
#include "UserManager.h"

extern QSqlDatabase *g_sqlDB;
extern QString g_strUserPer;

UserManager::UserManager(QWidget* parent):QWidget(parent)
{
    InitDialog();
    InsertDataToTableView();
    translate();
    connect(m_pbAddUser,SIGNAL(clicked()),this,SLOT(OnAddUser()));
    connect(m_pbUserDialog,SIGNAL(UpdateTableView()),this,SLOT(UpdateTable()));
    connect(m_pbEditPassword,SIGNAL(clicked()),this,SLOT(OnEditPwd()));
    connect(m_pEditPwdDlg,SIGNAL(UpdateTableViewFromEditPwd()),this,SLOT(UpdateTable()));
    connect(m_pbDeleteUser,SIGNAL(clicked()),this,SLOT(OnDeleteUser()));
    connect(m_pbEditUserPer,SIGNAL(clicked()),this,SLOT(OnEditUserPermission()));
    connect(m_pEditUserPerDlg,SIGNAL(OnUpdateTableViewFEUPDlg()),this,SLOT(UpdateTable()));
}


void UserManager::OnDeleteUser(void)                //删除记录
{
    int nRow = m_pUserInformation->currentIndex().row();
    QModelIndex modelIndex = m_pUserInfoModel->index(nRow,1);
    QString strUserName = m_pUserInfoModel->data(modelIndex).toString();
    if (strUserName.isEmpty())
    {
        QMessageBox::information(this,"UserManager::OnDeleteUser",tr("请选择要删除的记录"));
        return;
    }
    QMessageBox::StandardButton rb =  QMessageBox::information(this,tr("删除用户"),tr("确认是否删除当前选中用户?"),QMessageBox::Yes|QMessageBox::No);
    if (rb != QMessageBox::Yes )
    {
        return;
    }
    QString strSQL = QString("%1%2%3").arg("delete from t_User where UserName = '").arg(strUserName).arg("'");
    QSqlQuery query(*g_sqlDB);
    query.exec(strSQL);
    if (!query.isActive())
    {
        QMessageBox::information(this,"UserManager::OnDeleteUser",tr("SQL语句执失败"));
        return;
    }
    //更新视图
    UpdateTable();
}

void UserManager::OnEditPwd(void)                   //修改密码
{
    //获得当前的用户名
    int nRow = m_pUserInformation->currentIndex().row();
    QModelIndex modeIndex = m_pUserInfoModel->index(nRow,1);
    QString strUserName = m_pUserInfoModel->data(modeIndex).toString();
    if (strUserName.isEmpty())
    {
        QMessageBox::information(this,"UserManager::OnEditPwd",tr("请选择要更改的用户"));
        return;
    }
    m_pEditPwdDlg->m_strUserName = strUserName;
    m_pEditPwdDlg->InsetDataToDialog();
    m_pEditPwdDlg->exec();
}

void UserManager::OnAddUser(void)
{
    m_pbUserDialog->exec();
}

void UserManager::UpdateTable(void)
{
    InsertDataToTableView();
}


UserManager::~UserManager(void)
{
    m_pUserInfoModel->clear();
    delete m_pEditPwdDlg;
    delete m_pEditUserPerDlg;
}


void UserManager::InsertDataToTableView(void)
{
#if HAVE_DATA_BASE
    if (NULL == g_sqlDB)
    {
        QMessageBox::information(this,"InsertDataToTableView",tr("g_sqlDB为空,TableView初始化失败"));
        return;
    }
    QSqlQuery query(*g_sqlDB);
    QString strSql = QString("%1").arg("select * from UserInfo");//chenyuzhu modify on 2014-7-11 替换新用户管理数据表
    query.exec(strSql);
    if (!query.isActive())
    {
        QMessageBox::information(this,"InsertDataToTableView",tr("SQL语句执行失败"));
        return;
    }
    m_pUserInfoModel->clear();
    m_pUserInfoModel->setColumnCount(4);
    m_pUserInfoModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));
    m_pUserInfoModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("用户名"));
    m_pUserInfoModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("名称"));
    m_pUserInfoModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("权限"));

    m_pUserInformation->setModel(m_pUserInfoModel);
    m_pUserInformation->verticalHeader()->setHidden(true);  //隐藏序号列
    m_pUserInformation->setColumnWidth(0,50);  //设置列宽度

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    m_pUserInformation->horizontalHeader()->setSectionsClickable(false);
#else
    m_pUserInformation->horizontalHeader()->setClickable(false); //设置表头不可点击
#endif
    m_pUserInformation->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    m_pUserInformation->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_pUserInformation->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pUserInformation->setSelectionMode(QAbstractItemView::SingleSelection);

    int nRow = 0;
    while (query.next())
    {
        if (-1 == query.value(13).toInt())
        {
            continue;
        }
        m_pUserInfoModel->setItem(nRow,0,new QStandardItem(QString::number(nRow+1)));
        m_pUserInfoModel->setItem(nRow,1,new QStandardItem(query.value(1).toString()));
        m_pUserInfoModel->setItem(nRow,2,new QStandardItem(query.value(4).toString()));
        m_pUserInfoModel->setItem(nRow,3,new QStandardItem(query.value(12).toString()));
        nRow++;
    }
#else 
	QMessageBox::information(this, "UserManager::InsertDataToTableView", tr("未连接数据库"));
#endif
}

void UserManager::OnEditUserPermission(void)        //修改用户权限
{
    int nRow = m_pUserInformation->currentIndex().row();
    QModelIndex indextemp = m_pUserInfoModel->index(nRow,1);
    QString strUserName = m_pUserInfoModel->data(indextemp).toString();
    if (strUserName.isEmpty())
    {
        QMessageBox::information(this,"UserManager::OnEditUserPermission",tr("请选择要更改的记录"));
        return;
    }

    m_pEditUserPerDlg->m_strUserName = strUserName;
    m_pEditUserPerDlg->InsertDataToDialog();
    m_pEditUserPerDlg->exec();

}

void UserManager::InitDialog(void)
{
    horizontalLayout_2 = new QHBoxLayout(this);
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    groupBox = new QGroupBox(this);
    horizontalLayout = new QHBoxLayout(groupBox);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(20, 20, 20, 20);
    m_pUserInformation = new QTableView(groupBox);
    m_pUserInformation->setMinimumWidth(360);
    horizontalLayout->addWidget(m_pUserInformation);
    m_pUserInfoModel = new QStandardItemModel();

    m_pUserInformation->verticalHeader()->setHidden(true);  //隐藏序号列
    m_pUserInformation->setColumnWidth(0,50);  //设置列宽度
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    m_pUserInformation->horizontalHeader()->setSectionsClickable(false);
#else
    m_pUserInformation->horizontalHeader()->setClickable(false); //设置表头不可点击
#endif
    m_pUserInformation->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    m_pUserInformation->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_pUserInformation->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pUserInformation->setSelectionMode(QAbstractItemView::SingleSelection);

//设置TableView样式
    m_pUserInformation->horizontalHeader()->setStyleSheet("QHeaderView::section{font-family:Helvetica;font-size:20px;color:rgb(255,255,255);background:rgb(73, 73, 73);}");
    m_pUserInformation->setStyleSheet("QTableView{background-color:rgb(73, 73, 73);font-family:Helvetica;font-size:20px;color:rgb(255,255,255)}");

    m_pRightButtonLayOut = new QVBoxLayout();
    m_pRightButtonLayOut->setSpacing(6);
    m_pbEditUserPer = new QPushButton(groupBox);
    m_pbEditUserPer->setMinimumSize(QSize(100, 0));
    m_pRightButtonLayOut->addWidget(m_pbEditUserPer);

    m_pFVerSpacer = new QSpacerItem(20,20,QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_pRightButtonLayOut->addItem(m_pFVerSpacer);

    m_pbDeleteUser = new QPushButton(groupBox);
    m_pbDeleteUser->setMinimumSize(QSize(100, 0));
    m_pRightButtonLayOut->addWidget(m_pbDeleteUser);

    m_pSVerSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_pRightButtonLayOut->addItem(m_pSVerSpacer);

    m_pbAddUser = new QPushButton(groupBox);
    m_pbAddUser->setMinimumSize(QSize(100, 0));
    m_pRightButtonLayOut->addWidget(m_pbAddUser);

    m_pTVerSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_pRightButtonLayOut->addItem(m_pTVerSpacer);

    m_pbEditPassword = new QPushButton(groupBox);
    m_pbEditPassword->setMinimumSize(QSize(100, 0));
    m_pRightButtonLayOut->addWidget(m_pbEditPassword);

    m_pFoVerSpacer = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_pRightButtonLayOut->addItem(m_pFoVerSpacer);

    m_pbExit = new QPushButton(groupBox);
    m_pbExit->setMinimumSize(QSize(100, 0));
    m_pbExit->setHidden(true);
    m_pRightButtonLayOut->addWidget(m_pbExit);
    horizontalLayout->addLayout(m_pRightButtonLayOut);
    horizontalLayout_2->addWidget(groupBox);

    m_pbUserDialog = new AddUser(this);
    m_pEditPwdDlg  = new EditPwdDlg(this);
    m_pEditUserPerDlg = new EditUserPerDlg(this);
}
void UserManager::translate(void)
{
    setWindowTitle(tr("用户账号信息"));
    groupBox->setTitle(tr("用户账号信息"));
    m_pbEditUserPer->setText(tr("权限修改"));
    m_pbDeleteUser->setText(tr("删除用户"));
    m_pbAddUser->setText(tr("增加用户"));
    m_pbEditPassword->setText(tr("修改密码"));
    m_pbExit->setText(tr("退出"));
}
