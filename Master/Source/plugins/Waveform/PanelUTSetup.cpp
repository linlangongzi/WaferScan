#include "PanelUTSetup.h"
#include "ui_PanelUTSetup.h"
#include <Core/ICore.h>
#include <Core/LicenseManager.h>
#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <Ultrasonic/InstrumentSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/GeneralDelegate.h>
#include <allrising/MetaType/GadgetTreeItem.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>
#include <QLoggingCategory>

using namespace Core;
using namespace Utils;

static Q_LOGGING_CATEGORY(debug, "wave.utsetup");

class GateItem : public GadgetTreeItem
{
public:
    GateItem() : GadgetTreeItem(qMetaTypeId<UTSetupGate>()) {}

    QVariant toTimebase(qreal time, int role) const
    {
        const UTSetupTimebase timebase = panel()->setup().timebase();
        if (role == Qt::EditRole) {
            return timebase.toTimebaseUnit(time);
        } else if (role == Qt::DisplayRole) {
            return timebase.timebaseText(time);
        }
        return QVariant();
    }

    PanelUTSetup *panel() const
    {
        return qobject_cast<PanelUTSetup *>(model()->parent());
    }

    QVariant data(int column, int role) const override
    {
        if (column == 0 && role == Qt::DecorationRole) {
            return value().value<UTSetupGate>().color();
        }
        return GadgetTreeItem::data(column, role);
    }

    QVariant value() const override
    {
        const UTSetupGate gate = panel()->setup().gate(index().row());
        return QVariant::fromValue(gate);
    }

    bool setValue(const QVariant &gadget) override
    {
        UTSetup setup = panel()->setup();
        UTSetupGate gate = gadget.value<UTSetupGate>();
        setup.setGate(index().row(), gate);
        panel()->setSetup(setup);
        return true;
    }

    QVariant status(int role) const override
    {
        if (role == Qt::DisplayRole) {
            return QString("%1% (%2)").arg(amp, 0, 'f', 2).arg(toTimebase(time, role).toString());
        }
        return QVariant();
    }

    QVariant toDisplayValue(int role, const QString &key, const QVariant &value) const override
    {
        if (key == "start" || key == "width") {
            QVariant v = panel()->setup().toTimebaseUnit(value.toReal());
            return GadgetTreeItem::toDisplayValue(role, key, v);
        }
        return GadgetTreeItem::toDisplayValue(role, key, value);
    }

    QVariant fromDisplayValue(int role, const QString &key, const QVariant &value) const override
    {
        if (key == "start" || key == "width") {
            QVariant v = GadgetTreeItem::fromDisplayValue(role, key, value);
            return panel()->setup().fromTimebaseUnit(v.toReal());
        }
        return GadgetTreeItem::fromDisplayValue(role, key, value);
    }

    QVariantHash editorProperty(const QString &key) const override
    {
        if (key == "start" || key == "width") {
            QVariantHash hash = GadgetTreeItem::editorProperty(key);
            hash["suffix"] = panel()->setup().timebaseSuffix();
            return hash;
        }
        return GadgetTreeItem::editorProperty(key);
    }

    qreal time = 0;
    qreal amp = 0;
};

class DacItem : public TypedTreeItem<DacItem>
{
public:
    enum Columns {
        ColTime,
        ColGain,
        ColTcgGain,
    };

    QVariant data(int column, int role) const override
    {
        const InterpolateTable dac = panel()->setup().dac();
        auto it = dac.begin() + index().row();

        if (column == ColTime) {
            return toTimebase(it.key(), role);
        } else if (column == ColGain) {
            if (role == Qt::DisplayRole) {
                return QString("%1 dB").arg(it.value());
            } else if (role == Qt::EditRole) {
                return it.value();
            }
        } else if (column == ColTcgGain) {
            qreal tcgGain = -(it.value() + panel()->setup().gain());
            if (role == Qt::DisplayRole) {
                return QString("%1 dB").arg(tcgGain);
            } else if (role == Qt::EditRole) {
                return tcgGain;
            }
        }
        return QVariant();
    }
    bool setData(int column, const QVariant &data, int role) override
    {
        UTSetup setup = panel()->setup();
        InterpolateTable dac = setup.dac();
        auto it = dac.begin() + index().row();
        if (column == ColTime && role == Qt::EditRole) {
            qreal gain = it.value();
            dac.erase(it);
            dac.insert(data.toReal(), gain);
        } else if (column == ColGain && role == Qt::EditRole) {
            it.value() = data.toReal();
        } else if (column == ColTcgGain && role == Qt::EditRole) {
            it.value() = -data.toReal() - setup.gain();
        } else {
            return false;
        }
        setup.setDac(dac);
        panel()->setSetup(setup);
        return true;
    }

