#pragma once

#include <QDialog>
#include "Motion/RotationInfo.h"

namespace Ui {
class DialogRotationInfo;
}

class DialogRotationInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRotationInfo(QWidget *parent = 0);
    ~DialogRotationInfo();

public:
    void SetRotationInfo(const RotationInfo &rotation);
    RotationInfo GetRotationInfo() const;

private:
    void InitUI();

private:
    Ui::DialogRotationInfo *ui;
};
