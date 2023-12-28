#ifndef PANELCHANNELMGR_H
#define PANELCHANNELMGR_H

#include <QWidget>
#include "ChannelTreeView.h"
#include "AbstractModel/ChannelFilterProxyModel.h"

namespace Ui {
class PanelChannelMgr;
}

class PanelChannelMgr : public QWidget
{
    Q_OBJECT

public:
    typedef ChannelTreeView::WaveType WaveType;

public:
    explicit PanelChannelMgr(QWidget *parent = 0);
    ~PanelChannelMgr();

public slots:

signals:
    void SendChannelNo(int nChannoNo, WaveType type);

private slots:
    void applyFilter(const QString &text);
    void ExpandTreeView();


private:
    Ui::PanelChannelMgr *ui;
    void Init();
    void ConnectTo();


private:

};

#endif // PANELCHANNELMGR_H
