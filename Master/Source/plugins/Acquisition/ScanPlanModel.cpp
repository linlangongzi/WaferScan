#include "ScanPlanModel.h"
#include "ScanPathFactory.h"
#include "ScanPlan.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <Motion/MotionManager.h>
#include <QDebug>
#include <QItemSelectionModel>
#include <QLoggingCategory>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/GeneralDelegate.h>
#include <allrising/Util.h>

static Q_LOGGING_CATEGORY(debug, "acquire.plan.model");

static ScanPlanModel *m_instance = nullptr;

enum Columns {
    ColName = 0,
    ColTotal = 1,
};

ScanPlanModel::ScanPlanModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_selectionModel( new QItemSelectionModel(this, this) )
{
    Q_ASSERT(m_instance == nullptr);
    m_instance = this;

    m_root = new ScanPlan(this);

    icons["NONE"] =             QIcon(tr(":/fatcow/16x16/folder.png"));
    icons["LINE"] =             QIcon(tr(":/fatcow/16x16/draw_line.png"));
    icons["RECTANGLE"] =        QIcon(tr(":/fatcow/16x16/card_chip_silver.png"));
    icons["CYLINDER"] =         QIcon(tr(":/fatcow/16x16/database.png"));
    icons["CYLINDERROTATION"] = QIcon(tr(":/fatcow/16x16/database.png"));
    icons["DISK"] =             QIcon(tr(":/fatcow/16x16/cd.png"));
    icons["DISKROTATION"] =     QIcon(tr(":/fatcow/16x16/cd.png"));
    icons["LOCATE"] =           QIcon(tr(":/fatcow/16x16/bullet_pink.png"));
    icons["CAM"] =              QIcon(tr(":/fatcow/16x16/draw_spiral.png"));

    headers << tr("名称") << tr("总时间");

    connect(MotionManager::instance(), &MotionManager::axisListChanged, this, &ScanPlanModel::updateTotalTime);
    connect(MotionManager::instance(), &MotionManager::endPointsChanged, this, &ScanPlanModel::updateTotalTime);
    connect(UltrasonicManager::instance(), &UltrasonicManager::channelsChanged, this, &ScanPlanModel::updateTotalTime);
}

ScanPlanModel::~ScanPlanModel()
{
    m_instance = nullptr;
}

ScanPlanModel *ScanPlanModel::instance()
{
    return m_instance;
}

/*!
 * \brief   设置parent 对应的项的列数( 此函数为必须重载函数 )
 * \param   parent 父节点对应的index
 */
int ScanPlanModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return headers.count();
}

/*!
 * \brief   设置根据参数条件得到的表头
 * \param   section     列序号
 *          orientation 横竖标志
 *          role        角色（ 用来区分显示、图片、复选框等 ）
 */
QVariant ScanPlanModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( (orientation == Qt::Horizontal) && (role == Qt::DisplayRole)) {
        return headers.value(section);
    }
    return QVariant();
}

/*!
 * \brief   设置index 对应项在 role 角色上的数据 （ 为必须重载函数 ）
 * \param   index     项标志
 *          role      角色
 */
QVariant ScanPlanModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    ScanPlan *curPlan = scanPlanFromIndex(index);
    if (curPlan == nullptr) {
        return QVariant();
    }
    switch ( index.column() ) {
    case ColName:
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return curPlan->name();
        } else if (role == Qt::CheckStateRole && index.column() == 0 && index.parent().isValid()) {
//            return curPlan->Get("Checked").toBool() ? Qt::Checked : Qt::Unchecked;
        } else if (role == Qt::DecorationRole) {
            if (curPlan->isFolder()) {
                return QIcon(tr(":/fatcow/16x16/folder.png"));
            }
            auto *factory = ScanPathFactory::fromName(curPlan->pathType());
            if (factory && factory != ScanPathFactory::advancedFactory()) {
                return factory->icon();
            }
            QString mode = EnumToKey(curPlan->motionType());
            return icons.value(mode);
        }
        break;
    case ColTotal:
        if (role == Qt::DisplayRole) {
            return timeString( curPlan->timeExpected() );
        }
    default:
        return QVariant();
    }
    return QVariant();
}

/*!
 * \brief   parent 对应项的子项行数 (为必须重载函数)
 * \param   parent     项标志
 */
int ScanPlanModel::rowCount(const QModelIndex &parent) const
{
    if ( !parent.isValid() ) {
        return m_root->childrenCount();
    }
    ScanPlan *parentItem = scanPlanFromIndex(parent);
    if (parentItem != nullptr) {
        return parentItem->childrenCount();
    }
    return 0;
}

/*!
 * \brief   根据index 获得对应的扫描计划
 * \param   index     项标志
 */
