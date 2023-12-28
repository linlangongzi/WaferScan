#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QObject>
#include <QVector>

class AbstractItem : public QObject
{
    Q_OBJECT

    friend class ItemModel;

public:
    typedef QVector<AbstractItem *> ItemVector;

protected:
    virtual AbstractItem *New(QObject* parent) const = 0;
    explicit AbstractItem(QObject *parent = 0);
    virtual void FromJson(const QJsonObject &source);
    virtual const QJsonObject ToJson();

public slots:

public:
    virtual int ColumnCount() = 0;
    virtual Qt::ItemFlags Flags(int column) = 0;
    virtual QVariant Data(int column, int role) = 0;
    virtual bool SetData(const QModelIndex &index, const QVariant &value, int role) = 0;
    virtual QVariant HeaderData(int section, Qt::Orientation orientation, int role) = 0;
    virtual AbstractItem *GetChild(int row);
    virtual int GetNumChildren();
    virtual ItemVector GetChildren();
    virtual void SetDepth(int newDepth);
    virtual int GetDepth() const;
    virtual void RemoveChild(AbstractItem *item);
    virtual void AttachChild(AbstractItem *item, int index = -1);
    virtual void DetachChild(AbstractItem *item);
    AbstractItem *GetParent() const;
    QVariant Get(const QString &key);
    QVariant Get(char *key);
    void Set(const QString &key, const QVariant &value);
    void Set(char *key, const QVariant &value);

private:
    ItemVector children;
    QJsonObject data;

};

#endif // ABSTRACTITEM_H
