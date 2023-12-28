#include "ColorPaletteEdit.h"
#include "ui_ColorPaletteEdit.h"
#include "ArrowRadioButton.h"
#include "DialogColorPaletteList.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>
#include <QVBoxLayout>
#include <allrising/Util.h>

ColorPaletteEdit::ColorPaletteEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorPaletteEdit)
{
    ui->setupUi(this);

    ui->gradient->setOrientation(Qt::Horizontal);
    ui->gradient->setReversed(false);
    ui->arrows->setContentsMargins(ui->arrowSpacerLeft->sizeHint().width(), 0, ui->arrowSpacerRight->sizeHint().width(), 0);
    auto *tools = new QMenu(this);
    tools->addAction(ui->actionExport);
    tools->addAction(ui->actionImport);
    tools->addAction(ui->actionAutoSpace);
    ui->tools->setMenu(tools);
    connect(&arrowGroup, SIGNAL(buttonToggled(QAbstractButton*,bool)), this, SLOT(UpdateCurrentArrow()));

    connect(new QShortcut(QString("Ctrl+C"), this), &QShortcut::activated, this, [=]() {
        writeClipboard(colorPalette());
    });
    connect(new QShortcut(QString("Ctrl+V"), this), &QShortcut::activated, this, [=]() {
        setColorPalette(readClipboard<ColorPalette>());
    });
}

ColorPaletteEdit::~ColorPaletteEdit()
{
    delete ui;
}

UTImagePalette ColorPaletteEdit::GetImagePalette() const
{
    return colorPalette().GetColorPalette();
}

ColorPalette ColorPaletteEdit::colorPalette() const
{
    return ui->gradient->colorPalette();
}

int ColorPaletteEdit::currentIndex() const
{
    auto *radio = static_cast<ArrowRadioButton *>(arrowGroup.checkedButton());
    return arrows.indexOf(radio);
}

void ColorPaletteEdit::setAutoLimitSupported(bool enable)
{
    ui->autoMax->setVisible(enable);
    ui->autoMin->setVisible(enable);
    ui->gradient->setAutoLimitSupported(enable);
}

void ColorPaletteEdit::setColorPalette(const ColorPalette &colorPalette)
{
    if (colorPalette.gradientType() == ColorPalette::LinearGradient) {
        ui->interpolationLinear->setChecked(true);
    } else {
        ui->interpolationStep->setChecked(true);
    }

    ui->autoMin->setChecked(colorPalette.isAutoMin());
    ui->autoMax->setChecked(colorPalette.isAutoMax());
    ui->min->setValue(colorPalette.minValue());
    ui->max->setValue(colorPalette.maxValue());
    ui->position->setRange(colorPalette.minValue(), colorPalette.maxValue());
    ui->gradient->setColorPalette(colorPalette);
    ui->emptyColor->setColor(colorPalette.emptyColor());

    RefreshArrows();

    emit colorPaletteChanged(colorPalette);
}

void ColorPaletteEdit::setCurrentIndex(int currentIndex)
{
    if (uint(currentIndex) < uint(arrows.count())) {
        arrows[currentIndex]->setChecked(true);
    }
}

