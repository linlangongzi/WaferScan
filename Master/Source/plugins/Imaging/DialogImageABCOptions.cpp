#include "DialogImageABCOptions.h"
#include "ui_DialogImageABCOptions.h"

DialogImageABCOptions::DialogImageABCOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageABCOptions)
{
    ui->setupUi(this);

    ui->widImageA->setRange(64, 1024);
}

DialogImageABCOptions::~DialogImageABCOptions()
{
    delete ui;
}

int DialogImageABCOptions::GetWidthImageA()
{
    return ui->widImageA->value();
}

bool DialogImageABCOptions::isBlank()
{
    if (ui->isBlank->isChecked()) {
        return true;
    } else {
        return false;
    }
}

bool DialogImageABCOptions::isTimeStamp()
{
    if (ui->isTimeStamp->isChecked()) {
        return true;
    } else {
        return false;
    }
}

void DialogImageABCOptions::on_submit_clicked()
{
    QDialog::accept();
}

void DialogImageABCOptions::on_cancel_clicked()
{
    QDialog::reject();
}