    Qt::ItemFlags flags(int column) const override
    {
        Q_UNUSED(column);
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }

    QVariant toTimebase(qreal time, int role) const
    {
        const UTSetupTimebase timebase = panel()->setup().timebase();
        if (role == Qt::EditRole) {
            return timebase.toTimebaseUnit(time);
        } else if (role == Qt::DisplayRole) {
            return QString("%1 %2").arg(timebase.toTimebaseUnit(time)).arg(timebase.timebaseSuffix());
        }
        return QVariant();
    }

    PanelUTSetup *panel() const
    {
        return qobject_cast<PanelUTSetup *>(model()->parent());
    }
};

PanelUTSetup::PanelUTSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelUTSetup),
    gateModel(new TreeModel<GateRootItem, GateItem>(this)),
    dacModel(new TreeModel<DacItem, DacItem>(this)),
    handler(this, &PanelUTSetup::setup, &PanelUTSetup::setSetup),
    popup(new QToolButton(this))
{
    ui->setupUi(this);
    popup->setIcon(QIcon(":/fatcow/16x16/external.png"));
    connect(popup, &QToolButton::clicked, this, &PanelUTSetup::requestPopup);

    ui->tab->setCornerWidget(popup, Qt::TopRightCorner);
    initUi();

    ui->gates->setItemDelegate(new GeneralDelegate(this));
    gateModel->setHeader({tr("名称"), tr("值")});
    ui->gates->setModel(gateModel);

    dacModel->setHeader({tr("时间"), tr("增益"), tr("TCG增益")});
    ui->dac->setModel(dacModel);
    ui->dacTimebase->setSpecialValueText(tr("禁用"));
    refreshControls();

    LicenseManager::registerLicenseUpdate(this, &PanelUTSetup::onLicenseUpdated);
}

PanelUTSetup::~PanelUTSetup()
{
    delete ui;
}

/*! 隐藏基本信息界面上的单晶关联选框 */
void PanelUTSetup::hideAlterSourceCheckbox(bool enable)
{
    ui->alterSource->setHidden(enable);
    ui->alterSourceTitle->setHidden(enable);
}

UTSetup PanelUTSetup::setup() const
{
    return m_setup;
}

void PanelUTSetup::setChannel(UTChannel *newChannel)
{
    if (channel) {
        disconnect(channel, nullptr, this, nullptr);
        disconnect(this, nullptr, channel, nullptr);
    }
    channel = newChannel;
    if (newChannel) {
        connect(newChannel, &UTChannel::setupChanged, this, &PanelUTSetup::setSetup);
        connect(this, &PanelUTSetup::setupChanged, newChannel, &UTChannel::setSetup);
        m_setup = channel->setup();
        refreshControls();
    }
}

void PanelUTSetup::setPrfVisible(bool isVisible)
{
    ui->prfSlider->setVisible(isVisible);
    ui->prf->setVisible(isVisible);
    ui->transmitVoltage->setVisible(isVisible);
}

/*! 根据数据帧，显示闸门特征值 */
void PanelUTSetup::setDataFrame(const UTDataFrame &frame)
{
    ui->tabData->setDataFrame(frame);
    auto *root = gateModel->rootItem();
    for (int index = 0; index < root->childCount(); ++index) {
        auto *item = root->childAt(index);
        if (item) {
            item->amp = frame.gateAmp(index);
            item->time = frame.gateTime(index);
            item->update();
        }
    }
}

void PanelUTSetup::setInstrumentInfo(const InstrumentInfo &info)
{
    ui->prfSlider->setMaximum(info.maxPrf);
    ui->prf->setMaximum(info.maxPrf);
    ui->gain->setMaximum(info.maxGain);
    ui->gainSlider->setMaximum(info.maxGain);
    ui->gainRef->setMaximum(info.maxGain);
    ui->gainRefSlider->setMaximum(info.maxGain);
    ui->alterSource->setVisible(info.supportAlterSource);
    ui->alterSourceTitle->setVisible(info.supportAlterSource);
    ui->prfGroup->setVisible(info.supportAdjustTransmitMode);
    ui->groupImpedance->setVisible(info.supportImpedance);
    ui->widgetTimebase->setSurfaceTracingVisible(info.supportSurfaceTracking);
    ui->pulseWidthGroup->setVisible(info.supportPulseWidth);
    ui->pulseWidthGroup->setVisible(info.supportPulseType);
    ui->groupTransmitEnergy->setVisible(info.supportEnergy);
    ui->sampleAverageGroup->setVisible(info.supportFilter);
    if (!info.supportPulseType) {
        ui->pulseWidth->setEnabled(true);
    }

    int voltage = this->voltage();
    ui->transmitVoltage->blockSignals(true);
    ui->transmitVoltage->clear();
    for (int value: info.voltage) {
        ui->transmitVoltage->addItem(tr("%1 V").arg(value), value);
    }
    ui->transmitVoltage->blockSignals(false);
    setTransmitVoltage(voltage);
}

