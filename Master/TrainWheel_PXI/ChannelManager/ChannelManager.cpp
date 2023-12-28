#include "ChannelManager.h"
#include <QDebug>
#include "ConfigObject/JsonFile.h"
#include "AbstractModel/ChannelItemModel.h"
ChannelManager::ChannelManager(QObject *parent) :
    QObject(parent),
    m_pModel(NULL)
{
    m_pModel = new ChannelItemModel(this);
    m_pJsonFile = new JsonFile(this);
}

void ChannelManager::OnSaveFile(const QString &strFileName)
{
    if(!SaveFile(strFileName))
    {
        qDebug() << "ChannelManager::OnSaveFile() save file failed";
    }

    return;
}

bool ChannelManager::ReadFile(const QString &strFileName)
{
    ClearData();
    m_pJsonFile->setFileName(strFileName);
    if (!m_pJsonFile->open())
    {
        qWarning() << "ChannelManager::OnReadFile() jsonFile Open Failed";
        return false;
    }

    m_pModel->FromJson(m_pJsonFile->Get("ChannelList"));

    emit ReadCompleted();

    return true;
}

void ChannelManager::OnReadFile(const QString &strFileName)
{
    ReadFile(strFileName);
    return;
}

bool ChannelManager::SaveFile(const QString &strFileName)
{
    QJsonArray array = m_pModel->ToJson();
    m_pJsonFile->setFileName(strFileName);
    m_pJsonFile->WriteJson(array);

    emit SaveCompleted();

    return true;
}

ChannelItemModel* ChannelManager::GetModel()
{
    return m_pModel;
}

void ChannelManager::ClearData()
{
    m_pModel->Reset();
    emit Clear();
    return;
}
