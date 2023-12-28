#pragma once

#include <QDialog>
#include <Imaging/ColorPalette.h>

namespace Ui {
class DialogColorPaletteList;
}

class DialogColorPaletteList : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(ColorPalette::DefaultGradient globalPalette READ globalPalette WRITE setGlobalPalette NOTIFY globalPaletteChanged)

public:
    explicit DialogColorPaletteList(QWidget *parent = 0);
    ~DialogColorPaletteList();

    ColorPalette::DefaultGradient globalPalette() const;
    ColorPalette colorPalette() const;

public slots:
    void setGlobalPalette(ColorPalette::DefaultGradient globalPalette);

signals:
    void globalPaletteChanged(ColorPalette::DefaultGradient globalPalette);

private:
    Ui::DialogColorPaletteList *ui;
    ColorPalette::DefaultGradient m_globalPalette;
};
