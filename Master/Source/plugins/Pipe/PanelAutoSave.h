#pragma once

#include <QWidget>

namespace Ui {
class PanelAutoSave;
}

class PanelAutoSave : public QWidget
{
    Q_OBJECT

public:
    explicit PanelAutoSave(QWidget *parent = 0);
    ~PanelAutoSave();
    quint32 GetTime() const;
    void Show();

signals:
    void TimerChanged(const quint32 &timer);

private slots:
    void Yes();
    void Cancel();

private:
    void Connect();

private:
    Ui::PanelAutoSave *ui;
    quint32 autoTime;
};
