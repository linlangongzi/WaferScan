#pragma once

#include <QObject>

class PxiCard;

class PxiManager : public QObject
{
    Q_OBJECT

public:
    explicit PxiManager(QObject *parent = 0);
    ~PxiManager();

    void rescan();

signals:

public slots:

private:
    QList<PxiCard *> cards;
};
