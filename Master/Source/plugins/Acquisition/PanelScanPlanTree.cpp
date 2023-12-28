#include "PanelScanPlanTree.h"
#include "ui_PanelScanPlanTree.h"

#include "Platform/Platform.h"

PanelScanPlanTree::PanelScanPlanTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PanelScanPlanTree),
    NUM_COLUMN(4)
{
    ui->setupUi(this);
    InitUI();
//    SetModel();
}

PanelScanPlanTree::~PanelScanPlanTree()
{
    delete ui;
}

void PanelScanPlanTree::InitUI()
{
    // 设置tree是行选择与不可编辑
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置委托进度条
    ProgressBarDelegate *barDeleagate = new ProgressBarDelegate(ui->treeView);
    ui->treeView->setItemDelegateForColumn(1, barDeleagate);
    // 为了保持一行的所有列选中时高亮一致，给2、3列也设置一个委托，但是执行的是默认委托
    ui->treeView->setItemDelegateForColumn(2, barDeleagate);
    ui->treeView->setItemDelegateForColumn(3, barDeleagate);
    // 给第0 行设置编辑框委托
    EditDelegate *editor = new EditDelegate(ui->treeView);
    ui->treeView->setItemDelegateForColumn(0, editor);
    // 绑定信号和槽
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( ui->treeView, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( slotCustomContextMenu(const QPoint &) ) );
    connect( ui->treeView, SIGNAL( doubleClicked(const QModelIndex &) ), this, SLOT( OnDoubleClicked(const QModelIndex &) ) );
}

void PanelScanPlanTree::slotCustomContextMenu(const QPoint &)
{
    QMenu menu;
    QAction *act_addFolder = new QAction(tr("Add Folder"), &menu);
    QAction *act_addLine = new QAction(tr("Add Line"), &menu);
    QAction *act_addRectangle = new QAction(tr("Add Rectangle"), &menu);
    QAction *act_addCylinder = new QAction(tr("Add Cylinder"), &menu);          //
    QAction *act_addDisk = new QAction(tr("Add Disk"), &menu);
    QAction *act_addLoate = new QAction(tr("Add Locate"), &menu);

    QAction *act_rename = new QAction(tr("Rename"), &menu);
    QAction *act_Edit = new QAction(tr("Edit ScanPlan"), &menu);
    QAction *act_Del = new QAction(tr("Del ScanPlan"), &menu);

    connect( act_addFolder, SIGNAL( triggered() ), this, SLOT( AddFolder() ) );
    connect( act_addLine, SIGNAL( triggered() ), this, SLOT( AddLine() ) );
    connect( act_addRectangle, SIGNAL( triggered() ), this, SLOT( AddRectangle() ) );
    connect( act_addCylinder, SIGNAL( triggered() ), this, SLOT( AddCylinder() ) );
    connect( act_addDisk, SIGNAL( triggered() ), this, SLOT( AddDisk() ) );
    connect( act_addLoate, SIGNAL( triggered() ), this, SLOT( AddLocate() ) );

    connect( act_rename, SIGNAL( triggered() ), this, SLOT( RenameScanPlan() ) );
    connect( act_Edit, SIGNAL( triggered() ), this, SLOT( EditScanPlan() ) );
    connect( act_Del, SIGNAL( triggered() ), this, SLOT( DelScanPlan() ) );

    QModelIndex index = ui->treeView->currentIndex();
    if ( !index.isValid() )
    {
        return;
    }
    ScanPlan *scanPlan = platform()->GetScanPlanModel()->GetScanPlanFromIndex(index);
    QString mode = scanPlan->Get("MotionType").toString();
    if ( ( scanPlan->GetDepth() < platform()->GetScanPlanModel()->GetMaxDepth() ) && (mode == "NONE") ) // 小于最大深度且为NONE
    {
        if (scanPlan->GetDepth() < platform()->GetScanPlanModel()->GetMaxDepth() - 1)
        {
            menu.addAction(act_addFolder);
        }
        menu.addAction(act_addLine);
        menu.addAction(act_addRectangle);
        menu.addAction(act_addCylinder);
        menu.addAction(act_addDisk);
        menu.addAction(act_addLoate);
        menu.addSeparator();
    }
    if ( index.parent().isValid() )
    {
        menu.addSeparator();
        if ( (mode == "LINE") || (mode == "RECTANGLE") || (mode == "CYLINDER") || (mode == "DISK") || (mode == "LOCATE") )
        {
            menu.addAction(act_Edit);
        }
        menu.addSeparator();
        menu.addAction(act_rename);
        menu.addSeparator();
        menu.addAction(act_Del);

    }
//    /***************test action start**********************/
//    QAction *act_next = new QAction(tr("Next"), &menu);
//    menu.addSeparator();
//    menu.addAction(act_next);
//    connect(act_next, SIGNAL(triggered()), this, SLOT(OnNext()));
//    /***************test action end***********************/
    menu.exec( QCursor::pos() );
}

void PanelScanPlanTree::OnDoubleClicked(const QModelIndex &index)
{
    ScanPlan *scanPlan = platform()->GetScanPlanModel()->GetScanPlanFromIndex(index);
    if ( (scanPlan != NULL) && index.isValid() )
    {
        QString motionType = scanPlan->Get("MotionType").toString();
        if ( (motionType == "LINE") || (motionType == "RECTANGLE") || (motionType == "CYLINDER") || (motionType == "DISK") || (motionType == "LOCATE") )
        {
            EditScanPlan();
        }
    }
}

