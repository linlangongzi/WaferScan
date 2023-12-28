#include "DialogCheating.h"
#include "ui_DialogCheating.h"
#include <arsCore/Platform.h>
#include <QFileDialog>
#include <QDebug>
#include <allrising/Util.h>

DialogCheating::DialogCheating(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCheating)
{
    ui->setupUi(this);

    ui->filePathList->clear();
    ui->filePathList->addItems(convertFromJson<QStringList>(platform()->Get(K_CHEAT_FILE_LIST_PATH)));
    ui->noise->setValue(platform()->Get(K_CHEAT_FILE_NOISE).toDouble());
    ui->cheatAfterScan->setChecked(platform()->Get(K_CHEAT_AFTER_SCAN).toBool());
    ui->shortcut->setKeySequence(QKeySequence(platform()->Get(K_CHEAT_SHORTCUT).toString(), QKeySequence::PortableText));
    ui->allowEdit->setChecked(platform()->Get(K_CHEAT_EDIT).toBool());
}

DialogCheating::~DialogCheating()
{
    delete ui;
}

void DialogCheating::accept()
{
    QStringList pathList;
    const int rowCount = ui->filePathList->count();
    for (int row = 0; row < rowCount; ++row) {
        pathList << ui->filePathList->item(row)->text();
    }
    platform()->Set(K_CHEAT_FILE_LIST_PATH, convertToJson(pathList));
    platform()->Set(K_CHEAT_FILE_NOISE, ui->noise->value());
    platform()->Set(K_CHEAT_AFTER_SCAN, ui->cheatAfterScan->isChecked());
    platform()->Set(K_CHEAT_SHORTCUT, ui->shortcut->keySequence().toString(QKeySequence::PortableText));
    platform()->Set(K_CHEAT_EDIT, ui->allowEdit->isChecked());

    QDialog::accept();
}

void DialogCheating::on_addPath_clicked()
{
    QString path = QFileDialog::getOpenFileName(this);
    if (!path.isEmpty()) {
        ui->filePathList->addItem(path);
    }
}

void DialogCheating::on_removePath_clicked()
{
    delete ui->filePathList->takeItem(ui->filePathList->currentRow());
}
