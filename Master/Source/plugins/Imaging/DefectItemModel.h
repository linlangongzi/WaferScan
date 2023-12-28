#pragma once

#include <utils/treemodel.h>
#include <QDebug>
#include <QItemSelectionModel>
#include "DefectItem.h"

using DefectItemModelRoot = Utils::TypedTreeItem<DefectItem>;
class DefectItemModel : public Utils::TreeModel<DefectItemModelRoot, DefectItem>
{
    Q_OBJECT

public:
    enum ColumnHeader
    {
        ColArea,
        ColLength,
        ColWidth,
        ColCharacteristicSize,
        ColPixel,
        ColCenter,
        ColMaxAmp,
        ColPercentage,
        ColAverageDepth,
        ColMaxDepth,
        ColMinDepth,
        ColBottomDepth,
        ColComment,
    };

    explicit DefectItemModel(QObject *parent = nullptr);

public:
    void sort(int column, Qt::SortOrder order) override;
    void fromDefectList(const UTDefectList &list);
    void appendDefect(UTDefect defect);
    void appendDefectList(const UTDefectList &list);
    QItemSelectionModel *selectionModel();
    void reloadItem(DefectItem *item);
    int pixelCount() const;
    void setOrigin(const QVector2D &origin);
    void loadJson(const QJsonValue &value);
    QJsonArray toJson() const;

private:
    QItemSelectionModel *m_selectionModel = nullptr;
};
