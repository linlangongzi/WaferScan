#ifndef PANELUTGATE_H
#define PANELUTGATE_H

#include <QWidget>
#include <ConfigControl/ConfigDoubleSpinBox.h>
#include "ConfigObject/UTSetup.h"
#include <QStandardItemModel>
#include <QDoubleValidator>
#include <QMessageBox>
#include "ConfigObject/UTGate.h"
#include <QScrollBar>

namespace Ui {
class PanelUTGate;
}

class PanelUTGate : public QWidget
{
    Q_OBJECT
    typedef QList <QStandardItem*> StdItemList;
public:
    explicit PanelUTGate(UTSetup *setup, QWidget *parent = 0);
    ~PanelUTGate();

private slots:
    void ModifyGate(const QModelIndex &index);

    void on_gateTableView_pressed(const QModelIndex &index);

    void UpdateModel(const QString &key, const QJsonValue &value);

private:
    void InitUI();
    void InitModel();
    void ModifyGateValue(int row, int col, QString &value, QDoubleValidator *validator);
    void InsertGateListFromUTSetup(UTSetup *setup, int index);
    StdItemList GetGateStandardItemList(UTSetup *setup, int index);
    void InitGateControl(UTSetup *setup, int index);
    void UpdateGateControl(UTSetup *setup, int index);
    void UpdateSliderRange(const QString &key, const QJsonValue &value);

private:
    Ui::PanelUTGate *ui;
    UTSetup *m_pUTSetup;
    QStandardItemModel *gateItemModel;
    QDoubleValidator *dGateStartValidator;
    QDoubleValidator *dGateWidthValidator;
    QDoubleValidator *dGateAmpValidator;
    static const double MAX_GATE_START;

    static const double MIN_GATE_START;

    static const double MIN_GATE_WIDTH;

    static const double MAX_GATE_WIDTH;

    static const double MIN_GATE_AMP;

    static const double MAX_GATE_AMP;
    double  m_range;
    double  m_delay;
    int gateIndex;
};

#endif // PANELUTGATE_H
