#pragma once

#include <QPointer>
#include <QWidget>
#include <Core/IOptionsPage.h>

namespace Ui {
class UltrasonicOptions;
}

class UltrasonicOptions : public QWidget
{
    Q_OBJECT

public:
    explicit UltrasonicOptions(QWidget *parent = 0);
    ~UltrasonicOptions();

private slots:
    void on_timebasePrecision_editingFinished();

private:
    Ui::UltrasonicOptions *ui;
};

class UltrasonicOptionsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    UltrasonicOptionsPage(QObject *parent = Q_NULLPTR);
    QWidget *widget() Q_DECL_OVERRIDE;
    void apply() Q_DECL_OVERRIDE;
    void finish() Q_DECL_OVERRIDE;

    QPointer<UltrasonicOptions> m_widget;
};
