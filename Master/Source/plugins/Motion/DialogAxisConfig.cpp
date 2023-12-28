#include "DialogAxisConfig.h"
#include "ui_DialogAxisConfig.h"
#include "Axis.h"
#include "MotionManager.h"
#include <Core/LicenseManager.h>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <allrising/Util.h>

static Q_LOGGING_CATEGORY(debug, "motion.axisConfig");

enum {
    NameRole = Qt::UserRole,
    IdRole = Qt::UserRole + 1,
};

DialogAxisConfig::DialogAxisConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAxisConfig)
{
    ui->setupUi(this);
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Apply);

    ui->type->addItem(tr("平动"), EnumToKey(AxisConfig::Prismatic));
    ui->type->addItem(tr("旋转"), EnumToKey(AxisConfig::Rotation));
    ui->type->addItem(tr("有限旋转"), EnumToKey(AxisConfig::RotationLimited));

    ui->groupRobotics->setEnabled(false);

    LicenseManager::registerLicenseUpdate(this, &DialogAxisConfig::onLicenseUpdated);

    initUi(MotionManager::driver()->status());
}

DialogAxisConfig::~DialogAxisConfig()
{
    delete ui;
}

void DialogAxisConfig::loadAxis(Axis *newAxis)
{
    axis = newAxis;
    if (!axis) {
        return;
    }

    AxisConfig config = axis->config();
    AxisSetup setup = axis->setup();

    ui->uuid->setText(axis->uuid().toString());
    ui->name->setText(config.name);
    ui->type->setCurrentIndex(config.axisType);
    ui->axisId->setValue(config.axisId);
    ui->hidden->setChecked(config.hidden);

    ui->units->setValue(config.units);
    ui->repDist->setValue(config.repeatDistance);
    ui->encoderId->setValue(config.encoderId);
    ui->unitsInstrument->setValue(config.unitsInstrument);
    ui->backlash->setValue(config.backlash);

    ui->speedMax->setValue(config.speedMax);
    ui->speedJog->setValue(setup.speedJog);
    ui->speedLocate->setValue(setup.speedLocate);
    ui->step->setValue(setup.jogStep);

    ui->accelMax->setValue(config.accelLimit);
    ui->accel->setValue(setup.accel);
    ui->accel->setMaximum(config.accelLimit);
    ui->decelMax->setValue(config.decelLimit);
    ui->decel->setValue(setup.decel);
    ui->decel->setMaximum(config.decelLimit);
    ui->sRamp->setValue(config.sRamp);

    ui->datumIn->setValue(config.datumInput);
    ui->datumSpeed->setValue(config.datumSpeed);
    ui->datumCreep->setValue(config.datumCreep);
    ui->datumSequence->setValue(config.datumSequence);
    ui->datumReversed->setChecked(config.datumReversed);
    ui->datumDef->setValue(config.datumPosition);

    ui->jogForwardInput->setValue(config.jogForwardInput);
    ui->jogReverseInput->setValue(config.jogReverseInput);

    ui->forwardLimit->setValue(config.limitForward);
    ui->reverseLimit->setValue(config.limitReverse);
    ui->forwardIn->setValue(config.limitForwardInput);
    ui->reverseIn->setValue(config.limitReverseInput);
    ui->forwardKey->setKeySequence(setup.forwardKey);
    ui->reverseKey->setKeySequence(setup.reverseKey);

    ui->offset->setText(Vector3DToString(config.basePoint));
    ui->direction->setText(Vector3DToString(config.baseDirection));
    ui->carryAxleDiameter->setValue(config.axleDiameter);
    ui->sensorList->clear();
    const auto sensors = config.sensors;
    for (auto it = sensors.begin(); it != sensors.end(); ++it) {
        addSensor(it.key(), it.value());
    }

    const QString unit = axis->axisUnit();
    foreach (QDoubleSpinBox *box, findChildren<QDoubleSpinBox *>()) {
        const QString suffix = box->suffix();
        if (suffix.contains("%1")) {
            box->setSuffix(suffix.arg(unit));
        }
    }
}

int DialogAxisConfig::exec()
{
    qDebug() << "AxisConfig: using full button";
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    return QDialog::exec();
}

void DialogAxisConfig::addSensor(const QString &name, int id)
{
    auto *item = new QListWidgetItem(QListWidgetItem(tr("%1 (%2)").arg(name).arg(id)));
    item->setData(NameRole, name);
    item->setData(IdRole, id);
    ui->sensorList->addItem(item);
}

void DialogAxisConfig::onLicenseUpdated()
{
    QWidgetList developWidgets({
        ui->groupBasic,
        ui->groupJog,
        ui->speedMax,
        ui->accelMax,
        ui->carryAxleDiameter,
        ui->decelMax,
        ui->sRamp,
        ui->units,
        ui->unitsInstrument,
        ui->encoderId,
        ui->axisId,
        ui->repDist,
        ui->tabOther,
        ui->groupDatum,
        ui->reverseIn,
        ui->forwardIn,
        ui->tabOther,
    });

    bool develop = LicenseManager::instance()->hasDevelopLicense();
    for (QWidget *widget: developWidgets) {
        widget->setEnabled(develop);
    }
    ui->licenseWarning->setVisible(!develop);
}

void DialogAxisConfig::on_forwardKeyReset_clicked()
{
    ui->forwardKey->setKeySequence(QKeySequence(QString()));
}

void DialogAxisConfig::on_reverseKeyReset_clicked()
{
    ui->reverseKey->setKeySequence(QKeySequence(QString()));
}

void DialogAxisConfig::on_stop_clicked()
{
    axis->cancelMove();
}

