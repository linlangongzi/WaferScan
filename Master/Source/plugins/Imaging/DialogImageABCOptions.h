#pragma once

#include <QDialog>

namespace Ui {
class DialogImageABCOptions;
}

class DialogImageABCOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImageABCOptions(QWidget *parent = 0);
    ~DialogImageABCOptions();

    int GetWidthImageA();
    bool isTimeStamp();
    bool isBlank();

private slots:
    void on_submit_clicked();
    void on_cancel_clicked();

private:
    Ui::DialogImageABCOptions *ui;
};
