#pragma once

#include <Waveform/AbstractGraphicsItemManager.h>

class CScanGraphicsItemManager : public AbstractGraphicsItemManager
{
    Q_OBJECT

public:
    explicit CScanGraphicsItemManager(QObject *parent);

public:
    void SetLayerID(int layerID);

signals:

protected slots:
    virtual void ConnectToLayer(int layerID);
    virtual void DisconnectToLayer(int layerID);

protected:
    int LayerID;
};
