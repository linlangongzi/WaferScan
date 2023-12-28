#pragma once

#include "SafetyBarrier.h"

#include <QDialog>

namespace Ui {
class DialogSafetyBarrierOptions;
}

class DialogSafetyBarrierOptions : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(SafetyBarrierOptions options READ options WRITE setOptions)

public:
    explicit DialogSafetyBarrierOptions(QWidget *parent = 0);
    ~DialogSafetyBarrierOptions();

    SafetyBarrierOptions options() const;

public slots:
    void setOptions(const SafetyBarrierOptions &options);

private:
    Ui::DialogSafetyBarrierOptions *ui;
};
