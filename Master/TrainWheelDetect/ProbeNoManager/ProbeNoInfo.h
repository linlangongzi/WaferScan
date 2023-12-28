#ifndef PROBENOINFO_H
#define PROBENOINFO_H

#include <QObject>

class ProbeNoInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int axialMax READ axialMax WRITE SetAxialMax)                                        /* 轮辋轮幅轴向最大编号 */
    Q_PROPERTY(int axialMin READ axialMin WRITE SetAxialMin)                                        /* 轮辋轮幅轴向最小编号 */
    Q_PROPERTY(int axialOutSideMax READ axialOutSideMax WRITE SetAxialOutSideMax)                   /* 轮辋轮幅轴向外侧最大编号 */
    Q_PROPERTY(int axialOutSideMin READ axialOutSideMin WRITE SetAxialOutSideMin)                   /* 轮辋轮幅轴向外侧最小编号 */
    Q_PROPERTY(int axialInSideMax READ axialInSideMax WRITE SetAxialInSideMax)                      /* 轮辋轮幅轴向内侧最大编号 */
    Q_PROPERTY(int axialInSideMin READ axialInSideMin WRITE SetAxialInSideMin)                      /* 轮辋轮幅轴向内侧最小编号 */

    Q_PROPERTY(int radialFlangeMax READ radialFlangeMax WRITE SetRadialFlangeMax)                   /* 轮缘径向最大编号 */
    Q_PROPERTY(int radialFlangeMin READ radialFlangeMin WRITE SetRadialFlangeMin)                   /* 轮缘径向最大编号 */

    Q_PROPERTY(int radialPositiveMax READ radialPositiveMax WRITE SetRadialPositiveMax)             /* 轮辋轮幅径向正向最大编号 */
    Q_PROPERTY(int radialPositiveMin READ radialPositiveMin WRITE SetRadialPositiveMin)             /* 轮辋轮幅径向正向最小编号 */
    Q_PROPERTY(int radialPositiveOutSideMax READ radialPositiveOutSideMax WRITE SetRadialPositiveOutSideMax)/* 轮辋轮幅径向正向外侧最大编号 */
    Q_PROPERTY(int radialPositiveOutSideMin READ radialPositiveOutSideMin WRITE SetRadialPositiveOutSideMin)/* 轮辋轮幅径向正向外侧最小编号 */
    Q_PROPERTY(int radialPositiveInSideMax READ radialPositiveInSideMax WRITE SetRadialPositiveInSideMax)/* 轮辋轮幅径向正向内侧最大编号 */
    Q_PROPERTY(int radialPositiveInSideMin READ radialPositiveInSideMin WRITE SetRadialPositiveInSideMin)/* 轮辋轮幅径向正向内侧最小编号 */

    Q_PROPERTY(int radialNegativeMax READ radialNegativeMax WRITE SetRadialNegativeMax)             /* 轮辋轮幅径向负向最大编号 */
    Q_PROPERTY(int radialNegativeMin READ radialNegativeMin WRITE SetRadialNegativeMin)             /* 轮辋轮幅径向负向最小编号 */
    Q_PROPERTY(int radialNegativeOutSideMax READ radialNegativeOutSideMax WRITE SetRadialNegativeOutSideMax)/* 轮辋轮幅径向负向外侧最大编号 */
    Q_PROPERTY(int radialNegativeOutSideMin READ radialNegativeOutSideMin WRITE SetRadialNegativeOutSideMin)/* 轮辋轮幅径向负向外侧最小编号 */
    Q_PROPERTY(int radialNegativeInSideMax READ radialNegativeInSideMax WRITE SetRadialNegativeInSideMax) /* 轮辋轮幅径向负向内侧最大编号 */
    Q_PROPERTY(int radialNegativeInSideMin READ radialNegativeInSideMin WRITE SetRadialNegativeInSideMin) /* 轮辋轮幅径向负向内侧最小编号 */

    Q_PROPERTY(int sensorMax READ sensorMax WRITE SetSensorMax)                                     /* 传感器最大编号 */
    Q_PROPERTY(int sensorMin READ sensorMin WRITE SetSensorMin)                                     /* 传感器最小编号 */

    //Q_PROPERTY(Track track READ GetTrack WRITE setTrack)
    //Q_PROPERTY(int count READ count)

