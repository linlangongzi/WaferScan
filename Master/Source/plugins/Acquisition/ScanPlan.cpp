#include "ScanPlan.h"
#include "ScanPathAdvanced.h"
#include "ScanPathFactory.h"
#include "ScanPathRing.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Motion/TrioQueue.h>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QTimer>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTSetup.h>
#include <allrising/MetaType/MetaType.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>

using Motion::TrioQueue;

static Q_LOGGING_CATEGORY(debug, "acquire.plan");

static QSet<ScanPlan::MotionType> editableMotionType = {
    ScanPlan::RECTANGLE,
    ScanPlan::CYLINDER,
    ScanPlan::CYLINDERROTATION,
    ScanPlan::DISK,
    ScanPlan::DISKROTATION,
    ScanPlan::CAM,
    ScanPlan::ANNULUS
};

ScanPlan::ScanPlan(QObject *parent) :
    QObject(parent)
{
}

QString ScanPlan::name() const
{
    return m_name;
}

bool ScanPlan::isFolder() const
{
    return pathType() == "Folder" || (m_pathType.isEmpty() && motionType() == NONE);
}

bool ScanPlan::isEditable() const
{
    return !isFolder() && (editableMotionType.contains(motionType()) || ScanPathFactory::fromName(pathType()));
}

QString ScanPlan::pathType() const
{
    if (m_pathType.isEmpty()) {
        return ScanPathFactory::advancedFactory()->name();
    }
    return m_pathType;
}

QVariant ScanPlan::options() const
{
    return m_pathOptions;
}

QJsonObject ScanPlan::toJson() const
{
    QJsonObject json;
    QJsonArray childrenArray;
    if (m_children.size() != 0) {
        for (ScanPlan* child: m_children) {
            childrenArray << child->toJson();
        }
        json["Children"] = childrenArray;
    }

    json["ImageOptions"] = convertToJson(options().value<ScanPathAdvancedOptions>().imageOptions);
    json["MotionPath"] = motionPath().toJson();
    json["Name"] = m_name;
    json["MotionType"] = EnumToKey(motionType());
    json["pathType"] = m_pathType;
    json["optionsType"] = m_pathOptions.typeName();
    json["options"] = VariantToJson(m_pathOptions);

    return json;
}

ScanPlan *ScanPlan::child(int index)
{
    return m_children.value(index);
}

int ScanPlan::childrenCount() const
{
    return m_children.count();
}

QList<ScanPlan *> ScanPlan::children() const
{
    return m_children;
}

/*!
 * \brief 遍历所有子项，搜寻满足name=value的子节点。
 * \param key
 * \param value
 * \return 第一个子节点的指针。当找不到时返回NULL。
 */
ScanPlan *ScanPlan::findChild(const QString &value)
{
    if (name() == value) {
        return this;
    } else {
        for (ScanPlan *child: m_children) {
            if (child->name() == value) {
                return child;
            }
        }
        for (ScanPlan *child: m_children) {
            ScanPlan *grandChild = child->findChild(value);
            if (grandChild) {
                return grandChild;
            }
        }
    }
    return nullptr;
}

bool ScanPlan::isRoot() const
{
    return parent() == nullptr;
}

bool ScanPlan::hasValidPath() const
{
    if (isFolder()) {
        return false;
    }
    auto *factory = ScanPathFactory::fromName(pathType());
    if (factory) {
        return factory->checkOptions(options()).isValid;
    }
    return false;
}

QStringList ScanPlan::pathError() const
{
    auto *factory = ScanPathFactory::fromName(pathType());
    if (factory) {
        return factory->checkOptions(options()).errors;
    }
    return QStringList();
}

QStringList ScanPlan::pathWarning() const
{
    auto *factory = ScanPathFactory::fromName(pathType());
    if (factory) {
        return factory->checkOptions(options()).warnings;
    }
    return QStringList();
}

ScanPlan *ScanPlan::root()
{
    if (isRoot()) {
        return this;
    }
    return parent()->root();
}

int ScanPlan::depth() const
{
    ScanPlan *p = parent();
    if (p == nullptr) {
        return 0;
    }
    return p->depth() + 1;
}

/*! 获取第elementIndex之后的总时间 */
qreal ScanPlan::timeExpected() const
{
    qreal total = m_totalTime;
    for (ScanPlan *child: m_children) {
        total += child->timeExpected();
    }
    return total;
}

ScanPlan::MotionType ScanPlan::motionType() const
{
    return KeyToEnum(m_pathOptions.value<ScanPathAdvancedOptions>().motionType, NONE);
}