void DialogAxisConfig::on_forward_clicked()
{
    axis->SetSpeed(ui->speedJog->value());
    axis->MoveForward();
}

void DialogAxisConfig::on_reverse_clicked()
{
    axis->SetSpeed(ui->speedJog->value());
    axis->MoveReverse();
}

void DialogAxisConfig::on_moveabsGo_clicked()
{
    axis->SetSpeed(ui->speedJog->value());
    axis->MoveAbs(ui->moveabsPosition->value());
}

void DialogAxisConfig::on_moveReverse_clicked()
{
    axis->SetSpeed(ui->speedJog->value());
    axis->MoveRel(-ui->moveDisplacement->value());
}

void DialogAxisConfig::on_moveForward_clicked()
{
    axis->SetSpeed(ui->speedJog->value());
    axis->MoveRel(ui->moveDisplacement->value());
}

void DialogAxisConfig::on_buttonBox_accepted()
{
    apply();
    accept();
}

void DialogAxisConfig::on_buttonBox_rejected()
{
    reject();
}

void DialogAxisConfig::apply()
{
    if (axis.isNull()) {
        return;
    }
    if (ui->forwardLimit->value() < ui->reverseLimit->value()) {
        QMessageBox::warning(this, QApplication::applicationName(), tr("前向限位小于后向限位"));
        return;
    }

    AxisConfig config;
    AxisSetup setup;
    config.axisId = ui->axisId->value();
    config.name = ui->name->text();
    config.axisType = AxisConfig::AxisType(ui->type->currentIndex());
    config.hidden = ui->hidden->isChecked();

    config.units = ui->units->value();

    config.repeatDistance = ui->repDist->value();
    config.encoderId = ui->encoderId->value();
    config.unitsInstrument = ui->unitsInstrument->value();
    config.backlash = ui->backlash->value();

    setup.speedJog = ui->speedJog->value();
    setup.speedLocate = ui->speedLocate->value();
    config.speedMax = ui->speedMax->value();
    setup.jogStep = ui->step->value();

    setup.accel = ui->accel->value();
    config.accelLimit = ui->accelMax->value();
    setup.decel = ui->decel->value();
    config.decelLimit = ui->decelMax->value();
    config.sRamp = ui->sRamp->value();

    config.datumInput = ui->datumIn->value();
    config.datumSpeed = ui->datumSpeed->value();
    config.datumCreep = ui->datumCreep->value();
    config.datumSequence = ui->datumSequence->value();
    config.datumPosition = ui->datumDef->value();
    config.datumReversed = ui->datumReversed->isChecked();

    config.jogForwardInput = ui->jogForwardInput->value();
    config.jogReverseInput = ui->jogReverseInput->value();
    config.axleDiameter = ui->carryAxleDiameter->value();
    config.limitForward = ui->forwardLimit->value();
    config.limitReverse = ui->reverseLimit->value();
    config.limitForwardInput = ui->forwardIn->value();
    config.limitReverseInput = ui->reverseIn->value();
    setup.forwardKey = ui->forwardKey->keySequence();
    setup.reverseKey = ui->reverseKey->keySequence();

    config.basePoint = Vector3DFromString(ui->offset->text());
    config.baseDirection = Vector3DFromString(ui->direction->text());

    for (int index = 0; index < ui->sensorList->count(); ++index) {
        auto *item = ui->sensorList->item(index);
        config.sensors[item->data(NameRole).toString()] = item->data(IdRole).toInt();
    }

    axis->setConfig(config);
    axis->setSetup(setup);
}

void DialogAxisConfig::on_speedMax_editingFinished()
{
    qreal maxSpeed = ui->speedMax->value();
    ui->speedJog->setMaximum(maxSpeed);
    ui->datumSpeed->setMaximum(maxSpeed);
    ui->datumCreep->setMaximum(maxSpeed);
    ui->speedLocate->setMaximum(maxSpeed);
}

void DialogAxisConfig::on_accelMax_editingFinished()
{
    ui->accel->setMaximum(ui->accelMax->value());
}

void DialogAxisConfig::on_decelMax_editingFinished()
{
    ui->decel->setMaximum(ui->decelMax->value());
}

void DialogAxisConfig::on_forwardLimit_editingFinished()
{
    ui->reverseLimit->setMaximum(ui->forwardLimit->value());
}

void DialogAxisConfig::on_sensorAdd_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, QString(), tr("传感器名称"), QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }
    int addr = QInputDialog::getInt(this, QString(), tr("传感器IO地址"), 0, 0, 1024, 1, &ok);
    if (!ok) {
        return;
    }
    addSensor(name, addr);
}

void DialogAxisConfig::on_sensorDelete_clicked()
{
    delete ui->sensorList->currentItem();
}

void DialogAxisConfig::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply) {
        apply();
    }
}

void DialogAxisConfig::on_licenseManualRobotics_clicked()
{
    ui->groupRobotics->setEnabled(true);
}

void DialogAxisConfig::initUi(const Motion::ControllerStatus &status)
{
    bool isDisconnect = (status.state == Motion::ControllerState::Disconnected);
    QWidgetList editableWidgets({
        ui->groupBasic,
        ui->groupRobotics,
        ui->groupAccel,
        ui->groupJog,
        ui->groupDatum,
        ui->groupPosition,
        ui->groupLimit,

        ui->speedMax,
        ui->sensorList,
        ui->licenseManualRobotics,
    });
    for (QWidget *widget: editableWidgets) {
        widget->setEnabled(isDisconnect);
    }

    bool isNormal = (status.state == Motion::ControllerState::Normal);
    ui->groupDebug->setEnabled(isNormal);
}
