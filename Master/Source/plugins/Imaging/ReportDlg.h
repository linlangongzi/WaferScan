#pragma once

#include <QDialog>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <Acquisition/ScanTaskCustomField.h>

namespace Ui {
class ReportDlg;
}

class DefectItemModel;
class QLabel;
class QComboBox;
class ScanPlan;

class ReportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDlg(QWidget *parent = 0);
    ~ReportDlg();

    static QString askFileName(const QMap<QString, QString> customData, const QString &filter);
    QMap<QString, QString> customData(ScanPlan *plan) const;

private slots:
    void OnCancel();
    void SaveSettings();
    void on_save_clicked();

private:
    void InitUI();
    void RefreshUI();
    int CheckData();

private:
    Ui::ReportDlg *ui;
    ScanTaskCustomFieldModel *customFieldModel;
    struct ReportRow {
        QLabel *name;
        QComboBox *valueList;
        bool isRequired = false;
        QString key;
    };
    QList<ReportRow> widgets;
};
