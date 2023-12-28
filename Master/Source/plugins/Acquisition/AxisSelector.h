#pragma once

#include <QComboBox>
#include <Motion/Axis.h>
#include <QMap>
#include <QDebug>

class AxisSelector : public QComboBox
{
    Q_OBJECT

public:
    using Filter = std::function<bool (Axis *)>;

    explicit AxisSelector(QWidget *parent = nullptr);

public:
    Axis *currentAxis() const;
    void setFilter(Filter newFilter);
    QUuid currentAxisUuid() const;
    void setCurrentAxis(Axis *axis);
    void setCurrentUuid(const QUuid &axisUuid);
    int currentAxisIndex();

signals:
    void currentAxisUuidChanged(const QUuid &id);

private:
    Filter filter;
};
