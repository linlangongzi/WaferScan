#pragma once

#include <QDialog>
#include <Ultrasonic/Probe.h>
#include <QDialogButtonBox>
#include <QSortFilterProxyModel>

class ProbeItem;
class Probe;
class QSortFilterProxyModel;

namespace Ui {
class DialogProbeList;
}

class DialogProbeList : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(bool isEditable READ isEditable WRITE setEditable NOTIFY editableChanged)

public:
    explicit DialogProbeList(QWidget *parent = nullptr);
    ~DialogProbeList() override;

public:
    Probe currentProbe() const;
    bool isEditable() const;

public slots:
    void setEditable(bool isEditable);

signals:
    void currentProbeChanged(Probe currentProbe);
    void editableChanged(bool isEditable);

private slots:
    void on_addProbe_clicked();
    void on_removeProbe_clicked();
    void onDoubleClicked(const QModelIndex &index);
    void on_filter_textChanged(const QString &arg1);
    void on_actionCopy_triggered();

private:
    ProbeItem *currentProbeItem() const;
    void createItem(Probe probe);

private:
    Ui::DialogProbeList *ui;
    QSortFilterProxyModel *m_sortModel = nullptr;
    bool m_isEditable = true;
};