ScanPlan *ScanPlan::parent() const
{
    return qobject_cast<ScanPlan *>(QObject::parent());
}

QString ScanPlan::planPath() const
{
    if (parent() && parent()->parent()) {
        QString path = name();
        QString parentPath = parent()->planPath();
        if (!parentPath.isEmpty()) {
            path = parentPath + "/" + path;
        }
        return path;
    }
    return QString();
}

/*!
 * \brief 获取下一个子节点，用于从根节点开始，遍历整个树，使用深度优先搜索。
 * \param root 根节点
 * \param skipChildren 是否禁止当前节点的子节点。此参数为内部使用。
 * \return 下一个节点的指针。当不存在时返回NULL。
 */
ScanPlan *ScanPlan::next(ScanPlan *root, bool skipChildren)
{
    if ( (childrenCount() > 0) && !skipChildren ) {                             /* 如果自己有子节点，那么返回第一个子节点 */
        return child(0);
    }
    if (root == this) {                                                         /* 在没有子节点的情况下，如果root 就是本身，那么就不应该有next */
        return nullptr;
    }
    ScanPlan *p = parent();                                                     /* 获取父节点 */
    if ( (p == nullptr) || ( skipChildren && (this == root) ) ) {                  /* 当前节点为根节点，且已经结束子项遍历 */
        return nullptr;
    }
    int index = p->m_children.indexOf(this);                                    /* 获取自身的索引 */
    if (index < 0) {
        qCCritical(debug) << "Got invalid index for self:" << index;
        return nullptr;
    }
    if ( index + 1 >= p->childrenCount() ) {                                    /* 如果当前节点是最后一个子节点，返回父节点的下一个节点 */
        return p->next(root, true);                                             /* 跳过父节点的子项，因为现在已经是父节点的子项了，不跳过父节点的子项则会循环回来 */
    }
    return p->child(index + 1);                                                 /* 返回父节点的下一个子节点 */
}

QRectF ScanPlan::boundingRect(int axisX, int axisY)
{
    Range imageX = boundingRange(axisX);
    Range imageY = boundingRange(axisY);
    QRectF rect = Range::toRect(imageX, imageY);
    for (ScanPlan *child: m_children) {
        if (!child) {
            continue;
        }
        QRectF childRect = Range::toRect(child->boundingRange(axisX), child->boundingRange(axisX));
        if (childRect.isValid()) {
            if (rect.isValid()) {
                rect = rect.united(childRect);
                continue;
            }
            rect = childRect;
        }
    }
    return rect;
}

Range ScanPlan::boundingRange(int axisIndex)
{
    auto *factory = ScanPathFactory::fromName(pathType());
    if (factory) {
        auto range = factory->boundingRange(options(), axisIndex);
        if (range.first > range.second) {
            qSwap(range.first, range.second);
        }
        return Range(range.first, range.second);
    }
    return Range(Q_SNAN, Q_SNAN);
}

/*! 获得当前节点的（包括子节点）的第一个包含有scanpath的节点 */
ScanPlan *ScanPlan::nextValid(ScanPlan *root, bool allowSelf)
{
    if (allowSelf && hasValidPath()) {   /* 叶片测试：CAM允许无效路径 */
        return this;
    } else {
        if (root == nullptr) {
            root = this;
        }
        ScanPlan *nextPlan = this->next(root);
        while (nextPlan != nullptr && hasValidPath()) {
            nextPlan = nextPlan->next(this);
        }
        return nextPlan;
    }
}

ImageOptions ScanPlan::imageOptionsByIndex(int index)
{
    auto *factory = ScanPathFactory::fromName(pathType());
    if (factory) {
        return factory->imageOptions(options()).value(index);
    }
    return ImageOptions();
}

/*! 获取与该扫描计划相关的轴列表 */
MotionAxisList ScanPlan::associatedAxes()
{
    MotionAxisList axisList;
    const auto path = motionPath();
    for (const MotionElement &element: path.elements) {
        axisList.combine(element.axis);
    }
    for (const MotionElement &element: path.prerequisites) {
        if (element.type == MotionElement::Continuous) {
            axisList.combine(element.axis);
        }
    }
    for (ScanPlan *plan: children()) {
        axisList.combine(plan->associatedAxes());
    }
    return axisList;
}

