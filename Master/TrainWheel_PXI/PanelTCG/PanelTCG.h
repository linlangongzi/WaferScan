#ifndef PANELTCG_H
#define PANELTCG_H

#include <QWidget>
#include <QDebug>
#include <QModelIndex>
#include "PanelTCGEdit.h"
#include <QMenu>
#include "ConfigControl/ConfigControlInterface.h"
#include <QJsonArray>
#include "ConfigObject/UTSetup.h"

namespace Ui {
class PanelTCG;
}

//class UTSetup;
class PanelTCGEdit;
class QStandardItem;
class QStandardItemModel;

class PanelTCG : public QWidget, public ConfigControlInterface
{
    Q_OBJECT
    typedef QList<QStandardItem *> StdItemList;
public:
    explicit PanelTCG(QWidget *parent = 0);
    ~PanelTCG();

    void ConnectTo(QObject *obj);
    void Disconnect(QObject *obj);

public:
    void InitModel();
    void InitUi();
    void InsertTCGListFromUTSetup(int index);
    StdItemList GetGateStandardItemList(int index);

public:
    int currentIndex;
    double currentTime;
    double currentGain;

private slots:
    void on_addBtn_clicked();
    void on_modifyBtn_clicked();
    void on_deleteBtn_clicked();
    void AddTCGPoint();
    void ModifyTCGPoint();
    void DeleteTCGPoint();
    void customContextMenu(const QPoint &pos);
    void ModifyModelTCGPoint(const QModelIndex &index);
    void UpdateModel(const QString &key, const QJsonValue &value);
    void on_TCGTableView_clicked(const QModelIndex &index);

private:
    Ui::PanelTCG *ui;
    QStandardItemModel *TCGItemModel;
    static const int tableViewWidth;
    QJsonArray TCGJsonArray;
    QPointer<UTSetup> setup;
    QMenu *menu;
};

#endif // PANELTCG_H
