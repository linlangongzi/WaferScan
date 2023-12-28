#pragma once

#include <QWidget>
#include <Ultrasonic/UTData.h>

namespace Ui {
class DataInfoWidget;
}

struct lua_State;
class QTreeWidgetItem;
class QStandardItemModel;
class QLabel;

class DataInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataInfoWidget(QWidget *parent = 0);
    ~DataInfoWidget();

public slots:
    void setDataFrame(const UTDataFrame &frame);
    void setDataHeader(const UTDataHeader &header);
//    void setSetup(const UTSetup &setup);
    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *) override;

private slots:
    void onLicenseUpdated();
    void OnPlatformValueChanged(const QString &key, const QJsonValue &value);
    void refreshAxisList();
    void refreshFrame();
    void refreshMathList();
    void on_addMath_clicked();
    void on_removeMath_clicked();
    void on_math_doubleClicked(const QModelIndex &index);
    void on_sampleButton_clicked();

private:
    void InitUI();
    void setNoData();

private:
    Ui::DataInfoWidget *ui;
    QList<QLabel *> axisCoords;
    UTDataFrame lastFrame;
    int currentHeaderId = -1;
    bool needRefresh = false;
    lua_State *L = nullptr;
};
