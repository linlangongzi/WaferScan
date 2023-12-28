/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 工艺设置主界面
* Filename: TechnologySetDialog.h
*
* @author        DongQi
* @version       1.00         Date: 2013-04-27
*/

#ifndef TECHNOLOGYSETTINGDIALOG_H
#define TECHNOLOGYSETTINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSqlDatabase>
#include <QGridLayout>
#include <QGroupBox>
#include <QKeyEvent>

class CPlateInfo;
class CTechnologySetDialog : public QDialog
{
	Q_OBJECT
public:
	CTechnologySetDialog(QMap<QString,int> &FlagMap,CPlateInfo&  PlateInfo,QSqlDatabase *pDatabaseConnection = NULL,QWidget *parent = NULL);
	~CTechnologySetDialog(void);
private:
	QSqlDatabase *m_pDatabaseConnection;
	QLabel *m_plblName;
	QComboBox *m_pcbName;
	QPushButton *m_pbgOk;
	QPushButton *m_pbgCancel;
	CPlateInfo  *m_pPlateInfo;
	QMap<QString,int> *m_pFlagMap;  //标示的集合
    QGridLayout *m_pMainGridLayout; //窗口布局 
public slots:
	void SettingConfig(void);                    //配置文件处理
	void DestroyTechnologySettingDialog(void);   //销毁对话框
private:
	void InsertDataIntoCbName(void);   
protected:
	void keyPressEvent(QKeyEvent *e);   //过滤ESC
};

#endif //TECHNOLOGYSETTINGDIALOG_H