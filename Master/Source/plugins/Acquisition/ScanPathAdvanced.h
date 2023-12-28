#pragma once

#include "ScanPathFactory.h"
#include <allrising/Range.h>

class ScanPathAdvancedOptions
{
    Q_GADGET

    Q_PROPERTY(QString motionType MEMBER motionType)
    Q_PROPERTY(MotionPath motionPath MEMBER motionPath)
    Q_PROPERTY(QList<ImageOptions> imageOptions MEMBER imageOptions)
    //    Q_PROPERTY(QList<RecordConfig> records MEMBER records)
    Q_PROPERTY(bool setupEnable MEMBER setupEnable)
    Q_PROPERTY(QHash<int, UTSetup> m_setups MEMBER m_setups)
//    Q_PROPERTY(SafetyBarrierOptions safetyBarrier MEMBER safetyBarrier)

public:
    DECLARE_COMPARE_OPERATORS(ScanPathAdvancedOptions)

    QPair<qreal, qreal> boundingRange(int axis) const;
    QRectF boundingRect(int axisX, int axisY) const;
    /*! 获取扫描计划所对应的userTransform的scale */
    QSizeF unitScale(int axisIndexX, int axisIndexY) const;
    /*! 获取该扫描计划的建议网格尺寸 */
    QSizeF adviceGridSize(int axisX, int axisY) const;
    /*! 获取通道偏移 */
    QVector<QVector<float> > channelOffset(int imageIndex = 0) const;
    MotionAxisList channelsSelected(int index) const;
    /*! 获取推荐的返程补偿 */
    QPointF adviceBacklash(int axisIndexX, int axisIndexY) const;
    ImageOptions adviceImageOptions(int index) const;

    QString motionType;
    MotionPath motionPath;
    QList<ImageOptions> imageOptions;
//    QList<RecordConfig> records;
    bool setupEnable = false;
    QHash<int, UTSetup> m_setups;
//    SafetyBarrierOptions safetyBarrier;
};

class ScanPathAdvanced : public ScanPathFactory
{
    Q_OBJECT

public:
    ScanPathAdvanced(QObject *parent = nullptr);

    QString name() const override;
    QString displayName() const override;
    QIcon icon() const override;
    MotionPath motionPath(const QVariant &options) const override;
    QHash<int, UTSetup> setups(const QVariant &options) const override;
    QList<ImageOptions> imageOptions(const QVariant &options) const override;
    QPair<qreal, qreal> boundingRange(const QVariant &options, int axis) const override;
//    QList<RecordConfig> recordOptions(const QVariant &options) const override;
//    SafetyBarrierOptions safetyBarrier(const QVariant &options) const override;
    CheckResult checkOptions(const QVariant &options) const override;
    QWidget *widget(const QVariant &options) const override;
    QVariant widgetOptions(QWidget *widget) const override;
    void setWidgetEnabled(QWidget *widget, bool enable) const override;
};
