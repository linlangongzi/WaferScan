#include "PanelMaterial.h"
#include "ui_PanelMaterial.h"
#include "Platform/Platform.h"
#include <QDebug>


//const int PanelMaterial::COLUMN_WIDTH = 210;
//const int PanelMaterial::COLUMN_HEIGH = 20;
const double PanelMaterial::MAX_VELOCITY = 999999.0;
const double PanelMaterial::MIN_VELOCITY = 0.0f;

PanelMaterial::PanelMaterial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelMaterial),
    validator(MIN_VELOCITY, MAX_VELOCITY, 10, this)
{
    ui->setupUi(this);
    InitModel();
    InitUI();
    SetSelectMode(false);
}

PanelMaterial::~PanelMaterial()
{
    if (materialItemModel)
    {
        delete materialItemModel;
        materialItemModel = NULL;
    }
    if (menu)
    {
        delete menu;
        menu = NULL;
    }
    delete ui;
}

Material *PanelMaterial::GetSelection()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    if (selection == NULL || !selection->hasSelection())
    {
        return NULL;
    }
    QModelIndex index = selection->currentIndex();
    QStandardItem *item = materialItemModel->item(index.row(), MATERIAL_ID);
    Config::ConfigID id = item ? Config::ToObjectID(item->text()) : 0;
    return platform()->Materials()->GetByID<Material>(id);
}

void PanelMaterial::SetSelectMode(bool selectMode)
{
    ui->tableView->setEditTriggers(selectMode ? QTableView::NoEditTriggers : QTableView::DoubleClicked);
    ui->buttonGroup->setVisible(!selectMode);
}

/**
 * 自定义菜单
 * @brief PanelMaterial::customContextMenu
 * @param pos 选择的菜单位置
 */
void PanelMaterial::customContextMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
    menu->exec(QCursor::pos());
}

/**
 * 响应表中的内容变化
 * @brief PanelMaterial::ModifyMaterial
 * @param ModelIndex 当前选中的Model索引
 */
void PanelMaterial::ModifyMaterial(const QModelIndex& index)
{
    ConfigObject::ConfigID id = Config::ToObjectID(materialItemModel->item(index.row())->text());
    Material *material = platform()->Materials()->GetByID<Material>(id);
    if (material == NULL)
    {
        return;
    }
    QStandardItem *item = materialItemModel->item(index.row(), index.column());
    QString text = item ? item->text() : QString();
    switch (index.column())
    {
    case MATERIAL_NAME:
        material->Set("MaterialName", text);
        break;
    default:
    case MATERIAL_VELOCITY:
        int pos = 0;
        if (QValidator::Invalid == validator.validate(text, pos))
        {
            QMessageBox::warning(this, tr("Warning"), tr("Warning::input is invalid!"));
            materialItemModel->item(index.row(), index.column())->setText(QString::number(material->Get("MaterialVelocity").toDouble()));
            return;
        }
        material->Set("MaterialVelocity",QJsonValue(text.toInt()));
        break;
    }
}

void PanelMaterial::SelectMaterial()
{
    emit SelectSubmit();
}

/**
 * 初始与ui相关的控件
 * 连接对应的信号与槽
 * @brief PanelMaterial::InitUI
 */
void PanelMaterial::InitUI()
{
    setWindowTitle("MaterialTable");
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    menu = new QMenu(this);
    QAction *actAdd = new QAction(tr("Add Material"), menu);
    QAction *actDel = new QAction(tr("Delete Material"), menu);
    QAction *actSelect = new QAction(tr("Select Material"), menu);
    menu->addAction(actAdd);
    menu->addAction(actDel);
    menu->addAction(actSelect);
    connect(actAdd, SIGNAL(triggered()), this, SLOT(on_addMaterial_clicked()));
    connect(actDel, SIGNAL(triggered()), this, SLOT(on_deleteMaterial_clicked()));
    connect(actSelect, SIGNAL(triggered()), this, SLOT(SelectMaterial()));

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(customContextMenu(const QPoint&)));
    connect(materialItemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(ModifyMaterial(QModelIndex)));
    connect(materialItemModel, SIGNAL(rowsInserted(QModelIndex,int,int)), ui->tableView, SLOT(resizeColumnsToContents()));
//    connect(ui->NewBtn, SIGNAL(clicked()), this, SLOT(NewMaterial()));
//    connect(ui->DeleteBtn, SIGNAL(clicked()), this, SLOT(DeleteMaterial()));
}

/**
 * 创建表头
 * 设置好键值
 * @brief PanelMaterial::InitModel
 */
void PanelMaterial::InitModel()
{
    materialItemModel = new QStandardItemModel(this);
    QStringList headerList;
    headerList << tr("ID") << tr("MaterialName") << tr("MaterialVelocity(m/s)");
    materialItemModel->setHorizontalHeaderLabels(headerList);

//    foreach (Material *material, platform()->Materials()->GetRegistry<Material>().values())
//    {
//        if (material->Get("Deleted").toBool())
//        {
//            continue;
//        }
//        InsertMaterial(material);
//    }
    ui->tableView->setModel(materialItemModel);
    ui->tableView->setColumnHidden(MATERIAL_ID, true);
    ui->tableView->resizeColumnsToContents();
}

/**
 * 向表中插入数据
 * @brief PanelMaterial::InsertMaterial
 * @param material Material Json表指针
 */
void PanelMaterial::InsertMaterial(Material *material)
{
    StandardItemRow itemList;
    itemList << new QStandardItem(QString::number(material->GetID()));
    itemList << new QStandardItem(material->Get("MaterialName").toString());
    itemList << new QStandardItem(QString::number(material->Get("MaterialVelocity").toDouble()));
    materialItemModel->appendRow(itemList);
}

void PanelMaterial::on_tableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    emit SelectSubmit();
}

void PanelMaterial::on_addMaterial_clicked()
{
    PanelMaterialEdit materialDialog(this);
    if (materialDialog.exec() == QDialog::Accepted)
    {
        QString materialName = materialDialog.GetMaterialName();
        if (materialName.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Invalid material name"));
            return;
        }
        Material* material = platform()->Materials()->NewItem<Material>();
        if (material)
        {
            material->Set("MaterialName", materialName);
            material->Set("MaterialVelocity", QJsonValue(materialDialog.GetMaterialVelocity()));
            InsertMaterial(material);
        }
    }
}

void PanelMaterial::on_deleteMaterial_clicked()
{
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    if (selections == NULL)
    {
        return;
    }
    QModelIndexList indexList = selections->selectedIndexes();
    if (indexList.count() <= 0)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Delete items not selected!"));
        return;
    }
    if (QMessageBox::warning(this, tr("Warning"), tr("Are you sure want to delete the selected item ?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
        int NumDelRow = 0;
        foreach (QModelIndex index, indexList)
        {
            if (index.column() == 0)
            {
                ConfigObject::ConfigID id = Config::ToObjectID(materialItemModel->item(index.row())->text());
                Material *material = platform()->Materials()->GetByID<Material>(id);

                ConfigObject::DelObject(material);
                materialItemModel->removeRow(index.row() - NumDelRow);                      // 从model中删除
//                NumDelRow++;
                qDebug() << "PanelMaterial::DeleteMaterial: Deleting material" << material->ToJson();
            }
        }
        materialItemModel->submit();
    }
}
