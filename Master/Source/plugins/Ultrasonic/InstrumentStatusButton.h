#pragma once

#include <QToolButton>

class InstrumentStatusButton : public QToolButton
{
    Q_OBJECT

public:
    InstrumentStatusButton(QWidget *parent = nullptr);

private slots:
    void refresh();
    void onClicked();
};
