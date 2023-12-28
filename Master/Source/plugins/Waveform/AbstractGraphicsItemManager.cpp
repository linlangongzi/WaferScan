#include "AbstractGraphicsItemManager.h"
#include "UTGraphicsScene.h"

AbstractGraphicsItemManager::AbstractGraphicsItemManager(QObject *parent) :
    QObject(parent),
    scene(nullptr)
{

}

AbstractGraphicsItemManager::~AbstractGraphicsItemManager() = default;

/*!
 * \brief AbstractGraphicsItemManager::AddToScene 将manager添加到scene中
 * \param scene
 */
void AbstractGraphicsItemManager::AddToScene(UTGraphicsScene *graphicsScene)
{
    if (scene) {
        disconnect( scene, nullptr, this, nullptr );
        disconnect( this, nullptr, scene, nullptr );
    }
    scene = graphicsScene;
//    connect(scene, &UTGraphicsScene::MatrixChanged, this, &AbstractGraphicsItemManager::UpdateMatrix);
    connect(scene, &UTGraphicsScene::contextMenuRequest, this, &AbstractGraphicsItemManager::OnSceneContextMenuRequest);
    connect(scene, &QObject::destroyed, this, &AbstractGraphicsItemManager::OnSceneDestroyed);
    connect(this, &AbstractGraphicsItemManager::ContextMenuTriggered, scene, &UTGraphicsScene::onManagerContextMenuTriggered);
//    UpdateMatrix();
}

//void AbstractGraphicsItemManager::UpdateMatrix()
//{
//}

/*!
 * \brief AbstractGraphicsItemManager::OnSceneContextMenuRequest 接收scene的右键菜单请求
 */
void AbstractGraphicsItemManager::OnSceneContextMenuRequest()
{

}

/*!
 * \brief AbstractGraphicsItemManager::InitMenu 初始化自身右键菜单
 */
void AbstractGraphicsItemManager::InitMenu()
{
    qDeleteAll(actions);
    actions.clear();
}

/*!
 * \brief AbstractGraphicsItemManager::OnSceneDestroyed SCENE析构之前发送的析构信号的槽函数
 */
void AbstractGraphicsItemManager::OnSceneDestroyed()
{

}

