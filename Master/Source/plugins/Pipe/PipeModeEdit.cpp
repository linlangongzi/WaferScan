#include "PipeModeEdit.h"
#include "ui_PipeModeEdit.h"
#include <Imaging/DialogImageOptions.h>

#include <QShortcut>
#include <QComboBox>
#include <QInputDialog>
#include <Plot/PlotViewEditor.h>

PipeModeEdit::PipeModeEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PipeModeEdit)
{
    ui->setupUi(this);

    connect(new QShortcut(QString("Ctrl+C"), this), &QShortcut::activated, this, [=]() {
        writeClipboard(config());
    });
    connect(new QShortcut(QString("Ctrl+V"), this), &QShortcut::activated, this, [=]() {
        setConfig(readClipboard<PipeModeConfig>());
    });
}

PipeModeEdit::~PipeModeEdit()
{
    delete ui;
}

PipeModeConfig PipeModeEdit::config() const
{
    PipeModeConfig config;
    config.setName(ui->name->text());

    for (int index = 0; index < ui->bscan->count(); ++index) {
        config.plots << ui->bscan->item(index)->data(Qt::UserRole).value<PlotView>();
    }

    for (int index = 0; index < ui->images->count(); ++index) {
        ImageOptions image = ui->images->item(index)->data(Qt::UserRole).value<ImageOptions>();
        config.images << image;
    }

    return config;
}

void PipeModeEdit::setConfig(PipeModeConfig config)
{
    ui->name->setText(config.name());

    ui->bscan->clear();
    for (const auto &plot: config.plots) {
        addPlot(plot);
    }

    ui->images->clear();
    int count = config.images.count();
    for (int index = 0; index < count; ++index) {
        auto item = new QListWidgetItem(ui->images);
        item->setText(tr("C扫%1").arg(index + 1));
        item->setData(Qt::UserRole, QVariant::fromValue(config.images.value(index)));
    }

    m_config = config;
}

void PipeModeEdit::addPlot(const PlotView &view)
{
    auto item = new QListWidgetItem(ui->bscan);
    item->setText(view.name);
    item->setData(Qt::UserRole, QVariant::fromValue(view));
}

void PipeModeEdit::on_bscan_itemDoubleClicked(QListWidgetItem *item)
{
    PlotViewEditor dialog(this);
    dialog.setOption(item->data(Qt::UserRole).value<PlotView>());
    if (dialog.exec() == QDialog::Accepted) {
        const auto option = dialog.option();
        item->setText(option.name);
        item->setData(Qt::UserRole, QVariant::fromValue(option));
    }
}

void PipeModeEdit::on_addPlot_clicked()
{
    PlotViewEditor dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        addPlot(dialog.option());
    }
}

void PipeModeEdit::on_removePlot_clicked()
{
    delete ui->bscan->currentItem();
}

void PipeModeEdit::on_addImage_clicked()
{
    int index = ui->images->count();
    auto item = new QListWidgetItem(ui->images);
    item->setText(tr("C扫%1").arg(index));

    if (index < m_config.images.count()) {
        item->setData(Qt::UserRole, QVariant::fromValue(m_config.images.value(index)));
    }
}

void PipeModeEdit::on_removeImage_clicked()
{
    delete ui->images->currentItem();
}

void PipeModeEdit::on_images_itemDoubleClicked(QListWidgetItem *item)
{
    DialogImageOptions dialog(this);
    dialog.SetOptions(item->data(Qt::UserRole).value<ImageOptions>());
    if (dialog.exec() == QDialog::Accepted) {
        const auto option = dialog.GetOptions();
        item->setData(Qt::UserRole, QVariant::fromValue(option));
    }
}
