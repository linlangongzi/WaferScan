#ifndef DIALOGTRAINCONFIG_H
#define DIALOGTRAINCONFIG_H

#include <QDialog>
#include <QListWidgetItem>
#include <Wheel/ControlPanel.h>

namespace Ui {
class DialogTrainConfig;
}

class DialogTrainConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTrainConfig(QWidget *parent = 0);
    ~DialogTrainConfig();
    void setConfig(const WheelSetLayout &config);
    WheelSetLayout config() const;

private slots:
    void on_add_clicked();
    void on_remove_clicked();
    void on_selectSetup_clicked();
    void on_wheelNames_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::DialogTrainConfig *ui;
};

#endif // DIALOGTRAINCONFIG_H
