#include "Item3d.h"
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/TranslateAxisDragger>
#include <QMenu>
#include <QVector3D>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <utils/MetaType/DialogGadgetEditor.h>
#include "OsgContainer.h"
#include "qtosgUtils.h"
#include <allrising/Util.h>

using namespace osg;

REGISTER_JSON_CONVERTER(BlendOptions);

Item3d::Item3d()
{
    getOrCreateStateSet()->setRenderingHint(StateSet::TRANSPARENT_BIN);
}

/*! 节点被选中，添加dragger并高亮 */
void Item3d::Select()
{
    AddDragger(TranslateDragger);
}

/*! 节点撤销选中，删除dragger，解除高亮 */
void Item3d::Deselect()
{
    RemoveDragger();
}

/*! 添加dragger */
void Item3d::AddDragger(DraggerType type)
{
    RemoveDragger();
    ref_ptr<osgManipulator::Dragger> dragger;
    if (type == TrackballDragger) {
       dragger = new osgManipulator::TrackballDragger();
    } else {
        dragger = new osgManipulator::TranslateAxisDragger();
    }
    dragger->setupDefaultGeometry();
    float scale = getBound().radius() * 1.2;
    dragger->setMatrix(Matrix::scale(scale, scale, scale) * Matrix::translate(getBound().center()));
    dragger->addTransformUpdating(this);
    dragger->setHandleEvents(true);
//    dragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);      //按下ctrl键方可使用Dragger
    addChild(dragger);

    this->dragger = dragger;
    qDebug() << "Succeed to add dragger";
}

/*! 删除Dragger */
void Item3d::RemoveDragger()
{
    if (dragger.valid()) {
        removeChild(dragger.get());
    }
}

BlendOptions Item3d::blendOptions() const
{
    return m_blend;
}

void Item3d::setBlendOptions(BlendOptions blend)
{
    for (unsigned int index = 0; index < getNumChildren(); ++index) {
        StateSet *state = getChild(index)->getOrCreateStateSet();
        ref_ptr<BlendFunc> blendFunc = dynamic_cast<BlendFunc *>(state->getAttribute(StateAttribute::BLENDFUNC));
        ref_ptr<BlendColor> blendColor = dynamic_cast<BlendColor *>(state->getAttribute(StateAttribute::BLENDCOLOR));
        state->removeAttribute(blendFunc);
        state->removeAttribute(blendColor);
        if (!blendColor) {
            blendFunc = new BlendFunc;
            blendColor = new BlendColor;
        }

        blendFunc->setSource(blend.source);
        blendFunc->setDestination(blend.destination);
        blendFunc->setSourceAlpha(blend.sourceAlpha);
        blendFunc->setDestinationAlpha(blend.destinationAlpha);

        blendColor->setConstantColor(toVec4(blend.color));
        state->setAttributeAndModes(blendColor, StateAttribute::OVERRIDE | StateAttribute::ON);
        state->setAttributeAndModes(blendFunc, StateAttribute::OVERRIDE | StateAttribute::ON);
        state->setRenderingHint(StateSet::TRANSPARENT_BIN);
        if (blend.enabled) {
            state->setMode(GL_BLEND, StateAttribute::ON);
        } else {
            state->setMode(GL_BLEND, StateAttribute::OFF);
        }
    }

    m_blend = blend;
}

void Item3d::ContextMenu(QMenu *menu, const QVector3D &p)
{
    Q_UNUSED(p);

    menu->addAction(tr("编辑渲染参数"), [=]() {
        DialogGadgetEditor dialog(qApp->activeWindow());
        BlendOptions original = blendOptions();
        dialog.setValue(original);
        connect(&dialog, &DialogGadgetEditor::valueChanged, [&]() {
            setBlendOptions(dialog.value<BlendOptions>());
        });
        if (dialog.exec() != QDialog::Accepted) {
            setBlendOptions(original);
        }
    });

    menu->addAction(tr("删除"), [this]() {
        for (Group *parent: getParents()) {
            parent->removeChild(this);
        }
    });

    menu->addAction(tr("移动"), [=]() {
        AddDragger(TranslateDragger);
    });
    menu->addAction(tr("旋转"), [=]() {
        AddDragger(TrackballDragger);
    });
}
