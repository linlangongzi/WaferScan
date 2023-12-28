#pragma once

#include <QDialog>
#include <Ultrasonic/Probe.h>
#include <allrising/ConnectionHelper.h>
#include <QListWidgetItem>

class Probe;
class ProbeModel;
struct ProbeSize;

namespace Ui {
class ProbeEditor;
}

class ProbeEditor : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(Probe probe READ probe WRITE setProbe NOTIFY probeChanged)
    Q_PROPERTY(ProbeSize probeSize READ probeSize WRITE setProbeSize NOTIFY probeSizeChanged)

public:
    explicit ProbeEditor(QWidget *parent = nullptr);
    ~ProbeEditor() override;
    Probe probe() const;
    ProbeSize probeSize() const;

public slots:
    void accept() override;
    void setProbe(const Probe &probe);
    void setProbeSize(const ProbeSize &size);

signals:
    void probeChanged(Probe probe);
    void probeSizeChanged(const ProbeSize &probeSize);

private slots:
    void on_focalSi_valueChanged(double arg1);
    void on_crystalSizeSi_valueChanged(double arg1);
    void on_recoverDate_clicked();
    void on_deleteDate_clicked();
    void on_addDgs_clicked();
    void on_removeDgs_clicked();
    void on_dgsTreeView_doubleClicked(const QModelIndex &index);
    void on_setDefaultTable_clicked();

private:
    DgsTable currentTable() const;
    DgsTableItem *currentItem() const;

private:
    Ui::ProbeEditor *ui;
    Probe m_probe;
    ProbeSize m_probeSize;
    DgsTableModel *m_model = nullptr;
    ConnectionHelper<ProbeEditor, Probe> m_handler;
    ConnectionHelper<ProbeEditor, ProbeSize> m_handlerSize;
};
