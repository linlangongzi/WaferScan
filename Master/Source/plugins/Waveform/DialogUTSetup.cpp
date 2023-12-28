#include "DialogUTSetup.h"
#include "ui_DialogUTSetup.h"

DialogUTSetup::DialogUTSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUTSetup)
{
    ui->setupUi(this);
    ui->setup->hideAlterSourceCheckbox(true);
    ui->setup->setPrfVisible(true);
}

DialogUTSetup::~DialogUTSetup()
{
    delete ui;
}

UTSetup DialogUTSetup::setup() const
{
    return ui->setup->setup();
}

void DialogUTSetup::setSetup(const UTSetup &setup)
{
    ui->setup->setSetup(setup);
}
