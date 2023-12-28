#include "EnumComboBox.h"
#include <QMetaEnum>

EnumComboBox::EnumComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        Q_UNUSED(index);
        emit currentValueChanged(currentData().toInt());
    });
}

void EnumComboBox::setItems(const QMetaEnum &enums)
{
    clear();
    for (int index = 0; index < enums.keyCount(); ++index) {
        addItem(enums.key(index), enums.value(index));
    }
}

int EnumComboBox::currentValue() const
{
    return currentData().toInt();
}

void EnumComboBox::setCurrentValue(int currentValue)
{
    setCurrentIndex(findData(currentValue));
}