ScanPlan *ScanPlanModel::scanPlanFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (index.model() != this) {
            qCWarning(debug) << "Model mismatch";
            return nullptr;
        }
        auto *curPlan = static_cast<ScanPlan *>(index.internalPointer());
        return curPlan;
    }
    return m_root;
}

/*!
 * \brief   添加一个新的扫描计划
 * \param   obj         该扫描计划对应的jsonobject
 *          motionType  该扫描计划的类型（line/rectangle）
 *          parent      新添加的扫描计划对应的父节点
 */
ScanPlan *ScanPlanModel::insertNewScanPlan(const QJsonObject &obj, const QModelIndex &parent, int index)
{
    ScanPlan *parentPlan = scanPlanFromIndex(parent);
    if (!parentPlan) {
        return nullptr;
    }
    auto *newItem = new ScanPlan(parentPlan);
    if (newItem == nullptr) {
        qCritical() << "ScanPlanModel::InsertNewScanPlan" << "NewItem Faild";
        return nullptr;
    }
    newItem->fromJson(obj);
    if (newItem->name().length() <= 0) {
        // 如果名称不存在 需要加载默认名称
        newItem->setName(newScanPlanName(KeyToEnum(obj["MotionType"].toString(), ScanPlan::NONE)));
    }
    beginInsertRows( parent, parentPlan->childrenCount(), parentPlan->childrenCount() );
    parentPlan->attachChild(newItem, index);
    endInsertRows();
    return newItem;
}

QString ScanPlanModel::newScanPlanName(ScanPlan::MotionType motionType)
{
    return newScanPlanName(defaultName(motionType));
}

/*!
 * \brief   获得默认的扫描计划名称
 * \param   motionType  该扫描计划的类型（line/rectangle）
 */
QString ScanPlanModel::newScanPlanName(const QString &displayName)
{
    QString name;
    int num = 0;
    do {
        num++;
        name = QStringLiteral("%1 %2").arg(displayName).arg(num);
    } while (m_root->findChild(name) != nullptr);
    return name;
}

/*!
 * \brief   更新扫描计划的jsonoject
 * \param   scanPlan  扫描计划
 *          obj       新的jsonobject
 */
void ScanPlanModel::reloadJson(ScanPlan *scanPlan, const QJsonObject &obj)
{
    QString oldName = scanPlan->name();
    scanPlan->fromJson(obj);
    QString newName = scanPlan->name();
    if (oldName == newName) {
        return;
    }
    QModelIndex index = indexOf(scanPlan);
    QModelIndex lefttop = createIndex(index.row(), 0, scanPlan);
    emit dataChanged(lefttop, lefttop);
}

/*! 获得最大的深度（ 在实际使用中，考虑到效率等因素，一般限制树形节点的最大深度 ） */
int ScanPlanModel::maxDepth() const
{
    return platform()->Get("ScanPlanMaxDepth").toInt(DEFAULT_MAX_DEPTH);
}

/*!
 * \brief   给model 从json 中加载节点
 * \param   array   传入的jsonArray
 */
void ScanPlanModel::fromJson(const QJsonArray &array)
{
    beginResetModel();
    foreach (ScanPlan *scan, m_root->children()) {
        m_root->removeChild(scan);
    }
    foreach (QJsonValue value, array) {
        if ( value.isObject() ) {
            auto *child = new ScanPlan(m_root);
            child->fromJson(value.toObject());
            m_root->attachChild(child);
        } else {
            qWarning() << "ScanPlanModel::FromJson value is not object";
        }
    }

    if (m_root->children().isEmpty()) {
        QJsonObject json;
        json["Name"] = tr("扫描计划");
        json["MotionType"] = "NONE";
        auto *folder = new ScanPlan(m_root);
        folder->fromJson(json);
        m_root->attachChild(folder);
    }
    endResetModel();
}

/*!
 * \brief ScanPlanModel::GetTimeByFlost  通过浮点数值，转换成时间的字符串
 * \param value
 * \return
 */
QString ScanPlanModel::timeString(float value)
{
    return QTime(0, 0, 0, 0).addSecs(value).toString();
}

QString ScanPlanModel::defaultName(ScanPlan::MotionType motionType)
{
    switch (motionType) {
    case ScanPlan::NONE:                return tr("计划组");
    case ScanPlan::RECTANGLE:           return tr("平板");
    case ScanPlan::CYLINDER:            return tr("管棒");
    case ScanPlan::CYLINDERROTATION:    return tr("管棒-旋转步进");
    case ScanPlan::DISK:                return tr("圆盘");
    case ScanPlan::DISKROTATION:        return tr("圆盘-旋转步进");
    case ScanPlan::ANNULUS:             return tr("圆环");
    case ScanPlan::CAM:                 return tr("路径");
    default:                            return tr("");
    }
}

