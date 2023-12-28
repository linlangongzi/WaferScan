#pragma once

#include <QDialog>
#include <Plot/PlotConfig.h>

namespace Ui {
class PlotAlarmGroupEdit;
}
class QTreeWidgetItem;

class PlotAlarmGroupEdit : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(PlotAlarmGroup option READ option WRITE setOption)

public:
    explicit PlotAlarmGroupEdit(QWidget *parent = 0);
    ~PlotAlarmGroupEdit();

    PlotAlarmGroup option() const;

public slots:
    void setOption(const PlotAlarmGroup &option);

private slots:
    void on_indicators_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_add_clicked();

    int askChannel(int channelId, bool *ok);
    QString askName(QString name, bool *ok);
    static void refreshIndicatorItem(QTreeWidgetItem *item, const PlotAlarmIndicator &indicator);
    static PlotAlarmIndicator getIndicator(QTreeWidgetItem *item);

private:
    Ui::PlotAlarmGroupEdit *ui;
};
