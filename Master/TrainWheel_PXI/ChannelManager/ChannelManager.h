#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include <QObject>
class ChannelItemModel;
class JsonFile;

class ChannelManager : public QObject
{
    Q_OBJECT
public:
    explicit ChannelManager(QObject *parent = 0);

private:
    ChannelItemModel* m_pModel;
    JsonFile* m_pJsonFile;

private:
    void ConnectTo();

public:
    ChannelItemModel* GetModel();

public:
    bool SaveFile(const QString &strFileName);
    bool ReadFile(const QString &strFileName);
    void ClearData();

signals:
    void ReadCompleted();
    void SaveCompleted();
    void Clear();

public slots:
    void OnSaveFile(const QString &strFileName);
    void OnReadFile(const QString &strFileName);
};

#endif // CHANNELMANAGER_H
