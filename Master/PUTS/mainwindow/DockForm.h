/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 可停靠窗体
* Filename: DockForm.h
*
* @author        MaChao
* @version       1.00         Date: 2013-05-20
*/

#ifndef DOCKFORM_H
#define DOCKFORM_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>

class CDockForm : public QWidget
{
	Q_OBJECT

public:
	CDockForm(QWidget *parent = 0);
	~CDockForm();
protected:
	void paintEvent(QPaintEvent * e);
public:
	QTimer *timer;
	QPixmap *m_pBitmap;     //存放显示内容的位图
	void SetSize(int w, int h); //chenyuzhu add on 2014-10-30
private:
	void leaveEvent(QEvent *event);
	void enterEvent(QEvent *event);
};

#endif // DOCKFORM_H
