#include "DialogGadgetEditor.h"
#include "ui_DialogGadgetEditor.h"
#include "../Util.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QLoggingCategory>
#include <QSequentialIterable>
#include <QShortcut>
#include <QSpinBox>

static Q_LOGGING_CATEGORY(debug, "util.gadget.edit");
static const char USER_PROP[] = "UserProperty";

DialogGadgetEditor::DialogGadgetEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGadgetEditor)
{
    ui->setupUi(this);

    connect(new QShortcut(QString("Ctrl+C"), this), &QShortcut::activated, this, [=]() {
        writeClipboard(m_value);
    });
    connect(new QShortcut(QString("Ctrl+V"), this), &QShortcut::activated, this, [=]() {
        setValue(readClipboard(m_value.userType()));
    });
}

DialogGadgetEditor::~DialogGadgetEditor()
{
    delete ui;
}

QVariant DialogGadgetEditor::value() const
{
    return m_value;
}

void DialogGadgetEditor::setValue(const QVariant &value)
{
    if (!value.isValid()) {
        qCDebug(debug) << "Can't create gadget editor: Invalid value";
        return;
    }

    if (!(QMetaType::typeFlags(value.userType()) & QMetaType::IsGadget)) {
        qCDebug(debug) << "Can't create gadget editor: type" << value.typeName() << "is not gadget";
        return refreshUi(QMetaType::UnknownType);
    }

    bool typeChanged = (value.userType() != m_value.userType());

    m_value = value;

    if (typeChanged) {
        refreshUi(value.userType());
    } else {
        for (QWidget *control: controls) {
            const QString userProperty = control->property(USER_PROP).toString();
            QVariant propertyValue = MetaType::readGadgetProperty(value, control->objectName());
            control->setProperty(qPrintable(userProperty), propertyValue);
        }
    }
}

void DialogGadgetEditor::controlChanged()
{
    QVariant value = m_value;
    for (QWidget *control: controls) {
        const QString userProperty = control->property(USER_PROP).toString();
        QVariant propertyValue = control->property(qPrintable(userProperty));
        MetaType::writeGadgetProperty(value, control->objectName(), propertyValue);
    }

    if (value == m_value) {
        return;
    }
    m_value = value;
    emit valueChanged(value);
}

void DialogGadgetEditor::refreshUi(int typeId)
{
    qDeleteAll(controls);
    controls.clear();

    const QMetaObject *meta = QMetaType::metaObjectForType(typeId);
    if (!meta) {
        return;
    }
    MetaType gadgetMeta = MetaType::fromType(typeId);

    const QMetaMethod slot = staticMetaObject.method(staticMetaObject.indexOfSlot("controlChanged()"));
    if (!slot.isValid()) {
        qCCritical(debug) << "controlChanged not exist for type" << meta->className();
    }
    Q_ASSERT(slot.isValid());
    for (int index = 0; index < meta->propertyCount(); ++index) {
        const QMetaProperty prop = meta->property(index);
        auto info = MetaType::fromType(prop.userType());

        const QString propertyName = info.editorPropertyName();
        QWidget *widget = info.createEditor();
        if (!widget) {
            qCCritical(debug) << "Can't create editor for unsupported type" << prop.typeName();
            continue;
        }

        widget->setProperty(USER_PROP, propertyName);
        widget->setProperty(qPrintable(propertyName), MetaType::readGadgetProperty(m_value, prop.name()));

        const QMetaObject *meta = widget->metaObject();
        int propertyIndex = meta->indexOfProperty(qPrintable(propertyName));
        if (propertyIndex >= 0) {
            const QMetaProperty widgetProp = meta->property(propertyIndex);
            bool success = connect(widget, widgetProp.notifySignal(), this, slot);
            if (!success) {
                qCCritical(debug) << "Can't connect property" << propertyName << "for editor" << widget << "with type" << prop.typeName();
            }
        } else {
            qCCritical(debug) << "Can't find property" << propertyName << "for editor" << widget << "with type" << prop.typeName();
        }
        widget->setObjectName(prop.name());

        QString nameTr = gadgetMeta.gadgetKeyText(prop.name());
        ui->mainLayout->addRow(nameTr, widget);
        controls << widget;
    }
    emit valueChanged(m_value);
}
