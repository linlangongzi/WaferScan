#ifndef DIALOGCHANNELGROUP_H
#define DIALOGCHANNELGROUP_H

#include <QDialog>
#include <Wheel/ControlPanel.h>

namespace Ui {
class DialogChannelGroup;
}

class DialogChannelGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChannelGroup(QWidget *parent = 0);
    ~DialogChannelGroup();
    void setConfig(const ChannelGroup &config);
    ChannelGroup config();

private:
    Ui::DialogChannelGroup *ui;
};

#endif // DIALOGCHANNELGROUP_H
