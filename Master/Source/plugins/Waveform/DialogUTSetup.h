#pragma once

#include "WaveformGlobal.h"
#include <QDialog>
#include <Ultrasonic/UTSetup.h>

namespace Ui {
class DialogUTSetup;
}

class WAVEFORM_EXPORT DialogUTSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUTSetup(QWidget *parent = nullptr);
    ~DialogUTSetup();

    UTSetup setup() const;
    void setSetup(const UTSetup &setup);

private:
    Ui::DialogUTSetup *ui;
};

