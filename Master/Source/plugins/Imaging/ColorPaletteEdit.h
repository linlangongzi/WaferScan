#pragma once

#include "ColorPalette.h"
#include <Waveform/GraphicsAxis.h>
#include <QButtonGroup>
#include <QDialog>

class ConfigDoubleSpinBox;
class ArrowRadioButton;

namespace Ui {
class ColorPaletteEdit;
}

/*! 调色板编辑器控件 */
class ColorPaletteEdit : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(ColorPalette colorPalette READ colorPalette WRITE setColorPalette NOTIFY colorPaletteChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)

public:
    explicit ColorPaletteEdit(QWidget *parent = 0);
    ~ColorPaletteEdit();

    UTImagePalette GetImagePalette() const;

    ColorPalette colorPalette() const;
    int currentIndex() const;
    void setAutoLimitSupported(bool enable);

public slots:
    void setColorPalette(const ColorPalette &colorPalette);
    void setCurrentIndex(int currentIndex);

signals:
    void colorPaletteChanged(ColorPalette colorPalette);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    ArrowRadioButton *currentArrow();
    void on_addStop_clicked();
    void on_removeStop_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_red_valueChanged(int value);
    void on_green_valueChanged(int value);
    void on_blue_valueChanged(int value);
    void on_alpha_valueChanged(int value);
    void on_position_valueChanged(double arg1);
    void on_max_valueChanged(double arg1);
    void on_min_valueChanged(double arg1);
    void on_interpolationLinear_toggled(bool checked);
    void on_interpolationStep_toggled(bool checked);
    void on_prev_clicked();
    void on_next_clicked();
    void on_emptyColor_colorChanged(QColor color);
    void on_color_colorChanged(QColor color);

    void RefreshArrows();
    void UpdateCurrentArrow();

    void on_actionImport_triggered();
    void on_actionExport_triggered();

    void on_actionAutoSpace_triggered();

    void on_autoMax_toggled(bool checked);
    void on_autoMin_toggled(bool checked);

private:
    void UpdateGradientFromArrow();

private:
    Ui::ColorPaletteEdit *ui;

    QList<ArrowRadioButton *> arrows;
    QButtonGroup arrowGroup;
    QPoint mouseDownPos;
    qreal mouseDownPosition = Q_SNAN;

};
