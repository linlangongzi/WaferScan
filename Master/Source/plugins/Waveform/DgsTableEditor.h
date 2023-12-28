#pragma once

#include "Ultrasonic/DgsTable.h"
#include <QDialog>
#include <Ultrasonic/DgsCurveModel.h>
#include <allrising/ConnectionHelper.h>
#include <QIdentityProxyModel>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

namespace Ui {
class DgsTableEditor;
}

class DgsTable;

class DgsTableEditor : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(DgsTable dgsTable READ dgsTable WRITE setDgsTable NOTIFY dgsTableChanged)

public:
    explicit DgsTableEditor(QWidget *parent = nullptr);
    ~DgsTableEditor() override;

    DgsTable dgsTable() const;
    void setDgsTable(const DgsTable &dgsTable);

signals:
    void dgsTableChanged(DgsTable dgsTable);

private slots:
    void on_recoverDate_clicked();
    void on_deleteDate_clicked();
    void on_actionAddCurveItem_triggered();
    void on_actionAddPointItem_triggered();
    void on_actionRemoveCurveItem_triggered();
    void on_actionRemovePointItem_triggered();
    void on_actionCleanAll_triggered();
    void updateChart();

private:
    DgsCurveItem *currentCurveItem() const;
    DgsCurvePointItem *currentCurvePointItem() const;

private:
    Ui::DgsTableEditor *ui;

    QtCharts::QLogValueAxis *m_depthAxis = nullptr;
    QtCharts::QValueAxis *m_gainAxis = nullptr;

    DgsTable m_dgsTable;
    ConnectionHelper<DgsTableEditor, DgsTable> m_handler;
    DgsCurveModel *m_model = nullptr;
    QIdentityProxyModel *m_sortModel = nullptr;
};
