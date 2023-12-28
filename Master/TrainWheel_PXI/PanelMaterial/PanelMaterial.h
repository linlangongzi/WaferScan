#ifndef PANELMATERIAL_H
#define PANELMATERIAL_H

#include <QWidget>
#include <ConfigObject/Material.h>
#include <QStandardItemModel>
#include "ConfigObject/ConfigManager.h"
#include "PanelMaterialEdit.h"
#include <QJsonDocument>
#include <QMessageBox>
#include <QMenu>
#include <QVector>
#include <QDoubleValidator>
#include <QDialog>
#include <ConfigObject/UTSetup.h>

namespace Ui {
class PanelMaterial;
}
/**
 * @brief The PanelMaterial class
 * 材料声速表的显示
 * 包含增加新的材料、删除一种材料、选择一种材料为当前使用的材料
 */
class PanelMaterial : public QWidget
{
    Q_OBJECT
    typedef QList<QStandardItem *> StandardItemRow;
    enum TableViewOrder
    {
        MATERIAL_ID = 0,
        MATERIAL_NAME = 1,
        MATERIAL_VELOCITY = 2,
    };

public:
    explicit PanelMaterial(QWidget *parent = 0);
    ~PanelMaterial();
    void SetUI();
    Material *GetSelection();

signals:
    void SelectSubmit();

public slots:
    void SetSelectMode(bool selectMode);
    void customContextMenu(const QPoint &pos);
    void ModifyMaterial(const QModelIndex& ModelIndex);
//    void SetRowHeight();
    void SelectMaterial();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_addMaterial_clicked();
    void on_deleteMaterial_clicked();

private:
    void InitUI();
    void InitModel();
    void InsertMaterial(Material *material);

private:
    Ui::PanelMaterial *ui;
    QDoubleValidator validator;
    QStandardItemModel *materialItemModel;
    typedef QPointer<ConfigManager> ConfigManagerPointer;
    QMenu *menu;
//    static const int COLUMN_WIDTH;
//    static const int COLUMN_HEIGH;

    static const double MAX_VELOCITY;
    static const double MIN_VELOCITY;
};

#endif // PANELMATERIAL_H
