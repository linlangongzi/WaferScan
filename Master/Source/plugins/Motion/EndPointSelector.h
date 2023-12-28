#pragma once

#include "MotionGlobal.h"
#include <QComboBox>

class EndPoint;

class MOTION_EXPORT EndPointSelector : public QComboBox
{
    Q_OBJECT

public:
    using Filter = std::function<bool (EndPoint *)>;
    explicit EndPointSelector(QWidget *parent = nullptr);

    EndPoint *currentEndPoint() const;
    QUuid currentId() const;

    void setFilter(Filter filter);
    void setCurrentEndPoint(EndPoint *endPoint);
    void setCurrentUuid(const QUuid &endPointId);

signals:
    void currentUuidChanged(const QUuid &endPointId);

private:
    Filter m_filter = nullptr;
};
