#pragma once

#include <QPointer>
#include <QWidget>
#include <Core/IOptionsPage.h>

namespace Ui {
class ChannelOptions;
}
class ChannelModel;

class ChannelOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelOptions(QWidget *parent = 0);
    ~ChannelOptions();

private slots:
    void on_add_clicked();
    void on_edit_clicked();
    void on_remove_clicked();
    void on_addMultiple_clicked();
    void on_channels_doubleClicked(const QModelIndex &index);

private:
    Ui::ChannelOptions *ui;
    ChannelModel *model = Q_NULLPTR;
};

class ChannelOptionsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    ChannelOptionsPage(QObject *parent);

public:
    QWidget *widget() override;
    void apply() override {}
    void finish() override;

private:
    QPointer<ChannelOptions> m_widget;
};
