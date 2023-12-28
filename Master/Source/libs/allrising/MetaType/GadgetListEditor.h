#pragma once

#include <QWidget>
#include <QVariantList>

class QListWidgetItem;

namespace Ui {
class GadgetListEditor;
}

class GadgetListEditor : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString displayProperty READ displayProperty WRITE setDisplayProperty)
    Q_PROPERTY(QVariantList values READ values WRITE setValues NOTIFY valuesChanged)

public:
    explicit GadgetListEditor(QWidget *parent = 0);
    ~GadgetListEditor();

    QVariantList values() const;
    QString displayProperty() const;

public slots:
    void setValues(const QVariantList &values);
    void setDisplayProperty(QString displayProperty);

signals:
    void valuesChanged(QVariantList values);

private slots:
    void on_add_clicked();
    void on_remove_clicked();

private:
    void refreshTitle(QListWidgetItem *item);

private:
    Ui::GadgetListEditor *ui;
    QString m_displayProperty;
};
