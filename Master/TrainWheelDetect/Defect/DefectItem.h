#ifndef DEFECTITEM_H
#define DEFECTITEM_H

#include <Ultrasonic/UTData.h>
#include <utils/treemodel.h>
#include <QGraphicsItem>

class DefectItem : public Utils::TypedTreeItem<DefectItem, DefectItem>
{
public:
    enum Columns {
        ColIndex,
        ColWheel,
        ColChannel,
        ColAecCard,
        ColSensor,
        ColGroupName,
        ColExtreme,
        ColMerge,
        ColJudgments,
    };

    ~DefectItem();

    QVariant data(int column, int role) const;
    Qt::ItemFlags flags(int column) const;

    void setDefect(UTSharedDataFrame frame);
    UTSharedDataFrame getDefect() const {return defect;}

    void setFigure(QGraphicsPathItem *item);
    QGraphicsPathItem *getFigure() const {return figure;}

    QVariantMap toVariantMap();

private:
    UTSharedDataFrame defect;
    QGraphicsPathItem *figure = NULL;                                           // 只有DefectItem有权限析构,其他地方不允许析构此图源
};

#endif // DEFECTITEM_H
