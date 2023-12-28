#pragma once
#include <QString>
#include <QMap>
#include <Imaging/UTImage.h>
#include <QStringList>

typedef void* (*CreateClass)(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);

/*!
 * \class ClassFactory
 * \brief 类工厂,通过一个MAP成员来实现类名与类的实例化对象函数的映射,向外部提供一个接口函数,通过类名来创建该类对象
 *        MAP是静态的,这样可以在程序运行前就可以存在
 */
class ClassFactory
{
public:
//  typedef void* (ClassFactory::*CreateClass)(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
public:
    ClassFactory();
    ~ClassFactory();
public:
    static CreateClass GetClassByName(QString className);

    static void RegistClass(QString name, CreateClass method);
    static QMap<QString, CreateClass> &GetMap();
private:
    static QMap<QString, CreateClass> m_classMap;
};


/*!
 * \class GenDynamic
 * \brief 动态创建类,动态创建的类通过包含该类的一个静态对象
 *        向类工厂里注册自己的创建对象函数
 */
class GenDynamic
{
public:
    GenDynamic(QString name, CreateClass method);
};


/*!< 定义宏,类通过包含该宏,实现动态创建 */
#define DECLARE_RUNTIME(class_name)\
    QString class_name##Name;\
    static GenDynamic* class_name##gd;


/*!< 宏实现,类通过实现该宏,实现动态创建 */
#define IMPLEMENT_RUNTIME(class_name)\
    GenDynamic* class_name::class_name##gd\
    = new GenDynamic(#class_name,class_name::CreateInstance);
