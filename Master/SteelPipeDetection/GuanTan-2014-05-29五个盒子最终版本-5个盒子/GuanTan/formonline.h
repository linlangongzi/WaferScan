#ifndef FORMONLINE_H
#define FORMONLINE_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class FormOnLine : public QDialog
{
	Q_OBJECT

public:
	FormOnLine(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormOnLine();

public:
	Ui::FormOnLineClass ui;
private slots:
	void Yes();
	void No();
	void TestOutPut();//�޶�����
public:
	bool m_bSingleOut;		//��һ�������
	bool m_bWholeOut;		//���屨�����
	bool m_bTestOut;		//���������
	bool m_bQualified;		//�ϸ�Ʒ
	bool m_bDefect;			//ȱ��Ʒ
	bool m_bBadCouple;		//��ϲ���Ʒ
	bool m_bCur;			//��ǰ����״̬
};

#endif // FORMONLINE_H
