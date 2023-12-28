/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 数据查询界面
* Filename: ScanData.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-10-09
*/
#ifndef SCANDATA_H
#define SCANDATA_H

#include <QDialog>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QDateEdit>
#include <QTimeEdit>
#include <QSqlQuery>
class ScanData :public QDialog
{
	Q_OBJECT
public:
	ScanData(QWidget *parent);
	~ScanData(void);
public:
	void InitDialog();
	void translate();
private:
	void PlateNoQuery(); //按序号查询
	void DataPlateNumQuery(); //按钢板号查询
	void DataTimeQuery(); // 按日期查询
	void UpdateTableView(QSqlQuery &query); //更新视图
	void SetRowToTableView(int row);
signals:
	void SignalDrawSelectedFile(QList<QString> argList); //双击TableView的发出的信号 画图
	void SignalUpdateChannelDock(QList<QString> argList); //双击TableView的发出的信号 更新界面信息

public slots:
	void OnRadioSwitch(bool flag);
	void OnSearchButtom();
	void OnDoubleClicked(const QModelIndex& Index);
	void OnExit(); //退出按钮
	void OnNextDrawingImage();  //响应向前按钮
	void OnPreDrawingImage();   //响应向后按钮
public:
	QGroupBox *m_gTopGroup;
	QRadioButton *m_rbOrderCheck;
	QRadioButton *m_rbPlateNCheck;
	QRadioButton *m_rbDateCheck;
	QGroupBox *m_gOrder;
	QLineEdit *m_leOrder;
	QGroupBox *m_gPlateNM;
	QLineEdit *m_lePlateNM;
	QPushButton *m_pbCheck;
	QGroupBox *m_gDate;
	QLabel *m_lbStartTime;
	QLabel *m_lbEndTime;
	QDateEdit *m_cbStartTime;
	QDateEdit *m_cbEndTime;
	QTimeEdit *m_spStartTime;
	QTimeEdit *m_spEndTime;
	QLabel *m_lbCurrentSelect;
	QTableView *m_tvDetial;
	QStandardItemModel *m_pDetialInfoModel;
	QPushButton *m_pbExit;
	int  m_nCurrentIndex; //当前选中行号

	//chenyuzhu add start on 2014.5.13
	QGroupBox *m_gShibanGroup;
	QRadioButton *m_rbShibanCheck;
	QRadioButton *m_rbUnShibanCheck;
	//chenyuzhu add end on 2014.5.13
	QGroupBox *m_gGongyiGroup;
	QRadioButton *m_rbGongyiCheck;
};


#endif SCANDATA_H
