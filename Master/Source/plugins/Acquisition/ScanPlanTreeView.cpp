#include "ScanPlanTreeView.h"
#include "ui_PanelScanPlanControl.h"
#include "ScanPathAdvanced.h"
#include "ScanPathDiskEdit.h"
#include <Imaging/UTLayer.h>
#include <QFileDialog>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <allrising/GeneralDelegate.h>
#include <allrising/Util.h>

static Q_LOGGING_CATEGORY(debug, "acquire.plan.tree");

ScanPlanTreeView::ScanPlanTreeView(QWidget *parent) :
    QTreeView(parent),
    dialogScanPlan( new DialogScanPlanEdit(window()) )                        /* Dialog不应当有parent，如果parent为this，则setEnabled会影响对话框 */
{
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setAllColumnsShowFocus(true);

    initUi();
    setCurrentIndex( QModelIndex() );
}

ScanPlanTreeView::~ScanPlanTreeView() = default;

/*! 外部接口，获得当前TreeView上选中的ScanPlan */
ScanPlan *ScanPlanTreeView::currentScanPlan()
{
    return scanPlanFromIndex( currentIndex() );
}

ScanPlan *ScanPlanTreeView::scanPlanFromIndex(const QModelIndex &index) const
{
    return ScanPlanModel::instance()->scanPlanFromIndex(index);
}

/*! 初始化 */
void ScanPlanTreeView::initUi()
{
    // 设置tree是行选择与不可编辑
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置委托进度条
    setItemDelegateForColumn(1, new GeneralDelegate(this));
    // 绑定信号和槽
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &ScanPlanTreeView::OnCustomContextMenu);
    connect(this, &QAbstractItemView::doubleClicked, this, &ScanPlanTreeView::OnDoubleClicked);
    connect(dialogScanPlan, &QDialog::finished, this, &ScanPlanTreeView::OnDialogScanPlanAccepted);

    setModel( ScanPlanModel::instance() );
    setSelectionModel( ScanPlanModel::instance()->selectionModel() );
    setColumnWidth(0, 180);
    setColumnWidth(1, 120);
    expandToDepth(3);
    // 设置完model之后，需要将对应的selectionmodel的改变信号发送到对应的槽函数
    connect(ScanPlanModel::instance()->selectionModel(), &QItemSelectionModel::currentChanged, this, &ScanPlanTreeView::refreshActionsEnable);
}

/*! 新增扫描计划 */
void ScanPlanTreeView::addLegacyScanPlan(ScanPlan::MotionType motionType)
{
    QString name = ScanPlanModel::instance()->newScanPlanName(motionType);
//    dialogScanPlan->SetInfo(name, motionType, MotionPath(), QList<ImageOptions>());
    setEnabled(false);
    ScanPlan scanPlan;
    scanPlan.setName(name);
    scanPlan.setPathType("Advanced");
    ScanPathAdvancedOptions options;
    options.motionType = EnumToKey(motionType);
    scanPlan.setOptions(QVariant::fromValue(options));
    dialogScanPlan->setScanPlan(&scanPlan);
    dialogScanPlan->show();
    resizeDialog();
    dialogMode = New;
    setActionsEnable(false);
}

void ScanPlanTreeView::addScanPlan(const QString &typeName)
{
    auto factory = ScanPathFactory::fromName(typeName);
    if (!factory) {
        qCWarning(debug) << "Can't add scan plan with invalid type name" << typeName;
        return;
    }
    QString name = ScanPlanModel::instance()->newScanPlanName(factory->displayName());
    ScanPlan scanPlan;
    scanPlan.setName(name);
    scanPlan.setPathType(typeName);
    dialogScanPlan->setScanPlan(&scanPlan);
    dialogScanPlan->show();
    resizeDialog();
    dialogMode = New;
    setActionsEnable(false);
}

/*!
 * \brief ScanPlanTreeView::SetCurrentItem 选中指定的scanpaln
 * \param row 在父节点中的序号
 * \param parent 父节点
 */
