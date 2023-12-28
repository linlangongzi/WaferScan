#include "Material.h"
#include <QDebug>

Material::Material(QObject *parent) :
    Config(parent)
{
    SetRaw("Type", "Material");
}

ConfigObject *Material::New(QObject *parent) const
{
    Material *material = new Material(parent);
    return dynamic_cast<ConfigObject*>(material);
}

void Material::SetParameter(const QString &key, const QJsonValue &value)
{
    Config::Set(key, value);
}

void Material::RequestUpdate(const QString &key)
{
    emit RequestUpdateAnswer(key, Get(key));
}
