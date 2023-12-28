#pragma once

#include <QDialog>
#include "DefectItem.h"

namespace Ui {
class DialogDefectProperty;
}

class DialogDefectProperty : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDefectProperty(DefectItem *pItem, QWidget *parent = 0);
    ~DialogDefectProperty();
    DefectItem *GetItemData();
    void GenerateDefectValues();

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();
    void RemoveImage();

private:
    Ui::DialogDefectProperty *ui;
    void InitControls();
    void SetControlValues();
    void AddItem(const QString &name, const QString &displayName, const QString &value, const QString &unit, bool bEditable = false);

    void ResetMapData();

private:
    DefectItem *m_pDefectItem;
    int m_nRow;

    struct STVALUE
    {
        QString strValue;
        QString strUnit;
        bool bEditable;

        STVALUE(QString value, QString unit, bool edit)
        {
            strValue = value;
            strUnit = unit;
            bEditable = edit;
        }
    };

    typedef QMap<QString, STVALUE *> MapValues;
    MapValues m_mapValues;
};
