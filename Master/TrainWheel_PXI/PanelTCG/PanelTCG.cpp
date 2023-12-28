#include "PanelTCG.h"
#include "ui_PanelTCG.h"
#include "ConfigObject/UTSetup.h"
#include <QStandardItemModel>

const int PanelTCG::tableViewWidth = 155;
PanelTCG::PanelTCG(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelTCG),
    TCGItemModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
//    InitModel();
    InitUi();
}

PanelTCG::~PanelTCG()
{
    if (TCGItemModel)
    {
        delete TCGItemModel;
        TCGItemModel = NULL;
    }
    if (menu)
    {
        delete menu;
        menu = NULL;
    }
    delete ui;
}

void PanelTCG::ConnectTo(QObject *obj)
{
    Disconnect(setup);
    setup = dynamic_cast<UTSetup *>(obj);
    if (setup)
    {
        connect(setup, SIGNAL(ValueChangedSetup(QString, QJsonValue)), this, SLOT(UpdateModel(QString, QJsonValue)));
//        connect(this, SIGNAL(RequestSetParameter(QString,QJsonValue)), setup, SLOT(SetParameter(QString,QJsonValue)));
        ui->TCGSwitch->ConnectTo(setup);
        InitModel();
    }
}

void PanelTCG::Disconnect(QObject *obj)
{
    if (obj)
    {
        disconnect(obj, 0, this, 0);
        disconnect(this, 0, obj, 0);
        ui->TCGSwitch->Disconnect(obj);
    }
}

void PanelTCG::InitModel()
{
    /* TCG参数的显示 */
    QStringList TCGHeaderList;
    TCGHeaderList << tr("No") << tr("Time") << tr("Gain");
    TCGItemModel->setHorizontalHeaderLabels(TCGHeaderList);
    TCGJsonArray = setup->GetTCGJsonArray();
   for (int itemIndex = 0; itemIndex < TCGJsonArray.count(); ++itemIndex)
    {
        if (TCGJsonArray[itemIndex].toObject()["Deleted"].toBool())
        {
            continue;
        }
        InsertTCGListFromUTSetup(itemIndex);
    }
    ui->TCGTableView->setModel(TCGItemModel);
}

void PanelTCG::InitUi()
{
    ui->TCGTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->TCGTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TCGTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    menu = new QMenu(this);
    QAction* act_add = new QAction(tr("Add TCG Point"), menu);
    QAction* act_del = new QAction(tr("Delete TCG Point"), menu);
    QAction* act_select = new QAction(tr("Modify TCG Point"), menu);
    menu->addAction(act_add);
    menu->addAction(act_del);
    menu->addAction(act_select);
    connect(act_add, SIGNAL(triggered()), this, SLOT(AddTCGPoint()));
    connect(act_del, SIGNAL(triggered()), this, SLOT(DeleteTCGPoint()));
    connect(act_select, SIGNAL(triggered()), this, SLOT(ModifyTCGPoint()));

    ui->TCGTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->TCGTableView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(customContextMenu(const QPoint&)));

    int TCGItemColumnCount = TCGItemModel->columnCount();
    for (int index = 0; index < TCGItemColumnCount; ++index)
    {
        ui->TCGTableView->setColumnWidth(index, tableViewWidth);
    }
    ui->TCGSwitch->SetParameterKey("DACEnable");
    connect(TCGItemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(ModifyModelTCGPoint(QModelIndex)));
}

void PanelTCG::InsertTCGListFromUTSetup(int index)
{
    StdItemList itemList;
    itemList = GetGateStandardItemList(index);
    TCGItemModel->appendRow(itemList);
}

PanelTCG::StdItemList PanelTCG::GetGateStandardItemList(int index)
{
    StdItemList itemList;
    for (int i = 0; i < 3; i++)  // 4为Table表所包含的列数
    {
        QStandardItem *item;
        switch(i)
        {
       case 0:
            item = new QStandardItem(QString("TCG%1").arg(index + 1));
            break;
       case 1:
            item = new QStandardItem(QString::number(TCGJsonArray[index].toObject()["Time"].toDouble(), '.', 2));
            break;
       case 2:
            item = new QStandardItem(QString::number(TCGJsonArray[index].toObject()["Gain"].toDouble(), '.', 2));
            break;
        default:
            break;
        }
        itemList.append(item);
    }
    return itemList;
}

void PanelTCG::on_addBtn_clicked()
{
    AddTCGPoint();
}

void PanelTCG::on_modifyBtn_clicked()
{
    ModifyTCGPoint();
}

void PanelTCG::on_deleteBtn_clicked()
{
    DeleteTCGPoint();
}

void PanelTCG::AddTCGPoint()
{
    PanelTCGEdit TCGEdit(setup, currentIndex, false, this);
    if (TCGEdit.exec() == QDialog::Accepted)
    {
        QJsonValue TCGTimeValue = QJsonValue(TCGEdit.GetTCGTimeValue());
        QJsonValue TCGGainValue = QJsonValue(TCGEdit.GetTCGGainValue());

        if (setup)
        {
            setup->AddTCGItem(TCGTimeValue, TCGGainValue);
        }
    }
}

void PanelTCG::ModifyTCGPoint()
{
    QItemSelectionModel *selections = ui->TCGTableView->selectionModel();
    QModelIndexList indexList = selections->selectedIndexes();
    if (indexList.count() <= 0)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Delete items not selected!"));
        return;
    }
    PanelTCGEdit TCGEdit(setup, currentIndex, true, this);
    if (TCGEdit.exec() == QDialog::Accepted)
    {
        QJsonValue  TCGTimeValue = TCGEdit.GetTCGTimeValue();
        QJsonValue TCGGainValue = TCGEdit.GetTCGGainValue();

        if (setup)
        {
            setup->ModifyTCGJsonArrayItem(TCGTimeValue, TCGGainValue, ui->TCGTableView->currentIndex().row());
        }
    }
}

void PanelTCG::DeleteTCGPoint()
{
    QItemSelectionModel *selections = ui->TCGTableView->selectionModel();
    QModelIndexList indexList = selections->selectedIndexes();
    if (indexList.count() <= 0)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Delete items not selected!"));
        return;
    }
    if (QMessageBox::warning(this, tr("Warning"), tr("Are you sure delete the seleted item ?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
        QModelIndex modelIndex = ui->TCGTableView->currentIndex();
        setup->RemoveTCGItemAt(modelIndex.row());
    }
}

void PanelTCG::customContextMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
    menu->exec(QCursor::pos());
}

void PanelTCG::ModifyModelTCGPoint(const QModelIndex &index)
{
    int row = index.row();
    double TCGTime = TCGItemModel->item(row, 1)->text().toDouble();
    double TCGGain = TCGItemModel->item(row, 2)->text().toDouble();
    if (setup)
    {
        setup->ModifyTCGJsonArrayItem(TCGTime, TCGGain, row);
    }
}

void PanelTCG::UpdateModel(const QString &key, const QJsonValue &value)
{
    Q_UNUSED(value);
    if ("TCG" == key)
    {
        TCGItemModel->clear();
        InitModel();
        ui->TCGTableView->resizeColumnsToContents();
    }
}

void PanelTCG::on_TCGTableView_clicked(const QModelIndex &index)
{
    currentIndex = index.row();
    currentTime = TCGItemModel->item(currentIndex, 1)->text().toDouble();
    currentGain = TCGItemModel->item(currentIndex, 2)->text().toDouble();
}
