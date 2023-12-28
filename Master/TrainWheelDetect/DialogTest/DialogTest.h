#ifndef DIALOGTEST_H
#define DIALOGTEST_H

#include <QDialog>
#include "TimerTest.h"

namespace Ui {
class DialogTest;
}

class DialogTest : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTest(QWidget *parent = 0);
    ~DialogTest();

    TimerTest* GetTimer();

signals:
    void SelfTestConfirm(int);

private slots:
    void on_pushButton_clicked();
    void on_repeatFrequence_valueChanged(int arg1);
    void on_pushButtonSelfTest_clicked();
    void on_btnReadResult_clicked();

private:
    Ui::DialogTest *ui;
    TimerTest* m_pTimer;

};

#endif // DIALOGTEST_H
