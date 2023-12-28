#include "Material.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <allrising/Util.h>
#include <Ultrasonic/UltrasonicManager.h>

Q_DECL_UNUSED static bool typeRegistered = Material::registerType();

Material Material::fromJson(const QJsonValue &json)
{
    QJsonObject obj = json.toObject();
    Material m;
    if (obj.contains("ID")) {
        m.legacyId = obj["ID"].toString().toLongLong();
        m.longitudinalVelocity = obj["MaterialVelocity"].toDouble();
        m.name = obj["MaterialName"].toString();
    } else {
        JsonToGadget(json, staticMetaObject, &m);
    }
    if (m.id.isNull()) {
        m.id = QUuid::createUuid();
    }
    return m;
}

QString Material::description() const
{
    return tr("%1 (%2 m/s)").arg(name).arg(longitudinalVelocity);
}

QString Material::stateText() const
{
    MetaType meta = MetaType::fromType<Material::State>();
    return meta.gadgetKeyText(EnumToKey(state));
}

bool Material::registerType()
{
    bool ret = registerJsonConverter<Material>();

    ret |= QMetaType::registerConverter(&Material::description);

    MetaType meta(qMetaTypeId<Material>());

    meta.setTr("name", tr("名称"));
    meta.setTr("speed", tr("声速"));

    MetaType::setTr(Solid, tr("固态"));
    MetaType::setTr(Liquid, tr("液态"));
    MetaType::setTr(Gas, tr("气态"));

    MetaType::registerMeta(meta);

    return ret;
}

qreal Material::acousticImpedance() const
{
    return longitudinalVelocity * density;
}

QVariant MaterialItem::data(int column, int role) const
{
    if (role == MaterialModel::UuidRole) {
        return material.id;
    }
    if (column == ColUuid && role == Qt::DisplayRole) {
        return material.id;
    }
    if (column == ColLegacyId && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return material.legacyId;
    }
    if (column == ColName && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return material.name;
    }
    if (column == ColState) {
        if (role == Qt::DisplayRole) {
            return material.stateText();
        } else if (role == Qt::EditRole) {
            return QVariant::fromValue(material.state);
        }
    }
    if (column == ColDensity) {
        if (role == Qt::DisplayRole) {
            return tr("%1 kg/m³").arg(material.density);
        } else if (role == Qt::EditRole) {
            return material.density;
        }
    }
    if (column == ColAcousticImpedance && role == Qt::DisplayRole) {
        return tr("%1 Pa·s/m³").arg(material.acousticImpedance());
    }
    qreal velocity = 0;
    if (column == ColSurfaceVelocity) {
        velocity = material.surfaceVelocity;
    }
    if (column == ColShearVelocity) {
        velocity = material.shearVelocity;
    }
    if (column == ColLongitudinalVelocity) {
        velocity = material.longitudinalVelocity;
    }
    if (role == Qt::DisplayRole) {
        return tr("%1 m/s ").arg(velocity);
    } else if (role == Qt::EditRole) {
        return velocity;
    }
    return QVariant();
}

bool MaterialItem::setData(int column, const QVariant &data, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }
    if (column == ColName) {
        material.name = data.toString();
    } else if (column == ColLongitudinalVelocity) {
        material.longitudinalVelocity = data.toReal();
    } else if (column == ColShearVelocity) {
        material.shearVelocity = data.toReal();
    } else if (column == ColState) {
        material.state = Material::State(data.toInt());
    } else if (column == ColSurfaceVelocity) {
        material.surfaceVelocity = data.toReal();
    } else if (column == ColDensity) {
        material.density = data.toReal();
    } else {
        return false;
    }
    return true;
}

Qt::ItemFlags MaterialItem::flags(int column) const
{
    auto flags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    if (column == ColUuid || column == ColLegacyId) {
        flags |= Qt::ItemIsEnabled;
    }
    if (column == ColName
            || column == ColState
            || column == ColLongitudinalVelocity
            || column == ColDensity) {
        flags |= Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
    if ((column == ColShearVelocity || column == ColSurfaceVelocity) && material.state == Material::Solid) {
        flags |= Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
    return flags;
}

MaterialModel::MaterialModel(QObject *parent) :
    Utils::TreeModel<MaterialRootItem, MaterialItem>(parent)
{
    setHeader({tr("UUID"), tr("Id"), tr("名称"), tr("形态"), tr("密度"), tr("纵波声速"), tr("横波声速"), tr("表面波声速"), tr("声阻抗")});
}

void MaterialModel::load(const QList<Material> &materials)
{
    clear();
    for (const auto &material : materials) {
        auto *item = new MaterialItem;
        item->material = material;
        rootItem()->appendChild(item);
    }
}

QList<Material> MaterialModel::save() const
{
    QList<Material> data;
    forItemsAtLevel<1>([&] (MaterialItem *item) {
        data << item->material;
    });
    return data;
}

Material MaterialModel::material(const QUuid &uuid)
{
    auto *item = findItemAtLevel<1>([uuid] (MaterialItem *materialItem) {
        return materialItem->material.id == uuid;
    });
    return (item != nullptr) ? item->material : Material();
}

Material MaterialModel::materialFromLegacyId(int id)
{
    auto *item = findItemAtLevel<1>([id] (MaterialItem *materialItem) {
        return materialItem->material.legacyId == id;
    });
    return (item != nullptr) ? item->material : Material();
}
