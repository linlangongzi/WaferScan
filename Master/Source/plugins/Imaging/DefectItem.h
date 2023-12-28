#pragma once

#include "UTDefect.h"
#include <QItemSelectionModel>
#include <QMap>
#include <QModelIndex>
#include <QVariant>
#include <QCoreApplication>
#include <utils/treemodel.h>

class DefectItem : public Utils::TreeItem
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(DefectItem)

public:
    typedef QMap<int, QString> Headers;
    friend class DefectItemModel;

public:
    Qt::ItemFlags flags(int column) const override;
    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &value, int role) override;

public slots:
    void setDefect(const UTDefect &defect);
    const UTDefect &defect();

private:
    UTDefect m_defect;
    QVector2D origin;
};
