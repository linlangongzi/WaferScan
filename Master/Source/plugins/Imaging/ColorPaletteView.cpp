#include "ColorPaletteView.h"
#include <Waveform/PlotterRuler.h>
#include "ColorPaletteEdit.h"
#include <QMenu>
#include <QPainter>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "image.color");

enum {
    RulerWidth = 15
};

ColorPaletteView::ColorPaletteView(QWidget *parent) :
    QWidget(parent)
{
}

ColorPalette ColorPaletteView::colorPalette() const
{
    return m_palette;
}

Qt::Orientation ColorPaletteView::orientation() const
{
    return m_orientation;
}

bool ColorPaletteView::rulerEnable() const
{
    return m_rulerEnable;
}

bool ColorPaletteView::isReversed() const
{
    return m_reversed;
}

qreal ColorPaletteView::imageMin() const
{
    return  m_imageMin;
}

qreal ColorPaletteView::imageMax() const
{
    return  m_imageMax;
}

void ColorPaletteView::setColorPalette(ColorPalette palette)
{
    if (palette == m_palette) {
        return;
    }
    m_palette = palette;
    update();
    emit colorPaletteChanged(m_palette);
}

void ColorPaletteView::setOrientation(Qt::Orientation orientation)
{
    if (orientation == m_orientation) {
        return;
    }
    m_orientation = orientation;
    update();
}

void ColorPaletteView::setRulerEnable(bool rulerEnable)
{
    if (m_rulerEnable == rulerEnable) {
        return;
    }
    m_rulerEnable = rulerEnable;
    update();
}

void ColorPaletteView::editPalette()
{
    ColorPalette original = colorPalette();

    ColorPaletteEdit dialog(this);
    dialog.setAutoLimitSupported(m_autoLimitSupported);
    dialog.setColorPalette(original);
    connect(&dialog, &ColorPaletteEdit::colorPaletteChanged, this, &ColorPaletteView::setColorPalette);
    if (dialog.exec() != QDialog::Accepted) {
        setColorPalette(original);
    }
}

void ColorPaletteView::setReversed(bool reversed)
{
    if (m_reversed == reversed) {
        return;
    }
    m_reversed = reversed;
    update();
}

void ColorPaletteView::setImageRange(qreal min, qreal max)
{
    if (qFuzzyCompare(m_imageMax, max) && qFuzzyCompare(m_imageMin, min)) {
        return;
    }
    m_imageMin = min;
    m_imageMax = max;
    update();
}

void ColorPaletteView::setAutoLimitSupported(bool enable)
{
   m_autoLimitSupported = enable;
}

void ColorPaletteView::paintEvent(QPaintEvent *event)
{
//    qDebug() << "ColorPalette: " << "painting";
    QWidget::paintEvent(event);
    QPainter painter(this);

    QRect gradientRect = rect();
    QRect rulerRect = rect();
    Qt::Edge edge = Qt::LeftEdge;
    if (m_rulerEnable) {
        if (m_orientation == Qt::Horizontal) {
//            gradientRect.setHeight(rect().height() - RulerWidth);
//            rulerRect.setTop(gradientRect.bottom());
            rulerRect.setHeight(RulerWidth);
            gradientRect.setTop(RulerWidth);
            edge = Qt::BottomEdge;
        } else {
            gradientRect.setWidth(rect().width() - RulerWidth);
            rulerRect.setLeft(gradientRect.right());
        }
    }

    painter.fillRect(gradientRect, QPixmap(":/TransparentBackground.png"));

    painter.setPen(Qt::NoPen);
    m_palette.paint(&painter, gradientRect, m_orientation, m_reversed);

    if (m_rulerEnable) {
        PlotterRuler plotter;
        painter.translate(rulerRect.topLeft());
        QRect bounding = rulerRect.translated(-rulerRect.topLeft());
        bool isAutoMin = m_palette.isAutoMin() && qIsFinite(m_imageMin);
        bool isAutoMax = m_palette.isAutoMax() && qIsFinite(m_imageMax);
        qreal min = isAutoMin ? m_imageMin : m_palette.minValue();
        qreal max = isAutoMax ? m_imageMax : m_palette.maxValue();
        plotter.drawRuler(&painter, min, max, bounding, bounding, edge, m_reversed);
    }
}

void ColorPaletteView::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(tr("编辑调色板"), this, &ColorPaletteView::editPalette);
    if (m_autoLimitSupported) {
        QAction *actionAutoMax = menu.addAction(tr("自动上限"), this, &ColorPaletteView::setAutoMax);
        actionAutoMax->setCheckable(true);
        actionAutoMax->setChecked(m_palette.isAutoMax());

        QAction *actionAutoMin = menu.addAction(tr("自动下限"), this, &ColorPaletteView::setAutoMin);
        actionAutoMin->setCheckable(true);
        actionAutoMin->setChecked(m_palette.isAutoMin());
    }
    menu.addActions(actions());
    menu.exec(QCursor::pos());
}

void ColorPaletteView::setAutoMax(bool checked)
{
    ColorPalette palette = colorPalette();
    palette.setAutoMax(checked);
    setColorPalette(palette);
}

void ColorPaletteView::setAutoMin(bool checked)
{
    ColorPalette palette = colorPalette();
    palette.setAutoMin(checked);
    setColorPalette(palette);
}