void ScanPlan::fromJson(const QJsonObject &source)
{
    QJsonObject json = source;
    m_name = json["Name"].toString();
    for (const QJsonValue &value: json.value("Children").toArray()) {
        if (value.isObject()) {
            auto *child = new ScanPlan(this);
            child->fromJson(value.toObject());
            this->attachChild(child);
        } else {
            qCWarning(debug) << "Bad child type" << value.type();
        }
    }
    json.remove("Children");

    ScanPathAdvancedOptions adv;
    adv.imageOptions = convertFromJson<QList<ImageOptions> >(json.value("ImageOptions").toArray());
    // 向motionInfo的版本兼容
    if (json.contains("MotionInfo")) {
        QJsonObject motionInfo = json["MotionInfo"].toObject();

        adv.motionPath.interval = motionInfo.value("Interval").toDouble();
        adv.motionPath.multiChannelEnable = motionInfo["MultipleChannelSelect"].toBool();
        adv.motionPath.channelOffset = motionInfo["OffsetInterval"].toDouble();

        ChannelList channelList = convertFromJson<ChannelList>( motionInfo["ChannelSelect"] );
        QList<ChannelList> multiChannels;
        for (const QJsonValue &value: json["ImageOptions"].toArray()) {
            ChannelList channels = convertFromJson<ChannelList>(value.toObject().value("Channels"));
            ChannelList newChannels = Utils::filtered(channels, [&] (int channel) {return channelList.contains(channel);});
            multiChannels.append(newChannels);
        }
        adv.motionPath.multiChannels = multiChannels;

        QString motionType = json.value("MotionType").toString();
        MotionElement element;
        element.type = MotionElement::MoveLine;
        element.interPolicy = MotionElement::InterpolateBidiretional;
        element.mainAxis = motionInfo.value("ScanAxis").toInt();
        element.stepInterval = motionInfo["Step"].toDouble();
        element.speed = motionInfo["Speed"].toDouble();
        MotionPose startPos1;
        startPos1 << motionInfo.value("X1").toDouble() << motionInfo.value("Y1").toDouble() << motionInfo.value("Z1").toDouble();
        MotionPose endPos2;
        endPos2 << motionInfo.value("X2").toDouble() << motionInfo.value("Y2").toDouble() << motionInfo.value("Z2").toDouble();
        if (motionType == "RECTANGLE") {
            element.axis = MotionAxisList(QList<int>() << 0 << 1 << 2);
        } else if (motionType == "CYLINDER" || motionType == "DISK") {
            element.axis = MotionAxisList( QList<int>() << 0 << 1 << 2 << element.mainAxis);
        }
        int stepAxis = -1;
        QList<int> axisList = element.axis.toList();
        for (int index = 0; index < axisList.count(); ++index) {
            int axis = axisList.value(index);
            if (axis != element.mainAxis) {
                if (stepAxis < 0) {
                    stepAxis = axis;
                    continue;
                }
                if (startPos1.distanceOnAxis(endPos2, index) > startPos1.distanceOnAxis(endPos2, axisList.indexOf(stepAxis))) {
                    stepAxis = axis;
                }
            }
        }

        for (int axis: element.axis.toList()) {
            if (axis != element.mainAxis) {
                if (stepAxis < 0){
                    stepAxis = axis;
                    continue;
                }
                if (startPos1.distanceOnAxis(endPos2, axis) > startPos1.distanceOnAxis(endPos2, stepAxis)) {
                    stepAxis = axis;
                }
            }
        }
        element.stepAxis = stepAxis;
        Axis *axis = MotionManager::axis(stepAxis);
        element.stepSpeed = axis ? axis->speedLocate() : 20;
        MotionElement firstElement(element);
        MotionElement secondElement(element);
        MotionPose startPos2(startPos1);
        int index = element.axis.toList().indexOf( element.mainAxis );
        startPos2.ensureSize(index + 1);
        startPos2[index] = endPos2.value(index);
        MotionPose endPos1(endPos2);
        endPos1.ensureSize(index + 1);
        endPos1[index] = startPos1.value(index);
        firstElement.pos << startPos1 << startPos2;
        secondElement.pos << endPos1 << endPos2;
        if (motionType == "CYLINDER" || motionType == "DISK") {
            RotationInfo rotation;
            rotation.axis = motionInfo["ScanAxis"].toInt();
            rotation.type = motionInfo["LoadType"].toString() == "Bear" ? RotationInfo::SurfaceContact : RotationInfo::AxisContact;
            rotation.direction = motionInfo["Reversed"].toBool() ? RotationInfo::Reverse : RotationInfo::Forward;
            rotation.axisDiameter = motionInfo["AxisDiameter"].toString().toDouble();
            rotation.partDiameter = motionInfo["MaterialDiameter"].toArray().at(0).toDouble();
            rotation.speedType = motionType == "CYLINDER" ? RotationInfo::Uniform : RotationInfo::LimitAngularSpeed;
            rotation.maxAngularSpeed = motionInfo["AngularSpeedMax"].toDouble();
            adv.motionPath.rotations << rotation;

            MotionElement prerequisity;
            prerequisity.type = MotionElement::Continuous;
            prerequisity.mainAxis = motionInfo["ScanAxis"].toInt();
            element.speed = motionInfo["AngularSpeed"].toDouble();
            prerequisity.axis = MotionAxisList( QList<int>() << motionInfo["ScanAxis"].toInt() );
            adv.motionPath.prerequisites << prerequisity;

//            firstElement.axis = MotionAxisList( QList<int>() << 0 << 1 << 2 << rotation.axis);
//            secondElement.axis = MotionAxisList( QList<int>() << 0 << 1 << 2 << rotation.axis);
            firstElement.speed = motionInfo["AngularSpeed"].toDouble();
            secondElement.speed = motionInfo["AngularSpeed"].toDouble();
            for (const QJsonValue &value: motionInfo["Trigger"].toArray()) {
                MotionCondition condition;
                condition.type = motionInfo["RisingEdge"].toBool() ? MotionCondition::ConditionIoRising : MotionCondition::ConditionIoFalling;
                condition.repCount = motionInfo["TriggerCount"].toInt();
                condition.arg = value.toInt();
                condition.axis = rotation.axis;
                firstElement.conditions << condition;
                secondElement.conditions << condition;
            }
        }
        adv.motionPath.elements << firstElement << secondElement;

        json.remove("ImageOptions");
        json.remove("MotionInfo");
        json.insert("MotionPath", adv.motionPath.toJson());
    } else {
        adv.motionPath = MotionPath::fromJson(json.value("MotionPath").toObject());
    }
    adv.motionType = json.value("MotionType").toString("NONE");

    m_pathType = json["pathType"].toString();
    QString typeName = json["optionsType"].toString();
    int typeId = QMetaType::type(qPrintable(typeName));
    if (!m_pathType.isEmpty()) {
        if (typeId == QMetaType::UnknownType) {
            qCWarning(debug) << "Can't convert from json: unknown option type" << typeName;
        } else {
            m_pathOptions = JsonToVariant(json["options"], typeId);
        }
    } else {
        if (adv.motionType == "NONE") {
            m_pathType = "Folder";
            m_pathOptions = QVariant();
        } else {
            m_pathType = "Advanced";
            m_pathOptions = QVariant::fromValue(adv);
        }
    }
    m_totalTime = motionPath().expandMultiChannel().timeExpected();
}

