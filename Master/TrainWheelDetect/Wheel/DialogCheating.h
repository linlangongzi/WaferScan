#ifndef DIALOGCHEATING_H
#define DIALOGCHEATING_H

#include <QDialog>

static const char K_CHEAT_FILE_PATH[] = "CP";                                   /*!< 当前选中的文件 */
static const char K_CHEAT_FILE_LIST_PATH[] = "CL";                              /*!< 文件列表 */
static const char K_CHEAT_FILE_NOISE[] = "CN";
static const char K_CHEAT_AFTER_SCAN[] = "CE";
static const char K_CHEAT_SHORTCUT[] = "CS";
static const char K_CHEAT_EDIT[] = "CA";

namespace Ui {
class DialogCheating;
}

class DialogCheating : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCheating(QWidget *parent = 0);
    ~DialogCheating();

    void accept() override;

private slots:
    void on_addPath_clicked();
    void on_removePath_clicked();

private:
    Ui::DialogCheating *ui;
};

#endif // DIALOGCHEATING_H
