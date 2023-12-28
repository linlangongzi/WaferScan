#pragma once

#include <QDialog>
#include <QMenu>
#include <Imaging/ImageOptions.h>

namespace Ui {
class DialogImageOptions;
}
class QAbstractButton;

class DialogImageOptions : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(ImageOptions options READ GetOptions WRITE SetOptions)
    Q_PROPERTY(ImageOptions defaultOptions READ GetDefaultOptions WRITE SetDefaultOptions)

public:
    explicit DialogImageOptions(QWidget *parent = nullptr);
    ~DialogImageOptions() override;

    ImageOptions GetOptions() const {return options;}
    ImageOptions GetDefaultOptions() const;

public slots:
    void SetOptions(const ImageOptions &newOptions);
    void SetDefaultOptions(const ImageOptions &newDefaults);
    void resetFeatrueComboBox(int featureIndex);

private slots:
    void RefreshUi();
    void RefreshData(bool swap = false);
    void refreshDataWithoutSwap();
    void on_swap_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);
    void OnChannelOffsetSelectedChanged();
    void OnAddOffset();
    void OnEditOffset();
    void OnDeleteOffset();

private:
    Ui::DialogImageOptions *ui;
    ImageOptions options;
    ImageOptions defaultOptions;
    QMenu channelMenu;
    bool isRefreshing;
    int m_defaultChannel = 0;
};
