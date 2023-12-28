#ifndef CHANNEL_H
#define CHANNEL_H

#include <QVector>
#include <QPolygonF>
#include <QVector2D>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QSharedPointer>

class Channel;

typedef QList<Channel> ChannelListBase;
class ChannelList : public ChannelListBase
{
public:
    QJsonArray ToJson() const;
    static ChannelList FromJson(const QJsonValue &value);
};

class Channel
{
public:
    Channel();
    Channel(const Channel &other);

private:
    ChannelList children;
    QString m_strName;

public:

    ChannelList GetChildren() const;
    QString GetName();

    QJsonObject ToJson() const;
    static Channel FromJson(const QJsonObject &obj);
};

typedef QSharedPointer<ChannelList> UTSharedChannelList;

#endif // CHANNEL_H