void PanelUTSetup::setSetup(const UTSetup &setup)
{
    if (m_setup == setup) {
        return;
    }

    m_setup = setup;
//    ui->tabData->setSetup(setup);
    refreshControls();
    emit setupChanged(setup);
}

void PanelUTSetup::setReadonly(bool readonly)
{
    m_readonly = readonly;
    onLocked(readonly);
    emit readonlyChanged(m_readonly);
}

void PanelUTSetup::refreshControls()
{
    const auto gates = m_setup.gates();
    auto *gateRoot = gateModel->rootItem();
    while (gateRoot->childCount() > gates.count()) {
        delete gateModel->takeItem(gateRoot->lastChild());
    }
    while (gateRoot->childCount() < gates.count()) {
        gateRoot->appendChild(new GateItem);
    }
    gateRoot->forFirstLevelChildren([](GateItem *item) {
        item->update();
    });

    const auto dac = m_setup.dac();
    auto *dacRoot = dacModel->rootItem();
    while (dacRoot->childCount() > dac.count()) {
        delete dacModel->takeItem(dacRoot->lastChild());
    }
    while (dacRoot->childCount() < dac.count()) {
        dacRoot->appendChild(new DacItem);
    }
    dacRoot->forFirstLevelChildren([](DacItem *item) {
        item->update();
    });

    handler.setControls(m_setup);

    ui->gainContainer->setCurrentWidget(m_setup.manualGain() ? ui->pageManualGain : ui->pageAutoGain);
}

void PanelUTSetup::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->manualGain->setVisible(develop);
//    ui->probeSummery->setVisible(develop);
//    ui->probeClear->setVisible(develop);
//    ui->probeEdit->setVisible(develop);
//    ui->probeTitle->setVisible(develop);
    ui->probeMultiplex->setVisible(develop);
    ui->probeMultiplexTitle->setVisible(develop);
}

void PanelUTSetup::onLocked(bool locked)
{
    ui->lock->setText(locked ? tr("解锁") : tr("锁定"));
    ui->lock->setChecked(locked);
    ui->importSetup->setDisabled(locked);
    ui->pulserEnable->setDisabled(locked);
    ui->name->setDisabled(locked);
    ui->alterSource->setDisabled(locked);
    ui->alterSourceTitle->setDisabled(locked);

    ui->prfGroup->setDisabled(locked);
    ui->groupTransmitEnergy->setDisabled(locked);
    ui->groupImpedance->setDisabled(locked);
    ui->groupTransmitVoltage->setDisabled(locked);
    ui->gainGroupBox->setDisabled(locked);
    ui->gainRefGroup->setDisabled(locked);
    ui->rectifierModeGroup->setDisabled(locked);
    ui->suppressGroup->setDisabled(locked);
    ui->sampleAverageGroup->setDisabled(locked);
    ui->groupTransmitMode->setDisabled(locked);
    ui->probeBandGroup->setDisabled(locked);
}

/*! 关联检测状态改变 */
void PanelUTSetup::onAlterSourceChanged(bool checked)
{
    int count = ui->tab->count();
    for (int i = 1; i < count; ++i) {                                           // 第一个界面不需要锁定
        ui->tab->widget(i)->setEnabled(!checked);
    }
}

