#pragma once

#include "EndPoint.h"
#include <QDialog>

class QListWidgetItem;
class QAbstractButton;

namespace Ui {
class DialogEndPointOptions;
}

class DialogEndPointOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEndPointOptions(QWidget *parent = nullptr);
    ~DialogEndPointOptions();

    void loadEndPoint(EndPoint *endPoint);

private slots:
    void on_add_clicked();
    void on_remove_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    void addAxis(const QUuid &uuid);

private:
    Ui::DialogEndPointOptions *ui;
    EndPoint *m_endPoint = nullptr;
};