void ScanPlanModel::updateTotalTime() const
{
    m_root->updateTotalTime();
}

/*!
 * \brief   将model 保存为jsonArray, 以便于保存文件
 */
QJsonArray ScanPlanModel::toJson() const
{
    if (m_root == nullptr) {
        qFatal("ScanPlanModel::ToJson: root is null");
        return QJsonArray();
    }
    QJsonValue value = m_root->toJson().value("Children");
    QJsonArray array;
    foreach (QJsonValue scanplan, value.toArray()) {
        QJsonObject obj = scanplan.toObject();
        if ( ( !obj.isEmpty() ) && ( !obj["ReadOnly"].toBool() ) ) {  // 过滤读文件产生的记录子节点
            array.append(scanplan);
        }
    }
    return array;
}

void ScanPlanModel::loadSettings()
{
    fromJson(ReadFileAsJson("./ScanPlan.json").toArray());
}

void ScanPlanModel::saveSettings() const
{
    WriteFileAsJson("./ScanPlan.json", toJson());
}

/*! 获得model 的根节点（ 根节点是不可见节点、其项标志必须为QModelIndex() ） */
ScanPlan *ScanPlanModel::root()
{
    return m_root;
}

/*!
 * \brief   通过ScanPlan 找到对应的项标志index
 * \param   scanPlan   需要查询的扫描计划
 */
QModelIndex ScanPlanModel::indexOf(ScanPlan *scanPlan, int column)
{
    if ( (scanPlan == nullptr) || (scanPlan == m_root) ) {
        return QModelIndex();
    }
    ScanPlan *parent = scanPlan->parent();
    if (parent == nullptr) {
        return QModelIndex();
    }
    int row = parent->children().indexOf(scanPlan);
    return createIndex(row, column, scanPlan);
}

QItemSelectionModel *ScanPlanModel::selectionModel()
{
    return m_selectionModel;
}

/*!
 * \brief   返回对应项的项标志index （ 为必须重载函数 ）
 * \param   row 行号（ 相对量，是在父节点中的行号 ）
 *          column 列号（ 相对量，是在父节点中的列号 ）
 */
QModelIndex ScanPlanModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) ) {
        return {};
    }
    if ( !parent.isValid() ) {//ROOT下挂载节点
        return createIndex( row, column, m_root->child(row) );
    }
    ScanPlan *item = scanPlanFromIndex(parent);
    if (item) {
        return createIndex( row, column, item->child(row) );
    }
    return {};
}

/*!
 * \brief   设置index 项对应role 觉得的数值
 * \param   index 设置项的项标志
 *          value 设置的数值
 *          role  设置的角色
 */
bool ScanPlanModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() ) {
        return false;
    }
    ScanPlan *curPlan = scanPlanFromIndex(index);
    if (curPlan != nullptr) {
//        if ( (role == Qt::CheckStateRole) && (index.column() == 0) && index.parent().isValid() ) {
//            if (value == Qt::Checked) {
//                curPlan->Set("Checked", true);
//                emit dataChanged(index, index);
//            } else if (value == Qt::Unchecked) {
//                curPlan->Set("Checked", false);
//                emit dataChanged(index, index);
//            }
//            for (int row = 0; row < curPlan->GetNumChildren(); row++) {          // 有子节点，需要将子节点同样改变选中状态
//                QModelIndex child = this->index(row, 0, index);
//                setData(child, value, Qt::CheckStateRole);
//            }
//        } else
        if ( (role == Qt::EditRole) && (index.column() == 0) && index.parent().isValid() ) { // 可以改变名称
            curPlan->setName(value.toString());
            emit dataChanged(index, index);
        }
    }
    return QAbstractItemModel::setData(index, value, role);
}

/*!
 * \brief   设置index 对应项的父节点的项标志 （必须重载函数）
 * \param   index 查询项的项标志
 */
QModelIndex ScanPlanModel::parent(const QModelIndex &index) const
{
    if ( !index.isValid() ) {
        return {};
    }
    ScanPlan *child = scanPlanFromIndex(index);
    ScanPlan *parent = child->parent();
    if ( (parent == nullptr) || (parent == m_root) ) {
        return {};
    }
    ScanPlan *grandParent = parent->parent();
    if (grandParent == nullptr) {
        return {};
    }
    int row = grandParent->children().indexOf(parent);
    return createIndex(row, 0, parent);
}

/*!
 * \brief   设置index 项的标志（标志决定是否可以编辑、是否可选、是否可以拖动、是否可以放置等等）
 * \param   index 项标志
 */