void ScanPlanTreeView::setCurrentItem(int row, const QModelIndex &parent)
{
    ScanPlan *parentScanPlan = ScanPlanModel::instance()->scanPlanFromIndex(parent);
    if (parentScanPlan != nullptr) {
        clearSelection();
        int rowCount = parentScanPlan->childrenCount();
        if ( (rowCount > row) && (rowCount > 0) ) {
            setCurrentIndex( ScanPlanModel::instance()->index(row, 0, parent) );
        } else if ( (rowCount <= row) && (rowCount > 0) ) {
            setCurrentIndex( ScanPlanModel::instance()->index(rowCount - 1, 0, parent) );
        } else if (rowCount <= 0) {
            setCurrentIndex(parent);
        }
    }
}

/*! 重载的拖动接口 */
void ScanPlanTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    event->ignore();
    if ((event->source() != this) || !(event->possibleActions() & Qt::MoveAction)) {
        return;
    }
    QModelIndex index = indexAt( event->pos() );
    ScanPlan *scanPlan = ScanPlanModel::instance()->scanPlanFromIndex(index);
    if (scanPlan != nullptr) {
        QTreeView::dragMoveEvent(event);
    }
}

/*! 重载的鼠标拖放事件 */
void ScanPlanTreeView::dropEvent(QDropEvent *event)
{
    event->ignore();
    if ( (event->source() != this) || !(event->possibleActions() & Qt::MoveAction) ) {
        return;
    }
    ScanPlanModel *model = ScanPlanModel::instance();
    QModelIndex destindex = indexAt( event->pos() );

    ScanPlan *destScanPlan = model->scanPlanFromIndex(destindex);
    QModelIndex sourceIndex = this->currentIndex();

    ScanPlan *sourceScanPlan = model->scanPlanFromIndex(sourceIndex);
    if (rect().contains( event->pos() ) && (destScanPlan != nullptr) && (sourceScanPlan != nullptr)) {
        auto *sourceParent = dynamic_cast<ScanPlan *>( sourceScanPlan->parent() );
        QModelIndex sourceParentIndex = sourceIndex.parent();
        auto *destParent = dynamic_cast<ScanPlan *>( destScanPlan->parent() );
        QModelIndex destParentIndex = destindex.parent();
        if ( (sourceParent == nullptr) || (destParent == nullptr) ) {
            return;                                                             // 移动源不能为基本节点，目标源不能是基本节点位置（不能添加到root）
        }

        int sourceRow = sourceParent->children().indexOf(sourceScanPlan);
        int destChild = destParent->children().indexOf(destScanPlan);
        QRect rect = visualRect(destindex);

        if ( ( destScanPlan->depth() < ScanPlanModel::instance()->maxDepth() ) && destScanPlan->isFolder()) {           // 可以接受放
            if (event->pos().y() - rect.top() < MARGIN) {                       // 插入到index 的上面
                // 不能移动到原来位置且不能插在基本节点的上面和下面
                if ( ( (sourceParent == destParent) && (sourceRow == destChild - 1) ) || !sourceParentIndex.isValid() || !destParentIndex.isValid() ) {
                    return;
                }
                model->moveRows(sourceParentIndex, sourceRow, 1, destParentIndex, destChild);
            } else if (rect.bottom() - event->pos().y() < MARGIN) {                // 插入到index 的下面
                // 不能移动到原来位置且不能插在基本节点的上面和下面
                if ( ((sourceParent == destParent) && (sourceRow == destChild + 1)) || !sourceParentIndex.isValid() || !destParentIndex.isValid() ) {
                    return;
                }
                model->moveRows(sourceParentIndex, sourceRow, 1, destParentIndex, destChild + 1);
            } else {                                                            // 添加为index 的子节点
                if (sourceParent != destScanPlan) {
                    model->moveRows( sourceParentIndex, sourceRow, 1, destindex, destScanPlan->childrenCount() );
                }
            }
        } else {
            if (event->pos().y() - rect.top() < MARGIN) {                        // 插入到index 的上面
                // 不能移动到原来位置且不能插在基本节点的上面和下面
                if ( ( (sourceParent == destParent) && (sourceRow == destChild - 1) ) || !sourceParentIndex.isValid() || !destParentIndex.isValid() ) {
                    return;
                }
                model->moveRows(sourceParentIndex, sourceRow, 1, destParentIndex, destChild);
            } else {                                                            // 插入到index 的下面
                // 不能移动到原来位置且不能插在基本节点的上面和下面
                if ( ( (sourceParent == destParent) && (sourceRow == destChild + 1) ) || !sourceParentIndex.isValid() || !destParentIndex.isValid() ) {
                    return;
                }
                model->moveRows(sourceParentIndex, sourceRow, 1, destParentIndex, destChild + 1);
            }
        }
    }
}

