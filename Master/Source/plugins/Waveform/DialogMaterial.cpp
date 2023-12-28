#include "DialogMaterial.h"
#include "ui_DialogMaterial.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <Ultrasonic/Material.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/GeneralDelegate.h>

DialogMaterial::DialogMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMaterial)
{
    ui->setupUi(this);
    m_sortModel = new QSortFilterProxyModel(this);
    m_sortModel->setSourceModel(UltrasonicManager::materialModel());
    m_sortModel->setSortRole(Qt::DisplayRole);
    m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_sortModel->setFilterKeyColumn(-1);
    ui->material->setModel(m_sortModel);
    ui->material->setItemDelegate(new GeneralDelegate(ui->material));
    connect(ui->material, &QTreeView::doubleClicked, this, [this] (QModelIndex index) {
        if (!m_editable) {
            ui->material->setCurrentIndex(index);
            accept();
        }
    });
    setEditable(m_editable);
}

DialogMaterial::~DialogMaterial()
{
    delete ui;
}

void DialogMaterial::setCurrentMaterial(const Material &currentMaterial)
{
    auto *rootItem = UltrasonicManager::materialModel()->rootItem();
    auto *materialItem = rootItem->findFirstLevelChild([&] (auto *item) {return item->material == currentMaterial;});
    if (materialItem != nullptr) {
        QModelIndex proxyIndex = m_sortModel->mapFromSource(materialItem->index());
        ui->material->setCurrentIndex(proxyIndex);
    }
}

void DialogMaterial::setEditable(bool editable)
{
    m_editable = editable;
    ui->material->setEditTriggers(m_editable ? (QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed) : QAbstractItemView::NoEditTriggers);
    ui->editContainer->setVisible(m_editable);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(!m_editable);
    emit editableChanged(m_editable);
}

bool DialogMaterial::isEditable() const
{
    return m_editable;
}

Material DialogMaterial::currentMaterial() const
{
    QModelIndex index = m_sortModel->mapToSource(ui->material->currentIndex());
    MaterialItem *item = UltrasonicManager::materialModel()->itemForIndexAtLevel<1>(index);
    if (item != nullptr) {
        return item->material;
    }
    return Material();
}

void DialogMaterial::on_add_clicked()
{
    bool ok;
    QString materialName = QInputDialog::getText(this, QString(), tr("材料名:"), QLineEdit::Normal, QString(), &ok);
    if (!materialName.isEmpty() && ok) {
        auto materialItem = new MaterialItem;
        materialItem->material.id = QUuid::createUuid();
        materialItem->material.name = materialName;
        materialItem->material.legacyId = UltrasonicManager::materialModel()->rootItem()->childCount() + 1;
        UltrasonicManager::materialModel()->rootItem()->appendChild(materialItem);
        QModelIndex index = m_sortModel->mapFromSource(materialItem->index());
        ui->material->setCurrentIndex(index);
    }
}

void DialogMaterial::on_remove_clicked()
{
    QModelIndex index = m_sortModel->mapToSource(ui->material->currentIndex());
    MaterialItem *item = UltrasonicManager::materialModel()->itemForIndexAtLevel<1>(index);
    if (item != nullptr) {
        if (QMessageBox::warning(this, QString(), tr("确认删除 %1?").arg(item->material.name), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
            UltrasonicManager::materialModel()->destroyItem(item);
        }
    }
}

void DialogMaterial::on_filter_textChanged(const QString &arg1)
{
    m_sortModel->setFilterFixedString(arg1);
}
