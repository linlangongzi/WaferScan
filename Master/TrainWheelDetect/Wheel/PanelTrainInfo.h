#ifndef PANELTRAININFO_H
#define PANELTRAININFO_H

#include <QFrame>

namespace Ui {
class PanelTrainInfo;
}

class PanelTrainInfo : public QFrame
{
    Q_OBJECT

public:
    explicit PanelTrainInfo(QWidget *parent = 0);
    ~PanelTrainInfo();

public slots:
    void refresh();
    void actionModify(bool checked);

private:
    Ui::PanelTrainInfo *ui;
};

#endif // PANELTRAININFO_H
