#ifndef JUDGE_H
#define JUDGE_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class Judge : public QDialog
{
	Q_OBJECT

public:
	Judge(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Judge();

public:
	Ui::JudgeClass ui;
private:
	void UiInit();
private slots:
	void Check1_10();
	void Check11_20();
	void Check21_30();
	void SelectAll();
	void SelectAny();

	void AllI();
	void AllA();
	void AllB();
	void AllI2();
	void AnyI();
	void AnyA();
	void AnyB();
	void AnyI2();
};

#endif // JUDGE_H
