#pragma once

#include "UltrasonicGlobal.h"
#include "InterpolateTable.h"
#include "Material.h"
#include <QDate>
#include <QCoreApplication>
#include <allrising/MetaType/MetaType.h>
#include <utils/treemodel.h>
#include <QUuid>

class ULTRASONIC_EXPORT DgsCurve
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(DgsCurve)

public:
    DECLARE_COMPARE_OPERATORS(DgsCurve)

    Q_PROPERTY(InterpolateTable interpolateTable MEMBER interpolateTable)
    Q_PROPERTY(qreal diameter MEMBER diameter)

public:
    qreal diameter = 0;
    InterpolateTable interpolateTable;
};

class ULTRASONIC_EXPORT DgsTable
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(DgsTable)

public:
    DECLARE_COMPARE_OPERATORS(DgsTable)

    Q_PROPERTY(QUuid uuid MEMBER uuid)
    Q_PROPERTY(QList<DgsCurve> dgsCurves MEMBER dgsCurves)
    Q_PROPERTY(QUuid materialUuid MEMBER materialUuid)
    Q_PROPERTY(qreal waterPathLength MEMBER waterPathLength)
    Q_PROPERTY(QDate date MEMBER date)

    QString materialName() const;
    QString waterPathLengthText() const;
    QString depthRangeText() const;
    QString diameterRangeText() const;
    bool hasDgsCurve(qreal diameter) const;
    void updateDgsCurve(const DgsCurve &curve);

public:
    QUuid uuid;
    QList<DgsCurve> dgsCurves;
    QUuid materialUuid;
    qreal waterPathLength = 0;
    QDate date;
};

class DgsTableItem : public Utils::TreeItem
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(DgsTableItem)

public:
    enum Columns {
        ColMaterialName,
        ColWaterPathLength,
        ColDepthRange,
        ColDiameter
    };
    Q_ENUM(Columns)

    // TreeItem interface
public:
    QVariant data(int column, int role) const override;

    DgsTable table;
};

class DgsTableRootItem : public Utils::TypedTreeItem<DgsTableItem> {};

class DgsTableModel : public Utils::TreeModel<DgsTableRootItem, DgsTableItem>
{
    Q_OBJECT
    Q_PROPERTY(QList<DgsTable> dgsTables READ dgsTables WRITE setDgsTables NOTIFY dgsTablesChanged)
    Q_PROPERTY(QUuid defaultTableUuid READ defaultTableUuid WRITE setDefaultTableUuid NOTIFY defaultTableUuidChanged)

public:
    DgsTableModel(QObject *parent = nullptr);

    void setDgsTables(const QList<DgsTable> &tables);
    QList<DgsTable> dgsTables() const;
    QUuid defaultTableUuid() const;

    DgsTable dgsTable(const QUuid &uuid);

public slots:
    void setDefaultTableUuid(const QUuid &defaultTableUuid);

signals:
    void dgsTablesChanged(const QList<DgsTable> &tables);
    void defaultTableUuidChanged(const QUuid &defaultTableUuid);

private:
    QUuid m_defaultTableUuid;
};
