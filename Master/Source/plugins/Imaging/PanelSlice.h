#pragma once

#include <QFrame>

namespace Ui {
class PanelSlice;
}

class PanelSlice : public QFrame
{
    Q_OBJECT

public:
    explicit PanelSlice(QWidget *parent = 0);
    ~PanelSlice();

private:
    Ui::PanelSlice *ui;
};