/*!
 * \brief ScanPlanTreeView::OnSelectionChanged 接收scanplanmodel的selectionmodel的currentchanged信号的槽函数
 * \param current       当前选中的项的index,
 * \param previous      原来选中的项的index
 */
void ScanPlanTreeView::refreshActionsEnable(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // 在菜单控制之前，需要将所有的菜单初始化使能状态
    setActionsEnable(false);
    ScanPlan *scanPlan = ScanPlanModel::instance()->scanPlanFromIndex(current);
    // 根据选中的scanplan的实际情况，控制action的使能状态
    if ( !current.isValid() || (scanPlan == nullptr) ) {
        return;
    }
    if (scanPlan->isFolder()) {
        if (scanPlan->depth() < ScanPlanModel::instance()->maxDepth() - 1) {
            m_control->ui->actionAddFolder->setEnabled(true);
        }
        m_control->ui->actionAddRectangle->setEnabled(true);
        m_control->ui->actionAddLegacyRectangle->setEnabled(true);
        m_control->ui->actionAddLegacyCylinder->setEnabled(true);
        m_control->ui->actionAddCylinder->setEnabled(true);
        m_control->ui->actionAddCylinderRotation->setEnabled(true);
        m_control->ui->actionAddDisk->setEnabled(true);
        m_control->ui->actionAddLegacyDisk->setEnabled(true);
        m_control->ui->actionAddDiskRotation->setEnabled(true);
        m_control->ui->actionAddAnnulus->setEnabled(true);
        m_control->ui->actionQuickStart->setEnabled(false);
    } else {
        m_control->ui->actionQuickStart->setEnabled(true);
    }
    if ( current.parent().isValid() ) {
        if (scanPlan->isEditable()) {
            m_control->ui->actionEdit->setEnabled(true);
        }
        m_control->ui->actionRename->setEnabled(true);
        m_control->ui->actionDelete->setEnabled(true);
    }
}

/*! 右键菜单 */
void ScanPlanTreeView::OnCustomContextMenu(const QPoint &)
{
    QMenu menu;
    menu.addActions(m_control->ui->toolbar->actions());
    menu.addAction(m_control->ui->actionCreateTask);
    menu.exec(QCursor::pos());
    for (QAction *action: m_control->ui->toolbar->actions()) {
        action->setParent(this);
    }
}

/*! 双击事件 */
void ScanPlanTreeView::OnDoubleClicked(const QModelIndex &index)
{
    ScanPlan *scanPlan = ScanPlanModel::instance()->scanPlanFromIndex(index);
    if ((scanPlan != nullptr) && index.isValid()) {
        if (scanPlan->isEditable()) {
            EditScanPlan();
        }
    }
}

/*! 添加文件夹类型 */
void ScanPlanTreeView::addFolder()
{
    QModelIndex index = currentIndex();
    if ( index.isValid() ) {
        QJsonObject obj;
        obj.insert("Type", "ScanPlan");
        obj.insert("MotionType", "NONE");
        ScanPlan *folder = ScanPlanModel::instance()->insertNewScanPlan(obj, index);
        if (folder != nullptr) {
            QModelIndex folderIndex = ScanPlanModel::instance()->indexOf(folder);
            if ( folderIndex.isValid() ) {
                setCurrentIndex(folderIndex);
                RenameScanPlan();
            }
        }
    }
}

void ScanPlanTreeView::addRectangle()
{
    addScanPlan("Rectangle");
}

void ScanPlanTreeView::addLegacyRectangle()
{
    addLegacyScanPlan(ScanPlan::RECTANGLE);
}

void ScanPlanTreeView::addLegacyCylinder()
{
    addLegacyScanPlan(ScanPlan::CYLINDER);
}

void ScanPlanTreeView::addCylinder()
{
    addScanPlan("Cylinder");
}

void ScanPlanTreeView::addCylinderRotation()
{
    addLegacyScanPlan(ScanPlan::CYLINDERROTATION);
}

void ScanPlanTreeView::addDisk()
{
    addScanPlan("Disk");
}

