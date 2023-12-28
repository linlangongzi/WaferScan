#pragma once

#include <QWidget>
#include <QPointer>
#include <Core/IOptionsPage.h>
#include <utils/treemodel.h>

namespace Ui {
class InstrumentOptions;
}

class InstrumentItem;
class InstrumentModel;

class InstrumentOptions : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentOptions(QWidget *parent = 0);
    ~InstrumentOptions();

private slots:
    void onLicenseUpdated();
    void on_treeView_activated(const QModelIndex &index);
    void on_add_clicked();
    void on_clone_clicked();
    void on_remove_clicked();

private:
    Ui::InstrumentOptions *ui;
    InstrumentModel *model = nullptr;
};

class InstrumentOptionsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    InstrumentOptionsPage(QObject *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    QPointer<InstrumentOptions> m_widget;
};
