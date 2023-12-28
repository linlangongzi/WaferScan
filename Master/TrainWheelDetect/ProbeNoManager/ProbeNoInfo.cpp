#include "ProbeNoInfo.h"
#include <QMetaMethod>
#include <QDebug>
#include <allrising/Util.h>

ProbeNoInfo::ProbeNoInfo(QObject *parent, Track track) :
    QObject(parent),
    m_track(track),
    m_fileName("./ProbeNoInfo"),
    m_groupNo(10000)
{
    FromJson();
}

void ProbeNoInfo::FromJson()
{
    QJsonObject obj = ReadFileAsJson(m_fileName + ".json").toObject();

    const QMetaObject meta = ProbeNoInfo::staticMetaObject;
    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        QMetaProperty prop = meta.property(index);
        int value = ToRealNo(obj[prop.name()].toInt());
        prop.write(this, value);
        //qDebug() << prop.name() << ToRealNo(obj[prop.name()].toInt());
    }
    m_count = (m_axialMax - m_axialMin + 1)
            + (m_radialFlangeMax - m_radialFlangeMin + 1)
            + (m_radialPositiveMax - m_radialPositiveMin + 1)
            + (m_radialNegativeMax - m_radialNegativeMin + 1)
            + (m_sensorMax - m_sensorMin + 1);

    return;
}

/*!
 * \brief ProbeNoInfo::ToRealNo 计算实际编号
 * \param num 与左右轨无关编号
 * \return 探头实际编号
 */
int ProbeNoInfo::ToRealNo(int num)
{
    return (num + GetStartProbeNo());
}

/*!
 * \brief ProbeNoInfo::ToRealNo 获取起始编号
 * \return 起始编号
 */
int ProbeNoInfo::GetStartProbeNo() const
{
    return (m_track + 1) * m_groupNo;
}