bool ColorPaletteEdit::eventFilter(QObject *obj, QEvent *event)
{
    bool ret = QDialog::eventFilter(obj, event);

    auto *radio = qobject_cast<ArrowRadioButton *>(obj);
    if (!radio) {
        return false;
    }
    auto *e = static_cast<QMouseEvent *>(event);

    if (event->type() == QEvent::MouseButtonPress) {
        radio->raise();
        radio->setChecked(true);
        mouseDownPos = e->globalPos();
        mouseDownPosition = radio->position();
    } else if (event->type() == QEvent::MouseMove) {
        const QPoint offset = e->globalPos() - mouseDownPos;
        const qreal delta = qreal(offset.x()) / ui->gradient->width();
        auto *radio = static_cast<ArrowRadioButton *>(arrowGroup.checkedButton());
        if (qIsFinite(mouseDownPosition) && radio) {
            const qreal newPos = qBound<qreal>(0, mouseDownPosition + delta, 1);
            radio->setPosition(newPos);
            ColorPalette p = ui->gradient->colorPalette();
            UpdateGradientFromArrow();
            ui->position->setValue(p.mapToRange(newPos));
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mouseDownPos = QPoint();
        mouseDownPosition = Q_SNAN;
        RefreshArrows();
    }
    return ret;
}

void ColorPaletteEdit::RefreshArrows()
{
    const ColorPalette palette = ui->gradient->colorPalette();
    const QGradientStops &stops = palette.gradient().stops();

    while (arrows.count() < stops.count()) {                                    /* 增加不足的控件 */
        auto *radio = new ArrowRadioButton(ui->arrows);
        arrowGroup.addButton(radio);
        radio->installEventFilter(this);
        arrows << radio;
    }
    while (stops.count() < arrows.count()) {                                    /* 移除多余的控件 */
        ArrowRadioButton *radio = arrows.takeLast();
        arrowGroup.removeButton(radio);
        delete radio;
    }
    if (arrows.isEmpty()) {
        return;
    }

    for (int index = 0; index < stops.count(); ++index) {                       /* 刷新现有的控件 */
        ArrowRadioButton *radio = arrows[index];
        const auto &stop = stops[index];
        radio->setPosition(stop.first);
        radio->setColor(stop.second);
        radio->show();
    }

    UpdateCurrentArrow();
}

void ColorPaletteEdit::UpdateCurrentArrow()
{
    auto *radio = static_cast<ArrowRadioButton *>(arrowGroup.checkedButton());
    const int index = arrows.indexOf(radio);

    ui->editColor->setEnabled(radio);
    ui->removeStop->setEnabled(radio && arrows.count() > 2);
    if (!radio) {
        ui->prev->setEnabled(false);
        ui->next->setEnabled(false);
        return;
    }
    ui->position->setValue(ui->gradient->colorPalette().mapToRange(radio->position()));
    const QColor color = radio->color();
    ui->red->setValue(color.red());
    ui->blue->setValue(color.blue());
    ui->green->setValue(color.green());
    ui->alpha->setValue(color.alpha());

    ui->prev->setEnabled(index > 0);
    ui->next->setEnabled(index < arrows.count() - 1);
    ui->color->setColor(color);
}

void ColorPaletteEdit::UpdateGradientFromArrow()
{
    ColorPalette palette = ui->gradient->colorPalette();
    QLinearGradient gradient;
    for (auto *radio: arrows) {
        gradient.setColorAt(radio->position(), radio->color());
    }
    if (palette.gradient() != gradient) {
        palette.setGradient(gradient);
        ui->gradient->setColorPalette(palette);
        emit colorPaletteChanged(palette);
    }
}

ArrowRadioButton *ColorPaletteEdit::currentArrow()
{
    return static_cast<ArrowRadioButton *>(arrowGroup.checkedButton());
}

void ColorPaletteEdit::on_addStop_clicked()
{
    bool ok = false;
    ColorPalette p = ui->gradient->colorPalette();
    qreal pos = QInputDialog::getDouble(this, QString(), tr("新建颜色点位置"), 0, p.minValue(), p.maxValue(), 2, &ok);
    if (!ok) {
        return;
    }
    qreal normalPos = p.mapFromRange(pos);
    QColor color = p.colorAt(normalPos);
    QLinearGradient gradient;
    gradient.setStops(p.gradient().stops() << qMakePair(normalPos, color));
    p.setGradient(gradient);
    qDebug() << "Adding new stop" << color << "at" << normalPos << gradient.stops();
    setColorPalette(p);
}

void ColorPaletteEdit::on_removeStop_clicked()
{
    auto *radio = static_cast<ArrowRadioButton *>(arrowGroup.checkedButton());
    arrows.removeAll(radio);
    arrowGroup.removeButton(radio);
    delete radio;

    UpdateGradientFromArrow();
}

void ColorPaletteEdit::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults) {
        DialogColorPaletteList dialog(this);
        if (dialog.exec() != QDialog::Accepted) {
            return;
        }
        setColorPalette(dialog.colorPalette());
    }
}

