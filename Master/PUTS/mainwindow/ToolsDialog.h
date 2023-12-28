/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 工具面板
* Filename: ToolsDialog.h
*
* @author        DongQi
* @version       1.00         Date: 2013-05-30
*/

#ifndef TOOLSDIALOG_H
#define TOOLSDIALOG_H


#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QSettings>
#include <QDir>
#include <QTextEdit>
#include "InjPointControl.h"
#include "LogViewWidget.h"
#include "PrintReportControl.h"
#include "OtherControlWidget.h"
#include "UserManager.h"

class CToolsDialogInfo
{
public:
	//伤点控制详细面板信息封装
	bool m_bTopEdgePicture;			// 上侧边缘伤点视图
	bool m_bBottonEdgePicture;		// 下侧
	bool m_bLeftEdgePicture;		// 左侧
	bool m_bRightEdgePicture;		// 右侧
	bool m_bThickNessPicture;		// 厚度图
	bool m_bFrontPicture;			// 正视图
	bool m_bSidePicture;			// 侧视图
	bool m_bWholePicture;			// 总体视图

	bool m_bCentreCrosingLine;		// 中心十字线 
	bool m_bInjPointRangeLine;		// 伤点范围线
	bool m_bInjPointPath;			// 伤点路径

	bool m_bInjPointFilter;			// 伤点过滤
	bool m_bFilterBiger;			// 伤点过滤 
	bool m_bFilterSmaller;			// 伤点过滤
	int  m_nFilterBigerNum;
	int  m_nFilterSmallerNum;

	float m_fHeadDepthFilterNum;	// 头端深度过滤
	float m_fEndDepthFilterNum;		// 尾端深度过滤
	float m_fEdgeDepthFilterNum;	// 边缘深度过滤
	float m_fBodyDepthFilterNum;	// 主体深度过滤

	bool m_bNextDefectDistant;		//相邻缺陷距离
	int  m_nDefectDistaneSmaller;


	//打印报表信息封装
	bool m_bDependOnMInput;  //人工输入数据为准
	bool m_bPrintInjPointDetailPic; //打印伤点详细图

	//其他控制窗口的数据封装
	QString m_strAddress;
	QString m_strUserName;
	QString m_strPwd;
};




class CToolsDialog : public QDialog
{
	Q_OBJECT

public:
	CToolsDialog(CToolsDialogInfo *pToolsDialogInfo,QWidget *parent = NULL);
	~CToolsDialog(void);

private:
	void InitDialog(void);
	void translate(void);
	void ReadToolDialogInfo(void);

public slots:
	void OnListWidgetChange(int Index);		//响应ListWidget的Change事件
	void OnOK(void);
	void OnCancel(void);					//响应取消事件
	void OnDetectingLogFile();				//打开探测日志
	void OnCidentLogFile();					//打开事件日志chenyuzhu add on 2014-6-24

public:
	QGridLayout *m_pMainGridLayout;
	QHBoxLayout *m_pButtonLayout;
	QHBoxLayout *m_pButtonHLayout;
	
	QPushButton *m_pOK;
	QPushButton *m_pCancel;
	QPushButton *m_pRestoreDefault;
    QPushButton *m_pLilunBiaoding;

	QLabel *m_pDetailedPanel;
	QLabel *m_plblCategory;
	QStackedWidget *m_pSWDetailedPanel;
	CInjPointControl *m_pInConPointPage;		//伤点控制
	CLogViewWidget *m_pLogViewPage;				//日志查看
	CPrintReportControl *m_pPrintControlPage;	//打印报表控制
	COtherControlWidget *m_pOtherControlPage;	//其他控制
	UserManager         *m_pUserManager;		//用户管理
	//QWidget *m_pHelpPage;						//帮助
	QListWidget *m_plistCategory;				//类别视图 
	QListWidgetItem *m_pItemInConPoint;			//伤点控制Item
	QListWidgetItem *m_pItemLogView;			//日志查看Item
	QListWidgetItem  *m_pItemPrintControl;		//打印报表控制Item
	QListWidgetItem  *m_pItemOtherContro;		//其他控制Item
//	QListWidgetItem  *m_pItemHelp;				//帮助Item

	QListWidgetItem  *m_pItemUserManage;		//用户管理Item

	CToolsDialogInfo *m_pTToolsDialogInfo;		//工具栏对话框信息
	QSettings *m_pSettings;						//配置文件对象
	QString m_IniFlieName;						//配置文件名

	QTextEdit   *m_pDetectTextEdit;				//显示检测日志
	QTextEdit   *m_pCidentLogTextEdit;			//显示事件日志chenyuzhu add on 2013-7-24

signals:
	void UpdateMainWindowOnCancel(void);
	void SignalOnCidentLog(void);				//事件日志chenyuzhu add on 2013-7-24
	void SignalToUpdateCscan(void);				//chenyuzhu add on 2014-7-24
  
};

#endif //TOOLSDIALOG_H
