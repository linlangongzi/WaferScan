#ifndef DIALOGMATERIAL_H
#define DIALOGMATERIAL_H

#include <QDialog>

class Material;

namespace Ui {
class DialogMaterial;
}

class DialogMaterial : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMaterial(QWidget *parent = 0);
    ~DialogMaterial();

    Material *GetMaterial();

private:
    Ui::DialogMaterial *ui;
};

#endif // DIALOGMATERIAL_H