void PanelScanPlanTree::AddFolder()
{
    QModelIndex index = ui->treeView->currentIndex();
    if ( index.isValid() )
    {
        ScanPlan *folder = platform()->GetScanPlanModel()->InsertNewScanPlan(QJsonObject(), "NONE", index);
        if (folder != NULL)
        {
            QModelIndex folderIndex = platform()->GetScanPlanModel()->GetIndexByScanPlan(folder);
            if ( folderIndex.isValid() )
            {
                ui->treeView->setCurrentIndex(folderIndex);
                RenameScanPlan();
            }
        }
    }
}

void PanelScanPlanTree::AddLine()
{
    AddScanPlan("LINE");
}

void PanelScanPlanTree::AddRectangle()
{
    AddScanPlan("RECTANGLE");
}

void PanelScanPlanTree::AddCylinder()
{
    AddScanPlan("CYLINDER");
}

void PanelScanPlanTree::AddDisk()
{
    AddScanPlan("DISK");
}

void PanelScanPlanTree::AddLocate()
{
    AddScanPlan("LOCATE");
}

void PanelScanPlanTree::RenameScanPlan()
{
    QModelIndex index = ui->treeView->currentIndex();
    ScanPlan *curPlan = platform()->GetScanPlanModel()->GetScanPlanFromIndex(index);
    if ( (curPlan != NULL) && index.isValid() )
    {
        // 使用代理编辑框 ，调用model 的setdata
        ui->treeView->edit(index);
    }
}

void PanelScanPlanTree::EditScanPlan()
{
    QModelIndex index = ui->treeView->currentIndex();
    ScanPlan *curPlan = platform()->GetScanPlanModel()->GetScanPlanFromIndex(index);
    if ( (curPlan != NULL) && index.isValid() )
    {
        DialogScanPlan dlg(curPlan, this);
        if (dlg.exec() == QDialog::Accepted)
        {
            platform()->GetScanPlanModel()->ReloadJson( curPlan, dlg.GetJsonObject() );
        }
    }
}

void PanelScanPlanTree::DelScanPlan()
{
    QModelIndex index = ui->treeView->currentIndex();
    ScanPlan *curPlan = platform()->GetScanPlanModel()->GetScanPlanFromIndex(index);
    qDebug() << "delScanPlan curPlan" << curPlan << curPlan->Get("Name") << "index" << index;
    if ( (curPlan != NULL) && index.isValid() )
    {
        if (QMessageBox::warning(NULL, tr("Warning"),
        tr("Are you sure to delete this ScanPlan ?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        {
            QModelIndex parentIndex = index.parent();
            ScanPlan *parent = platform()->GetScanPlanModel()->GetScanPlanFromIndex(parentIndex);
            qDebug() << "delScanPlan parent" << parent << parent->Get("Name") << "index" << parentIndex;
            if ( (parent != NULL) && parentIndex.isValid() )
            {
                int row = parent->GetChildren().indexOf(curPlan);
                platform()->GetScanPlanModel()->removeRows(row, 1, parentIndex);
                // 删除之后，需要正确移动选中光标
//                ui->treeView->SetCurrentItem(row, parentIndex);
            }
        }
    }
    else
    {
        QMessageBox::warning(NULL, tr("Warning"), "The base node can not be deleted");
    }
}

/***************test action start**********************/
//void PanelScanPlanTree::OnNext()
//{
//    QModelIndex index = ui->treeView->currentIndex();
//    ScanPlan* curPlan = platform()->GetScanPlanModel()->GetScanPlanFromIndex(index);
//    if (curPlan != NULL && index.isValid())
//    {
//        ScanPlan* nextPlan = curPlan->GetNext(platform()->GetScanPlanModel()->GetRoot()->GetChild(0));
//        if (nextPlan != NULL)
//        {
//            QModelIndex nextIndex = platform()->GetScanPlanModel()->GetIndexByScanPlan(nextPlan);
//            ui->treeView->setCurrentIndex(nextIndex);
//        }
//        else
//        {
//            QMessageBox::warning(this, tr("Warning"), tr("this is the end"));
//        }
//    }
//}
/***************test action end**********************/

void PanelScanPlanTree::AddScanPlan(const QString &motiontype)
{
    platform()->GetScanPlanModel()
    DialogScanPlan dlg(motiontype, info, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        QModelIndex index = ui->treeView->currentIndex();
        if ( index.isValid() )
        {
            ScanPlan *newItem = platform()->GetScanPlanModel()->InsertNewScanPlan(dlg.GetJsonObject(), motiontype, index);
            ui->treeView->expand(index);
            if (newItem != NULL)
            {
                QModelIndex newIndex = platform()->GetScanPlanModel()->GetIndexByScanPlan(newItem);
                if ( newIndex.isValid() )
                {
                    ui->treeView->setCurrentIndex(newIndex);
                }
            }
        }
    }
}

void PanelScanPlanTree::SetModel()
{
//    ui->treeView->SetModel();
}

ScanPlan *PanelScanPlanTree::GetCurrentScanPlan()
{
    QModelIndex index = ui->treeView->currentIndex();
    if ( index.isValid() )
    {
        return static_cast<ScanPlan *>( index.internalPointer() );
    }
    return NULL;
}
