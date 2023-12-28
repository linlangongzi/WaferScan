#pragma once

#include <QObject>
#include <QSettings>
#include <Ultrasonic/UltrasonicManager.h>
#include <utils/treemodel.h>
#include "AT4500.h"

class DataSourcePath
{
    Q_GADGET

public:
    DataSourcePath(const QString &path = QString());

    enum Source {
        None,
        Ultrasonic,
        ThermalCoupler,
    };
    Q_ENUM(Source)

    DECLARE_COMPARE_OPERATORS(DataSourcePath)

    QString toString() const;

    Source source = None;
    int channel = 0;
    int feature = 0;
};

class TemperatureSource : public QObject, public Utils::TypedTreeItem<TemperatureSource>
{
    Q_OBJECT

    Q_PROPERTY(DataSourcePath path MEMBER m_path)
    Q_PROPERTY(QByteArray func MEMBER m_func)

    enum Columns {
        ColPath,
        ColValue,
        ColFunc,
    };

public:
    TemperatureSource(QObject *parent) :
        QObject(parent)
    {}
    QVariant data(int column, int role) const Q_DECL_OVERRIDE;
    bool setData(int column, const QVariant &data, int role) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(int column) const Q_DECL_OVERRIDE;

    DataSourcePath path() const;

public slots:
    void setPath(const DataSourcePath &path);
    void setValue(qreal m_value);

private slots:
    void onUltrasonicData(const UTSharedDataFrame &frame);
    void onThermalData(const QList<qreal> &data);

private:
    DataSourcePath m_path;
    qreal m_value = Q_SNAN;
    QByteArray m_func;

    QObject *obj = Q_NULLPTR;

    friend class TemperatureManager;
};

struct lua_State;

class TemperatureManager : public QObject
{
    Q_OBJECT

public:
    explicit TemperatureManager(QObject *parent = 0);
    ~TemperatureManager();

    static TemperatureManager *instance();
    static Utils::UniformTreeModel<TemperatureSource> *model();
    TemperatureSource *addSource(const QString &path);
    void removeSource(const QModelIndex &index);
    qreal evaluate(qreal v, const QByteArray &func);
    QList<qreal> values() const;

public slots:
    void saveSettings(QSettings *setting) const;
    void loadSettings(QSettings *setting);

signals:
    void sourcesChanged();

private slots:
    void onChannelsChanged();

private:
    Utils::UniformTreeModel<TemperatureSource> *sources;
    AT4500 at;
    lua_State *L = NULL;

    friend class TemperatureSource;
};
