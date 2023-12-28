#pragma once

#include <QComboBox>
#include "../allrising_global.h"

class ALLRISING_UTILS_EXPORT EnumComboBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(int currentValue READ currentValue WRITE setCurrentValue NOTIFY currentValueChanged)

public:
    EnumComboBox(QWidget *parent = nullptr);

    void setItems(const QMetaEnum &enums);
    int currentValue() const;

public slots:
    void setCurrentValue(int currentValue);

signals:
    void currentValueChanged(int currentValue);
};
