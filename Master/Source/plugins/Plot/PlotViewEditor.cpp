#include "PlotCurveEdit.h"
#include "PlotLineEdit.h"
#include "PlotViewEditor.h"
#include "ui_PlotViewEditor.h"
#include <QShortcut>

PlotViewEditor::PlotViewEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotViewEditor)
{
    ui->setupUi(this);

    connect(new QShortcut(QString("Ctrl+C"), this), &QShortcut::activated, this, [=]() {
        writeClipboard(option());
    });
    connect(new QShortcut(QString("Ctrl+V"), this), &QShortcut::activated, this, [=]() {
        setOption(readClipboard<PlotView>());
    });
}

PlotViewEditor::~PlotViewEditor()
{
    delete ui;
}

PlotView PlotViewEditor::option() const
{
    PlotView option;

    option.name = ui->name->text();
    qreal left = ui->left->value();
    qreal right = ui->right->value();
    qreal top = ui->top->value();
    qreal bottom = ui->bottom->value();
    option.range = QRectF(QPointF(left, top), QPointF(right, bottom)).normalized();

    for (int index = 0; index < ui->curves->count(); ++index) {
        option.curves << ui->curves->item(index)->data(Qt::UserRole).value<PlotCurve>();
    }

    for (int index = 0; index < ui->lines->count(); ++index) {
        option.lines << ui->lines->item(index)->data(Qt::UserRole).value<PlotLine>();
    }

    option.unitNameH = ui->unitNameH->text();
    option.unitNameV = ui->unitNameV->text();
    option.deadZoneLeft = ui->deadZoneLeft->value();
    option.deadZoneRight = ui->deadZoneRight->value();

    return option;
}

void PlotViewEditor::setOption(PlotView option)
{
    if (m_option == option) {
        return;
    }

    ui->name->setText(option.name);
    ui->left->setValue(option.range.left());
    ui->right->setValue(option.range.right());
    ui->top->setValue(option.range.top());
    ui->bottom->setValue(option.range.bottom());
    ui->unitNameH->setText(option.unitNameH);
    ui->unitNameV->setText(option.unitNameV);
    ui->deadZoneLeft->setValue(option.deadZoneLeft);
    ui->deadZoneRight->setValue(option.deadZoneRight);

    ui->curves->clear();
    for (const PlotCurve &curve: option.curves) {
        auto *item = new QListWidgetItem(ui->curves);
        item->setText(curve.name.isEmpty() ? tr("数据线 %1").arg(ui->curves->count()) : curve.name);
        item->setData(Qt::UserRole, QVariant::fromValue(curve));
    }

    ui->lines->clear();
    for (const PlotLine &line: option.lines) {
        auto *item = new QListWidgetItem(ui->lines);
        item->setText(line.name);
        item->setData(Qt::UserRole, QVariant::fromValue(line));
    }

    m_option = option;
    emit optionChanged(option);
}

void PlotViewEditor::on_curves_itemDoubleClicked(QListWidgetItem *item)
{
    PlotCurve curve = item->data(Qt::UserRole).value<PlotCurve>();
    PlotCurveEdit dialog(this);
    dialog.setOption(curve);
    if (dialog.exec() == QDialog::Accepted) {
        curve = dialog.option();
        item->setText(curve.name);
        item->setData(Qt::UserRole, QVariant::fromValue(curve));
    }
}

void PlotViewEditor::on_addCurve_clicked()
{
    PlotCurveEdit dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        PlotCurve curve = dialog.option();
        auto *item = new QListWidgetItem(ui->curves);
        item->setText(curve.name);
        item->setData(Qt::UserRole, QVariant::fromValue(curve));
    }
}

void PlotViewEditor::on_removeCurve_clicked()
{
    delete ui->curves->currentItem();
}

void PlotViewEditor::on_unitNameH_textChanged(const QString &arg1)
{
    const QString suffix = " " + arg1;
    ui->left->setSuffix(suffix);
    ui->right->setSuffix(suffix);
    ui->deadZoneLeft->setSuffix(suffix);
    ui->deadZoneRight->setSuffix(suffix);
}

void PlotViewEditor::on_unitNameV_textChanged(const QString &arg1)
{
    const QString suffix = " " + arg1;
    ui->top->setSuffix(suffix);
    ui->bottom->setSuffix(suffix);
}

void PlotViewEditor::on_lines_itemDoubleClicked(QListWidgetItem *item)
{
    PlotLine line = item->data(Qt::UserRole).value<PlotLine>();
    PlotLineEdit dialog(this);
    dialog.setOption(line);
    if (dialog.exec() == QDialog::Accepted) {
        line = dialog.option();
        item->setText(line.name);
        item->setData(Qt::UserRole, QVariant::fromValue(line));
    }
}

void PlotViewEditor::on_addLine_clicked()
{
    PlotLineEdit dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        PlotLine line = dialog.option();
        auto *item = new QListWidgetItem(ui->lines);
        item->setText(line.name);
        item->setData(Qt::UserRole, QVariant::fromValue(line));
    }
}

void PlotViewEditor::on_removeLine_clicked()
{
    delete ui->lines->currentItem();
}
