#include "VectorEditor.h"
#include "ui_VectorEditor.h"
#include "MetaType.h"
#include "../Util.h"
#include <QBoxLayout>

static const bool metaTypeVector3D = MetaType::registerEditor(qMetaTypeId<QVector3D>(), []() {
    auto *c = new VectorEditor;
    c->setFixedLength(3);
    return c;
}, "vector3D");

static const bool metaTypeVector4D = MetaType::registerEditor(qMetaTypeId<QVector4D>(), []() {
    auto *c = new VectorEditor;
    c->setFixedLength(4);
    return c;
}, "vector4D");

VectorEditor::VectorEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VectorEditor)
{
    ui->setupUi(this);
}

VectorEditor::~VectorEditor()
{
    delete ui;
}

QVector<qreal> VectorEditor::vector() const
{
    QVector<qreal> vec;
    for (auto control: controls) {
        vec << control.second->value();
    }
    return vec;
}

QVector3D VectorEditor::vector3D() const
{
    auto vec = vector();
    return QVector3D(vec.value(0), vec.value(1), vec.value(2));
}

QVector4D VectorEditor::vector4D() const
{
    auto vec = vector();
    return QVector4D(vec.value(0), vec.value(1), vec.value(2), vec.value(3));
}

int VectorEditor::fixedLength() const
{
    return m_fixedLength;
}

QStringList VectorEditor::names() const
{
    return m_names;
}

void VectorEditor::setVector(const QVector<qreal> &vector)
{
    auto vec = vector;
    if (m_fixedLength > 0) {
        vec.resize(m_fixedLength);
    }
    if (vec.size() != controls.size()) {
        refreshLength(vec.size());
    }
    for (int index = 0; index < controls.size(); ++index) {
        QDoubleSpinBox *box = controls[index].second;
        box->setValue(vec.value(index));
    }
}

void VectorEditor::setVector3D(const QVector3D &vector3D)
{
    setFixedLength(3);
    setNames({"X", "Y", "Z"});
    setVector({vector3D.x(), vector3D.y(), vector3D.z()});
}

void VectorEditor::setVector4D(const QVector4D &vector4D)
{
    setFixedLength(4);
    setNames({"X", "Y", "Z", "W"});
    setVector({vector4D.x(), vector4D.y(), vector4D.z(), vector4D.w()});
}

void VectorEditor::setFixedLength(int fixedLength)
{
    m_fixedLength = fixedLength;
}

void VectorEditor::setNames(const QStringList &names)
{
    m_names = names;
}

void VectorEditor::spinBoxChanged()
{

}

void VectorEditor::refreshLength(int length)
{
    resizeContainer(length, controls, [=]() {
        int newIndex = controls.size();
        auto *box = new QDoubleSpinBox(this);
        auto *label = new QLabel(m_names.value(newIndex), this);
        connect(box, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &VectorEditor::spinBoxChanged);
        ui->formLayout->addRow(label, box);
        return qMakePair(label, box);
    }, [](const QPair<QLabel *, QDoubleSpinBox *> control) {
        delete control.first;
        delete control.second;
    });
}
