#include "LicenseEditor.h"
#include "ui_LicenseEditor.h"
#include <allrising/Util.h>
#include "LicenseDriver.h"
#include <allrising/GeneralDelegate.h>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QMetaMethod>

enum Column {
    Name,
    ExpireEnable,
    Expire,
    Enable,
    Status,
};

LicenseEditor::LicenseEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LicenseEditor)
{
    ui->setupUi(this);

    startTimer(500);

    SetReadOnly(false);

    for (int row = 0; row < ui->infoLayout->rowCount(); ++row) {
        QLayoutItem *item = ui->infoLayout->itemAt(row, QFormLayout::FieldRole);
        auto *label = qobject_cast<QLabel *>(item->widget());
        if (label) {
            label->installEventFilter(this);
        }
    }

    auto delegate = new GeneralDelegate(this);
    ui->levels->setItemDelegateForColumn(0, delegate);
}

LicenseEditor::~LicenseEditor()
{
    delete ui;
}

bool LicenseEditor::IsReadOnly() const
{
    return readOnly;
}

LicenseDevice LicenseEditor::GetDevice() const
{
    LicenseDevice dev = device;
    dev.isValid = ui->isValid->isChecked();
    const QMetaObject &meta = LicenseDevice::staticMetaObject;
    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        const QMetaProperty prop = meta.property(index);
        auto *label = findChild<QLabel *>(prop.name());
        Q_ASSERT_X(prop.type() != QVariant::String || label, "LicenseEditor missing field", prop.name());
        if (label && prop.type() == QVariant::String) {
            prop.writeOnGadget(&dev, label->text());
        }
    }

    QList<LicenseLevel> levels;
    for (int index = 0; index < ui->levels->topLevelItemCount(); ++index) {
        levels << GetLevel(index);
    }

    dev.licenseLevels = levels;

    return dev;
}

LicenseLevel LicenseEditor::GetLevel(int index) const
{
    QTreeWidgetItem *item = ui->levels->topLevelItem(index);
    if (!item) {
        return LicenseLevel();
    }

    LicenseLevel level;
    level.SetName(item->text(0));
    bool expireEnable = item->data(ExpireEnable, Qt::CheckStateRole).toInt();
    QDateTime expire = item->data(Expire, Qt::EditRole).toDateTime();
    if (expireEnable) {
        level.expire = expire.isValid() ? expire : QDateTime::currentDateTime();
    } else {
        level.expire = QDateTime();
    }
    level.isValid = item->data(Enable, Qt::CheckStateRole).toInt();

    return level;
}

void LicenseEditor::SetReadOnly(bool newReadOnly)
{
    readOnly = newReadOnly;
    UpdateReadonlyControls();
}

void LicenseEditor::SetDevice(const LicenseDevice &newDevice)
{
    if (isUpdating) {
        return;
    }
    device = newDevice;
    isUpdating = true;

    ui->isValid->setChecked(device.isValid);
    const QMetaObject &meta = LicenseDevice::staticMetaObject;
    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        const QMetaProperty prop = meta.property(index);
        auto *label = findChild<QLabel *>(prop.name());
        Q_ASSERT_X(prop.type() != QVariant::String || label, "LicenseEditor missing field", prop.name());
        if (label && prop.type() == QVariant::String) {
            label->setText(prop.readOnGadget(&newDevice).toString());
        }
    }
    UpdateTime();

    QStringList availableLevels = MetaType::enumKeyList<LicenseLevel::Level>();
    while (ui->levels->topLevelItemCount() > newDevice.licenseLevels.count()) {
        delete ui->levels->takeTopLevelItem(ui->levels->topLevelItemCount() - 1);
    }
    for (int index = 0; index < newDevice.licenseLevels.count(); ++index) {
        QTreeWidgetItem *item = ui->levels->topLevelItem(index);
        if (!item) {
            item = new QTreeWidgetItem(ui->levels);
        }
        const LicenseLevel &level = newDevice.licenseLevels.value(index);

        item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);

        item->setText(Name, level.GetName());
        item->setData(Name, GeneralDelegate::DelegateRole, GeneralDelegate::ComboBox);
        item->setData(Name, GeneralDelegate::EnumRole, availableLevels);
        item->setData(ExpireEnable, Qt::CheckStateRole, level.expire.isValid() ? Qt::Checked : Qt::Unchecked);
        item->setData(Expire, Qt::EditRole, level.expire);
        item->setData(Enable, Qt::CheckStateRole, level.isValid ? Qt::Checked : Qt::Unchecked);
        bool isValid = device.IsLevelValid(index);
        item->setText(Status, device.GetLevelStatus(index));
        item->setIcon(Status, isValid ? QPixmap(":/fatcow/16x16/accept.png") : QPixmap(":/fatcow/16x16/delete.png"));
    }
