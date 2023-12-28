#include "PlotLineEdit.h"
#include "ui_PlotLineEdit.h"

#include <QShortcut>

PlotLineEdit::PlotLineEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotLineEdit)
{
    ui->setupUi(this);

    connect(new QShortcut(QString("Ctrl+C"), this), &QShortcut::activated, this, [=]() {
        writeClipboard(option());
    });
    connect(new QShortcut(QString("Ctrl+V"), this), &QShortcut::activated, this, [=]() {
        setOption(readClipboard<PlotLine>());
    });
}

PlotLineEdit::~PlotLineEdit()
{
    delete ui;
}

PlotLine PlotLineEdit::option() const
{
    PlotLine option;
    option.name = ui->name->text();
    option.orientation = Qt::Orientation(ui->orientation->currentIndex() + Qt::Horizontal);
    option.value = ui->value->value();
    option.color = ui->color->color();
    option.alarmMode = PlotLine::AlarmMode(ui->alarmMode->currentIndex());
    option.comment = ui->comment->text();

    return option;
}

void PlotLineEdit::setOption(const PlotLine &option)
{
    ui->name->setText(option.name);
    ui->orientation->setCurrentIndex(option.orientation - Qt::Horizontal);
    ui->value->setValue(option.value);
    ui->color->setColor(option.color);
    ui->alarmMode->setCurrentIndex(option.alarmMode);
    ui->comment->setText(option.comment);
}
