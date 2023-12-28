#pragma once

#include <osg/Material>
#include <osg/MatrixTransform>
#include <osgManipulator/Dragger>
#include <QColor>
#include <QObject>
#include <osg/BlendFunc>

class QMenu;
class OsgContainer;

struct BlendOptions
{
    Q_GADGET

    Q_PROPERTY(bool enabled                   MEMBER enabled         )
    Q_PROPERTY(BlendFuncMode source           MEMBER source          )
    Q_PROPERTY(BlendFuncMode destination      MEMBER destination     )
    Q_PROPERTY(BlendFuncMode sourceAlpha      MEMBER sourceAlpha     )
    Q_PROPERTY(BlendFuncMode destinationAlpha MEMBER destinationAlpha)
    Q_PROPERTY(QColor color                   MEMBER color           )

public:
    enum BlendFuncMode {
        DST_ALPHA                = osg::BlendFunc::DST_ALPHA               ,
        DST_COLOR                = osg::BlendFunc::DST_COLOR               ,
        ONE                      = osg::BlendFunc::ONE                     ,
        ONE_MINUS_DST_ALPHA      = osg::BlendFunc::ONE_MINUS_DST_ALPHA     ,
        ONE_MINUS_DST_COLOR      = osg::BlendFunc::ONE_MINUS_DST_COLOR     ,
        ONE_MINUS_SRC_ALPHA      = osg::BlendFunc::ONE_MINUS_SRC_ALPHA     ,
        ONE_MINUS_SRC_COLOR      = osg::BlendFunc::ONE_MINUS_SRC_COLOR     ,
        SRC_ALPHA                = osg::BlendFunc::SRC_ALPHA               ,
        SRC_ALPHA_SATURATE       = osg::BlendFunc::SRC_ALPHA_SATURATE      ,
        SRC_COLOR                = osg::BlendFunc::SRC_COLOR               ,
        CONSTANT_COLOR           = osg::BlendFunc::CONSTANT_COLOR          ,
        ONE_MINUS_CONSTANT_COLOR = osg::BlendFunc::ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA           = osg::BlendFunc::CONSTANT_ALPHA          ,
        ONE_MINUS_CONSTANT_ALPHA = osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA,
        ZERO                     = osg::BlendFunc::ZERO                    ,
    };
    Q_ENUM(BlendFuncMode)

    bool enabled = false;
    BlendFuncMode source = ONE;
    BlendFuncMode destination = ZERO;
    BlendFuncMode sourceAlpha = ONE;
    BlendFuncMode destinationAlpha = ZERO;
    QColor color;
};

class Item3d : public QObject, public osg::MatrixTransform
{
    Q_OBJECT

    Q_PROPERTY(BlendOptions blend READ blendOptions WRITE setBlendOptions)

public:
    enum DraggerType {
        TrackballDragger,
        TranslateDragger
    };
    Item3d();

    void Select();
    void Deselect();

    void AddDragger(DraggerType type = TranslateDragger);
    void RemoveDragger();

    virtual void ContextMenu(QMenu *menu, const QVector3D &p);
    BlendOptions blendOptions() const;

public slots:
    void setBlendOptions(BlendOptions blend);

protected:
    osg::observer_ptr<osgManipulator::Dragger> dragger;
    BlendOptions m_blend;
};
