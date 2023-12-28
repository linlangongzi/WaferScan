#ifndef PANELINFORMATION_H
#define PANELINFORMATION_H

#include <QFrame>

namespace Ui {
class PanelInformation;
}

class PanelInformation : public QFrame
{
    Q_OBJECT

public:
    explicit PanelInformation(QWidget *parent = 0);
    ~PanelInformation();

private:
    Ui::PanelInformation *ui;
    int timerStartId;
    int timerEndId;

private:
    void timerEvent(QTimerEvent *e);

public slots:
    void OnScanStart();
    void OnScanEnd();
};

#endif // PANELINFORMATION_H
