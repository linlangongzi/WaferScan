#include "MaterialComboBox.h"
#include <Ultrasonic/UltrasonicManager.h>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "wave.materialcombobox");

MaterialComboBox::MaterialComboBox(QWidget *parent) :
    QComboBox(parent)
{
    setModel(UltrasonicManager::materialModel());
    setModelColumn(UltrasonicManager::materialModel()->rootItem()->childAt(0)->ColName);
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), [this] (int currentIndex) {
        const QUuid id = itemData(currentIndex, MaterialModel::UuidRole).toUuid();
        emit currentUuidChanged(id);
    });
}

QUuid MaterialComboBox::currentUuid() const
{
    return m_currentUuid;
}

void MaterialComboBox::setCurrentUuid(const QUuid &currentUuid)
{
    if (m_currentUuid == currentUuid)
        return;

    m_currentUuid = currentUuid;

    int index = findData(currentUuid);
    if (index >= 0) {
        setCurrentIndex(index);
    }

    emit currentUuidChanged(m_currentUuid);
}