void ColorPaletteEdit::on_red_valueChanged(int value)
{
    QColor c = currentArrow()->color();
    c.setRed(value);
    currentArrow()->setColor(c);
    UpdateGradientFromArrow();
}

void ColorPaletteEdit::on_green_valueChanged(int value)
{
    QColor c = currentArrow()->color();
    c.setGreen(value);
    currentArrow()->setColor(c);
    UpdateGradientFromArrow();
}

void ColorPaletteEdit::on_blue_valueChanged(int value)
{
    QColor c = currentArrow()->color();
    c.setBlue(value);
    currentArrow()->setColor(c);
    UpdateGradientFromArrow();
}

void ColorPaletteEdit::on_alpha_valueChanged(int value)
{
    QColor c = currentArrow()->color();
    c.setAlpha(value);
    currentArrow()->setColor(c);
    UpdateGradientFromArrow();
}

void ColorPaletteEdit::on_position_valueChanged(double arg1)
{
    auto *arrow = currentArrow();
    if (arrow) {
        arrow->setPosition(colorPalette().mapFromRange(arg1));
        UpdateGradientFromArrow();
    }
}

void ColorPaletteEdit::on_max_valueChanged(double arg1)
{
    ColorPalette palette = ui->gradient->colorPalette();
    palette.setMaxValue(arg1);
    setColorPalette(palette);
}

void ColorPaletteEdit::on_min_valueChanged(double arg1)
{
    ColorPalette palette = ui->gradient->colorPalette();
    palette.setMinValue(arg1);
    setColorPalette(palette);
}

void ColorPaletteEdit::on_interpolationLinear_toggled(bool checked)
{
    if (checked) {
        ColorPalette palette = ui->gradient->colorPalette();
        palette.setGradientType(ColorPalette::LinearGradient);
        setColorPalette(palette);
    }
}

void ColorPaletteEdit::on_interpolationStep_toggled(bool checked)
{
    if (checked) {
        ColorPalette palette = ui->gradient->colorPalette();
        palette.setGradientType(ColorPalette::StepGradient);
        setColorPalette(palette);
    }
}

void ColorPaletteEdit::on_prev_clicked()
{
    setCurrentIndex(currentIndex() - 1);
}

void ColorPaletteEdit::on_next_clicked()
{
    setCurrentIndex(currentIndex() + 1);
}

void ColorPaletteEdit::on_emptyColor_colorChanged(QColor color)
{
    ColorPalette palette = ui->gradient->colorPalette();
    palette.setEmptyColor(color);
    setColorPalette(palette);
}

void ColorPaletteEdit::on_color_colorChanged(QColor color)
{
    ArrowRadioButton *radio = currentArrow();
    if (radio) {
        radio->setColor(color);
        UpdateGradientFromArrow();
    }
}

void ColorPaletteEdit::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName( this, QString(), QString(), tr("调色板文件 (*.json)") );
    if (fileName.isEmpty()) {
        return;
    }
    QJsonValue v = ReadFileAsJson(fileName);
    setColorPalette(ColorPalette::fromJson(v));
}

void ColorPaletteEdit::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr("导出调色板"), QString(), tr("调色板文件 (*.json)") );
    if (fileName.isEmpty()) {
        return;
    }
    WriteFileAsJson(fileName, colorPalette().toJson());
}

void ColorPaletteEdit::on_actionAutoSpace_triggered()
{
    qreal min = arrows.first()->position();
    qreal max = arrows.last()->position();
    qreal step = (max - min) / (arrows.count() - 1);
    for (int index = 1; index < arrows.count() - 1; ++index) {
        arrows[index]->setPosition(min + step * index);
    }
    UpdateGradientFromArrow();
}

void ColorPaletteEdit::on_autoMax_toggled(bool checked)
{
    ui->max->setEnabled(!checked);
    ColorPalette palette = colorPalette();
    palette.setAutoMax(checked);
    setColorPalette(palette);
}

void ColorPaletteEdit::on_autoMin_toggled(bool checked)
{
    ui->min->setEnabled(!checked);
    ColorPalette palette = colorPalette();
    palette.setAutoMin(checked);
    setColorPalette(palette);
}
