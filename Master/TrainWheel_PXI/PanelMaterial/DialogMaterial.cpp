#include "DialogMaterial.h"
#include "ui_DialogMaterial.h"
#include "PanelMaterial.h"

DialogMaterial::DialogMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMaterial)
{
    ui->setupUi(this);
    ui->materialList->SetSelectMode(true);
    connect(ui->materialList, SIGNAL(SelectSubmit()), this, SLOT(accept()));
}

DialogMaterial::~DialogMaterial()
{
    delete ui;
}

Material *DialogMaterial::GetMaterial()
{
    return ui->materialList->GetSelection();
}
