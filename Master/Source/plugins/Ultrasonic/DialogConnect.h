#pragma once

#include <QDialog>

namespace Ui {
class DialogConnect;
}

class DialogConnect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnect(QWidget *parent = nullptr);
    ~DialogConnect();

private slots:
    void RefreshInstrument();

private:
    Ui::DialogConnect *ui;
};
