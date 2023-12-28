/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 伤点信息列表对话框
* Filename: ScanData.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-10-14
*/
#ifndef DETECTLISTDLG_H
#define DETECTLISTDLG_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QStandardItemModel>
#include <QHeaderView>
#include "DataDefine.h"

class DetectListDlg : public QDialog
{
	Q_OBJECT
public:
	DetectListDlg(QWidget *parent);
	~DetectListDlg();
signals:
	void SignalToCscanDrawDashRect(int id);  //信号通知Cscan画选中虚框
public slots:
	void UpdateTableView(); //更新伤点信息
	void OnDoubleClicked(const QModelIndex& Index);  //响应视图双击事件

private:
	QPushButton *m_pQuitButton;
public:
	const QVector<DLPointInfo>* m_pvHeaderRect;     //DelectListDlg 列的相关信息
	QTableView *m_pTableView;
	QStandardItemModel *m_pModel;
};



#endif // DETECTLISTDLG_H
