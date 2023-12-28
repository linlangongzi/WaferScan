#pragma once

#include <QFrame>

namespace Ui {
class PanelMotionConsole;
}

class PanelMotionConsole : public QFrame
{
    Q_OBJECT

public:
    explicit PanelMotionConsole(QWidget *parent = 0);
    ~PanelMotionConsole();

private:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void OnDataArrived(int channel, const QString &data);
    void on_clear_clicked();
    void on_sensorDebug_toggled(bool checked);

private:
    Ui::PanelMotionConsole *ui;
    QList<QAction *> filters;
};
