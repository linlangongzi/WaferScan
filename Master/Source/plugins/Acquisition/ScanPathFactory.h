#pragma once

#include <QIcon>
#include <QObject>

#include <Motion/MotionPath.h>
//#include <Imaging/ImageRecord.h>
#include <Imaging/ImageOptions.h>
#include <Ultrasonic/UTSetup.h>

class ScanPathFactory : public QObject
{
    Q_OBJECT

public:
    struct CheckResult
    {
        bool isValid = true;
        QStringList errors;
        QStringList warnings;
        void addError(const QString &error)
        {
            isValid = false;
            errors << error;
        }
        void addWarning(const QString warning)
        {
            warnings << warning;
        }
    };

public:
    explicit ScanPathFactory(QObject *parent = nullptr);
    ~ScanPathFactory() override;

    virtual QString name() const;                                               /*!< 路径的唯一名称, 用于系统内部索引. 必须是英文. */
    virtual QString displayName() const;                                        /*!< 路径的显示名, 用于界面显示, 及扫描计划默认名称 */
    virtual QIcon icon() const;
    virtual MotionPath motionPath(const QVariant &options) const;
    virtual QHash<int, UTSetup> setups(const QVariant &options) const;
    virtual QList<ImageOptions> imageOptions(const QVariant &options) const;
    virtual QPair<qreal, qreal> boundingRange(const QVariant &options, int axis) const;
//    virtual QList<RecordConfig> recordOptions(const QVariant &options) const;
//    virtual SafetyBarrierOptions safetyBarrier(const QVariant &options) const;
    virtual CheckResult checkOptions(const QVariant &options) const;
    virtual QWidget *widget(const QVariant &options) const;
    virtual QVariant widgetOptions(QWidget *widget) const;
    virtual void setWidgetEnabled(QWidget *widget, bool enable) const;

    static QList<ScanPathFactory *> allFactories();
    static ScanPathFactory *fromName(const QString &name);
    static ScanPathFactory *advancedFactory();
};
