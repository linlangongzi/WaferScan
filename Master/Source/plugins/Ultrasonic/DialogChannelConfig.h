#pragma once

#include "ChannelConfig.h"

#include <QDialog>

namespace Ui {
class DialogChannelConfig;
}

class DialogChannelConfig : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(ChannelConfig config READ config WRITE setConfig)

public:
    explicit DialogChannelConfig(QWidget *parent = 0);
    ~DialogChannelConfig();

    ChannelConfig config() const;

public slots:
    void setConfig(ChannelConfig config);

private:
    Ui::DialogChannelConfig *ui;
};
