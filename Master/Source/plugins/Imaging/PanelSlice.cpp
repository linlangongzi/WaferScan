#include "PanelSlice.h"
#include "ui_PanelSlice.h"

PanelSlice::PanelSlice(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelSlice)
{
    ui->setupUi(this);
}

PanelSlice::~PanelSlice()
{
    delete ui;
}
