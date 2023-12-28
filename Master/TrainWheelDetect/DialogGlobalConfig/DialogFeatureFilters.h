#ifndef DIALOGFEATUREFILTERS_H
#define DIALOGFEATUREFILTERS_H

#include <QDialog>
#include <QListWidget>
#include <Wheel/ControlPanel.h>

namespace Ui {
class DialogFeatureFilters;
}

class DialogFeatureFilters : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFeatureFilters(QWidget *parent = 0);
    ~DialogFeatureFilters();

    void setConfig(QList<FeatureFilterConfig> filters);
    QList<FeatureFilterConfig> config() const;

private slots:
    void on_add_clicked();
    void on_remove_clicked();
    void on_filters_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::DialogFeatureFilters *ui;

};

#endif // DIALOGFEATUREFILTERS_H
