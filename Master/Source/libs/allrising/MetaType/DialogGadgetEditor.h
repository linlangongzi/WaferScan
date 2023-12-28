#pragma once

#include <QComboBox>
#include <QDialog>
#include <QListWidget>
#include "../allrising_global.h"

namespace Ui {
class DialogGadgetEditor;
}

class ALLRISING_UTILS_EXPORT DialogGadgetEditor : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit DialogGadgetEditor(QWidget *parent = 0);
    ~DialogGadgetEditor();

    QVariant value() const;

    template <typename T>
    T value() const
    {
        return value().value<T>();
    }

    template <typename T>
    void setValue(const T &value)
    {
        setValue(QVariant::fromValue(value));
    }

public slots:
    void setValue(const QVariant &value);

signals:
    void valueChanged(QVariant value);

private slots:
    void controlChanged();

private:
    void refreshUi(int typeId);

    QWidgetList controls;
    Ui::DialogGadgetEditor *ui;
    QVariant m_value;
};
