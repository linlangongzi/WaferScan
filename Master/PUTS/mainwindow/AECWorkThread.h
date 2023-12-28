/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 线程类
* Filename: AECWorkThread.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-06-06
*/


#ifndef _AECWORKTHREAD_H_
#define _AECWORKTHREAD_H_

#define WAIT_TIME_SET 3000

#include <QThread>
#include "DataDefine.h"

class MainWindow;
class AECWorkThread : public QThread
{
	Q_OBJECT
public:
	AECWorkThread(void);
	~AECWorkThread(void);
private:
	BOOL AECCardInit(AECTHREAD_PARAM * pThreadParam);
	BOOL AEC_plate_Start(AECTHREAD_PARAM * pThreadParam);
	int  AECThread_Idle(AECTHREAD_PARAM * pThreadParam);
	BOOL AEC_plate_Stop(AECTHREAD_PARAM * pThreadParam);
	BOOL AEC_selChannel(AECTHREAD_PARAM * pThreadParam);
	BOOL AEC_palte_setone(AECTHREAD_PARAM * pThreadParam);
protected:
	void run();
public:
	AECTHREAD_PARAM m_pAecThreadParam;
	UINT   m_pResult;
	MainWindow* m_pMainWindow;
};

#endif