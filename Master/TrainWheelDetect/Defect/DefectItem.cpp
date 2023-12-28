#include "DefectItem.h"
#include <arsCore/Platform.h>
#include <Wheel/ControlPanel.h>
#include <WheelDataset/WheelDataset.h>

DefectItem::~DefectItem()
{
    delete figure;
}

QVariant DefectItem::data(int column, int role) const
{
    const auto control = controlPanel();

    if (role == Qt::ToolTipRole) {
        return control->frameText(defect);
    }

    if (column == ColIndex && role == Qt::DisplayRole) {
        return index().row() + 1;
    } else if (column == ColWheel && role == Qt::DisplayRole) {
        return control->wheelName(defect);
    } else if (column == ColWheel && role == Qt::EditRole) {
        return ControlPanel::wheelId(defect);
    } else if (column == ColChannel && role == Qt::DisplayRole) {
        return ControlPanel::channelId(defect);
    } else if (column == ColAecCard && role == Qt::DisplayRole) {
        return control->aecCardId(defect);
    } else if (column == ColSensor && role == Qt::DisplayRole) {
        return control->sensorId(defect) % 1000;
    } else if (column == ColGroupName && role == Qt::DisplayRole) {
        return control->channelGroupName(defect);
    } else if (column == ColExtreme && role == Qt::DisplayRole) {
        return control->extremeText(defect);
    } else if (column == ColMerge && role == Qt::DisplayRole) {
        return control->mergeText(defect);
    } else if (column == ColJudgments && role == Qt::DisplayRole) {
        return control->judgmentText(defect);
    }

    return QVariant();
}

Qt::ItemFlags DefectItem::flags(int column) const
{
    Q_UNUSED(column);
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void DefectItem::setDefect(UTSharedDataFrame frame)
{
    defect = frame;
}

void DefectItem::setFigure(QGraphicsPathItem *item)
{
    figure = item;
}

QVariantMap DefectItem::toVariantMap()
{
    QVariantMap m;
    m["index"] = data(ColIndex, Qt::DisplayRole).toInt();
    m["wheelId"] = data(ColWheel, Qt::DisplayRole).toString();
    m["channelId"] = data(ColChannel, Qt::DisplayRole).toInt();
    m["aecCardId"] = data(ColAecCard, Qt::DisplayRole).toInt();
    m["sensorId"] = data(ColSensor, Qt::DisplayRole).toInt();
    m["channelGroup"] = data(ColGroupName, Qt::DisplayRole).toString();
    m["eigenvalue"] = data(ColExtreme, Qt::DisplayRole).toString();
    return m;
}
