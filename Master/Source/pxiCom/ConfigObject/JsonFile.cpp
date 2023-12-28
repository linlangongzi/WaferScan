#include "JsonFile.h"

JsonFile::JsonFile(QObject *parent) :
    Config(parent),
    fileName("")
{
}

/*!
 * \brief JsonFile::SetFileName 设置文件名称
 * \param name 文件名称
 */
void JsonFile::setFileName(const QString &name)
{
    fileName = name;
}

/*!
 * \brief JsonFile::open json文件的打开接口
 * \param mode 打开模式
 * \return true: 成功
 *         false: 失败
 */
bool JsonFile::open()
{
    QFile file;
    file.setFileName(fileName + ".json");
    if (file.open(QIODevice::ReadWrite))
    {
        if (file.size() <= 0)
        {
            file.close();
            return true;
        }
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error == QJsonParseError::NoError)
        {
            if (jsonDoc.isObject())
            {
                FromJson(jsonDoc.object());
            }
            file.close();
            return true;
        }
    }
    return false;
}

/*!
 * \brief JsonFile::close 关闭文件（关闭文件之后，本身的object应该被清空）
 */
void JsonFile::close()
{
    FromJson(QJsonObject());
}

/*!
 * \brief JsonFile::WriteJson 给json文件写入数据
 * \param doc 传入的jsonDocument
 */
/*
void JsonFile::WriteJson()
{
    QSaveFile file(this);
    file.setFileName(fileName + ".json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(QJsonDocument(ToJson()).toJson());
        file.commit();
        return;
    }
    qDebug() << "JsonFile WriteJson Failed";
}
*/
/*!
 * \brief JsonFile::WriteJson 给json文件写入数据
 * \param array DefectItemModel的defectList.ToJson()转成的QJsonArray
 */
void JsonFile::WriteJson(QJsonArray array)
{
    QSaveFile file(this);
    file.setFileName(fileName + ".json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        foreach(QJsonValue value, array)
        {
            file.write(QJsonDocument(value.toObject()).toJson());
        }
        file.commit();
        return;
    }
    qDebug() << "JsonFile WriteJson Failed";
}

/*!
 * \brief JsonFile::New 重载的config的构造函数
 * \param parent 父对象
 * \return
 */
ConfigObject *JsonFile::New(QObject *parent) const
{
    JsonFile *file = new JsonFile(parent);
    return dynamic_cast<ConfigObject *>(file);
}
