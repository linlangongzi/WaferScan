#include "PanelMaterialEdit.h"
#include "ui_PanelMaterialEdit.h"

const double PanelMaterialEdit::MAX_VELOCITY = 0x8FFFFFFFFFFFF;
const double PanelMaterialEdit::MIN_VELOCITY = 0.0;
PanelMaterialEdit::PanelMaterialEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PanelMaterialEdit)
{
    ui->setupUi(this);
    setWindowTitle(tr("MaterialEditDialog"));
    dValidator = new QDoubleValidator(MIN_VELOCITY, MAX_VELOCITY, 10, this);  // 10为进制的格式
    ui->MaterialVelocityLineEdit->setValidator(dValidator);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

PanelMaterialEdit::~PanelMaterialEdit()
{
    if (dValidator)
    {
        delete dValidator;
        dValidator = NULL;
    }
    delete ui;
}

QString PanelMaterialEdit::GetMaterialName()
{
    return MaterialName;
}

double PanelMaterialEdit::GetMaterialVelocity()
{
    return MaterialVelocity;
}

void PanelMaterialEdit::on_buttonBox_accepted()
{
    if (ui->MaterialNameLineEdit->text() == "")
    {
        QMessageBox::warning(this, tr("Warning"), tr("The Name can't br null!"));
        return;
    }
    MaterialName = ui->MaterialNameLineEdit->text();
    MaterialVelocity = ui->MaterialVelocityLineEdit->text().toDouble();
}
