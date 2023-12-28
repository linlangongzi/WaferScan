#pragma once

#include <QDialog>
#include <Motion/MotionManager.h>

namespace Ui {
class DialogNamedPosition;
}
class QDoubleSpinBox;

class DialogNamedPosition : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(MotionPose position READ position WRITE setPosition)
    Q_PROPERTY(bool isLoadingPosition READ isLoadingPosition WRITE setLoadingPosition)
//    Q_PROPERTY(MotionAxisList axis READ axis WRITE setAxis)

public:
    explicit DialogNamedPosition(QWidget *parent = 0);
    ~DialogNamedPosition();

    QString name() const;
    MotionPose position() const;
    bool isLoadingPosition() const;
//    MotionAxisList axis() const;

public slots:
    void setName(const QString &name);
    void setPosition(const MotionPose &position);
//    void setAxis(MotionAxisList axis);
    void setLoadingPosition(bool isLoadingPosition);

private:
    void initUi();

private:
    Ui::DialogNamedPosition *ui;
    QList<QDoubleSpinBox *> posEditList;
};
