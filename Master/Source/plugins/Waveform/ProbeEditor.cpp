#include "ProbeEditor.h"
#include "ui_ProbeEditor.h"
#include "DgsTableEditor.h"
#include <QUuid>
#include <QLoggingCategory>
#include <Ultrasonic/UltrasonicManager.h>
#include <QStringList>
#include <QDate>
#include <allrising/Util.h>
#include <QMessageBox>

static Q_LOGGING_CATEGORY(debug, "wave.probe.editor");

ProbeEditor::ProbeEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProbeEditor),
    m_handler(this, &ProbeEditor::probe, &ProbeEditor::setProbe),
    m_handlerSize(this, &ProbeEditor::probeSize, &ProbeEditor::setProbeSize)
{
    ui->setupUi(this);

    QSet<QString> names;
    UltrasonicManager::probeModel()->rootItem()->forAllChildren([&](ProbeItem *child) {
        names << child->probe.manufactureName;
    });
    ui->manufactureName->addItems(names.toList());

    m_model = new DgsTableModel(this);
    ui->dgsTreeView->setModel(m_model);

    m_handler.add(ui->name, &Probe::name);
    m_handler.add(ui->manufactureName, &Probe::manufactureName);
    m_handler.addHandler(ui->calibrationDate, &QDateEdit::setDate, &QDateEdit::dateChanged, &Probe::calibrationDate);
    m_handler.add(ui->modelName, &Probe::modelName);
    m_handler.add(ui->serialName, &Probe::serialName);
    m_handler.add(ui->probeType, &Probe::probeType);
    m_handler.add(ui->shape, &Probe::shape);
    m_handler.add(ui->focalType, &Probe::focalType);
    m_handler.add(ui->frequency, &Probe::frequency);
    m_handler.add(ui->crystalSizeInch, &Probe::crystalSize);
    m_handler.add(ui->focalInch, &Probe::focalInch);
    m_handler.add(ui->focalCylinderLength, &Probe::focalCylinderLength);
    m_handler.add(ui->emissionDelay, &Probe::emissionDelay);
    m_handler.add(ui->sensitivity, &Probe::sensitivity);
    m_handler.add(ui->azimuth, &Probe::azimuth);
    m_handler.add(ui->elevation, &Probe::elevation);
    m_handler.addHandler(this, &ProbeEditor::setProbeSize, &ProbeEditor::probeSizeChanged, &Probe::size);
    m_handler.addHandler(m_model, &DgsTableModel::setDgsTables, &DgsTableModel::dgsTablesChanged, &Probe::dgsTables);
    m_handler.addHandler(m_model, &DgsTableModel::setDefaultTableUuid, &DgsTableModel::defaultTableUuidChanged, &Probe::defaultDgsTableUuid);

    m_handlerSize.add(ui->length, &ProbeSize::length);
    m_handlerSize.add(ui->width, &ProbeSize::width);
    m_handlerSize.add(ui->height, &ProbeSize::height);
    m_handlerSize.add(ui->radius, &ProbeSize::radius);
}

ProbeEditor::~ProbeEditor()
{
    delete ui;
}

Probe ProbeEditor::probe() const
{
    return m_probe;
}

ProbeSize ProbeEditor::probeSize() const
{
    return m_probeSize;
}

void ProbeEditor::accept()
{
    if (ui->name->text().isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("名称不能为空"));
        return;
    }
    QDialog::accept();
}

void ProbeEditor::setProbe(const Probe &probe)
{
    if (m_probe == probe) {
        return;
    }
    m_probe = probe;

    if (m_probe.defaultDgsTable().uuid.isNull() && !m_probe.dgsTables.isEmpty()) {
        m_model->setDefaultTableUuid(m_probe.dgsTables.first().uuid);
    }

    ui->uuid->setText(m_probe.uuid.toString());

    QSignalBlocker blocker(ui->focalSi);
    ui->focalSi->setValue(InchToMillimetre(m_probe.focalInch));
    blocker.unblock();

    QSignalBlocker block(ui->crystalSizeSi);
    ui->crystalSizeSi->setValue(InchToMillimetre(m_probe.crystalSize));
    block.unblock();

    bool hasFocus = m_probe.focalType != Probe::FocusNone;
    ui->focalInch->setEnabled(hasFocus);
    ui->focalSi->setEnabled(hasFocus);

    m_handler.setControls(m_probe);

    emit probeChanged(m_probe);
}

void ProbeEditor::setProbeSize(const ProbeSize &size)
{
    if (m_probeSize == size) {
        return;
    }
    m_probeSize = size;
    m_handlerSize.setControls(m_probeSize);

    emit probeSizeChanged(m_probeSize);
}

void ProbeEditor::on_focalSi_valueChanged(double arg1)
{
    qreal focalInch = MillimetreToInch(arg1);
    m_handler.setField(&Probe::focalInch, focalInch);
}

void ProbeEditor::on_crystalSizeSi_valueChanged(double arg1)
{
    qreal crystalSizeInch = MillimetreToInch(arg1);
    m_handler.setField(&Probe::crystalSize, crystalSizeInch);
}

void ProbeEditor::on_recoverDate_clicked()
{
    ui->calibrationDate->setDate(QDate::currentDate());
}

void ProbeEditor::on_deleteDate_clicked()
{
    ui->calibrationDate->setDate(QDate(2000, 1, 1));
}

void ProbeEditor::on_addDgs_clicked()
{
    DgsTable table;
    table.uuid = QUuid::createUuid();
    DgsTableEditor dialog(this);
    dialog.setDgsTable(table);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    auto probe = m_probe;
    probe.dgsTables << dialog.dgsTable();
    setProbe(probe);
}

void ProbeEditor::on_removeDgs_clicked()
{
    if (!currentItem()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除当前选中的Dgs曲线吗"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_model->destroyItem(currentItem());
    }
    emit m_model->dgsTablesChanged(m_model->dgsTables());
}

void ProbeEditor::on_dgsTreeView_doubleClicked(const QModelIndex &index)
{
    DgsTableEditor dialog(this);
    dialog.setDgsTable(currentTable());
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    int row = index.row();
    auto probe = m_probe;
    if (uint(row) < uint(probe.dgsTables.size())) {
        probe.dgsTables[row] = dialog.dgsTable();
        setProbe(probe);
    }
}

DgsTable ProbeEditor::currentTable() const
{
    if (currentItem()) {
        return currentItem()->table;
    }
    return {};
}

DgsTableItem *ProbeEditor::currentItem() const
{
    auto *item = m_model->itemForIndexAtLevel<1>(ui->dgsTreeView->currentIndex());
    return item;
}

void ProbeEditor::on_setDefaultTable_clicked()
{
    if (!currentItem()) {
        return;
    }
    m_model->setDefaultTableUuid(currentItem()->table.uuid);
}
