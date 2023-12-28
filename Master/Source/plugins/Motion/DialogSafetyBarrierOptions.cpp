#include "DialogSafetyBarrierOptions.h"
#include "ui_DialogSafetyBarrierOptions.h"

DialogSafetyBarrierOptions::DialogSafetyBarrierOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSafetyBarrierOptions)
{
    ui->setupUi(this);

}

DialogSafetyBarrierOptions::~DialogSafetyBarrierOptions()
{
    delete ui;
}

SafetyBarrierOptions DialogSafetyBarrierOptions::options() const
{
    SafetyBarrierOptions options;
    QPointF topLeft(ui->startX->value(), ui->startY->value());
    QPointF bottomRight(ui->endX->value(), ui->endY->value());

    options.disabled = ui->disabled->isChecked();
    options.rect = QRectF(topLeft, bottomRight);
    options.minDepth = ui->minDepth->value();
    options.maxDepth = ui->maxDepth->value();

    return options;
}

void DialogSafetyBarrierOptions::setOptions(const SafetyBarrierOptions &options)
{
    ui->disabled->setChecked(options.disabled);

    ui->startX->setValue(options.rect.left());
    ui->endX->setValue(options.rect.right());
    ui->startY->setValue(options.rect.top());
    ui->endY->setValue(options.rect.bottom());

    ui->minDepth->setValue(options.minDepth);
    ui->maxDepth->setValue(options.maxDepth);
}
