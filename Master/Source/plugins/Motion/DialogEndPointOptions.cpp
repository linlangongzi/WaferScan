#include "DialogEndPointOptions.h"
#include "ui_DialogEndPointOptions.h"
#include "MotionManager.h"
#include "DialogAxisConfig.h"
#include <utils/algorithm.h>
#include <QInputDialog>


DialogEndPointOptions::DialogEndPointOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEndPointOptions)
{
    ui->setupUi(this);
    this->setEnabled(MotionManager::driver()->status().state == Motion::ControllerState::Disconnected);
}

DialogEndPointOptions::~DialogEndPointOptions()
{
    delete ui;
}

void DialogEndPointOptions::loadEndPoint(EndPoint *endPoint)
{
    m_endPoint = endPoint;

    ui->uuid->setText(endPoint->uuid().toString());
    ui->name->setText(endPoint->name());
    ui->type->setCurrentIndex(endPoint->type());
    ui->offset->setText(Vector3DToString(endPoint->offset()));

    ui->axes->clear();
    for (Axis *axis: endPoint->axisList()) {
        auto *item = new QListWidgetItem;
        item->setText(axis->name());
        item->setData(Qt::UserRole, QVariant::fromValue(axis->uuid()));
        ui->axes->addItem(item);
    }
}

void DialogEndPointOptions::on_add_clicked()
{
    QList<Axis *> axes;
    for (int row = 0; row < ui->axes->count(); ++row) {
        QUuid uuid = ui->axes->item(row)->data(Qt::UserRole).toUuid();
        axes << MotionManager::axis(uuid);
    }

    axes = Utils::filtered(MotionManager::axisList(), [&](Axis *axis){
        return !axes.contains(axis);
    });

    QList<QString> names = Utils::transform(axes, &Axis::name);
    if (names.isEmpty()) {
        return;
    }

    bool ok = false;
    QString name = QInputDialog::getItem(this, QString(), tr("选择轴"), names, 0, false, &ok);
    if (!ok) {
        return;
    }
    addAxis(axes.value(names.indexOf(name))->uuid());
}

void DialogEndPointOptions::on_remove_clicked()
{
    delete ui->axes->takeItem(ui->axes->currentRow());
}

void DialogEndPointOptions::addAxis(const QUuid &uuid)
{
    auto *item = new QListWidgetItem;
    item->setText(MotionManager::axis(uuid)->name());
    item->setData(Qt::UserRole, QVariant::fromValue(uuid));
    ui->axes->addItem(item);
}

void DialogEndPointOptions::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok) {
        m_endPoint->setName(ui->name->text());
        m_endPoint->setType(EndPoint::EndPointType(ui->type->currentIndex()));
        m_endPoint->setOffset(Vector3DFromString(ui->offset->text()));

        QList<Axis *> axes;
        for (int row = 0; row < ui->axes->count(); ++row) {
            QUuid uuid = ui->axes->item(row)->data(Qt::UserRole).toUuid();
            axes << MotionManager::axis(uuid);
        }
        m_endPoint->setAxisList(axes);
    }
}
