#pragma once

#include <QComboBox>
#include <Ultrasonic/Material.h>

class MaterialComboBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QUuid currentUuid READ currentUuid WRITE setCurrentUuid NOTIFY currentUuidChanged)

public:
    explicit MaterialComboBox(QWidget *parent = nullptr);
    QUuid currentUuid() const;

public slots:
    void setCurrentUuid(const QUuid &currentUuid);

signals:
    void currentUuidChanged(const QUuid &currentUuid);

private:
    QUuid m_currentUuid;
};

