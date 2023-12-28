#pragma once

#include <QWidget>
#include "ColorPalette.h"

/*! 渐变条显示控件 */
class ColorPaletteView : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(ColorPalette colorPalette READ colorPalette WRITE setColorPalette NOTIFY colorPaletteChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(bool rulerEnable READ rulerEnable WRITE setRulerEnable)
    Q_PROPERTY(bool reversed READ isReversed WRITE setReversed)

public:
    ColorPaletteView(QWidget *parent);

    ColorPalette colorPalette() const;
    Qt::Orientation orientation() const;
    bool rulerEnable() const;
    bool isReversed() const;
    qreal imageMin() const;
    qreal imageMax() const;

public slots:
    void setColorPalette(ColorPalette colorPalette);
    void setOrientation(Qt::Orientation orientation);
    void setRulerEnable(bool rulerEnable);
    void editPalette();
    void setReversed(bool reversed);
    void setImageRange(qreal min, qreal max);
    void setAutoLimitSupported(bool enable);

signals:
    void colorPaletteChanged(ColorPalette colorPalette);

protected:
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *) override;

private slots:
    void setAutoMax(bool checked);
    void setAutoMin(bool checked);

private:
    ColorPalette m_palette;
    qreal m_imageMin = Q_SNAN;
    qreal m_imageMax = Q_SNAN;
    Qt::Orientation m_orientation = Qt::Vertical;
    bool m_rulerEnable = true;
    bool m_reversed = true;
    bool m_autoLimitSupported = false;
};
