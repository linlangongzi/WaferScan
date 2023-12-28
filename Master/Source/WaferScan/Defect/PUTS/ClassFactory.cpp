#include "ClassFactory.h"

QMap<QString, CreateClass> ClassFactory::m_classMap;

ClassFactory::ClassFactory()
{

}

ClassFactory::~ClassFactory()
{

}

CreateClass ClassFactory::GetClassByName(QString className)
{
    /*if ( m_classMap.find(className) != m_classMap.end() )
    {
        if (!m_classMap[className])
        {
            return m_classMap[className];
        }
    }*/
    if ( GetMap().find(className) != GetMap().end() )
    {
        if (GetMap()[className])
        {
            return GetMap()[className];
        }
    }
    return NULL;
}

void ClassFactory::RegistClass(QString name, CreateClass method)
{
    //m_classMap.insert(name, method);
    GetMap().insert(name, method);
}

QMap<QString, CreateClass> &ClassFactory::GetMap()
{
    static QMap<QString, CreateClass> classMap;
    return classMap;
}


GenDynamic::GenDynamic(QString name, CreateClass method)
{
    ClassFactory::RegistClass(name, method);
}