/*! 初始化UI界面 */
void PanelUTSetup::initUi()
{
    handler.add(ui->alterSource,        &UTSetup::alterSource,      &UTSetup::setAlterSource);
    handler.addHandler(ui->name, &QLineEdit::setText, &QLineEdit::textChanged, &UTSetup::name, &UTSetup::setName);

    handler.addHandler(ui->widgetTimebase, &PanelUTSetupTimebase::setTimebase, &PanelUTSetupTimebase::timebaseChanged, &UTSetup::timebase, &UTSetup::setTimebase);

    handler.add(ui->pulserEnable,       &UTSetup::pulserEnable,     &UTSetup::setPulserEnable);
    handler.add(ui->pulseWidth,         &UTSetup::pulseWidth,       &UTSetup::setPulseWidth);
    handler.add(ui->impedance,          &UTSetup::impedance,        &UTSetup::setImpedance);
    handler.add(ui->transmitVoltage,    &UTSetup::transmitVoltage,  &UTSetup::setTransmitVoltage);
    handler.add(ui->transmitMode,       &UTSetup::transmitMode,     &UTSetup::setTransmitMode);
    handler.add(ui->transmitEnergy,     &UTSetup::transmitEnergy,   &UTSetup::setTransmitEnergy);

    handler.add(ui->prf,                &UTSetup::prf,              &UTSetup::setPrf);
    handler.add(ui->prfSlider,          &UTSetup::prf,              &UTSetup::setPrf);
    handler.add(ui->pulseWidth,         &UTSetup::pulseWidth,       &UTSetup::setPulseWidth);

    handler.add(ui->gain,               &UTSetup::gain,             &UTSetup::setGain);
    handler.add(ui->gainSlider,         &UTSetup::gain,             &UTSetup::setGain);
    handler.add(ui->gainRef,            &UTSetup::gainRef,          &UTSetup::setGainRef);
    handler.add(ui->gainRefSlider,      &UTSetup::gainRef,          &UTSetup::setGainRef);
    handler.add(ui->manualGain,         &UTSetup::manualGain,       &UTSetup::setManualGain);
    handler.add(ui->preAmplify,         &UTSetup::preAmplify,       &UTSetup::setPreAmplify);
    handler.add(ui->amplify1,           &UTSetup::amplify1,         &UTSetup::setAmplify1);
    handler.add(ui->amplify2,           &UTSetup::amplify2,         &UTSetup::setAmplify2);
    handler.add(ui->sampleAverage,      &UTSetup::sampleAverage,    &UTSetup::setSampleAverage);
    handler.add(ui->probeBand,          &UTSetup::probeBand,        &UTSetup::setProbeBand);
    handler.add(ui->suppress,           &UTSetup::suppress,         &UTSetup::setSuppress);
    handler.add(ui->rectifierMode,      &UTSetup::rectifierMode,    &UTSetup::setRectifierMode);
    handler.add(ui->probeMultiplex,     &UTSetup::probeMultiplex,   &UTSetup::setProbeMultiplex);

    handler.add(ui->dacMode,            &UTSetup::dacMode,          &UTSetup::setDacMode);
    handler.add(ui->dacTarget,          &UTSetup::dacTarget,        &UTSetup::setDacTarget);
    handler.add(ui->dacTimebase,        &UTSetup::dacTimebase,      &UTSetup::setDacTimebase);
}

void PanelUTSetup::setTabPage(const QString &name)
{
    auto *page = ui->tab->findChild<QWidget *>(name);
    if (page && ui->tab->indexOf(page) >= 0) {
        ui->tab->setCurrentWidget(page);
    }
}

bool PanelUTSetup::isReadonly() const
{
    return m_readonly;
}

void PanelUTSetup::on_importSetup_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("导入工艺"), QString(), tr("工艺文件 (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }
    QString error;
    QJsonObject json = ReadFileAsJson(fileName, &error).toObject();
    if (json.isEmpty()) {
        QMessageBox::warning(this, QString(), tr("导入失败: %1").arg(error));
        return;
    }
    setSetup(UTSetup::fromJson(json));
}

void PanelUTSetup::on_exportSetup_clicked()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hhmmss");;
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出工艺"), time, tr("工艺文件 (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }
    QString error;
    bool ok = WriteFileAsJson(fileName, m_setup.toJson(), false, &error);
    if (!ok) {
        QMessageBox::critical(this, QString(), tr("导出失败: %1").arg(error));
    }
}

void PanelUTSetup::on_addGate_clicked()
{
    UTSetupGate gate = m_setup.suggestGate();
    handler.set(&UTSetup::appendGate, gate);
    ui->gates->selectionModel()->select(gateModel->rootItem()->lastChild()->index(), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows | QItemSelectionModel::Clear);
}

void PanelUTSetup::on_removeGate_clicked()
{
    QModelIndex index = ui->gates->currentIndex();
    if (index.isValid() && index.parent() == gateModel->rootItem()->index()) {
        handler.set(&UTSetup::removeGate, index.row());
    }
}

void PanelUTSetup::on_addDac_clicked()
{
    bool ok;
    qreal time = QInputDialog::getDouble(this, QString(), tr("DAC样点位置 (%1)").arg(m_setup.timebaseSuffix()), 0, 0, Q_INFINITY, 2, &ok);
    if (!ok) {
        return;
    }

    time = m_setup.fromTimebaseUnit(time);
    InterpolateTable dac = m_setup.dac();
    qreal gain = dac.linearInterpolate(time);
    dac[time] = gain;
    handler.set(&UTSetup::setDac, dac);
}

