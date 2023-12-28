#pragma once

#include "allrising_global.h"
#include <QPushButton>

class ALLRISING_UTILS_EXPORT ComboBoxMulti : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(QString emptyText READ emptyText WRITE setEmptyText)

public:
    ComboBoxMulti(QWidget *parent = nullptr);

    void addItem(const QString &text, const QVariant &data);
    void clear();
    QVariantList selectedItemData() const;
    QBitArray selectedItemIndex() const;
    void setSelectedItem(const QVariantList &data);
    void setSelecteditem(const QBitArray &data);
    void deselectAll();
    void selectItem(const QVariant &data, bool selected = true);

    QString emptyText() const;

public slots:
    void setEmptyText(QString emptyText);

private slots:
    void refreshText();

private:
    QString m_emptyText;
};
