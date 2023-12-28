#include "CScanGraphicsItemManager.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <Imaging/UTLayer.h>

CScanGraphicsItemManager::CScanGraphicsItemManager(QObject *parent) :
    AbstractGraphicsItemManager(parent),
    LayerID(1)
{}

/*! 更换layerID */
void CScanGraphicsItemManager::SetLayerID(int layerID)
{
    DisconnectToLayer(LayerID);
    LayerID = layerID;
    ConnectToLayer(LayerID);
}

/*! 连接与layer的信号 */
void CScanGraphicsItemManager::ConnectToLayer(int layerID)
{
    Q_UNUSED(layerID);
}

/*! 解除与layer的绑定 */
void CScanGraphicsItemManager::DisconnectToLayer(int layerID)
{
    UTLayer *layer = document()->GetLayerByID(layerID);
    if (!layer) {
        return;
    }
    disconnect(layer, nullptr, this, nullptr);
}
