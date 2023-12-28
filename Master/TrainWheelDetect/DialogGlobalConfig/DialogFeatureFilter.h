#ifndef DIALOGFEATUREFILTER_H
#define DIALOGFEATUREFILTER_H

#include <QDialog>
#include <Wheel/ControlPanel.h>

namespace Ui {
class DialogFeatureFilter;
}

class DialogFeatureFilter : public QDialog
{
    Q_OBJECT

    typedef FeatureFilterConfig::FeatureType FeatureType;

public:
    explicit DialogFeatureFilter(QWidget *parent = 0);
    ~DialogFeatureFilter();

    void setConfig(FeatureFilterConfig config);
    FeatureFilterConfig config() const;

private slots:
    void on_feature_currentIndexChanged(int index);

private:
    Ui::DialogFeatureFilter *ui;
};

#endif // DIALOGFEATUREFILTER_H
