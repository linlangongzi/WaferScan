#ifndef PANELTCGEDIT_H
#define PANELTCGEDIT_H

#include <QWidget>
#include <QDialog>
#include <QDoubleValidator>
#include <QJsonValue>
#include <QMessageBox>
#include "PanelTCG.h"
#include <QJsonArray>
#include <QJsonObject>

namespace Ui {
class PanelTCGEdit;
}

class UTSetup;
class PanelTCG;

class PanelTCGEdit : public QDialog
{
    Q_OBJECT

public:
    explicit PanelTCGEdit(UTSetup *setup, int index = 0, bool bModify = false, QWidget *parent = 0);
    ~PanelTCGEdit();
public:

    double GetTCGTimeValue();
    double GetTCGGainValue();
private slots:
    void on_saveBtn_clicked();

    void on_cancelBtn_clicked();

private:
    QDoubleValidator *dTCGTimeValidator;
    QDoubleValidator *dTCGGainValidator;
    static const double MAX_TIME_VALUE;
    static const double MIN_TIME_VALUE;
    static const double MAX_GAIN_VALUE;
    static const double MIN_GAIN_VALUE;
    double timeValue;
    double gainValue;
    int modelIndex;
    Ui::PanelTCGEdit *ui;
};

#endif // PANELTCGEDIT_H
