#pragma once

#include <Imaging/UTImage.h>
#include <QJsonObject>
#include <QStringList>
/*!
 * \class DefectRecognizer
 * \brief 缺陷识别器的基类。
 */
class DefectRecognizer
{
public:
    virtual ~DefectRecognizer() {}

    /*! 获取判伤标准的名称 */
    virtual QString GetStandardName() const = 0;

    /*! 获取判伤标准中工件属性的列表 */
    virtual QStringList GetPartKeys() const {return QStringList();}
    /*! 获取判伤标准中工件属性的描述
     *  \param key 查询的属性名 */
    virtual QString GetPartKeyDescription(const QString &key) const {Q_UNUSED(key); return QString();}
    /*! 获取判伤标准中工件属性
     *  \param key 查询的属性名 */
    virtual CDataValue GetPartValue(const QString &key) const {Q_UNUSED(key); return 0;}

    /*! 获取缺陷对象的属性列表 */
    virtual QStringList GetObjectKeys() const {return QStringList();}
    /*! 获取缺陷对象的属性描述，如果属性名无效，返回空字符串
     *  \param key 查询的属性名*/
    virtual QString GetObjectKeyDescription(const QString &key) const {Q_UNUSED(key); return QString();}
    /*! 获取缺陷对象的数量 */
    virtual int GetNumObjects() const {return 0;}
    /*! 获取指定缺陷的边界，坐标的单位为世界坐标系的单位。如果索引无效，返回空多边形
     *  \param index 缺陷对象的索引*/
    virtual QPolygonF GetObjectBoundary(int index) const {Q_UNUSED(index); return QPolygonF();}
    /*! 获取缺陷对象的属性值，如果索引无效或属性名无效，返回0
     *  \param index 缺陷对象的索引
     *  \param key 缺陷属性名 */
    virtual CDataValue GetObjectValue(int index, const QString &key) const {Q_UNUSED(index); Q_UNUSED(key); return 0;}

    /*! 按照名称构造缺陷识别器，返回缺陷识别器的指针。
     *  建议使用 QScopedPointer 存放该指针，以确保识别器能被正常析构 */
//    static DefectRecognizer *Create(const QString &name, const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);

protected:
    /*! 受保护的构造体，外部应当使用 Create 来构造缺陷识别器。 */
    DefectRecognizer(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);

protected:
    const ImageOptions trans;                                                 /*!< 图像的变换矩阵 */
    const UTImageList images;                                                   /*!< 待识别的源图像 */
    const QJsonObject options;                                                  /*!< 选项表 */
};
