#pragma once

#include "CoreGlobal.h"
#include <QDateTime>
#include <QObject>
#include <QUuid>
#include <QVariant>
#include <allrising/Util.h>

class CORE_EXPORT DocumentInfo
{
    Q_GADGET

    Q_PROPERTY(QUuid uuid MEMBER uuid)                                          /*!< 文档ID */
    Q_PROPERTY(QString title MEMBER title)                                      /*!< 文档标题 */
    Q_PROPERTY(QString comment MEMBER comment)                                  /*!< 注释 */
    Q_PROPERTY(QString creationTool MEMBER creationTool)                        /*!< 首次创建使用的软件 */
    Q_PROPERTY(QString creationToolVersion MEMBER creationToolVersion)          /*!< 首次创建使用的软件版本 */
    Q_PROPERTY(QString creationPath MEMBER creationPath)                        /*!< 首次创建所在路径 */
    Q_PROPERTY(QString creationHost MEMBER creationHost)                        /*!< 首次创建所在主机名 */
    Q_PROPERTY(QString creationDomain MEMBER creationDomain)                    /*!< 首次创建主机所在域名 */
    Q_PROPERTY(QDateTime creationTime MEMBER creationTime)                      /*!< 首次创建时间 */
    Q_PROPERTY(QDateTime aquisitionStart MEMBER aquisitionStart)                /*!< 扫描/数据捕获开始时间 */
    Q_PROPERTY(QDateTime aquisitionEnd MEMBER aquisitionEnd)                    /*!< 扫描/数据捕获结束时间. null表示扫描未结束 */
    Q_PROPERTY(QString partType MEMBER partType)                                /*!< 工件类型 */
    Q_PROPERTY(QString partModel MEMBER partModel)                              /*!< 工件型号 */
    Q_PROPERTY(QString partIdentifier MEMBER partIdentifier)                    /*!< 工件识别码 */
    Q_PROPERTY(QVariant info MEMBER info)                                       /*!< 模块自定义信息 */

public:
    DECLARE_COMPARE_OPERATORS(DocumentInfo)

    inline bool hasFinished() const;

    QUuid uuid;
    QString title;
    QString comment;
    QString creationTool;
    QString creationToolVersion;
    QString creationPath;
    QString creationHost;
    QString creationDomain;
    QDateTime creationTime;
    QDateTime aquisitionStart;
    QDateTime aquisitionEnd;
    QString partType;
    QString partModel;
    QString partIdentifier;
    QVariant info;
};

bool DocumentInfo::hasFinished() const
{
    return aquisitionEnd.isValid();
}
