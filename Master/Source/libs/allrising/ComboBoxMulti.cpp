#include "ComboBoxMulti.h"
#include <QMenu>
#include <QBitArray>

ComboBoxMulti::ComboBoxMulti(QWidget *parent) :
    QPushButton(parent)
{
    setMenu(new QMenu(this));
    refreshText();
}

void ComboBoxMulti::addItem(const QString &text, const QVariant &data)
{
    QAction *action = menu()->addAction(text);
    action->setCheckable(true);
    action->setData(data);

    connect(action, &QAction::toggled, this, &ComboBoxMulti::refreshText);
}

void ComboBoxMulti::clear()
{
    menu()->clear();
    refreshText();
}

QVariantList ComboBoxMulti::selectedItemData() const
{
    QVariantList data;
    for (QAction *action: menu()->actions()) {
        if (action->isChecked()) {
            data << action->data();
        }
    }
    return data;
}

QBitArray ComboBoxMulti::selectedItemIndex() const
{
    const QList<QAction *> actions = menu()->actions();
    QBitArray data(actions.count());
    for (int index = 0; index < actions.count(); ++index) {
        data.setBit(actions[index]->isChecked());
    }
    return data;
}

void ComboBoxMulti::setSelectedItem(const QVariantList &data)
{
    for (QAction *action: menu()->actions()) {
        action->setChecked(data.contains(action->data()));
    }
}

void ComboBoxMulti::setSelecteditem(const QBitArray &data)
{
    deselectAll();
    QList<QAction *> actions = menu()->actions();
    const int count = qMin(actions.count(), data.count());
    for (int index = 0; index < count; ++index) {
        actions[index]->setChecked(data.testBit(index));
    }
}

void ComboBoxMulti::deselectAll()
{
    for (QAction *action: menu()->actions()) {
        action->setChecked(false);
    }
}

void ComboBoxMulti::selectItem(const QVariant &data, bool selected)
{
    for (QAction *action: menu()->actions()) {
        if (data == action->data()) {
            action->setChecked(selected);
        }
    }
}

QString ComboBoxMulti::emptyText() const
{
    return m_emptyText;
}

void ComboBoxMulti::setEmptyText(QString emptyText)
{
    m_emptyText = emptyText;
    refreshText();
}

void ComboBoxMulti::refreshText()
{
    QStringList texts;
    for (QAction *action: menu()->actions()) {
        if (action->isChecked()) {
            texts << action->text();
        }
    }
    if (texts.isEmpty()) {
        setText(m_emptyText);
    } else {
        setText(texts.join("\n"));
    }
}
