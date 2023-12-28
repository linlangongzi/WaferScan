#pragma once

#include <QDialog>
#include <Motion/MotionManager.h>

namespace Ui {
class DialogNamedPositionList;
}

class DialogNamedPositionList : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(bool selectionMode READ isSelectionMode WRITE setSelectionMode)

public:
    explicit DialogNamedPositionList(QWidget *parent = 0);
    ~DialogNamedPositionList();
    NamedPosition *selectedNamedPosition() const;
    double coordinate(int axis) const;
    bool isSelectionMode() const;

public slots:
    void setSelectionMode(bool enable);

private slots:
    void on_actionAdd_triggered();              //!< 添加功能
    void on_actionRemove_triggered();           //!< 删除功能
    void on_actionModify_triggered();           //!< 修改功能
    void on_actionMove_triggered();             //!< 移动到此位置功能
    void on_actionLoad_triggered();             //!< 设为上料位置
    void customContextMenu(const QPoint &point);
    void onDoubleClicked();

private:
    void initModel();

private:
    Ui::DialogNamedPositionList *ui;
    bool m_selectionMode = false;
};