void PanelUTSetup::on_removeDac_clicked()
{
    QModelIndex index = ui->dac->currentIndex();
    if (index.isValid()) {
        InterpolateTable dac = m_setup.dac();
        dac.erase(dac.begin() + index.row());
        handler.set(&UTSetup::setDac, dac);
    }
}

void PanelUTSetup::on_gates_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->gates->indexAt(pos);
    TreeItem *item = gateModel->itemForIndex(index);
    if (item && item->parent() == gateModel->rootItem()) {
        int gateIndex = index.row();
        QMenu menu(this);
        menu.addAction(tr("重置位置"), [=] () {
            UTSetupGate gate = setup().suggestGate(gateIndex);
            handler.set(&UTSetup::setGate, gateIndex, gate);
        });

        menu.exec(QCursor::pos());
    }
}

void PanelUTSetup::setTransmitVoltage(const int voltage)
{
    ui->transmitVoltage->blockSignals(true);
    ui->transmitVoltage->removeItem(ui->transmitVoltage->findData(QVariant()));
    ui->transmitVoltage->blockSignals(false);
    int index = ui->transmitVoltage->findData(voltage);
    if (index < 0) {
        ui->transmitVoltage->setCurrentIndex(0);
    } else {
        ui->transmitVoltage->setCurrentIndex(index);
    }
}

int PanelUTSetup::voltage() const
{
    QVariant data = ui->transmitVoltage->currentData();
    if (data.isValid()) {
        return data.toInt();
    }
    return ui->transmitVoltage->currentText().toInt();
}

void PanelUTSetup::on_lock_toggled(bool checked)
{
    setReadonly(checked);
}

void PanelUTSetup::on_dgsImport_clicked()
{
    if (channel.isNull()) {
        return;
    }

    bool ok = false;
    QStringList items;
    Probe probe = channel->probe();
    DgsTable table = probe.defaultDgsTable();
    const QList<DgsCurve> &curves = table.dgsCurves;
    if (probe.uuid.isNull()) {
        QMessageBox::warning(this, {}, tr("未选择默认探头"));
        return;
    }
    if (probe.dgsTables.isEmpty() || curves.isEmpty()) {
        QMessageBox::warning(this, {}, tr("DGS曲线列表为空"));
        return;
    }

    foreach (const auto &item, curves) {
        QString string = tr("Φ%1").arg(item.diameter);
        items << string;
    }

    QString currentItem = QInputDialog::getItem(this, {}, tr("请选择平底孔当量"), items, 0, false, &ok);
    if (!ok || currentItem.isEmpty()) {
        return;
    }
    bool isNum = false;
    qreal diameter = currentItem.remove("Φ").toDouble(&isNum);
    if (!isNum || diameter <= 0) {
        return;
    }

    const DgsCurve &dgs = Utils::findOrDefault(curves, [&] (const DgsCurve &dgsCurve) {
        return qFuzzyCompare(dgsCurve.diameter, diameter);
    });
    handler.set(&UTSetup::setDac, dgs.interpolateTable);
}

void PanelUTSetup::on_dgsExport_clicked()
{
    if (channel.isNull()) {
        return;
    }
    Probe probe = channel->probe();
    if (probe.uuid.isNull()) {
        QMessageBox::warning(this, {}, tr("未选择默认探头"));
        return;
    }

    DgsTable table = probe.defaultDgsTable();
    QList<DgsCurve> dgsCurves = table.dgsCurves;
    if (probe.dgsTables.isEmpty()) {
        QMessageBox::warning(this, {}, tr("DGS曲线列表为空"));
        return;
    }
    if (m_setup.dac().isEmpty()) {
        QMessageBox::warning(this, {}, tr("DAC曲线列表为空"));
        return;
    }

    bool ok = false;
    qreal diameter = QInputDialog::getDouble(this, QString(), tr("请输入平底孔当量"), 0.3, 0.3, std::numeric_limits<qreal>::max(), 1, &ok);
    if (!ok) {
        return;
    }

    DgsCurve newCurve;
    newCurve.interpolateTable = m_setup.dac();
    newCurve.diameter = diameter;

    if (table.hasDgsCurve(diameter)) {
        if (QMessageBox::warning(this, {}, tr("确定要覆盖当前\"Φ%1\"的DGS曲线吗？").arg(newCurve.diameter), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
            return;
        }
    }
    table.updateDgsCurve(newCurve);
    probe.updateTable(table);

    if (UltrasonicManager::probeModel()->updateProbe(probe)) {
        QMessageBox::information(this, {}, tr("导出成功"));
    } else {
        QMessageBox::information(this, {}, tr("导出失败"));
    }
}
