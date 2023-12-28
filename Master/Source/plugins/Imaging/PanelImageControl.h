#pragma once

#include <QFrame>

class QDockWidget;
class QListWidgetItem;

namespace Ui {
class PanelImageControl;
}

class PanelImageControl : public QFrame
{
    Q_OBJECT

public:
    explicit PanelImageControl(QWidget *parent = 0);
    ~PanelImageControl();

public slots:
    void refresh();

private slots:
    void on_partList_itemClicked(QListWidgetItem *item);
    void on_actionImport_triggered();

private:
    Ui::PanelImageControl *ui;
//    QList<QDockWidget *> cscans;
};