void ScanPlanTreeView::addLegacyDisk()
{
    addLegacyScanPlan(ScanPlan::DISK);
}

void ScanPlanTreeView::addDiskRotation()
{
    addLegacyScanPlan(ScanPlan::DISKROTATION);
}

void ScanPlanTreeView::addRing()
{
    addScanPlan("Ring");
}

void ScanPlanTreeView::ImportCam()
{
    ScanPlan *currentItem = currentScanPlan();
    if (!currentItem || !currentItem->isFolder()) {
        return;
    }

    MotionPath path;

    QString pathBase = QFileDialog::getExistingDirectory(this);
    if (pathBase.isEmpty()) {
        return;
    }
    qCDebug(debug) << "Using base path" << pathBase;
    QFile lengthFile(pathBase + "/ScanningNum.csv");
    qCDebug(debug) << "Opening line count file:" << lengthFile.open(QIODevice::ReadOnly);
    QStringList pointFileNames = QStringList() << "joint_x.csv" << "joint_y.csv" << "joint_z.csv" << "joint_b.csv" << "joint_a.csv";
    MotionAxisList axisList = QList<int>({0, 1, 2, 3, 4});

    qreal speed = QInputDialog::getDouble(nullptr, QString(), "Virtual speed: ", 50000, 1);
    qreal returnSpeed = QInputDialog::getDouble(nullptr, QString(), "Return virtual speed: ", 50000, 1);

    auto count = ReadCsvAsDouble(pathBase + "/ScanningNum.csv");
    QList<QList<QList<double> > > data;
    for (const auto &name: pointFileNames) {
        data << ReadCsvAsDouble(pathBase + "/" + name);
    }
    for (int elementIndex = 0; elementIndex < count.count(); ++elementIndex) {
        const int length = qRound(count[elementIndex].value(0));
        MotionElement element;
        element.type = MotionElement::Cam;
        element.stepInterval = 1;
        element.axis = axisList;
        element.speed = speed;
        element.stepSpeed = returnSpeed;
        for (int poseIndex = 0; poseIndex < length; ++poseIndex) {
            MotionPose pose;
            for (int axisIndex = 0; axisIndex < data.count(); ++axisIndex) {
                pose << data.value(axisIndex).value(elementIndex).value(poseIndex);
            }
            element.pos << pose;
        }
        path.elements << element;
    }

    QJsonObject json/* = dialogScanPlan->GetJson()*/;
    json["Type"] = "ScanPlan";
    json["Name"] = ScanPlanModel::instance()->newScanPlanName(ScanPlan::CAM);
    json["MotionType"] = EnumToKey(ScanPlan::CAM);
    json["MotionPath"] = path.toJson();
    ScanPlanModel *model = ScanPlanModel::instance();

    ScanPlan *newItem = model->insertNewScanPlan( json, currentIndex() );
    expand( currentIndex() );
    if (newItem != nullptr) {
        QModelIndex newIndex = model->indexOf(newItem);
        if ( newIndex.isValid() ) {
            setCurrentIndex(newIndex);
        }
    }
}

/*! 重命名 */
void ScanPlanTreeView::RenameScanPlan()
{
    QModelIndex index = currentIndex();
    ScanPlan *curPlan = ScanPlanModel::instance()->scanPlanFromIndex(index);
    if ( (curPlan != nullptr) && index.isValid() ) {
        // 使用代理编辑框 ，调用model 的setdata
        edit(index);
    }
}

/*! 编辑 */
void ScanPlanTreeView::EditScanPlan()
{
    setEnabled(false);
    ScanPlan *current = currentScanPlan();
    if (current) {
        dialogScanPlan->setScanPlan(current);
        dialogScanPlan->show();
        resizeDialog();
        dialogMode = Edit;
    }
}

