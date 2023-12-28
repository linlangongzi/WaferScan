#pragma once

#include "MotionPose.h"
#include <utils/treemodel.h>

class NamedPosition : public Utils::TreeItem
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(MotionPose pos MEMBER pos)
    Q_PROPERTY(MotionAxisList axis MEMBER axis)
    Q_PROPERTY(bool isLoadingPosition MEMBER isLoadingPosition)

public:
    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    QString name;
    MotionPose pos;
    MotionAxisList axis;
    bool isLoadingPosition = false;
};

using NamedPositionRoot = Utils::TypedTreeItem<NamedPosition>;

class NamedPositionModel : public Utils::TreeModel<NamedPositionRoot, NamedPosition>
{
    Q_OBJECT

public:
    NamedPosition *findByName(const QString &name) const;
    NamedPosition *loadingPosition() const;
};
