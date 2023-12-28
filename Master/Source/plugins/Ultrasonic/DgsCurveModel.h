#pragma once

#include "DgsTable.h"
#include <utils/treemodel.h>

/**
 * @brief The DgsCurve class
 * model:
 *  >m_root                                     //DgsCurveRootItem
 *      DgsCurve(area Φ2)                       //DgsCurveItem
 *          |-  Interpolate(6mm, -10dB)         //DgsCurvePointItem
 *          |-  Interpolate(10mm, -20dB)
 *          |-  Interpolate(20mm, -30dB)
 *      DgsCurve(area Φ3)
 *          |-  Interpolate(6mm, -10dB)
 *          |-  Interpolate(10mm, -20dB)
 *          |-  Interpolate(20mm, -30dB)
 */
/* Forward declaration */
class DgsCurveItem;

using DgsCurveRootItem = Utils::TypedTreeItem<DgsCurveItem>;

class DgsCurvePointItem : public Utils::TypedTreeItem<DgsCurvePointItem, DgsCurveItem>
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(DgsCurvePointItem)

public:
    enum Columns {
        ColDepth,
        ColGain,
    };
    Q_ENUM(Columns)

    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    qreal depth = 0;
    qreal gain = 0;
};

class DgsCurveItem : public Utils::TypedTreeItem<DgsCurvePointItem, DgsCurveRootItem>
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(DgsCurveItem)

    Q_PROPERTY(DgsCurve dgsCurve READ dgsCurve WRITE setDgsCurve)

public:
    enum Columns {
        ColDiameter,
        ColNull,
    };
    Q_ENUM(Columns)

    // TreeItem interface
    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    DgsCurve dgsCurve() const;
    void setDgsCurve(const DgsCurve &curve);

    void notifyModel();

private:
    qreal m_diameter = 0;
};

class DgsCurveModel : public Utils::TreeModel<DgsCurveRootItem, DgsCurveItem, DgsCurvePointItem>
{
    Q_OBJECT

    Q_PROPERTY(QList<DgsCurve> dgsCurves READ dgsCurves WRITE setDgsCurves NOTIFY dgsCurvesChanged)

public:
    explicit DgsCurveModel(QObject *parent = nullptr);
    ~DgsCurveModel() override = default;

    QList<DgsCurve> dgsCurves() const;
    void notifyDgsCurvesChanged();

public slots:
    void setDgsCurves(const QList<DgsCurve> &curves);

signals:
    void dgsCurvesChanged(const QList<DgsCurve> &curves);
};