/*! 删除 */
void ScanPlanTreeView::DelScanPlan()
{
    QModelIndex index = currentIndex();
    ScanPlan *curPlan = currentScanPlan();
    qCDebug(debug) << "Remove ScanPlan" << curPlan << curPlan->name() << "index" << index;
    if ( (curPlan != nullptr) && index.isValid() ) {
        if (QMessageBox::warning(this, QString(), tr("确定删除此扫描计划吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
            QModelIndex parentIndex = index.parent();
            ScanPlan *parent = scanPlanFromIndex(parentIndex);
            if (!parent) {
                return;
            }
            qCDebug(debug) << "Remove from parent" << parent << parent->name() << "index" << parentIndex;
            int row = parent->children().indexOf(curPlan);
            ScanPlanModel::instance()->removeRows(row, 1, parentIndex);
            // 删除之后，需要正确移动选中光标
            setCurrentItem(row, parentIndex);
        }
    } else {
        QMessageBox::warning(this, QString(), "The base node can not be deleted");
    }
}

void ScanPlanTreeView::OnDialogScanPlanAccepted(int result)
{
    setEnabled(true);
    if (result != QDialog::Accepted) {
        refreshActionsEnable(ScanPlanModel::instance()->selectionModel()->currentIndex(), QModelIndex());
        return;
    }
    QJsonObject json = dialogScanPlan->scanPlan()->toJson();
    ScanPlan *currentItem = currentScanPlan();
    if (!currentItem) {
        return;
    }
    ScanPlanModel *model = ScanPlanModel::instance();
    if (dialogMode == Edit) {
        model->reloadJson(currentScanPlan(), json);
    } else if (dialogMode == New) {
        ScanPlan *newItem = model->insertNewScanPlan( json, currentIndex() );
        expand( currentIndex() );
        if (newItem != nullptr) {
            QModelIndex newIndex = model->indexOf(newItem);
            if ( newIndex.isValid() ) {
                setCurrentIndex(newIndex);
            }
        }
    }
    ScanPlanModel::instance()->saveSettings();
}

void ScanPlanTreeView::setActionsEnable(bool enabled)
{
    for (QAction *button: m_control->ui->toolbar->actions().mid(3, -1)) {       /* FIXME: 增加获取非扫描控制的actions接口 */
        button->setEnabled(enabled);
    }
}

void ScanPlanTreeView::resizeDialog()
{
    if (dialogScanPlan) {
        dialogScanPlan->resize(0, 0);
    }
}

/*! 连接到控制面板的按键 */
void ScanPlanTreeView::initActions(PanelScanPlanControl *control)
{
    if (control == nullptr) {
        return;
    }
    m_control = control;
    connect(control->ui->actionAddFolder, &QAction::triggered, this, &ScanPlanTreeView::addFolder);
    connect(control->ui->actionAddRectangle, &QAction::triggered, this, &ScanPlanTreeView::addRectangle);
    connect(control->ui->actionAddLegacyRectangle, &QAction::triggered, this, &ScanPlanTreeView::addLegacyRectangle);
    connect(control->ui->actionAddLegacyCylinder, &QAction::triggered, this, &ScanPlanTreeView::addLegacyCylinder);
    connect(control->ui->actionAddCylinder, &QAction::triggered, this, &ScanPlanTreeView::addCylinder);
    connect(control->ui->actionAddCylinderRotation, &QAction::triggered, this, &ScanPlanTreeView::addCylinderRotation);
    connect(control->ui->actionAddDisk, &QAction::triggered, this, &ScanPlanTreeView::addDisk);
    connect(control->ui->actionAddLegacyDisk, &QAction::triggered, this, &ScanPlanTreeView::addLegacyDisk);
    connect(control->ui->actionAddDiskRotation, &QAction::triggered, this, &ScanPlanTreeView::addDiskRotation);
    connect(control->ui->actionAddAnnulus, &QAction::triggered, this, &ScanPlanTreeView::addRing);
    connect(control->ui->actionImportCam, &QAction::triggered, this, &ScanPlanTreeView::ImportCam);
    connect(control->ui->actionRename, &QAction::triggered, this, &ScanPlanTreeView::RenameScanPlan);
    connect(control->ui->actionEdit, &QAction::triggered, this, &ScanPlanTreeView::EditScanPlan);
    connect(control->ui->actionDelete, &QAction::triggered, this, &ScanPlanTreeView::DelScanPlan);
//    connect(control->ui->actionSetup, &QAction::triggered, this, &ScanPlanTreeView::OnEditSetup);
    control->ui->actionAddLegacyCylinder->setVisible(false);
    control->ui->actionAddLegacyRectangle->setVisible(false);
    control->ui->actionAddLegacyDisk->setVisible(false);
    setActionsEnable(false);
}
