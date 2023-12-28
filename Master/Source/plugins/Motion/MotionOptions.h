#pragma once

#include "AxisModel.h"
#include <Core/IOptionsPage.h>
#include <QPointer>
#include <QWidget>
#include <utils/treemodel.h>

namespace Ui {
class MotionOptions;
}
class IOConfigItem;
namespace Motion{
class ControllerStatus;
}

class MotionOptions : public QWidget
{
    Q_OBJECT

public:
    explicit MotionOptions(QWidget *parent = 0);
    ~MotionOptions();

    void apply();
    void refreshEndPoint();
    void updateIoStatus();
    void updateControllerInfo();

private slots:
    void on_axisList_doubleClicked(const QModelIndex &index);
    void on_axisAdd_clicked();
    void on_axisRemove_clicked();
    void on_endPointList_doubleClicked(const QModelIndex &index);
    void on_endPointAdd_clicked();
    void on_endPointRemove_clicked();
    void on_ioAdd_clicked();
    void on_ioRemove_clicked();
    void on_simulateDynamic_toggled(bool checked);

private:
    void initUi(const Motion::ControllerStatus &status);

private:
    Ui::MotionOptions *ui;
    Utils::TreeModel<Utils::TreeItem, IOConfigItem> ioModel;
    AxisModel *axisModel = nullptr;
};

class MotionOptionsPage: public Core::IOptionsPage
{
    Q_OBJECT

public:
    MotionOptionsPage(QObject *parent);
    QWidget *widget() override;
    void apply() override;
    void finish() override;

    QPointer<MotionOptions> m_widget;
};