void ScanPlan::removeChild(ScanPlan *scan)
{
    if (scan != nullptr) {
        int index = m_children.indexOf(scan);
        if (index < 0) {
            return;
        }
        for (auto child: scan->children()) {                                 /* 删除子节点 */
            scan->removeChild(child);
        }
        m_children.removeAt(index);
        delete scan;                                                            /* 从列表中移除之后，还需要将此节点析构 */
        scan = nullptr;
    }
}

void ScanPlan::attachChild(ScanPlan *scan, int index)
{
    if (scan == nullptr) {
        return;
    }
    scan->setParent(this);
    if ( ( index < childrenCount() ) && (index >= 0) ) {
        m_children.insert(index, scan);                                       // 附加节点时，需要指定附加位置
    } else if ( ( index >= childrenCount() ) || (index < 0) ) {
        m_children.append(scan);
    }
}

void ScanPlan::detachChild(ScanPlan *scan)
{
    if (scan != nullptr) {
        int index = m_children.indexOf(scan);
        m_children.removeAt(index);                                               // 分离节点只需要从列表中移除，不需要析构
    }
}

MotionPath ScanPlan::motionPath() const
{
    if (isFolder()) {
        return {};
    }
    auto *factory = ScanPathFactory::fromName(pathType());
    if (factory) {
        return factory->motionPath(options());
    }
    return {};
}

QHash<int, UTSetup> ScanPlan::setups() const
{
    auto *factory = ScanPathFactory::fromName(pathType());
    if (factory) {
        return factory->setups(options());
    }
    return {};
}

void ScanPlan::updateTotalTime()
{
    for (auto *child: m_children) {
        child->updateTotalTime();
    }
    m_totalTime = motionPath().expandMultiChannel().timeExpected();
}

void ScanPlan::setName(const QString &name)
{
    m_name = name;
}

void ScanPlan::setPathType(const QString &name)
{
    m_pathType = name;
}

void ScanPlan::setOptions(const QVariant &option)
{
    qCDebug(debug) << "setOptions" << option;
    m_pathOptions = option;
}
