#ifndef PANELUTSETUPTIMEBASE_H
#define PANELUTSETUPTIMEBASE_H

#include "ConfigControl/ConfigControlInterface.h"
#include "PanelMaterial/PanelMaterial.h"
#include "PanelMaterial/PanelMaterialEdit.h"
#include <QWidget>
#include <QJsonValue>

namespace Ui {
class PanelUTSetupTimebase;
}

class UTSetup;
class Material;

class PanelUTSetupTimebase : public QWidget, public ConfigControlInterface
{
    Q_OBJECT
public:
    typedef ConfigObject::ConfigID ConfigID;

public:
    explicit PanelUTSetupTimebase(QWidget *parent = 0);
    ~PanelUTSetupTimebase();
    void ConnectTo(QObject *obj);
    void Disconnect(QObject *obj);

signals:
    void RequestSetParameter(QString key, QJsonValue value);
    void ParameterValueChanged(const QString &key, const QJsonValue &value);

private slots:
    void OnParameterChanged(const QString &key, const QJsonValue &value);

    void on_delayTime_valueChanged(double arg1);
    void on_delayDistance_valueChanged(double arg1);
    void on_rangeTime_valueChanged(double arg1);
    void on_rangeDistance_valueChanged(double arg1);

    void on_delayTime_editingFinished();
    void on_delayDistance_editingFinished();
    void on_rangeTime_editingFinished();
    void on_rangeDistance_editingFinished();

    void on_changeMaterial_clicked();

    void EmitNewDelayTime(double delay);
    void EmitNewDelayDistance(double delay);
    void EmitNewRangeTime(double range);
    void EmitNewRangeDistance(double range);

private:
    double GetMaterialSpeed();
    QString GetMaterialName();
    double ToDistance(double time);
    double ToTime(double distance);
    void UpdateMaterial();
    void InitUI();

private:
    Ui::PanelUTSetupTimebase *ui;
    typedef QPointer<UTSetup> UTSetupPointer;
    UTSetupPointer setup;
};

#endif // PANELUTSETUPTIMEBASE_H
