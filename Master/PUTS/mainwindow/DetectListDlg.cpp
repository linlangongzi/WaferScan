/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved.
*
* 伤点信息列表对话框
* Filename: ScanData.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-10-14
*/
#include "DetectListDlg.h"

DetectListDlg::DetectListDlg(QWidget *parent)
    : QDialog(parent)
{
    setStyleSheet("QLabel{color:rgb(255,255,0);font-size:18px}\
                  QPushButton{color:rgb(255,255,0);font-size:18px}}");
    m_pvHeaderRect = NULL;
    setMinimumSize(QSize(1000, 600));
    setMaximumSize(QSize(1000, 600));
    setWindowTitle(tr("伤点信息"));

    m_pQuitButton = new QPushButton(tr("退出"),this);
    m_pQuitButton->setGeometry(QRect(750,540,91,31));
    m_pTableView = new QTableView(this);
    m_pTableView->setGeometry(QRect(50,30,900,500));
    m_pModel = new QStandardItemModel();
    m_pTableView->setModel(m_pModel);
    m_pTableView->verticalHeader()->setHidden(true);  //隐藏序号列

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    m_pTableView->horizontalHeader()->setSectionsClickable(false);
#else
    m_pTableView->horizontalHeader()->setClickable(false); //设置表头不可点击
#endif
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    m_pTableView->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //设置TableView样式
    m_pTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{font-size:18px;color:rgb(255,255,0);background:rgb(73, 73, 73);}");
    m_pTableView->setStyleSheet("QTableView{background-color:rgb(73, 73, 73);font-size:18px;color:rgb(255,255,0)}");
    UpdateTableView();
    connect(m_pQuitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_pTableView,SIGNAL(doubleClicked(const QModelIndex&)),this,SLOT(OnDoubleClicked(const QModelIndex&)));
}

DetectListDlg::~DetectListDlg()
{

}

void DetectListDlg::UpdateTableView() //更新伤点信息
{
    m_pModel->setColumnCount(8);
    m_pModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));
    m_pModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("长(mm)"));
    m_pModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("长差值(mm)"));
    m_pModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("宽(mm)"));
    m_pModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("宽差值(mm)"));
    m_pModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("深(mm)"));
    m_pModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("指示长度(mm)"));
    m_pModel->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("面伤面积(cm^2)"));
    //chenyuzhu modify start on 2014-10-30
    m_pTableView->setColumnWidth(1,140);  //设置列宽度
    m_pTableView->setColumnWidth(2,100);  //设置列宽度
    m_pTableView->setColumnWidth(3,100);  //设置列宽度
    m_pTableView->setColumnWidth(4,100);  //设置列宽度
    m_pTableView->setColumnWidth(6,120);  //设置列宽度
    m_pTableView->setColumnWidth(7,140);  //设置列宽度
    //chenyuzhu modify end on 2014-10-30
    if (NULL == m_pvHeaderRect)
    {
        return;
    }
    int rows = m_pvHeaderRect->count();
    if (rows >1000)
    {
        rows = 1000;
    }else
    {
        if (rows <= 0 )
        {
            return;
        }
    }
    for (int i = 0 ; i<rows; i++)
    {
        m_pModel->setItem(i,0,new QStandardItem(QString::number(i+1)));
        QString temp = QString("%1%2%3").arg((*m_pvHeaderRect)[i].minX).arg("-").arg((*m_pvHeaderRect)[i].maxX);
        m_pModel->setItem(i,1,new QStandardItem(temp));
        m_pModel->setItem(i,2,new QStandardItem(QString::number((*m_pvHeaderRect)[i].maxX - (*m_pvHeaderRect)[i].minX)));
        temp = QString("%1%2%3").arg((*m_pvHeaderRect)[i].minY).arg("-").arg((*m_pvHeaderRect)[i].maxY);
        m_pModel->setItem(i,3,new QStandardItem(temp));
        m_pModel->setItem(i,4,new QStandardItem(QString::number((*m_pvHeaderRect)[i].maxY - (*m_pvHeaderRect)[i].minY)));
        m_pModel->setItem(i,5,new QStandardItem(QString::number((*m_pvHeaderRect)[i].z)));
        m_pModel->setItem(i,6,new QStandardItem(QString::number((*m_pvHeaderRect)[i].zhishiLength)));
        m_pModel->setItem(i,7,new QStandardItem(QString::number((*m_pvHeaderRect)[i].mianShangArea)));
    }
    m_pTableView->setModel(m_pModel);
}


void DetectListDlg::OnDoubleClicked(const QModelIndex& Index)  //响应视图双击事件
{
    int rowid = Index.row();
    emit SignalToCscanDrawDashRect(rowid);
}
