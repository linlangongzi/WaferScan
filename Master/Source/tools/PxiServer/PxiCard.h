#pragma once

#include <QObject>

typedef struct P9054_STRUCT *P9054_HANDLE;

class PxiCard : public QObject
{
    Q_OBJECT

protected:
    explicit PxiCard(P9054_HANDLE handle, QObject *parent = 0);

public:
    ~PxiCard();

    void dumpInfo() const;
    bool ramTest(quint32 startAddress, quint32 size, int count = 10);
    quint16 read(quint32 address) const;
    void write(quint32 address, quint16 data);

signals:

public slots:

private:
    P9054_HANDLE handle;
    int index = -1;
    int addressSpace = 2;

    friend class PxiManager;
};
