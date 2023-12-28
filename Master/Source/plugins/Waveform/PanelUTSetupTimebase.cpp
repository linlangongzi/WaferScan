#include "PanelUTSetupTimebase.h"
#include "ui_PanelUTSetupTimebase.h"
#include <Ultrasonic/UTSetup.h>
#include "DialogMaterial.h"
#include <allrising/Util.h>
#include <QDebug>

static const QString warningStyle("color: #FF0000");

PanelUTSetupTimebase::PanelUTSetupTimebase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelUTSetupTimebase)
{
    ui->setupUi(this);

    using Getter = qreal (UTSetupTimebase::*)() const;
    using Setter = void (UTSetupTimebase::*)(qreal);

    auto makeControl = [=](QDoubleSpinBox *time, QDoubleSpinBox *distance, QSlider *slider, Getter getter, Setter setter) {
        connect(time, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [=](double arg1) {
                (m_timebase.*setter)(arg1);
                QSignalBlocker blocker(distance);
                distance->setValue(toDistance(arg1));
                blocker.unblock();
                emit timebaseChanged(m_timebase);
            }
        );

        connect(distance, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [=](double arg1) {
                (m_timebase.*setter)(toTime(arg1));
                QSignalBlocker blocker(time);
                time->setValue((m_timebase.*getter)());
                blocker.unblock();
                emit timebaseChanged(m_timebase);
            }
        );

        connect(slider, &QSlider::sliderPressed, this, [=]() {
            slider->setProperty("startValue", (m_timebase.*getter)());
        });

        connect(slider, &QSlider::sliderReleased, this, [=]() {
            slider->setValue(0);
        });

        connect(slider, &QSlider::sliderMoved, this, [=](int value) {
            qreal startValue = slider->property("startValue").toReal();
            if (!slider->isSliderDown()) {
                startValue = (m_timebase.*getter)();
                slider->setValue(0);
            }
            qreal delta = qMax<qreal>(10, startValue);
            qreal newValue = startValue + delta * qreal(value) / 100;
            time->setValue(newValue);
        });
    };

    makeControl(ui->delayTime,  ui->delayDistance,  ui->delaySlider,    &UTSetupTimebase::delay,    &UTSetupTimebase::setDelay);
    makeControl(ui->rangeTime,  ui->rangeDistance,  ui->rangeSlider,    &UTSetupTimebase::range,    &UTSetupTimebase::setRange);
    makeControl(ui->zeroTime,   ui->zeroDistance,   ui->zeroSlider,     &UTSetupTimebase::zero,     &UTSetupTimebase::setZero);

    connect(ui->surfaceTracing, &PanelUTSetupSurfaceTracing::surfaceTracingChanged, this, &PanelUTSetupTimebase::onSurfaceTracingChanged);

    refreshControls();
}

PanelUTSetupTimebase::~PanelUTSetupTimebase()
{
    delete ui;
}

UTSetupTimebase PanelUTSetupTimebase::timebase() const
{
    return m_timebase;
}

bool PanelUTSetupTimebase::isReadOnly() const
{
    return m_readOnly;
}

void PanelUTSetupTimebase::setSurfaceTracingVisible(bool visible)
{
    ui->surfaceTracing->setVisible(visible);
}

void PanelUTSetupTimebase::setTimebase(const UTSetupTimebase &timebase)
{
    if (m_timebase == timebase) {
        return;
    }

    m_timebase = timebase;

    refreshControls();
    emit timebaseChanged(timebase);
}

void PanelUTSetupTimebase::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
    for (auto *spinbox: findChildren<QDoubleSpinBox *>(QString(), Qt::FindDirectChildrenOnly)) {
        spinbox->setDisabled(readOnly);
    }
    for (auto *slider: findChildren<QSlider *>()) {
        slider->setDisabled(readOnly);
    }
}

void PanelUTSetupTimebase::refreshControls()
{
    ui->delayTime->setValue(m_timebase.delay());
    ui->delayDistance->setValue(toDistance(m_timebase.delay()));
    ui->rangeTime->setValue(m_timebase.range());
    ui->rangeDistance->setValue(toDistance(m_timebase.range()));
    ui->zeroTime->setValue(m_timebase.zero());
    ui->zeroDistance->setValue(toDistance(m_timebase.zero()));
    ui->surfaceTracing->setSurfaceTracing(m_timebase.surfaceTracing());
    QSignalBlocker blockerDistance(ui->preferDistance);
    QSignalBlocker blockerTime(ui->preferTime);
    if (m_timebase.timebaseUnit() == UTSetupTimebase::Distance) {
        ui->preferDistance->setChecked(true);
    } else {
        ui->preferTime->setChecked(true);
    }

    ui->materialName->setText(m_timebase.material().name);
    QSignalBlocker blockerMaterial(ui->materialSpeed);
    ui->materialSpeed->setValue(m_timebase.material().longitudinalVelocity);
    blockerMaterial.unblock();
    blockerTime.unblock();
    blockerDistance.unblock();
}

void PanelUTSetupTimebase::on_changeMaterial_clicked()
{
    DialogMaterial dialog;
    if (dialog.exec() == QDialog::Accepted) {
        m_timebase.setMaterial(dialog.currentMaterial());
        refreshControls();
        emit timebaseChanged(m_timebase);
    }
}

void PanelUTSetupTimebase::on_preferTime_toggled(bool checked)
{
    auto unit = checked ? UTSetupTimebase::Time : UTSetupTimebase::Distance;
    UTSetupTimebase timebase = m_timebase;
    timebase.setTimebaseUnit(unit);
    setTimebase(timebase);
}

qreal PanelUTSetupTimebase::toTime(qreal value) const
{
    return m_timebase.toTime(value);
}

qreal PanelUTSetupTimebase::toDistance(qreal value) const
{
    return m_timebase.toDistance(value);
}

void PanelUTSetupTimebase::on_materialSpeed_valueChanged(qreal arg1)
{
    auto timebase = m_timebase;
    auto material = timebase.material();
    material.longitudinalVelocity = arg1;
    timebase.setMaterial(material);
    setTimebase(timebase);
}

void PanelUTSetupTimebase::onSurfaceTracingChanged(const UTSetupSurfaceTracing &surfaceTracing)
{
    auto timebase = m_timebase;
    timebase.setSurfaceTracing(surfaceTracing);
    setTimebase(timebase);
}
