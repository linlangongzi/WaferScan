#include "DefectItem.h"
#include <QVector3D>
#include <Imaging/DefectItemModel.h>

const int precision = 2;

Qt::ItemFlags DefectItem::flags(int column) const
{
    Q_UNUSED(column);
    Qt::ItemFlags flags = (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return flags;
}

QVariant DefectItem::data(int column, int role) const
{
    if (column >= 0) {
        if (role == Qt::DisplayRole) {
            switch (column) {
            case DefectItemModel::ColComment:                return m_defect.comment;
            case DefectItemModel::ColArea:                   return QString::number(m_defect.GetArea(), 'f', precision);
            case DefectItemModel::ColLength:                 return QString::number(m_defect.GetLength(), 'f', precision);
            case DefectItemModel::ColWidth:                  return QString::number(m_defect.GetWidth(), 'f', precision);
            case DefectItemModel::ColCharacteristicSize:     return QString::number(m_defect.GetCharacteristicSize(), 'f', precision);
            case DefectItemModel::ColPixel:                  return QString::number(m_defect.pixelCount);
            case DefectItemModel::ColCenter: {
                QVector2D center = m_defect.GetCenter() - origin;
                return QString("(%1, %2)").arg(center.x(), 0, 'f', precision).arg(center.y(), 0, 'f', precision);
            }
            case DefectItemModel::ColMaxAmp:                 return QString::number(m_defect.GetMaxAmp(), 'f', precision);
            case DefectItemModel::ColPercentage:             return QString::number(m_defect.percentage * 100, 'f', precision);
            case DefectItemModel::ColAverageDepth:           return QString::number(m_defect.GetAverageDepth(), 'f', precision);
            case DefectItemModel::ColBottomDepth:            return QString::number(m_defect.GetBottomDepth(), 'f', precision);
            case DefectItemModel::ColMaxDepth:               return QString::number(m_defect.maxDepth, 'f', precision);
            case DefectItemModel::ColMinDepth:               return QString::number(m_defect.minDepth, 'f', precision);
            default:                        return QVariant();
            }                        return QVariant();
        }
        else if (role == Qt::EditRole) {
            switch (column) {
            case DefectItemModel::ColComment:                return m_defect.comment;
            case DefectItemModel::ColArea:                   return m_defect.GetArea();
            case DefectItemModel::ColLength:                 return m_defect.GetLength();
            case DefectItemModel::ColWidth:                  return m_defect.GetWidth();
            case DefectItemModel::ColCharacteristicSize:     return m_defect.GetCharacteristicSize();
            case DefectItemModel::ColPixel:                  return m_defect.pixelCount;
            case DefectItemModel::ColCenter: {
                return (m_defect.GetCenter() - origin).x();
            }
            case DefectItemModel::ColMaxAmp:                 return m_defect.GetMaxAmp();
            case DefectItemModel::ColPercentage:             return m_defect.percentage;
            case DefectItemModel::ColAverageDepth:           return m_defect.GetAverageDepth();
            case DefectItemModel::ColBottomDepth:            return m_defect.GetBottomDepth();
            case DefectItemModel::ColMaxDepth:               return m_defect.maxDepth;
            case DefectItemModel::ColMinDepth:               return m_defect.minDepth;
            default:                        return QVariant();
            }
        }
    }
    return QVariant();
}

/*!
 * \brief 修改data,暂时没有修改功能
 * \param index
 * \param value
 * \param role
 * \return 暂时返回false
 */
bool DefectItem::setData(int column, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (column == 0) {                                                // 修改名称
            m_defect.comment = value.toString();
            return true;
        }
    }
    return false;
}

void DefectItem::setDefect(const UTDefect &defect)
{
    m_defect = defect;
}

/*!
 * \brief DefectItem::GetDefect 获得utdefect的指针
 * \return utdefect的指针
 */
const UTDefect &DefectItem::defect()
{
    return m_defect;
}
