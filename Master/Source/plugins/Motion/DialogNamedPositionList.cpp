#include "DialogNamedPositionList.h"
#include "ui_DialogNamedPositionList.h"
#include "DialogNamedPosition.h"
#include <Motion/Axis.h>
#include <Motion/EndPoint.h>
#include <QLoggingCategory>
#include <QMenu>
#include <QMessageBox>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "motion.marker.list");

DialogNamedPositionList::DialogNamedPositionList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNamedPositionList)
{
    ui->setupUi(this);
    setSelectionMode(true);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QWidget::customContextMenuRequested, this, &DialogNamedPositionList::customContextMenu);
    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &DialogNamedPositionList::onDoubleClicked);

    initModel();
}

DialogNamedPositionList::~DialogNamedPositionList()
{
    delete ui;
}

/*! 初始化model */
void DialogNamedPositionList::initModel()
{
    QStringList headerList = {tr("名称")};
    for (Axis *axis: MotionManager::axisList()) {
        headerList.append(tr("%1 %2").arg(axis->name(), axis->axisUnit()));
    }
    MotionManager::positionModel()->setHeader(headerList);
    ui->tableView->setModel(MotionManager::positionModel());
}

/*!
 * \brief DialogNamedPositionList::setSelectedMode 设计界面模式，选择模式/操作模式
 * \param enabel true: 选择模式，只有选择浏览效果
 *               false: 操作模式，可以操作坐标点
 */
void DialogNamedPositionList::setSelectionMode(bool enable)
{
    m_selectionMode = enable;
    QDialogButtonBox::StandardButtons flags = ui->buttonBox->standardButtons();
    flags.setFlag(QDialogButtonBox::Cancel, enable);
    ui->buttonBox->setStandardButtons(flags);
}

/*! 获取当前选择的 NamedPositionProperty */
NamedPosition *DialogNamedPositionList::selectedNamedPosition() const
{
    QModelIndex index = ui->tableView->currentIndex();
    return MotionManager::positionModel()->itemForIndexAtLevel<1>(index);
}

/*! 鼠标右键添加事件函数，向表中插入新坐标数据，并写入model中 */
void DialogNamedPositionList::on_actionAdd_triggered()
{
    DialogNamedPosition dialog(this);

    auto *model = MotionManager::positionModel();
    /* 设置默认坐标 */
    MotionPose currentPos;
    for (Axis *axis: MotionManager::axisList()) {
        currentPos << axis->position();
    }
    dialog.setName(tr("坐标 %1").arg(model->rootItem()->childCount() + 1));
    dialog.setPosition(currentPos);

    forever {
        if (dialog.exec() != QDialog::Accepted) {
            return;
        }

        auto duplicateItem = model->findByName(dialog.name());
        if (duplicateItem) {
            QMessageBox::warning(this, QString(), tr("名称已经存在, 请重新输入."));
            continue;
        }

        duplicateItem = model->loadingPosition();
        if (dialog.isLoadingPosition() && duplicateItem) {
            QMessageBox::warning(this, QString(), tr("上料位置已存在, 不能添加多个上料位置."));
            continue;
        }

        auto *newItem = new NamedPosition;
        newItem->name = dialog.name();
        newItem->isLoadingPosition = dialog.isLoadingPosition();
        newItem->pos = dialog.position();
        MotionManager::positionModel()->rootItem()->appendChild(newItem);       //!< positionModel take Ownship of User
        return;
    }
}

/*! 删除坐标点 */
void DialogNamedPositionList::on_actionRemove_triggered()
{
    NamedPosition *item = selectedNamedPosition();
    if (!item) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除坐标?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
        MotionManager::positionModel()->destroyItem(item);
    }
}

/*!
 * \brief DialogNamedPositionList::ModifyMarker 修改坐标点
 */
void DialogNamedPositionList::on_actionModify_triggered()
{
    DialogNamedPosition dialog(this);
    NamedPosition *item = selectedNamedPosition();
    if (!item) {
        return;
    }
    dialog.setName(item->name);
    dialog.setPosition(item->pos);
    dialog.setLoadingPosition(item->isLoadingPosition);

    auto *model = MotionManager::positionModel();
    forever {
        if (dialog.exec() != QDialog::Accepted) {
            return;
        }
        auto *duplicateItem = model->findByName(dialog.name());
        if (duplicateItem && duplicateItem != item) {
            QMessageBox::warning(this, QString(), tr("名称已经存在, 请重新输入."));
            continue;
        }

        duplicateItem = model->loadingPosition();
        if (dialog.isLoadingPosition() && duplicateItem && duplicateItem != item) {
            QMessageBox::warning(this, QString(), tr("上料位置已经存在."));
            dialog.setLoadingPosition(false);
            continue;
        }
        item->name = dialog.name();
        item->isLoadingPosition = dialog.isLoadingPosition();
        item->pos = dialog.position();
        item->update();
        return;
    }
}

/*! 移动到坐标点 */
void DialogNamedPositionList::on_actionMove_triggered()
{
    NamedPosition *item = selectedNamedPosition();
    if (!item) {
        return;
    }
    EndPoint *endPoint = MotionManager::endPoint(0);
    if (!endPoint) {
        qCCritical(debug) << "Can't move to pointed position: Go invalid endPoint by id 1";
        return;
    }
    endPoint->Locate(item->pos, item->axis);
}

/*! 设置当前点为上料位置 */
void DialogNamedPositionList::on_actionLoad_triggered()
{
    NamedPosition *item = selectedNamedPosition();
    if (!item) {
        return;
    }
    auto duplicateItem = MotionManager::positionModel()->loadingPosition();
    if (duplicateItem) {
        QMessageBox::warning(this, QString(), tr("上料位置已经存在"));
        return;
    }
    item->isLoadingPosition = true;
    item->update();
}

/*!
 * \brief DialogNamedPositionList::customContextMenu 右键菜单的相应接口
 * \param point 请求上下文菜单的位置
 */
void DialogNamedPositionList::customContextMenu(const QPoint &point)
{
    Q_UNUSED(point);
    if (m_selectionMode) {
        return;
    }
    QMenu menu(this);
    menu.addAction(ui->actionAdd);
    menu.addAction(ui->actionModify);
    menu.addAction(ui->actionRemove);
    menu.addSeparator();
    menu.addAction(ui->actionMove);
    menu.addSeparator();
    menu.addAction(ui->actionLoad);
    menu.exec(QCursor::pos());
}

/*! 双击的槽函数接口 */
void DialogNamedPositionList::onDoubleClicked()
{
    if (!m_selectionMode) {                                                     // 选择模式
        on_actionModify_triggered();
    }
}

/*!
 * \brief DialogNamedPositionList::GetCoord 获取轴位置
 * \param axis 轴号
 * \return     轴所在位置
 */
double DialogNamedPositionList::coordinate(int axis) const
{
    NamedPosition *item = selectedNamedPosition();
    if (!item) {
        return 0;
    }
    return item->pos.value(axis);
}

bool DialogNamedPositionList::isSelectionMode() const
{
    return m_selectionMode;
}
