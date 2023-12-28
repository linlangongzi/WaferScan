#pragma once

#include <QDialog>
#include <QPair>
#include <QVector>

namespace Ui {
class DialogChannelOffset;
}

class DialogChannelOffset : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChannelOffset(QWidget *parent = 0);
    ~DialogChannelOffset();

public:
    QPair<int, QVector<float> > channelOffset();
    void setChannelOffset(const QPair<int, QVector<float> > &offset);

private:
    void initUi();

private:
    Ui::DialogChannelOffset *ui;
};
