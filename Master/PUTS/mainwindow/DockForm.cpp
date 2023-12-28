/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 可停靠窗体
* Filename: DockForm.cpp
*
* @author        MaChao
* @version       1.00         Date: 2013-05-20
*/


#include <QPainter>
#include "DockForm.h"

CDockForm::CDockForm(QWidget *parent)
	:QWidget(parent)
{
	m_pBitmap = NULL;
	setWindowOpacity(0.7);
	setMinimumSize(300,400);
	setMaximumSize(300,400);
	setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
	setWindowFlags(Qt::SubWindow | Qt::WindowStaysOnTopHint);
	timer = new QTimer(this);
//	connect(timer, SIGNAL(timeout()), this, SLOT(close()));
}

CDockForm::~CDockForm()
{
	if (NULL != timer)
	{
		delete timer;
		timer = NULL;
	}
}

void CDockForm::leaveEvent(QEvent *event)
{
//	this->close();
}

void CDockForm::enterEvent(QEvent *event)
{
//	timer->stop();
}
void CDockForm::paintEvent(QPaintEvent * e)
{
	if (NULL != m_pBitmap)
	{
		QPainter painter(this);
		painter.drawPixmap(this->rect(),*m_pBitmap,m_pBitmap->rect());
		painter.end();
	}
}

//chenyuzhu add start on 2014-10-30
void CDockForm::SetSize(int w, int h)
{
	setMinimumSize(w, h);
	setMaximumSize(w, h);
}
//chenyuzhu add end on 2014-10-30