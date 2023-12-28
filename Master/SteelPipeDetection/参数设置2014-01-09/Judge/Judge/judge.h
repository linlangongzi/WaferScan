#ifndef JUDGE_H
#define JUDGE_H

#include <QtGui/QDialog>
#include "ui_judge.h"

class Judge : public QDialog
{
	Q_OBJECT

public:
	Judge(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Judge();

private:
	Ui::JudgeClass ui;
};

#endif // JUDGE_H