public:
    enum Track
    {
        TrackLeft = 0,
        TrackRight
    };

    explicit ProbeNoInfo(QObject *parent = 0, Track track = TrackLeft);

    int axialMax() const
    {
        return m_axialMax;
    }

    int axialMin() const
    {
        return m_axialMin;
    }

    int axialOutSideMax() const
    {
        return m_axialOutSideMax;
    }

    int axialOutSideMin() const
    {
        return m_axialOutSideMin;
    }

    int axialInSideMax() const
    {
        return m_axialInSideMax;
    }

    int axialInSideMin() const
    {
        return m_axialInSideMin;
    }

    int radialFlangeMax() const
    {
        return m_radialFlangeMax;
    }

    int radialFlangeMin() const
    {
        return m_radialFlangeMin;
    }

    int radialPositiveMax() const
    {
        return m_radialPositiveMax;
    }

    int radialPositiveMin() const
    {
        return m_radialPositiveMin;
    }

    int radialPositiveOutSideMax() const
    {
        return m_radialPositiveOutSideMax;
    }

    int radialPositiveOutSideMin() const
    {
        return m_radialPositiveOutSideMin;
    }

    int radialPositiveInSideMax() const
    {
        return m_radialPositiveInSideMax;
    }

    int radialPositiveInSideMin() const
    {
        return m_radialPositiveInSideMin;
    }

    int radialNegativeMax() const
    {
        return m_radialNegativeMax;
    }

    int radialNegativeMin() const
    {
        return m_radialNegativeMin;
    }

    int radialNegativeOutSideMax() const
    {
        return m_radialNegativeOutSideMax;
    }

    int radialNegativeOutSideMin() const
    {
        return m_radialNegativeOutSideMin;
    }

    int radialNegativeInSideMax() const
    {
        return m_radialNegativeInSideMax;
    }

    int radialNegativeInSideMin() const
    {
        return m_radialNegativeInSideMin;
    }

    int sensorMax() const
    {
        return m_sensorMax;
    }

    int sensorMin() const
    {
        return m_sensorMin;
    }

    Track GetTrack() const
    {
        return m_track;
    }

    void setTrack(Track track)
    {
        m_track = track;
    }

    void FromJson();

    int count() const
    {
        return m_count;
    }

    int GetStartProbeNo() const;

public slots:
    void SetAxialMax(int axialMax)
    {
        m_axialMax = axialMax;
    }

    void SetAxialMin(int axialMin)
    {
        m_axialMin = axialMin;
    }

    void SetAxialOutSideMax(int axialOutSideMax)
    {
        m_axialOutSideMax = axialOutSideMax;
    }

    void SetAxialOutSideMin(int axialOutSideMin)
    {
        m_axialOutSideMin = axialOutSideMin;
    }

    void SetAxialInSideMax(int axialInSideMax)
    {
        m_axialInSideMax = axialInSideMax;
    }

    void SetAxialInSideMin(int axialInSideMin)
    {
        m_axialInSideMin = axialInSideMin;
    }

    void SetRadialFlangeMax(int radialFlangeMax)
    {
        m_radialFlangeMax = radialFlangeMax;
    }

    void SetRadialFlangeMin(int radialFlangeMin)
    {
        m_radialFlangeMin = radialFlangeMin;
    }

    void SetRadialPositiveMax(int radialPositiveMax)
    {
        m_radialPositiveMax = radialPositiveMax;
    }

    void SetRadialPositiveMin(int radialPositiveMin)
    {
        m_radialPositiveMin = radialPositiveMin;
    }

    void SetRadialPositiveOutSideMax(int radialPositiveOutSideMax)
    {
        m_radialPositiveOutSideMax = radialPositiveOutSideMax;
    }

    void SetRadialPositiveOutSideMin(int radialPositiveOutSideMin)
    {
        m_radialPositiveOutSideMin = radialPositiveOutSideMin;
    }

    void SetRadialPositiveInSideMax(int radialPositiveInSideMax)
    {
        m_radialPositiveInSideMax = radialPositiveInSideMax;
    }

    void SetRadialPositiveInSideMin(int radialPositiveInSideMin)
    {
        m_radialPositiveInSideMin = radialPositiveInSideMin;
    }

    void SetRadialNegativeMax(int radialNegativeMax)
    {
        m_radialNegativeMax = radialNegativeMax;
    }

    void SetRadialNegativeMin(int radialNegativeMin)
    {
        m_radialNegativeMin = radialNegativeMin;
    }

    void SetRadialNegativeOutSideMax(int radialNegativeOutSideMax)
    {
        m_radialNegativeOutSideMax = radialNegativeOutSideMax;
    }

    void SetRadialNegativeOutSideMin(int radialNegativeOutSideMin)
    {
        m_radialNegativeOutSideMin = radialNegativeOutSideMin;
    }

    void SetRadialNegativeInSideMax(int radialNegativeInSideMax)
    {
        m_radialNegativeInSideMax = radialNegativeInSideMax;
    }

    void SetRadialNegativeInSideMin(int radialNegativeInSideMin)
    {
        m_radialNegativeInSideMin = radialNegativeInSideMin;
    }

    void SetSensorMax(int sensorMax)
    {
        m_sensorMax = sensorMax;
    }

    void SetSensorMin(int sensorMin)
    {
        m_sensorMin = sensorMin;
    }

private:
    int ToRealNo(int num);

private:
    int m_axialMax;
    int m_axialMin;
    int m_axialOutSideMax;
    int m_axialOutSideMin;
    int m_axialInSideMax;
    int m_axialInSideMin;
    int m_radialFlangeMax;
    int m_radialFlangeMin;
    int m_radialPositiveMax;
    int m_radialPositiveMin;
    int m_radialPositiveOutSideMax;
    int m_radialPositiveOutSideMin;
    int m_radialPositiveInSideMax;
    int m_radialPositiveInSideMin;
    int m_radialNegativeMax;
    int m_radialNegativeMin;
    int m_radialNegativeOutSideMax;
    int m_radialNegativeOutSideMin;
    int m_radialNegativeInSideMax;
    int m_radialNegativeInSideMin;
    int m_sensorMax;
    int m_sensorMin;
    Track m_track;
    QString m_fileName;
    int m_groupNo;
    int m_count;
};

#endif // PROBENOINFO_H
