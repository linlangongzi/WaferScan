#include "DialogProbeList.h"
#include "ui_DialogProbeList.h"
#include <Ultrasonic/UltrasonicManager.h>
#include "ProbeEditor.h"
#include <QLoggingCategory>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMenu>

static Q_LOGGING_CATEGORY(debug, "wave.probe.view");

DialogProbeList::DialogProbeList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProbeList)
{
    ui->setupUi(this);

    m_sortModel = new QSortFilterProxyModel(this);
    m_sortModel->setSourceModel(UltrasonicManager::probeModel());
    m_sortModel->setSortRole(Qt::EditRole);
    m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_sortModel->setFilterKeyColumn(-1);
    ui->treeView->setModel(m_sortModel);

    connect(ui->treeView, &QAbstractItemView::doubleClicked, this, &DialogProbeList::onDoubleClicked);
    ui->treeView->addAction(ui->actionCopy);

    ui->treeView->setColumnHidden(ProbeItem::ColUuid, true);
}

DialogProbeList::~DialogProbeList()
{
    delete ui;
}

Probe DialogProbeList::currentProbe() const
{
    auto *item = currentProbeItem();
    if (!item) {
        return {};
    }
    return item->probe;
}

bool DialogProbeList::isEditable() const
{
    return m_isEditable;
}

void DialogProbeList::setEditable(bool isEditable)
{
    if (m_isEditable == isEditable)
        return;

    m_isEditable = isEditable;

    ui->addProbe->setEnabled(m_isEditable);
    ui->removeProbe->setEnabled(m_isEditable);

    emit editableChanged(m_isEditable);
}

void DialogProbeList::on_addProbe_clicked()
{
    createItem({});
}

void DialogProbeList::on_removeProbe_clicked()
{
    auto *item = currentProbeItem();
    if (!item) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确认删除 %1?").arg(item->probe.name), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
        UltrasonicManager::probeModel()->destroyItem(item);
    }
}

void DialogProbeList::onDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    if (m_isEditable == false) {
        accept();
        return;
    }

    auto *item = currentProbeItem();
    if (!item) {
        return;
    }
    ProbeEditor dialog(this);
    dialog.setProbe(item->probe);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    item->probe = dialog.probe();
    item->update();
}

ProbeItem *DialogProbeList::currentProbeItem() const
{
    QModelIndex index = m_sortModel->mapToSource(ui->treeView->currentIndex());
    return UltrasonicManager::probeModel()->itemForIndexAtLevel<1>(index);
}

void DialogProbeList::createItem(Probe probe)
{
    probe.uuid = QUuid::createUuid();
    ProbeEditor dialog(this);
    dialog.setProbe(probe);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    auto *probeItem = new ProbeItem;
    probeItem->probe = dialog.probe();
    UltrasonicManager::probeModel()->rootItem()->appendChild(probeItem);

    QModelIndex index = m_sortModel->mapFromSource(probeItem->index());
    ui->treeView->setCurrentIndex(index);
}

void DialogProbeList::on_filter_textChanged(const QString &arg1)
{
    m_sortModel->setFilterFixedString(arg1);
}

void DialogProbeList::on_actionCopy_triggered()
{
    auto *item = currentProbeItem();
    if (!item) {
        return;
    }
    createItem(item->probe);
}