//    ui->recharge->setVisible(device.driver && device.driver->CanRecharge());
    UpdateReadonlyControls();

    for (int index = 0; index < ui->levels->columnCount(); ++index) {
        ui->levels->resizeColumnToContents(index);
    }
    isUpdating = false;
}

void LicenseEditor::timerEvent(QTimerEvent *)
{
    if (isVisible()) {
        UpdateTime();
    }
}

bool LicenseEditor::eventFilter(QObject *watch, QEvent *event)
{
    QEvent::Type type = event->type();
    if (type == QEvent::MouseButtonDblClick && !readOnly) {
        auto *label = qobject_cast<QLabel *>(watch);
        if (label) {
            auto *title = qobject_cast<QLabel *>(ui->infoLayout->labelForField(label));
            if (!title) {
                return QWidget::eventFilter(watch, event);
            }
            bool ok = false;
            QString text = QInputDialog::getText(this, QString(), title->text(), QLineEdit::Normal, label->text(), &ok);
            if (ok) {
                label->setText(text);
            }
        }
    }
    return QWidget::eventFilter(watch, event);
}

void LicenseEditor::UpdateTime()
{
    ui->deviceTime->setText(device.GetTime().toString());
}

void LicenseEditor::UpdateReadonlyControls()
{
    ui->levels->setColumnHidden(ExpireEnable, readOnly);                        /* 隐藏“过期”列 */
    ui->levels->setColumnHidden(Enable, readOnly);                              /* 隐藏“激活”列 */

    ui->levels->setDragEnabled(!readOnly);
    ui->levels->setEditTriggers(readOnly ? QTreeWidget::NoEditTriggers : (QTreeWidget::DoubleClicked | QTreeWidget::EditKeyPressed));

    ui->addLevel->setHidden(readOnly);
    ui->removeLevel->setHidden(readOnly);
    ui->update->setHidden(readOnly || !(device.CanUpdate()));
    ui->recharge->setVisible(device.CanRecharge());
    ui->generateRecharge->setVisible(!readOnly && device.CanGenerageRecharge());
    ui->editConfig->setHidden(readOnly);

    ui->isValid->setHidden(readOnly);
    ui->isValidTitle->setHidden(readOnly);
    ui->deviceTime->setHidden(readOnly);
    ui->deviceTimeTitle->setHidden(readOnly);
}

void LicenseEditor::on_addLevel_clicked()
{
    LicenseLevel level;
    level.SetName(QInputDialog::getItem(this, QString(), tr("名称:"), MetaType::enumKeyList<LicenseLevel::Level>(), 0, false));
    if (level.IsValid()) {
        return;
    }

    LicenseDevice dev = GetDevice();
    dev.licenseLevels << level;
    SetDevice(dev);
}

void LicenseEditor::on_removeLevel_clicked()
{
    delete ui->levels->currentItem();
}

void LicenseEditor::on_levels_itemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    LicenseDevice dev = GetDevice();
    SetDevice(dev);
}

void LicenseEditor::on_recharge_clicked()
{
    bool ok;
    const QString code = QInputDialog::getText(this, QString(), tr("输入授权码:"), QLineEdit::Normal, QString(), &ok);
    if (ok) {
        emit RequestRecharge(code);
    }
}

void LicenseEditor::on_update_clicked()
{
    emit RequestUpdate();
}

void LicenseEditor::on_editConfig_clicked()
{
    const QByteArray oldText = JsonToByteArray(device.config);
    bool ok = false;
    const QString newText = QInputDialog::getMultiLineText(this, QString(), tr("编辑配置表:"), oldText, &ok);
    if (ok) {
        const QJsonValue v = ByteArrayToJson(newText.toLatin1());
        if (v.isNull() || v.isUndefined()) {
            QMessageBox::warning(this, QString(), tr("配置表无效"));
            return;
        }
        device.config = v;
    }
}

void LicenseEditor::on_generateRecharge_clicked()
{
    bool ok = false;
    QString target = QInputDialog::getText(this, QString(), tr("请输入目标设备的序列号"), QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }
    LicenseLevel level = GetLevel(ui->levels->currentIndex().row());
    emit RequestgenerateRecharge(device.deviceId, target, level);
}
