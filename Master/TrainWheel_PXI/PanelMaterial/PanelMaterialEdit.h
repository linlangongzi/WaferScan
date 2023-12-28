#ifndef PANELMATERIALEDIT_H
#define PANELMATERIALEDIT_H

#include <QDialog>
#include <QDoubleValidator>
#include <QMessageBox>
#include <QPushButton>

namespace Ui {
class PanelMaterialEdit;
}

class PanelMaterialEdit : public QDialog
{
    Q_OBJECT

public:
    explicit PanelMaterialEdit(QWidget *parent = 0);
    ~PanelMaterialEdit();
public:
    QString GetMaterialName();
    double GetMaterialVelocity();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::PanelMaterialEdit *ui;
    QString MaterialName;
    double  MaterialVelocity;
    QDoubleValidator *dValidator;
    const static double MAX_VELOCITY;
    const static double MIN_VELOCITY;
};

#endif // PANELMATERIALEDIT_H
