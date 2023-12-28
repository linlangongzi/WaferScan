#include "DialogTrainConfig.h"
#include "ui_DialogTrainConfig.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

DialogTrainConfig::DialogTrainConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTrainConfig)
{
    ui->setupUi(this);
}

DialogTrainConfig::~DialogTrainConfig()
{
    delete ui;
}

void DialogTrainConfig::setConfig(const WheelSetLayout &config)
{
    ui->name->setText(config.name);
    ui->pattern->setText(config.pattern);
    ui->trainType->setText(config.trainType);
    ui->setupPath->setText(config.setupPath);

    ui->wheelNames->clear();
    for (int index = 0; index < config.wheelName.size(); ++index) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(config.wheelName.value(index));
        ui->wheelNames->addItem(item);
    }

    QFile file;
    file.setFileName(config.setupPath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(NULL, QString(), tr("当前车型的默认工艺路径无效，请重新设置默认工艺"));
    }
}

WheelSetLayout DialogTrainConfig::config() const
{
    WheelSetLayout cfg;
    cfg.name = ui->name->text();
    cfg.pattern = ui->pattern->text();
    cfg.trainType = ui->trainType->text();
    cfg.setupPath = ui->setupPath->text();

    for (int index = 0; index < ui->wheelNames->count(); ++index) {
        cfg.wheelName << ui->wheelNames->item(index)->text();
    }

    return cfg;
}

void DialogTrainConfig::on_add_clicked()
{
    bool ok = false;
    const QString name = QInputDialog::getText(this, QString(), tr("车轮名称"), QLineEdit::Normal, tr(""), &ok);
    if (tr("") == name) {
        QMessageBox::information(this, tr("车轮名称"), tr("不能添加空名称"));
        return;
    }

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(name);
    ui->wheelNames->addItem(item);
}

void DialogTrainConfig::on_remove_clicked()
{
    delete ui->wheelNames->currentItem();
}

void DialogTrainConfig::on_selectSetup_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, QString(), QString(), "(*.json)");
    if (path.isEmpty()) {
        QMessageBox::information(this, QString(), tr("选择的工艺路径为空"));
        return;
    }

    ui->setupPath->setText(path);
}

void DialogTrainConfig::on_wheelNames_itemDoubleClicked(QListWidgetItem *item)
{
    bool ok = false;
    const QString name = QInputDialog::getText(this, QString(), tr("车型:"), QLineEdit::Normal, item->text(), &ok);
    if (ok) {
        item->setText(name);
    }
}
