#pragma once

#include "UltrasonicGlobal.h"
#include <QCoreApplication>
#include <QUuid>
#include <allrising/MetaType/MetaType.h>
#include <utils/treemodel.h>
#include <QCoreApplication>

class ULTRASONIC_EXPORT Material
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(Material)

    Q_PROPERTY(QUuid id MEMBER id)
    Q_PROPERTY(int legacyId MEMBER legacyId)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(State state MEMBER state)
    Q_PROPERTY(qreal longitudinalVelocity MEMBER longitudinalVelocity)          /*!< 纵波声速. */
    Q_PROPERTY(qreal shearVelocity MEMBER shearVelocity)                        /*!< 横波声速. 仅有固体能传播横波, 其他状态无法传播横波. */
    Q_PROPERTY(qreal surfaceVelocity MEMBER surfaceVelocity)                    /*!< 表面波声速. 仅有固体能传播表面波, 其他状态无法传播表面波. */
    Q_PROPERTY(qreal density MEMBER density)                                    /*!< 密度  单位kg/m^3*/

public:
    enum State {
        Solid,
        Liquid,
        Gas,
    };
    Q_ENUM(State)

    inline bool isValid() const {return !id.isNull();}
    static Material fromJson(const QJsonValue &json);
    QString description() const;
    QString stateText() const;
    static bool registerType();
    qreal acousticImpedance() const;

    DECLARE_COMPARE_OPERATORS(Material)

    QUuid id;
    int legacyId = 0;
    QString name;
    State state = Solid;
    qreal longitudinalVelocity = 1000;
    qreal shearVelocity = Q_SNAN;
    qreal surfaceVelocity = Q_SNAN;
    qreal density = Q_SNAN;
};

class MaterialItem : public Utils::TreeItem
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(MaterialItem)

public:
    enum Columns {
        ColUuid,
        ColLegacyId,
        ColName,
        ColState,
        ColDensity,
        ColLongitudinalVelocity,
        ColShearVelocity,
        ColSurfaceVelocity,
        ColAcousticImpedance,
    };
    Q_ENUM(Columns)

    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    Material material;
};

using MaterialRootItem = Utils::TypedTreeItem<MaterialItem>;

class MaterialModel : public Utils::TreeModel<MaterialRootItem, MaterialItem>
{
    Q_OBJECT

public:
    enum Roles {
        UuidRole = Qt::UserRole,
    };
    Q_ENUM(Roles)

    MaterialModel(QObject *parent = nullptr);

    void load(const QList<Material> &materials);
    QList<Material> save() const;

    Material material(const QUuid &uuid);
    Material materialFromLegacyId(int id);

signals:
    void isEditableChanged();

private:
    bool isEditable = false;
};
