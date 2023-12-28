#pragma once

#include <QDialog>
#include <Ultrasonic/Material.h>

class Material;
class QSortFilterProxyModel;

namespace Ui {
class DialogMaterial;
}

class DialogMaterial : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(Material currentMaterial READ currentMaterial WRITE setCurrentMaterial NOTIFY currentMaterialChanged)
    Q_PROPERTY(bool isEditable READ isEditable WRITE setEditable NOTIFY editableChanged)

public:
    explicit DialogMaterial(QWidget *parent = nullptr);
    ~DialogMaterial() override;
    Material currentMaterial() const;
    bool isEditable() const;

public slots:
    void setCurrentMaterial(const Material &currentMaterial);
    void setEditable(bool isEditable);

signals:
    void currentMaterialChanged(Material material);
    void editableChanged(bool isEditable);

private slots:
    void on_add_clicked();
    void on_remove_clicked();
    void on_filter_textChanged(const QString &arg1);

private:
    Ui::DialogMaterial *ui;
    bool m_editable = false;
    QSortFilterProxyModel *m_sortModel = nullptr;
};
