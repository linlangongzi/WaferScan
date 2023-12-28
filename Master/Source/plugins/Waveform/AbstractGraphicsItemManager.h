#pragma once

#include <QObject>
#include <QAction>

class UTGraphicsScene;

class AbstractGraphicsItemManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGraphicsItemManager(QObject *parent = nullptr);
    ~AbstractGraphicsItemManager();

public:
    virtual void AddToScene(UTGraphicsScene *graphicsScene);                            // 注：添加Item到scene的同时，需要将scene中的matrix设置给item

signals:
    void ContextMenuTriggered(QList<QAction *> actions);

protected slots:
//    virtual void UpdateMatrix();
    virtual void OnSceneContextMenuRequest();
    virtual void InitMenu();
    virtual void OnSceneDestroyed();

protected:
    UTGraphicsScene *scene;
    QList<QAction *> actions;
};