Qt::ItemFlags ScanPlanModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (!index.isValid()) {                                                     // 本身不是root
        return flag;
    }
    ScanPlan *scanPlan = scanPlanFromIndex(index);
    if (index.parent().isValid()) {                                             // 父节点不是root
        if (index.column() == ColName) {
            flag |= Qt::ItemIsEditable;
        }
        if ((scanPlan->depth() < maxDepth()) && (scanPlan->motionType() == ScanPlan::NONE)) { // 只有小于最大深度才允许放
            flag |= Qt::ItemIsDropEnabled;
        }
        return Qt::ItemIsDragEnabled | flag;
    } else {                                                                    // 基本节点允许放，不允许拖
        return Qt::ItemIsDropEnabled | flag;
    }
}

Qt::DropActions ScanPlanModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions ScanPlanModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

/*!
 * \brief   指示由row/column/parent 指向的项的data 是否支持action 对应的拖拽（一般的简单拖拽处理，可以直接在次函数中实现）
 * \param   data 对应的mimedata数据
 *          action 对应的dropAction (与前面的supportedDropActions 的返回值一致)
 *          row 项的行号
 *          column 项的列号
 *          parent 项的父节点标志
 */
bool ScanPlanModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    return QAbstractItemModel::dropMimeData(data, action, row, column, parent);
}

/*!
 * \brief   设置一行所有项的mimedata数据（dropMimeData 就是获取的这项数据，如果直接在这里实现拖拽功能，这数据就是拖拽处理的数据）
 * \param   indexes 某一行对用的所有列的index
 */
QMimeData *ScanPlanModel::mimeData(const QModelIndexList &indexes) const
{
    return QAbstractItemModel::mimeData(indexes);
}

/*!
 * \brief   设置数据的多媒体类型（用于在dropMimeData 和mimeData 中的数据进行类型判断）
 */
QStringList ScanPlanModel::mimeTypes() const
{
    return QAbstractItemModel::mimeTypes();
}

/*!
 * \brief   删除行（ removeRow 是通过对count 设置为1实现，这一点与其他类型中的实现方式相反，所以只要model涉及到删除项，必须重载）
 * \param   row 行号
 *          count 行数量
 *          parent 父节点标志
 */
bool ScanPlanModel::removeRows(int row, int count, const QModelIndex &parent)
{
    ScanPlan *parentItem = scanPlanFromIndex(parent);
    if (parentItem && (parentItem->childrenCount() >= (row + count))) {
        for (int i = 0; i < count; i++) {
            ScanPlan *child = parentItem->child(row + i);
            if (child != nullptr) {
                qDebug() << "removeRow child" << child << child->name();
                beginRemoveRows(parent, row, row + count - 1);                  // 实现model 的项删除时，必须以beginRemoveRows 开头，
                parentItem->removeChild(child);
                endRemoveRows();                                                // endRemoveRows 结尾，这样model 就会自动刷新数据在界面上,最主要是内部更新index
            }
        }
        return true;
    }
    return false;
}

/*!
 * \brief   移动行（ moveRow 是通过对count 设置为1实现，这一点与其他类型中的实现方式相反，所以只要model涉及到移动项，必须重载）
 * \param   sourceParent 移动项的父节点项标志
 *          sourceRow 移动项的行号
 *          count 行数
 *          destinationParent 目标节点的父节点项标志
 *          destinationChild 目标节点位置
 */
bool ScanPlanModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    ScanPlan *sourceParentItem = scanPlanFromIndex(sourceParent);
    ScanPlan *destinationParentItem = scanPlanFromIndex(destinationParent);
    if ( (sourceParentItem != nullptr) && (destinationParentItem != nullptr) && (destinationParentItem != m_root)
    && ( sourceParentItem->childrenCount() >= sourceRow + count) ) {
        for (int i = 0; i < count; ++i) {
            ScanPlan *child = sourceParentItem->child(sourceRow + i);
            if (!child) {
                continue;
            }
            if ( (sourceRow == destinationChild) && (sourceParentItem == destinationParentItem) ) {
                return false;
            }
            beginMoveRows(sourceParent, sourceRow + i, sourceRow + i, destinationParent, destinationChild);
            sourceParentItem->detachChild(child);
            if (sourceParentItem == destinationParentItem) {                // 同一个父节点下面的移动，位置要前移一个，因为chidlren 在detachchild 的时候被移除了一个
                if (sourceRow > destinationChild) {
                    destinationParentItem->attachChild(child, destinationChild + i);
                } else {
                    destinationParentItem->attachChild(child, destinationChild + i - 1);
                }
            } else {
                destinationParentItem->attachChild(child, destinationChild + i);
            }
            endMoveRows();
        }
        return true;
    } else {
        qWarning() << "ScanPlanItemModel::moveRows input sourceParent or destinationParent is invalid";
        return false;
    }
}
