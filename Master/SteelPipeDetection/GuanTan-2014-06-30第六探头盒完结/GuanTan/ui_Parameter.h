/********************************************************************************
** Form generated from reading UI file 'parametersetting.ui'
**
** Created: Mon Jul 8 16:31:53 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETERSETTING_H
#define UI_PARAMETERSETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ParameterSettingClass
{
public:
	QLabel *label;
	QLabel *label_2;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QPushButton *pushButtonSaveAs;
	QPushButton *pushButtonAllDo;
	QPushButton *pushButtonOrderOpen;
	QPushButton *pushButtonOrderDo;
	QPushButton *pushButtonUltrasoundDo;
	QPushButton *pushButtonUltrasoundOpen;
	QPushButton *pushButtonShowOpen;
	QPushButton *pushButtonShowDo;
	QPushButton *pushButtonJudgeOpen;
	QPushButton *pushButtonJudgeDo;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QLabel *label_All;
	QLabel *label_Order;
	QLabel *label_Ultrasound;
	QLabel *label_Show;
	QLabel *label_Judge;
	QLineEdit *lineEditOrder;
	QLineEdit *lineEditUltrasound;
	QLineEdit *lineEditShow;
	QLineEdit *lineEditJudge;
	QLineEdit *lineEditAll;
    QPushButton *pushButtonSend;

	void setupUi(QDialog *ParameterSettingClass)
	{
		if (ParameterSettingClass->objectName().isEmpty())
			ParameterSettingClass->setObjectName(QString::fromUtf8("ParameterSettingClass"));
		ParameterSettingClass->resize(486, 500);
		ParameterSettingClass->setMinimumSize(QSize(486, 500));
		ParameterSettingClass->setMaximumSize(QSize(486, 500));
		ParameterSettingClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label = new QLabel(ParameterSettingClass);
		label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(180, 20, 100, 21));
		label->setMinimumSize(QSize(100, 21));
		label->setMaximumSize(QSize(100, 21));
		label->setStyleSheet(QString::fromUtf8("font: 75 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(255, 255, 255);"));
		label_2 = new QLabel(ParameterSettingClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(20, 150, 90, 21));
		label_2->setMinimumSize(QSize(90, 21));
		label_2->setMaximumSize(QSize(90, 21));
		label_2->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(255, 255, 255);"));
		label_3 = new QLabel(ParameterSettingClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(20, 230, 90, 21));
		label_3->setMinimumSize(QSize(90, 21));
		label_3->setMaximumSize(QSize(90, 21));
		label_3->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(255, 255, 255);"));
		label_4 = new QLabel(ParameterSettingClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(20, 310, 90, 21));
		label_4->setMinimumSize(QSize(90, 21));
		label_4->setMaximumSize(QSize(90, 21));
		label_4->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(255, 255, 255);"));
		label_5 = new QLabel(ParameterSettingClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(20, 390, 90, 21));
		label_5->setMinimumSize(QSize(90, 21));
		label_5->setMaximumSize(QSize(90, 21));
		label_5->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(255, 255, 255);"));
		pushButtonSaveAs = new QPushButton(ParameterSettingClass);
		pushButtonSaveAs->setObjectName(QString::fromUtf8("pushButtonSaveAs"));
		pushButtonSaveAs->setGeometry(QRect(20, 100, 75, 23));
		pushButtonSaveAs->setMinimumSize(QSize(75, 23));
		pushButtonSaveAs->setMaximumSize(QSize(75, 23));
		pushButtonSaveAs->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonAllDo = new QPushButton(ParameterSettingClass);
		pushButtonAllDo->setObjectName(QString::fromUtf8("pushButtonAllDo"));
		pushButtonAllDo->setGeometry(QRect(360, 100, 75, 23));
		pushButtonAllDo->setMinimumSize(QSize(75, 23));
		pushButtonAllDo->setMaximumSize(QSize(75, 23));
		pushButtonAllDo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonOrderOpen = new QPushButton(ParameterSettingClass);
		pushButtonOrderOpen->setObjectName(QString::fromUtf8("pushButtonOrderOpen"));
		pushButtonOrderOpen->setGeometry(QRect(123, 180, 75, 23));
		pushButtonOrderOpen->setMinimumSize(QSize(75, 23));
		pushButtonOrderOpen->setMaximumSize(QSize(75, 23));
		pushButtonOrderOpen->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonOrderDo = new QPushButton(ParameterSettingClass);
		pushButtonOrderDo->setObjectName(QString::fromUtf8("pushButtonOrderDo"));
		pushButtonOrderDo->setGeometry(QRect(360, 180, 75, 23));
		pushButtonOrderDo->setMinimumSize(QSize(75, 23));
		pushButtonOrderDo->setMaximumSize(QSize(75, 23));
		pushButtonOrderDo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonUltrasoundDo = new QPushButton(ParameterSettingClass);
		pushButtonUltrasoundDo->setObjectName(QString::fromUtf8("pushButtonUltrasoundDo"));
		pushButtonUltrasoundDo->setGeometry(QRect(360, 260, 75, 23));
		pushButtonUltrasoundDo->setMinimumSize(QSize(75, 23));
		pushButtonUltrasoundDo->setMaximumSize(QSize(75, 23));
		pushButtonUltrasoundDo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonUltrasoundOpen = new QPushButton(ParameterSettingClass);
		pushButtonUltrasoundOpen->setObjectName(QString::fromUtf8("pushButtonUltrasoundOpen"));
		pushButtonUltrasoundOpen->setGeometry(QRect(123, 260, 75, 23));
		pushButtonUltrasoundOpen->setMinimumSize(QSize(75, 23));
		pushButtonUltrasoundOpen->setMaximumSize(QSize(75, 23));
		pushButtonUltrasoundOpen->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonShowOpen = new QPushButton(ParameterSettingClass);
		pushButtonShowOpen->setObjectName(QString::fromUtf8("pushButtonShowOpen"));
		pushButtonShowOpen->setGeometry(QRect(123, 340, 75, 23));
		pushButtonShowOpen->setMinimumSize(QSize(75, 23));
		pushButtonShowOpen->setMaximumSize(QSize(75, 23));
		pushButtonShowOpen->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonShowDo = new QPushButton(ParameterSettingClass);
		pushButtonShowDo->setObjectName(QString::fromUtf8("pushButtonShowDo"));
		pushButtonShowDo->setGeometry(QRect(360, 340, 75, 23));
		pushButtonShowDo->setMinimumSize(QSize(75, 23));
		pushButtonShowDo->setMaximumSize(QSize(75, 23));
		pushButtonShowDo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonJudgeOpen = new QPushButton(ParameterSettingClass);
		pushButtonJudgeOpen->setObjectName(QString::fromUtf8("pushButtonJudgeOpen"));
		pushButtonJudgeOpen->setGeometry(QRect(123, 420, 75, 23));
		pushButtonJudgeOpen->setMinimumSize(QSize(75, 23));
		pushButtonJudgeOpen->setMaximumSize(QSize(75, 23));
		pushButtonJudgeOpen->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonJudgeDo = new QPushButton(ParameterSettingClass);
		pushButtonJudgeDo->setObjectName(QString::fromUtf8("pushButtonJudgeDo"));
		pushButtonJudgeDo->setGeometry(QRect(360, 420, 75, 23));
		pushButtonJudgeDo->setMinimumSize(QSize(75, 23));
		pushButtonJudgeDo->setMaximumSize(QSize(75, 23));
		pushButtonJudgeDo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonYes = new QPushButton(ParameterSettingClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(250, 470, 75, 23));
		pushButtonYes->setMinimumSize(QSize(75, 23));
		pushButtonYes->setMaximumSize(QSize(75, 23));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonNo = new QPushButton(ParameterSettingClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(360, 470, 75, 23));
		pushButtonNo->setMinimumSize(QSize(75, 23));
		pushButtonNo->setMaximumSize(QSize(75, 23));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_All = new QLabel(ParameterSettingClass);
		label_All->setObjectName(QString::fromUtf8("label_All"));
		label_All->setGeometry(QRect(445, 60, 25, 25));
		label_All->setMinimumSize(QSize(25, 25));
		label_All->setMaximumSize(QSize(25, 25));
		label_Order = new QLabel(ParameterSettingClass);
		label_Order->setObjectName(QString::fromUtf8("label_Order"));
		label_Order->setGeometry(QRect(445, 150, 25, 25));
		label_Order->setMinimumSize(QSize(25, 25));
		label_Order->setMaximumSize(QSize(25, 25));
		label_Ultrasound = new QLabel(ParameterSettingClass);
		label_Ultrasound->setObjectName(QString::fromUtf8("label_Ultrasound"));
		label_Ultrasound->setGeometry(QRect(445, 220, 25, 25));
		label_Ultrasound->setMinimumSize(QSize(25, 25));
		label_Ultrasound->setMaximumSize(QSize(25, 25));
		label_Show = new QLabel(ParameterSettingClass);
		label_Show->setObjectName(QString::fromUtf8("label_Show"));
		label_Show->setGeometry(QRect(445, 300, 25, 25));
		label_Show->setMinimumSize(QSize(25, 25));
		label_Show->setMaximumSize(QSize(25, 25));
		label_Judge = new QLabel(ParameterSettingClass);
		label_Judge->setObjectName(QString::fromUtf8("label_Judge"));
		label_Judge->setGeometry(QRect(445, 390, 25, 25));
		label_Judge->setMinimumSize(QSize(25, 25));
		label_Judge->setMaximumSize(QSize(25, 25));
		lineEditOrder = new QLineEdit(ParameterSettingClass);
		lineEditOrder->setObjectName(QString::fromUtf8("lineEditOrder"));
		lineEditOrder->setGeometry(QRect(123, 150, 311, 25));
		lineEditOrder->setMinimumSize(QSize(311, 25));
		lineEditOrder->setMaximumSize(QSize(311, 25));
		lineEditOrder->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditOrder->setAlignment(Qt::AlignCenter);
		lineEditUltrasound = new QLineEdit(ParameterSettingClass);
		lineEditUltrasound->setObjectName(QString::fromUtf8("lineEditUltrasound"));
		lineEditUltrasound->setGeometry(QRect(123, 220, 311, 25));
		lineEditUltrasound->setMinimumSize(QSize(311, 25));
		lineEditUltrasound->setMaximumSize(QSize(311, 25));
		lineEditUltrasound->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditUltrasound->setAlignment(Qt::AlignCenter);
		lineEditShow = new QLineEdit(ParameterSettingClass);
		lineEditShow->setObjectName(QString::fromUtf8("lineEditShow"));
		lineEditShow->setGeometry(QRect(123, 300, 311, 25));
		lineEditShow->setMinimumSize(QSize(311, 25));
		lineEditShow->setMaximumSize(QSize(311, 25));
		lineEditShow->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditShow->setAlignment(Qt::AlignCenter);
		lineEditJudge = new QLineEdit(ParameterSettingClass);
		lineEditJudge->setObjectName(QString::fromUtf8("lineEditJudge"));
		lineEditJudge->setGeometry(QRect(123, 390, 311, 25));
		lineEditJudge->setMinimumSize(QSize(311, 25));
		lineEditJudge->setMaximumSize(QSize(311, 25));
		lineEditJudge->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditJudge->setAlignment(Qt::AlignCenter);
		lineEditAll = new QLineEdit(ParameterSettingClass);
		lineEditAll->setObjectName(QString::fromUtf8("lineEditAll"));
		lineEditAll->setGeometry(QRect(20, 60, 415, 25));
		lineEditAll->setMinimumSize(QSize(415, 25));
		lineEditAll->setMaximumSize(QSize(415, 25));
		lineEditAll->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditAll->setAlignment(Qt::AlignCenter);
        pushButtonSend = new QPushButton(ParameterSettingClass);
        pushButtonSend->setObjectName(QString::fromUtf8("pushButtonSend"));
        pushButtonSend->setGeometry(QRect(120, 470, 95, 23));
        pushButtonSend->setMinimumSize(QSize(95, 23));
        pushButtonSend->setMaximumSize(QSize(95, 23));
        pushButtonSend->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

		retranslateUi(ParameterSettingClass);

		QMetaObject::connectSlotsByName(ParameterSettingClass);
	} // setupUi

	void retranslateUi(QDialog *ParameterSettingClass)
	{
		ParameterSettingClass->setWindowTitle(QApplication::translate("ParameterSettingClass", "ParameterSetting", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("ParameterSettingClass", "\345\217\202\346\225\260\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("ParameterSettingClass", "\350\256\242\345\215\225\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("ParameterSettingClass", "\350\266\205\345\243\260\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("ParameterSettingClass", "\346\230\276\347\244\272\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("ParameterSettingClass", "\350\257\204\345\210\244\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
		pushButtonSaveAs->setText(QApplication::translate("ParameterSettingClass", "\345\217\246\345\255\230\344\270\272", 0, QApplication::UnicodeUTF8));
		pushButtonAllDo->setText(QApplication::translate("ParameterSettingClass", "\350\260\203\347\224\250", 0, QApplication::UnicodeUTF8));
		pushButtonOrderOpen->setText(QApplication::translate("ParameterSettingClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
		pushButtonOrderDo->setText(QApplication::translate("ParameterSettingClass", "\350\260\203\347\224\250", 0, QApplication::UnicodeUTF8));
		pushButtonUltrasoundDo->setText(QApplication::translate("ParameterSettingClass", "\350\260\203\347\224\250", 0, QApplication::UnicodeUTF8));
		pushButtonUltrasoundOpen->setText(QApplication::translate("ParameterSettingClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
		pushButtonShowOpen->setText(QApplication::translate("ParameterSettingClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
		pushButtonShowDo->setText(QApplication::translate("ParameterSettingClass", "\350\260\203\347\224\250", 0, QApplication::UnicodeUTF8));
		pushButtonJudgeOpen->setText(QApplication::translate("ParameterSettingClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
		pushButtonJudgeDo->setText(QApplication::translate("ParameterSettingClass", "\350\260\203\347\224\250", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("ParameterSettingClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("ParameterSettingClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		label_All->setText(QString());
		label_Order->setText(QString());
		label_Ultrasound->setText(QString());
		label_Show->setText(QString());
		label_Judge->setText(QString());
		lineEditOrder->setText(QString());
		lineEditAll->setText(QString());
        pushButtonSend->setText(QApplication::translate("ParameterSettingClass", "\345\217\221\351\200\201\345\210\260\346\266\241\346\265\201\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class ParameterSettingClass: public Ui_ParameterSettingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERSETTING_H

/********************************************************************************
** Form generated from reading UI file 'judgeparameter.ui'
**
** Created: Thu Oct 24 16:20:40 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JUDGEPARAMETER_H
#define UI_JUDGEPARAMETER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JudgeParameterClass
{
public:
	QTabWidget *tabWidget;
	QWidget *tab;
	QLabel *label;
	QLabel *label_2;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QLabel *label_6;
	QLabel *label_7;
	QLabel *label_8;
	QComboBox *comboBoxYOut;
	QComboBox *comboBoxYIn;
	QComboBox *comboBoxXOut;
	QComboBox *comboBoxXIn;
	QComboBox *comboBoxPlyCut;
	QComboBox *comboBoxPlyAdd;
	QComboBox *comboBoxLayering;
	QWidget *tab_2;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QLabel *label_14;
	QLabel *label_15;
	QLabel *label_16;
	QWidget *widget;
	QLineEdit *lineEditYesCouplingNumber;
	QLabel *label_17;
	QLineEdit *lineEditYesCouplingLength;
	QLabel *label_29;
	QLabel *label_30;
	QLabel *label_41;
	QWidget *widget_2;
	QLineEdit *lineEditYesFaultNumber;
	QLabel *label_18;
	QLineEdit *lineEditYesFaultLength;
	QLabel *label_31;
	QLabel *label_38;
	QLabel *label_40;
	QLabel *label_19;
	QLabel *label_20;
	QLabel *label_21;
	QLabel *label_22;
	QLabel *label_23;
	QWidget *widget_3;
	QLineEdit *lineEditNoCouplingNumber;
	QLabel *label_24;
	QLineEdit *lineEditNoCouplingLength;
	QLabel *label_33;
	QLabel *label_35;
	QLabel *label_42;
	QWidget *widget_4;
	QLineEdit *lineEditNoFaultNumber;
	QLabel *label_25;
	QLineEdit *lineEditNoFaultLength;
	QLabel *label_32;
	QLabel *label_39;
	QLabel *label_44;
	QLabel *label_45;
	QLabel *label_37;
	QWidget *widget_5;
	QLineEdit *lineEditCouplingNumber;
	QLabel *label_27;
	QLineEdit *lineEditCouplingLength;
	QLabel *label_43;
	QLabel *label_46;
	QLabel *label_34;
	QLabel *label_47;
	QLabel *label_36;
	QWidget *tab_3;
	QPushButton *pushButtonSoundOpen;
	QPushButton *pushButtonSoundClose;
	QPushButton *pushButtonLightOpen;
	QPushButton *pushButtonLightClose;
	QLabel *label_26;
	QLabel *label_28;
	QPushButton *pushButtonSaveAs;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;

	void setupUi(QDialog *JudgeParameterClass)
	{
		if (JudgeParameterClass->objectName().isEmpty())
			JudgeParameterClass->setObjectName(QString::fromUtf8("JudgeParameterClass"));
		JudgeParameterClass->resize(550, 426);
		JudgeParameterClass->setMinimumSize(QSize(550, 426));
		JudgeParameterClass->setMaximumSize(QSize(550, 426));
		JudgeParameterClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		tabWidget = new QTabWidget(JudgeParameterClass);
		tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
		tabWidget->setGeometry(QRect(0, 10, 550, 351));
		tabWidget->setMinimumSize(QSize(550, 351));
		tabWidget->setMaximumSize(QSize(550, 351));
		tabWidget->setStyleSheet(QString::fromUtf8(""));
		tab = new QWidget();
		tab->setObjectName(QString::fromUtf8("tab"));
		label = new QLabel(tab);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(310, 20, 65, 20));
		label->setMinimumSize(QSize(65, 20));
		label->setMaximumSize(QSize(65, 20));
		label->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_2 = new QLabel(tab);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(90, 50, 100, 20));
		label_2->setMinimumSize(QSize(100, 20));
		label_2->setMaximumSize(QSize(100, 20));
		label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_3 = new QLabel(tab);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(90, 90, 100, 20));
		label_3->setMinimumSize(QSize(100, 20));
		label_3->setMaximumSize(QSize(100, 20));
		label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_4 = new QLabel(tab);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(90, 130, 100, 20));
		label_4->setMinimumSize(QSize(100, 20));
		label_4->setMaximumSize(QSize(100, 20));
		label_4->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_5 = new QLabel(tab);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(90, 170, 100, 20));
		label_5->setMinimumSize(QSize(100, 20));
		label_5->setMaximumSize(QSize(100, 20));
		label_5->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_6 = new QLabel(tab);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(90, 210, 100, 20));
		label_6->setMinimumSize(QSize(100, 20));
		label_6->setMaximumSize(QSize(100, 20));
		label_6->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_7 = new QLabel(tab);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(90, 250, 100, 20));
		label_7->setMinimumSize(QSize(100, 20));
		label_7->setMaximumSize(QSize(100, 20));
		label_7->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_8 = new QLabel(tab);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(90, 290, 100, 20));
		label_8->setMinimumSize(QSize(100, 20));
		label_8->setMaximumSize(QSize(100, 20));
		label_8->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		comboBoxYOut = new QComboBox(tab);
		comboBoxYOut->setObjectName(QString::fromUtf8("comboBoxYOut"));
		comboBoxYOut->setGeometry(QRect(270, 50, 160, 22));
		comboBoxYOut->setMinimumSize(QSize(160, 22));
		comboBoxYOut->setMaximumSize(QSize(160, 22));
		comboBoxYOut->setStyleSheet(QString::fromUtf8("QComboBox{font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(244, 244, 244);\n"
			"}\n"
			"QComboBox {\n"
			"border: 1px solid rgb(244,244,244);\n"
			"border-radius: 3px;\n"
			"}\n"
			"QComboBox::drop-down{\n"
			"width:20px;\n"
			"}\n"
			"\n"
			"\n"
			"\n"
			"\n"
			""));
		comboBoxYIn = new QComboBox(tab);
		comboBoxYIn->setObjectName(QString::fromUtf8("comboBoxYIn"));
		comboBoxYIn->setGeometry(QRect(270, 90, 160, 22));
		comboBoxYIn->setMinimumSize(QSize(160, 22));
		comboBoxYIn->setMaximumSize(QSize(160, 22));
		comboBoxYIn->setStyleSheet(QString::fromUtf8("QComboBox{font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(244, 244, 244);\n"
			"}\n"
			"QComboBox {\n"
			"border: 1px solid rgb(244,244,244);\n"
			"border-radius: 3px;\n"
			"}\n"
			"QComboBox::drop-down{\n"
			"width:20px;\n"
			"}\n"
			"\n"
			"\n"
			"\n"
			"\n"
			""));
		comboBoxXOut = new QComboBox(tab);
		comboBoxXOut->setObjectName(QString::fromUtf8("comboBoxXOut"));
		comboBoxXOut->setGeometry(QRect(270, 130, 160, 22));
		comboBoxXOut->setMinimumSize(QSize(160, 22));
		comboBoxXOut->setMaximumSize(QSize(160, 22));
		comboBoxXOut->setStyleSheet(QString::fromUtf8("QComboBox{font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(244, 244, 244);\n"
			"}\n"
			"QComboBox {\n"
			"border: 1px solid rgb(244,244,244);\n"
			"border-radius: 3px;\n"
			"}\n"
			"QComboBox::drop-down{\n"
			"width:20px;\n"
			"}\n"
			"\n"
			"\n"
			"\n"
			"\n"
			""));
		comboBoxXIn = new QComboBox(tab);
		comboBoxXIn->setObjectName(QString::fromUtf8("comboBoxXIn"));
		comboBoxXIn->setGeometry(QRect(270, 170, 160, 22));
		comboBoxXIn->setMinimumSize(QSize(160, 22));
		comboBoxXIn->setMaximumSize(QSize(160, 22));
		comboBoxXIn->setStyleSheet(QString::fromUtf8("QComboBox{font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(244, 244, 244);\n"
			"}\n"
			"QComboBox {\n"
			"border: 1px solid rgb(244,244,244);\n"
			"border-radius: 3px;\n"
			"}\n"
			"QComboBox::drop-down{\n"
			"width:20px;\n"
			"}\n"
			"\n"
			"\n"
			"\n"
			"\n"
			""));
		comboBoxPlyCut = new QComboBox(tab);
		comboBoxPlyCut->setObjectName(QString::fromUtf8("comboBoxPlyCut"));
		comboBoxPlyCut->setGeometry(QRect(270, 210, 160, 22));
		comboBoxPlyCut->setMinimumSize(QSize(160, 22));
		comboBoxPlyCut->setMaximumSize(QSize(160, 22));
		comboBoxPlyCut->setStyleSheet(QString::fromUtf8("QComboBox{font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(244, 244, 244);\n"
			"}\n"
			"QComboBox {\n"
			"border: 1px solid rgb(244,244,244);\n"
			"border-radius: 3px;\n"
			"}\n"
			"QComboBox::drop-down{\n"
			"width:20px;\n"
			"}\n"
			"\n"
			"\n"
			"\n"
			"\n"
			""));
		comboBoxPlyAdd = new QComboBox(tab);
		comboBoxPlyAdd->setObjectName(QString::fromUtf8("comboBoxPlyAdd"));
		comboBoxPlyAdd->setGeometry(QRect(270, 250, 160, 22));
		comboBoxPlyAdd->setMinimumSize(QSize(160, 22));
		comboBoxPlyAdd->setMaximumSize(QSize(160, 22));
		comboBoxPlyAdd->setStyleSheet(QString::fromUtf8("QComboBox{font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(244, 244, 244);\n"
			"}\n"
			"QComboBox {\n"
			"border: 1px solid rgb(244,244,244);\n"
			"border-radius: 3px;\n"
			"}\n"
			"QComboBox::drop-down{\n"
			"width:20px;\n"
			"}\n"
			"\n"
			"\n"
			"\n"
			"\n"
			""));
		comboBoxLayering = new QComboBox(tab);
		comboBoxLayering->setObjectName(QString::fromUtf8("comboBoxLayering"));
		comboBoxLayering->setGeometry(QRect(270, 290, 160, 22));
		comboBoxLayering->setMinimumSize(QSize(160, 22));
		comboBoxLayering->setMaximumSize(QSize(160, 22));
		comboBoxLayering->setStyleSheet(QString::fromUtf8("QComboBox{font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"color: rgb(244, 244, 244);\n"
			"}\n"
			"QComboBox {\n"
			"border: 1px solid rgb(244,244,244);\n"
			"border-radius: 3px;\n"
			"}\n"
			"QComboBox::drop-down{\n"
			"width:20px;\n"
			"}\n"
			"\n"
			"\n"
			"\n"
			"\n"
			""));
		tabWidget->addTab(tab, QString());
		tab_2 = new QWidget();
		tab_2->setObjectName(QString::fromUtf8("tab_2"));
		label_9 = new QLabel(tab_2);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(80, 30, 60, 15));
		label_9->setMinimumSize(QSize(60, 15));
		label_9->setMaximumSize(QSize(60, 15));
		label_9->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_10 = new QLabel(tab_2);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(80, 50, 60, 15));
		label_10->setMinimumSize(QSize(60, 15));
		label_10->setMaximumSize(QSize(60, 15));
		label_10->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_11 = new QLabel(tab_2);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(200, 50, 60, 15));
		label_11->setMinimumSize(QSize(60, 15));
		label_11->setMaximumSize(QSize(60, 15));
		label_11->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_12 = new QLabel(tab_2);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(200, 30, 60, 15));
		label_12->setMinimumSize(QSize(60, 15));
		label_12->setMaximumSize(QSize(60, 15));
		label_12->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_13 = new QLabel(tab_2);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(5, 90, 45, 15));
		label_13->setMinimumSize(QSize(45, 15));
		label_13->setMaximumSize(QSize(45, 15));
		label_13->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_14 = new QLabel(tab_2);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(5, 170, 45, 15));
		label_14->setMinimumSize(QSize(45, 15));
		label_14->setMaximumSize(QSize(45, 15));
		label_14->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_15 = new QLabel(tab_2);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(5, 240, 45, 15));
		label_15->setMinimumSize(QSize(45, 15));
		label_15->setMaximumSize(QSize(45, 15));
		label_15->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_16 = new QLabel(tab_2);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(5, 260, 45, 15));
		label_16->setMinimumSize(QSize(45, 15));
		label_16->setMaximumSize(QSize(45, 15));
		label_16->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		widget = new QWidget(tab_2);
		widget->setObjectName(QString::fromUtf8("widget"));
		widget->setGeometry(QRect(47, 80, 231, 35));
		widget->setMinimumSize(QSize(0, 35));
		widget->setMaximumSize(QSize(16777215, 50));
		widget->setStyleSheet(QString::fromUtf8("QWidget#widget{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		lineEditYesCouplingNumber = new QLineEdit(widget);
		lineEditYesCouplingNumber->setObjectName(QString::fromUtf8("lineEditYesCouplingNumber"));
		lineEditYesCouplingNumber->setGeometry(QRect(40, 5, 40, 25));
		lineEditYesCouplingNumber->setMinimumSize(QSize(40, 25));
		lineEditYesCouplingNumber->setMaximumSize(QSize(40, 25));
		lineEditYesCouplingNumber->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_17 = new QLabel(widget);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(90, 11, 25, 15));
		label_17->setMinimumSize(QSize(25, 15));
		label_17->setMaximumSize(QSize(25, 15));
		label_17->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditYesCouplingLength = new QLineEdit(widget);
		lineEditYesCouplingLength->setObjectName(QString::fromUtf8("lineEditYesCouplingLength"));
		lineEditYesCouplingLength->setGeometry(QRect(160, 5, 40, 25));
		lineEditYesCouplingLength->setMinimumSize(QSize(40, 25));
		lineEditYesCouplingLength->setMaximumSize(QSize(40, 25));
		lineEditYesCouplingLength->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_29 = new QLabel(widget);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(10, 11, 25, 15));
		label_29->setMinimumSize(QSize(25, 15));
		label_29->setMaximumSize(QSize(25, 15));
		label_29->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_30 = new QLabel(widget);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(130, 11, 25, 15));
		label_30->setMinimumSize(QSize(25, 15));
		label_30->setMaximumSize(QSize(25, 15));
		label_30->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_41 = new QLabel(widget);
		label_41->setObjectName(QString::fromUtf8("label_41"));
		label_41->setGeometry(QRect(210, 10, 15, 15));
		label_41->setMinimumSize(QSize(15, 15));
		label_41->setMaximumSize(QSize(15, 15));
		label_41->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		widget_2 = new QWidget(tab_2);
		widget_2->setObjectName(QString::fromUtf8("widget_2"));
		widget_2->setGeometry(QRect(310, 80, 231, 35));
		widget_2->setMinimumSize(QSize(0, 35));
		widget_2->setMaximumSize(QSize(16777215, 35));
		widget_2->setStyleSheet(QString::fromUtf8("QWidget#widget_2{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		lineEditYesFaultNumber = new QLineEdit(widget_2);
		lineEditYesFaultNumber->setObjectName(QString::fromUtf8("lineEditYesFaultNumber"));
		lineEditYesFaultNumber->setGeometry(QRect(40, 5, 40, 25));
		lineEditYesFaultNumber->setMinimumSize(QSize(40, 25));
		lineEditYesFaultNumber->setMaximumSize(QSize(40, 25));
		lineEditYesFaultNumber->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_18 = new QLabel(widget_2);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(90, 11, 25, 15));
		label_18->setMinimumSize(QSize(25, 15));
		label_18->setMaximumSize(QSize(25, 15));
		label_18->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditYesFaultLength = new QLineEdit(widget_2);
		lineEditYesFaultLength->setObjectName(QString::fromUtf8("lineEditYesFaultLength"));
		lineEditYesFaultLength->setGeometry(QRect(160, 5, 40, 25));
		lineEditYesFaultLength->setMinimumSize(QSize(40, 25));
		lineEditYesFaultLength->setMaximumSize(QSize(40, 25));
		lineEditYesFaultLength->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_31 = new QLabel(widget_2);
		label_31->setObjectName(QString::fromUtf8("label_31"));
		label_31->setGeometry(QRect(10, 11, 25, 15));
		label_31->setMinimumSize(QSize(25, 15));
		label_31->setMaximumSize(QSize(25, 15));
		label_31->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_38 = new QLabel(widget_2);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(130, 11, 25, 15));
		label_38->setMinimumSize(QSize(25, 15));
		label_38->setMaximumSize(QSize(25, 15));
		label_38->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_40 = new QLabel(widget_2);
		label_40->setObjectName(QString::fromUtf8("label_40"));
		label_40->setGeometry(QRect(210, 10, 15, 15));
		label_40->setMinimumSize(QSize(15, 15));
		label_40->setMaximumSize(QSize(15, 15));
		label_40->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_19 = new QLabel(tab_2);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(280, 90, 25, 15));
		label_19->setMinimumSize(QSize(25, 15));
		label_19->setMaximumSize(QSize(25, 15));
		label_19->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_20 = new QLabel(tab_2);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(340, 30, 60, 15));
		label_20->setMinimumSize(QSize(60, 15));
		label_20->setMaximumSize(QSize(60, 15));
		label_20->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_21 = new QLabel(tab_2);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(460, 50, 60, 15));
		label_21->setMinimumSize(QSize(60, 15));
		label_21->setMaximumSize(QSize(60, 15));
		label_21->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_22 = new QLabel(tab_2);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(460, 30, 60, 15));
		label_22->setMinimumSize(QSize(60, 15));
		label_22->setMaximumSize(QSize(60, 15));
		label_22->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_23 = new QLabel(tab_2);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(280, 170, 25, 15));
		label_23->setMinimumSize(QSize(25, 15));
		label_23->setMaximumSize(QSize(25, 15));
		label_23->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		widget_3 = new QWidget(tab_2);
		widget_3->setObjectName(QString::fromUtf8("widget_3"));
		widget_3->setGeometry(QRect(47, 160, 231, 35));
		widget_3->setMinimumSize(QSize(0, 35));
		widget_3->setMaximumSize(QSize(16777215, 35));
		widget_3->setStyleSheet(QString::fromUtf8("QWidget#widget_3{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		lineEditNoCouplingNumber = new QLineEdit(widget_3);
		lineEditNoCouplingNumber->setObjectName(QString::fromUtf8("lineEditNoCouplingNumber"));
		lineEditNoCouplingNumber->setGeometry(QRect(40, 5, 40, 25));
		lineEditNoCouplingNumber->setMinimumSize(QSize(40, 25));
		lineEditNoCouplingNumber->setMaximumSize(QSize(40, 25));
		lineEditNoCouplingNumber->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_24 = new QLabel(widget_3);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(90, 11, 25, 15));
		label_24->setMinimumSize(QSize(25, 15));
		label_24->setMaximumSize(QSize(25, 15));
		label_24->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditNoCouplingLength = new QLineEdit(widget_3);
		lineEditNoCouplingLength->setObjectName(QString::fromUtf8("lineEditNoCouplingLength"));
		lineEditNoCouplingLength->setGeometry(QRect(160, 5, 40, 25));
		lineEditNoCouplingLength->setMinimumSize(QSize(40, 25));
		lineEditNoCouplingLength->setMaximumSize(QSize(40, 25));
		lineEditNoCouplingLength->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_33 = new QLabel(widget_3);
		label_33->setObjectName(QString::fromUtf8("label_33"));
		label_33->setGeometry(QRect(10, 11, 25, 15));
		label_33->setMinimumSize(QSize(25, 15));
		label_33->setMaximumSize(QSize(25, 15));
		label_33->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_35 = new QLabel(widget_3);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(130, 11, 25, 15));
		label_35->setMinimumSize(QSize(25, 15));
		label_35->setMaximumSize(QSize(25, 15));
		label_35->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_42 = new QLabel(widget_3);
		label_42->setObjectName(QString::fromUtf8("label_42"));
		label_42->setGeometry(QRect(210, 10, 15, 15));
		label_42->setMinimumSize(QSize(15, 15));
		label_42->setMaximumSize(QSize(15, 15));
		label_42->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		widget_4 = new QWidget(tab_2);
		widget_4->setObjectName(QString::fromUtf8("widget_4"));
		widget_4->setGeometry(QRect(310, 160, 231, 35));
		widget_4->setMinimumSize(QSize(0, 35));
		widget_4->setMaximumSize(QSize(16777215, 35));
		widget_4->setStyleSheet(QString::fromUtf8("QWidget#widget_4{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		lineEditNoFaultNumber = new QLineEdit(widget_4);
		lineEditNoFaultNumber->setObjectName(QString::fromUtf8("lineEditNoFaultNumber"));
		lineEditNoFaultNumber->setGeometry(QRect(40, 5, 40, 25));
		lineEditNoFaultNumber->setMinimumSize(QSize(40, 25));
		lineEditNoFaultNumber->setMaximumSize(QSize(40, 25));
		lineEditNoFaultNumber->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_25 = new QLabel(widget_4);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(90, 11, 25, 15));
		label_25->setMinimumSize(QSize(25, 15));
		label_25->setMaximumSize(QSize(25, 15));
		label_25->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditNoFaultLength = new QLineEdit(widget_4);
		lineEditNoFaultLength->setObjectName(QString::fromUtf8("lineEditNoFaultLength"));
		lineEditNoFaultLength->setGeometry(QRect(160, 5, 40, 25));
		lineEditNoFaultLength->setMinimumSize(QSize(40, 25));
		lineEditNoFaultLength->setMaximumSize(QSize(40, 25));
		lineEditNoFaultLength->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_32 = new QLabel(widget_4);
		label_32->setObjectName(QString::fromUtf8("label_32"));
		label_32->setGeometry(QRect(10, 4, 25, 15));
		label_32->setMinimumSize(QSize(25, 15));
		label_32->setMaximumSize(QSize(25, 15));
		label_32->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_39 = new QLabel(widget_4);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(210, 10, 15, 15));
		label_39->setMinimumSize(QSize(15, 15));
		label_39->setMaximumSize(QSize(15, 15));
		label_39->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_44 = new QLabel(widget_4);
		label_44->setObjectName(QString::fromUtf8("label_44"));
		label_44->setGeometry(QRect(10, 18, 25, 15));
		label_44->setMinimumSize(QSize(25, 15));
		label_44->setMaximumSize(QSize(25, 15));
		label_44->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_45 = new QLabel(widget_4);
		label_45->setObjectName(QString::fromUtf8("label_45"));
		label_45->setGeometry(QRect(130, 19, 25, 15));
		label_45->setMinimumSize(QSize(25, 15));
		label_45->setMaximumSize(QSize(25, 15));
		label_45->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_37 = new QLabel(widget_4);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(130, 5, 25, 15));
		label_37->setMinimumSize(QSize(25, 15));
		label_37->setMaximumSize(QSize(25, 15));
		label_37->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		widget_5 = new QWidget(tab_2);
		widget_5->setObjectName(QString::fromUtf8("widget_5"));
		widget_5->setGeometry(QRect(47, 240, 231, 35));
		widget_5->setMinimumSize(QSize(0, 35));
		widget_5->setMaximumSize(QSize(16777215, 35));
		widget_5->setStyleSheet(QString::fromUtf8("QWidget#widget_5{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		lineEditCouplingNumber = new QLineEdit(widget_5);
		lineEditCouplingNumber->setObjectName(QString::fromUtf8("lineEditCouplingNumber"));
		lineEditCouplingNumber->setGeometry(QRect(40, 5, 40, 25));
		lineEditCouplingNumber->setMinimumSize(QSize(40, 25));
		lineEditCouplingNumber->setMaximumSize(QSize(40, 25));
		lineEditCouplingNumber->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_27 = new QLabel(widget_5);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(90, 11, 25, 15));
		label_27->setMinimumSize(QSize(25, 15));
		label_27->setMaximumSize(QSize(25, 15));
		label_27->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditCouplingLength = new QLineEdit(widget_5);
		lineEditCouplingLength->setObjectName(QString::fromUtf8("lineEditCouplingLength"));
		lineEditCouplingLength->setGeometry(QRect(160, 5, 40, 25));
		lineEditCouplingLength->setMinimumSize(QSize(40, 25));
		lineEditCouplingLength->setMaximumSize(QSize(40, 25));
		lineEditCouplingLength->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_43 = new QLabel(widget_5);
		label_43->setObjectName(QString::fromUtf8("label_43"));
		label_43->setGeometry(QRect(210, 10, 15, 15));
		label_43->setMinimumSize(QSize(15, 15));
		label_43->setMaximumSize(QSize(15, 15));
		label_43->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_46 = new QLabel(widget_5);
		label_46->setObjectName(QString::fromUtf8("label_46"));
		label_46->setGeometry(QRect(10, 19, 25, 15));
		label_46->setMinimumSize(QSize(25, 15));
		label_46->setMaximumSize(QSize(25, 15));
		label_46->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_34 = new QLabel(widget_5);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(10, 5, 25, 15));
		label_34->setMinimumSize(QSize(25, 15));
		label_34->setMaximumSize(QSize(25, 15));
		label_34->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_47 = new QLabel(widget_5);
		label_47->setObjectName(QString::fromUtf8("label_47"));
		label_47->setGeometry(QRect(130, 19, 25, 15));
		label_47->setMinimumSize(QSize(25, 15));
		label_47->setMaximumSize(QSize(25, 15));
		label_47->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_36 = new QLabel(widget_5);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(130, 5, 25, 15));
		label_36->setMinimumSize(QSize(25, 15));
		label_36->setMaximumSize(QSize(25, 15));
		label_36->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 0, 0);\n"
			"font: 75 10pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		tabWidget->addTab(tab_2, QString());
		tab_3 = new QWidget();
		tab_3->setObjectName(QString::fromUtf8("tab_3"));
		pushButtonSoundOpen = new QPushButton(tab_3);
		pushButtonSoundOpen->setObjectName(QString::fromUtf8("pushButtonSoundOpen"));
		pushButtonSoundOpen->setGeometry(QRect(110, 50, 60, 25));
		pushButtonSoundOpen->setMinimumSize(QSize(60, 25));
		pushButtonSoundOpen->setMaximumSize(QSize(60, 25));
		pushButtonSoundOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
		pushButtonSoundClose = new QPushButton(tab_3);
		pushButtonSoundClose->setObjectName(QString::fromUtf8("pushButtonSoundClose"));
		pushButtonSoundClose->setGeometry(QRect(190, 50, 60, 25));
		pushButtonSoundClose->setMinimumSize(QSize(60, 25));
		pushButtonSoundClose->setMaximumSize(QSize(60, 25));
		pushButtonSoundClose->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
		pushButtonLightOpen = new QPushButton(tab_3);
		pushButtonLightOpen->setObjectName(QString::fromUtf8("pushButtonLightOpen"));
		pushButtonLightOpen->setGeometry(QRect(110, 110, 60, 25));
		pushButtonLightOpen->setMinimumSize(QSize(60, 25));
		pushButtonLightOpen->setMaximumSize(QSize(60, 25));
		pushButtonLightOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
		pushButtonLightClose = new QPushButton(tab_3);
		pushButtonLightClose->setObjectName(QString::fromUtf8("pushButtonLightClose"));
		pushButtonLightClose->setGeometry(QRect(190, 110, 60, 25));
		pushButtonLightClose->setMinimumSize(QSize(60, 25));
		pushButtonLightClose->setMaximumSize(QSize(60, 25));
		pushButtonLightClose->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
		label_26 = new QLabel(tab_3);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(40, 50, 65, 20));
		label_26->setMinimumSize(QSize(65, 20));
		label_26->setMaximumSize(QSize(65, 20));
		label_26->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_28 = new QLabel(tab_3);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(40, 110, 65, 20));
		label_28->setMinimumSize(QSize(65, 20));
		label_28->setMaximumSize(QSize(65, 20));
		label_28->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		tabWidget->addTab(tab_3, QString());
		pushButtonSaveAs = new QPushButton(JudgeParameterClass);
		pushButtonSaveAs->setObjectName(QString::fromUtf8("pushButtonSaveAs"));
		pushButtonSaveAs->setGeometry(QRect(90, 380, 60, 25));
		pushButtonSaveAs->setMinimumSize(QSize(60, 25));
		pushButtonSaveAs->setMaximumSize(QSize(60, 25));
		pushButtonSaveAs->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonYes = new QPushButton(JudgeParameterClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(230, 380, 60, 25));
		pushButtonYes->setMinimumSize(QSize(60, 25));
		pushButtonYes->setMaximumSize(QSize(60, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonNo = new QPushButton(JudgeParameterClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(370, 380, 60, 25));
		pushButtonNo->setMinimumSize(QSize(60, 25));
		pushButtonNo->setMaximumSize(QSize(60, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

		retranslateUi(JudgeParameterClass);

		tabWidget->setCurrentIndex(1);


		QMetaObject::connectSlotsByName(JudgeParameterClass);
	} // setupUi

	void retranslateUi(QDialog *JudgeParameterClass)
	{
		JudgeParameterClass->setWindowTitle(QApplication::translate("JudgeParameterClass", "JudgeParameter", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("JudgeParameterClass", "\346\211\223\346\240\207\344\277\241\345\217\267", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("JudgeParameterClass", "\347\272\265\345\220\221\345\244\226\345\243\201\347\274\272\351\231\267", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("JudgeParameterClass", "\347\272\265\345\220\221\345\206\205\345\243\201\347\274\272\351\231\267", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("JudgeParameterClass", "\346\250\252\345\220\221\345\244\226\345\243\201\347\274\272\351\231\267", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("JudgeParameterClass", "\346\250\252\345\220\221\345\206\205\345\243\201\347\274\272\351\231\267", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("JudgeParameterClass", "\345\243\201\345\216\232\345\207\217\350\226\204", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("JudgeParameterClass", "\345\243\201\345\216\232\345\242\236\345\212\240", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("JudgeParameterClass", "\345\210\206\345\261\202\347\274\272\351\231\267", 0, QApplication::UnicodeUTF8));
		comboBoxYOut->clear();
		comboBoxYOut->insertItems(0, QStringList()
			<< QApplication::translate("JudgeParameterClass", "     \347\272\242   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \351\273\204   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \347\231\275   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     N O N E", 0, QApplication::UnicodeUTF8)
			);
		comboBoxYIn->clear();
		comboBoxYIn->insertItems(0, QStringList()
			<< QApplication::translate("JudgeParameterClass", "     \347\272\242   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \351\273\204   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \347\231\275   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     N O N E", 0, QApplication::UnicodeUTF8)
			);
		comboBoxXOut->clear();
		comboBoxXOut->insertItems(0, QStringList()
			<< QApplication::translate("JudgeParameterClass", "     \347\272\242   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \351\273\204   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \347\231\275   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     N O N E", 0, QApplication::UnicodeUTF8)
			);
		comboBoxXIn->clear();
		comboBoxXIn->insertItems(0, QStringList()
			<< QApplication::translate("JudgeParameterClass", "     \347\272\242   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \351\273\204   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \347\231\275   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     N O N E", 0, QApplication::UnicodeUTF8)
			);
		comboBoxPlyCut->clear();
		comboBoxPlyCut->insertItems(0, QStringList()
			<< QApplication::translate("JudgeParameterClass", "     \347\272\242   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \351\273\204   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \347\231\275   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     N O N E", 0, QApplication::UnicodeUTF8)
			);
		comboBoxPlyAdd->clear();
		comboBoxPlyAdd->insertItems(0, QStringList()
			<< QApplication::translate("JudgeParameterClass", "     \347\272\242   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \351\273\204   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \347\231\275   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     N O N E", 0, QApplication::UnicodeUTF8)
			);
		comboBoxLayering->clear();
		comboBoxLayering->insertItems(0, QStringList()
			<< QApplication::translate("JudgeParameterClass", "     \347\272\242   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \351\273\204   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     \347\231\275   \350\211\262", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeParameterClass", "     N O N E", 0, QApplication::UnicodeUTF8)
			);
		tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("JudgeParameterClass", "\346\211\223\346\240\207", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("JudgeParameterClass", "\350\200\246\345\220\210\344\270\215\350\211\257", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("JudgeParameterClass", "  \346\225\260\351\207\217", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("JudgeParameterClass", "\344\270\215\350\211\257\351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("JudgeParameterClass", "\346\234\200\345\244\247\350\200\246\345\220\210", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("JudgeParameterClass", "\345\220\210\346\240\274\345\223\201", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("JudgeParameterClass", "\347\274\272\351\231\267\345\223\201", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("JudgeParameterClass", "\350\200\246\345\220\210\344\270\215", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("JudgeParameterClass", " \350\211\257\345\223\201", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("JudgeParameterClass", "\345\271\266\344\270\224", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("JudgeParameterClass", "\345\260\217\344\272\216", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("JudgeParameterClass", "\345\260\217\344\272\216", 0, QApplication::UnicodeUTF8));
		label_41->setText(QApplication::translate("JudgeParameterClass", "cm", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("JudgeParameterClass", "\345\271\266\344\270\224", 0, QApplication::UnicodeUTF8));
		label_31->setText(QApplication::translate("JudgeParameterClass", "\345\260\217\344\272\216", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("JudgeParameterClass", "\345\260\217\344\272\216", 0, QApplication::UnicodeUTF8));
		label_40->setText(QApplication::translate("JudgeParameterClass", "cm", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("JudgeParameterClass", "\345\271\266\344\270\224", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("JudgeParameterClass", "\347\274\272\351\231\267\346\225\260\351\207\217", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("JudgeParameterClass", "  \351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("JudgeParameterClass", "\346\234\200\345\244\247\347\274\272\351\231\267", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("JudgeParameterClass", "\345\271\266\344\270\224", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("JudgeParameterClass", "\345\271\266\344\270\224", 0, QApplication::UnicodeUTF8));
		label_33->setText(QApplication::translate("JudgeParameterClass", "\345\260\217\344\272\216", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("JudgeParameterClass", "\345\260\217\344\272\216", 0, QApplication::UnicodeUTF8));
		label_42->setText(QApplication::translate("JudgeParameterClass", "cm", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("JudgeParameterClass", "\346\210\226\350\200\205", 0, QApplication::UnicodeUTF8));
		label_32->setText(QApplication::translate("JudgeParameterClass", "\345\244\247\344\272\216", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("JudgeParameterClass", "cm", 0, QApplication::UnicodeUTF8));
		label_44->setText(QApplication::translate("JudgeParameterClass", "\347\255\211\344\272\216", 0, QApplication::UnicodeUTF8));
		label_45->setText(QApplication::translate("JudgeParameterClass", "\347\255\211\344\272\216", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("JudgeParameterClass", "\345\244\247\344\272\216", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("JudgeParameterClass", "\346\210\226\350\200\205", 0, QApplication::UnicodeUTF8));
		label_43->setText(QApplication::translate("JudgeParameterClass", "cm", 0, QApplication::UnicodeUTF8));
		label_46->setText(QApplication::translate("JudgeParameterClass", "\347\255\211\344\272\216", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("JudgeParameterClass", "\345\244\247\344\272\216", 0, QApplication::UnicodeUTF8));
		label_47->setText(QApplication::translate("JudgeParameterClass", "\347\255\211\344\272\216", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("JudgeParameterClass", "\345\244\247\344\272\216", 0, QApplication::UnicodeUTF8));
		tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("JudgeParameterClass", "\345\210\206\351\200\211", 0, QApplication::UnicodeUTF8));
		pushButtonSoundOpen->setText(QApplication::translate("JudgeParameterClass", "\345\274\200\345\220\257", 0, QApplication::UnicodeUTF8));
		pushButtonSoundClose->setText(QApplication::translate("JudgeParameterClass", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
		pushButtonLightOpen->setText(QApplication::translate("JudgeParameterClass", "\345\274\200\345\220\257", 0, QApplication::UnicodeUTF8));
		pushButtonLightClose->setText(QApplication::translate("JudgeParameterClass", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("JudgeParameterClass", "\345\243\260\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("JudgeParameterClass", "\345\205\211\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
		tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("JudgeParameterClass", "\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
		pushButtonSaveAs->setText(QApplication::translate("JudgeParameterClass", "\345\217\246\345\255\230\344\270\272", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("JudgeParameterClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("JudgeParameterClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class JudgeParameterClass: public Ui_JudgeParameterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JUDGEPARAMETER_H


/********************************************************************************
** Form generated from reading UI file 'orderparameter.ui'
**
** Created: Mon Jul 8 16:30:30 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORDERPARAMETER_H
#define UI_ORDERPARAMETER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_OrderParameterClass
{
public:
	QLabel *label;
	QLabel *label_2;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QLabel *label_6;
	QLabel *label_7;
	QLineEdit *lineEditOrderNumber;
	QPushButton *pushButtonDemand;
	QLineEdit *lineEditSteelNumber;
	QPushButton *pushButtonDiaAdd;
	QPushButton *pushButtonDiaCut;
	QPushButton *pushButtonLengthAdd;
	QPushButton *pushButtonLengthCut;
	QPushButton *pushButtonPlyAdd;
	QPushButton *pushButtonPlyCut;
	QPushButton *pushButtonRunkCut;
	QPushButton *pushButtonRunkAdd;
	QComboBox *comboBoxSteelDia;
	QComboBox *comboBoxSteelLength;
	QComboBox *comboBoxSteelPly;
	QComboBox *comboBoxSteelRank;
	QPushButton *pushButtonSaveAs;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;

	void setupUi(QDialog *OrderParameterClass)
	{
		if (OrderParameterClass->objectName().isEmpty())
			OrderParameterClass->setObjectName(QString::fromUtf8("OrderParameterClass"));
		OrderParameterClass->resize(380, 580);
		OrderParameterClass->setMinimumSize(QSize(380, 580));
		OrderParameterClass->setMaximumSize(QSize(380, 580));
		OrderParameterClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label = new QLabel(OrderParameterClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(140, 20, 95, 25));
		label->setMinimumSize(QSize(95, 25));
		label->setMaximumSize(QSize(95, 25));
		label->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_2 = new QLabel(OrderParameterClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(20, 70, 95, 25));
		label_2->setMinimumSize(QSize(95, 25));
		label_2->setMaximumSize(QSize(95, 25));
		label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_3 = new QLabel(OrderParameterClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(20, 130, 95, 25));
		label_3->setMinimumSize(QSize(95, 25));
		label_3->setMaximumSize(QSize(95, 25));
		label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_4 = new QLabel(OrderParameterClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(20, 200, 95, 25));
		label_4->setMinimumSize(QSize(95, 25));
		label_4->setMaximumSize(QSize(95, 25));
		label_4->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_5 = new QLabel(OrderParameterClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(20, 280, 95, 25));
		label_5->setMinimumSize(QSize(95, 25));
		label_5->setMaximumSize(QSize(95, 25));
		label_5->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_6 = new QLabel(OrderParameterClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(20, 360, 95, 25));
		label_6->setMinimumSize(QSize(95, 25));
		label_6->setMaximumSize(QSize(95, 25));
		label_6->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_7 = new QLabel(OrderParameterClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(20, 440, 95, 25));
		label_7->setMinimumSize(QSize(95, 25));
		label_7->setMaximumSize(QSize(95, 25));
		label_7->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		lineEditOrderNumber = new QLineEdit(OrderParameterClass);
		lineEditOrderNumber->setObjectName(QString::fromUtf8("lineEditOrderNumber"));
		lineEditOrderNumber->setGeometry(QRect(120, 70, 150, 25));
		lineEditOrderNumber->setMinimumSize(QSize(150, 25));
		lineEditOrderNumber->setMaximumSize(QSize(150, 25));
		lineEditOrderNumber->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"color: rgb(255, 255, 255);\n"
			"border: 2px solid rgb(255,255,255);\n"
			"}"));
		pushButtonDemand = new QPushButton(OrderParameterClass);
		pushButtonDemand->setObjectName(QString::fromUtf8("pushButtonDemand"));
		pushButtonDemand->setGeometry(QRect(300, 70, 60, 25));
		pushButtonDemand->setMinimumSize(QSize(60, 25));
		pushButtonDemand->setMaximumSize(QSize(60, 25));
		pushButtonDemand->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		lineEditSteelNumber = new QLineEdit(OrderParameterClass);
		lineEditSteelNumber->setObjectName(QString::fromUtf8("lineEditSteelNumber"));
		lineEditSteelNumber->setGeometry(QRect(120, 130, 200, 25));
		lineEditSteelNumber->setMinimumSize(QSize(200, 25));
		lineEditSteelNumber->setMaximumSize(QSize(200, 25));
		lineEditSteelNumber->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"color: rgb(255, 255, 255);\n"
			"border: 2px solid rgb(255,255,255);\n"
			"}"));
		pushButtonDiaAdd = new QPushButton(OrderParameterClass);
		pushButtonDiaAdd->setObjectName(QString::fromUtf8("pushButtonDiaAdd"));
		pushButtonDiaAdd->setGeometry(QRect(120, 230, 60, 22));
		pushButtonDiaAdd->setMinimumSize(QSize(60, 22));
		pushButtonDiaAdd->setMaximumSize(QSize(60, 22));
		pushButtonDiaAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonDiaCut = new QPushButton(OrderParameterClass);
		pushButtonDiaCut->setObjectName(QString::fromUtf8("pushButtonDiaCut"));
		pushButtonDiaCut->setGeometry(QRect(260, 230, 60, 22));
		pushButtonDiaCut->setMinimumSize(QSize(60, 22));
		pushButtonDiaCut->setMaximumSize(QSize(60, 22));
		pushButtonDiaCut->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonLengthAdd = new QPushButton(OrderParameterClass);
		pushButtonLengthAdd->setObjectName(QString::fromUtf8("pushButtonLengthAdd"));
		pushButtonLengthAdd->setGeometry(QRect(120, 310, 60, 22));
		pushButtonLengthAdd->setMinimumSize(QSize(60, 22));
		pushButtonLengthAdd->setMaximumSize(QSize(60, 22));
		pushButtonLengthAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonLengthCut = new QPushButton(OrderParameterClass);
		pushButtonLengthCut->setObjectName(QString::fromUtf8("pushButtonLengthCut"));
		pushButtonLengthCut->setGeometry(QRect(260, 310, 60, 22));
		pushButtonLengthCut->setMinimumSize(QSize(60, 22));
		pushButtonLengthCut->setMaximumSize(QSize(60, 22));
		pushButtonLengthCut->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonPlyAdd = new QPushButton(OrderParameterClass);
		pushButtonPlyAdd->setObjectName(QString::fromUtf8("pushButtonPlyAdd"));
		pushButtonPlyAdd->setGeometry(QRect(120, 390, 60, 22));
		pushButtonPlyAdd->setMinimumSize(QSize(60, 22));
		pushButtonPlyAdd->setMaximumSize(QSize(60, 22));
		pushButtonPlyAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonPlyCut = new QPushButton(OrderParameterClass);
		pushButtonPlyCut->setObjectName(QString::fromUtf8("pushButtonPlyCut"));
		pushButtonPlyCut->setGeometry(QRect(260, 390, 60, 22));
		pushButtonPlyCut->setMinimumSize(QSize(60, 22));
		pushButtonPlyCut->setMaximumSize(QSize(60, 22));
		pushButtonPlyCut->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonRunkCut = new QPushButton(OrderParameterClass);
		pushButtonRunkCut->setObjectName(QString::fromUtf8("pushButtonRunkCut"));
		pushButtonRunkCut->setGeometry(QRect(260, 470, 60, 22));
		pushButtonRunkCut->setMinimumSize(QSize(60, 22));
		pushButtonRunkCut->setMaximumSize(QSize(60, 22));
		pushButtonRunkCut->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonRunkAdd = new QPushButton(OrderParameterClass);
		pushButtonRunkAdd->setObjectName(QString::fromUtf8("pushButtonRunkAdd"));
		pushButtonRunkAdd->setGeometry(QRect(120, 470, 60, 22));
		pushButtonRunkAdd->setMinimumSize(QSize(60, 22));
		pushButtonRunkAdd->setMaximumSize(QSize(60, 22));
		pushButtonRunkAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		comboBoxSteelDia = new QComboBox(OrderParameterClass);
		comboBoxSteelDia->setObjectName(QString::fromUtf8("comboBoxSteelDia"));
		comboBoxSteelDia->setGeometry(QRect(120, 200, 200, 25));
		comboBoxSteelDia->setMinimumSize(QSize(200, 25));
		comboBoxSteelDia->setMaximumSize(QSize(200, 25));
		comboBoxSteelDia->setStyleSheet(QString::fromUtf8("QComboBox{\n"
			"color: rgb(255, 255, 255);\n"
			"border: 2px solid rgb(255,255,255);\n"
			"}"));
		comboBoxSteelDia->setEditable(true);
		comboBoxSteelLength = new QComboBox(OrderParameterClass);
		comboBoxSteelLength->setObjectName(QString::fromUtf8("comboBoxSteelLength"));
		comboBoxSteelLength->setGeometry(QRect(120, 280, 200, 25));
		comboBoxSteelLength->setMinimumSize(QSize(200, 25));
		comboBoxSteelLength->setMaximumSize(QSize(200, 25));
		comboBoxSteelLength->setStyleSheet(QString::fromUtf8("QComboBox{\n"
			"color: rgb(255, 255, 255);\n"
			"border: 2px solid rgb(255,255,255);\n"
			"}"));
		comboBoxSteelLength->setEditable(true);
		comboBoxSteelPly = new QComboBox(OrderParameterClass);
		comboBoxSteelPly->setObjectName(QString::fromUtf8("comboBoxSteelPly"));
		comboBoxSteelPly->setGeometry(QRect(120, 360, 200, 25));
		comboBoxSteelPly->setMinimumSize(QSize(200, 25));
		comboBoxSteelPly->setMaximumSize(QSize(200, 25));
		comboBoxSteelPly->setStyleSheet(QString::fromUtf8("QComboBox{\n"
			"color: rgb(255, 255, 255);\n"
			"border: 2px solid rgb(255,255,255);\n"
			"}"));
		comboBoxSteelPly->setEditable(true);
		comboBoxSteelRank = new QComboBox(OrderParameterClass);
		comboBoxSteelRank->setObjectName(QString::fromUtf8("comboBoxSteelRank"));
		comboBoxSteelRank->setGeometry(QRect(120, 440, 200, 25));
		comboBoxSteelRank->setMinimumSize(QSize(200, 25));
		comboBoxSteelRank->setMaximumSize(QSize(200, 25));
		comboBoxSteelRank->setStyleSheet(QString::fromUtf8("QComboBox{\n"
			"color: rgb(255, 255, 255);\n"
			"border: 2px solid rgb(255,255,255);\n"
			"}"));
		comboBoxSteelRank->setEditable(true);
		pushButtonSaveAs = new QPushButton(OrderParameterClass);
		pushButtonSaveAs->setObjectName(QString::fromUtf8("pushButtonSaveAs"));
		pushButtonSaveAs->setGeometry(QRect(50, 530, 60, 22));
		pushButtonSaveAs->setMinimumSize(QSize(60, 22));
		pushButtonSaveAs->setMaximumSize(QSize(60, 22));
		pushButtonSaveAs->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonYes = new QPushButton(OrderParameterClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(170, 530, 60, 22));
		pushButtonYes->setMinimumSize(QSize(60, 22));
		pushButtonYes->setMaximumSize(QSize(60, 22));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonNo = new QPushButton(OrderParameterClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(290, 530, 60, 22));
		pushButtonNo->setMinimumSize(QSize(60, 22));
		pushButtonNo->setMaximumSize(QSize(60, 22));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label_9 = new QLabel(OrderParameterClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(330, 360, 25, 25));
		label_9->setMinimumSize(QSize(25, 25));
		label_9->setMaximumSize(QSize(25, 25));
		label_9->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_10 = new QLabel(OrderParameterClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(330, 280, 25, 25));
		label_10->setMinimumSize(QSize(25, 25));
		label_10->setMaximumSize(QSize(25, 25));
		label_10->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_11 = new QLabel(OrderParameterClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(330, 200, 25, 25));
		label_11->setMinimumSize(QSize(25, 25));
		label_11->setMaximumSize(QSize(25, 25));
		label_11->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));

		retranslateUi(OrderParameterClass);

		QMetaObject::connectSlotsByName(OrderParameterClass);
	} // setupUi

	void retranslateUi(QDialog *OrderParameterClass)
	{
		OrderParameterClass->setWindowTitle(QApplication::translate("OrderParameterClass", "OrderParameter", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("OrderParameterClass", "\350\256\242\345\215\225\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("OrderParameterClass", "\350\256\242 \345\215\225 \345\217\267", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("OrderParameterClass", "\351\222\242\347\256\241\346\225\260\351\207\217", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("OrderParameterClass", "\351\222\242\347\256\241\347\233\264\345\276\204", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("OrderParameterClass", "\351\222\242\347\256\241\351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("OrderParameterClass", "\351\222\242\347\256\241\345\243\201\345\216\232", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("OrderParameterClass", "\351\222\242    \347\272\247", 0, QApplication::UnicodeUTF8));
		lineEditOrderNumber->setText(QString());
		pushButtonDemand->setText(QApplication::translate("OrderParameterClass", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
		lineEditSteelNumber->setText(QString());
		pushButtonDiaAdd->setText(QApplication::translate("OrderParameterClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		pushButtonDiaCut->setText(QApplication::translate("OrderParameterClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		pushButtonLengthAdd->setText(QApplication::translate("OrderParameterClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		pushButtonLengthCut->setText(QApplication::translate("OrderParameterClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		pushButtonPlyAdd->setText(QApplication::translate("OrderParameterClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		pushButtonPlyCut->setText(QApplication::translate("OrderParameterClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		pushButtonRunkCut->setText(QApplication::translate("OrderParameterClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		pushButtonRunkAdd->setText(QApplication::translate("OrderParameterClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		pushButtonSaveAs->setText(QApplication::translate("OrderParameterClass", "\345\217\246\345\255\230\344\270\272", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("OrderParameterClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("OrderParameterClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("OrderParameterClass", "mm", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("OrderParameterClass", "mm", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("OrderParameterClass", "mm", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class OrderParameterClass: public Ui_OrderParameterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORDERPARAMETER_H

/********************************************************************************
** Form generated from reading UI file 'showparameter.ui'
**
** Created: Thu Jul 4 18:02:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWPARAMETER_H
#define UI_SHOWPARAMETER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShowParameterClass
{
public:
    QLabel *label;
    QWidget *widgetX;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEditLength;
    QWidget *widgetY;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *lineEditToleranceMax;
    QLabel *label_8;
    QLineEdit *lineEditToleranceMin;
    QLabel *label_9;
    QLineEdit *lineEditLimitsMax;
    QLineEdit *lineEditLimitsMin;
    QLabel *label_10;
    QLineEdit *lineEditPly;
    QPushButton *pushButtonSaveAs;
    QPushButton *pushButtonYes;
    QPushButton *pushButtonNo;

    void setupUi(QDialog *ShowParameterClass)
    {
        if (ShowParameterClass->objectName().isEmpty())
            ShowParameterClass->setObjectName(QString::fromUtf8("ShowParameterClass"));
		ShowParameterClass->resize(392, 449);
        ShowParameterClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
        label = new QLabel(ShowParameterClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 20, 95, 25));
        label->setMinimumSize(QSize(95, 25));
        label->setMaximumSize(QSize(95, 25));
        label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        widgetX = new QWidget(ShowParameterClass);
        widgetX->setObjectName(QString::fromUtf8("widgetX"));
        widgetX->setGeometry(QRect(10, 60, 371, 80));
        widgetX->setStyleSheet(QString::fromUtf8("QWidget#widgetX{\n"
"border:2px solid rgb(255,255,255);\n"
"}"));
        label_2 = new QLabel(widgetX);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 60, 25));
        label_2->setMinimumSize(QSize(60, 25));
        label_2->setMaximumSize(QSize(60, 25));
        label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        label_3 = new QLabel(widgetX);
        label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(10, 40, 160, 25));
		label_3->setMinimumSize(QSize(160, 25));
		label_3->setMaximumSize(QSize(160, 25));
        label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        lineEditLength = new QLineEdit(widgetX);
        lineEditLength->setObjectName(QString::fromUtf8("lineEditLength"));
		lineEditLength->setGeometry(QRect(180, 40, 171, 25));
		lineEditLength->setMinimumSize(QSize(171, 25));
		lineEditLength->setMaximumSize(QSize(171, 25));
        lineEditLength->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"color: rgb(255, 255, 255);\n"
"border:2px solid rgb(255,255,255);\n"
"}"));
        widgetY = new QWidget(ShowParameterClass);
        widgetY->setObjectName(QString::fromUtf8("widgetY"));
		widgetY->setGeometry(QRect(10, 160, 371, 231));
        widgetY->setStyleSheet(QString::fromUtf8("QWidget#widgetY{\n"
"border:2px solid rgb(255,255,255);\n"
"}"));
        label_4 = new QLabel(widgetY);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 10, 60, 25));
        label_4->setMinimumSize(QSize(60, 25));
        label_4->setMaximumSize(QSize(60, 25));
        label_4->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        label_6 = new QLabel(widgetY);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(190, 40, 50, 25));
        label_6->setMinimumSize(QSize(50, 25));
        label_6->setMaximumSize(QSize(50, 25));
        label_6->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        label_7 = new QLabel(widgetY);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(290, 40, 50, 25));
        label_7->setMinimumSize(QSize(50, 25));
        label_7->setMaximumSize(QSize(50, 25));
        label_7->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        lineEditToleranceMax = new QLineEdit(widgetY);
        lineEditToleranceMax->setObjectName(QString::fromUtf8("lineEditToleranceMax"));
		lineEditToleranceMax->setGeometry(QRect(280, 80, 71, 25));
        lineEditToleranceMax->setMinimumSize(QSize(50, 25));
        lineEditToleranceMax->setMaximumSize(QSize(200, 25));
        lineEditToleranceMax->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"color: rgb(255, 255, 255);\n"
"border:2px solid rgb(255,255,255);\n"
"}"));
        label_8 = new QLabel(widgetY);
        label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(10, 80, 160, 25));
        label_8->setMinimumSize(QSize(160, 25));
        label_8->setMaximumSize(QSize(160, 25));
        label_8->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        lineEditToleranceMin = new QLineEdit(widgetY);
        lineEditToleranceMin->setObjectName(QString::fromUtf8("lineEditToleranceMin"));
		lineEditToleranceMin->setGeometry(QRect(180, 80, 71, 25));
        lineEditToleranceMin->setMinimumSize(QSize(50, 25));
        lineEditToleranceMin->setMaximumSize(QSize(200, 25));
        lineEditToleranceMin->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"color: rgb(255, 255, 255);\n"
"border:2px solid rgb(255,255,255);\n"
"}"));
        label_9 = new QLabel(widgetY);
        label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(10, 130, 160, 25));
        label_9->setMinimumSize(QSize(160, 25));
        label_9->setMaximumSize(QSize(160, 25));
        label_9->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        lineEditLimitsMax = new QLineEdit(widgetY);
        lineEditLimitsMax->setObjectName(QString::fromUtf8("lineEditLimitsMax"));
		lineEditLimitsMax->setGeometry(QRect(280, 130, 71, 25));
        lineEditLimitsMax->setMinimumSize(QSize(50, 25));
        lineEditLimitsMax->setMaximumSize(QSize(200, 25));
        lineEditLimitsMax->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"color: rgb(255, 255, 255);\n"
"border:2px solid rgb(255,255,255);\n"
"}"));
        lineEditLimitsMin = new QLineEdit(widgetY);
        lineEditLimitsMin->setObjectName(QString::fromUtf8("lineEditLimitsMin"));
		lineEditLimitsMin->setGeometry(QRect(180, 130, 71, 25));
        lineEditLimitsMin->setMinimumSize(QSize(50, 25));
        lineEditLimitsMin->setMaximumSize(QSize(200, 25));
        lineEditLimitsMin->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"color: rgb(255, 255, 255);\n"
"border:2px solid rgb(255,255,255);\n"
"}"));
        label_10 = new QLabel(widgetY);
        label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(90, 180, 80, 25));
		label_10->setMinimumSize(QSize(80, 25));
		label_10->setMaximumSize(QSize(80, 25));
        label_10->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: rgb(255, 255, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"}"));
        lineEditPly = new QLineEdit(widgetY);
        lineEditPly->setObjectName(QString::fromUtf8("lineEditPly"));
		lineEditPly->setEnabled(false);
		lineEditPly->setGeometry(QRect(180, 180, 171, 25));
		lineEditPly->setMinimumSize(QSize(171, 25));
		lineEditPly->setMaximumSize(QSize(171, 25));
        lineEditPly->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"color: rgb(255, 255, 255);\n"
			"border:2px solid rgb(110,110,110);\n"
"}"));
        pushButtonSaveAs = new QPushButton(ShowParameterClass);
        pushButtonSaveAs->setObjectName(QString::fromUtf8("pushButtonSaveAs"));
		pushButtonSaveAs->setGeometry(QRect(50, 410, 60, 25));
        pushButtonSaveAs->setMinimumSize(QSize(60, 25));
        pushButtonSaveAs->setMaximumSize(QSize(60, 25));
        pushButtonSaveAs->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        pushButtonYes = new QPushButton(ShowParameterClass);
        pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(170, 410, 60, 25));
        pushButtonYes->setMinimumSize(QSize(60, 25));
        pushButtonYes->setMaximumSize(QSize(60, 25));
        pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        pushButtonNo = new QPushButton(ShowParameterClass);
        pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(290, 410, 60, 25));
        pushButtonNo->setMinimumSize(QSize(60, 25));
        pushButtonNo->setMaximumSize(QSize(60, 25));
        pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        retranslateUi(ShowParameterClass);

        QMetaObject::connectSlotsByName(ShowParameterClass);
    } // setupUi

    void retranslateUi(QDialog *ShowParameterClass)
    {
        ShowParameterClass->setWindowTitle(QApplication::translate("ShowParameterClass", "ShowParameter", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ShowParameterClass", "\346\230\276\347\244\272\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ShowParameterClass", "  X\350\275\264", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("ShowParameterClass", "\346\234\200\345\244\247\346\243\200\346\265\213\351\225\277\345\272\246(mm)", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ShowParameterClass", "  Y\350\275\264", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ShowParameterClass", " Min", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ShowParameterClass", " Max", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ShowParameterClass", "\345\243\201\345\216\232\345\205\201\350\256\270\345\205\254\345\267\256(mm)", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("ShowParameterClass", "\345\243\201\345\216\232\346\265\213\351\207\217\350\214\203\345\233\264(mm)", 0, QApplication::UnicodeUTF8));
        lineEditLimitsMin->setText(QString());
        label_10->setText(QApplication::translate("ShowParameterClass", "\345\243\201\345\216\232(mm)", 0, QApplication::UnicodeUTF8));
        pushButtonSaveAs->setText(QApplication::translate("ShowParameterClass", "\345\217\246\345\255\230\344\270\272", 0, QApplication::UnicodeUTF8));
        pushButtonYes->setText(QApplication::translate("ShowParameterClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        pushButtonNo->setText(QApplication::translate("ShowParameterClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ShowParameterClass: public Ui_ShowParameterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPARAMETER_H

/********************************************************************************
** Form generated from reading UI file 'ultrasoundparameter.ui'
**
** Created: Thu Jan 9 16:02:43 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ULTRASOUNDPARAMETER_H
#define UI_ULTRASOUNDPARAMETER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UltrasoundParameterClass
{
public:
	QWidget *widget;
	QPushButton *pushButtonGain;
	QPushButton *pushButtonCGain;
	QPushButton *pushButtonSoundSpeed;
	QPushButton *pushButtonBandWidth;
	QPushButton *pushButtonGateSet;
	QPushButton *pushButtonBurstDelay;
	QLabel *label_2;
	QLabel *label;
	QWidget *widget_2;
	QPushButton *pushButtonMeasureMode;
	QPushButton *pushButtonStaNoise;
	QLabel *label_3;
	QPushButton *pushButtonDeadZone;
	QWidget *widget_3;
	QPushButton *pushButtonReFreq;
	QPushButton *pushButtonImpedanceSet;
	QPushButton *pushButtonProbeSet;
	QLabel *label_4;
	QPushButton *pushButtonSaveAs;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;

	void setupUi(QDialog *UltrasoundParameterClass)
	{
		if (UltrasoundParameterClass->objectName().isEmpty())
			UltrasoundParameterClass->setObjectName(QString::fromUtf8("UltrasoundParameterClass"));
		UltrasoundParameterClass->resize(384, 455);
		UltrasoundParameterClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		widget = new QWidget(UltrasoundParameterClass);
		widget->setObjectName(QString::fromUtf8("widget"));
		widget->setGeometry(QRect(5, 50, 371, 131));
		widget->setStyleSheet(QString::fromUtf8("QWidget#widget{\n"
			"border:2px solid rgb(255,255,255);\n"
			"}"));
		pushButtonGain = new QPushButton(widget);
		pushButtonGain->setObjectName(QString::fromUtf8("pushButtonGain"));
		pushButtonGain->setGeometry(QRect(10, 50, 110, 25));
		pushButtonGain->setMinimumSize(QSize(110, 25));
		pushButtonGain->setMaximumSize(QSize(110, 25));
		pushButtonGain->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonCGain = new QPushButton(widget);
		pushButtonCGain->setObjectName(QString::fromUtf8("pushButtonCGain"));
		pushButtonCGain->setGeometry(QRect(130, 50, 110, 25));
		pushButtonCGain->setMinimumSize(QSize(110, 25));
		pushButtonCGain->setMaximumSize(QSize(110, 25));
		pushButtonCGain->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonSoundSpeed = new QPushButton(widget);
		pushButtonSoundSpeed->setObjectName(QString::fromUtf8("pushButtonSoundSpeed"));
		pushButtonSoundSpeed->setGeometry(QRect(250, 50, 110, 25));
		pushButtonSoundSpeed->setMinimumSize(QSize(110, 25));
		pushButtonSoundSpeed->setMaximumSize(QSize(110, 25));
		pushButtonSoundSpeed->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonBandWidth = new QPushButton(widget);
		pushButtonBandWidth->setObjectName(QString::fromUtf8("pushButtonBandWidth"));
		pushButtonBandWidth->setGeometry(QRect(250, 90, 110, 25));
		pushButtonBandWidth->setMinimumSize(QSize(110, 25));
		pushButtonBandWidth->setMaximumSize(QSize(110, 25));
		pushButtonBandWidth->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonGateSet = new QPushButton(widget);
		pushButtonGateSet->setObjectName(QString::fromUtf8("pushButtonGateSet"));
		pushButtonGateSet->setGeometry(QRect(130, 90, 110, 25));
		pushButtonGateSet->setMinimumSize(QSize(110, 25));
		pushButtonGateSet->setMaximumSize(QSize(110, 25));
		pushButtonGateSet->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonBurstDelay = new QPushButton(widget);
		pushButtonBurstDelay->setObjectName(QString::fromUtf8("pushButtonBurstDelay"));
		pushButtonBurstDelay->setGeometry(QRect(10, 90, 110, 25));
		pushButtonBurstDelay->setMinimumSize(QSize(110, 25));
		pushButtonBurstDelay->setMaximumSize(QSize(110, 25));
		pushButtonBurstDelay->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_2 = new QLabel(widget);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(30, 10, 50, 25));
		label_2->setMinimumSize(QSize(50, 25));
		label_2->setMaximumSize(QSize(50, 25));
		label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label = new QLabel(UltrasoundParameterClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(140, 10, 95, 25));
		label->setMinimumSize(QSize(95, 25));
		label->setMaximumSize(QSize(95, 25));
		label->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		widget_2 = new QWidget(UltrasoundParameterClass);
		widget_2->setObjectName(QString::fromUtf8("widget_2"));
		widget_2->setGeometry(QRect(5, 190, 371, 91));
		widget_2->setStyleSheet(QString::fromUtf8("QWidget#widget_2{\n"
			"border:2px solid rgb(255,255,255);\n"
			"}"));
		pushButtonMeasureMode = new QPushButton(widget_2);
		pushButtonMeasureMode->setObjectName(QString::fromUtf8("pushButtonMeasureMode"));
		pushButtonMeasureMode->setGeometry(QRect(10, 50, 110, 25));
		pushButtonMeasureMode->setMinimumSize(QSize(110, 25));
		pushButtonMeasureMode->setMaximumSize(QSize(110, 25));
		pushButtonMeasureMode->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonStaNoise = new QPushButton(widget_2);
		pushButtonStaNoise->setObjectName(QString::fromUtf8("pushButtonStaNoise"));
		pushButtonStaNoise->setGeometry(QRect(130, 50, 110, 25));
		pushButtonStaNoise->setMinimumSize(QSize(110, 25));
		pushButtonStaNoise->setMaximumSize(QSize(110, 25));
		pushButtonStaNoise->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_3 = new QLabel(widget_2);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(30, 10, 50, 25));
		label_3->setMinimumSize(QSize(50, 25));
		label_3->setMaximumSize(QSize(50, 25));
		label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		pushButtonDeadZone = new QPushButton(widget_2);
		pushButtonDeadZone->setObjectName(QString::fromUtf8("pushButtonDeadZone"));
		pushButtonDeadZone->setGeometry(QRect(250, 50, 110, 25));
		pushButtonDeadZone->setMinimumSize(QSize(110, 25));
		pushButtonDeadZone->setMaximumSize(QSize(110, 25));
		pushButtonDeadZone->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		widget_3 = new QWidget(UltrasoundParameterClass);
		widget_3->setObjectName(QString::fromUtf8("widget_3"));
		widget_3->setGeometry(QRect(5, 290, 371, 91));
		widget_3->setStyleSheet(QString::fromUtf8("QWidget#widget_3{\n"
			"border:2px solid rgb(255,255,255);\n"
			"}"));
		pushButtonReFreq = new QPushButton(widget_3);
		pushButtonReFreq->setObjectName(QString::fromUtf8("pushButtonReFreq"));
		pushButtonReFreq->setGeometry(QRect(10, 50, 110, 25));
		pushButtonReFreq->setMinimumSize(QSize(110, 25));
		pushButtonReFreq->setMaximumSize(QSize(110, 25));
		pushButtonReFreq->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonImpedanceSet = new QPushButton(widget_3);
		pushButtonImpedanceSet->setObjectName(QString::fromUtf8("pushButtonImpedanceSet"));
		pushButtonImpedanceSet->setGeometry(QRect(130, 50, 110, 25));
		pushButtonImpedanceSet->setMinimumSize(QSize(110, 25));
		pushButtonImpedanceSet->setMaximumSize(QSize(110, 25));
		pushButtonImpedanceSet->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonProbeSet = new QPushButton(widget_3);
		pushButtonProbeSet->setObjectName(QString::fromUtf8("pushButtonProbeSet"));
		pushButtonProbeSet->setGeometry(QRect(250, 50, 110, 25));
		pushButtonProbeSet->setMinimumSize(QSize(110, 25));
		pushButtonProbeSet->setMaximumSize(QSize(110, 25));
		pushButtonProbeSet->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font:15pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_4 = new QLabel(widget_3);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(30, 10, 50, 25));
		label_4->setMinimumSize(QSize(50, 25));
		label_4->setMaximumSize(QSize(50, 25));
		label_4->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		pushButtonSaveAs = new QPushButton(UltrasoundParameterClass);
		pushButtonSaveAs->setObjectName(QString::fromUtf8("pushButtonSaveAs"));
		pushButtonSaveAs->setGeometry(QRect(50, 410, 60, 25));
		pushButtonSaveAs->setMinimumSize(QSize(60, 25));
		pushButtonSaveAs->setMaximumSize(QSize(60, 25));
		pushButtonSaveAs->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonYes = new QPushButton(UltrasoundParameterClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(160, 410, 60, 25));
		pushButtonYes->setMinimumSize(QSize(60, 25));
		pushButtonYes->setMaximumSize(QSize(60, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonNo = new QPushButton(UltrasoundParameterClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(270, 410, 60, 25));
		pushButtonNo->setMinimumSize(QSize(60, 25));
		pushButtonNo->setMaximumSize(QSize(60, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

		retranslateUi(UltrasoundParameterClass);

		QMetaObject::connectSlotsByName(UltrasoundParameterClass);
	} // setupUi

	void retranslateUi(QDialog *UltrasoundParameterClass)
	{
		UltrasoundParameterClass->setWindowTitle(QApplication::translate("UltrasoundParameterClass", "UltrasoundParameter", 0, QApplication::UnicodeUTF8));
		pushButtonGain->setText(QApplication::translate("UltrasoundParameterClass", "\345\242\236\347\233\212", 0, QApplication::UnicodeUTF8));
		pushButtonCGain->setText(QApplication::translate("UltrasoundParameterClass", "\345\217\202\350\200\203\345\242\236\347\233\212", 0, QApplication::UnicodeUTF8));
		pushButtonSoundSpeed->setText(QApplication::translate("UltrasoundParameterClass", "\345\243\260\351\200\237", 0, QApplication::UnicodeUTF8));
		pushButtonBandWidth->setText(QApplication::translate("UltrasoundParameterClass", "\346\212\221\345\210\266", 0, QApplication::UnicodeUTF8));
		pushButtonGateSet->setText(QApplication::translate("UltrasoundParameterClass", "\351\227\270\351\227\250\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
		pushButtonBurstDelay->setText(QApplication::translate("UltrasoundParameterClass", "\345\273\266\350\277\237", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("UltrasoundParameterClass", "\345\270\270\350\247\204", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("UltrasoundParameterClass", "\350\266\205\345\243\260\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
		pushButtonMeasureMode->setText(QApplication::translate("UltrasoundParameterClass", "\351\227\270\351\227\250\345\274\200\345\205\263", 0, QApplication::UnicodeUTF8));
		pushButtonStaNoise->setText(QApplication::translate("UltrasoundParameterClass", "\347\273\237\350\256\241\351\231\215\345\231\252\346\225\260", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("UltrasoundParameterClass", "\350\257\204\345\210\244", 0, QApplication::UnicodeUTF8));
		pushButtonDeadZone->setText(QApplication::translate("UltrasoundParameterClass", "\347\233\262\345\214\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
		pushButtonReFreq->setText(QApplication::translate("UltrasoundParameterClass", "\351\207\215\345\244\215\351\242\221\347\216\207", 0, QApplication::UnicodeUTF8));
		pushButtonImpedanceSet->setText(QApplication::translate("UltrasoundParameterClass", "\351\230\273\346\212\227\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
		pushButtonProbeSet->setText(QApplication::translate("UltrasoundParameterClass", "\346\216\242\345\244\264\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("UltrasoundParameterClass", "\345\205\266\344\273\226", 0, QApplication::UnicodeUTF8));
		pushButtonSaveAs->setText(QApplication::translate("UltrasoundParameterClass", "\345\217\246\345\255\230\344\270\272", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("UltrasoundParameterClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("UltrasoundParameterClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class UltrasoundParameterClass: public Ui_UltrasoundParameterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ULTRASOUNDPARAMETER_H



/********************************************************************************
** Form generated from reading UI file 'burstdelay.ui'
**
** Created: Wed Jun 18 22:15:46 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BURSTDELAY_H
#define UI_BURSTDELAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_BurstDelayClass
{
public:
	QCheckBox *checkBoxProbe30;
	QCheckBox *checkBoxProbe20;
	QLineEdit *lineEditProbe24;
	QRadioButton *radioButtonAny;
	QCheckBox *checkBoxProbe18;
	QCheckBox *checkBoxProbe13;
	QLineEdit *lineEditProbe29;
	QLabel *label;
	QCheckBox *checkBoxProbe11;
	QPushButton *pushButtonYes;
	QCheckBox *checkBoxProbe28;
	QLineEdit *lineEditProbe1;
	QLineEdit *lineEditAll;
	QCheckBox *checkBoxProbe8;
	QLineEdit *lineEditProbe28;
	QCheckBox *checkBoxProbe16;
	QLineEdit *lineEditProbe22;
	QLineEdit *lineEditProbe6;
	QCheckBox *checkBoxProbe23;
	QLineEdit *lineEditProbe9;
	QLabel *label_2;
	QLineEdit *lineEditProbe21;
	QCheckBox *checkBoxProbe24;
	QLineEdit *lineEditProbe23;
	QCheckBox *checkBoxProbe1;
	QLineEdit *lineEditProbe11;
	QCheckBox *checkBoxProbe9;
	QLineEdit *lineEditProbe25;
	QLineEdit *lineEditProbe26;
	QCheckBox *checkBoxProbe21;
	QLineEdit *lineEditProbe20;
	QLineEdit *lineEditProbe3;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe6;
	QCheckBox *checkBoxProbe22;
	QLineEdit *lineEditProbe2;
	QCheckBox *checkBoxProbe5;
	QCheckBox *checkBoxProbe7;
	QLineEdit *lineEditProbe8;
	QCheckBox *checkBoxProbe10;
	QLineEdit *lineEditProbe7;
	QCheckBox *checkBoxProbe17;
	QPushButton *pushButtonNo;
	QCheckBox *checkBoxProbe3;
	QLineEdit *lineEditProbe4;
	QLineEdit *lineEditProbe27;
	QCheckBox *checkBoxProbe25;
	QLineEdit *lineEditAny;
	QCheckBox *checkBoxProbe12;
	QCheckBox *checkBoxProbe26;
	QLineEdit *lineEditProbe14;
	QLineEdit *lineEditProbe16;
	QLineEdit *lineEditProbe18;
	QCheckBox *checkBoxProbe19;
	QLineEdit *lineEditProbe10;
	QLineEdit *lineEditProbe19;
	QLineEdit *lineEditProbe13;
	QCheckBox *checkBoxProbe2;
	QLineEdit *lineEditProbe17;
	QLineEdit *lineEditProbe15;
	QLineEdit *lineEditProbe5;
	QCheckBox *checkBoxProbe29;
	QCheckBox *checkBoxProbe4;
	QCheckBox *checkBoxProbe15;
	QRadioButton *radioButtonAll;
	QLineEdit *lineEditProbe30;
	QCheckBox *checkBoxProbe27;
	QLineEdit *lineEditProbe12;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QLabel *label_6;
	QLabel *label_7;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QLabel *label_14;
	QLabel *label_15;
	QLabel *label_16;
	QLabel *label_17;
	QLabel *label_18;
	QLabel *label_19;
	QLabel *label_20;
	QLabel *label_21;
	QLabel *label_22;
	QLabel *label_23;
	QLabel *label_24;
	QLabel *label_25;
	QLabel *label_26;
	QLabel *label_27;
	QLabel *label_28;
	QLabel *label_29;
	QLabel *label_30;
	QLabel *label_31;
	QLabel *label_32;
	QLabel *label_33;
	QPushButton *pushButtonAllIn;
	QPushButton *pushButtonAnyIn;
	QLabel *label_34;
	QLineEdit *lineEditProbe31;
	QCheckBox *checkBoxProbe31;
	QLineEdit *lineEditProbe32;
	QCheckBox *checkBoxProbe35;
	QLineEdit *lineEditProbe33;
	QLineEdit *lineEditProbe34;
	QLabel *label_35;
	QCheckBox *checkBoxProbe32;
	QCheckBox *checkBoxProbe34;
	QLabel *label_36;
	QLineEdit *lineEditProbe36;
	QCheckBox *checkBoxProbe36;
	QLabel *label_37;
	QCheckBox *checkBoxProbe33;
	QLabel *label_38;
	QLineEdit *lineEditProbe35;
	QLabel *label_39;

	void setupUi(QDialog *BurstDelayClass)
	{
		if (BurstDelayClass->objectName().isEmpty())
			BurstDelayClass->setObjectName(QString::fromUtf8("BurstDelayClass"));
		BurstDelayClass->resize(800, 600);
		BurstDelayClass->setMinimumSize(QSize(800, 600));
		BurstDelayClass->setMaximumSize(QSize(800, 600));
		BurstDelayClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		checkBoxProbe30 = new QCheckBox(BurstDelayClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(410, 510, 50, 20));
		checkBoxProbe30->setMinimumSize(QSize(50, 20));
		checkBoxProbe30->setMaximumSize(QSize(50, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe20 = new QCheckBox(BurstDelayClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(220, 510, 50, 20));
		checkBoxProbe20->setMinimumSize(QSize(50, 20));
		checkBoxProbe20->setMaximumSize(QSize(50, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe24 = new QLineEdit(BurstDelayClass);
		lineEditProbe24->setObjectName(QString::fromUtf8("lineEditProbe24"));
		lineEditProbe24->setGeometry(QRect(470, 270, 80, 20));
		lineEditProbe24->setMinimumSize(QSize(80, 20));
		lineEditProbe24->setMaximumSize(QSize(80, 20));
		lineEditProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		radioButtonAny = new QRadioButton(BurstDelayClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(40, 100, 89, 16));
		radioButtonAny->setMinimumSize(QSize(89, 16));
		radioButtonAny->setMaximumSize(QSize(89, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe18 = new QCheckBox(BurstDelayClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(220, 430, 50, 20));
		checkBoxProbe18->setMinimumSize(QSize(50, 20));
		checkBoxProbe18->setMaximumSize(QSize(50, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13 = new QCheckBox(BurstDelayClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(220, 230, 50, 20));
		checkBoxProbe13->setMinimumSize(QSize(50, 20));
		checkBoxProbe13->setMaximumSize(QSize(50, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe29 = new QLineEdit(BurstDelayClass);
		lineEditProbe29->setObjectName(QString::fromUtf8("lineEditProbe29"));
		lineEditProbe29->setGeometry(QRect(470, 470, 80, 20));
		lineEditProbe29->setMinimumSize(QSize(80, 20));
		lineEditProbe29->setMaximumSize(QSize(80, 20));
		lineEditProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label = new QLabel(BurstDelayClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(210, 60, 30, 20));
		label->setMinimumSize(QSize(30, 20));
		label->setMaximumSize(QSize(30, 20));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(BurstDelayClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(220, 150, 50, 20));
		checkBoxProbe11->setMinimumSize(QSize(50, 20));
		checkBoxProbe11->setMaximumSize(QSize(50, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(BurstDelayClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(600, 550, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe28 = new QCheckBox(BurstDelayClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(410, 430, 50, 20));
		checkBoxProbe28->setMinimumSize(QSize(50, 20));
		checkBoxProbe28->setMaximumSize(QSize(50, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe1 = new QLineEdit(BurstDelayClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setGeometry(QRect(90, 150, 80, 20));
		lineEditProbe1->setMinimumSize(QSize(80, 20));
		lineEditProbe1->setMaximumSize(QSize(80, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAll = new QLineEdit(BurstDelayClass);
		lineEditAll->setObjectName(QString::fromUtf8("lineEditAll"));
		lineEditAll->setGeometry(QRect(130, 60, 80, 20));
		lineEditAll->setMinimumSize(QSize(80, 20));
		lineEditAll->setMaximumSize(QSize(80, 20));
		lineEditAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe8 = new QCheckBox(BurstDelayClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(40, 430, 50, 20));
		checkBoxProbe8->setMinimumSize(QSize(50, 20));
		checkBoxProbe8->setMaximumSize(QSize(50, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe28 = new QLineEdit(BurstDelayClass);
		lineEditProbe28->setObjectName(QString::fromUtf8("lineEditProbe28"));
		lineEditProbe28->setGeometry(QRect(470, 430, 80, 20));
		lineEditProbe28->setMinimumSize(QSize(80, 20));
		lineEditProbe28->setMaximumSize(QSize(80, 20));
		lineEditProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe16 = new QCheckBox(BurstDelayClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(220, 350, 50, 20));
		checkBoxProbe16->setMinimumSize(QSize(50, 20));
		checkBoxProbe16->setMaximumSize(QSize(50, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe22 = new QLineEdit(BurstDelayClass);
		lineEditProbe22->setObjectName(QString::fromUtf8("lineEditProbe22"));
		lineEditProbe22->setGeometry(QRect(470, 190, 80, 20));
		lineEditProbe22->setMinimumSize(QSize(80, 20));
		lineEditProbe22->setMaximumSize(QSize(80, 20));
		lineEditProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6 = new QLineEdit(BurstDelayClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setGeometry(QRect(90, 350, 80, 20));
		lineEditProbe6->setMinimumSize(QSize(80, 20));
		lineEditProbe6->setMaximumSize(QSize(80, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe23 = new QCheckBox(BurstDelayClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(410, 230, 50, 20));
		checkBoxProbe23->setMinimumSize(QSize(50, 20));
		checkBoxProbe23->setMaximumSize(QSize(50, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe9 = new QLineEdit(BurstDelayClass);
		lineEditProbe9->setObjectName(QString::fromUtf8("lineEditProbe9"));
		lineEditProbe9->setGeometry(QRect(90, 470, 80, 20));
		lineEditProbe9->setMinimumSize(QSize(80, 20));
		lineEditProbe9->setMaximumSize(QSize(80, 20));
		lineEditProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_2 = new QLabel(BurstDelayClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(330, 20, 160, 20));
		label_2->setMinimumSize(QSize(160, 20));
		label_2->setMaximumSize(QSize(160, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		lineEditProbe21 = new QLineEdit(BurstDelayClass);
		lineEditProbe21->setObjectName(QString::fromUtf8("lineEditProbe21"));
		lineEditProbe21->setGeometry(QRect(470, 150, 80, 20));
		lineEditProbe21->setMinimumSize(QSize(80, 20));
		lineEditProbe21->setMaximumSize(QSize(80, 20));
		lineEditProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe24 = new QCheckBox(BurstDelayClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(410, 270, 50, 20));
		checkBoxProbe24->setMinimumSize(QSize(50, 20));
		checkBoxProbe24->setMaximumSize(QSize(50, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe23 = new QLineEdit(BurstDelayClass);
		lineEditProbe23->setObjectName(QString::fromUtf8("lineEditProbe23"));
		lineEditProbe23->setGeometry(QRect(470, 230, 80, 20));
		lineEditProbe23->setMinimumSize(QSize(80, 20));
		lineEditProbe23->setMaximumSize(QSize(80, 20));
		lineEditProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe1 = new QCheckBox(BurstDelayClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(40, 150, 50, 20));
		checkBoxProbe1->setMinimumSize(QSize(50, 20));
		checkBoxProbe1->setMaximumSize(QSize(50, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe11 = new QLineEdit(BurstDelayClass);
		lineEditProbe11->setObjectName(QString::fromUtf8("lineEditProbe11"));
		lineEditProbe11->setGeometry(QRect(280, 150, 80, 20));
		lineEditProbe11->setMinimumSize(QSize(80, 20));
		lineEditProbe11->setMaximumSize(QSize(80, 20));
		lineEditProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe9 = new QCheckBox(BurstDelayClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(40, 470, 50, 20));
		checkBoxProbe9->setMinimumSize(QSize(50, 20));
		checkBoxProbe9->setMaximumSize(QSize(50, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe25 = new QLineEdit(BurstDelayClass);
		lineEditProbe25->setObjectName(QString::fromUtf8("lineEditProbe25"));
		lineEditProbe25->setGeometry(QRect(470, 310, 80, 20));
		lineEditProbe25->setMinimumSize(QSize(80, 20));
		lineEditProbe25->setMaximumSize(QSize(80, 20));
		lineEditProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe26 = new QLineEdit(BurstDelayClass);
		lineEditProbe26->setObjectName(QString::fromUtf8("lineEditProbe26"));
		lineEditProbe26->setGeometry(QRect(470, 350, 80, 20));
		lineEditProbe26->setMinimumSize(QSize(80, 20));
		lineEditProbe26->setMaximumSize(QSize(80, 20));
		lineEditProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe21 = new QCheckBox(BurstDelayClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(410, 150, 50, 20));
		checkBoxProbe21->setMinimumSize(QSize(50, 20));
		checkBoxProbe21->setMaximumSize(QSize(50, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe20 = new QLineEdit(BurstDelayClass);
		lineEditProbe20->setObjectName(QString::fromUtf8("lineEditProbe20"));
		lineEditProbe20->setGeometry(QRect(280, 510, 80, 20));
		lineEditProbe20->setMinimumSize(QSize(80, 20));
		lineEditProbe20->setMaximumSize(QSize(80, 20));
		lineEditProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3 = new QLineEdit(BurstDelayClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setGeometry(QRect(90, 230, 80, 20));
		lineEditProbe3->setMinimumSize(QSize(80, 20));
		lineEditProbe3->setMaximumSize(QSize(80, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe14 = new QCheckBox(BurstDelayClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(220, 270, 50, 20));
		checkBoxProbe14->setMinimumSize(QSize(50, 20));
		checkBoxProbe14->setMaximumSize(QSize(50, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe6 = new QCheckBox(BurstDelayClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(40, 350, 50, 20));
		checkBoxProbe6->setMinimumSize(QSize(50, 20));
		checkBoxProbe6->setMaximumSize(QSize(50, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe22 = new QCheckBox(BurstDelayClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(410, 190, 50, 20));
		checkBoxProbe22->setMinimumSize(QSize(50, 20));
		checkBoxProbe22->setMaximumSize(QSize(50, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe2 = new QLineEdit(BurstDelayClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setGeometry(QRect(90, 190, 80, 20));
		lineEditProbe2->setMinimumSize(QSize(80, 20));
		lineEditProbe2->setMaximumSize(QSize(80, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe5 = new QCheckBox(BurstDelayClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(40, 310, 50, 20));
		checkBoxProbe5->setMinimumSize(QSize(50, 20));
		checkBoxProbe5->setMaximumSize(QSize(50, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe7 = new QCheckBox(BurstDelayClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(40, 390, 50, 20));
		checkBoxProbe7->setMinimumSize(QSize(50, 20));
		checkBoxProbe7->setMaximumSize(QSize(50, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe8 = new QLineEdit(BurstDelayClass);
		lineEditProbe8->setObjectName(QString::fromUtf8("lineEditProbe8"));
		lineEditProbe8->setGeometry(QRect(90, 430, 80, 20));
		lineEditProbe8->setMinimumSize(QSize(80, 20));
		lineEditProbe8->setMaximumSize(QSize(80, 20));
		lineEditProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe10 = new QCheckBox(BurstDelayClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(40, 510, 50, 20));
		checkBoxProbe10->setMinimumSize(QSize(50, 20));
		checkBoxProbe10->setMaximumSize(QSize(50, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe7 = new QLineEdit(BurstDelayClass);
		lineEditProbe7->setObjectName(QString::fromUtf8("lineEditProbe7"));
		lineEditProbe7->setGeometry(QRect(90, 390, 80, 20));
		lineEditProbe7->setMinimumSize(QSize(80, 20));
		lineEditProbe7->setMaximumSize(QSize(80, 20));
		lineEditProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe17 = new QCheckBox(BurstDelayClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(220, 390, 50, 20));
		checkBoxProbe17->setMinimumSize(QSize(50, 20));
		checkBoxProbe17->setMaximumSize(QSize(50, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(BurstDelayClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(700, 550, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(BurstDelayClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(40, 230, 50, 20));
		checkBoxProbe3->setMinimumSize(QSize(50, 20));
		checkBoxProbe3->setMaximumSize(QSize(50, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe4 = new QLineEdit(BurstDelayClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setGeometry(QRect(90, 270, 80, 20));
		lineEditProbe4->setMinimumSize(QSize(80, 20));
		lineEditProbe4->setMaximumSize(QSize(80, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe27 = new QLineEdit(BurstDelayClass);
		lineEditProbe27->setObjectName(QString::fromUtf8("lineEditProbe27"));
		lineEditProbe27->setGeometry(QRect(470, 390, 80, 20));
		lineEditProbe27->setMinimumSize(QSize(80, 20));
		lineEditProbe27->setMaximumSize(QSize(80, 20));
		lineEditProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe25 = new QCheckBox(BurstDelayClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(410, 310, 50, 20));
		checkBoxProbe25->setMinimumSize(QSize(50, 20));
		checkBoxProbe25->setMaximumSize(QSize(50, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditAny = new QLineEdit(BurstDelayClass);
		lineEditAny->setObjectName(QString::fromUtf8("lineEditAny"));
		lineEditAny->setGeometry(QRect(130, 100, 80, 20));
		lineEditAny->setMinimumSize(QSize(80, 20));
		lineEditAny->setMaximumSize(QSize(80, 20));
		lineEditAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe12 = new QCheckBox(BurstDelayClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(220, 190, 50, 20));
		checkBoxProbe12->setMinimumSize(QSize(50, 20));
		checkBoxProbe12->setMaximumSize(QSize(50, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe26 = new QCheckBox(BurstDelayClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(410, 350, 50, 20));
		checkBoxProbe26->setMinimumSize(QSize(50, 20));
		checkBoxProbe26->setMaximumSize(QSize(50, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe14 = new QLineEdit(BurstDelayClass);
		lineEditProbe14->setObjectName(QString::fromUtf8("lineEditProbe14"));
		lineEditProbe14->setGeometry(QRect(280, 270, 80, 20));
		lineEditProbe14->setMinimumSize(QSize(80, 20));
		lineEditProbe14->setMaximumSize(QSize(80, 20));
		lineEditProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe16 = new QLineEdit(BurstDelayClass);
		lineEditProbe16->setObjectName(QString::fromUtf8("lineEditProbe16"));
		lineEditProbe16->setGeometry(QRect(280, 350, 80, 20));
		lineEditProbe16->setMinimumSize(QSize(80, 20));
		lineEditProbe16->setMaximumSize(QSize(80, 20));
		lineEditProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe18 = new QLineEdit(BurstDelayClass);
		lineEditProbe18->setObjectName(QString::fromUtf8("lineEditProbe18"));
		lineEditProbe18->setGeometry(QRect(280, 430, 80, 20));
		lineEditProbe18->setMinimumSize(QSize(80, 20));
		lineEditProbe18->setMaximumSize(QSize(80, 20));
		lineEditProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe19 = new QCheckBox(BurstDelayClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(220, 470, 50, 20));
		checkBoxProbe19->setMinimumSize(QSize(50, 20));
		checkBoxProbe19->setMaximumSize(QSize(50, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe10 = new QLineEdit(BurstDelayClass);
		lineEditProbe10->setObjectName(QString::fromUtf8("lineEditProbe10"));
		lineEditProbe10->setGeometry(QRect(90, 510, 80, 20));
		lineEditProbe10->setMinimumSize(QSize(80, 20));
		lineEditProbe10->setMaximumSize(QSize(80, 20));
		lineEditProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe19 = new QLineEdit(BurstDelayClass);
		lineEditProbe19->setObjectName(QString::fromUtf8("lineEditProbe19"));
		lineEditProbe19->setGeometry(QRect(280, 470, 80, 20));
		lineEditProbe19->setMinimumSize(QSize(80, 20));
		lineEditProbe19->setMaximumSize(QSize(80, 20));
		lineEditProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe13 = new QLineEdit(BurstDelayClass);
		lineEditProbe13->setObjectName(QString::fromUtf8("lineEditProbe13"));
		lineEditProbe13->setGeometry(QRect(280, 230, 80, 20));
		lineEditProbe13->setMinimumSize(QSize(80, 20));
		lineEditProbe13->setMaximumSize(QSize(80, 20));
		lineEditProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe2 = new QCheckBox(BurstDelayClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(40, 190, 50, 20));
		checkBoxProbe2->setMinimumSize(QSize(50, 20));
		checkBoxProbe2->setMaximumSize(QSize(50, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe17 = new QLineEdit(BurstDelayClass);
		lineEditProbe17->setObjectName(QString::fromUtf8("lineEditProbe17"));
		lineEditProbe17->setGeometry(QRect(280, 390, 80, 20));
		lineEditProbe17->setMinimumSize(QSize(80, 20));
		lineEditProbe17->setMaximumSize(QSize(80, 20));
		lineEditProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe15 = new QLineEdit(BurstDelayClass);
		lineEditProbe15->setObjectName(QString::fromUtf8("lineEditProbe15"));
		lineEditProbe15->setGeometry(QRect(280, 310, 80, 20));
		lineEditProbe15->setMinimumSize(QSize(80, 20));
		lineEditProbe15->setMaximumSize(QSize(80, 20));
		lineEditProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5 = new QLineEdit(BurstDelayClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setGeometry(QRect(90, 310, 80, 20));
		lineEditProbe5->setMinimumSize(QSize(80, 20));
		lineEditProbe5->setMaximumSize(QSize(80, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe29 = new QCheckBox(BurstDelayClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(410, 470, 50, 20));
		checkBoxProbe29->setMinimumSize(QSize(50, 20));
		checkBoxProbe29->setMaximumSize(QSize(50, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe4 = new QCheckBox(BurstDelayClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(40, 270, 50, 20));
		checkBoxProbe4->setMinimumSize(QSize(50, 20));
		checkBoxProbe4->setMaximumSize(QSize(50, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe15 = new QCheckBox(BurstDelayClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(220, 310, 50, 20));
		checkBoxProbe15->setMinimumSize(QSize(50, 20));
		checkBoxProbe15->setMaximumSize(QSize(50, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAll = new QRadioButton(BurstDelayClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(40, 60, 89, 16));
		radioButtonAll->setMinimumSize(QSize(89, 16));
		radioButtonAll->setMaximumSize(QSize(89, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe30 = new QLineEdit(BurstDelayClass);
		lineEditProbe30->setObjectName(QString::fromUtf8("lineEditProbe30"));
		lineEditProbe30->setGeometry(QRect(470, 510, 80, 20));
		lineEditProbe30->setMinimumSize(QSize(80, 20));
		lineEditProbe30->setMaximumSize(QSize(80, 20));
		lineEditProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe27 = new QCheckBox(BurstDelayClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(410, 390, 50, 20));
		checkBoxProbe27->setMinimumSize(QSize(50, 20));
		checkBoxProbe27->setMaximumSize(QSize(50, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe12 = new QLineEdit(BurstDelayClass);
		lineEditProbe12->setObjectName(QString::fromUtf8("lineEditProbe12"));
		lineEditProbe12->setGeometry(QRect(280, 190, 80, 20));
		lineEditProbe12->setMinimumSize(QSize(80, 20));
		lineEditProbe12->setMaximumSize(QSize(80, 20));
		lineEditProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_3 = new QLabel(BurstDelayClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(210, 100, 30, 20));
		label_3->setMinimumSize(QSize(30, 20));
		label_3->setMaximumSize(QSize(30, 20));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(BurstDelayClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(170, 150, 30, 20));
		label_4->setMinimumSize(QSize(30, 20));
		label_4->setMaximumSize(QSize(30, 20));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_5 = new QLabel(BurstDelayClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(170, 190, 30, 20));
		label_5->setMinimumSize(QSize(30, 20));
		label_5->setMaximumSize(QSize(30, 20));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_6 = new QLabel(BurstDelayClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(170, 230, 30, 20));
		label_6->setMinimumSize(QSize(30, 20));
		label_6->setMaximumSize(QSize(30, 20));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(BurstDelayClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(170, 270, 30, 20));
		label_7->setMinimumSize(QSize(30, 20));
		label_7->setMaximumSize(QSize(30, 20));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(BurstDelayClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(170, 310, 30, 20));
		label_8->setMinimumSize(QSize(30, 20));
		label_8->setMaximumSize(QSize(30, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(BurstDelayClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(170, 350, 30, 20));
		label_9->setMinimumSize(QSize(30, 20));
		label_9->setMaximumSize(QSize(30, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(BurstDelayClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(170, 390, 30, 20));
		label_10->setMinimumSize(QSize(30, 20));
		label_10->setMaximumSize(QSize(30, 20));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_11 = new QLabel(BurstDelayClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(170, 430, 30, 20));
		label_11->setMinimumSize(QSize(30, 20));
		label_11->setMaximumSize(QSize(30, 20));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_12 = new QLabel(BurstDelayClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(170, 470, 30, 20));
		label_12->setMinimumSize(QSize(30, 20));
		label_12->setMaximumSize(QSize(30, 20));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_13 = new QLabel(BurstDelayClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(170, 510, 30, 20));
		label_13->setMinimumSize(QSize(30, 20));
		label_13->setMaximumSize(QSize(30, 20));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_14 = new QLabel(BurstDelayClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(360, 510, 30, 20));
		label_14->setMinimumSize(QSize(30, 20));
		label_14->setMaximumSize(QSize(30, 20));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_15 = new QLabel(BurstDelayClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(360, 310, 30, 20));
		label_15->setMinimumSize(QSize(30, 20));
		label_15->setMaximumSize(QSize(30, 20));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_16 = new QLabel(BurstDelayClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(360, 430, 30, 20));
		label_16->setMinimumSize(QSize(30, 20));
		label_16->setMaximumSize(QSize(30, 20));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_17 = new QLabel(BurstDelayClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(360, 190, 30, 20));
		label_17->setMinimumSize(QSize(30, 20));
		label_17->setMaximumSize(QSize(30, 20));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_18 = new QLabel(BurstDelayClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(360, 350, 30, 20));
		label_18->setMinimumSize(QSize(30, 20));
		label_18->setMaximumSize(QSize(30, 20));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_19 = new QLabel(BurstDelayClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(360, 470, 30, 20));
		label_19->setMinimumSize(QSize(30, 20));
		label_19->setMaximumSize(QSize(30, 20));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(BurstDelayClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(360, 150, 30, 20));
		label_20->setMinimumSize(QSize(30, 20));
		label_20->setMaximumSize(QSize(30, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_21 = new QLabel(BurstDelayClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(360, 230, 30, 20));
		label_21->setMinimumSize(QSize(30, 20));
		label_21->setMaximumSize(QSize(30, 20));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_22 = new QLabel(BurstDelayClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(360, 270, 30, 20));
		label_22->setMinimumSize(QSize(30, 20));
		label_22->setMaximumSize(QSize(30, 20));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(BurstDelayClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(360, 390, 30, 20));
		label_23->setMinimumSize(QSize(30, 20));
		label_23->setMaximumSize(QSize(30, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_24 = new QLabel(BurstDelayClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(550, 390, 30, 20));
		label_24->setMinimumSize(QSize(30, 20));
		label_24->setMaximumSize(QSize(30, 20));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_25 = new QLabel(BurstDelayClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(550, 310, 30, 20));
		label_25->setMinimumSize(QSize(30, 20));
		label_25->setMaximumSize(QSize(30, 20));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_26 = new QLabel(BurstDelayClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(550, 230, 30, 20));
		label_26->setMinimumSize(QSize(30, 20));
		label_26->setMaximumSize(QSize(30, 20));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_27 = new QLabel(BurstDelayClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(550, 270, 30, 20));
		label_27->setMinimumSize(QSize(30, 20));
		label_27->setMaximumSize(QSize(30, 20));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_28 = new QLabel(BurstDelayClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(550, 150, 30, 20));
		label_28->setMinimumSize(QSize(30, 20));
		label_28->setMaximumSize(QSize(30, 20));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(BurstDelayClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(550, 430, 30, 20));
		label_29->setMinimumSize(QSize(30, 20));
		label_29->setMaximumSize(QSize(30, 20));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_30 = new QLabel(BurstDelayClass);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(550, 470, 30, 20));
		label_30->setMinimumSize(QSize(30, 20));
		label_30->setMaximumSize(QSize(30, 20));
		label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_31 = new QLabel(BurstDelayClass);
		label_31->setObjectName(QString::fromUtf8("label_31"));
		label_31->setGeometry(QRect(550, 350, 30, 20));
		label_31->setMinimumSize(QSize(30, 20));
		label_31->setMaximumSize(QSize(30, 20));
		label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_32 = new QLabel(BurstDelayClass);
		label_32->setObjectName(QString::fromUtf8("label_32"));
		label_32->setGeometry(QRect(550, 510, 30, 20));
		label_32->setMinimumSize(QSize(30, 20));
		label_32->setMaximumSize(QSize(30, 20));
		label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_33 = new QLabel(BurstDelayClass);
		label_33->setObjectName(QString::fromUtf8("label_33"));
		label_33->setGeometry(QRect(550, 190, 30, 20));
		label_33->setMinimumSize(QSize(30, 20));
		label_33->setMaximumSize(QSize(30, 20));
		label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllIn = new QPushButton(BurstDelayClass);
		pushButtonAllIn->setObjectName(QString::fromUtf8("pushButtonAllIn"));
		pushButtonAllIn->setGeometry(QRect(260, 60, 50, 23));
		pushButtonAllIn->setMinimumSize(QSize(50, 23));
		pushButtonAllIn->setMaximumSize(QSize(50, 23));
		pushButtonAllIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyIn = new QPushButton(BurstDelayClass);
		pushButtonAnyIn->setObjectName(QString::fromUtf8("pushButtonAnyIn"));
		pushButtonAnyIn->setGeometry(QRect(260, 100, 50, 23));
		pushButtonAnyIn->setMinimumSize(QSize(50, 23));
		pushButtonAnyIn->setMaximumSize(QSize(50, 23));
		pushButtonAnyIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_34 = new QLabel(BurstDelayClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(740, 230, 30, 20));
		label_34->setMinimumSize(QSize(30, 20));
		label_34->setMaximumSize(QSize(30, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe31 = new QLineEdit(BurstDelayClass);
		lineEditProbe31->setObjectName(QString::fromUtf8("lineEditProbe31"));
		lineEditProbe31->setGeometry(QRect(660, 150, 80, 20));
		lineEditProbe31->setMinimumSize(QSize(80, 20));
		lineEditProbe31->setMaximumSize(QSize(80, 20));
		lineEditProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe31 = new QCheckBox(BurstDelayClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(600, 150, 50, 20));
		checkBoxProbe31->setMinimumSize(QSize(50, 20));
		checkBoxProbe31->setMaximumSize(QSize(50, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe32 = new QLineEdit(BurstDelayClass);
		lineEditProbe32->setObjectName(QString::fromUtf8("lineEditProbe32"));
		lineEditProbe32->setGeometry(QRect(660, 190, 80, 20));
		lineEditProbe32->setMinimumSize(QSize(80, 20));
		lineEditProbe32->setMaximumSize(QSize(80, 20));
		lineEditProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe35 = new QCheckBox(BurstDelayClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(600, 310, 50, 20));
		checkBoxProbe35->setMinimumSize(QSize(50, 20));
		checkBoxProbe35->setMaximumSize(QSize(50, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe33 = new QLineEdit(BurstDelayClass);
		lineEditProbe33->setObjectName(QString::fromUtf8("lineEditProbe33"));
		lineEditProbe33->setGeometry(QRect(660, 230, 80, 20));
		lineEditProbe33->setMinimumSize(QSize(80, 20));
		lineEditProbe33->setMaximumSize(QSize(80, 20));
		lineEditProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe34 = new QLineEdit(BurstDelayClass);
		lineEditProbe34->setObjectName(QString::fromUtf8("lineEditProbe34"));
		lineEditProbe34->setGeometry(QRect(660, 270, 80, 20));
		lineEditProbe34->setMinimumSize(QSize(80, 20));
		lineEditProbe34->setMaximumSize(QSize(80, 20));
		lineEditProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_35 = new QLabel(BurstDelayClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(740, 350, 30, 20));
		label_35->setMinimumSize(QSize(30, 20));
		label_35->setMaximumSize(QSize(30, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe32 = new QCheckBox(BurstDelayClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(600, 190, 50, 20));
		checkBoxProbe32->setMinimumSize(QSize(50, 20));
		checkBoxProbe32->setMaximumSize(QSize(50, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(BurstDelayClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(600, 270, 50, 20));
		checkBoxProbe34->setMinimumSize(QSize(50, 20));
		checkBoxProbe34->setMaximumSize(QSize(50, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(BurstDelayClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(740, 270, 30, 20));
		label_36->setMinimumSize(QSize(30, 20));
		label_36->setMaximumSize(QSize(30, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe36 = new QLineEdit(BurstDelayClass);
		lineEditProbe36->setObjectName(QString::fromUtf8("lineEditProbe36"));
		lineEditProbe36->setGeometry(QRect(660, 350, 80, 20));
		lineEditProbe36->setMinimumSize(QSize(80, 20));
		lineEditProbe36->setMaximumSize(QSize(80, 20));
		lineEditProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe36 = new QCheckBox(BurstDelayClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(600, 350, 50, 20));
		checkBoxProbe36->setMinimumSize(QSize(50, 20));
		checkBoxProbe36->setMaximumSize(QSize(50, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_37 = new QLabel(BurstDelayClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(740, 190, 30, 20));
		label_37->setMinimumSize(QSize(30, 20));
		label_37->setMaximumSize(QSize(30, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(BurstDelayClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(600, 230, 50, 20));
		checkBoxProbe33->setMinimumSize(QSize(50, 20));
		checkBoxProbe33->setMaximumSize(QSize(50, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_38 = new QLabel(BurstDelayClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(740, 150, 30, 20));
		label_38->setMinimumSize(QSize(30, 20));
		label_38->setMaximumSize(QSize(30, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe35 = new QLineEdit(BurstDelayClass);
		lineEditProbe35->setObjectName(QString::fromUtf8("lineEditProbe35"));
		lineEditProbe35->setGeometry(QRect(660, 310, 80, 20));
		lineEditProbe35->setMinimumSize(QSize(80, 20));
		lineEditProbe35->setMaximumSize(QSize(80, 20));
		lineEditProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_39 = new QLabel(BurstDelayClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(740, 310, 30, 20));
		label_39->setMinimumSize(QSize(30, 20));
		label_39->setMaximumSize(QSize(30, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));

		retranslateUi(BurstDelayClass);

		QMetaObject::connectSlotsByName(BurstDelayClass);
	} // setupUi

	void retranslateUi(QDialog *BurstDelayClass)
	{
		BurstDelayClass->setWindowTitle(QApplication::translate("BurstDelayClass", "BurstDelay", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("BurstDelayClass", " 30", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("BurstDelayClass", " 20", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("BurstDelayClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("BurstDelayClass", " 18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("BurstDelayClass", " 13", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("BurstDelayClass", " 11", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("BurstDelayClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("BurstDelayClass", " 28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("BurstDelayClass", " 8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("BurstDelayClass", " 16", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("BurstDelayClass", " 23", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("BurstDelayClass", "\345\273\266\350\277\237\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("BurstDelayClass", " 24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("BurstDelayClass", " 1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("BurstDelayClass", " 9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("BurstDelayClass", " 21", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("BurstDelayClass", " 14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("BurstDelayClass", " 6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("BurstDelayClass", " 22", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("BurstDelayClass", " 5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("BurstDelayClass", " 7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("BurstDelayClass", " 10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("BurstDelayClass", " 17", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("BurstDelayClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("BurstDelayClass", " 3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("BurstDelayClass", " 25", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("BurstDelayClass", " 12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("BurstDelayClass", " 26", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("BurstDelayClass", " 19", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("BurstDelayClass", " 2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("BurstDelayClass", " 29", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("BurstDelayClass", " 4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("BurstDelayClass", " 15", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("BurstDelayClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("BurstDelayClass", " 27", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_31->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_32->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_33->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		pushButtonAllIn->setText(QApplication::translate("BurstDelayClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyIn->setText(QApplication::translate("BurstDelayClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("BurstDelayClass", "31", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("BurstDelayClass", "35", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("BurstDelayClass", "32", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("BurstDelayClass", "34", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("BurstDelayClass", "36", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("BurstDelayClass", "33", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("BurstDelayClass", "(us)", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class BurstDelayClass: public Ui_BurstDelayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BURSTDELAY_H






/********************************************************************************
** Form generated from reading UI file 'cgain.ui'
**
** Created: Wed Jun 18 22:27:36 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CGAIN_H
#define UI_CGAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_CGainClass
{
public:
	QCheckBox *checkBoxProbe26;
	QLineEdit *lineEditProbe17;
	QLabel *label_19;
	QCheckBox *checkBoxProbe28;
	QLabel *label_16;
	QLineEdit *lineEditProbe30;
	QCheckBox *checkBoxProbe16;
	QLineEdit *lineEditProbe12;
	QLabel *label_18;
	QCheckBox *checkBoxProbe18;
	QCheckBox *checkBoxProbe11;
	QCheckBox *checkBoxProbe17;
	QCheckBox *checkBoxProbe25;
	QLineEdit *lineEditProbe27;
	QLineEdit *lineEditProbe13;
	QLineEdit *lineEditProbe25;
	QLabel *label_15;
	QLabel *label_17;
	QCheckBox *checkBoxProbe6;
	QPushButton *pushButtonNo;
	QLineEdit *lineEditProbe29;
	QPushButton *pushButtonYes;
	QLineEdit *lineEditProbe28;
	QLineEdit *lineEditProbe21;
	QCheckBox *checkBoxProbe1;
	QLineEdit *lineEditProbe16;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe30;
	QLabel *label_2;
	QCheckBox *checkBoxProbe23;
	QCheckBox *checkBoxProbe24;
	QLabel *label_32;
	QLabel *label_14;
	QLineEdit *lineEditProbe23;
	QLineEdit *lineEditProbe3;
	QCheckBox *checkBoxProbe8;
	QCheckBox *checkBoxProbe10;
	QLabel *label_4;
	QLabel *label_13;
	QLineEdit *lineEditAll;
	QLineEdit *lineEditProbe20;
	QLabel *label_31;
	QLabel *label_3;
	QLineEdit *lineEditProbe22;
	QLineEdit *lineEditProbe26;
	QLabel *label_30;
	QCheckBox *checkBoxProbe5;
	QLabel *label_33;
	QLabel *label_12;
	QLineEdit *lineEditProbe7;
	QLineEdit *lineEditProbe4;
	QCheckBox *checkBoxProbe9;
	QLabel *label_29;
	QLineEdit *lineEditProbe5;
	QCheckBox *checkBoxProbe27;
	QRadioButton *radioButtonAny;
	QLineEdit *lineEditProbe19;
	QLabel *label_11;
	QCheckBox *checkBoxProbe15;
	QLineEdit *lineEditProbe15;
	QCheckBox *checkBoxProbe7;
	QCheckBox *checkBoxProbe4;
	QLineEdit *lineEditProbe24;
	QCheckBox *checkBoxProbe29;
	QLineEdit *lineEditProbe1;
	QLabel *label_28;
	QCheckBox *checkBoxProbe3;
	QLabel *label_8;
	QLineEdit *lineEditProbe9;
	QLabel *label_24;
	QLabel *label_10;
	QLineEdit *lineEditProbe8;
	QLineEdit *lineEditProbe10;
	QLabel *label_27;
	QCheckBox *checkBoxProbe12;
	QCheckBox *checkBoxProbe19;
	QLabel *label_7;
	QLabel *label_9;
	QLabel *label_23;
	QLabel *label_25;
	QLabel *label_5;
	QLabel *label_6;
	QLineEdit *lineEditProbe6;
	QLineEdit *lineEditProbe11;
	QLabel *label_22;
	QLabel *label_26;
	QLineEdit *lineEditProbe2;
	QLabel *label_21;
	QLineEdit *lineEditProbe14;
	QCheckBox *checkBoxProbe2;
	QLineEdit *lineEditProbe18;
	QCheckBox *checkBoxProbe22;
	QRadioButton *radioButtonAll;
	QLineEdit *lineEditAny;
	QCheckBox *checkBoxProbe21;
	QLabel *label_20;
	QCheckBox *checkBoxProbe13;
	QCheckBox *checkBoxProbe20;
	QLabel *label;
	QPushButton *pushButtonAllIn;
	QPushButton *pushButtonAnyIn;
	QLineEdit *lineEditProbe32;
	QLabel *label_34;
	QCheckBox *checkBoxProbe31;
	QCheckBox *checkBoxProbe34;
	QLabel *label_35;
	QLineEdit *lineEditProbe36;
	QLineEdit *lineEditProbe34;
	QCheckBox *checkBoxProbe32;
	QLabel *label_36;
	QCheckBox *checkBoxProbe33;
	QLabel *label_37;
	QLabel *label_38;
	QCheckBox *checkBoxProbe36;
	QLabel *label_39;
	QCheckBox *checkBoxProbe35;
	QLineEdit *lineEditProbe31;
	QLineEdit *lineEditProbe33;
	QLineEdit *lineEditProbe35;

	void setupUi(QDialog *CGainClass)
	{
		if (CGainClass->objectName().isEmpty())
			CGainClass->setObjectName(QString::fromUtf8("CGainClass"));
		CGainClass->resize(800, 586);
		CGainClass->setMinimumSize(QSize(800, 586));
		CGainClass->setMaximumSize(QSize(800, 586));
		CGainClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		checkBoxProbe26 = new QCheckBox(CGainClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(410, 350, 50, 20));
		checkBoxProbe26->setMinimumSize(QSize(50, 20));
		checkBoxProbe26->setMaximumSize(QSize(50, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe17 = new QLineEdit(CGainClass);
		lineEditProbe17->setObjectName(QString::fromUtf8("lineEditProbe17"));
		lineEditProbe17->setGeometry(QRect(280, 390, 80, 20));
		lineEditProbe17->setMinimumSize(QSize(80, 20));
		lineEditProbe17->setMaximumSize(QSize(80, 20));
		lineEditProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_19 = new QLabel(CGainClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(360, 430, 30, 20));
		label_19->setMinimumSize(QSize(30, 20));
		label_19->setMaximumSize(QSize(30, 20));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe28 = new QCheckBox(CGainClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(410, 430, 50, 20));
		checkBoxProbe28->setMinimumSize(QSize(50, 20));
		checkBoxProbe28->setMaximumSize(QSize(50, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_16 = new QLabel(CGainClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(360, 310, 30, 20));
		label_16->setMinimumSize(QSize(30, 20));
		label_16->setMaximumSize(QSize(30, 20));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe30 = new QLineEdit(CGainClass);
		lineEditProbe30->setObjectName(QString::fromUtf8("lineEditProbe30"));
		lineEditProbe30->setGeometry(QRect(470, 510, 80, 20));
		lineEditProbe30->setMinimumSize(QSize(80, 20));
		lineEditProbe30->setMaximumSize(QSize(80, 20));
		lineEditProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe16 = new QCheckBox(CGainClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(220, 350, 50, 20));
		checkBoxProbe16->setMinimumSize(QSize(50, 20));
		checkBoxProbe16->setMaximumSize(QSize(50, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe12 = new QLineEdit(CGainClass);
		lineEditProbe12->setObjectName(QString::fromUtf8("lineEditProbe12"));
		lineEditProbe12->setGeometry(QRect(280, 190, 80, 20));
		lineEditProbe12->setMinimumSize(QSize(80, 20));
		lineEditProbe12->setMaximumSize(QSize(80, 20));
		lineEditProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_18 = new QLabel(CGainClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(360, 390, 30, 20));
		label_18->setMinimumSize(QSize(30, 20));
		label_18->setMaximumSize(QSize(30, 20));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe18 = new QCheckBox(CGainClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(220, 430, 50, 20));
		checkBoxProbe18->setMinimumSize(QSize(50, 20));
		checkBoxProbe18->setMaximumSize(QSize(50, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(CGainClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(220, 150, 50, 20));
		checkBoxProbe11->setMinimumSize(QSize(50, 20));
		checkBoxProbe11->setMaximumSize(QSize(50, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe17 = new QCheckBox(CGainClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(220, 390, 50, 20));
		checkBoxProbe17->setMinimumSize(QSize(50, 20));
		checkBoxProbe17->setMaximumSize(QSize(50, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25 = new QCheckBox(CGainClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(410, 310, 50, 20));
		checkBoxProbe25->setMinimumSize(QSize(50, 20));
		checkBoxProbe25->setMaximumSize(QSize(50, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe27 = new QLineEdit(CGainClass);
		lineEditProbe27->setObjectName(QString::fromUtf8("lineEditProbe27"));
		lineEditProbe27->setGeometry(QRect(470, 390, 80, 20));
		lineEditProbe27->setMinimumSize(QSize(80, 20));
		lineEditProbe27->setMaximumSize(QSize(80, 20));
		lineEditProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe13 = new QLineEdit(CGainClass);
		lineEditProbe13->setObjectName(QString::fromUtf8("lineEditProbe13"));
		lineEditProbe13->setGeometry(QRect(280, 230, 80, 20));
		lineEditProbe13->setMinimumSize(QSize(80, 20));
		lineEditProbe13->setMaximumSize(QSize(80, 20));
		lineEditProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe25 = new QLineEdit(CGainClass);
		lineEditProbe25->setObjectName(QString::fromUtf8("lineEditProbe25"));
		lineEditProbe25->setGeometry(QRect(470, 310, 80, 20));
		lineEditProbe25->setMinimumSize(QSize(80, 20));
		lineEditProbe25->setMaximumSize(QSize(80, 20));
		lineEditProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_15 = new QLabel(CGainClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(360, 470, 30, 20));
		label_15->setMinimumSize(QSize(30, 20));
		label_15->setMaximumSize(QSize(30, 20));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_17 = new QLabel(CGainClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(360, 350, 30, 20));
		label_17->setMinimumSize(QSize(30, 20));
		label_17->setMaximumSize(QSize(30, 20));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe6 = new QCheckBox(CGainClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(40, 350, 50, 20));
		checkBoxProbe6->setMinimumSize(QSize(50, 20));
		checkBoxProbe6->setMaximumSize(QSize(50, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(CGainClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(690, 540, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe29 = new QLineEdit(CGainClass);
		lineEditProbe29->setObjectName(QString::fromUtf8("lineEditProbe29"));
		lineEditProbe29->setGeometry(QRect(470, 470, 80, 20));
		lineEditProbe29->setMinimumSize(QSize(80, 20));
		lineEditProbe29->setMaximumSize(QSize(80, 20));
		lineEditProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonYes = new QPushButton(CGainClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(590, 540, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe28 = new QLineEdit(CGainClass);
		lineEditProbe28->setObjectName(QString::fromUtf8("lineEditProbe28"));
		lineEditProbe28->setGeometry(QRect(470, 430, 80, 20));
		lineEditProbe28->setMinimumSize(QSize(80, 20));
		lineEditProbe28->setMaximumSize(QSize(80, 20));
		lineEditProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe21 = new QLineEdit(CGainClass);
		lineEditProbe21->setObjectName(QString::fromUtf8("lineEditProbe21"));
		lineEditProbe21->setGeometry(QRect(470, 150, 80, 20));
		lineEditProbe21->setMinimumSize(QSize(80, 20));
		lineEditProbe21->setMaximumSize(QSize(80, 20));
		lineEditProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe1 = new QCheckBox(CGainClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(40, 150, 50, 20));
		checkBoxProbe1->setMinimumSize(QSize(50, 20));
		checkBoxProbe1->setMaximumSize(QSize(50, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe16 = new QLineEdit(CGainClass);
		lineEditProbe16->setObjectName(QString::fromUtf8("lineEditProbe16"));
		lineEditProbe16->setGeometry(QRect(280, 350, 80, 20));
		lineEditProbe16->setMinimumSize(QSize(80, 20));
		lineEditProbe16->setMaximumSize(QSize(80, 20));
		lineEditProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe14 = new QCheckBox(CGainClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(220, 270, 50, 20));
		checkBoxProbe14->setMinimumSize(QSize(50, 20));
		checkBoxProbe14->setMaximumSize(QSize(50, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe30 = new QCheckBox(CGainClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(410, 510, 50, 20));
		checkBoxProbe30->setMinimumSize(QSize(50, 20));
		checkBoxProbe30->setMaximumSize(QSize(50, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_2 = new QLabel(CGainClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(310, 20, 160, 20));
		label_2->setMinimumSize(QSize(160, 20));
		label_2->setMaximumSize(QSize(160, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe23 = new QCheckBox(CGainClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(410, 230, 50, 20));
		checkBoxProbe23->setMinimumSize(QSize(50, 20));
		checkBoxProbe23->setMaximumSize(QSize(50, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe24 = new QCheckBox(CGainClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(410, 270, 50, 20));
		checkBoxProbe24->setMinimumSize(QSize(50, 20));
		checkBoxProbe24->setMaximumSize(QSize(50, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_32 = new QLabel(CGainClass);
		label_32->setObjectName(QString::fromUtf8("label_32"));
		label_32->setGeometry(QRect(550, 510, 30, 20));
		label_32->setMinimumSize(QSize(30, 20));
		label_32->setMaximumSize(QSize(30, 20));
		label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_14 = new QLabel(CGainClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(360, 510, 30, 20));
		label_14->setMinimumSize(QSize(30, 20));
		label_14->setMaximumSize(QSize(30, 20));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe23 = new QLineEdit(CGainClass);
		lineEditProbe23->setObjectName(QString::fromUtf8("lineEditProbe23"));
		lineEditProbe23->setGeometry(QRect(470, 230, 80, 20));
		lineEditProbe23->setMinimumSize(QSize(80, 20));
		lineEditProbe23->setMaximumSize(QSize(80, 20));
		lineEditProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3 = new QLineEdit(CGainClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setGeometry(QRect(90, 230, 80, 20));
		lineEditProbe3->setMinimumSize(QSize(80, 20));
		lineEditProbe3->setMaximumSize(QSize(80, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe8 = new QCheckBox(CGainClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(40, 430, 50, 20));
		checkBoxProbe8->setMinimumSize(QSize(50, 20));
		checkBoxProbe8->setMaximumSize(QSize(50, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe10 = new QCheckBox(CGainClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(40, 510, 50, 20));
		checkBoxProbe10->setMinimumSize(QSize(50, 20));
		checkBoxProbe10->setMaximumSize(QSize(50, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(CGainClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(170, 150, 30, 20));
		label_4->setMinimumSize(QSize(30, 20));
		label_4->setMaximumSize(QSize(30, 20));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_13 = new QLabel(CGainClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(170, 510, 30, 20));
		label_13->setMinimumSize(QSize(30, 20));
		label_13->setMaximumSize(QSize(30, 20));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditAll = new QLineEdit(CGainClass);
		lineEditAll->setObjectName(QString::fromUtf8("lineEditAll"));
		lineEditAll->setGeometry(QRect(130, 60, 80, 20));
		lineEditAll->setMinimumSize(QSize(80, 20));
		lineEditAll->setMaximumSize(QSize(80, 20));
		lineEditAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe20 = new QLineEdit(CGainClass);
		lineEditProbe20->setObjectName(QString::fromUtf8("lineEditProbe20"));
		lineEditProbe20->setGeometry(QRect(280, 510, 80, 20));
		lineEditProbe20->setMinimumSize(QSize(80, 20));
		lineEditProbe20->setMaximumSize(QSize(80, 20));
		lineEditProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_31 = new QLabel(CGainClass);
		label_31->setObjectName(QString::fromUtf8("label_31"));
		label_31->setGeometry(QRect(550, 430, 30, 20));
		label_31->setMinimumSize(QSize(30, 20));
		label_31->setMaximumSize(QSize(30, 20));
		label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_3 = new QLabel(CGainClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(210, 100, 30, 20));
		label_3->setMinimumSize(QSize(30, 20));
		label_3->setMaximumSize(QSize(30, 20));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe22 = new QLineEdit(CGainClass);
		lineEditProbe22->setObjectName(QString::fromUtf8("lineEditProbe22"));
		lineEditProbe22->setGeometry(QRect(470, 190, 80, 20));
		lineEditProbe22->setMinimumSize(QSize(80, 20));
		lineEditProbe22->setMaximumSize(QSize(80, 20));
		lineEditProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe26 = new QLineEdit(CGainClass);
		lineEditProbe26->setObjectName(QString::fromUtf8("lineEditProbe26"));
		lineEditProbe26->setGeometry(QRect(470, 350, 80, 20));
		lineEditProbe26->setMinimumSize(QSize(80, 20));
		lineEditProbe26->setMaximumSize(QSize(80, 20));
		lineEditProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_30 = new QLabel(CGainClass);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(550, 390, 30, 20));
		label_30->setMinimumSize(QSize(30, 20));
		label_30->setMaximumSize(QSize(30, 20));
		label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe5 = new QCheckBox(CGainClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(40, 310, 50, 20));
		checkBoxProbe5->setMinimumSize(QSize(50, 20));
		checkBoxProbe5->setMaximumSize(QSize(50, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_33 = new QLabel(CGainClass);
		label_33->setObjectName(QString::fromUtf8("label_33"));
		label_33->setGeometry(QRect(550, 270, 30, 20));
		label_33->setMinimumSize(QSize(30, 20));
		label_33->setMaximumSize(QSize(30, 20));
		label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_12 = new QLabel(CGainClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(170, 470, 30, 20));
		label_12->setMinimumSize(QSize(30, 20));
		label_12->setMaximumSize(QSize(30, 20));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe7 = new QLineEdit(CGainClass);
		lineEditProbe7->setObjectName(QString::fromUtf8("lineEditProbe7"));
		lineEditProbe7->setGeometry(QRect(90, 390, 80, 20));
		lineEditProbe7->setMinimumSize(QSize(80, 20));
		lineEditProbe7->setMaximumSize(QSize(80, 20));
		lineEditProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4 = new QLineEdit(CGainClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setGeometry(QRect(90, 270, 80, 20));
		lineEditProbe4->setMinimumSize(QSize(80, 20));
		lineEditProbe4->setMaximumSize(QSize(80, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe9 = new QCheckBox(CGainClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(40, 470, 50, 20));
		checkBoxProbe9->setMinimumSize(QSize(50, 20));
		checkBoxProbe9->setMaximumSize(QSize(50, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(CGainClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(550, 310, 30, 20));
		label_29->setMinimumSize(QSize(30, 20));
		label_29->setMaximumSize(QSize(30, 20));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe5 = new QLineEdit(CGainClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setGeometry(QRect(90, 310, 80, 20));
		lineEditProbe5->setMinimumSize(QSize(80, 20));
		lineEditProbe5->setMaximumSize(QSize(80, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe27 = new QCheckBox(CGainClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(410, 390, 50, 20));
		checkBoxProbe27->setMinimumSize(QSize(50, 20));
		checkBoxProbe27->setMaximumSize(QSize(50, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAny = new QRadioButton(CGainClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(40, 100, 89, 16));
		radioButtonAny->setMinimumSize(QSize(89, 16));
		radioButtonAny->setMaximumSize(QSize(89, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe19 = new QLineEdit(CGainClass);
		lineEditProbe19->setObjectName(QString::fromUtf8("lineEditProbe19"));
		lineEditProbe19->setGeometry(QRect(280, 470, 80, 20));
		lineEditProbe19->setMinimumSize(QSize(80, 20));
		lineEditProbe19->setMaximumSize(QSize(80, 20));
		lineEditProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_11 = new QLabel(CGainClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(170, 430, 30, 20));
		label_11->setMinimumSize(QSize(30, 20));
		label_11->setMaximumSize(QSize(30, 20));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe15 = new QCheckBox(CGainClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(220, 310, 50, 20));
		checkBoxProbe15->setMinimumSize(QSize(50, 20));
		checkBoxProbe15->setMaximumSize(QSize(50, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe15 = new QLineEdit(CGainClass);
		lineEditProbe15->setObjectName(QString::fromUtf8("lineEditProbe15"));
		lineEditProbe15->setGeometry(QRect(280, 310, 80, 20));
		lineEditProbe15->setMinimumSize(QSize(80, 20));
		lineEditProbe15->setMaximumSize(QSize(80, 20));
		lineEditProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe7 = new QCheckBox(CGainClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(40, 390, 50, 20));
		checkBoxProbe7->setMinimumSize(QSize(50, 20));
		checkBoxProbe7->setMaximumSize(QSize(50, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe4 = new QCheckBox(CGainClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(40, 270, 50, 20));
		checkBoxProbe4->setMinimumSize(QSize(50, 20));
		checkBoxProbe4->setMaximumSize(QSize(50, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe24 = new QLineEdit(CGainClass);
		lineEditProbe24->setObjectName(QString::fromUtf8("lineEditProbe24"));
		lineEditProbe24->setGeometry(QRect(470, 270, 80, 20));
		lineEditProbe24->setMinimumSize(QSize(80, 20));
		lineEditProbe24->setMaximumSize(QSize(80, 20));
		lineEditProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe29 = new QCheckBox(CGainClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(410, 470, 50, 20));
		checkBoxProbe29->setMinimumSize(QSize(50, 20));
		checkBoxProbe29->setMaximumSize(QSize(50, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe1 = new QLineEdit(CGainClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setGeometry(QRect(90, 150, 80, 20));
		lineEditProbe1->setMinimumSize(QSize(80, 20));
		lineEditProbe1->setMaximumSize(QSize(80, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_28 = new QLabel(CGainClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(550, 150, 30, 20));
		label_28->setMinimumSize(QSize(30, 20));
		label_28->setMaximumSize(QSize(30, 20));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(CGainClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(40, 230, 50, 20));
		checkBoxProbe3->setMinimumSize(QSize(50, 20));
		checkBoxProbe3->setMaximumSize(QSize(50, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(CGainClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(170, 310, 30, 20));
		label_8->setMinimumSize(QSize(30, 20));
		label_8->setMaximumSize(QSize(30, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe9 = new QLineEdit(CGainClass);
		lineEditProbe9->setObjectName(QString::fromUtf8("lineEditProbe9"));
		lineEditProbe9->setGeometry(QRect(90, 470, 80, 20));
		lineEditProbe9->setMinimumSize(QSize(80, 20));
		lineEditProbe9->setMaximumSize(QSize(80, 20));
		lineEditProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_24 = new QLabel(CGainClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(550, 230, 30, 20));
		label_24->setMinimumSize(QSize(30, 20));
		label_24->setMaximumSize(QSize(30, 20));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(CGainClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(170, 390, 30, 20));
		label_10->setMinimumSize(QSize(30, 20));
		label_10->setMaximumSize(QSize(30, 20));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe8 = new QLineEdit(CGainClass);
		lineEditProbe8->setObjectName(QString::fromUtf8("lineEditProbe8"));
		lineEditProbe8->setGeometry(QRect(90, 430, 80, 20));
		lineEditProbe8->setMinimumSize(QSize(80, 20));
		lineEditProbe8->setMaximumSize(QSize(80, 20));
		lineEditProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10 = new QLineEdit(CGainClass);
		lineEditProbe10->setObjectName(QString::fromUtf8("lineEditProbe10"));
		lineEditProbe10->setGeometry(QRect(90, 510, 80, 20));
		lineEditProbe10->setMinimumSize(QSize(80, 20));
		lineEditProbe10->setMaximumSize(QSize(80, 20));
		lineEditProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_27 = new QLabel(CGainClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(550, 350, 30, 20));
		label_27->setMinimumSize(QSize(30, 20));
		label_27->setMaximumSize(QSize(30, 20));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe12 = new QCheckBox(CGainClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(220, 190, 50, 20));
		checkBoxProbe12->setMinimumSize(QSize(50, 20));
		checkBoxProbe12->setMaximumSize(QSize(50, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe19 = new QCheckBox(CGainClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(220, 470, 50, 20));
		checkBoxProbe19->setMinimumSize(QSize(50, 20));
		checkBoxProbe19->setMaximumSize(QSize(50, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(CGainClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(170, 270, 30, 20));
		label_7->setMinimumSize(QSize(30, 20));
		label_7->setMaximumSize(QSize(30, 20));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(CGainClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(170, 350, 30, 20));
		label_9->setMinimumSize(QSize(30, 20));
		label_9->setMaximumSize(QSize(30, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(CGainClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(360, 230, 30, 20));
		label_23->setMinimumSize(QSize(30, 20));
		label_23->setMaximumSize(QSize(30, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_25 = new QLabel(CGainClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(550, 470, 30, 20));
		label_25->setMinimumSize(QSize(30, 20));
		label_25->setMaximumSize(QSize(30, 20));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_5 = new QLabel(CGainClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(170, 190, 30, 20));
		label_5->setMinimumSize(QSize(30, 20));
		label_5->setMaximumSize(QSize(30, 20));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_6 = new QLabel(CGainClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(170, 230, 30, 20));
		label_6->setMinimumSize(QSize(30, 20));
		label_6->setMaximumSize(QSize(30, 20));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe6 = new QLineEdit(CGainClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setGeometry(QRect(90, 350, 80, 20));
		lineEditProbe6->setMinimumSize(QSize(80, 20));
		lineEditProbe6->setMaximumSize(QSize(80, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11 = new QLineEdit(CGainClass);
		lineEditProbe11->setObjectName(QString::fromUtf8("lineEditProbe11"));
		lineEditProbe11->setGeometry(QRect(280, 150, 80, 20));
		lineEditProbe11->setMinimumSize(QSize(80, 20));
		lineEditProbe11->setMaximumSize(QSize(80, 20));
		lineEditProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_22 = new QLabel(CGainClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(360, 270, 30, 20));
		label_22->setMinimumSize(QSize(30, 20));
		label_22->setMaximumSize(QSize(30, 20));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_26 = new QLabel(CGainClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(550, 190, 30, 20));
		label_26->setMinimumSize(QSize(30, 20));
		label_26->setMaximumSize(QSize(30, 20));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe2 = new QLineEdit(CGainClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setGeometry(QRect(90, 190, 80, 20));
		lineEditProbe2->setMinimumSize(QSize(80, 20));
		lineEditProbe2->setMaximumSize(QSize(80, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_21 = new QLabel(CGainClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(360, 190, 30, 20));
		label_21->setMinimumSize(QSize(30, 20));
		label_21->setMaximumSize(QSize(30, 20));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe14 = new QLineEdit(CGainClass);
		lineEditProbe14->setObjectName(QString::fromUtf8("lineEditProbe14"));
		lineEditProbe14->setGeometry(QRect(280, 270, 80, 20));
		lineEditProbe14->setMinimumSize(QSize(80, 20));
		lineEditProbe14->setMaximumSize(QSize(80, 20));
		lineEditProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe2 = new QCheckBox(CGainClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(40, 190, 50, 20));
		checkBoxProbe2->setMinimumSize(QSize(50, 20));
		checkBoxProbe2->setMaximumSize(QSize(50, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe18 = new QLineEdit(CGainClass);
		lineEditProbe18->setObjectName(QString::fromUtf8("lineEditProbe18"));
		lineEditProbe18->setGeometry(QRect(280, 430, 80, 20));
		lineEditProbe18->setMinimumSize(QSize(80, 20));
		lineEditProbe18->setMaximumSize(QSize(80, 20));
		lineEditProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe22 = new QCheckBox(CGainClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(410, 190, 50, 20));
		checkBoxProbe22->setMinimumSize(QSize(50, 20));
		checkBoxProbe22->setMaximumSize(QSize(50, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAll = new QRadioButton(CGainClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(40, 60, 89, 16));
		radioButtonAll->setMinimumSize(QSize(89, 16));
		radioButtonAll->setMaximumSize(QSize(89, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditAny = new QLineEdit(CGainClass);
		lineEditAny->setObjectName(QString::fromUtf8("lineEditAny"));
		lineEditAny->setGeometry(QRect(130, 100, 80, 20));
		lineEditAny->setMinimumSize(QSize(80, 20));
		lineEditAny->setMaximumSize(QSize(80, 20));
		lineEditAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe21 = new QCheckBox(CGainClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(410, 150, 50, 20));
		checkBoxProbe21->setMinimumSize(QSize(50, 20));
		checkBoxProbe21->setMaximumSize(QSize(50, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(CGainClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(360, 150, 30, 20));
		label_20->setMinimumSize(QSize(30, 20));
		label_20->setMaximumSize(QSize(30, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13 = new QCheckBox(CGainClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(220, 230, 50, 20));
		checkBoxProbe13->setMinimumSize(QSize(50, 20));
		checkBoxProbe13->setMaximumSize(QSize(50, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe20 = new QCheckBox(CGainClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(220, 510, 50, 20));
		checkBoxProbe20->setMinimumSize(QSize(50, 20));
		checkBoxProbe20->setMaximumSize(QSize(50, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label = new QLabel(CGainClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(210, 60, 30, 20));
		label->setMinimumSize(QSize(30, 20));
		label->setMaximumSize(QSize(30, 20));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllIn = new QPushButton(CGainClass);
		pushButtonAllIn->setObjectName(QString::fromUtf8("pushButtonAllIn"));
		pushButtonAllIn->setGeometry(QRect(260, 60, 50, 23));
		pushButtonAllIn->setMinimumSize(QSize(50, 23));
		pushButtonAllIn->setMaximumSize(QSize(50, 23));
		pushButtonAllIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyIn = new QPushButton(CGainClass);
		pushButtonAnyIn->setObjectName(QString::fromUtf8("pushButtonAnyIn"));
		pushButtonAnyIn->setGeometry(QRect(260, 100, 50, 23));
		pushButtonAnyIn->setMinimumSize(QSize(50, 23));
		pushButtonAnyIn->setMaximumSize(QSize(50, 23));
		pushButtonAnyIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe32 = new QLineEdit(CGainClass);
		lineEditProbe32->setObjectName(QString::fromUtf8("lineEditProbe32"));
		lineEditProbe32->setGeometry(QRect(660, 190, 80, 20));
		lineEditProbe32->setMinimumSize(QSize(80, 20));
		lineEditProbe32->setMaximumSize(QSize(80, 20));
		lineEditProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_34 = new QLabel(CGainClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(740, 230, 30, 20));
		label_34->setMinimumSize(QSize(30, 20));
		label_34->setMaximumSize(QSize(30, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(CGainClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(600, 150, 50, 20));
		checkBoxProbe31->setMinimumSize(QSize(50, 20));
		checkBoxProbe31->setMaximumSize(QSize(50, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(CGainClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(600, 270, 50, 20));
		checkBoxProbe34->setMinimumSize(QSize(50, 20));
		checkBoxProbe34->setMaximumSize(QSize(50, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_35 = new QLabel(CGainClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(740, 310, 30, 20));
		label_35->setMinimumSize(QSize(30, 20));
		label_35->setMaximumSize(QSize(30, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe36 = new QLineEdit(CGainClass);
		lineEditProbe36->setObjectName(QString::fromUtf8("lineEditProbe36"));
		lineEditProbe36->setGeometry(QRect(660, 350, 80, 20));
		lineEditProbe36->setMinimumSize(QSize(80, 20));
		lineEditProbe36->setMaximumSize(QSize(80, 20));
		lineEditProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe34 = new QLineEdit(CGainClass);
		lineEditProbe34->setObjectName(QString::fromUtf8("lineEditProbe34"));
		lineEditProbe34->setGeometry(QRect(660, 270, 80, 20));
		lineEditProbe34->setMinimumSize(QSize(80, 20));
		lineEditProbe34->setMaximumSize(QSize(80, 20));
		lineEditProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe32 = new QCheckBox(CGainClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(600, 190, 50, 20));
		checkBoxProbe32->setMinimumSize(QSize(50, 20));
		checkBoxProbe32->setMaximumSize(QSize(50, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(CGainClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(740, 350, 30, 20));
		label_36->setMinimumSize(QSize(30, 20));
		label_36->setMaximumSize(QSize(30, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(CGainClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(600, 230, 50, 20));
		checkBoxProbe33->setMinimumSize(QSize(50, 20));
		checkBoxProbe33->setMaximumSize(QSize(50, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_37 = new QLabel(CGainClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(740, 270, 30, 20));
		label_37->setMinimumSize(QSize(30, 20));
		label_37->setMaximumSize(QSize(30, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_38 = new QLabel(CGainClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(740, 150, 30, 20));
		label_38->setMinimumSize(QSize(30, 20));
		label_38->setMaximumSize(QSize(30, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe36 = new QCheckBox(CGainClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(600, 350, 50, 20));
		checkBoxProbe36->setMinimumSize(QSize(50, 20));
		checkBoxProbe36->setMaximumSize(QSize(50, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_39 = new QLabel(CGainClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(740, 190, 30, 20));
		label_39->setMinimumSize(QSize(30, 20));
		label_39->setMaximumSize(QSize(30, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe35 = new QCheckBox(CGainClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(600, 310, 50, 20));
		checkBoxProbe35->setMinimumSize(QSize(50, 20));
		checkBoxProbe35->setMaximumSize(QSize(50, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe31 = new QLineEdit(CGainClass);
		lineEditProbe31->setObjectName(QString::fromUtf8("lineEditProbe31"));
		lineEditProbe31->setGeometry(QRect(660, 150, 80, 20));
		lineEditProbe31->setMinimumSize(QSize(80, 20));
		lineEditProbe31->setMaximumSize(QSize(80, 20));
		lineEditProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe33 = new QLineEdit(CGainClass);
		lineEditProbe33->setObjectName(QString::fromUtf8("lineEditProbe33"));
		lineEditProbe33->setGeometry(QRect(660, 230, 80, 20));
		lineEditProbe33->setMinimumSize(QSize(80, 20));
		lineEditProbe33->setMaximumSize(QSize(80, 20));
		lineEditProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe35 = new QLineEdit(CGainClass);
		lineEditProbe35->setObjectName(QString::fromUtf8("lineEditProbe35"));
		lineEditProbe35->setGeometry(QRect(660, 310, 80, 20));
		lineEditProbe35->setMinimumSize(QSize(80, 20));
		lineEditProbe35->setMaximumSize(QSize(80, 20));
		lineEditProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));

		retranslateUi(CGainClass);

		QMetaObject::connectSlotsByName(CGainClass);
	} // setupUi

	void retranslateUi(QDialog *CGainClass)
	{
		CGainClass->setWindowTitle(QApplication::translate("CGainClass", "CGain", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("CGainClass", " 26", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("CGainClass", " 28", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("CGainClass", " 16", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("CGainClass", " 18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("CGainClass", " 11", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("CGainClass", " 17", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("CGainClass", " 25", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("CGainClass", " 6", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("CGainClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("CGainClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("CGainClass", " 1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("CGainClass", " 14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("CGainClass", " 30", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("CGainClass", "\345\217\202\350\200\203\345\242\236\347\233\212\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("CGainClass", " 23", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("CGainClass", " 24", 0, QApplication::UnicodeUTF8));
		label_32->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("CGainClass", " 8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("CGainClass", " 10", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_31->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("CGainClass", " 5", 0, QApplication::UnicodeUTF8));
		label_33->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("CGainClass", " 9", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("CGainClass", " 27", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("CGainClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("CGainClass", " 15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("CGainClass", " 7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("CGainClass", " 4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("CGainClass", " 29", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("CGainClass", " 3", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("CGainClass", " 12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("CGainClass", " 19", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("CGainClass", " 2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("CGainClass", " 22", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("CGainClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("CGainClass", " 21", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("CGainClass", " 13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("CGainClass", " 20", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		pushButtonAllIn->setText(QApplication::translate("CGainClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyIn->setText(QApplication::translate("CGainClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("CGainClass", " 31", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("CGainClass", " 34", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("CGainClass", " 32", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("CGainClass", " 33", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("CGainClass", " 36", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("CGainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("CGainClass", " 35", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class CGainClass: public Ui_CGainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CGAIN_H




/********************************************************************************
** Form generated from reading UI file 'gain.ui'
**
** Created: Thu Jun 19 09:27:59 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAIN_H
#define UI_GAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_GainClass
{
public:
	QRadioButton *radioButtonAll;
	QRadioButton *radioButtonAny;
	QLabel *label;
	QLabel *label_2;
	QLineEdit *lineEditAll;
	QLineEdit *lineEditAny;
	QCheckBox *checkBoxProbe1;
	QLineEdit *lineEditProbe1;
	QLineEdit *lineEditProbe2;
	QCheckBox *checkBoxProbe2;
	QCheckBox *checkBoxProbe3;
	QLineEdit *lineEditProbe3;
	QLineEdit *lineEditProbe4;
	QCheckBox *checkBoxProbe4;
	QCheckBox *checkBoxProbe8;
	QLineEdit *lineEditProbe5;
	QLineEdit *lineEditProbe8;
	QCheckBox *checkBoxProbe6;
	QLineEdit *lineEditProbe7;
	QLineEdit *lineEditProbe6;
	QCheckBox *checkBoxProbe7;
	QCheckBox *checkBoxProbe5;
	QCheckBox *checkBoxProbe9;
	QLineEdit *lineEditProbe9;
	QCheckBox *checkBoxProbe10;
	QLineEdit *lineEditProbe10;
	QLineEdit *lineEditProbe20;
	QCheckBox *checkBoxProbe19;
	QLineEdit *lineEditProbe12;
	QLineEdit *lineEditProbe11;
	QCheckBox *checkBoxProbe11;
	QLineEdit *lineEditProbe19;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe12;
	QLineEdit *lineEditProbe18;
	QLineEdit *lineEditProbe15;
	QCheckBox *checkBoxProbe15;
	QLineEdit *lineEditProbe16;
	QCheckBox *checkBoxProbe20;
	QCheckBox *checkBoxProbe13;
	QCheckBox *checkBoxProbe16;
	QCheckBox *checkBoxProbe18;
	QCheckBox *checkBoxProbe17;
	QLineEdit *lineEditProbe13;
	QLineEdit *lineEditProbe14;
	QLineEdit *lineEditProbe17;
	QLineEdit *lineEditProbe27;
	QLineEdit *lineEditProbe24;
	QCheckBox *checkBoxProbe29;
	QCheckBox *checkBoxProbe26;
	QLineEdit *lineEditProbe21;
	QCheckBox *checkBoxProbe21;
	QCheckBox *checkBoxProbe28;
	QCheckBox *checkBoxProbe23;
	QLineEdit *lineEditProbe23;
	QLineEdit *lineEditProbe25;
	QLineEdit *lineEditProbe28;
	QLineEdit *lineEditProbe26;
	QCheckBox *checkBoxProbe27;
	QCheckBox *checkBoxProbe22;
	QCheckBox *checkBoxProbe25;
	QCheckBox *checkBoxProbe30;
	QCheckBox *checkBoxProbe24;
	QLineEdit *lineEditProbe22;
	QLineEdit *lineEditProbe30;
	QLineEdit *lineEditProbe29;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QLabel *label_6;
	QLabel *label_7;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QLabel *label_14;
	QLabel *label_15;
	QLabel *label_16;
	QLabel *label_17;
	QLabel *label_18;
	QLabel *label_19;
	QLabel *label_20;
	QLabel *label_21;
	QLabel *label_22;
	QLabel *label_23;
	QLabel *label_24;
	QLabel *label_25;
	QLabel *label_26;
	QLabel *label_27;
	QLabel *label_28;
	QLabel *label_29;
	QLabel *label_30;
	QLabel *label_31;
	QLabel *label_32;
	QLabel *label_33;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QPushButton *pushButtonAllIn;
	QPushButton *pushButtonAnyIn;
	QLabel *label_34;
	QLineEdit *lineEditProbe34;
	QLineEdit *lineEditProbe33;
	QLabel *label_35;
	QLineEdit *lineEditProbe35;
	QCheckBox *checkBoxProbe35;
	QLabel *label_36;
	QCheckBox *checkBoxProbe33;
	QCheckBox *checkBoxProbe36;
	QCheckBox *checkBoxProbe32;
	QLabel *label_37;
	QLabel *label_38;
	QLineEdit *lineEditProbe36;
	QLabel *label_39;
	QCheckBox *checkBoxProbe31;
	QLineEdit *lineEditProbe32;
	QLineEdit *lineEditProbe31;
	QCheckBox *checkBoxProbe34;

	void setupUi(QDialog *GainClass)
	{
		if (GainClass->objectName().isEmpty())
			GainClass->setObjectName(QString::fromUtf8("GainClass"));
		GainClass->resize(800, 586);
		GainClass->setMinimumSize(QSize(800, 586));
		GainClass->setMaximumSize(QSize(800, 586));
		GainClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		radioButtonAll = new QRadioButton(GainClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(30, 60, 89, 16));
		radioButtonAll->setMinimumSize(QSize(89, 16));
		radioButtonAll->setMaximumSize(QSize(89, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAny = new QRadioButton(GainClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(30, 100, 89, 16));
		radioButtonAny->setMinimumSize(QSize(89, 16));
		radioButtonAny->setMaximumSize(QSize(89, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label = new QLabel(GainClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(200, 60, 30, 20));
		label->setMinimumSize(QSize(30, 20));
		label->setMaximumSize(QSize(30, 20));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_2 = new QLabel(GainClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(330, 20, 120, 20));
		label_2->setMinimumSize(QSize(120, 20));
		label_2->setMaximumSize(QSize(120, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		lineEditAll = new QLineEdit(GainClass);
		lineEditAll->setObjectName(QString::fromUtf8("lineEditAll"));
		lineEditAll->setGeometry(QRect(120, 60, 80, 20));
		lineEditAll->setMinimumSize(QSize(80, 20));
		lineEditAll->setMaximumSize(QSize(80, 20));
		lineEditAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAny = new QLineEdit(GainClass);
		lineEditAny->setObjectName(QString::fromUtf8("lineEditAny"));
		lineEditAny->setGeometry(QRect(120, 100, 80, 20));
		lineEditAny->setMinimumSize(QSize(80, 20));
		lineEditAny->setMaximumSize(QSize(80, 20));
		lineEditAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe1 = new QCheckBox(GainClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(30, 150, 50, 20));
		checkBoxProbe1->setMinimumSize(QSize(50, 20));
		checkBoxProbe1->setMaximumSize(QSize(50, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe1 = new QLineEdit(GainClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setGeometry(QRect(80, 150, 80, 20));
		lineEditProbe1->setMinimumSize(QSize(80, 20));
		lineEditProbe1->setMaximumSize(QSize(80, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2 = new QLineEdit(GainClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setGeometry(QRect(80, 190, 80, 20));
		lineEditProbe2->setMinimumSize(QSize(80, 20));
		lineEditProbe2->setMaximumSize(QSize(80, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe2 = new QCheckBox(GainClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(30, 190, 50, 20));
		checkBoxProbe2->setMinimumSize(QSize(50, 20));
		checkBoxProbe2->setMaximumSize(QSize(50, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(GainClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(30, 230, 50, 20));
		checkBoxProbe3->setMinimumSize(QSize(50, 20));
		checkBoxProbe3->setMaximumSize(QSize(50, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe3 = new QLineEdit(GainClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setGeometry(QRect(80, 230, 80, 20));
		lineEditProbe3->setMinimumSize(QSize(80, 20));
		lineEditProbe3->setMaximumSize(QSize(80, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4 = new QLineEdit(GainClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setGeometry(QRect(80, 270, 80, 20));
		lineEditProbe4->setMinimumSize(QSize(80, 20));
		lineEditProbe4->setMaximumSize(QSize(80, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe4 = new QCheckBox(GainClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(30, 270, 50, 20));
		checkBoxProbe4->setMinimumSize(QSize(50, 20));
		checkBoxProbe4->setMaximumSize(QSize(50, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe8 = new QCheckBox(GainClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(30, 430, 50, 20));
		checkBoxProbe8->setMinimumSize(QSize(50, 20));
		checkBoxProbe8->setMaximumSize(QSize(50, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe5 = new QLineEdit(GainClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setGeometry(QRect(80, 310, 80, 20));
		lineEditProbe5->setMinimumSize(QSize(80, 20));
		lineEditProbe5->setMaximumSize(QSize(80, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8 = new QLineEdit(GainClass);
		lineEditProbe8->setObjectName(QString::fromUtf8("lineEditProbe8"));
		lineEditProbe8->setGeometry(QRect(80, 430, 80, 20));
		lineEditProbe8->setMinimumSize(QSize(80, 20));
		lineEditProbe8->setMaximumSize(QSize(80, 20));
		lineEditProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe6 = new QCheckBox(GainClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(30, 350, 50, 20));
		checkBoxProbe6->setMinimumSize(QSize(50, 20));
		checkBoxProbe6->setMaximumSize(QSize(50, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe7 = new QLineEdit(GainClass);
		lineEditProbe7->setObjectName(QString::fromUtf8("lineEditProbe7"));
		lineEditProbe7->setGeometry(QRect(80, 390, 80, 20));
		lineEditProbe7->setMinimumSize(QSize(80, 20));
		lineEditProbe7->setMaximumSize(QSize(80, 20));
		lineEditProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6 = new QLineEdit(GainClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setGeometry(QRect(80, 350, 80, 20));
		lineEditProbe6->setMinimumSize(QSize(80, 20));
		lineEditProbe6->setMaximumSize(QSize(80, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe7 = new QCheckBox(GainClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(30, 390, 50, 20));
		checkBoxProbe7->setMinimumSize(QSize(50, 20));
		checkBoxProbe7->setMaximumSize(QSize(50, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe5 = new QCheckBox(GainClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(30, 310, 50, 20));
		checkBoxProbe5->setMinimumSize(QSize(50, 20));
		checkBoxProbe5->setMaximumSize(QSize(50, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe9 = new QCheckBox(GainClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(30, 470, 50, 20));
		checkBoxProbe9->setMinimumSize(QSize(50, 20));
		checkBoxProbe9->setMaximumSize(QSize(50, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe9 = new QLineEdit(GainClass);
		lineEditProbe9->setObjectName(QString::fromUtf8("lineEditProbe9"));
		lineEditProbe9->setGeometry(QRect(80, 470, 80, 20));
		lineEditProbe9->setMinimumSize(QSize(80, 20));
		lineEditProbe9->setMaximumSize(QSize(80, 20));
		lineEditProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe10 = new QCheckBox(GainClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(30, 510, 50, 20));
		checkBoxProbe10->setMinimumSize(QSize(50, 20));
		checkBoxProbe10->setMaximumSize(QSize(50, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe10 = new QLineEdit(GainClass);
		lineEditProbe10->setObjectName(QString::fromUtf8("lineEditProbe10"));
		lineEditProbe10->setGeometry(QRect(80, 510, 80, 20));
		lineEditProbe10->setMinimumSize(QSize(80, 20));
		lineEditProbe10->setMaximumSize(QSize(80, 20));
		lineEditProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe20 = new QLineEdit(GainClass);
		lineEditProbe20->setObjectName(QString::fromUtf8("lineEditProbe20"));
		lineEditProbe20->setGeometry(QRect(270, 510, 80, 20));
		lineEditProbe20->setMinimumSize(QSize(80, 20));
		lineEditProbe20->setMaximumSize(QSize(80, 20));
		lineEditProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe19 = new QCheckBox(GainClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(210, 470, 50, 20));
		checkBoxProbe19->setMinimumSize(QSize(50, 20));
		checkBoxProbe19->setMaximumSize(QSize(50, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe12 = new QLineEdit(GainClass);
		lineEditProbe12->setObjectName(QString::fromUtf8("lineEditProbe12"));
		lineEditProbe12->setGeometry(QRect(270, 190, 80, 20));
		lineEditProbe12->setMinimumSize(QSize(80, 20));
		lineEditProbe12->setMaximumSize(QSize(80, 20));
		lineEditProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11 = new QLineEdit(GainClass);
		lineEditProbe11->setObjectName(QString::fromUtf8("lineEditProbe11"));
		lineEditProbe11->setGeometry(QRect(270, 150, 80, 20));
		lineEditProbe11->setMinimumSize(QSize(80, 20));
		lineEditProbe11->setMaximumSize(QSize(80, 20));
		lineEditProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe11 = new QCheckBox(GainClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(210, 150, 50, 20));
		checkBoxProbe11->setMinimumSize(QSize(50, 20));
		checkBoxProbe11->setMaximumSize(QSize(50, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe19 = new QLineEdit(GainClass);
		lineEditProbe19->setObjectName(QString::fromUtf8("lineEditProbe19"));
		lineEditProbe19->setGeometry(QRect(270, 470, 80, 20));
		lineEditProbe19->setMinimumSize(QSize(80, 20));
		lineEditProbe19->setMaximumSize(QSize(80, 20));
		lineEditProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe14 = new QCheckBox(GainClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(210, 270, 50, 20));
		checkBoxProbe14->setMinimumSize(QSize(50, 20));
		checkBoxProbe14->setMaximumSize(QSize(50, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe12 = new QCheckBox(GainClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(210, 190, 50, 20));
		checkBoxProbe12->setMinimumSize(QSize(50, 20));
		checkBoxProbe12->setMaximumSize(QSize(50, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe18 = new QLineEdit(GainClass);
		lineEditProbe18->setObjectName(QString::fromUtf8("lineEditProbe18"));
		lineEditProbe18->setGeometry(QRect(270, 430, 80, 20));
		lineEditProbe18->setMinimumSize(QSize(80, 20));
		lineEditProbe18->setMaximumSize(QSize(80, 20));
		lineEditProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe15 = new QLineEdit(GainClass);
		lineEditProbe15->setObjectName(QString::fromUtf8("lineEditProbe15"));
		lineEditProbe15->setGeometry(QRect(270, 310, 80, 20));
		lineEditProbe15->setMinimumSize(QSize(80, 20));
		lineEditProbe15->setMaximumSize(QSize(80, 20));
		lineEditProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe15 = new QCheckBox(GainClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(210, 310, 50, 20));
		checkBoxProbe15->setMinimumSize(QSize(50, 20));
		checkBoxProbe15->setMaximumSize(QSize(50, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe16 = new QLineEdit(GainClass);
		lineEditProbe16->setObjectName(QString::fromUtf8("lineEditProbe16"));
		lineEditProbe16->setGeometry(QRect(270, 350, 80, 20));
		lineEditProbe16->setMinimumSize(QSize(80, 20));
		lineEditProbe16->setMaximumSize(QSize(80, 20));
		lineEditProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe20 = new QCheckBox(GainClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(210, 510, 50, 20));
		checkBoxProbe20->setMinimumSize(QSize(50, 20));
		checkBoxProbe20->setMaximumSize(QSize(50, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13 = new QCheckBox(GainClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(210, 230, 50, 20));
		checkBoxProbe13->setMinimumSize(QSize(50, 20));
		checkBoxProbe13->setMaximumSize(QSize(50, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe16 = new QCheckBox(GainClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(210, 350, 50, 20));
		checkBoxProbe16->setMinimumSize(QSize(50, 20));
		checkBoxProbe16->setMaximumSize(QSize(50, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe18 = new QCheckBox(GainClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(210, 430, 50, 20));
		checkBoxProbe18->setMinimumSize(QSize(50, 20));
		checkBoxProbe18->setMaximumSize(QSize(50, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe17 = new QCheckBox(GainClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(210, 390, 50, 20));
		checkBoxProbe17->setMinimumSize(QSize(50, 20));
		checkBoxProbe17->setMaximumSize(QSize(50, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe13 = new QLineEdit(GainClass);
		lineEditProbe13->setObjectName(QString::fromUtf8("lineEditProbe13"));
		lineEditProbe13->setGeometry(QRect(270, 230, 80, 20));
		lineEditProbe13->setMinimumSize(QSize(80, 20));
		lineEditProbe13->setMaximumSize(QSize(80, 20));
		lineEditProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe14 = new QLineEdit(GainClass);
		lineEditProbe14->setObjectName(QString::fromUtf8("lineEditProbe14"));
		lineEditProbe14->setGeometry(QRect(270, 270, 80, 20));
		lineEditProbe14->setMinimumSize(QSize(80, 20));
		lineEditProbe14->setMaximumSize(QSize(80, 20));
		lineEditProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe17 = new QLineEdit(GainClass);
		lineEditProbe17->setObjectName(QString::fromUtf8("lineEditProbe17"));
		lineEditProbe17->setGeometry(QRect(270, 390, 80, 20));
		lineEditProbe17->setMinimumSize(QSize(80, 20));
		lineEditProbe17->setMaximumSize(QSize(80, 20));
		lineEditProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe27 = new QLineEdit(GainClass);
		lineEditProbe27->setObjectName(QString::fromUtf8("lineEditProbe27"));
		lineEditProbe27->setGeometry(QRect(460, 390, 80, 20));
		lineEditProbe27->setMinimumSize(QSize(80, 20));
		lineEditProbe27->setMaximumSize(QSize(80, 20));
		lineEditProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe24 = new QLineEdit(GainClass);
		lineEditProbe24->setObjectName(QString::fromUtf8("lineEditProbe24"));
		lineEditProbe24->setGeometry(QRect(460, 270, 80, 20));
		lineEditProbe24->setMinimumSize(QSize(80, 20));
		lineEditProbe24->setMaximumSize(QSize(80, 20));
		lineEditProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe29 = new QCheckBox(GainClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(400, 470, 50, 20));
		checkBoxProbe29->setMinimumSize(QSize(50, 20));
		checkBoxProbe29->setMaximumSize(QSize(50, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe26 = new QCheckBox(GainClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(400, 350, 50, 20));
		checkBoxProbe26->setMinimumSize(QSize(50, 20));
		checkBoxProbe26->setMaximumSize(QSize(50, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe21 = new QLineEdit(GainClass);
		lineEditProbe21->setObjectName(QString::fromUtf8("lineEditProbe21"));
		lineEditProbe21->setGeometry(QRect(460, 150, 80, 20));
		lineEditProbe21->setMinimumSize(QSize(80, 20));
		lineEditProbe21->setMaximumSize(QSize(80, 20));
		lineEditProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe21 = new QCheckBox(GainClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(400, 150, 50, 20));
		checkBoxProbe21->setMinimumSize(QSize(50, 20));
		checkBoxProbe21->setMaximumSize(QSize(50, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe28 = new QCheckBox(GainClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(400, 430, 50, 20));
		checkBoxProbe28->setMinimumSize(QSize(50, 20));
		checkBoxProbe28->setMaximumSize(QSize(50, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe23 = new QCheckBox(GainClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(400, 230, 50, 20));
		checkBoxProbe23->setMinimumSize(QSize(50, 20));
		checkBoxProbe23->setMaximumSize(QSize(50, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe23 = new QLineEdit(GainClass);
		lineEditProbe23->setObjectName(QString::fromUtf8("lineEditProbe23"));
		lineEditProbe23->setGeometry(QRect(460, 230, 80, 20));
		lineEditProbe23->setMinimumSize(QSize(80, 20));
		lineEditProbe23->setMaximumSize(QSize(80, 20));
		lineEditProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe25 = new QLineEdit(GainClass);
		lineEditProbe25->setObjectName(QString::fromUtf8("lineEditProbe25"));
		lineEditProbe25->setGeometry(QRect(460, 310, 80, 20));
		lineEditProbe25->setMinimumSize(QSize(80, 20));
		lineEditProbe25->setMaximumSize(QSize(80, 20));
		lineEditProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe28 = new QLineEdit(GainClass);
		lineEditProbe28->setObjectName(QString::fromUtf8("lineEditProbe28"));
		lineEditProbe28->setGeometry(QRect(460, 430, 80, 20));
		lineEditProbe28->setMinimumSize(QSize(80, 20));
		lineEditProbe28->setMaximumSize(QSize(80, 20));
		lineEditProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe26 = new QLineEdit(GainClass);
		lineEditProbe26->setObjectName(QString::fromUtf8("lineEditProbe26"));
		lineEditProbe26->setGeometry(QRect(460, 350, 80, 20));
		lineEditProbe26->setMinimumSize(QSize(80, 20));
		lineEditProbe26->setMaximumSize(QSize(80, 20));
		lineEditProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe27 = new QCheckBox(GainClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(400, 390, 50, 20));
		checkBoxProbe27->setMinimumSize(QSize(50, 20));
		checkBoxProbe27->setMaximumSize(QSize(50, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe22 = new QCheckBox(GainClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(400, 190, 50, 20));
		checkBoxProbe22->setMinimumSize(QSize(50, 20));
		checkBoxProbe22->setMaximumSize(QSize(50, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25 = new QCheckBox(GainClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(400, 310, 50, 20));
		checkBoxProbe25->setMinimumSize(QSize(50, 20));
		checkBoxProbe25->setMaximumSize(QSize(50, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe30 = new QCheckBox(GainClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(400, 510, 50, 20));
		checkBoxProbe30->setMinimumSize(QSize(50, 20));
		checkBoxProbe30->setMaximumSize(QSize(50, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe24 = new QCheckBox(GainClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(400, 270, 50, 20));
		checkBoxProbe24->setMinimumSize(QSize(50, 20));
		checkBoxProbe24->setMaximumSize(QSize(50, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe22 = new QLineEdit(GainClass);
		lineEditProbe22->setObjectName(QString::fromUtf8("lineEditProbe22"));
		lineEditProbe22->setGeometry(QRect(460, 190, 80, 20));
		lineEditProbe22->setMinimumSize(QSize(80, 20));
		lineEditProbe22->setMaximumSize(QSize(80, 20));
		lineEditProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe30 = new QLineEdit(GainClass);
		lineEditProbe30->setObjectName(QString::fromUtf8("lineEditProbe30"));
		lineEditProbe30->setGeometry(QRect(460, 510, 80, 20));
		lineEditProbe30->setMinimumSize(QSize(80, 20));
		lineEditProbe30->setMaximumSize(QSize(80, 20));
		lineEditProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe29 = new QLineEdit(GainClass);
		lineEditProbe29->setObjectName(QString::fromUtf8("lineEditProbe29"));
		lineEditProbe29->setGeometry(QRect(460, 470, 80, 20));
		lineEditProbe29->setMinimumSize(QSize(80, 20));
		lineEditProbe29->setMaximumSize(QSize(80, 20));
		lineEditProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_3 = new QLabel(GainClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(200, 100, 30, 20));
		label_3->setMinimumSize(QSize(30, 20));
		label_3->setMaximumSize(QSize(30, 20));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(GainClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(160, 150, 30, 20));
		label_4->setMinimumSize(QSize(30, 20));
		label_4->setMaximumSize(QSize(30, 20));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_5 = new QLabel(GainClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(160, 190, 30, 20));
		label_5->setMinimumSize(QSize(30, 20));
		label_5->setMaximumSize(QSize(30, 20));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_6 = new QLabel(GainClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(160, 230, 30, 20));
		label_6->setMinimumSize(QSize(30, 20));
		label_6->setMaximumSize(QSize(30, 20));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(GainClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(160, 270, 30, 20));
		label_7->setMinimumSize(QSize(30, 20));
		label_7->setMaximumSize(QSize(30, 20));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(GainClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(160, 310, 30, 20));
		label_8->setMinimumSize(QSize(30, 20));
		label_8->setMaximumSize(QSize(30, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(GainClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(160, 350, 30, 20));
		label_9->setMinimumSize(QSize(30, 20));
		label_9->setMaximumSize(QSize(30, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(GainClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(160, 390, 30, 20));
		label_10->setMinimumSize(QSize(30, 20));
		label_10->setMaximumSize(QSize(30, 20));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_11 = new QLabel(GainClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(160, 430, 30, 20));
		label_11->setMinimumSize(QSize(30, 20));
		label_11->setMaximumSize(QSize(30, 20));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_12 = new QLabel(GainClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(160, 470, 30, 20));
		label_12->setMinimumSize(QSize(30, 20));
		label_12->setMaximumSize(QSize(30, 20));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_13 = new QLabel(GainClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(160, 510, 30, 20));
		label_13->setMinimumSize(QSize(30, 20));
		label_13->setMaximumSize(QSize(30, 20));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_14 = new QLabel(GainClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(350, 510, 30, 20));
		label_14->setMinimumSize(QSize(30, 20));
		label_14->setMaximumSize(QSize(30, 20));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_15 = new QLabel(GainClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(350, 470, 30, 20));
		label_15->setMinimumSize(QSize(30, 20));
		label_15->setMaximumSize(QSize(30, 20));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_16 = new QLabel(GainClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(350, 310, 30, 20));
		label_16->setMinimumSize(QSize(30, 20));
		label_16->setMaximumSize(QSize(30, 20));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_17 = new QLabel(GainClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(350, 350, 30, 20));
		label_17->setMinimumSize(QSize(30, 20));
		label_17->setMaximumSize(QSize(30, 20));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_18 = new QLabel(GainClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(350, 390, 30, 20));
		label_18->setMinimumSize(QSize(30, 20));
		label_18->setMaximumSize(QSize(30, 20));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_19 = new QLabel(GainClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(350, 430, 30, 20));
		label_19->setMinimumSize(QSize(30, 20));
		label_19->setMaximumSize(QSize(30, 20));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(GainClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(350, 150, 30, 20));
		label_20->setMinimumSize(QSize(30, 20));
		label_20->setMaximumSize(QSize(30, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_21 = new QLabel(GainClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(350, 190, 30, 20));
		label_21->setMinimumSize(QSize(30, 20));
		label_21->setMaximumSize(QSize(30, 20));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_22 = new QLabel(GainClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(350, 270, 30, 20));
		label_22->setMinimumSize(QSize(30, 20));
		label_22->setMaximumSize(QSize(30, 20));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(GainClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(350, 230, 30, 20));
		label_23->setMinimumSize(QSize(30, 20));
		label_23->setMaximumSize(QSize(30, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_24 = new QLabel(GainClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(540, 230, 30, 20));
		label_24->setMinimumSize(QSize(30, 20));
		label_24->setMaximumSize(QSize(30, 20));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_25 = new QLabel(GainClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(540, 470, 30, 20));
		label_25->setMinimumSize(QSize(30, 20));
		label_25->setMaximumSize(QSize(30, 20));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_26 = new QLabel(GainClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(540, 190, 30, 20));
		label_26->setMinimumSize(QSize(30, 20));
		label_26->setMaximumSize(QSize(30, 20));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_27 = new QLabel(GainClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(540, 350, 30, 20));
		label_27->setMinimumSize(QSize(30, 20));
		label_27->setMaximumSize(QSize(30, 20));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_28 = new QLabel(GainClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(540, 150, 30, 20));
		label_28->setMinimumSize(QSize(30, 20));
		label_28->setMaximumSize(QSize(30, 20));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(GainClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(540, 310, 30, 20));
		label_29->setMinimumSize(QSize(30, 20));
		label_29->setMaximumSize(QSize(30, 20));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_30 = new QLabel(GainClass);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(540, 390, 30, 20));
		label_30->setMinimumSize(QSize(30, 20));
		label_30->setMaximumSize(QSize(30, 20));
		label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_31 = new QLabel(GainClass);
		label_31->setObjectName(QString::fromUtf8("label_31"));
		label_31->setGeometry(QRect(540, 430, 30, 20));
		label_31->setMinimumSize(QSize(30, 20));
		label_31->setMaximumSize(QSize(30, 20));
		label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_32 = new QLabel(GainClass);
		label_32->setObjectName(QString::fromUtf8("label_32"));
		label_32->setGeometry(QRect(540, 510, 30, 20));
		label_32->setMinimumSize(QSize(30, 20));
		label_32->setMaximumSize(QSize(30, 20));
		label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_33 = new QLabel(GainClass);
		label_33->setObjectName(QString::fromUtf8("label_33"));
		label_33->setGeometry(QRect(540, 270, 30, 20));
		label_33->setMinimumSize(QSize(30, 20));
		label_33->setMaximumSize(QSize(30, 20));
		label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(GainClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(430, 550, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(GainClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(530, 550, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllIn = new QPushButton(GainClass);
		pushButtonAllIn->setObjectName(QString::fromUtf8("pushButtonAllIn"));
		pushButtonAllIn->setGeometry(QRect(250, 60, 50, 23));
		pushButtonAllIn->setMinimumSize(QSize(50, 23));
		pushButtonAllIn->setMaximumSize(QSize(50, 23));
		pushButtonAllIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyIn = new QPushButton(GainClass);
		pushButtonAnyIn->setObjectName(QString::fromUtf8("pushButtonAnyIn"));
		pushButtonAnyIn->setGeometry(QRect(250, 100, 50, 23));
		pushButtonAnyIn->setMinimumSize(QSize(50, 23));
		pushButtonAnyIn->setMaximumSize(QSize(50, 23));
		pushButtonAnyIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_34 = new QLabel(GainClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(740, 270, 30, 20));
		label_34->setMinimumSize(QSize(30, 20));
		label_34->setMaximumSize(QSize(30, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe34 = new QLineEdit(GainClass);
		lineEditProbe34->setObjectName(QString::fromUtf8("lineEditProbe34"));
		lineEditProbe34->setGeometry(QRect(660, 270, 80, 20));
		lineEditProbe34->setMinimumSize(QSize(80, 20));
		lineEditProbe34->setMaximumSize(QSize(80, 20));
		lineEditProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe33 = new QLineEdit(GainClass);
		lineEditProbe33->setObjectName(QString::fromUtf8("lineEditProbe33"));
		lineEditProbe33->setGeometry(QRect(660, 230, 80, 20));
		lineEditProbe33->setMinimumSize(QSize(80, 20));
		lineEditProbe33->setMaximumSize(QSize(80, 20));
		lineEditProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_35 = new QLabel(GainClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(740, 190, 30, 20));
		label_35->setMinimumSize(QSize(30, 20));
		label_35->setMaximumSize(QSize(30, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe35 = new QLineEdit(GainClass);
		lineEditProbe35->setObjectName(QString::fromUtf8("lineEditProbe35"));
		lineEditProbe35->setGeometry(QRect(660, 310, 80, 20));
		lineEditProbe35->setMinimumSize(QSize(80, 20));
		lineEditProbe35->setMaximumSize(QSize(80, 20));
		lineEditProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe35 = new QCheckBox(GainClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(600, 310, 50, 20));
		checkBoxProbe35->setMinimumSize(QSize(50, 20));
		checkBoxProbe35->setMaximumSize(QSize(50, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(GainClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(740, 230, 30, 20));
		label_36->setMinimumSize(QSize(30, 20));
		label_36->setMaximumSize(QSize(30, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(GainClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(600, 230, 50, 20));
		checkBoxProbe33->setMinimumSize(QSize(50, 20));
		checkBoxProbe33->setMaximumSize(QSize(50, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe36 = new QCheckBox(GainClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(600, 350, 50, 20));
		checkBoxProbe36->setMinimumSize(QSize(50, 20));
		checkBoxProbe36->setMaximumSize(QSize(50, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe32 = new QCheckBox(GainClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(600, 190, 50, 20));
		checkBoxProbe32->setMinimumSize(QSize(50, 20));
		checkBoxProbe32->setMaximumSize(QSize(50, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_37 = new QLabel(GainClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(740, 310, 30, 20));
		label_37->setMinimumSize(QSize(30, 20));
		label_37->setMaximumSize(QSize(30, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_38 = new QLabel(GainClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(740, 350, 30, 20));
		label_38->setMinimumSize(QSize(30, 20));
		label_38->setMaximumSize(QSize(30, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe36 = new QLineEdit(GainClass);
		lineEditProbe36->setObjectName(QString::fromUtf8("lineEditProbe36"));
		lineEditProbe36->setGeometry(QRect(660, 350, 80, 20));
		lineEditProbe36->setMinimumSize(QSize(80, 20));
		lineEditProbe36->setMaximumSize(QSize(80, 20));
		lineEditProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_39 = new QLabel(GainClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(740, 150, 30, 20));
		label_39->setMinimumSize(QSize(30, 20));
		label_39->setMaximumSize(QSize(30, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(GainClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(600, 150, 50, 20));
		checkBoxProbe31->setMinimumSize(QSize(50, 20));
		checkBoxProbe31->setMaximumSize(QSize(50, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe32 = new QLineEdit(GainClass);
		lineEditProbe32->setObjectName(QString::fromUtf8("lineEditProbe32"));
		lineEditProbe32->setGeometry(QRect(660, 190, 80, 20));
		lineEditProbe32->setMinimumSize(QSize(80, 20));
		lineEditProbe32->setMaximumSize(QSize(80, 20));
		lineEditProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe31 = new QLineEdit(GainClass);
		lineEditProbe31->setObjectName(QString::fromUtf8("lineEditProbe31"));
		lineEditProbe31->setGeometry(QRect(660, 150, 80, 20));
		lineEditProbe31->setMinimumSize(QSize(80, 20));
		lineEditProbe31->setMaximumSize(QSize(80, 20));
		lineEditProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe34 = new QCheckBox(GainClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(600, 270, 50, 20));
		checkBoxProbe34->setMinimumSize(QSize(50, 20));
		checkBoxProbe34->setMaximumSize(QSize(50, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));

		retranslateUi(GainClass);

		QMetaObject::connectSlotsByName(GainClass);
	} // setupUi

	void retranslateUi(QDialog *GainClass)
	{
		GainClass->setWindowTitle(QApplication::translate("GainClass", "Gain", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("GainClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("GainClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("GainClass", "\345\242\236\347\233\212\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("GainClass", " 1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("GainClass", " 2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("GainClass", " 3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("GainClass", " 4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("GainClass", " 8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("GainClass", " 6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("GainClass", " 7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("GainClass", " 5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("GainClass", " 9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("GainClass", " 10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("GainClass", " 19", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("GainClass", " 11", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("GainClass", " 14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("GainClass", " 12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("GainClass", " 15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("GainClass", " 20", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("GainClass", " 13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("GainClass", " 16", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("GainClass", " 18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("GainClass", " 17", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("GainClass", " 29", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("GainClass", " 26", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("GainClass", " 21", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("GainClass", " 28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("GainClass", " 23", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("GainClass", " 27", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("GainClass", " 22", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("GainClass", " 25", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("GainClass", " 30", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("GainClass", " 24", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_31->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_32->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_33->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("GainClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("GainClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		pushButtonAllIn->setText(QApplication::translate("GainClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyIn->setText(QApplication::translate("GainClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("GainClass", " 35", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("GainClass", " 33", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("GainClass", " 36", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("GainClass", " 32", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("GainClass", "(dB)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("GainClass", " 31", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("GainClass", " 34", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class GainClass: public Ui_GainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAIN_H




/********************************************************************************
** Form generated from reading UI file 'gateset.ui'
**
** Created: Tue Jun 24 00:02:16 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GATESET_H
#define UI_GATESET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_GateSetClass
{
public:
	QLabel *label_2;
	QLineEdit *lineEditAllIB;
	QRadioButton *radioButtonAll;
	QLabel *label;
	QLineEdit *lineEditAllIL;
	QLineEdit *lineEditAllIT;
	QRadioButton *radioButtonAny;
	QLabel *label_5;
	QLabel *label_6;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_7;
	QLineEdit *lineEditAnyIT;
	QLineEdit *lineEditAnyIL;
	QLineEdit *lineEditAnyIB;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QLineEdit *lineEditAnyAL;
	QLineEdit *lineEditAllAT;
	QLabel *label_11;
	QLineEdit *lineEditAnyAT;
	QLineEdit *lineEditAnyAB;
	QLabel *label_12;
	QLineEdit *lineEditAllAL;
	QLabel *label_13;
	QLineEdit *lineEditAllAB;
	QLabel *label_14;
	QLabel *label_15;
	QLabel *label_16;
	QLineEdit *lineEditAllBT;
	QLineEdit *lineEditAnyBT;
	QLabel *label_17;
	QLineEdit *lineEditAnyBB;
	QLabel *label_18;
	QLabel *label_19;
	QLineEdit *lineEditAllBB;
	QLabel *label_20;
	QLineEdit *lineEditAnyBL;
	QLabel *label_21;
	QLabel *label_22;
	QLineEdit *lineEditAllBL;
	QLineEdit *lineEditAllI2L;
	QLineEdit *lineEditAnyI2T;
	QLabel *label_23;
	QLabel *label_24;
	QLineEdit *lineEditAnyI2L;
	QLineEdit *lineEditAnyI2B;
	QLabel *label_25;
	QLineEdit *lineEditAllI2B;
	QLabel *label_26;
	QLabel *label_27;
	QLineEdit *lineEditAllI2T;
	QLabel *label_28;
	QLabel *label_29;
	QLineEdit *lineEditProbe2IB;
	QLineEdit *lineEditProbe2IL;
	QLineEdit *lineEditProbe1IT;
	QLineEdit *lineEditProbe2IT;
	QLineEdit *lineEditProbe1IL;
	QLineEdit *lineEditProbe1IB;
	QLineEdit *lineEditProbe3IB;
	QLineEdit *lineEditProbe3IT;
	QLineEdit *lineEditProbe4IT;
	QLineEdit *lineEditProbe3IL;
	QLineEdit *lineEditProbe4IB;
	QLineEdit *lineEditProbe4IL;
	QLineEdit *lineEditProbe6IL;
	QLineEdit *lineEditProbe6IB;
	QLineEdit *lineEditProbe5IL;
	QLineEdit *lineEditProbe5IT;
	QLineEdit *lineEditProbe6IT;
	QLineEdit *lineEditProbe5IB;
	QLineEdit *lineEditProbe7IB;
	QLineEdit *lineEditProbe8IL;
	QLineEdit *lineEditProbe7IT;
	QLineEdit *lineEditProbe8IT;
	QLineEdit *lineEditProbe8IB;
	QLineEdit *lineEditProbe7IL;
	QLineEdit *lineEditProbe9IL;
	QLineEdit *lineEditProbe10IB;
	QLineEdit *lineEditProbe10IL;
	QLineEdit *lineEditProbe9IT;
	QLineEdit *lineEditProbe9IB;
	QLineEdit *lineEditProbe10IT;
	QLineEdit *lineEditProbe10AT;
	QLineEdit *lineEditProbe1AB;
	QLineEdit *lineEditProbe2AL;
	QLineEdit *lineEditProbe4AL;
	QLineEdit *lineEditProbe2AB;
	QLineEdit *lineEditProbe1AL;
	QLineEdit *lineEditProbe3AT;
	QLineEdit *lineEditProbe9AL;
	QLineEdit *lineEditProbe8AB;
	QLineEdit *lineEditProbe3AL;
	QLineEdit *lineEditProbe7AT;
	QLineEdit *lineEditProbe5AB;
	QLineEdit *lineEditProbe1AT;
	QLineEdit *lineEditProbe6AL;
	QLineEdit *lineEditProbe5AT;
	QLineEdit *lineEditProbe4AB;
	QLineEdit *lineEditProbe2AT;
	QLineEdit *lineEditProbe6AT;
	QLineEdit *lineEditProbe6AB;
	QLineEdit *lineEditProbe8AT;
	QLineEdit *lineEditProbe4AT;
	QLineEdit *lineEditProbe10AL;
	QLineEdit *lineEditProbe7AL;
	QLineEdit *lineEditProbe7AB;
	QLineEdit *lineEditProbe8AL;
	QLineEdit *lineEditProbe5AL;
	QLineEdit *lineEditProbe10AB;
	QLineEdit *lineEditProbe9AT;
	QLineEdit *lineEditProbe3AB;
	QLineEdit *lineEditProbe9AB;
	QLineEdit *lineEditProbe9BB;
	QLineEdit *lineEditProbe4BB;
	QLineEdit *lineEditProbe1BT;
	QLineEdit *lineEditProbe9BL;
	QLineEdit *lineEditProbe7BB;
	QLineEdit *lineEditProbe7BT;
	QLineEdit *lineEditProbe6BT;
	QLineEdit *lineEditProbe2BB;
	QLineEdit *lineEditProbe5BB;
	QLineEdit *lineEditProbe3BT;
	QLineEdit *lineEditProbe8BB;
	QLineEdit *lineEditProbe6BB;
	QLineEdit *lineEditProbe3BL;
	QLineEdit *lineEditProbe10BL;
	QLineEdit *lineEditProbe2BT;
	QLineEdit *lineEditProbe4BL;
	QLineEdit *lineEditProbe8BL;
	QLineEdit *lineEditProbe9BT;
	QLineEdit *lineEditProbe7BL;
	QLineEdit *lineEditProbe8BT;
	QLineEdit *lineEditProbe10BT;
	QLineEdit *lineEditProbe5BL;
	QLineEdit *lineEditProbe6BL;
	QLineEdit *lineEditProbe2BL;
	QLineEdit *lineEditProbe5BT;
	QLineEdit *lineEditProbe1BB;
	QLineEdit *lineEditProbe1BL;
	QLineEdit *lineEditProbe3BB;
	QLineEdit *lineEditProbe4BT;
	QLineEdit *lineEditProbe10BB;
	QLineEdit *lineEditProbe10I2B;
	QLineEdit *lineEditProbe1I2T;
	QLineEdit *lineEditProbe5I2B;
	QLineEdit *lineEditProbe7I2T;
	QLineEdit *lineEditProbe4I2T;
	QLineEdit *lineEditProbe9I2T;
	QLineEdit *lineEditProbe3I2B;
	QLineEdit *lineEditProbe2I2T;
	QLineEdit *lineEditProbe2I2B;
	QLineEdit *lineEditProbe6I2B;
	QLineEdit *lineEditProbe5I2T;
	QLineEdit *lineEditProbe7I2B;
	QLineEdit *lineEditProbe3I2T;
	QLineEdit *lineEditProbe10I2L;
	QLineEdit *lineEditProbe9I2B;
	QLineEdit *lineEditProbe8I2B;
	QLineEdit *lineEditProbe5I2L;
	QLineEdit *lineEditProbe3I2L;
	QLineEdit *lineEditProbe8I2L;
	QLineEdit *lineEditProbe4I2B;
	QLineEdit *lineEditProbe7I2L;
	QLineEdit *lineEditProbe6I2T;
	QLineEdit *lineEditProbe6I2L;
	QLineEdit *lineEditProbe1I2B;
	QLineEdit *lineEditProbe9I2L;
	QLineEdit *lineEditProbe8I2T;
	QLineEdit *lineEditProbe10I2T;
	QLineEdit *lineEditProbe2I2L;
	QLineEdit *lineEditProbe1I2L;
	QLineEdit *lineEditProbe4I2L;
	QCheckBox *checkBoxProbe1;
	QCheckBox *checkBoxProbe2;
	QCheckBox *checkBoxProbe3;
	QCheckBox *checkBoxProbe4;
	QCheckBox *checkBoxProbe5;
	QCheckBox *checkBoxProbe6;
	QCheckBox *checkBoxProbe7;
	QCheckBox *checkBoxProbe8;
	QCheckBox *checkBoxProbe9;
	QCheckBox *checkBoxProbe10;
	QCheckBox *checkBoxProbe20;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe11;
	QCheckBox *checkBoxProbe16;
	QCheckBox *checkBoxProbe18;
	QCheckBox *checkBoxProbe15;
	QCheckBox *checkBoxProbe17;
	QCheckBox *checkBoxProbe19;
	QCheckBox *checkBoxProbe12;
	QCheckBox *checkBoxProbe13;
	QCheckBox *checkBoxProbe23;
	QCheckBox *checkBoxProbe26;
	QCheckBox *checkBoxProbe25;
	QCheckBox *checkBoxProbe27;
	QCheckBox *checkBoxProbe30;
	QCheckBox *checkBoxProbe24;
	QCheckBox *checkBoxProbe28;
	QCheckBox *checkBoxProbe29;
	QCheckBox *checkBoxProbe21;
	QCheckBox *checkBoxProbe22;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QCheckBox *checkBoxProbe1_12;
	QCheckBox *checkBoxProbe13_24;
	QCheckBox *checkBoxProbe25_36;
	QPushButton *pushButtonAllI;
	QPushButton *pushButtonAnyI;
	QPushButton *pushButtonAnyA;
	QPushButton *pushButtonAllA;
	QPushButton *pushButtonAllB;
	QPushButton *pushButtonAnyB;
	QPushButton *pushButtonAnyI2;
	QPushButton *pushButtonAllI2;
	QCheckBox *checkBoxProbe34;
	QCheckBox *checkBoxProbe33;
	QCheckBox *checkBoxProbe36;
	QCheckBox *checkBoxProbe35;
	QCheckBox *checkBoxProbe32;
	QCheckBox *checkBoxProbe31;
	QLineEdit *lineEditProbe12AL;
	QLineEdit *lineEditProbe12BL;
	QLineEdit *lineEditProbe11IB;
	QLineEdit *lineEditProbe11IL;
	QLineEdit *lineEditProbe11IT;
	QLineEdit *lineEditProbe11AB;
	QLineEdit *lineEditProbe11BT;
	QLineEdit *lineEditProbe12AT;
	QLineEdit *lineEditProbe12I2T;
	QLineEdit *lineEditProbe12IL;
	QLineEdit *lineEditProbe12I2L;
	QLineEdit *lineEditProbe12I2B;
	QLineEdit *lineEditProbe12IT;
	QLineEdit *lineEditProbe11I2T;
	QLineEdit *lineEditProbe11AT;
	QLineEdit *lineEditProbe11BL;
	QLineEdit *lineEditProbe12IB;
	QLineEdit *lineEditProbe11I2L;
	QLineEdit *lineEditProbe11BB;
	QLineEdit *lineEditProbe11I2B;
	QLineEdit *lineEditProbe12BT;
	QLineEdit *lineEditProbe12BB;
	QLineEdit *lineEditProbe11AL;
	QLineEdit *lineEditProbe12AB;

	void setupUi(QDialog *GateSetClass)
	{
		if (GateSetClass->objectName().isEmpty())
			GateSetClass->setObjectName(QString::fromUtf8("GateSetClass"));
		GateSetClass->resize(1000, 740);
		GateSetClass->setMinimumSize(QSize(1000, 740));
		GateSetClass->setMaximumSize(QSize(1000, 740));
		GateSetClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label_2 = new QLabel(GateSetClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(430, 20, 130, 20));
		label_2->setMinimumSize(QSize(130, 20));
		label_2->setMaximumSize(QSize(130, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 16pt \"\345\256\213\344\275\223\";"));
		lineEditAllIB = new QLineEdit(GateSetClass);
		lineEditAllIB->setObjectName(QString::fromUtf8("lineEditAllIB"));
		lineEditAllIB->setGeometry(QRect(170, 130, 40, 20));
		lineEditAllIB->setMinimumSize(QSize(40, 20));
		lineEditAllIB->setMaximumSize(QSize(40, 20));
		lineEditAllIB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		radioButtonAll = new QRadioButton(GateSetClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(80, 130, 81, 16));
		radioButtonAll->setMinimumSize(QSize(81, 16));
		radioButtonAll->setMaximumSize(QSize(81, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label = new QLabel(GateSetClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(178, 95, 30, 12));
		label->setMinimumSize(QSize(30, 12));
		label->setMaximumSize(QSize(30, 12));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllIL = new QLineEdit(GateSetClass);
		lineEditAllIL->setObjectName(QString::fromUtf8("lineEditAllIL"));
		lineEditAllIL->setGeometry(QRect(220, 130, 40, 20));
		lineEditAllIL->setMinimumSize(QSize(40, 20));
		lineEditAllIL->setMaximumSize(QSize(40, 20));
		lineEditAllIL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAllIT = new QLineEdit(GateSetClass);
		lineEditAllIT->setObjectName(QString::fromUtf8("lineEditAllIT"));
		lineEditAllIT->setGeometry(QRect(270, 130, 40, 20));
		lineEditAllIT->setMinimumSize(QSize(40, 20));
		lineEditAllIT->setMaximumSize(QSize(40, 20));
		lineEditAllIT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		radioButtonAny = new QRadioButton(GateSetClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(80, 169, 81, 16));
		radioButtonAny->setMinimumSize(QSize(81, 16));
		radioButtonAny->setMaximumSize(QSize(81, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_5 = new QLabel(GateSetClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(178, 109, 30, 12));
		label_5->setMinimumSize(QSize(30, 12));
		label_5->setMaximumSize(QSize(30, 12));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_6 = new QLabel(GateSetClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(227, 110, 30, 12));
		label_6->setMinimumSize(QSize(30, 12));
		label_6->setMaximumSize(QSize(30, 12));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_3 = new QLabel(GateSetClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(227, 96, 30, 12));
		label_3->setMinimumSize(QSize(30, 12));
		label_3->setMaximumSize(QSize(30, 12));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(GateSetClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(277, 96, 30, 12));
		label_4->setMinimumSize(QSize(30, 12));
		label_4->setMaximumSize(QSize(30, 12));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(GateSetClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(277, 110, 30, 12));
		label_7->setMinimumSize(QSize(30, 12));
		label_7->setMaximumSize(QSize(30, 12));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAnyIT = new QLineEdit(GateSetClass);
		lineEditAnyIT->setObjectName(QString::fromUtf8("lineEditAnyIT"));
		lineEditAnyIT->setGeometry(QRect(270, 169, 40, 20));
		lineEditAnyIT->setMinimumSize(QSize(40, 20));
		lineEditAnyIT->setMaximumSize(QSize(40, 20));
		lineEditAnyIT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyIL = new QLineEdit(GateSetClass);
		lineEditAnyIL->setObjectName(QString::fromUtf8("lineEditAnyIL"));
		lineEditAnyIL->setGeometry(QRect(220, 169, 40, 20));
		lineEditAnyIL->setMinimumSize(QSize(40, 20));
		lineEditAnyIL->setMaximumSize(QSize(40, 20));
		lineEditAnyIL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyIB = new QLineEdit(GateSetClass);
		lineEditAnyIB->setObjectName(QString::fromUtf8("lineEditAnyIB"));
		lineEditAnyIB->setGeometry(QRect(170, 169, 40, 20));
		lineEditAnyIB->setMinimumSize(QSize(40, 20));
		lineEditAnyIB->setMaximumSize(QSize(40, 20));
		lineEditAnyIB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_8 = new QLabel(GateSetClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(220, 60, 50, 20));
		label_8->setMinimumSize(QSize(50, 20));
		label_8->setMaximumSize(QSize(50, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(GateSetClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(430, 60, 50, 20));
		label_9->setMinimumSize(QSize(50, 20));
		label_9->setMaximumSize(QSize(50, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(GateSetClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(437, 96, 30, 12));
		label_10->setMinimumSize(QSize(30, 12));
		label_10->setMaximumSize(QSize(30, 12));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAnyAL = new QLineEdit(GateSetClass);
		lineEditAnyAL->setObjectName(QString::fromUtf8("lineEditAnyAL"));
		lineEditAnyAL->setGeometry(QRect(430, 169, 40, 20));
		lineEditAnyAL->setMinimumSize(QSize(40, 20));
		lineEditAnyAL->setMaximumSize(QSize(40, 20));
		lineEditAnyAL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAllAT = new QLineEdit(GateSetClass);
		lineEditAllAT->setObjectName(QString::fromUtf8("lineEditAllAT"));
		lineEditAllAT->setGeometry(QRect(480, 130, 40, 20));
		lineEditAllAT->setMinimumSize(QSize(40, 20));
		lineEditAllAT->setMaximumSize(QSize(40, 20));
		lineEditAllAT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_11 = new QLabel(GateSetClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(487, 110, 30, 12));
		label_11->setMinimumSize(QSize(30, 12));
		label_11->setMaximumSize(QSize(30, 12));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAnyAT = new QLineEdit(GateSetClass);
		lineEditAnyAT->setObjectName(QString::fromUtf8("lineEditAnyAT"));
		lineEditAnyAT->setGeometry(QRect(480, 169, 40, 20));
		lineEditAnyAT->setMinimumSize(QSize(40, 20));
		lineEditAnyAT->setMaximumSize(QSize(40, 20));
		lineEditAnyAT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyAB = new QLineEdit(GateSetClass);
		lineEditAnyAB->setObjectName(QString::fromUtf8("lineEditAnyAB"));
		lineEditAnyAB->setGeometry(QRect(380, 169, 40, 20));
		lineEditAnyAB->setMinimumSize(QSize(40, 20));
		lineEditAnyAB->setMaximumSize(QSize(40, 20));
		lineEditAnyAB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_12 = new QLabel(GateSetClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(388, 109, 30, 12));
		label_12->setMinimumSize(QSize(30, 12));
		label_12->setMaximumSize(QSize(30, 12));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllAL = new QLineEdit(GateSetClass);
		lineEditAllAL->setObjectName(QString::fromUtf8("lineEditAllAL"));
		lineEditAllAL->setGeometry(QRect(430, 130, 40, 20));
		lineEditAllAL->setMinimumSize(QSize(40, 20));
		lineEditAllAL->setMaximumSize(QSize(40, 20));
		lineEditAllAL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_13 = new QLabel(GateSetClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(487, 96, 30, 12));
		label_13->setMinimumSize(QSize(30, 12));
		label_13->setMaximumSize(QSize(30, 12));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllAB = new QLineEdit(GateSetClass);
		lineEditAllAB->setObjectName(QString::fromUtf8("lineEditAllAB"));
		lineEditAllAB->setGeometry(QRect(380, 130, 40, 20));
		lineEditAllAB->setMinimumSize(QSize(40, 20));
		lineEditAllAB->setMaximumSize(QSize(40, 20));
		lineEditAllAB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_14 = new QLabel(GateSetClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(437, 110, 30, 12));
		label_14->setMinimumSize(QSize(30, 12));
		label_14->setMaximumSize(QSize(30, 12));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_15 = new QLabel(GateSetClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(388, 95, 30, 12));
		label_15->setMinimumSize(QSize(30, 12));
		label_15->setMaximumSize(QSize(30, 12));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_16 = new QLabel(GateSetClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(598, 95, 30, 12));
		label_16->setMinimumSize(QSize(30, 12));
		label_16->setMaximumSize(QSize(30, 12));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllBT = new QLineEdit(GateSetClass);
		lineEditAllBT->setObjectName(QString::fromUtf8("lineEditAllBT"));
		lineEditAllBT->setGeometry(QRect(690, 130, 40, 20));
		lineEditAllBT->setMinimumSize(QSize(40, 20));
		lineEditAllBT->setMaximumSize(QSize(40, 20));
		lineEditAllBT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyBT = new QLineEdit(GateSetClass);
		lineEditAnyBT->setObjectName(QString::fromUtf8("lineEditAnyBT"));
		lineEditAnyBT->setGeometry(QRect(690, 169, 40, 20));
		lineEditAnyBT->setMinimumSize(QSize(40, 20));
		lineEditAnyBT->setMaximumSize(QSize(40, 20));
		lineEditAnyBT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_17 = new QLabel(GateSetClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(647, 96, 30, 12));
		label_17->setMinimumSize(QSize(30, 12));
		label_17->setMaximumSize(QSize(30, 12));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAnyBB = new QLineEdit(GateSetClass);
		lineEditAnyBB->setObjectName(QString::fromUtf8("lineEditAnyBB"));
		lineEditAnyBB->setGeometry(QRect(590, 169, 40, 20));
		lineEditAnyBB->setMinimumSize(QSize(40, 20));
		lineEditAnyBB->setMaximumSize(QSize(40, 20));
		lineEditAnyBB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_18 = new QLabel(GateSetClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(697, 96, 30, 12));
		label_18->setMinimumSize(QSize(30, 12));
		label_18->setMaximumSize(QSize(30, 12));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_19 = new QLabel(GateSetClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(697, 110, 30, 12));
		label_19->setMinimumSize(QSize(30, 12));
		label_19->setMaximumSize(QSize(30, 12));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllBB = new QLineEdit(GateSetClass);
		lineEditAllBB->setObjectName(QString::fromUtf8("lineEditAllBB"));
		lineEditAllBB->setGeometry(QRect(590, 130, 40, 20));
		lineEditAllBB->setMinimumSize(QSize(40, 20));
		lineEditAllBB->setMaximumSize(QSize(40, 20));
		lineEditAllBB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_20 = new QLabel(GateSetClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(640, 60, 50, 20));
		label_20->setMinimumSize(QSize(50, 20));
		label_20->setMaximumSize(QSize(50, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditAnyBL = new QLineEdit(GateSetClass);
		lineEditAnyBL->setObjectName(QString::fromUtf8("lineEditAnyBL"));
		lineEditAnyBL->setGeometry(QRect(640, 169, 40, 20));
		lineEditAnyBL->setMinimumSize(QSize(40, 20));
		lineEditAnyBL->setMaximumSize(QSize(40, 20));
		lineEditAnyBL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_21 = new QLabel(GateSetClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(647, 110, 30, 12));
		label_21->setMinimumSize(QSize(30, 12));
		label_21->setMaximumSize(QSize(30, 12));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_22 = new QLabel(GateSetClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(598, 109, 30, 12));
		label_22->setMinimumSize(QSize(30, 12));
		label_22->setMaximumSize(QSize(30, 12));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllBL = new QLineEdit(GateSetClass);
		lineEditAllBL->setObjectName(QString::fromUtf8("lineEditAllBL"));
		lineEditAllBL->setGeometry(QRect(640, 130, 40, 20));
		lineEditAllBL->setMinimumSize(QSize(40, 20));
		lineEditAllBL->setMaximumSize(QSize(40, 20));
		lineEditAllBL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAllI2L = new QLineEdit(GateSetClass);
		lineEditAllI2L->setObjectName(QString::fromUtf8("lineEditAllI2L"));
		lineEditAllI2L->setGeometry(QRect(850, 131, 40, 20));
		lineEditAllI2L->setMinimumSize(QSize(40, 20));
		lineEditAllI2L->setMaximumSize(QSize(40, 20));
		lineEditAllI2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyI2T = new QLineEdit(GateSetClass);
		lineEditAnyI2T->setObjectName(QString::fromUtf8("lineEditAnyI2T"));
		lineEditAnyI2T->setGeometry(QRect(900, 170, 40, 20));
		lineEditAnyI2T->setMinimumSize(QSize(40, 20));
		lineEditAnyI2T->setMaximumSize(QSize(40, 20));
		lineEditAnyI2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_23 = new QLabel(GateSetClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(850, 61, 50, 20));
		label_23->setMinimumSize(QSize(50, 20));
		label_23->setMaximumSize(QSize(50, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_24 = new QLabel(GateSetClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(907, 111, 30, 12));
		label_24->setMinimumSize(QSize(30, 12));
		label_24->setMaximumSize(QSize(30, 12));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAnyI2L = new QLineEdit(GateSetClass);
		lineEditAnyI2L->setObjectName(QString::fromUtf8("lineEditAnyI2L"));
		lineEditAnyI2L->setGeometry(QRect(850, 170, 40, 20));
		lineEditAnyI2L->setMinimumSize(QSize(40, 20));
		lineEditAnyI2L->setMaximumSize(QSize(40, 20));
		lineEditAnyI2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyI2B = new QLineEdit(GateSetClass);
		lineEditAnyI2B->setObjectName(QString::fromUtf8("lineEditAnyI2B"));
		lineEditAnyI2B->setGeometry(QRect(800, 170, 40, 20));
		lineEditAnyI2B->setMinimumSize(QSize(40, 20));
		lineEditAnyI2B->setMaximumSize(QSize(40, 20));
		lineEditAnyI2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_25 = new QLabel(GateSetClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(857, 111, 30, 12));
		label_25->setMinimumSize(QSize(30, 12));
		label_25->setMaximumSize(QSize(30, 12));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllI2B = new QLineEdit(GateSetClass);
		lineEditAllI2B->setObjectName(QString::fromUtf8("lineEditAllI2B"));
		lineEditAllI2B->setGeometry(QRect(800, 131, 40, 20));
		lineEditAllI2B->setMinimumSize(QSize(40, 20));
		lineEditAllI2B->setMaximumSize(QSize(40, 20));
		lineEditAllI2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_26 = new QLabel(GateSetClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(808, 96, 30, 12));
		label_26->setMinimumSize(QSize(30, 12));
		label_26->setMaximumSize(QSize(30, 12));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_27 = new QLabel(GateSetClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(907, 97, 30, 12));
		label_27->setMinimumSize(QSize(30, 12));
		label_27->setMaximumSize(QSize(30, 12));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditAllI2T = new QLineEdit(GateSetClass);
		lineEditAllI2T->setObjectName(QString::fromUtf8("lineEditAllI2T"));
		lineEditAllI2T->setGeometry(QRect(900, 131, 40, 20));
		lineEditAllI2T->setMinimumSize(QSize(40, 20));
		lineEditAllI2T->setMaximumSize(QSize(40, 20));
		lineEditAllI2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_28 = new QLabel(GateSetClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(857, 97, 30, 12));
		label_28->setMinimumSize(QSize(30, 12));
		label_28->setMaximumSize(QSize(30, 12));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(GateSetClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(808, 110, 30, 12));
		label_29->setMinimumSize(QSize(30, 12));
		label_29->setMaximumSize(QSize(30, 12));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 9pt \"\345\256\213\344\275\223\";"));
		lineEditProbe2IB = new QLineEdit(GateSetClass);
		lineEditProbe2IB->setObjectName(QString::fromUtf8("lineEditProbe2IB"));
		lineEditProbe2IB->setEnabled(false);
		lineEditProbe2IB->setGeometry(QRect(170, 260, 40, 20));
		lineEditProbe2IB->setMinimumSize(QSize(40, 20));
		lineEditProbe2IB->setMaximumSize(QSize(40, 20));
		lineEditProbe2IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2IL = new QLineEdit(GateSetClass);
		lineEditProbe2IL->setObjectName(QString::fromUtf8("lineEditProbe2IL"));
		lineEditProbe2IL->setEnabled(false);
		lineEditProbe2IL->setGeometry(QRect(220, 260, 40, 20));
		lineEditProbe2IL->setMinimumSize(QSize(40, 20));
		lineEditProbe2IL->setMaximumSize(QSize(40, 20));
		lineEditProbe2IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1IT = new QLineEdit(GateSetClass);
		lineEditProbe1IT->setObjectName(QString::fromUtf8("lineEditProbe1IT"));
		lineEditProbe1IT->setEnabled(false);
		lineEditProbe1IT->setGeometry(QRect(270, 220, 40, 20));
		lineEditProbe1IT->setMinimumSize(QSize(40, 20));
		lineEditProbe1IT->setMaximumSize(QSize(40, 20));
		lineEditProbe1IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2IT = new QLineEdit(GateSetClass);
		lineEditProbe2IT->setObjectName(QString::fromUtf8("lineEditProbe2IT"));
		lineEditProbe2IT->setEnabled(false);
		lineEditProbe2IT->setGeometry(QRect(270, 260, 40, 20));
		lineEditProbe2IT->setMinimumSize(QSize(40, 20));
		lineEditProbe2IT->setMaximumSize(QSize(40, 20));
		lineEditProbe2IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1IL = new QLineEdit(GateSetClass);
		lineEditProbe1IL->setObjectName(QString::fromUtf8("lineEditProbe1IL"));
		lineEditProbe1IL->setEnabled(false);
		lineEditProbe1IL->setGeometry(QRect(220, 220, 40, 20));
		lineEditProbe1IL->setMinimumSize(QSize(40, 20));
		lineEditProbe1IL->setMaximumSize(QSize(40, 20));
		lineEditProbe1IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1IB = new QLineEdit(GateSetClass);
		lineEditProbe1IB->setObjectName(QString::fromUtf8("lineEditProbe1IB"));
		lineEditProbe1IB->setEnabled(false);
		lineEditProbe1IB->setGeometry(QRect(170, 220, 40, 20));
		lineEditProbe1IB->setMinimumSize(QSize(40, 20));
		lineEditProbe1IB->setMaximumSize(QSize(40, 20));
		lineEditProbe1IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3IB = new QLineEdit(GateSetClass);
		lineEditProbe3IB->setObjectName(QString::fromUtf8("lineEditProbe3IB"));
		lineEditProbe3IB->setEnabled(false);
		lineEditProbe3IB->setGeometry(QRect(170, 300, 40, 20));
		lineEditProbe3IB->setMinimumSize(QSize(40, 20));
		lineEditProbe3IB->setMaximumSize(QSize(40, 20));
		lineEditProbe3IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3IT = new QLineEdit(GateSetClass);
		lineEditProbe3IT->setObjectName(QString::fromUtf8("lineEditProbe3IT"));
		lineEditProbe3IT->setEnabled(false);
		lineEditProbe3IT->setGeometry(QRect(270, 300, 40, 20));
		lineEditProbe3IT->setMinimumSize(QSize(40, 20));
		lineEditProbe3IT->setMaximumSize(QSize(40, 20));
		lineEditProbe3IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4IT = new QLineEdit(GateSetClass);
		lineEditProbe4IT->setObjectName(QString::fromUtf8("lineEditProbe4IT"));
		lineEditProbe4IT->setEnabled(false);
		lineEditProbe4IT->setGeometry(QRect(270, 340, 40, 20));
		lineEditProbe4IT->setMinimumSize(QSize(40, 20));
		lineEditProbe4IT->setMaximumSize(QSize(40, 20));
		lineEditProbe4IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3IL = new QLineEdit(GateSetClass);
		lineEditProbe3IL->setObjectName(QString::fromUtf8("lineEditProbe3IL"));
		lineEditProbe3IL->setEnabled(false);
		lineEditProbe3IL->setGeometry(QRect(220, 300, 40, 20));
		lineEditProbe3IL->setMinimumSize(QSize(40, 20));
		lineEditProbe3IL->setMaximumSize(QSize(40, 20));
		lineEditProbe3IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4IB = new QLineEdit(GateSetClass);
		lineEditProbe4IB->setObjectName(QString::fromUtf8("lineEditProbe4IB"));
		lineEditProbe4IB->setEnabled(false);
		lineEditProbe4IB->setGeometry(QRect(170, 340, 40, 20));
		lineEditProbe4IB->setMinimumSize(QSize(40, 20));
		lineEditProbe4IB->setMaximumSize(QSize(40, 20));
		lineEditProbe4IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4IL = new QLineEdit(GateSetClass);
		lineEditProbe4IL->setObjectName(QString::fromUtf8("lineEditProbe4IL"));
		lineEditProbe4IL->setEnabled(false);
		lineEditProbe4IL->setGeometry(QRect(220, 340, 40, 20));
		lineEditProbe4IL->setMinimumSize(QSize(40, 20));
		lineEditProbe4IL->setMaximumSize(QSize(40, 20));
		lineEditProbe4IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6IL = new QLineEdit(GateSetClass);
		lineEditProbe6IL->setObjectName(QString::fromUtf8("lineEditProbe6IL"));
		lineEditProbe6IL->setEnabled(false);
		lineEditProbe6IL->setGeometry(QRect(220, 420, 40, 20));
		lineEditProbe6IL->setMinimumSize(QSize(40, 20));
		lineEditProbe6IL->setMaximumSize(QSize(40, 20));
		lineEditProbe6IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6IB = new QLineEdit(GateSetClass);
		lineEditProbe6IB->setObjectName(QString::fromUtf8("lineEditProbe6IB"));
		lineEditProbe6IB->setEnabled(false);
		lineEditProbe6IB->setGeometry(QRect(170, 420, 40, 20));
		lineEditProbe6IB->setMinimumSize(QSize(40, 20));
		lineEditProbe6IB->setMaximumSize(QSize(40, 20));
		lineEditProbe6IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5IL = new QLineEdit(GateSetClass);
		lineEditProbe5IL->setObjectName(QString::fromUtf8("lineEditProbe5IL"));
		lineEditProbe5IL->setEnabled(false);
		lineEditProbe5IL->setGeometry(QRect(220, 380, 40, 20));
		lineEditProbe5IL->setMinimumSize(QSize(40, 20));
		lineEditProbe5IL->setMaximumSize(QSize(40, 20));
		lineEditProbe5IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5IT = new QLineEdit(GateSetClass);
		lineEditProbe5IT->setObjectName(QString::fromUtf8("lineEditProbe5IT"));
		lineEditProbe5IT->setEnabled(false);
		lineEditProbe5IT->setGeometry(QRect(270, 380, 40, 20));
		lineEditProbe5IT->setMinimumSize(QSize(40, 20));
		lineEditProbe5IT->setMaximumSize(QSize(40, 20));
		lineEditProbe5IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6IT = new QLineEdit(GateSetClass);
		lineEditProbe6IT->setObjectName(QString::fromUtf8("lineEditProbe6IT"));
		lineEditProbe6IT->setEnabled(false);
		lineEditProbe6IT->setGeometry(QRect(270, 420, 40, 20));
		lineEditProbe6IT->setMinimumSize(QSize(40, 20));
		lineEditProbe6IT->setMaximumSize(QSize(40, 20));
		lineEditProbe6IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5IB = new QLineEdit(GateSetClass);
		lineEditProbe5IB->setObjectName(QString::fromUtf8("lineEditProbe5IB"));
		lineEditProbe5IB->setEnabled(false);
		lineEditProbe5IB->setGeometry(QRect(170, 380, 40, 20));
		lineEditProbe5IB->setMinimumSize(QSize(40, 20));
		lineEditProbe5IB->setMaximumSize(QSize(40, 20));
		lineEditProbe5IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7IB = new QLineEdit(GateSetClass);
		lineEditProbe7IB->setObjectName(QString::fromUtf8("lineEditProbe7IB"));
		lineEditProbe7IB->setEnabled(false);
		lineEditProbe7IB->setGeometry(QRect(170, 460, 40, 20));
		lineEditProbe7IB->setMinimumSize(QSize(40, 20));
		lineEditProbe7IB->setMaximumSize(QSize(40, 20));
		lineEditProbe7IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8IL = new QLineEdit(GateSetClass);
		lineEditProbe8IL->setObjectName(QString::fromUtf8("lineEditProbe8IL"));
		lineEditProbe8IL->setEnabled(false);
		lineEditProbe8IL->setGeometry(QRect(220, 500, 40, 20));
		lineEditProbe8IL->setMinimumSize(QSize(40, 20));
		lineEditProbe8IL->setMaximumSize(QSize(40, 20));
		lineEditProbe8IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7IT = new QLineEdit(GateSetClass);
		lineEditProbe7IT->setObjectName(QString::fromUtf8("lineEditProbe7IT"));
		lineEditProbe7IT->setEnabled(false);
		lineEditProbe7IT->setGeometry(QRect(270, 460, 40, 20));
		lineEditProbe7IT->setMinimumSize(QSize(40, 20));
		lineEditProbe7IT->setMaximumSize(QSize(40, 20));
		lineEditProbe7IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8IT = new QLineEdit(GateSetClass);
		lineEditProbe8IT->setObjectName(QString::fromUtf8("lineEditProbe8IT"));
		lineEditProbe8IT->setEnabled(false);
		lineEditProbe8IT->setGeometry(QRect(270, 500, 40, 20));
		lineEditProbe8IT->setMinimumSize(QSize(40, 20));
		lineEditProbe8IT->setMaximumSize(QSize(40, 20));
		lineEditProbe8IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8IB = new QLineEdit(GateSetClass);
		lineEditProbe8IB->setObjectName(QString::fromUtf8("lineEditProbe8IB"));
		lineEditProbe8IB->setEnabled(false);
		lineEditProbe8IB->setGeometry(QRect(170, 500, 40, 20));
		lineEditProbe8IB->setMinimumSize(QSize(40, 20));
		lineEditProbe8IB->setMaximumSize(QSize(40, 20));
		lineEditProbe8IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7IL = new QLineEdit(GateSetClass);
		lineEditProbe7IL->setObjectName(QString::fromUtf8("lineEditProbe7IL"));
		lineEditProbe7IL->setEnabled(false);
		lineEditProbe7IL->setGeometry(QRect(220, 460, 40, 20));
		lineEditProbe7IL->setMinimumSize(QSize(40, 20));
		lineEditProbe7IL->setMaximumSize(QSize(40, 20));
		lineEditProbe7IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9IL = new QLineEdit(GateSetClass);
		lineEditProbe9IL->setObjectName(QString::fromUtf8("lineEditProbe9IL"));
		lineEditProbe9IL->setEnabled(false);
		lineEditProbe9IL->setGeometry(QRect(220, 540, 40, 20));
		lineEditProbe9IL->setMinimumSize(QSize(40, 20));
		lineEditProbe9IL->setMaximumSize(QSize(40, 20));
		lineEditProbe9IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10IB = new QLineEdit(GateSetClass);
		lineEditProbe10IB->setObjectName(QString::fromUtf8("lineEditProbe10IB"));
		lineEditProbe10IB->setEnabled(false);
		lineEditProbe10IB->setGeometry(QRect(170, 580, 40, 20));
		lineEditProbe10IB->setMinimumSize(QSize(40, 20));
		lineEditProbe10IB->setMaximumSize(QSize(40, 20));
		lineEditProbe10IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10IL = new QLineEdit(GateSetClass);
		lineEditProbe10IL->setObjectName(QString::fromUtf8("lineEditProbe10IL"));
		lineEditProbe10IL->setEnabled(false);
		lineEditProbe10IL->setGeometry(QRect(220, 580, 40, 20));
		lineEditProbe10IL->setMinimumSize(QSize(40, 20));
		lineEditProbe10IL->setMaximumSize(QSize(40, 20));
		lineEditProbe10IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9IT = new QLineEdit(GateSetClass);
		lineEditProbe9IT->setObjectName(QString::fromUtf8("lineEditProbe9IT"));
		lineEditProbe9IT->setEnabled(false);
		lineEditProbe9IT->setGeometry(QRect(270, 540, 40, 20));
		lineEditProbe9IT->setMinimumSize(QSize(40, 20));
		lineEditProbe9IT->setMaximumSize(QSize(40, 20));
		lineEditProbe9IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9IB = new QLineEdit(GateSetClass);
		lineEditProbe9IB->setObjectName(QString::fromUtf8("lineEditProbe9IB"));
		lineEditProbe9IB->setEnabled(false);
		lineEditProbe9IB->setGeometry(QRect(170, 540, 40, 20));
		lineEditProbe9IB->setMinimumSize(QSize(40, 20));
		lineEditProbe9IB->setMaximumSize(QSize(40, 20));
		lineEditProbe9IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10IT = new QLineEdit(GateSetClass);
		lineEditProbe10IT->setObjectName(QString::fromUtf8("lineEditProbe10IT"));
		lineEditProbe10IT->setEnabled(false);
		lineEditProbe10IT->setGeometry(QRect(270, 580, 40, 20));
		lineEditProbe10IT->setMinimumSize(QSize(40, 20));
		lineEditProbe10IT->setMaximumSize(QSize(40, 20));
		lineEditProbe10IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10AT = new QLineEdit(GateSetClass);
		lineEditProbe10AT->setObjectName(QString::fromUtf8("lineEditProbe10AT"));
		lineEditProbe10AT->setEnabled(false);
		lineEditProbe10AT->setGeometry(QRect(480, 580, 40, 20));
		lineEditProbe10AT->setMinimumSize(QSize(40, 20));
		lineEditProbe10AT->setMaximumSize(QSize(40, 20));
		lineEditProbe10AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1AB = new QLineEdit(GateSetClass);
		lineEditProbe1AB->setObjectName(QString::fromUtf8("lineEditProbe1AB"));
		lineEditProbe1AB->setEnabled(false);
		lineEditProbe1AB->setGeometry(QRect(380, 220, 40, 20));
		lineEditProbe1AB->setMinimumSize(QSize(40, 20));
		lineEditProbe1AB->setMaximumSize(QSize(40, 20));
		lineEditProbe1AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2AL = new QLineEdit(GateSetClass);
		lineEditProbe2AL->setObjectName(QString::fromUtf8("lineEditProbe2AL"));
		lineEditProbe2AL->setEnabled(false);
		lineEditProbe2AL->setGeometry(QRect(430, 260, 40, 20));
		lineEditProbe2AL->setMinimumSize(QSize(40, 20));
		lineEditProbe2AL->setMaximumSize(QSize(40, 20));
		lineEditProbe2AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4AL = new QLineEdit(GateSetClass);
		lineEditProbe4AL->setObjectName(QString::fromUtf8("lineEditProbe4AL"));
		lineEditProbe4AL->setEnabled(false);
		lineEditProbe4AL->setGeometry(QRect(430, 340, 40, 20));
		lineEditProbe4AL->setMinimumSize(QSize(40, 20));
		lineEditProbe4AL->setMaximumSize(QSize(40, 20));
		lineEditProbe4AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2AB = new QLineEdit(GateSetClass);
		lineEditProbe2AB->setObjectName(QString::fromUtf8("lineEditProbe2AB"));
		lineEditProbe2AB->setEnabled(false);
		lineEditProbe2AB->setGeometry(QRect(380, 260, 40, 20));
		lineEditProbe2AB->setMinimumSize(QSize(40, 20));
		lineEditProbe2AB->setMaximumSize(QSize(40, 20));
		lineEditProbe2AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1AL = new QLineEdit(GateSetClass);
		lineEditProbe1AL->setObjectName(QString::fromUtf8("lineEditProbe1AL"));
		lineEditProbe1AL->setEnabled(false);
		lineEditProbe1AL->setGeometry(QRect(430, 220, 40, 20));
		lineEditProbe1AL->setMinimumSize(QSize(40, 20));
		lineEditProbe1AL->setMaximumSize(QSize(40, 20));
		lineEditProbe1AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3AT = new QLineEdit(GateSetClass);
		lineEditProbe3AT->setObjectName(QString::fromUtf8("lineEditProbe3AT"));
		lineEditProbe3AT->setEnabled(false);
		lineEditProbe3AT->setGeometry(QRect(480, 300, 40, 20));
		lineEditProbe3AT->setMinimumSize(QSize(40, 20));
		lineEditProbe3AT->setMaximumSize(QSize(40, 20));
		lineEditProbe3AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9AL = new QLineEdit(GateSetClass);
		lineEditProbe9AL->setObjectName(QString::fromUtf8("lineEditProbe9AL"));
		lineEditProbe9AL->setEnabled(false);
		lineEditProbe9AL->setGeometry(QRect(430, 540, 40, 20));
		lineEditProbe9AL->setMinimumSize(QSize(40, 20));
		lineEditProbe9AL->setMaximumSize(QSize(40, 20));
		lineEditProbe9AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8AB = new QLineEdit(GateSetClass);
		lineEditProbe8AB->setObjectName(QString::fromUtf8("lineEditProbe8AB"));
		lineEditProbe8AB->setEnabled(false);
		lineEditProbe8AB->setGeometry(QRect(380, 500, 40, 20));
		lineEditProbe8AB->setMinimumSize(QSize(40, 20));
		lineEditProbe8AB->setMaximumSize(QSize(40, 20));
		lineEditProbe8AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3AL = new QLineEdit(GateSetClass);
		lineEditProbe3AL->setObjectName(QString::fromUtf8("lineEditProbe3AL"));
		lineEditProbe3AL->setEnabled(false);
		lineEditProbe3AL->setGeometry(QRect(430, 300, 40, 20));
		lineEditProbe3AL->setMinimumSize(QSize(40, 20));
		lineEditProbe3AL->setMaximumSize(QSize(40, 20));
		lineEditProbe3AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7AT = new QLineEdit(GateSetClass);
		lineEditProbe7AT->setObjectName(QString::fromUtf8("lineEditProbe7AT"));
		lineEditProbe7AT->setEnabled(false);
		lineEditProbe7AT->setGeometry(QRect(480, 460, 40, 20));
		lineEditProbe7AT->setMinimumSize(QSize(40, 20));
		lineEditProbe7AT->setMaximumSize(QSize(40, 20));
		lineEditProbe7AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5AB = new QLineEdit(GateSetClass);
		lineEditProbe5AB->setObjectName(QString::fromUtf8("lineEditProbe5AB"));
		lineEditProbe5AB->setEnabled(false);
		lineEditProbe5AB->setGeometry(QRect(380, 380, 40, 20));
		lineEditProbe5AB->setMinimumSize(QSize(40, 20));
		lineEditProbe5AB->setMaximumSize(QSize(40, 20));
		lineEditProbe5AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1AT = new QLineEdit(GateSetClass);
		lineEditProbe1AT->setObjectName(QString::fromUtf8("lineEditProbe1AT"));
		lineEditProbe1AT->setEnabled(false);
		lineEditProbe1AT->setGeometry(QRect(480, 220, 40, 20));
		lineEditProbe1AT->setMinimumSize(QSize(40, 20));
		lineEditProbe1AT->setMaximumSize(QSize(40, 20));
		lineEditProbe1AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6AL = new QLineEdit(GateSetClass);
		lineEditProbe6AL->setObjectName(QString::fromUtf8("lineEditProbe6AL"));
		lineEditProbe6AL->setEnabled(false);
		lineEditProbe6AL->setGeometry(QRect(430, 420, 40, 20));
		lineEditProbe6AL->setMinimumSize(QSize(40, 20));
		lineEditProbe6AL->setMaximumSize(QSize(40, 20));
		lineEditProbe6AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5AT = new QLineEdit(GateSetClass);
		lineEditProbe5AT->setObjectName(QString::fromUtf8("lineEditProbe5AT"));
		lineEditProbe5AT->setEnabled(false);
		lineEditProbe5AT->setGeometry(QRect(480, 380, 40, 20));
		lineEditProbe5AT->setMinimumSize(QSize(40, 20));
		lineEditProbe5AT->setMaximumSize(QSize(40, 20));
		lineEditProbe5AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4AB = new QLineEdit(GateSetClass);
		lineEditProbe4AB->setObjectName(QString::fromUtf8("lineEditProbe4AB"));
		lineEditProbe4AB->setEnabled(false);
		lineEditProbe4AB->setGeometry(QRect(380, 340, 40, 20));
		lineEditProbe4AB->setMinimumSize(QSize(40, 20));
		lineEditProbe4AB->setMaximumSize(QSize(40, 20));
		lineEditProbe4AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2AT = new QLineEdit(GateSetClass);
		lineEditProbe2AT->setObjectName(QString::fromUtf8("lineEditProbe2AT"));
		lineEditProbe2AT->setEnabled(false);
		lineEditProbe2AT->setGeometry(QRect(480, 260, 40, 20));
		lineEditProbe2AT->setMinimumSize(QSize(40, 20));
		lineEditProbe2AT->setMaximumSize(QSize(40, 20));
		lineEditProbe2AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6AT = new QLineEdit(GateSetClass);
		lineEditProbe6AT->setObjectName(QString::fromUtf8("lineEditProbe6AT"));
		lineEditProbe6AT->setEnabled(false);
		lineEditProbe6AT->setGeometry(QRect(480, 420, 40, 20));
		lineEditProbe6AT->setMinimumSize(QSize(40, 20));
		lineEditProbe6AT->setMaximumSize(QSize(40, 20));
		lineEditProbe6AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6AB = new QLineEdit(GateSetClass);
		lineEditProbe6AB->setObjectName(QString::fromUtf8("lineEditProbe6AB"));
		lineEditProbe6AB->setEnabled(false);
		lineEditProbe6AB->setGeometry(QRect(380, 420, 40, 20));
		lineEditProbe6AB->setMinimumSize(QSize(40, 20));
		lineEditProbe6AB->setMaximumSize(QSize(40, 20));
		lineEditProbe6AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8AT = new QLineEdit(GateSetClass);
		lineEditProbe8AT->setObjectName(QString::fromUtf8("lineEditProbe8AT"));
		lineEditProbe8AT->setEnabled(false);
		lineEditProbe8AT->setGeometry(QRect(480, 500, 40, 20));
		lineEditProbe8AT->setMinimumSize(QSize(40, 20));
		lineEditProbe8AT->setMaximumSize(QSize(40, 20));
		lineEditProbe8AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4AT = new QLineEdit(GateSetClass);
		lineEditProbe4AT->setObjectName(QString::fromUtf8("lineEditProbe4AT"));
		lineEditProbe4AT->setEnabled(false);
		lineEditProbe4AT->setGeometry(QRect(480, 340, 40, 20));
		lineEditProbe4AT->setMinimumSize(QSize(40, 20));
		lineEditProbe4AT->setMaximumSize(QSize(40, 20));
		lineEditProbe4AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10AL = new QLineEdit(GateSetClass);
		lineEditProbe10AL->setObjectName(QString::fromUtf8("lineEditProbe10AL"));
		lineEditProbe10AL->setEnabled(false);
		lineEditProbe10AL->setGeometry(QRect(430, 580, 40, 20));
		lineEditProbe10AL->setMinimumSize(QSize(40, 20));
		lineEditProbe10AL->setMaximumSize(QSize(40, 20));
		lineEditProbe10AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7AL = new QLineEdit(GateSetClass);
		lineEditProbe7AL->setObjectName(QString::fromUtf8("lineEditProbe7AL"));
		lineEditProbe7AL->setEnabled(false);
		lineEditProbe7AL->setGeometry(QRect(430, 460, 40, 20));
		lineEditProbe7AL->setMinimumSize(QSize(40, 20));
		lineEditProbe7AL->setMaximumSize(QSize(40, 20));
		lineEditProbe7AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7AB = new QLineEdit(GateSetClass);
		lineEditProbe7AB->setObjectName(QString::fromUtf8("lineEditProbe7AB"));
		lineEditProbe7AB->setEnabled(false);
		lineEditProbe7AB->setGeometry(QRect(380, 460, 40, 20));
		lineEditProbe7AB->setMinimumSize(QSize(40, 20));
		lineEditProbe7AB->setMaximumSize(QSize(40, 20));
		lineEditProbe7AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8AL = new QLineEdit(GateSetClass);
		lineEditProbe8AL->setObjectName(QString::fromUtf8("lineEditProbe8AL"));
		lineEditProbe8AL->setEnabled(false);
		lineEditProbe8AL->setGeometry(QRect(430, 500, 40, 20));
		lineEditProbe8AL->setMinimumSize(QSize(40, 20));
		lineEditProbe8AL->setMaximumSize(QSize(40, 20));
		lineEditProbe8AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5AL = new QLineEdit(GateSetClass);
		lineEditProbe5AL->setObjectName(QString::fromUtf8("lineEditProbe5AL"));
		lineEditProbe5AL->setEnabled(false);
		lineEditProbe5AL->setGeometry(QRect(430, 380, 40, 20));
		lineEditProbe5AL->setMinimumSize(QSize(40, 20));
		lineEditProbe5AL->setMaximumSize(QSize(40, 20));
		lineEditProbe5AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10AB = new QLineEdit(GateSetClass);
		lineEditProbe10AB->setObjectName(QString::fromUtf8("lineEditProbe10AB"));
		lineEditProbe10AB->setEnabled(false);
		lineEditProbe10AB->setGeometry(QRect(380, 580, 40, 20));
		lineEditProbe10AB->setMinimumSize(QSize(40, 20));
		lineEditProbe10AB->setMaximumSize(QSize(40, 20));
		lineEditProbe10AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9AT = new QLineEdit(GateSetClass);
		lineEditProbe9AT->setObjectName(QString::fromUtf8("lineEditProbe9AT"));
		lineEditProbe9AT->setEnabled(false);
		lineEditProbe9AT->setGeometry(QRect(480, 540, 40, 20));
		lineEditProbe9AT->setMinimumSize(QSize(40, 20));
		lineEditProbe9AT->setMaximumSize(QSize(40, 20));
		lineEditProbe9AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3AB = new QLineEdit(GateSetClass);
		lineEditProbe3AB->setObjectName(QString::fromUtf8("lineEditProbe3AB"));
		lineEditProbe3AB->setEnabled(false);
		lineEditProbe3AB->setGeometry(QRect(380, 300, 40, 20));
		lineEditProbe3AB->setMinimumSize(QSize(40, 20));
		lineEditProbe3AB->setMaximumSize(QSize(40, 20));
		lineEditProbe3AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9AB = new QLineEdit(GateSetClass);
		lineEditProbe9AB->setObjectName(QString::fromUtf8("lineEditProbe9AB"));
		lineEditProbe9AB->setEnabled(false);
		lineEditProbe9AB->setGeometry(QRect(380, 540, 40, 20));
		lineEditProbe9AB->setMinimumSize(QSize(40, 20));
		lineEditProbe9AB->setMaximumSize(QSize(40, 20));
		lineEditProbe9AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9BB = new QLineEdit(GateSetClass);
		lineEditProbe9BB->setObjectName(QString::fromUtf8("lineEditProbe9BB"));
		lineEditProbe9BB->setEnabled(false);
		lineEditProbe9BB->setGeometry(QRect(590, 540, 40, 20));
		lineEditProbe9BB->setMinimumSize(QSize(40, 20));
		lineEditProbe9BB->setMaximumSize(QSize(40, 20));
		lineEditProbe9BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4BB = new QLineEdit(GateSetClass);
		lineEditProbe4BB->setObjectName(QString::fromUtf8("lineEditProbe4BB"));
		lineEditProbe4BB->setEnabled(false);
		lineEditProbe4BB->setGeometry(QRect(590, 340, 40, 20));
		lineEditProbe4BB->setMinimumSize(QSize(40, 20));
		lineEditProbe4BB->setMaximumSize(QSize(40, 20));
		lineEditProbe4BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1BT = new QLineEdit(GateSetClass);
		lineEditProbe1BT->setObjectName(QString::fromUtf8("lineEditProbe1BT"));
		lineEditProbe1BT->setEnabled(false);
		lineEditProbe1BT->setGeometry(QRect(690, 220, 40, 20));
		lineEditProbe1BT->setMinimumSize(QSize(40, 20));
		lineEditProbe1BT->setMaximumSize(QSize(40, 20));
		lineEditProbe1BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9BL = new QLineEdit(GateSetClass);
		lineEditProbe9BL->setObjectName(QString::fromUtf8("lineEditProbe9BL"));
		lineEditProbe9BL->setEnabled(false);
		lineEditProbe9BL->setGeometry(QRect(640, 540, 40, 20));
		lineEditProbe9BL->setMinimumSize(QSize(40, 20));
		lineEditProbe9BL->setMaximumSize(QSize(40, 20));
		lineEditProbe9BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7BB = new QLineEdit(GateSetClass);
		lineEditProbe7BB->setObjectName(QString::fromUtf8("lineEditProbe7BB"));
		lineEditProbe7BB->setEnabled(false);
		lineEditProbe7BB->setGeometry(QRect(590, 460, 40, 20));
		lineEditProbe7BB->setMinimumSize(QSize(40, 20));
		lineEditProbe7BB->setMaximumSize(QSize(40, 20));
		lineEditProbe7BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7BT = new QLineEdit(GateSetClass);
		lineEditProbe7BT->setObjectName(QString::fromUtf8("lineEditProbe7BT"));
		lineEditProbe7BT->setEnabled(false);
		lineEditProbe7BT->setGeometry(QRect(690, 460, 40, 20));
		lineEditProbe7BT->setMinimumSize(QSize(40, 20));
		lineEditProbe7BT->setMaximumSize(QSize(40, 20));
		lineEditProbe7BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6BT = new QLineEdit(GateSetClass);
		lineEditProbe6BT->setObjectName(QString::fromUtf8("lineEditProbe6BT"));
		lineEditProbe6BT->setEnabled(false);
		lineEditProbe6BT->setGeometry(QRect(690, 420, 40, 20));
		lineEditProbe6BT->setMinimumSize(QSize(40, 20));
		lineEditProbe6BT->setMaximumSize(QSize(40, 20));
		lineEditProbe6BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2BB = new QLineEdit(GateSetClass);
		lineEditProbe2BB->setObjectName(QString::fromUtf8("lineEditProbe2BB"));
		lineEditProbe2BB->setEnabled(false);
		lineEditProbe2BB->setGeometry(QRect(590, 260, 40, 20));
		lineEditProbe2BB->setMinimumSize(QSize(40, 20));
		lineEditProbe2BB->setMaximumSize(QSize(40, 20));
		lineEditProbe2BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5BB = new QLineEdit(GateSetClass);
		lineEditProbe5BB->setObjectName(QString::fromUtf8("lineEditProbe5BB"));
		lineEditProbe5BB->setEnabled(false);
		lineEditProbe5BB->setGeometry(QRect(590, 380, 40, 20));
		lineEditProbe5BB->setMinimumSize(QSize(40, 20));
		lineEditProbe5BB->setMaximumSize(QSize(40, 20));
		lineEditProbe5BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3BT = new QLineEdit(GateSetClass);
		lineEditProbe3BT->setObjectName(QString::fromUtf8("lineEditProbe3BT"));
		lineEditProbe3BT->setEnabled(false);
		lineEditProbe3BT->setGeometry(QRect(690, 300, 40, 20));
		lineEditProbe3BT->setMinimumSize(QSize(40, 20));
		lineEditProbe3BT->setMaximumSize(QSize(40, 20));
		lineEditProbe3BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8BB = new QLineEdit(GateSetClass);
		lineEditProbe8BB->setObjectName(QString::fromUtf8("lineEditProbe8BB"));
		lineEditProbe8BB->setEnabled(false);
		lineEditProbe8BB->setGeometry(QRect(590, 500, 40, 20));
		lineEditProbe8BB->setMinimumSize(QSize(40, 20));
		lineEditProbe8BB->setMaximumSize(QSize(40, 20));
		lineEditProbe8BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6BB = new QLineEdit(GateSetClass);
		lineEditProbe6BB->setObjectName(QString::fromUtf8("lineEditProbe6BB"));
		lineEditProbe6BB->setEnabled(false);
		lineEditProbe6BB->setGeometry(QRect(590, 420, 40, 20));
		lineEditProbe6BB->setMinimumSize(QSize(40, 20));
		lineEditProbe6BB->setMaximumSize(QSize(40, 20));
		lineEditProbe6BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3BL = new QLineEdit(GateSetClass);
		lineEditProbe3BL->setObjectName(QString::fromUtf8("lineEditProbe3BL"));
		lineEditProbe3BL->setEnabled(false);
		lineEditProbe3BL->setGeometry(QRect(640, 300, 40, 20));
		lineEditProbe3BL->setMinimumSize(QSize(40, 20));
		lineEditProbe3BL->setMaximumSize(QSize(40, 20));
		lineEditProbe3BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10BL = new QLineEdit(GateSetClass);
		lineEditProbe10BL->setObjectName(QString::fromUtf8("lineEditProbe10BL"));
		lineEditProbe10BL->setEnabled(false);
		lineEditProbe10BL->setGeometry(QRect(640, 580, 40, 20));
		lineEditProbe10BL->setMinimumSize(QSize(40, 20));
		lineEditProbe10BL->setMaximumSize(QSize(40, 20));
		lineEditProbe10BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2BT = new QLineEdit(GateSetClass);
		lineEditProbe2BT->setObjectName(QString::fromUtf8("lineEditProbe2BT"));
		lineEditProbe2BT->setEnabled(false);
		lineEditProbe2BT->setGeometry(QRect(690, 260, 40, 20));
		lineEditProbe2BT->setMinimumSize(QSize(40, 20));
		lineEditProbe2BT->setMaximumSize(QSize(40, 20));
		lineEditProbe2BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4BL = new QLineEdit(GateSetClass);
		lineEditProbe4BL->setObjectName(QString::fromUtf8("lineEditProbe4BL"));
		lineEditProbe4BL->setEnabled(false);
		lineEditProbe4BL->setGeometry(QRect(640, 340, 40, 20));
		lineEditProbe4BL->setMinimumSize(QSize(40, 20));
		lineEditProbe4BL->setMaximumSize(QSize(40, 20));
		lineEditProbe4BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8BL = new QLineEdit(GateSetClass);
		lineEditProbe8BL->setObjectName(QString::fromUtf8("lineEditProbe8BL"));
		lineEditProbe8BL->setEnabled(false);
		lineEditProbe8BL->setGeometry(QRect(640, 500, 40, 20));
		lineEditProbe8BL->setMinimumSize(QSize(40, 20));
		lineEditProbe8BL->setMaximumSize(QSize(40, 20));
		lineEditProbe8BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9BT = new QLineEdit(GateSetClass);
		lineEditProbe9BT->setObjectName(QString::fromUtf8("lineEditProbe9BT"));
		lineEditProbe9BT->setEnabled(false);
		lineEditProbe9BT->setGeometry(QRect(690, 540, 40, 20));
		lineEditProbe9BT->setMinimumSize(QSize(40, 20));
		lineEditProbe9BT->setMaximumSize(QSize(40, 20));
		lineEditProbe9BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7BL = new QLineEdit(GateSetClass);
		lineEditProbe7BL->setObjectName(QString::fromUtf8("lineEditProbe7BL"));
		lineEditProbe7BL->setEnabled(false);
		lineEditProbe7BL->setGeometry(QRect(640, 460, 40, 20));
		lineEditProbe7BL->setMinimumSize(QSize(40, 20));
		lineEditProbe7BL->setMaximumSize(QSize(40, 20));
		lineEditProbe7BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8BT = new QLineEdit(GateSetClass);
		lineEditProbe8BT->setObjectName(QString::fromUtf8("lineEditProbe8BT"));
		lineEditProbe8BT->setEnabled(false);
		lineEditProbe8BT->setGeometry(QRect(690, 500, 40, 20));
		lineEditProbe8BT->setMinimumSize(QSize(40, 20));
		lineEditProbe8BT->setMaximumSize(QSize(40, 20));
		lineEditProbe8BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10BT = new QLineEdit(GateSetClass);
		lineEditProbe10BT->setObjectName(QString::fromUtf8("lineEditProbe10BT"));
		lineEditProbe10BT->setEnabled(false);
		lineEditProbe10BT->setGeometry(QRect(690, 580, 40, 20));
		lineEditProbe10BT->setMinimumSize(QSize(40, 20));
		lineEditProbe10BT->setMaximumSize(QSize(40, 20));
		lineEditProbe10BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5BL = new QLineEdit(GateSetClass);
		lineEditProbe5BL->setObjectName(QString::fromUtf8("lineEditProbe5BL"));
		lineEditProbe5BL->setEnabled(false);
		lineEditProbe5BL->setGeometry(QRect(640, 380, 40, 20));
		lineEditProbe5BL->setMinimumSize(QSize(40, 20));
		lineEditProbe5BL->setMaximumSize(QSize(40, 20));
		lineEditProbe5BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6BL = new QLineEdit(GateSetClass);
		lineEditProbe6BL->setObjectName(QString::fromUtf8("lineEditProbe6BL"));
		lineEditProbe6BL->setEnabled(false);
		lineEditProbe6BL->setGeometry(QRect(640, 420, 40, 20));
		lineEditProbe6BL->setMinimumSize(QSize(40, 20));
		lineEditProbe6BL->setMaximumSize(QSize(40, 20));
		lineEditProbe6BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2BL = new QLineEdit(GateSetClass);
		lineEditProbe2BL->setObjectName(QString::fromUtf8("lineEditProbe2BL"));
		lineEditProbe2BL->setEnabled(false);
		lineEditProbe2BL->setGeometry(QRect(640, 260, 40, 20));
		lineEditProbe2BL->setMinimumSize(QSize(40, 20));
		lineEditProbe2BL->setMaximumSize(QSize(40, 20));
		lineEditProbe2BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5BT = new QLineEdit(GateSetClass);
		lineEditProbe5BT->setObjectName(QString::fromUtf8("lineEditProbe5BT"));
		lineEditProbe5BT->setEnabled(false);
		lineEditProbe5BT->setGeometry(QRect(690, 380, 40, 20));
		lineEditProbe5BT->setMinimumSize(QSize(40, 20));
		lineEditProbe5BT->setMaximumSize(QSize(40, 20));
		lineEditProbe5BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1BB = new QLineEdit(GateSetClass);
		lineEditProbe1BB->setObjectName(QString::fromUtf8("lineEditProbe1BB"));
		lineEditProbe1BB->setEnabled(false);
		lineEditProbe1BB->setGeometry(QRect(590, 220, 40, 20));
		lineEditProbe1BB->setMinimumSize(QSize(40, 20));
		lineEditProbe1BB->setMaximumSize(QSize(40, 20));
		lineEditProbe1BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1BL = new QLineEdit(GateSetClass);
		lineEditProbe1BL->setObjectName(QString::fromUtf8("lineEditProbe1BL"));
		lineEditProbe1BL->setEnabled(false);
		lineEditProbe1BL->setGeometry(QRect(640, 220, 40, 20));
		lineEditProbe1BL->setMinimumSize(QSize(40, 20));
		lineEditProbe1BL->setMaximumSize(QSize(40, 20));
		lineEditProbe1BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3BB = new QLineEdit(GateSetClass);
		lineEditProbe3BB->setObjectName(QString::fromUtf8("lineEditProbe3BB"));
		lineEditProbe3BB->setEnabled(false);
		lineEditProbe3BB->setGeometry(QRect(590, 300, 40, 20));
		lineEditProbe3BB->setMinimumSize(QSize(40, 20));
		lineEditProbe3BB->setMaximumSize(QSize(40, 20));
		lineEditProbe3BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4BT = new QLineEdit(GateSetClass);
		lineEditProbe4BT->setObjectName(QString::fromUtf8("lineEditProbe4BT"));
		lineEditProbe4BT->setEnabled(false);
		lineEditProbe4BT->setGeometry(QRect(690, 340, 40, 20));
		lineEditProbe4BT->setMinimumSize(QSize(40, 20));
		lineEditProbe4BT->setMaximumSize(QSize(40, 20));
		lineEditProbe4BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10BB = new QLineEdit(GateSetClass);
		lineEditProbe10BB->setObjectName(QString::fromUtf8("lineEditProbe10BB"));
		lineEditProbe10BB->setEnabled(false);
		lineEditProbe10BB->setGeometry(QRect(590, 580, 40, 20));
		lineEditProbe10BB->setMinimumSize(QSize(40, 20));
		lineEditProbe10BB->setMaximumSize(QSize(40, 20));
		lineEditProbe10BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10I2B = new QLineEdit(GateSetClass);
		lineEditProbe10I2B->setObjectName(QString::fromUtf8("lineEditProbe10I2B"));
		lineEditProbe10I2B->setEnabled(false);
		lineEditProbe10I2B->setGeometry(QRect(800, 580, 40, 20));
		lineEditProbe10I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe10I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe10I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1I2T = new QLineEdit(GateSetClass);
		lineEditProbe1I2T->setObjectName(QString::fromUtf8("lineEditProbe1I2T"));
		lineEditProbe1I2T->setEnabled(false);
		lineEditProbe1I2T->setGeometry(QRect(900, 220, 40, 20));
		lineEditProbe1I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe1I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe1I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5I2B = new QLineEdit(GateSetClass);
		lineEditProbe5I2B->setObjectName(QString::fromUtf8("lineEditProbe5I2B"));
		lineEditProbe5I2B->setEnabled(false);
		lineEditProbe5I2B->setGeometry(QRect(800, 380, 40, 20));
		lineEditProbe5I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe5I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe5I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7I2T = new QLineEdit(GateSetClass);
		lineEditProbe7I2T->setObjectName(QString::fromUtf8("lineEditProbe7I2T"));
		lineEditProbe7I2T->setEnabled(false);
		lineEditProbe7I2T->setGeometry(QRect(900, 460, 40, 20));
		lineEditProbe7I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe7I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe7I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4I2T = new QLineEdit(GateSetClass);
		lineEditProbe4I2T->setObjectName(QString::fromUtf8("lineEditProbe4I2T"));
		lineEditProbe4I2T->setEnabled(false);
		lineEditProbe4I2T->setGeometry(QRect(900, 340, 40, 20));
		lineEditProbe4I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe4I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe4I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9I2T = new QLineEdit(GateSetClass);
		lineEditProbe9I2T->setObjectName(QString::fromUtf8("lineEditProbe9I2T"));
		lineEditProbe9I2T->setEnabled(false);
		lineEditProbe9I2T->setGeometry(QRect(900, 540, 40, 20));
		lineEditProbe9I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe9I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe9I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3I2B = new QLineEdit(GateSetClass);
		lineEditProbe3I2B->setObjectName(QString::fromUtf8("lineEditProbe3I2B"));
		lineEditProbe3I2B->setEnabled(false);
		lineEditProbe3I2B->setGeometry(QRect(800, 300, 40, 20));
		lineEditProbe3I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe3I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe3I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2I2T = new QLineEdit(GateSetClass);
		lineEditProbe2I2T->setObjectName(QString::fromUtf8("lineEditProbe2I2T"));
		lineEditProbe2I2T->setEnabled(false);
		lineEditProbe2I2T->setGeometry(QRect(900, 260, 40, 20));
		lineEditProbe2I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe2I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe2I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2I2B = new QLineEdit(GateSetClass);
		lineEditProbe2I2B->setObjectName(QString::fromUtf8("lineEditProbe2I2B"));
		lineEditProbe2I2B->setEnabled(false);
		lineEditProbe2I2B->setGeometry(QRect(800, 260, 40, 20));
		lineEditProbe2I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe2I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe2I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6I2B = new QLineEdit(GateSetClass);
		lineEditProbe6I2B->setObjectName(QString::fromUtf8("lineEditProbe6I2B"));
		lineEditProbe6I2B->setEnabled(false);
		lineEditProbe6I2B->setGeometry(QRect(800, 420, 40, 20));
		lineEditProbe6I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe6I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe6I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5I2T = new QLineEdit(GateSetClass);
		lineEditProbe5I2T->setObjectName(QString::fromUtf8("lineEditProbe5I2T"));
		lineEditProbe5I2T->setEnabled(false);
		lineEditProbe5I2T->setGeometry(QRect(900, 380, 40, 20));
		lineEditProbe5I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe5I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe5I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7I2B = new QLineEdit(GateSetClass);
		lineEditProbe7I2B->setObjectName(QString::fromUtf8("lineEditProbe7I2B"));
		lineEditProbe7I2B->setEnabled(false);
		lineEditProbe7I2B->setGeometry(QRect(800, 460, 40, 20));
		lineEditProbe7I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe7I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe7I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3I2T = new QLineEdit(GateSetClass);
		lineEditProbe3I2T->setObjectName(QString::fromUtf8("lineEditProbe3I2T"));
		lineEditProbe3I2T->setEnabled(false);
		lineEditProbe3I2T->setGeometry(QRect(900, 300, 40, 20));
		lineEditProbe3I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe3I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe3I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10I2L = new QLineEdit(GateSetClass);
		lineEditProbe10I2L->setObjectName(QString::fromUtf8("lineEditProbe10I2L"));
		lineEditProbe10I2L->setEnabled(false);
		lineEditProbe10I2L->setGeometry(QRect(850, 580, 40, 20));
		lineEditProbe10I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe10I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe10I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9I2B = new QLineEdit(GateSetClass);
		lineEditProbe9I2B->setObjectName(QString::fromUtf8("lineEditProbe9I2B"));
		lineEditProbe9I2B->setEnabled(false);
		lineEditProbe9I2B->setGeometry(QRect(800, 540, 40, 20));
		lineEditProbe9I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe9I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe9I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8I2B = new QLineEdit(GateSetClass);
		lineEditProbe8I2B->setObjectName(QString::fromUtf8("lineEditProbe8I2B"));
		lineEditProbe8I2B->setEnabled(false);
		lineEditProbe8I2B->setGeometry(QRect(800, 500, 40, 20));
		lineEditProbe8I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe8I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe8I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5I2L = new QLineEdit(GateSetClass);
		lineEditProbe5I2L->setObjectName(QString::fromUtf8("lineEditProbe5I2L"));
		lineEditProbe5I2L->setEnabled(false);
		lineEditProbe5I2L->setGeometry(QRect(850, 380, 40, 20));
		lineEditProbe5I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe5I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe5I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3I2L = new QLineEdit(GateSetClass);
		lineEditProbe3I2L->setObjectName(QString::fromUtf8("lineEditProbe3I2L"));
		lineEditProbe3I2L->setEnabled(false);
		lineEditProbe3I2L->setGeometry(QRect(850, 300, 40, 20));
		lineEditProbe3I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe3I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe3I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8I2L = new QLineEdit(GateSetClass);
		lineEditProbe8I2L->setObjectName(QString::fromUtf8("lineEditProbe8I2L"));
		lineEditProbe8I2L->setEnabled(false);
		lineEditProbe8I2L->setGeometry(QRect(850, 500, 40, 20));
		lineEditProbe8I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe8I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe8I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4I2B = new QLineEdit(GateSetClass);
		lineEditProbe4I2B->setObjectName(QString::fromUtf8("lineEditProbe4I2B"));
		lineEditProbe4I2B->setEnabled(false);
		lineEditProbe4I2B->setGeometry(QRect(800, 340, 40, 20));
		lineEditProbe4I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe4I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe4I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7I2L = new QLineEdit(GateSetClass);
		lineEditProbe7I2L->setObjectName(QString::fromUtf8("lineEditProbe7I2L"));
		lineEditProbe7I2L->setEnabled(false);
		lineEditProbe7I2L->setGeometry(QRect(850, 460, 40, 20));
		lineEditProbe7I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe7I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe7I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6I2T = new QLineEdit(GateSetClass);
		lineEditProbe6I2T->setObjectName(QString::fromUtf8("lineEditProbe6I2T"));
		lineEditProbe6I2T->setEnabled(false);
		lineEditProbe6I2T->setGeometry(QRect(900, 420, 40, 20));
		lineEditProbe6I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe6I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe6I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6I2L = new QLineEdit(GateSetClass);
		lineEditProbe6I2L->setObjectName(QString::fromUtf8("lineEditProbe6I2L"));
		lineEditProbe6I2L->setEnabled(false);
		lineEditProbe6I2L->setGeometry(QRect(850, 420, 40, 20));
		lineEditProbe6I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe6I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe6I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1I2B = new QLineEdit(GateSetClass);
		lineEditProbe1I2B->setObjectName(QString::fromUtf8("lineEditProbe1I2B"));
		lineEditProbe1I2B->setEnabled(false);
		lineEditProbe1I2B->setGeometry(QRect(800, 220, 40, 20));
		lineEditProbe1I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe1I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe1I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9I2L = new QLineEdit(GateSetClass);
		lineEditProbe9I2L->setObjectName(QString::fromUtf8("lineEditProbe9I2L"));
		lineEditProbe9I2L->setEnabled(false);
		lineEditProbe9I2L->setGeometry(QRect(850, 540, 40, 20));
		lineEditProbe9I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe9I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe9I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8I2T = new QLineEdit(GateSetClass);
		lineEditProbe8I2T->setObjectName(QString::fromUtf8("lineEditProbe8I2T"));
		lineEditProbe8I2T->setEnabled(false);
		lineEditProbe8I2T->setGeometry(QRect(900, 500, 40, 20));
		lineEditProbe8I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe8I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe8I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10I2T = new QLineEdit(GateSetClass);
		lineEditProbe10I2T->setObjectName(QString::fromUtf8("lineEditProbe10I2T"));
		lineEditProbe10I2T->setEnabled(false);
		lineEditProbe10I2T->setGeometry(QRect(900, 580, 40, 20));
		lineEditProbe10I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe10I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe10I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2I2L = new QLineEdit(GateSetClass);
		lineEditProbe2I2L->setObjectName(QString::fromUtf8("lineEditProbe2I2L"));
		lineEditProbe2I2L->setEnabled(false);
		lineEditProbe2I2L->setGeometry(QRect(850, 260, 40, 20));
		lineEditProbe2I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe2I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe2I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1I2L = new QLineEdit(GateSetClass);
		lineEditProbe1I2L->setObjectName(QString::fromUtf8("lineEditProbe1I2L"));
		lineEditProbe1I2L->setEnabled(false);
		lineEditProbe1I2L->setGeometry(QRect(850, 220, 40, 20));
		lineEditProbe1I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe1I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe1I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4I2L = new QLineEdit(GateSetClass);
		lineEditProbe4I2L->setObjectName(QString::fromUtf8("lineEditProbe4I2L"));
		lineEditProbe4I2L->setEnabled(false);
		lineEditProbe4I2L->setGeometry(QRect(850, 340, 40, 20));
		lineEditProbe4I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe4I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe4I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe1 = new QCheckBox(GateSetClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(10, 220, 35, 20));
		checkBoxProbe1->setMinimumSize(QSize(35, 20));
		checkBoxProbe1->setMaximumSize(QSize(35, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe2 = new QCheckBox(GateSetClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(10, 260, 35, 20));
		checkBoxProbe2->setMinimumSize(QSize(35, 20));
		checkBoxProbe2->setMaximumSize(QSize(35, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(GateSetClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(10, 300, 35, 20));
		checkBoxProbe3->setMinimumSize(QSize(35, 20));
		checkBoxProbe3->setMaximumSize(QSize(35, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe4 = new QCheckBox(GateSetClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(10, 340, 35, 20));
		checkBoxProbe4->setMinimumSize(QSize(35, 20));
		checkBoxProbe4->setMaximumSize(QSize(35, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe5 = new QCheckBox(GateSetClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(10, 380, 35, 20));
		checkBoxProbe5->setMinimumSize(QSize(35, 20));
		checkBoxProbe5->setMaximumSize(QSize(35, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe6 = new QCheckBox(GateSetClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(10, 420, 35, 20));
		checkBoxProbe6->setMinimumSize(QSize(35, 20));
		checkBoxProbe6->setMaximumSize(QSize(35, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe7 = new QCheckBox(GateSetClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(10, 460, 35, 20));
		checkBoxProbe7->setMinimumSize(QSize(35, 20));
		checkBoxProbe7->setMaximumSize(QSize(35, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe8 = new QCheckBox(GateSetClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(10, 500, 35, 20));
		checkBoxProbe8->setMinimumSize(QSize(35, 20));
		checkBoxProbe8->setMaximumSize(QSize(35, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe9 = new QCheckBox(GateSetClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(10, 540, 35, 20));
		checkBoxProbe9->setMinimumSize(QSize(35, 20));
		checkBoxProbe9->setMaximumSize(QSize(35, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe10 = new QCheckBox(GateSetClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(10, 580, 35, 20));
		checkBoxProbe10->setMinimumSize(QSize(35, 20));
		checkBoxProbe10->setMaximumSize(QSize(35, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe20 = new QCheckBox(GateSetClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(60, 500, 35, 20));
		checkBoxProbe20->setMinimumSize(QSize(35, 20));
		checkBoxProbe20->setMaximumSize(QSize(35, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe14 = new QCheckBox(GateSetClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(60, 260, 35, 20));
		checkBoxProbe14->setMinimumSize(QSize(35, 20));
		checkBoxProbe14->setMaximumSize(QSize(35, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(GateSetClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(10, 620, 35, 20));
		checkBoxProbe11->setMinimumSize(QSize(35, 20));
		checkBoxProbe11->setMaximumSize(QSize(35, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe16 = new QCheckBox(GateSetClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(60, 340, 35, 20));
		checkBoxProbe16->setMinimumSize(QSize(35, 20));
		checkBoxProbe16->setMaximumSize(QSize(35, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe18 = new QCheckBox(GateSetClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(60, 420, 35, 20));
		checkBoxProbe18->setMinimumSize(QSize(35, 20));
		checkBoxProbe18->setMaximumSize(QSize(35, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe15 = new QCheckBox(GateSetClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(60, 300, 35, 20));
		checkBoxProbe15->setMinimumSize(QSize(35, 20));
		checkBoxProbe15->setMaximumSize(QSize(35, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe17 = new QCheckBox(GateSetClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(60, 380, 35, 20));
		checkBoxProbe17->setMinimumSize(QSize(35, 20));
		checkBoxProbe17->setMaximumSize(QSize(35, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe19 = new QCheckBox(GateSetClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(60, 460, 35, 20));
		checkBoxProbe19->setMinimumSize(QSize(35, 20));
		checkBoxProbe19->setMaximumSize(QSize(35, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe12 = new QCheckBox(GateSetClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(10, 660, 35, 20));
		checkBoxProbe12->setMinimumSize(QSize(35, 20));
		checkBoxProbe12->setMaximumSize(QSize(35, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13 = new QCheckBox(GateSetClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(60, 220, 35, 20));
		checkBoxProbe13->setMinimumSize(QSize(35, 20));
		checkBoxProbe13->setMaximumSize(QSize(35, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe23 = new QCheckBox(GateSetClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(60, 620, 35, 20));
		checkBoxProbe23->setMinimumSize(QSize(35, 20));
		checkBoxProbe23->setMaximumSize(QSize(35, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe26 = new QCheckBox(GateSetClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(120, 260, 35, 20));
		checkBoxProbe26->setMinimumSize(QSize(35, 20));
		checkBoxProbe26->setMaximumSize(QSize(35, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25 = new QCheckBox(GateSetClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(120, 220, 35, 20));
		checkBoxProbe25->setMinimumSize(QSize(35, 20));
		checkBoxProbe25->setMaximumSize(QSize(35, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe27 = new QCheckBox(GateSetClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(120, 300, 35, 20));
		checkBoxProbe27->setMinimumSize(QSize(35, 20));
		checkBoxProbe27->setMaximumSize(QSize(35, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe30 = new QCheckBox(GateSetClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(120, 420, 35, 20));
		checkBoxProbe30->setMinimumSize(QSize(35, 20));
		checkBoxProbe30->setMaximumSize(QSize(35, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe24 = new QCheckBox(GateSetClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(60, 660, 35, 20));
		checkBoxProbe24->setMinimumSize(QSize(35, 20));
		checkBoxProbe24->setMaximumSize(QSize(35, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe28 = new QCheckBox(GateSetClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(120, 340, 35, 20));
		checkBoxProbe28->setMinimumSize(QSize(35, 20));
		checkBoxProbe28->setMaximumSize(QSize(35, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe29 = new QCheckBox(GateSetClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(120, 380, 35, 20));
		checkBoxProbe29->setMinimumSize(QSize(35, 20));
		checkBoxProbe29->setMaximumSize(QSize(35, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe21 = new QCheckBox(GateSetClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(60, 540, 35, 20));
		checkBoxProbe21->setMinimumSize(QSize(35, 20));
		checkBoxProbe21->setMaximumSize(QSize(35, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe22 = new QCheckBox(GateSetClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(60, 580, 35, 20));
		checkBoxProbe22->setMinimumSize(QSize(35, 20));
		checkBoxProbe22->setMaximumSize(QSize(35, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(GateSetClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(660, 700, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(GateSetClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(800, 700, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe1_12 = new QCheckBox(GateSetClass);
		checkBoxProbe1_12->setObjectName(QString::fromUtf8("checkBoxProbe1_12"));
		checkBoxProbe1_12->setGeometry(QRect(10, 110, 60, 20));
		checkBoxProbe1_12->setMinimumSize(QSize(60, 20));
		checkBoxProbe1_12->setMaximumSize(QSize(60, 20));
		checkBoxProbe1_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13_24 = new QCheckBox(GateSetClass);
		checkBoxProbe13_24->setObjectName(QString::fromUtf8("checkBoxProbe13_24"));
		checkBoxProbe13_24->setGeometry(QRect(10, 140, 60, 20));
		checkBoxProbe13_24->setMinimumSize(QSize(60, 20));
		checkBoxProbe13_24->setMaximumSize(QSize(60, 20));
		checkBoxProbe13_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25_36 = new QCheckBox(GateSetClass);
		checkBoxProbe25_36->setObjectName(QString::fromUtf8("checkBoxProbe25_36"));
		checkBoxProbe25_36->setGeometry(QRect(10, 170, 60, 20));
		checkBoxProbe25_36->setMinimumSize(QSize(60, 20));
		checkBoxProbe25_36->setMaximumSize(QSize(60, 20));
		checkBoxProbe25_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllI = new QPushButton(GateSetClass);
		pushButtonAllI->setObjectName(QString::fromUtf8("pushButtonAllI"));
		pushButtonAllI->setGeometry(QRect(320, 130, 50, 20));
		pushButtonAllI->setMinimumSize(QSize(50, 20));
		pushButtonAllI->setMaximumSize(QSize(50, 20));
		pushButtonAllI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyI = new QPushButton(GateSetClass);
		pushButtonAnyI->setObjectName(QString::fromUtf8("pushButtonAnyI"));
		pushButtonAnyI->setGeometry(QRect(320, 170, 50, 20));
		pushButtonAnyI->setMinimumSize(QSize(50, 20));
		pushButtonAnyI->setMaximumSize(QSize(50, 20));
		pushButtonAnyI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyA = new QPushButton(GateSetClass);
		pushButtonAnyA->setObjectName(QString::fromUtf8("pushButtonAnyA"));
		pushButtonAnyA->setGeometry(QRect(530, 170, 50, 20));
		pushButtonAnyA->setMinimumSize(QSize(50, 20));
		pushButtonAnyA->setMaximumSize(QSize(50, 20));
		pushButtonAnyA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllA = new QPushButton(GateSetClass);
		pushButtonAllA->setObjectName(QString::fromUtf8("pushButtonAllA"));
		pushButtonAllA->setGeometry(QRect(530, 130, 50, 20));
		pushButtonAllA->setMinimumSize(QSize(50, 20));
		pushButtonAllA->setMaximumSize(QSize(50, 20));
		pushButtonAllA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllB = new QPushButton(GateSetClass);
		pushButtonAllB->setObjectName(QString::fromUtf8("pushButtonAllB"));
		pushButtonAllB->setGeometry(QRect(740, 130, 50, 20));
		pushButtonAllB->setMinimumSize(QSize(50, 20));
		pushButtonAllB->setMaximumSize(QSize(50, 20));
		pushButtonAllB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyB = new QPushButton(GateSetClass);
		pushButtonAnyB->setObjectName(QString::fromUtf8("pushButtonAnyB"));
		pushButtonAnyB->setGeometry(QRect(740, 170, 50, 20));
		pushButtonAnyB->setMinimumSize(QSize(50, 20));
		pushButtonAnyB->setMaximumSize(QSize(50, 20));
		pushButtonAnyB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyI2 = new QPushButton(GateSetClass);
		pushButtonAnyI2->setObjectName(QString::fromUtf8("pushButtonAnyI2"));
		pushButtonAnyI2->setGeometry(QRect(950, 170, 50, 20));
		pushButtonAnyI2->setMinimumSize(QSize(50, 20));
		pushButtonAnyI2->setMaximumSize(QSize(50, 20));
		pushButtonAnyI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllI2 = new QPushButton(GateSetClass);
		pushButtonAllI2->setObjectName(QString::fromUtf8("pushButtonAllI2"));
		pushButtonAllI2->setGeometry(QRect(950, 130, 50, 20));
		pushButtonAllI2->setMinimumSize(QSize(50, 20));
		pushButtonAllI2->setMaximumSize(QSize(50, 20));
		pushButtonAllI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(GateSetClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(120, 580, 35, 20));
		checkBoxProbe34->setMinimumSize(QSize(35, 20));
		checkBoxProbe34->setMaximumSize(QSize(35, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(GateSetClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(120, 540, 35, 20));
		checkBoxProbe33->setMinimumSize(QSize(35, 20));
		checkBoxProbe33->setMaximumSize(QSize(35, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe36 = new QCheckBox(GateSetClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(120, 660, 35, 20));
		checkBoxProbe36->setMinimumSize(QSize(35, 20));
		checkBoxProbe36->setMaximumSize(QSize(35, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe35 = new QCheckBox(GateSetClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(120, 620, 35, 20));
		checkBoxProbe35->setMinimumSize(QSize(35, 20));
		checkBoxProbe35->setMaximumSize(QSize(35, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe32 = new QCheckBox(GateSetClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(120, 500, 35, 20));
		checkBoxProbe32->setMinimumSize(QSize(35, 20));
		checkBoxProbe32->setMaximumSize(QSize(35, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(GateSetClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(120, 460, 35, 20));
		checkBoxProbe31->setMinimumSize(QSize(35, 20));
		checkBoxProbe31->setMaximumSize(QSize(35, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe12AL = new QLineEdit(GateSetClass);
		lineEditProbe12AL->setObjectName(QString::fromUtf8("lineEditProbe12AL"));
		lineEditProbe12AL->setEnabled(false);
		lineEditProbe12AL->setGeometry(QRect(430, 660, 40, 20));
		lineEditProbe12AL->setMinimumSize(QSize(40, 20));
		lineEditProbe12AL->setMaximumSize(QSize(40, 20));
		lineEditProbe12AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12BL = new QLineEdit(GateSetClass);
		lineEditProbe12BL->setObjectName(QString::fromUtf8("lineEditProbe12BL"));
		lineEditProbe12BL->setEnabled(false);
		lineEditProbe12BL->setGeometry(QRect(640, 660, 40, 20));
		lineEditProbe12BL->setMinimumSize(QSize(40, 20));
		lineEditProbe12BL->setMaximumSize(QSize(40, 20));
		lineEditProbe12BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11IB = new QLineEdit(GateSetClass);
		lineEditProbe11IB->setObjectName(QString::fromUtf8("lineEditProbe11IB"));
		lineEditProbe11IB->setEnabled(false);
		lineEditProbe11IB->setGeometry(QRect(170, 620, 40, 20));
		lineEditProbe11IB->setMinimumSize(QSize(40, 20));
		lineEditProbe11IB->setMaximumSize(QSize(40, 20));
		lineEditProbe11IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11IL = new QLineEdit(GateSetClass);
		lineEditProbe11IL->setObjectName(QString::fromUtf8("lineEditProbe11IL"));
		lineEditProbe11IL->setEnabled(false);
		lineEditProbe11IL->setGeometry(QRect(220, 620, 40, 20));
		lineEditProbe11IL->setMinimumSize(QSize(40, 20));
		lineEditProbe11IL->setMaximumSize(QSize(40, 20));
		lineEditProbe11IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11IT = new QLineEdit(GateSetClass);
		lineEditProbe11IT->setObjectName(QString::fromUtf8("lineEditProbe11IT"));
		lineEditProbe11IT->setEnabled(false);
		lineEditProbe11IT->setGeometry(QRect(270, 620, 40, 20));
		lineEditProbe11IT->setMinimumSize(QSize(40, 20));
		lineEditProbe11IT->setMaximumSize(QSize(40, 20));
		lineEditProbe11IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11AB = new QLineEdit(GateSetClass);
		lineEditProbe11AB->setObjectName(QString::fromUtf8("lineEditProbe11AB"));
		lineEditProbe11AB->setEnabled(false);
		lineEditProbe11AB->setGeometry(QRect(380, 620, 40, 20));
		lineEditProbe11AB->setMinimumSize(QSize(40, 20));
		lineEditProbe11AB->setMaximumSize(QSize(40, 20));
		lineEditProbe11AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11BT = new QLineEdit(GateSetClass);
		lineEditProbe11BT->setObjectName(QString::fromUtf8("lineEditProbe11BT"));
		lineEditProbe11BT->setEnabled(false);
		lineEditProbe11BT->setGeometry(QRect(690, 620, 40, 20));
		lineEditProbe11BT->setMinimumSize(QSize(40, 20));
		lineEditProbe11BT->setMaximumSize(QSize(40, 20));
		lineEditProbe11BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12AT = new QLineEdit(GateSetClass);
		lineEditProbe12AT->setObjectName(QString::fromUtf8("lineEditProbe12AT"));
		lineEditProbe12AT->setEnabled(false);
		lineEditProbe12AT->setGeometry(QRect(480, 660, 40, 20));
		lineEditProbe12AT->setMinimumSize(QSize(40, 20));
		lineEditProbe12AT->setMaximumSize(QSize(40, 20));
		lineEditProbe12AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12I2T = new QLineEdit(GateSetClass);
		lineEditProbe12I2T->setObjectName(QString::fromUtf8("lineEditProbe12I2T"));
		lineEditProbe12I2T->setEnabled(false);
		lineEditProbe12I2T->setGeometry(QRect(900, 660, 40, 20));
		lineEditProbe12I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe12I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe12I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12IL = new QLineEdit(GateSetClass);
		lineEditProbe12IL->setObjectName(QString::fromUtf8("lineEditProbe12IL"));
		lineEditProbe12IL->setEnabled(false);
		lineEditProbe12IL->setGeometry(QRect(220, 660, 40, 20));
		lineEditProbe12IL->setMinimumSize(QSize(40, 20));
		lineEditProbe12IL->setMaximumSize(QSize(40, 20));
		lineEditProbe12IL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12I2L = new QLineEdit(GateSetClass);
		lineEditProbe12I2L->setObjectName(QString::fromUtf8("lineEditProbe12I2L"));
		lineEditProbe12I2L->setEnabled(false);
		lineEditProbe12I2L->setGeometry(QRect(850, 660, 40, 20));
		lineEditProbe12I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe12I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe12I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12I2B = new QLineEdit(GateSetClass);
		lineEditProbe12I2B->setObjectName(QString::fromUtf8("lineEditProbe12I2B"));
		lineEditProbe12I2B->setEnabled(false);
		lineEditProbe12I2B->setGeometry(QRect(800, 660, 40, 20));
		lineEditProbe12I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe12I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe12I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12IT = new QLineEdit(GateSetClass);
		lineEditProbe12IT->setObjectName(QString::fromUtf8("lineEditProbe12IT"));
		lineEditProbe12IT->setEnabled(false);
		lineEditProbe12IT->setGeometry(QRect(270, 660, 40, 20));
		lineEditProbe12IT->setMinimumSize(QSize(40, 20));
		lineEditProbe12IT->setMaximumSize(QSize(40, 20));
		lineEditProbe12IT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11I2T = new QLineEdit(GateSetClass);
		lineEditProbe11I2T->setObjectName(QString::fromUtf8("lineEditProbe11I2T"));
		lineEditProbe11I2T->setEnabled(false);
		lineEditProbe11I2T->setGeometry(QRect(900, 620, 40, 20));
		lineEditProbe11I2T->setMinimumSize(QSize(40, 20));
		lineEditProbe11I2T->setMaximumSize(QSize(40, 20));
		lineEditProbe11I2T->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11AT = new QLineEdit(GateSetClass);
		lineEditProbe11AT->setObjectName(QString::fromUtf8("lineEditProbe11AT"));
		lineEditProbe11AT->setEnabled(false);
		lineEditProbe11AT->setGeometry(QRect(480, 620, 40, 20));
		lineEditProbe11AT->setMinimumSize(QSize(40, 20));
		lineEditProbe11AT->setMaximumSize(QSize(40, 20));
		lineEditProbe11AT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11BL = new QLineEdit(GateSetClass);
		lineEditProbe11BL->setObjectName(QString::fromUtf8("lineEditProbe11BL"));
		lineEditProbe11BL->setEnabled(false);
		lineEditProbe11BL->setGeometry(QRect(640, 620, 40, 20));
		lineEditProbe11BL->setMinimumSize(QSize(40, 20));
		lineEditProbe11BL->setMaximumSize(QSize(40, 20));
		lineEditProbe11BL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12IB = new QLineEdit(GateSetClass);
		lineEditProbe12IB->setObjectName(QString::fromUtf8("lineEditProbe12IB"));
		lineEditProbe12IB->setEnabled(false);
		lineEditProbe12IB->setGeometry(QRect(170, 660, 40, 20));
		lineEditProbe12IB->setMinimumSize(QSize(40, 20));
		lineEditProbe12IB->setMaximumSize(QSize(40, 20));
		lineEditProbe12IB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11I2L = new QLineEdit(GateSetClass);
		lineEditProbe11I2L->setObjectName(QString::fromUtf8("lineEditProbe11I2L"));
		lineEditProbe11I2L->setEnabled(false);
		lineEditProbe11I2L->setGeometry(QRect(850, 620, 40, 20));
		lineEditProbe11I2L->setMinimumSize(QSize(40, 20));
		lineEditProbe11I2L->setMaximumSize(QSize(40, 20));
		lineEditProbe11I2L->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11BB = new QLineEdit(GateSetClass);
		lineEditProbe11BB->setObjectName(QString::fromUtf8("lineEditProbe11BB"));
		lineEditProbe11BB->setEnabled(false);
		lineEditProbe11BB->setGeometry(QRect(590, 620, 40, 20));
		lineEditProbe11BB->setMinimumSize(QSize(40, 20));
		lineEditProbe11BB->setMaximumSize(QSize(40, 20));
		lineEditProbe11BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11I2B = new QLineEdit(GateSetClass);
		lineEditProbe11I2B->setObjectName(QString::fromUtf8("lineEditProbe11I2B"));
		lineEditProbe11I2B->setEnabled(false);
		lineEditProbe11I2B->setGeometry(QRect(800, 620, 40, 20));
		lineEditProbe11I2B->setMinimumSize(QSize(40, 20));
		lineEditProbe11I2B->setMaximumSize(QSize(40, 20));
		lineEditProbe11I2B->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12BT = new QLineEdit(GateSetClass);
		lineEditProbe12BT->setObjectName(QString::fromUtf8("lineEditProbe12BT"));
		lineEditProbe12BT->setEnabled(false);
		lineEditProbe12BT->setGeometry(QRect(690, 660, 40, 20));
		lineEditProbe12BT->setMinimumSize(QSize(40, 20));
		lineEditProbe12BT->setMaximumSize(QSize(40, 20));
		lineEditProbe12BT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12BB = new QLineEdit(GateSetClass);
		lineEditProbe12BB->setObjectName(QString::fromUtf8("lineEditProbe12BB"));
		lineEditProbe12BB->setEnabled(false);
		lineEditProbe12BB->setGeometry(QRect(590, 660, 40, 20));
		lineEditProbe12BB->setMinimumSize(QSize(40, 20));
		lineEditProbe12BB->setMaximumSize(QSize(40, 20));
		lineEditProbe12BB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11AL = new QLineEdit(GateSetClass);
		lineEditProbe11AL->setObjectName(QString::fromUtf8("lineEditProbe11AL"));
		lineEditProbe11AL->setEnabled(false);
		lineEditProbe11AL->setGeometry(QRect(430, 620, 40, 20));
		lineEditProbe11AL->setMinimumSize(QSize(40, 20));
		lineEditProbe11AL->setMaximumSize(QSize(40, 20));
		lineEditProbe11AL->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12AB = new QLineEdit(GateSetClass);
		lineEditProbe12AB->setObjectName(QString::fromUtf8("lineEditProbe12AB"));
		lineEditProbe12AB->setEnabled(false);
		lineEditProbe12AB->setGeometry(QRect(380, 660, 40, 20));
		lineEditProbe12AB->setMinimumSize(QSize(40, 20));
		lineEditProbe12AB->setMaximumSize(QSize(40, 20));
		lineEditProbe12AB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));

		retranslateUi(GateSetClass);

		QMetaObject::connectSlotsByName(GateSetClass);
	} // setupUi

	void retranslateUi(QDialog *GateSetClass)
	{
		GateSetClass->setWindowTitle(QApplication::translate("GateSetClass", "GateSet", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("GateSetClass", "\351\227\270\351\227\250\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("GateSetClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("GateSetClass", "\350\265\267\347\202\271", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("GateSetClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("GateSetClass", "\351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("GateSetClass", "\351\230\210\345\200\274", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("GateSetClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("GateSetClass", "\351\227\270\351\227\250I", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("GateSetClass", "\351\227\270\351\227\250A", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("GateSetClass", "\351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("GateSetClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("GateSetClass", "\351\230\210\345\200\274", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("GateSetClass", "\350\265\267\347\202\271", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("GateSetClass", "\350\265\267\347\202\271", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("GateSetClass", "\351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("GateSetClass", "\351\230\210\345\200\274", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("GateSetClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("GateSetClass", "\351\227\270\351\227\250B", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("GateSetClass", "\351\227\270\351\227\250I2", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("GateSetClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("GateSetClass", "\350\265\267\347\202\271", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("GateSetClass", "\351\230\210\345\200\274", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("GateSetClass", "\351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("GateSetClass", "(mm)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("GateSetClass", "1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("GateSetClass", "2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("GateSetClass", "3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("GateSetClass", "4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("GateSetClass", "5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("GateSetClass", "6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("GateSetClass", "7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("GateSetClass", "8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("GateSetClass", "9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("GateSetClass", "10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("GateSetClass", "20", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("GateSetClass", "14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("GateSetClass", "11", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("GateSetClass", "16", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("GateSetClass", "18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("GateSetClass", "15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("GateSetClass", "17", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("GateSetClass", "19", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("GateSetClass", "12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("GateSetClass", "13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("GateSetClass", "23", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("GateSetClass", "26", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("GateSetClass", "25", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("GateSetClass", "27", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("GateSetClass", "30", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("GateSetClass", "24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("GateSetClass", "28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("GateSetClass", "29", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("GateSetClass", "21", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("GateSetClass", "22", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("GateSetClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("GateSetClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1_12->setText(QApplication::translate("GateSetClass", "1~12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13_24->setText(QApplication::translate("GateSetClass", "13~24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25_36->setText(QApplication::translate("GateSetClass", "25~36", 0, QApplication::UnicodeUTF8));
		pushButtonAllI->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyI->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyA->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllA->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllB->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyB->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyI2->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllI2->setText(QApplication::translate("GateSetClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("GateSetClass", "34", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("GateSetClass", "33", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("GateSetClass", "36", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("GateSetClass", "35", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("GateSetClass", "32", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("GateSetClass", "31", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class GateSetClass: public Ui_GateSetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GATESET_H





/********************************************************************************
** Form generated from reading UI file 'impedanceset.ui'
**
** Created: Thu Jun 19 10:06:39 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPEDANCESET_H
#define UI_IMPEDANCESET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ImpedanceSetClass
{
public:
	QLabel *label_2;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QLineEdit *lineEditProbe5;
	QLineEdit *lineEditProbe1;
	QLineEdit *lineEditProbe4;
	QLineEdit *lineEditProbe6;
	QLineEdit *lineEditProbe2;
	QLineEdit *lineEditProbe3;
	QLineEdit *lineEditProbe7;
	QLineEdit *lineEditProbe9;
	QLineEdit *lineEditProbe10;
	QLineEdit *lineEditProbe8;
	QLabel *label;
	QLabel *label_3;
	QLabel *label_34;
	QLabel *label_35;
	QLabel *label_36;
	QLabel *label_37;
	QLabel *label_38;
	QLabel *label_39;
	QLabel *label_40;
	QLabel *label_41;
	QLabel *label_42;
	QLabel *label_43;
	QLabel *label_44;
	QLabel *label_45;
	QLabel *label_46;
	QLabel *label_47;
	QLabel *label_48;
	QLabel *label_49;
	QLabel *label_50;
	QLabel *label_51;
	QLabel *label_52;
	QLabel *label_53;
	QLabel *label_54;
	QLabel *label_55;
	QLabel *label_56;
	QLabel *label_57;
	QLabel *label_58;
	QLabel *label_59;
	QLabel *label_60;
	QLabel *label_61;
	QLineEdit *lineEditProbe12;
	QLineEdit *lineEditProbe11;
	QLineEdit *lineEditProbe13;
	QLineEdit *lineEditProbe14;
	QLineEdit *lineEditProbe16;
	QLineEdit *lineEditProbe18;
	QLineEdit *lineEditProbe15;
	QLineEdit *lineEditProbe17;
	QLineEdit *lineEditProbe19;
	QLineEdit *lineEditProbe20;
	QLineEdit *lineEditProbe21;
	QLineEdit *lineEditProbe28;
	QLineEdit *lineEditProbe23;
	QLineEdit *lineEditProbe22;
	QLineEdit *lineEditProbe30;
	QLineEdit *lineEditProbe24;
	QLineEdit *lineEditProbe27;
	QLineEdit *lineEditProbe25;
	QLineEdit *lineEditProbe29;
	QLineEdit *lineEditProbe26;
	QLabel *label_62;
	QLineEdit *lineEditProbe34;
	QLineEdit *lineEditProbe33;
	QLabel *label_63;
	QLineEdit *lineEditProbe36;
	QLineEdit *lineEditProbe32;
	QLabel *label_64;
	QLineEdit *lineEditProbe35;
	QLabel *label_65;
	QLabel *label_66;
	QLabel *label_67;
	QLineEdit *lineEditProbe31;

	void setupUi(QDialog *ImpedanceSetClass)
	{
		if (ImpedanceSetClass->objectName().isEmpty())
			ImpedanceSetClass->setObjectName(QString::fromUtf8("ImpedanceSetClass"));
		ImpedanceSetClass->resize(650, 523);
		ImpedanceSetClass->setMinimumSize(QSize(650, 523));
		ImpedanceSetClass->setMaximumSize(QSize(650, 523));
		ImpedanceSetClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label_2 = new QLabel(ImpedanceSetClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(270, 20, 120, 20));
		label_2->setMinimumSize(QSize(120, 20));
		label_2->setMaximumSize(QSize(120, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(ImpedanceSetClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(330, 490, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(ImpedanceSetClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(430, 490, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe5 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setEnabled(false);
		lineEditProbe5->setGeometry(QRect(80, 240, 80, 20));
		lineEditProbe5->setMinimumSize(QSize(80, 20));
		lineEditProbe5->setMaximumSize(QSize(80, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5->setAlignment(Qt::AlignCenter);
		lineEditProbe1 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setEnabled(false);
		lineEditProbe1->setGeometry(QRect(80, 80, 80, 20));
		lineEditProbe1->setMinimumSize(QSize(80, 20));
		lineEditProbe1->setMaximumSize(QSize(80, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe1->setAlignment(Qt::AlignCenter);
		lineEditProbe4 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setEnabled(false);
		lineEditProbe4->setGeometry(QRect(80, 200, 80, 20));
		lineEditProbe4->setMinimumSize(QSize(80, 20));
		lineEditProbe4->setMaximumSize(QSize(80, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4->setAlignment(Qt::AlignCenter);
		lineEditProbe6 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setEnabled(false);
		lineEditProbe6->setGeometry(QRect(80, 280, 80, 20));
		lineEditProbe6->setMinimumSize(QSize(80, 20));
		lineEditProbe6->setMaximumSize(QSize(80, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe6->setAlignment(Qt::AlignCenter);
		lineEditProbe2 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setEnabled(false);
		lineEditProbe2->setGeometry(QRect(80, 120, 80, 20));
		lineEditProbe2->setMinimumSize(QSize(80, 20));
		lineEditProbe2->setMaximumSize(QSize(80, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2->setAlignment(Qt::AlignCenter);
		lineEditProbe3 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setEnabled(false);
		lineEditProbe3->setGeometry(QRect(80, 160, 80, 20));
		lineEditProbe3->setMinimumSize(QSize(80, 20));
		lineEditProbe3->setMaximumSize(QSize(80, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3->setAlignment(Qt::AlignCenter);
		lineEditProbe7 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe7->setObjectName(QString::fromUtf8("lineEditProbe7"));
		lineEditProbe7->setEnabled(false);
		lineEditProbe7->setGeometry(QRect(80, 320, 80, 20));
		lineEditProbe7->setMinimumSize(QSize(80, 20));
		lineEditProbe7->setMaximumSize(QSize(80, 20));
		lineEditProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7->setAlignment(Qt::AlignCenter);
		lineEditProbe9 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe9->setObjectName(QString::fromUtf8("lineEditProbe9"));
		lineEditProbe9->setEnabled(false);
		lineEditProbe9->setGeometry(QRect(80, 400, 80, 20));
		lineEditProbe9->setMinimumSize(QSize(80, 20));
		lineEditProbe9->setMaximumSize(QSize(80, 20));
		lineEditProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9->setAlignment(Qt::AlignCenter);
		lineEditProbe10 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe10->setObjectName(QString::fromUtf8("lineEditProbe10"));
		lineEditProbe10->setEnabled(false);
		lineEditProbe10->setGeometry(QRect(80, 440, 80, 20));
		lineEditProbe10->setMinimumSize(QSize(80, 20));
		lineEditProbe10->setMaximumSize(QSize(80, 20));
		lineEditProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10->setAlignment(Qt::AlignCenter);
		lineEditProbe8 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe8->setObjectName(QString::fromUtf8("lineEditProbe8"));
		lineEditProbe8->setEnabled(false);
		lineEditProbe8->setGeometry(QRect(80, 360, 80, 20));
		lineEditProbe8->setMinimumSize(QSize(80, 20));
		lineEditProbe8->setMaximumSize(QSize(80, 20));
		lineEditProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8->setAlignment(Qt::AlignCenter);
		label = new QLabel(ImpedanceSetClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(50, 80, 20, 20));
		label->setMinimumSize(QSize(20, 20));
		label->setMaximumSize(QSize(20, 20));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_3 = new QLabel(ImpedanceSetClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(50, 120, 20, 20));
		label_3->setMinimumSize(QSize(20, 20));
		label_3->setMaximumSize(QSize(20, 20));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_34 = new QLabel(ImpedanceSetClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(50, 160, 20, 20));
		label_34->setMinimumSize(QSize(20, 20));
		label_34->setMaximumSize(QSize(20, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_35 = new QLabel(ImpedanceSetClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(50, 200, 20, 20));
		label_35->setMinimumSize(QSize(20, 20));
		label_35->setMaximumSize(QSize(20, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(ImpedanceSetClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(50, 240, 20, 20));
		label_36->setMinimumSize(QSize(20, 20));
		label_36->setMaximumSize(QSize(20, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_37 = new QLabel(ImpedanceSetClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(50, 280, 20, 20));
		label_37->setMinimumSize(QSize(20, 20));
		label_37->setMaximumSize(QSize(20, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_38 = new QLabel(ImpedanceSetClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(50, 320, 20, 20));
		label_38->setMinimumSize(QSize(20, 20));
		label_38->setMaximumSize(QSize(20, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_39 = new QLabel(ImpedanceSetClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(50, 360, 20, 20));
		label_39->setMinimumSize(QSize(20, 20));
		label_39->setMaximumSize(QSize(20, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_40 = new QLabel(ImpedanceSetClass);
		label_40->setObjectName(QString::fromUtf8("label_40"));
		label_40->setGeometry(QRect(50, 400, 20, 20));
		label_40->setMinimumSize(QSize(20, 20));
		label_40->setMaximumSize(QSize(20, 20));
		label_40->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_41 = new QLabel(ImpedanceSetClass);
		label_41->setObjectName(QString::fromUtf8("label_41"));
		label_41->setGeometry(QRect(50, 440, 20, 20));
		label_41->setMinimumSize(QSize(20, 20));
		label_41->setMaximumSize(QSize(20, 20));
		label_41->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_42 = new QLabel(ImpedanceSetClass);
		label_42->setObjectName(QString::fromUtf8("label_42"));
		label_42->setGeometry(QRect(200, 440, 20, 20));
		label_42->setMinimumSize(QSize(20, 20));
		label_42->setMaximumSize(QSize(20, 20));
		label_42->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_43 = new QLabel(ImpedanceSetClass);
		label_43->setObjectName(QString::fromUtf8("label_43"));
		label_43->setGeometry(QRect(200, 320, 20, 20));
		label_43->setMinimumSize(QSize(20, 20));
		label_43->setMaximumSize(QSize(20, 20));
		label_43->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_44 = new QLabel(ImpedanceSetClass);
		label_44->setObjectName(QString::fromUtf8("label_44"));
		label_44->setGeometry(QRect(200, 120, 20, 20));
		label_44->setMinimumSize(QSize(20, 20));
		label_44->setMaximumSize(QSize(20, 20));
		label_44->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_45 = new QLabel(ImpedanceSetClass);
		label_45->setObjectName(QString::fromUtf8("label_45"));
		label_45->setGeometry(QRect(200, 360, 20, 20));
		label_45->setMinimumSize(QSize(20, 20));
		label_45->setMaximumSize(QSize(20, 20));
		label_45->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_46 = new QLabel(ImpedanceSetClass);
		label_46->setObjectName(QString::fromUtf8("label_46"));
		label_46->setGeometry(QRect(200, 400, 20, 20));
		label_46->setMinimumSize(QSize(20, 20));
		label_46->setMaximumSize(QSize(20, 20));
		label_46->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_47 = new QLabel(ImpedanceSetClass);
		label_47->setObjectName(QString::fromUtf8("label_47"));
		label_47->setGeometry(QRect(200, 160, 20, 20));
		label_47->setMinimumSize(QSize(20, 20));
		label_47->setMaximumSize(QSize(20, 20));
		label_47->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_48 = new QLabel(ImpedanceSetClass);
		label_48->setObjectName(QString::fromUtf8("label_48"));
		label_48->setGeometry(QRect(200, 80, 20, 20));
		label_48->setMinimumSize(QSize(20, 20));
		label_48->setMaximumSize(QSize(20, 20));
		label_48->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_49 = new QLabel(ImpedanceSetClass);
		label_49->setObjectName(QString::fromUtf8("label_49"));
		label_49->setGeometry(QRect(200, 200, 20, 20));
		label_49->setMinimumSize(QSize(20, 20));
		label_49->setMaximumSize(QSize(20, 20));
		label_49->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_50 = new QLabel(ImpedanceSetClass);
		label_50->setObjectName(QString::fromUtf8("label_50"));
		label_50->setGeometry(QRect(200, 240, 20, 20));
		label_50->setMinimumSize(QSize(20, 20));
		label_50->setMaximumSize(QSize(20, 20));
		label_50->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_51 = new QLabel(ImpedanceSetClass);
		label_51->setObjectName(QString::fromUtf8("label_51"));
		label_51->setGeometry(QRect(200, 280, 20, 20));
		label_51->setMinimumSize(QSize(20, 20));
		label_51->setMaximumSize(QSize(20, 20));
		label_51->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_52 = new QLabel(ImpedanceSetClass);
		label_52->setObjectName(QString::fromUtf8("label_52"));
		label_52->setGeometry(QRect(350, 280, 20, 20));
		label_52->setMinimumSize(QSize(20, 20));
		label_52->setMaximumSize(QSize(20, 20));
		label_52->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_53 = new QLabel(ImpedanceSetClass);
		label_53->setObjectName(QString::fromUtf8("label_53"));
		label_53->setGeometry(QRect(350, 120, 20, 20));
		label_53->setMinimumSize(QSize(20, 20));
		label_53->setMaximumSize(QSize(20, 20));
		label_53->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_54 = new QLabel(ImpedanceSetClass);
		label_54->setObjectName(QString::fromUtf8("label_54"));
		label_54->setGeometry(QRect(350, 320, 20, 20));
		label_54->setMinimumSize(QSize(20, 20));
		label_54->setMaximumSize(QSize(20, 20));
		label_54->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_55 = new QLabel(ImpedanceSetClass);
		label_55->setObjectName(QString::fromUtf8("label_55"));
		label_55->setGeometry(QRect(350, 80, 20, 20));
		label_55->setMinimumSize(QSize(20, 20));
		label_55->setMaximumSize(QSize(20, 20));
		label_55->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_56 = new QLabel(ImpedanceSetClass);
		label_56->setObjectName(QString::fromUtf8("label_56"));
		label_56->setGeometry(QRect(350, 400, 20, 20));
		label_56->setMinimumSize(QSize(20, 20));
		label_56->setMaximumSize(QSize(20, 20));
		label_56->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_57 = new QLabel(ImpedanceSetClass);
		label_57->setObjectName(QString::fromUtf8("label_57"));
		label_57->setGeometry(QRect(350, 240, 20, 20));
		label_57->setMinimumSize(QSize(20, 20));
		label_57->setMaximumSize(QSize(20, 20));
		label_57->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_58 = new QLabel(ImpedanceSetClass);
		label_58->setObjectName(QString::fromUtf8("label_58"));
		label_58->setGeometry(QRect(350, 360, 20, 20));
		label_58->setMinimumSize(QSize(20, 20));
		label_58->setMaximumSize(QSize(20, 20));
		label_58->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_59 = new QLabel(ImpedanceSetClass);
		label_59->setObjectName(QString::fromUtf8("label_59"));
		label_59->setGeometry(QRect(350, 160, 20, 20));
		label_59->setMinimumSize(QSize(20, 20));
		label_59->setMaximumSize(QSize(20, 20));
		label_59->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_60 = new QLabel(ImpedanceSetClass);
		label_60->setObjectName(QString::fromUtf8("label_60"));
		label_60->setGeometry(QRect(350, 200, 20, 20));
		label_60->setMinimumSize(QSize(20, 20));
		label_60->setMaximumSize(QSize(20, 20));
		label_60->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_61 = new QLabel(ImpedanceSetClass);
		label_61->setObjectName(QString::fromUtf8("label_61"));
		label_61->setGeometry(QRect(350, 440, 20, 20));
		label_61->setMinimumSize(QSize(20, 20));
		label_61->setMaximumSize(QSize(20, 20));
		label_61->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe12 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe12->setObjectName(QString::fromUtf8("lineEditProbe12"));
		lineEditProbe12->setEnabled(false);
		lineEditProbe12->setGeometry(QRect(230, 120, 80, 20));
		lineEditProbe12->setMinimumSize(QSize(80, 20));
		lineEditProbe12->setMaximumSize(QSize(80, 20));
		lineEditProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12->setAlignment(Qt::AlignCenter);
		lineEditProbe11 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe11->setObjectName(QString::fromUtf8("lineEditProbe11"));
		lineEditProbe11->setEnabled(false);
		lineEditProbe11->setGeometry(QRect(230, 80, 80, 20));
		lineEditProbe11->setMinimumSize(QSize(80, 20));
		lineEditProbe11->setMaximumSize(QSize(80, 20));
		lineEditProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11->setAlignment(Qt::AlignCenter);
		lineEditProbe13 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe13->setObjectName(QString::fromUtf8("lineEditProbe13"));
		lineEditProbe13->setEnabled(false);
		lineEditProbe13->setGeometry(QRect(230, 160, 80, 20));
		lineEditProbe13->setMinimumSize(QSize(80, 20));
		lineEditProbe13->setMaximumSize(QSize(80, 20));
		lineEditProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe13->setAlignment(Qt::AlignCenter);
		lineEditProbe14 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe14->setObjectName(QString::fromUtf8("lineEditProbe14"));
		lineEditProbe14->setEnabled(false);
		lineEditProbe14->setGeometry(QRect(230, 200, 80, 20));
		lineEditProbe14->setMinimumSize(QSize(80, 20));
		lineEditProbe14->setMaximumSize(QSize(80, 20));
		lineEditProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe14->setAlignment(Qt::AlignCenter);
		lineEditProbe16 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe16->setObjectName(QString::fromUtf8("lineEditProbe16"));
		lineEditProbe16->setEnabled(false);
		lineEditProbe16->setGeometry(QRect(230, 280, 80, 20));
		lineEditProbe16->setMinimumSize(QSize(80, 20));
		lineEditProbe16->setMaximumSize(QSize(80, 20));
		lineEditProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe16->setAlignment(Qt::AlignCenter);
		lineEditProbe18 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe18->setObjectName(QString::fromUtf8("lineEditProbe18"));
		lineEditProbe18->setEnabled(false);
		lineEditProbe18->setGeometry(QRect(230, 360, 80, 20));
		lineEditProbe18->setMinimumSize(QSize(80, 20));
		lineEditProbe18->setMaximumSize(QSize(80, 20));
		lineEditProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe18->setAlignment(Qt::AlignCenter);
		lineEditProbe15 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe15->setObjectName(QString::fromUtf8("lineEditProbe15"));
		lineEditProbe15->setEnabled(false);
		lineEditProbe15->setGeometry(QRect(230, 240, 80, 20));
		lineEditProbe15->setMinimumSize(QSize(80, 20));
		lineEditProbe15->setMaximumSize(QSize(80, 20));
		lineEditProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe15->setAlignment(Qt::AlignCenter);
		lineEditProbe17 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe17->setObjectName(QString::fromUtf8("lineEditProbe17"));
		lineEditProbe17->setEnabled(false);
		lineEditProbe17->setGeometry(QRect(230, 320, 80, 20));
		lineEditProbe17->setMinimumSize(QSize(80, 20));
		lineEditProbe17->setMaximumSize(QSize(80, 20));
		lineEditProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe17->setAlignment(Qt::AlignCenter);
		lineEditProbe19 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe19->setObjectName(QString::fromUtf8("lineEditProbe19"));
		lineEditProbe19->setEnabled(false);
		lineEditProbe19->setGeometry(QRect(230, 400, 80, 20));
		lineEditProbe19->setMinimumSize(QSize(80, 20));
		lineEditProbe19->setMaximumSize(QSize(80, 20));
		lineEditProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe19->setAlignment(Qt::AlignCenter);
		lineEditProbe20 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe20->setObjectName(QString::fromUtf8("lineEditProbe20"));
		lineEditProbe20->setEnabled(false);
		lineEditProbe20->setGeometry(QRect(230, 440, 80, 20));
		lineEditProbe20->setMinimumSize(QSize(80, 20));
		lineEditProbe20->setMaximumSize(QSize(80, 20));
		lineEditProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe20->setAlignment(Qt::AlignCenter);
		lineEditProbe21 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe21->setObjectName(QString::fromUtf8("lineEditProbe21"));
		lineEditProbe21->setEnabled(false);
		lineEditProbe21->setGeometry(QRect(380, 80, 80, 20));
		lineEditProbe21->setMinimumSize(QSize(80, 20));
		lineEditProbe21->setMaximumSize(QSize(80, 20));
		lineEditProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe21->setAlignment(Qt::AlignCenter);
		lineEditProbe28 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe28->setObjectName(QString::fromUtf8("lineEditProbe28"));
		lineEditProbe28->setEnabled(false);
		lineEditProbe28->setGeometry(QRect(380, 360, 80, 20));
		lineEditProbe28->setMinimumSize(QSize(80, 20));
		lineEditProbe28->setMaximumSize(QSize(80, 20));
		lineEditProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe28->setAlignment(Qt::AlignCenter);
		lineEditProbe23 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe23->setObjectName(QString::fromUtf8("lineEditProbe23"));
		lineEditProbe23->setEnabled(false);
		lineEditProbe23->setGeometry(QRect(380, 160, 80, 20));
		lineEditProbe23->setMinimumSize(QSize(80, 20));
		lineEditProbe23->setMaximumSize(QSize(80, 20));
		lineEditProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe23->setAlignment(Qt::AlignCenter);
		lineEditProbe22 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe22->setObjectName(QString::fromUtf8("lineEditProbe22"));
		lineEditProbe22->setEnabled(false);
		lineEditProbe22->setGeometry(QRect(380, 120, 80, 20));
		lineEditProbe22->setMinimumSize(QSize(80, 20));
		lineEditProbe22->setMaximumSize(QSize(80, 20));
		lineEditProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe22->setAlignment(Qt::AlignCenter);
		lineEditProbe30 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe30->setObjectName(QString::fromUtf8("lineEditProbe30"));
		lineEditProbe30->setEnabled(false);
		lineEditProbe30->setGeometry(QRect(380, 440, 80, 20));
		lineEditProbe30->setMinimumSize(QSize(80, 20));
		lineEditProbe30->setMaximumSize(QSize(80, 20));
		lineEditProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe30->setAlignment(Qt::AlignCenter);
		lineEditProbe24 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe24->setObjectName(QString::fromUtf8("lineEditProbe24"));
		lineEditProbe24->setEnabled(false);
		lineEditProbe24->setGeometry(QRect(380, 200, 80, 20));
		lineEditProbe24->setMinimumSize(QSize(80, 20));
		lineEditProbe24->setMaximumSize(QSize(80, 20));
		lineEditProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe24->setAlignment(Qt::AlignCenter);
		lineEditProbe27 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe27->setObjectName(QString::fromUtf8("lineEditProbe27"));
		lineEditProbe27->setEnabled(false);
		lineEditProbe27->setGeometry(QRect(380, 320, 80, 20));
		lineEditProbe27->setMinimumSize(QSize(80, 20));
		lineEditProbe27->setMaximumSize(QSize(80, 20));
		lineEditProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe27->setAlignment(Qt::AlignCenter);
		lineEditProbe25 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe25->setObjectName(QString::fromUtf8("lineEditProbe25"));
		lineEditProbe25->setEnabled(false);
		lineEditProbe25->setGeometry(QRect(380, 240, 80, 20));
		lineEditProbe25->setMinimumSize(QSize(80, 20));
		lineEditProbe25->setMaximumSize(QSize(80, 20));
		lineEditProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe25->setAlignment(Qt::AlignCenter);
		lineEditProbe29 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe29->setObjectName(QString::fromUtf8("lineEditProbe29"));
		lineEditProbe29->setEnabled(false);
		lineEditProbe29->setGeometry(QRect(380, 400, 80, 20));
		lineEditProbe29->setMinimumSize(QSize(80, 20));
		lineEditProbe29->setMaximumSize(QSize(80, 20));
		lineEditProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe29->setAlignment(Qt::AlignCenter);
		lineEditProbe26 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe26->setObjectName(QString::fromUtf8("lineEditProbe26"));
		lineEditProbe26->setEnabled(false);
		lineEditProbe26->setGeometry(QRect(380, 280, 80, 20));
		lineEditProbe26->setMinimumSize(QSize(80, 20));
		lineEditProbe26->setMaximumSize(QSize(80, 20));
		lineEditProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe26->setAlignment(Qt::AlignCenter);
		label_62 = new QLabel(ImpedanceSetClass);
		label_62->setObjectName(QString::fromUtf8("label_62"));
		label_62->setGeometry(QRect(500, 80, 20, 20));
		label_62->setMinimumSize(QSize(20, 20));
		label_62->setMaximumSize(QSize(20, 20));
		label_62->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe34 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe34->setObjectName(QString::fromUtf8("lineEditProbe34"));
		lineEditProbe34->setEnabled(false);
		lineEditProbe34->setGeometry(QRect(530, 200, 80, 20));
		lineEditProbe34->setMinimumSize(QSize(80, 20));
		lineEditProbe34->setMaximumSize(QSize(80, 20));
		lineEditProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe34->setAlignment(Qt::AlignCenter);
		lineEditProbe33 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe33->setObjectName(QString::fromUtf8("lineEditProbe33"));
		lineEditProbe33->setEnabled(false);
		lineEditProbe33->setGeometry(QRect(530, 160, 80, 20));
		lineEditProbe33->setMinimumSize(QSize(80, 20));
		lineEditProbe33->setMaximumSize(QSize(80, 20));
		lineEditProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe33->setAlignment(Qt::AlignCenter);
		label_63 = new QLabel(ImpedanceSetClass);
		label_63->setObjectName(QString::fromUtf8("label_63"));
		label_63->setGeometry(QRect(500, 120, 20, 20));
		label_63->setMinimumSize(QSize(20, 20));
		label_63->setMaximumSize(QSize(20, 20));
		label_63->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe36 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe36->setObjectName(QString::fromUtf8("lineEditProbe36"));
		lineEditProbe36->setEnabled(false);
		lineEditProbe36->setGeometry(QRect(530, 280, 80, 20));
		lineEditProbe36->setMinimumSize(QSize(80, 20));
		lineEditProbe36->setMaximumSize(QSize(80, 20));
		lineEditProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe36->setAlignment(Qt::AlignCenter);
		lineEditProbe32 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe32->setObjectName(QString::fromUtf8("lineEditProbe32"));
		lineEditProbe32->setEnabled(false);
		lineEditProbe32->setGeometry(QRect(530, 120, 80, 20));
		lineEditProbe32->setMinimumSize(QSize(80, 20));
		lineEditProbe32->setMaximumSize(QSize(80, 20));
		lineEditProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe32->setAlignment(Qt::AlignCenter);
		label_64 = new QLabel(ImpedanceSetClass);
		label_64->setObjectName(QString::fromUtf8("label_64"));
		label_64->setGeometry(QRect(500, 200, 20, 20));
		label_64->setMinimumSize(QSize(20, 20));
		label_64->setMaximumSize(QSize(20, 20));
		label_64->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe35 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe35->setObjectName(QString::fromUtf8("lineEditProbe35"));
		lineEditProbe35->setEnabled(false);
		lineEditProbe35->setGeometry(QRect(530, 240, 80, 20));
		lineEditProbe35->setMinimumSize(QSize(80, 20));
		lineEditProbe35->setMaximumSize(QSize(80, 20));
		lineEditProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe35->setAlignment(Qt::AlignCenter);
		label_65 = new QLabel(ImpedanceSetClass);
		label_65->setObjectName(QString::fromUtf8("label_65"));
		label_65->setGeometry(QRect(500, 160, 20, 20));
		label_65->setMinimumSize(QSize(20, 20));
		label_65->setMaximumSize(QSize(20, 20));
		label_65->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_66 = new QLabel(ImpedanceSetClass);
		label_66->setObjectName(QString::fromUtf8("label_66"));
		label_66->setGeometry(QRect(500, 280, 20, 20));
		label_66->setMinimumSize(QSize(20, 20));
		label_66->setMaximumSize(QSize(20, 20));
		label_66->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_67 = new QLabel(ImpedanceSetClass);
		label_67->setObjectName(QString::fromUtf8("label_67"));
		label_67->setGeometry(QRect(500, 240, 20, 20));
		label_67->setMinimumSize(QSize(20, 20));
		label_67->setMaximumSize(QSize(20, 20));
		label_67->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe31 = new QLineEdit(ImpedanceSetClass);
		lineEditProbe31->setObjectName(QString::fromUtf8("lineEditProbe31"));
		lineEditProbe31->setEnabled(false);
		lineEditProbe31->setGeometry(QRect(530, 80, 80, 20));
		lineEditProbe31->setMinimumSize(QSize(80, 20));
		lineEditProbe31->setMaximumSize(QSize(80, 20));
		lineEditProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe31->setAlignment(Qt::AlignCenter);

		retranslateUi(ImpedanceSetClass);

		QMetaObject::connectSlotsByName(ImpedanceSetClass);
	} // setupUi

	void retranslateUi(QDialog *ImpedanceSetClass)
	{
		ImpedanceSetClass->setWindowTitle(QApplication::translate("ImpedanceSetClass", "ImpedanceSet", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("ImpedanceSetClass", "\351\230\273\346\212\227\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("ImpedanceSetClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("ImpedanceSetClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("ImpedanceSetClass", "1", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("ImpedanceSetClass", "2", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("ImpedanceSetClass", "3", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("ImpedanceSetClass", "4", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("ImpedanceSetClass", "5", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("ImpedanceSetClass", "6", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("ImpedanceSetClass", "7", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("ImpedanceSetClass", "8", 0, QApplication::UnicodeUTF8));
		label_40->setText(QApplication::translate("ImpedanceSetClass", "9", 0, QApplication::UnicodeUTF8));
		label_41->setText(QApplication::translate("ImpedanceSetClass", "10", 0, QApplication::UnicodeUTF8));
		label_42->setText(QApplication::translate("ImpedanceSetClass", "20", 0, QApplication::UnicodeUTF8));
		label_43->setText(QApplication::translate("ImpedanceSetClass", "17", 0, QApplication::UnicodeUTF8));
		label_44->setText(QApplication::translate("ImpedanceSetClass", "12", 0, QApplication::UnicodeUTF8));
		label_45->setText(QApplication::translate("ImpedanceSetClass", "18", 0, QApplication::UnicodeUTF8));
		label_46->setText(QApplication::translate("ImpedanceSetClass", "19", 0, QApplication::UnicodeUTF8));
		label_47->setText(QApplication::translate("ImpedanceSetClass", "13", 0, QApplication::UnicodeUTF8));
		label_48->setText(QApplication::translate("ImpedanceSetClass", "11", 0, QApplication::UnicodeUTF8));
		label_49->setText(QApplication::translate("ImpedanceSetClass", "14", 0, QApplication::UnicodeUTF8));
		label_50->setText(QApplication::translate("ImpedanceSetClass", "15", 0, QApplication::UnicodeUTF8));
		label_51->setText(QApplication::translate("ImpedanceSetClass", "16", 0, QApplication::UnicodeUTF8));
		label_52->setText(QApplication::translate("ImpedanceSetClass", "26", 0, QApplication::UnicodeUTF8));
		label_53->setText(QApplication::translate("ImpedanceSetClass", "22", 0, QApplication::UnicodeUTF8));
		label_54->setText(QApplication::translate("ImpedanceSetClass", "27", 0, QApplication::UnicodeUTF8));
		label_55->setText(QApplication::translate("ImpedanceSetClass", "21", 0, QApplication::UnicodeUTF8));
		label_56->setText(QApplication::translate("ImpedanceSetClass", "29", 0, QApplication::UnicodeUTF8));
		label_57->setText(QApplication::translate("ImpedanceSetClass", "25", 0, QApplication::UnicodeUTF8));
		label_58->setText(QApplication::translate("ImpedanceSetClass", "28", 0, QApplication::UnicodeUTF8));
		label_59->setText(QApplication::translate("ImpedanceSetClass", "23", 0, QApplication::UnicodeUTF8));
		label_60->setText(QApplication::translate("ImpedanceSetClass", "24", 0, QApplication::UnicodeUTF8));
		label_61->setText(QApplication::translate("ImpedanceSetClass", "30", 0, QApplication::UnicodeUTF8));
		label_62->setText(QApplication::translate("ImpedanceSetClass", "31", 0, QApplication::UnicodeUTF8));
		label_63->setText(QApplication::translate("ImpedanceSetClass", "32", 0, QApplication::UnicodeUTF8));
		label_64->setText(QApplication::translate("ImpedanceSetClass", "34", 0, QApplication::UnicodeUTF8));
		label_65->setText(QApplication::translate("ImpedanceSetClass", "33", 0, QApplication::UnicodeUTF8));
		label_66->setText(QApplication::translate("ImpedanceSetClass", "36", 0, QApplication::UnicodeUTF8));
		label_67->setText(QApplication::translate("ImpedanceSetClass", "35", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class ImpedanceSetClass: public Ui_ImpedanceSetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPEDANCESET_H



/********************************************************************************
** Form generated from reading UI file 'judge.ui'
**
** Created: Thu Jun 19 10:14:02 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JUDGE_H
#define UI_JUDGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_JudgeClass
{
public:
	QLabel *label_2;
	QLabel *label_20;
	QLabel *label_9;
	QLabel *label_23;
	QLabel *label_8;
	QComboBox *comboBoxAllI;
	QComboBox *comboBoxAnyI;
	QComboBox *comboBoxAnyA;
	QComboBox *comboBoxAllA;
	QComboBox *comboBoxAllB;
	QComboBox *comboBoxAnyB;
	QComboBox *comboBoxAnyI2;
	QComboBox *comboBoxAllI2;
	QComboBox *comboBoxI2Probe1;
	QComboBox *comboBoxIProbe2;
	QComboBox *comboBoxIProbe1;
	QComboBox *comboBoxBProbe2;
	QComboBox *comboBoxAProbe1;
	QComboBox *comboBoxAProbe2;
	QComboBox *comboBoxBProbe1;
	QComboBox *comboBoxI2Probe2;
	QComboBox *comboBoxI2Probe4;
	QComboBox *comboBoxI2Probe3;
	QComboBox *comboBoxAProbe4;
	QComboBox *comboBoxBProbe4;
	QComboBox *comboBoxBProbe3;
	QComboBox *comboBoxAProbe3;
	QComboBox *comboBoxIProbe3;
	QComboBox *comboBoxIProbe4;
	QComboBox *comboBoxIProbe8;
	QComboBox *comboBoxI2Probe5;
	QComboBox *comboBoxAProbe6;
	QComboBox *comboBoxI2Probe6;
	QComboBox *comboBoxBProbe8;
	QComboBox *comboBoxBProbe6;
	QComboBox *comboBoxIProbe7;
	QComboBox *comboBoxAProbe8;
	QComboBox *comboBoxBProbe5;
	QComboBox *comboBoxAProbe5;
	QComboBox *comboBoxAProbe7;
	QComboBox *comboBoxIProbe5;
	QComboBox *comboBoxBProbe7;
	QComboBox *comboBoxI2Probe8;
	QComboBox *comboBoxIProbe6;
	QComboBox *comboBoxI2Probe7;
	QComboBox *comboBoxI2Probe9;
	QComboBox *comboBoxBProbe9;
	QComboBox *comboBoxBProbe0;
	QComboBox *comboBoxIProbe0;
	QComboBox *comboBoxAProbe9;
	QComboBox *comboBoxI2Probe0;
	QComboBox *comboBoxIProbe9;
	QComboBox *comboBoxAProbe0;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QRadioButton *radioButtonAll;
	QCheckBox *checkBoxProbe18;
	QCheckBox *checkBoxProbe22;
	QCheckBox *checkBoxProbe16;
	QCheckBox *checkBoxProbe23;
	QCheckBox *checkBoxProbe21;
	QCheckBox *checkBoxProbe13;
	QCheckBox *checkBoxProbe11_20;
	QCheckBox *checkBoxProbe5;
	QCheckBox *checkBoxProbe21_30;
	QCheckBox *checkBoxProbe1_10;
	QCheckBox *checkBoxProbe6;
	QCheckBox *checkBoxProbe29;
	QCheckBox *checkBoxProbe7;
	QCheckBox *checkBoxProbe12;
	QCheckBox *checkBoxProbe4;
	QCheckBox *checkBoxProbe8;
	QCheckBox *checkBoxProbe24;
	QCheckBox *checkBoxProbe11;
	QCheckBox *checkBoxProbe17;
	QCheckBox *checkBoxProbe28;
	QCheckBox *checkBoxProbe19;
	QCheckBox *checkBoxProbe2;
	QCheckBox *checkBoxProbe20;
	QCheckBox *checkBoxProbe3;
	QCheckBox *checkBoxProbe26;
	QRadioButton *radioButtonAny;
	QCheckBox *checkBoxProbe1;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe9;
	QCheckBox *checkBoxProbe27;
	QCheckBox *checkBoxProbe30;
	QCheckBox *checkBoxProbe10;
	QCheckBox *checkBoxProbe15;
	QCheckBox *checkBoxProbe25;
	QPushButton *pushButtonAnyI2;
	QPushButton *pushButtonAllI2;
	QPushButton *pushButtonAnyB;
	QPushButton *pushButtonAllB;
	QPushButton *pushButtonAnyA;
	QPushButton *pushButtonAllA;
	QPushButton *pushButtonAllI;
	QPushButton *pushButtonAnyI;
	QCheckBox *checkBoxProbe36;
	QCheckBox *checkBoxProbe35;
	QCheckBox *checkBoxProbe32;
	QCheckBox *checkBoxProbe33;
	QCheckBox *checkBoxProbe31;
	QCheckBox *checkBoxProbe34;
	QComboBox *comboBoxAProbe11;
	QComboBox *comboBoxAProbe12;
	QComboBox *comboBoxBProbe11;
	QComboBox *comboBoxI2Probe11;
	QComboBox *comboBoxIProbe12;
	QComboBox *comboBoxI2Probe12;
	QComboBox *comboBoxBProbe12;
	QComboBox *comboBoxIProbe11;

	void setupUi(QDialog *JudgeClass)
	{
		if (JudgeClass->objectName().isEmpty())
			JudgeClass->setObjectName(QString::fromUtf8("JudgeClass"));
		JudgeClass->resize(950, 750);
		JudgeClass->setMinimumSize(QSize(950, 750));
		JudgeClass->setMaximumSize(QSize(950, 750));
		JudgeClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label_2 = new QLabel(JudgeClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(390, 20, 180, 20));
		label_2->setMinimumSize(QSize(180, 20));
		label_2->setMaximumSize(QSize(180, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(JudgeClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(605, 70, 50, 20));
		label_20->setMinimumSize(QSize(50, 20));
		label_20->setMaximumSize(QSize(50, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(JudgeClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(417, 70, 50, 20));
		label_9->setMinimumSize(QSize(50, 20));
		label_9->setMaximumSize(QSize(50, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(JudgeClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(791, 70, 50, 20));
		label_23->setMinimumSize(QSize(50, 20));
		label_23->setMaximumSize(QSize(50, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(JudgeClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(227, 70, 50, 20));
		label_8->setMinimumSize(QSize(50, 20));
		label_8->setMaximumSize(QSize(50, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		comboBoxAllI = new QComboBox(JudgeClass);
		comboBoxAllI->setObjectName(QString::fromUtf8("comboBoxAllI"));
		comboBoxAllI->setGeometry(QRect(190, 110, 120, 20));
		comboBoxAllI->setMinimumSize(QSize(120, 20));
		comboBoxAllI->setMaximumSize(QSize(120, 20));
		comboBoxAllI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAnyI = new QComboBox(JudgeClass);
		comboBoxAnyI->setObjectName(QString::fromUtf8("comboBoxAnyI"));
		comboBoxAnyI->setGeometry(QRect(190, 150, 120, 20));
		comboBoxAnyI->setMinimumSize(QSize(120, 20));
		comboBoxAnyI->setMaximumSize(QSize(120, 20));
		comboBoxAnyI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAnyA = new QComboBox(JudgeClass);
		comboBoxAnyA->setObjectName(QString::fromUtf8("comboBoxAnyA"));
		comboBoxAnyA->setGeometry(QRect(380, 150, 120, 20));
		comboBoxAnyA->setMinimumSize(QSize(120, 20));
		comboBoxAnyA->setMaximumSize(QSize(120, 20));
		comboBoxAnyA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAllA = new QComboBox(JudgeClass);
		comboBoxAllA->setObjectName(QString::fromUtf8("comboBoxAllA"));
		comboBoxAllA->setGeometry(QRect(380, 110, 120, 20));
		comboBoxAllA->setMinimumSize(QSize(120, 20));
		comboBoxAllA->setMaximumSize(QSize(120, 20));
		comboBoxAllA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAllB = new QComboBox(JudgeClass);
		comboBoxAllB->setObjectName(QString::fromUtf8("comboBoxAllB"));
		comboBoxAllB->setGeometry(QRect(570, 110, 120, 20));
		comboBoxAllB->setMinimumSize(QSize(120, 20));
		comboBoxAllB->setMaximumSize(QSize(120, 20));
		comboBoxAllB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAnyB = new QComboBox(JudgeClass);
		comboBoxAnyB->setObjectName(QString::fromUtf8("comboBoxAnyB"));
		comboBoxAnyB->setGeometry(QRect(570, 150, 120, 20));
		comboBoxAnyB->setMinimumSize(QSize(120, 20));
		comboBoxAnyB->setMaximumSize(QSize(120, 20));
		comboBoxAnyB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAnyI2 = new QComboBox(JudgeClass);
		comboBoxAnyI2->setObjectName(QString::fromUtf8("comboBoxAnyI2"));
		comboBoxAnyI2->setGeometry(QRect(760, 150, 120, 20));
		comboBoxAnyI2->setMinimumSize(QSize(120, 20));
		comboBoxAnyI2->setMaximumSize(QSize(120, 20));
		comboBoxAnyI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAllI2 = new QComboBox(JudgeClass);
		comboBoxAllI2->setObjectName(QString::fromUtf8("comboBoxAllI2"));
		comboBoxAllI2->setGeometry(QRect(760, 110, 120, 20));
		comboBoxAllI2->setMinimumSize(QSize(120, 20));
		comboBoxAllI2->setMaximumSize(QSize(120, 20));
		comboBoxAllI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe1 = new QComboBox(JudgeClass);
		comboBoxI2Probe1->setObjectName(QString::fromUtf8("comboBoxI2Probe1"));
		comboBoxI2Probe1->setEnabled(false);
		comboBoxI2Probe1->setGeometry(QRect(760, 200, 120, 20));
		comboBoxI2Probe1->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe1->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe2 = new QComboBox(JudgeClass);
		comboBoxIProbe2->setObjectName(QString::fromUtf8("comboBoxIProbe2"));
		comboBoxIProbe2->setEnabled(false);
		comboBoxIProbe2->setGeometry(QRect(190, 240, 120, 20));
		comboBoxIProbe2->setMinimumSize(QSize(120, 20));
		comboBoxIProbe2->setMaximumSize(QSize(120, 20));
		comboBoxIProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe1 = new QComboBox(JudgeClass);
		comboBoxIProbe1->setObjectName(QString::fromUtf8("comboBoxIProbe1"));
		comboBoxIProbe1->setEnabled(false);
		comboBoxIProbe1->setGeometry(QRect(190, 200, 120, 20));
		comboBoxIProbe1->setMinimumSize(QSize(120, 20));
		comboBoxIProbe1->setMaximumSize(QSize(120, 20));
		comboBoxIProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe2 = new QComboBox(JudgeClass);
		comboBoxBProbe2->setObjectName(QString::fromUtf8("comboBoxBProbe2"));
		comboBoxBProbe2->setEnabled(false);
		comboBoxBProbe2->setGeometry(QRect(570, 240, 120, 20));
		comboBoxBProbe2->setMinimumSize(QSize(120, 20));
		comboBoxBProbe2->setMaximumSize(QSize(120, 20));
		comboBoxBProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe1 = new QComboBox(JudgeClass);
		comboBoxAProbe1->setObjectName(QString::fromUtf8("comboBoxAProbe1"));
		comboBoxAProbe1->setEnabled(false);
		comboBoxAProbe1->setGeometry(QRect(380, 200, 120, 20));
		comboBoxAProbe1->setMinimumSize(QSize(120, 20));
		comboBoxAProbe1->setMaximumSize(QSize(120, 20));
		comboBoxAProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe2 = new QComboBox(JudgeClass);
		comboBoxAProbe2->setObjectName(QString::fromUtf8("comboBoxAProbe2"));
		comboBoxAProbe2->setEnabled(false);
		comboBoxAProbe2->setGeometry(QRect(380, 240, 120, 20));
		comboBoxAProbe2->setMinimumSize(QSize(120, 20));
		comboBoxAProbe2->setMaximumSize(QSize(120, 20));
		comboBoxAProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe1 = new QComboBox(JudgeClass);
		comboBoxBProbe1->setObjectName(QString::fromUtf8("comboBoxBProbe1"));
		comboBoxBProbe1->setEnabled(false);
		comboBoxBProbe1->setGeometry(QRect(570, 200, 120, 20));
		comboBoxBProbe1->setMinimumSize(QSize(120, 20));
		comboBoxBProbe1->setMaximumSize(QSize(120, 20));
		comboBoxBProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe2 = new QComboBox(JudgeClass);
		comboBoxI2Probe2->setObjectName(QString::fromUtf8("comboBoxI2Probe2"));
		comboBoxI2Probe2->setEnabled(false);
		comboBoxI2Probe2->setGeometry(QRect(760, 240, 120, 20));
		comboBoxI2Probe2->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe2->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe4 = new QComboBox(JudgeClass);
		comboBoxI2Probe4->setObjectName(QString::fromUtf8("comboBoxI2Probe4"));
		comboBoxI2Probe4->setEnabled(false);
		comboBoxI2Probe4->setGeometry(QRect(760, 320, 120, 20));
		comboBoxI2Probe4->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe4->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe3 = new QComboBox(JudgeClass);
		comboBoxI2Probe3->setObjectName(QString::fromUtf8("comboBoxI2Probe3"));
		comboBoxI2Probe3->setEnabled(false);
		comboBoxI2Probe3->setGeometry(QRect(760, 280, 120, 20));
		comboBoxI2Probe3->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe3->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe4 = new QComboBox(JudgeClass);
		comboBoxAProbe4->setObjectName(QString::fromUtf8("comboBoxAProbe4"));
		comboBoxAProbe4->setEnabled(false);
		comboBoxAProbe4->setGeometry(QRect(380, 320, 120, 20));
		comboBoxAProbe4->setMinimumSize(QSize(120, 20));
		comboBoxAProbe4->setMaximumSize(QSize(120, 20));
		comboBoxAProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe4 = new QComboBox(JudgeClass);
		comboBoxBProbe4->setObjectName(QString::fromUtf8("comboBoxBProbe4"));
		comboBoxBProbe4->setEnabled(false);
		comboBoxBProbe4->setGeometry(QRect(570, 320, 120, 20));
		comboBoxBProbe4->setMinimumSize(QSize(120, 20));
		comboBoxBProbe4->setMaximumSize(QSize(120, 20));
		comboBoxBProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe3 = new QComboBox(JudgeClass);
		comboBoxBProbe3->setObjectName(QString::fromUtf8("comboBoxBProbe3"));
		comboBoxBProbe3->setEnabled(false);
		comboBoxBProbe3->setGeometry(QRect(570, 280, 120, 20));
		comboBoxBProbe3->setMinimumSize(QSize(120, 20));
		comboBoxBProbe3->setMaximumSize(QSize(120, 20));
		comboBoxBProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe3 = new QComboBox(JudgeClass);
		comboBoxAProbe3->setObjectName(QString::fromUtf8("comboBoxAProbe3"));
		comboBoxAProbe3->setEnabled(false);
		comboBoxAProbe3->setGeometry(QRect(380, 280, 120, 20));
		comboBoxAProbe3->setMinimumSize(QSize(120, 20));
		comboBoxAProbe3->setMaximumSize(QSize(120, 20));
		comboBoxAProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe3 = new QComboBox(JudgeClass);
		comboBoxIProbe3->setObjectName(QString::fromUtf8("comboBoxIProbe3"));
		comboBoxIProbe3->setEnabled(false);
		comboBoxIProbe3->setGeometry(QRect(190, 280, 120, 20));
		comboBoxIProbe3->setMinimumSize(QSize(120, 20));
		comboBoxIProbe3->setMaximumSize(QSize(120, 20));
		comboBoxIProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe4 = new QComboBox(JudgeClass);
		comboBoxIProbe4->setObjectName(QString::fromUtf8("comboBoxIProbe4"));
		comboBoxIProbe4->setEnabled(false);
		comboBoxIProbe4->setGeometry(QRect(190, 320, 120, 20));
		comboBoxIProbe4->setMinimumSize(QSize(120, 20));
		comboBoxIProbe4->setMaximumSize(QSize(120, 20));
		comboBoxIProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe8 = new QComboBox(JudgeClass);
		comboBoxIProbe8->setObjectName(QString::fromUtf8("comboBoxIProbe8"));
		comboBoxIProbe8->setEnabled(false);
		comboBoxIProbe8->setGeometry(QRect(190, 480, 120, 20));
		comboBoxIProbe8->setMinimumSize(QSize(120, 20));
		comboBoxIProbe8->setMaximumSize(QSize(120, 20));
		comboBoxIProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe5 = new QComboBox(JudgeClass);
		comboBoxI2Probe5->setObjectName(QString::fromUtf8("comboBoxI2Probe5"));
		comboBoxI2Probe5->setEnabled(false);
		comboBoxI2Probe5->setGeometry(QRect(760, 360, 120, 20));
		comboBoxI2Probe5->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe5->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe6 = new QComboBox(JudgeClass);
		comboBoxAProbe6->setObjectName(QString::fromUtf8("comboBoxAProbe6"));
		comboBoxAProbe6->setEnabled(false);
		comboBoxAProbe6->setGeometry(QRect(380, 400, 120, 20));
		comboBoxAProbe6->setMinimumSize(QSize(120, 20));
		comboBoxAProbe6->setMaximumSize(QSize(120, 20));
		comboBoxAProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe6 = new QComboBox(JudgeClass);
		comboBoxI2Probe6->setObjectName(QString::fromUtf8("comboBoxI2Probe6"));
		comboBoxI2Probe6->setEnabled(false);
		comboBoxI2Probe6->setGeometry(QRect(760, 400, 120, 20));
		comboBoxI2Probe6->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe6->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe8 = new QComboBox(JudgeClass);
		comboBoxBProbe8->setObjectName(QString::fromUtf8("comboBoxBProbe8"));
		comboBoxBProbe8->setEnabled(false);
		comboBoxBProbe8->setGeometry(QRect(570, 480, 120, 20));
		comboBoxBProbe8->setMinimumSize(QSize(120, 20));
		comboBoxBProbe8->setMaximumSize(QSize(120, 20));
		comboBoxBProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe6 = new QComboBox(JudgeClass);
		comboBoxBProbe6->setObjectName(QString::fromUtf8("comboBoxBProbe6"));
		comboBoxBProbe6->setEnabled(false);
		comboBoxBProbe6->setGeometry(QRect(570, 400, 120, 20));
		comboBoxBProbe6->setMinimumSize(QSize(120, 20));
		comboBoxBProbe6->setMaximumSize(QSize(120, 20));
		comboBoxBProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe7 = new QComboBox(JudgeClass);
		comboBoxIProbe7->setObjectName(QString::fromUtf8("comboBoxIProbe7"));
		comboBoxIProbe7->setEnabled(false);
		comboBoxIProbe7->setGeometry(QRect(190, 440, 120, 20));
		comboBoxIProbe7->setMinimumSize(QSize(120, 20));
		comboBoxIProbe7->setMaximumSize(QSize(120, 20));
		comboBoxIProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe8 = new QComboBox(JudgeClass);
		comboBoxAProbe8->setObjectName(QString::fromUtf8("comboBoxAProbe8"));
		comboBoxAProbe8->setEnabled(false);
		comboBoxAProbe8->setGeometry(QRect(380, 480, 120, 20));
		comboBoxAProbe8->setMinimumSize(QSize(120, 20));
		comboBoxAProbe8->setMaximumSize(QSize(120, 20));
		comboBoxAProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe5 = new QComboBox(JudgeClass);
		comboBoxBProbe5->setObjectName(QString::fromUtf8("comboBoxBProbe5"));
		comboBoxBProbe5->setEnabled(false);
		comboBoxBProbe5->setGeometry(QRect(570, 360, 120, 20));
		comboBoxBProbe5->setMinimumSize(QSize(120, 20));
		comboBoxBProbe5->setMaximumSize(QSize(120, 20));
		comboBoxBProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe5 = new QComboBox(JudgeClass);
		comboBoxAProbe5->setObjectName(QString::fromUtf8("comboBoxAProbe5"));
		comboBoxAProbe5->setEnabled(false);
		comboBoxAProbe5->setGeometry(QRect(380, 360, 120, 20));
		comboBoxAProbe5->setMinimumSize(QSize(120, 20));
		comboBoxAProbe5->setMaximumSize(QSize(120, 20));
		comboBoxAProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe7 = new QComboBox(JudgeClass);
		comboBoxAProbe7->setObjectName(QString::fromUtf8("comboBoxAProbe7"));
		comboBoxAProbe7->setEnabled(false);
		comboBoxAProbe7->setGeometry(QRect(380, 440, 120, 20));
		comboBoxAProbe7->setMinimumSize(QSize(120, 20));
		comboBoxAProbe7->setMaximumSize(QSize(120, 20));
		comboBoxAProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe5 = new QComboBox(JudgeClass);
		comboBoxIProbe5->setObjectName(QString::fromUtf8("comboBoxIProbe5"));
		comboBoxIProbe5->setEnabled(false);
		comboBoxIProbe5->setGeometry(QRect(190, 360, 120, 20));
		comboBoxIProbe5->setMinimumSize(QSize(120, 20));
		comboBoxIProbe5->setMaximumSize(QSize(120, 20));
		comboBoxIProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe7 = new QComboBox(JudgeClass);
		comboBoxBProbe7->setObjectName(QString::fromUtf8("comboBoxBProbe7"));
		comboBoxBProbe7->setEnabled(false);
		comboBoxBProbe7->setGeometry(QRect(570, 440, 120, 20));
		comboBoxBProbe7->setMinimumSize(QSize(120, 20));
		comboBoxBProbe7->setMaximumSize(QSize(120, 20));
		comboBoxBProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe8 = new QComboBox(JudgeClass);
		comboBoxI2Probe8->setObjectName(QString::fromUtf8("comboBoxI2Probe8"));
		comboBoxI2Probe8->setEnabled(false);
		comboBoxI2Probe8->setGeometry(QRect(760, 480, 120, 20));
		comboBoxI2Probe8->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe8->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe6 = new QComboBox(JudgeClass);
		comboBoxIProbe6->setObjectName(QString::fromUtf8("comboBoxIProbe6"));
		comboBoxIProbe6->setEnabled(false);
		comboBoxIProbe6->setGeometry(QRect(190, 400, 120, 20));
		comboBoxIProbe6->setMinimumSize(QSize(120, 20));
		comboBoxIProbe6->setMaximumSize(QSize(120, 20));
		comboBoxIProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe7 = new QComboBox(JudgeClass);
		comboBoxI2Probe7->setObjectName(QString::fromUtf8("comboBoxI2Probe7"));
		comboBoxI2Probe7->setEnabled(false);
		comboBoxI2Probe7->setGeometry(QRect(760, 440, 120, 20));
		comboBoxI2Probe7->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe7->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe9 = new QComboBox(JudgeClass);
		comboBoxI2Probe9->setObjectName(QString::fromUtf8("comboBoxI2Probe9"));
		comboBoxI2Probe9->setEnabled(false);
		comboBoxI2Probe9->setGeometry(QRect(760, 520, 120, 20));
		comboBoxI2Probe9->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe9->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe9 = new QComboBox(JudgeClass);
		comboBoxBProbe9->setObjectName(QString::fromUtf8("comboBoxBProbe9"));
		comboBoxBProbe9->setEnabled(false);
		comboBoxBProbe9->setGeometry(QRect(570, 520, 120, 20));
		comboBoxBProbe9->setMinimumSize(QSize(120, 20));
		comboBoxBProbe9->setMaximumSize(QSize(120, 20));
		comboBoxBProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe0 = new QComboBox(JudgeClass);
		comboBoxBProbe0->setObjectName(QString::fromUtf8("comboBoxBProbe0"));
		comboBoxBProbe0->setEnabled(false);
		comboBoxBProbe0->setGeometry(QRect(570, 560, 120, 20));
		comboBoxBProbe0->setMinimumSize(QSize(120, 20));
		comboBoxBProbe0->setMaximumSize(QSize(120, 20));
		comboBoxBProbe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe0 = new QComboBox(JudgeClass);
		comboBoxIProbe0->setObjectName(QString::fromUtf8("comboBoxIProbe0"));
		comboBoxIProbe0->setEnabled(false);
		comboBoxIProbe0->setGeometry(QRect(190, 560, 120, 20));
		comboBoxIProbe0->setMinimumSize(QSize(120, 20));
		comboBoxIProbe0->setMaximumSize(QSize(120, 20));
		comboBoxIProbe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe9 = new QComboBox(JudgeClass);
		comboBoxAProbe9->setObjectName(QString::fromUtf8("comboBoxAProbe9"));
		comboBoxAProbe9->setEnabled(false);
		comboBoxAProbe9->setGeometry(QRect(380, 520, 120, 20));
		comboBoxAProbe9->setMinimumSize(QSize(120, 20));
		comboBoxAProbe9->setMaximumSize(QSize(120, 20));
		comboBoxAProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe0 = new QComboBox(JudgeClass);
		comboBoxI2Probe0->setObjectName(QString::fromUtf8("comboBoxI2Probe0"));
		comboBoxI2Probe0->setEnabled(false);
		comboBoxI2Probe0->setGeometry(QRect(760, 560, 120, 20));
		comboBoxI2Probe0->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe0->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe9 = new QComboBox(JudgeClass);
		comboBoxIProbe9->setObjectName(QString::fromUtf8("comboBoxIProbe9"));
		comboBoxIProbe9->setEnabled(false);
		comboBoxIProbe9->setGeometry(QRect(190, 520, 120, 20));
		comboBoxIProbe9->setMinimumSize(QSize(120, 20));
		comboBoxIProbe9->setMaximumSize(QSize(120, 20));
		comboBoxIProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe0 = new QComboBox(JudgeClass);
		comboBoxAProbe0->setObjectName(QString::fromUtf8("comboBoxAProbe0"));
		comboBoxAProbe0->setEnabled(false);
		comboBoxAProbe0->setGeometry(QRect(380, 560, 120, 20));
		comboBoxAProbe0->setMinimumSize(QSize(120, 20));
		comboBoxAProbe0->setMaximumSize(QSize(120, 20));
		comboBoxAProbe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonYes = new QPushButton(JudgeClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(620, 700, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(JudgeClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(760, 700, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAll = new QRadioButton(JudgeClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(90, 110, 81, 16));
		radioButtonAll->setMinimumSize(QSize(81, 16));
		radioButtonAll->setMaximumSize(QSize(81, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe18 = new QCheckBox(JudgeClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(70, 400, 35, 20));
		checkBoxProbe18->setMinimumSize(QSize(35, 20));
		checkBoxProbe18->setMaximumSize(QSize(35, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe22 = new QCheckBox(JudgeClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(70, 560, 35, 20));
		checkBoxProbe22->setMinimumSize(QSize(35, 20));
		checkBoxProbe22->setMaximumSize(QSize(35, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe16 = new QCheckBox(JudgeClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(70, 320, 35, 20));
		checkBoxProbe16->setMinimumSize(QSize(35, 20));
		checkBoxProbe16->setMaximumSize(QSize(35, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe23 = new QCheckBox(JudgeClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(70, 600, 35, 20));
		checkBoxProbe23->setMinimumSize(QSize(35, 20));
		checkBoxProbe23->setMaximumSize(QSize(35, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe21 = new QCheckBox(JudgeClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(70, 520, 35, 20));
		checkBoxProbe21->setMinimumSize(QSize(35, 20));
		checkBoxProbe21->setMaximumSize(QSize(35, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13 = new QCheckBox(JudgeClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(70, 200, 35, 20));
		checkBoxProbe13->setMinimumSize(QSize(35, 20));
		checkBoxProbe13->setMaximumSize(QSize(35, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11_20 = new QCheckBox(JudgeClass);
		checkBoxProbe11_20->setObjectName(QString::fromUtf8("checkBoxProbe11_20"));
		checkBoxProbe11_20->setGeometry(QRect(20, 120, 60, 20));
		checkBoxProbe11_20->setMinimumSize(QSize(60, 20));
		checkBoxProbe11_20->setMaximumSize(QSize(60, 20));
		checkBoxProbe11_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe5 = new QCheckBox(JudgeClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(20, 360, 35, 20));
		checkBoxProbe5->setMinimumSize(QSize(35, 20));
		checkBoxProbe5->setMaximumSize(QSize(35, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe21_30 = new QCheckBox(JudgeClass);
		checkBoxProbe21_30->setObjectName(QString::fromUtf8("checkBoxProbe21_30"));
		checkBoxProbe21_30->setGeometry(QRect(20, 150, 60, 20));
		checkBoxProbe21_30->setMinimumSize(QSize(60, 20));
		checkBoxProbe21_30->setMaximumSize(QSize(60, 20));
		checkBoxProbe21_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe1_10 = new QCheckBox(JudgeClass);
		checkBoxProbe1_10->setObjectName(QString::fromUtf8("checkBoxProbe1_10"));
		checkBoxProbe1_10->setGeometry(QRect(20, 90, 60, 20));
		checkBoxProbe1_10->setMinimumSize(QSize(60, 20));
		checkBoxProbe1_10->setMaximumSize(QSize(60, 20));
		checkBoxProbe1_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe6 = new QCheckBox(JudgeClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(20, 400, 35, 20));
		checkBoxProbe6->setMinimumSize(QSize(35, 20));
		checkBoxProbe6->setMaximumSize(QSize(35, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe29 = new QCheckBox(JudgeClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(130, 360, 35, 20));
		checkBoxProbe29->setMinimumSize(QSize(35, 20));
		checkBoxProbe29->setMaximumSize(QSize(35, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe7 = new QCheckBox(JudgeClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(20, 440, 35, 20));
		checkBoxProbe7->setMinimumSize(QSize(35, 20));
		checkBoxProbe7->setMaximumSize(QSize(35, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe12 = new QCheckBox(JudgeClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(20, 640, 35, 20));
		checkBoxProbe12->setMinimumSize(QSize(35, 20));
		checkBoxProbe12->setMaximumSize(QSize(35, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe4 = new QCheckBox(JudgeClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(20, 320, 35, 20));
		checkBoxProbe4->setMinimumSize(QSize(35, 20));
		checkBoxProbe4->setMaximumSize(QSize(35, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe8 = new QCheckBox(JudgeClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(20, 480, 35, 20));
		checkBoxProbe8->setMinimumSize(QSize(35, 20));
		checkBoxProbe8->setMaximumSize(QSize(35, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe24 = new QCheckBox(JudgeClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(70, 640, 35, 20));
		checkBoxProbe24->setMinimumSize(QSize(35, 20));
		checkBoxProbe24->setMaximumSize(QSize(35, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(JudgeClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(20, 600, 35, 20));
		checkBoxProbe11->setMinimumSize(QSize(35, 20));
		checkBoxProbe11->setMaximumSize(QSize(35, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe17 = new QCheckBox(JudgeClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(70, 360, 35, 20));
		checkBoxProbe17->setMinimumSize(QSize(35, 20));
		checkBoxProbe17->setMaximumSize(QSize(35, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe28 = new QCheckBox(JudgeClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(130, 320, 35, 20));
		checkBoxProbe28->setMinimumSize(QSize(35, 20));
		checkBoxProbe28->setMaximumSize(QSize(35, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe19 = new QCheckBox(JudgeClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(70, 440, 35, 20));
		checkBoxProbe19->setMinimumSize(QSize(35, 20));
		checkBoxProbe19->setMaximumSize(QSize(35, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe2 = new QCheckBox(JudgeClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(20, 240, 35, 20));
		checkBoxProbe2->setMinimumSize(QSize(35, 20));
		checkBoxProbe2->setMaximumSize(QSize(35, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe20 = new QCheckBox(JudgeClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(70, 480, 35, 20));
		checkBoxProbe20->setMinimumSize(QSize(35, 20));
		checkBoxProbe20->setMaximumSize(QSize(35, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(JudgeClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(20, 280, 35, 20));
		checkBoxProbe3->setMinimumSize(QSize(35, 20));
		checkBoxProbe3->setMaximumSize(QSize(35, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe26 = new QCheckBox(JudgeClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(130, 240, 35, 20));
		checkBoxProbe26->setMinimumSize(QSize(35, 20));
		checkBoxProbe26->setMaximumSize(QSize(35, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAny = new QRadioButton(JudgeClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(90, 149, 81, 16));
		radioButtonAny->setMinimumSize(QSize(81, 16));
		radioButtonAny->setMaximumSize(QSize(81, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe1 = new QCheckBox(JudgeClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(20, 200, 35, 20));
		checkBoxProbe1->setMinimumSize(QSize(35, 20));
		checkBoxProbe1->setMaximumSize(QSize(35, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe14 = new QCheckBox(JudgeClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(70, 240, 35, 20));
		checkBoxProbe14->setMinimumSize(QSize(35, 20));
		checkBoxProbe14->setMaximumSize(QSize(35, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe9 = new QCheckBox(JudgeClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(20, 520, 35, 20));
		checkBoxProbe9->setMinimumSize(QSize(35, 20));
		checkBoxProbe9->setMaximumSize(QSize(35, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe27 = new QCheckBox(JudgeClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(130, 280, 35, 20));
		checkBoxProbe27->setMinimumSize(QSize(35, 20));
		checkBoxProbe27->setMaximumSize(QSize(35, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe30 = new QCheckBox(JudgeClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(130, 400, 35, 20));
		checkBoxProbe30->setMinimumSize(QSize(35, 20));
		checkBoxProbe30->setMaximumSize(QSize(35, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe10 = new QCheckBox(JudgeClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(20, 560, 35, 20));
		checkBoxProbe10->setMinimumSize(QSize(35, 20));
		checkBoxProbe10->setMaximumSize(QSize(35, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe15 = new QCheckBox(JudgeClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(70, 280, 35, 20));
		checkBoxProbe15->setMinimumSize(QSize(35, 20));
		checkBoxProbe15->setMaximumSize(QSize(35, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25 = new QCheckBox(JudgeClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(130, 200, 35, 20));
		checkBoxProbe25->setMinimumSize(QSize(35, 20));
		checkBoxProbe25->setMaximumSize(QSize(35, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyI2 = new QPushButton(JudgeClass);
		pushButtonAnyI2->setObjectName(QString::fromUtf8("pushButtonAnyI2"));
		pushButtonAnyI2->setGeometry(QRect(890, 150, 50, 20));
		pushButtonAnyI2->setMinimumSize(QSize(50, 20));
		pushButtonAnyI2->setMaximumSize(QSize(50, 20));
		pushButtonAnyI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllI2 = new QPushButton(JudgeClass);
		pushButtonAllI2->setObjectName(QString::fromUtf8("pushButtonAllI2"));
		pushButtonAllI2->setGeometry(QRect(890, 110, 50, 20));
		pushButtonAllI2->setMinimumSize(QSize(50, 20));
		pushButtonAllI2->setMaximumSize(QSize(50, 20));
		pushButtonAllI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyB = new QPushButton(JudgeClass);
		pushButtonAnyB->setObjectName(QString::fromUtf8("pushButtonAnyB"));
		pushButtonAnyB->setGeometry(QRect(700, 150, 50, 20));
		pushButtonAnyB->setMinimumSize(QSize(50, 20));
		pushButtonAnyB->setMaximumSize(QSize(50, 20));
		pushButtonAnyB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllB = new QPushButton(JudgeClass);
		pushButtonAllB->setObjectName(QString::fromUtf8("pushButtonAllB"));
		pushButtonAllB->setGeometry(QRect(700, 110, 50, 20));
		pushButtonAllB->setMinimumSize(QSize(50, 20));
		pushButtonAllB->setMaximumSize(QSize(50, 20));
		pushButtonAllB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyA = new QPushButton(JudgeClass);
		pushButtonAnyA->setObjectName(QString::fromUtf8("pushButtonAnyA"));
		pushButtonAnyA->setGeometry(QRect(510, 150, 50, 20));
		pushButtonAnyA->setMinimumSize(QSize(50, 20));
		pushButtonAnyA->setMaximumSize(QSize(50, 20));
		pushButtonAnyA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllA = new QPushButton(JudgeClass);
		pushButtonAllA->setObjectName(QString::fromUtf8("pushButtonAllA"));
		pushButtonAllA->setGeometry(QRect(510, 110, 50, 20));
		pushButtonAllA->setMinimumSize(QSize(50, 20));
		pushButtonAllA->setMaximumSize(QSize(50, 20));
		pushButtonAllA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllI = new QPushButton(JudgeClass);
		pushButtonAllI->setObjectName(QString::fromUtf8("pushButtonAllI"));
		pushButtonAllI->setGeometry(QRect(320, 110, 50, 20));
		pushButtonAllI->setMinimumSize(QSize(50, 20));
		pushButtonAllI->setMaximumSize(QSize(50, 20));
		pushButtonAllI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyI = new QPushButton(JudgeClass);
		pushButtonAnyI->setObjectName(QString::fromUtf8("pushButtonAnyI"));
		pushButtonAnyI->setGeometry(QRect(320, 150, 50, 20));
		pushButtonAnyI->setMinimumSize(QSize(50, 20));
		pushButtonAnyI->setMaximumSize(QSize(50, 20));
		pushButtonAnyI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe36 = new QCheckBox(JudgeClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(130, 640, 35, 20));
		checkBoxProbe36->setMinimumSize(QSize(35, 20));
		checkBoxProbe36->setMaximumSize(QSize(35, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe35 = new QCheckBox(JudgeClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(130, 600, 35, 20));
		checkBoxProbe35->setMinimumSize(QSize(35, 20));
		checkBoxProbe35->setMaximumSize(QSize(35, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe32 = new QCheckBox(JudgeClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(130, 480, 35, 20));
		checkBoxProbe32->setMinimumSize(QSize(35, 20));
		checkBoxProbe32->setMaximumSize(QSize(35, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(JudgeClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(130, 520, 35, 20));
		checkBoxProbe33->setMinimumSize(QSize(35, 20));
		checkBoxProbe33->setMaximumSize(QSize(35, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(JudgeClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(130, 440, 35, 20));
		checkBoxProbe31->setMinimumSize(QSize(35, 20));
		checkBoxProbe31->setMaximumSize(QSize(35, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(JudgeClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(130, 560, 35, 20));
		checkBoxProbe34->setMinimumSize(QSize(35, 20));
		checkBoxProbe34->setMaximumSize(QSize(35, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		comboBoxAProbe11 = new QComboBox(JudgeClass);
		comboBoxAProbe11->setObjectName(QString::fromUtf8("comboBoxAProbe11"));
		comboBoxAProbe11->setEnabled(false);
		comboBoxAProbe11->setGeometry(QRect(380, 600, 120, 20));
		comboBoxAProbe11->setMinimumSize(QSize(120, 20));
		comboBoxAProbe11->setMaximumSize(QSize(120, 20));
		comboBoxAProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxAProbe12 = new QComboBox(JudgeClass);
		comboBoxAProbe12->setObjectName(QString::fromUtf8("comboBoxAProbe12"));
		comboBoxAProbe12->setEnabled(false);
		comboBoxAProbe12->setGeometry(QRect(380, 640, 120, 20));
		comboBoxAProbe12->setMinimumSize(QSize(120, 20));
		comboBoxAProbe12->setMaximumSize(QSize(120, 20));
		comboBoxAProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe11 = new QComboBox(JudgeClass);
		comboBoxBProbe11->setObjectName(QString::fromUtf8("comboBoxBProbe11"));
		comboBoxBProbe11->setEnabled(false);
		comboBoxBProbe11->setGeometry(QRect(570, 600, 120, 20));
		comboBoxBProbe11->setMinimumSize(QSize(120, 20));
		comboBoxBProbe11->setMaximumSize(QSize(120, 20));
		comboBoxBProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe11 = new QComboBox(JudgeClass);
		comboBoxI2Probe11->setObjectName(QString::fromUtf8("comboBoxI2Probe11"));
		comboBoxI2Probe11->setEnabled(false);
		comboBoxI2Probe11->setGeometry(QRect(760, 600, 120, 20));
		comboBoxI2Probe11->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe11->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe12 = new QComboBox(JudgeClass);
		comboBoxIProbe12->setObjectName(QString::fromUtf8("comboBoxIProbe12"));
		comboBoxIProbe12->setEnabled(false);
		comboBoxIProbe12->setGeometry(QRect(190, 640, 120, 20));
		comboBoxIProbe12->setMinimumSize(QSize(120, 20));
		comboBoxIProbe12->setMaximumSize(QSize(120, 20));
		comboBoxIProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxI2Probe12 = new QComboBox(JudgeClass);
		comboBoxI2Probe12->setObjectName(QString::fromUtf8("comboBoxI2Probe12"));
		comboBoxI2Probe12->setEnabled(false);
		comboBoxI2Probe12->setGeometry(QRect(760, 640, 120, 20));
		comboBoxI2Probe12->setMinimumSize(QSize(120, 20));
		comboBoxI2Probe12->setMaximumSize(QSize(120, 20));
		comboBoxI2Probe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxBProbe12 = new QComboBox(JudgeClass);
		comboBoxBProbe12->setObjectName(QString::fromUtf8("comboBoxBProbe12"));
		comboBoxBProbe12->setEnabled(false);
		comboBoxBProbe12->setGeometry(QRect(570, 640, 120, 20));
		comboBoxBProbe12->setMinimumSize(QSize(120, 20));
		comboBoxBProbe12->setMaximumSize(QSize(120, 20));
		comboBoxBProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxIProbe11 = new QComboBox(JudgeClass);
		comboBoxIProbe11->setObjectName(QString::fromUtf8("comboBoxIProbe11"));
		comboBoxIProbe11->setEnabled(false);
		comboBoxIProbe11->setGeometry(QRect(190, 600, 120, 20));
		comboBoxIProbe11->setMinimumSize(QSize(120, 20));
		comboBoxIProbe11->setMaximumSize(QSize(120, 20));
		comboBoxIProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));

		retranslateUi(JudgeClass);

		QMetaObject::connectSlotsByName(JudgeClass);
	} // setupUi

	void retranslateUi(QDialog *JudgeClass)
	{
		JudgeClass->setWindowTitle(QApplication::translate("JudgeClass", "Judge", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("JudgeClass", "\351\227\270\351\227\250\345\274\200\345\205\263\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("JudgeClass", "\351\227\270\351\227\250B", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("JudgeClass", "\351\227\270\351\227\250A", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("JudgeClass", "\351\227\270\351\227\250I2", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("JudgeClass", "\351\227\270\351\227\250I", 0, QApplication::UnicodeUTF8));
		comboBoxAllI->clear();
		comboBoxAllI->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAnyI->clear();
		comboBoxAnyI->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAnyA->clear();
		comboBoxAnyA->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAllA->clear();
		comboBoxAllA->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAllB->clear();
		comboBoxAllB->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAnyB->clear();
		comboBoxAnyB->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAnyI2->clear();
		comboBoxAnyI2->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAllI2->clear();
		comboBoxAllI2->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe1->clear();
		comboBoxI2Probe1->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe2->clear();
		comboBoxIProbe2->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe1->clear();
		comboBoxIProbe1->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe2->clear();
		comboBoxBProbe2->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe1->clear();
		comboBoxAProbe1->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe2->clear();
		comboBoxAProbe2->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe1->clear();
		comboBoxBProbe1->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe2->clear();
		comboBoxI2Probe2->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe4->clear();
		comboBoxI2Probe4->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe3->clear();
		comboBoxI2Probe3->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe4->clear();
		comboBoxAProbe4->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe4->clear();
		comboBoxBProbe4->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe3->clear();
		comboBoxBProbe3->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe3->clear();
		comboBoxAProbe3->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe3->clear();
		comboBoxIProbe3->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe4->clear();
		comboBoxIProbe4->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe8->clear();
		comboBoxIProbe8->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe5->clear();
		comboBoxI2Probe5->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe6->clear();
		comboBoxAProbe6->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe6->clear();
		comboBoxI2Probe6->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe8->clear();
		comboBoxBProbe8->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe6->clear();
		comboBoxBProbe6->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe7->clear();
		comboBoxIProbe7->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe8->clear();
		comboBoxAProbe8->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe5->clear();
		comboBoxBProbe5->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe5->clear();
		comboBoxAProbe5->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe7->clear();
		comboBoxAProbe7->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe5->clear();
		comboBoxIProbe5->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe7->clear();
		comboBoxBProbe7->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe8->clear();
		comboBoxI2Probe8->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe6->clear();
		comboBoxIProbe6->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe7->clear();
		comboBoxI2Probe7->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe9->clear();
		comboBoxI2Probe9->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe9->clear();
		comboBoxBProbe9->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe0->clear();
		comboBoxBProbe0->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe0->clear();
		comboBoxIProbe0->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe9->clear();
		comboBoxAProbe9->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe0->clear();
		comboBoxI2Probe0->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe9->clear();
		comboBoxIProbe9->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe0->clear();
		comboBoxAProbe0->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		pushButtonYes->setText(QApplication::translate("JudgeClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("JudgeClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("JudgeClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("JudgeClass", "18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("JudgeClass", "22", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("JudgeClass", "16", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("JudgeClass", "23", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("JudgeClass", "21", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("JudgeClass", "13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11_20->setText(QApplication::translate("JudgeClass", "13~24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("JudgeClass", "5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21_30->setText(QApplication::translate("JudgeClass", "25~36", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1_10->setText(QApplication::translate("JudgeClass", "1~12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("JudgeClass", "6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("JudgeClass", "29", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("JudgeClass", "7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("JudgeClass", "12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("JudgeClass", "4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("JudgeClass", "8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("JudgeClass", "24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("JudgeClass", "11", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("JudgeClass", "17", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("JudgeClass", "28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("JudgeClass", "19", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("JudgeClass", "2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("JudgeClass", "20", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("JudgeClass", "3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("JudgeClass", "26", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("JudgeClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("JudgeClass", "1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("JudgeClass", "14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("JudgeClass", "9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("JudgeClass", "27", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("JudgeClass", "30", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("JudgeClass", "10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("JudgeClass", "15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("JudgeClass", "25", 0, QApplication::UnicodeUTF8));
		pushButtonAnyI2->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllI2->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyB->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllB->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyA->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllA->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllI->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyI->setText(QApplication::translate("JudgeClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("JudgeClass", "36", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("JudgeClass", "35", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("JudgeClass", "32", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("JudgeClass", "33", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("JudgeClass", "31", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("JudgeClass", "34", 0, QApplication::UnicodeUTF8));
		comboBoxAProbe11->clear();
		comboBoxAProbe11->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxAProbe12->clear();
		comboBoxAProbe12->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe11->clear();
		comboBoxBProbe11->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe11->clear();
		comboBoxI2Probe11->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe12->clear();
		comboBoxIProbe12->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxI2Probe12->clear();
		comboBoxI2Probe12->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxBProbe12->clear();
		comboBoxBProbe12->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
		comboBoxIProbe11->clear();
		comboBoxIProbe11->insertItems(0, QStringList()
			<< QApplication::translate("JudgeClass", "      \345\205\263\351\227\255", 0, QApplication::UnicodeUTF8)
			<< QApplication::translate("JudgeClass", "      \345\274\200\345\220\257", 0, QApplication::UnicodeUTF8)
			);
	} // retranslateUi

};

namespace Ui {
	class JudgeClass: public Ui_JudgeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JUDGE_H





/********************************************************************************
** Form generated from reading UI file 'probeset.ui'
**
** Created: Thu Jun 19 10:46:41 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROBESET_H
#define UI_PROBESET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ProbeSetClass
{
public:
	QLabel *label_2;
	QLabel *label;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QLineEdit *lineEditPro1Hz;
	QLineEdit *lineEditPro2Hz;
	QLineEdit *lineEditPro3Hz;
	QLineEdit *lineEditPro4Hz;
	QLineEdit *lineEditPro8Hz;
	QLineEdit *lineEditPro7Hz;
	QLineEdit *lineEditPro5Hz;
	QLineEdit *lineEditPro6Hz;
	QLineEdit *lineEditPro10Hz;
	QLineEdit *lineEditPro9Hz;
	QLineEdit *lineEditPro19Hz;
	QLineEdit *lineEditPro17Hz;
	QLineEdit *lineEditPro16Hz;
	QLineEdit *lineEditPro20Hz;
	QLineEdit *lineEditPro11Hz;
	QLineEdit *lineEditPro15Hz;
	QLineEdit *lineEditPro14Hz;
	QLineEdit *lineEditPro12Hz;
	QLineEdit *lineEditPro13Hz;
	QLineEdit *lineEditPro18Hz;
	QLineEdit *lineEditPro28Hz;
	QLineEdit *lineEditPro23Hz;
	QLineEdit *lineEditPro26Hz;
	QLineEdit *lineEditPro25Hz;
	QLineEdit *lineEditPro27Hz;
	QLineEdit *lineEditPro24Hz;
	QLineEdit *lineEditPro30Hz;
	QLineEdit *lineEditPro21Hz;
	QLineEdit *lineEditPro22Hz;
	QLineEdit *lineEditPro29Hz;
	QPushButton *pushButtonNo;
	QPushButton *pushButtonYes;
	QLabel *label_6;
	QLabel *label_7;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QLineEdit *lineEditPro9Cor;
	QLineEdit *lineEditPro4Cor;
	QLineEdit *lineEditPro3Cor;
	QLineEdit *lineEditPro6Cor;
	QLineEdit *lineEditPro1Cor;
	QLineEdit *lineEditPro10Cor;
	QLineEdit *lineEditPro5Cor;
	QLineEdit *lineEditPro2Cor;
	QLineEdit *lineEditPro8Cor;
	QLineEdit *lineEditPro7Cor;
	QLineEdit *lineEditPro18Cor;
	QLineEdit *lineEditPro15Cor;
	QLineEdit *lineEditPro14Cor;
	QLineEdit *lineEditPro20Cor;
	QLineEdit *lineEditPro17Cor;
	QLineEdit *lineEditPro11Cor;
	QLineEdit *lineEditPro12Cor;
	QLineEdit *lineEditPro19Cor;
	QLineEdit *lineEditPro16Cor;
	QLineEdit *lineEditPro13Cor;
	QLineEdit *lineEditPro29Cor;
	QLineEdit *lineEditPro22Cor;
	QLineEdit *lineEditPro28Cor;
	QLineEdit *lineEditPro21Cor;
	QLineEdit *lineEditPro30Cor;
	QLineEdit *lineEditPro23Cor;
	QLineEdit *lineEditPro25Cor;
	QLineEdit *lineEditPro24Cor;
	QLineEdit *lineEditPro27Cor;
	QLineEdit *lineEditPro26Cor;
	QLabel *label_37;
	QLabel *label_14;
	QLabel *label_39;
	QLabel *label_38;
	QLabel *label_34;
	QLabel *label_41;
	QLabel *label_15;
	QLabel *label_36;
	QLabel *label_40;
	QLabel *label_35;
	QLabel *label_49;
	QLabel *label_50;
	QLabel *label_46;
	QLabel *label_45;
	QLabel *label_44;
	QLabel *label_48;
	QLabel *label_51;
	QLabel *label_47;
	QLabel *label_43;
	QLabel *label_42;
	QLabel *label_54;
	QLabel *label_57;
	QLabel *label_61;
	QLabel *label_60;
	QLabel *label_53;
	QLabel *label_52;
	QLabel *label_59;
	QLabel *label_56;
	QLabel *label_55;
	QLabel *label_58;
	QLineEdit *lineEditProbe1;
	QLineEdit *lineEditProbe2;
	QLineEdit *lineEditProbe3;
	QLineEdit *lineEditProbe4;
	QLineEdit *lineEditProbe5;
	QLabel *label_16;
	QLabel *label_17;
	QLabel *label_18;
	QLabel *label_19;
	QLabel *label_20;
	QLabel *label_21;
	QLabel *label_22;
	QLabel *label_23;
	QLabel *label_24;
	QLabel *label_25;
	QLabel *label_26;
	QRadioButton *radioButtonSingle;
	QRadioButton *radioButtonDouble;
	QLabel *label_27;
	QLabel *label_28;
	QLineEdit *lineEditStrangePoint;
	QLineEdit *lineEditPro36Cor;
	QLineEdit *lineEditPro32Hz;
	QLineEdit *lineEditPro31Hz;
	QLineEdit *lineEditPro32Cor;
	QLineEdit *lineEditPro34Cor;
	QLineEdit *lineEditPro35Cor;
	QLineEdit *lineEditPro33Hz;
	QLineEdit *lineEditPro34Hz;
	QLabel *label_62;
	QLabel *label_63;
	QLineEdit *lineEditPro33Cor;
	QLabel *label_64;
	QLabel *label_65;
	QLabel *label_66;
	QLineEdit *lineEditPro36Hz;
	QLineEdit *lineEditPro31Cor;
	QLineEdit *lineEditPro35Hz;
	QLabel *label_67;
	QLabel *label_29;
	QLabel *label_30;
	QLineEdit *lineEditProbe6;

	void setupUi(QDialog *ProbeSetClass)
	{
		if (ProbeSetClass->objectName().isEmpty())
			ProbeSetClass->setObjectName(QString::fromUtf8("ProbeSetClass"));
		ProbeSetClass->setEnabled(true);
		ProbeSetClass->resize(800, 650);
		ProbeSetClass->setMinimumSize(QSize(800, 650));
		ProbeSetClass->setMaximumSize(QSize(800, 650));
		ProbeSetClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label_2 = new QLabel(ProbeSetClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(320, 20, 120, 20));
		label_2->setMinimumSize(QSize(120, 20));
		label_2->setMaximumSize(QSize(120, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		label = new QLabel(ProbeSetClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(70, 70, 35, 18));
		label->setMinimumSize(QSize(35, 18));
		label->setMaximumSize(QSize(35, 18));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_3 = new QLabel(ProbeSetClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(120, 70, 50, 18));
		label_3->setMinimumSize(QSize(50, 18));
		label_3->setMaximumSize(QSize(50, 18));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(ProbeSetClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(120, 90, 50, 15));
		label_4->setMinimumSize(QSize(50, 15));
		label_4->setMaximumSize(QSize(50, 15));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_5 = new QLabel(ProbeSetClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(70, 90, 35, 15));
		label_5->setMinimumSize(QSize(35, 15));
		label_5->setMaximumSize(QSize(35, 15));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditPro1Hz = new QLineEdit(ProbeSetClass);
		lineEditPro1Hz->setObjectName(QString::fromUtf8("lineEditPro1Hz"));
		lineEditPro1Hz->setEnabled(false);
		lineEditPro1Hz->setGeometry(QRect(60, 120, 50, 20));
		lineEditPro1Hz->setMinimumSize(QSize(50, 20));
		lineEditPro1Hz->setMaximumSize(QSize(50, 20));
		lineEditPro1Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro2Hz = new QLineEdit(ProbeSetClass);
		lineEditPro2Hz->setObjectName(QString::fromUtf8("lineEditPro2Hz"));
		lineEditPro2Hz->setEnabled(false);
		lineEditPro2Hz->setGeometry(QRect(60, 160, 50, 20));
		lineEditPro2Hz->setMinimumSize(QSize(50, 20));
		lineEditPro2Hz->setMaximumSize(QSize(50, 20));
		lineEditPro2Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro3Hz = new QLineEdit(ProbeSetClass);
		lineEditPro3Hz->setObjectName(QString::fromUtf8("lineEditPro3Hz"));
		lineEditPro3Hz->setEnabled(false);
		lineEditPro3Hz->setGeometry(QRect(60, 200, 50, 20));
		lineEditPro3Hz->setMinimumSize(QSize(50, 20));
		lineEditPro3Hz->setMaximumSize(QSize(50, 20));
		lineEditPro3Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro4Hz = new QLineEdit(ProbeSetClass);
		lineEditPro4Hz->setObjectName(QString::fromUtf8("lineEditPro4Hz"));
		lineEditPro4Hz->setEnabled(false);
		lineEditPro4Hz->setGeometry(QRect(60, 240, 50, 20));
		lineEditPro4Hz->setMinimumSize(QSize(50, 20));
		lineEditPro4Hz->setMaximumSize(QSize(50, 20));
		lineEditPro4Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro8Hz = new QLineEdit(ProbeSetClass);
		lineEditPro8Hz->setObjectName(QString::fromUtf8("lineEditPro8Hz"));
		lineEditPro8Hz->setEnabled(false);
		lineEditPro8Hz->setGeometry(QRect(60, 400, 50, 20));
		lineEditPro8Hz->setMinimumSize(QSize(50, 20));
		lineEditPro8Hz->setMaximumSize(QSize(50, 20));
		lineEditPro8Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro7Hz = new QLineEdit(ProbeSetClass);
		lineEditPro7Hz->setObjectName(QString::fromUtf8("lineEditPro7Hz"));
		lineEditPro7Hz->setEnabled(false);
		lineEditPro7Hz->setGeometry(QRect(60, 360, 50, 20));
		lineEditPro7Hz->setMinimumSize(QSize(50, 20));
		lineEditPro7Hz->setMaximumSize(QSize(50, 20));
		lineEditPro7Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro5Hz = new QLineEdit(ProbeSetClass);
		lineEditPro5Hz->setObjectName(QString::fromUtf8("lineEditPro5Hz"));
		lineEditPro5Hz->setEnabled(false);
		lineEditPro5Hz->setGeometry(QRect(60, 280, 50, 20));
		lineEditPro5Hz->setMinimumSize(QSize(50, 20));
		lineEditPro5Hz->setMaximumSize(QSize(50, 20));
		lineEditPro5Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro6Hz = new QLineEdit(ProbeSetClass);
		lineEditPro6Hz->setObjectName(QString::fromUtf8("lineEditPro6Hz"));
		lineEditPro6Hz->setEnabled(false);
		lineEditPro6Hz->setGeometry(QRect(60, 320, 50, 20));
		lineEditPro6Hz->setMinimumSize(QSize(50, 20));
		lineEditPro6Hz->setMaximumSize(QSize(50, 20));
		lineEditPro6Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro10Hz = new QLineEdit(ProbeSetClass);
		lineEditPro10Hz->setObjectName(QString::fromUtf8("lineEditPro10Hz"));
		lineEditPro10Hz->setEnabled(false);
		lineEditPro10Hz->setGeometry(QRect(60, 480, 50, 20));
		lineEditPro10Hz->setMinimumSize(QSize(50, 20));
		lineEditPro10Hz->setMaximumSize(QSize(50, 20));
		lineEditPro10Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro9Hz = new QLineEdit(ProbeSetClass);
		lineEditPro9Hz->setObjectName(QString::fromUtf8("lineEditPro9Hz"));
		lineEditPro9Hz->setEnabled(false);
		lineEditPro9Hz->setGeometry(QRect(60, 440, 50, 20));
		lineEditPro9Hz->setMinimumSize(QSize(50, 20));
		lineEditPro9Hz->setMaximumSize(QSize(50, 20));
		lineEditPro9Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro19Hz = new QLineEdit(ProbeSetClass);
		lineEditPro19Hz->setObjectName(QString::fromUtf8("lineEditPro19Hz"));
		lineEditPro19Hz->setEnabled(false);
		lineEditPro19Hz->setGeometry(QRect(250, 360, 50, 20));
		lineEditPro19Hz->setMinimumSize(QSize(50, 20));
		lineEditPro19Hz->setMaximumSize(QSize(50, 20));
		lineEditPro19Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro17Hz = new QLineEdit(ProbeSetClass);
		lineEditPro17Hz->setObjectName(QString::fromUtf8("lineEditPro17Hz"));
		lineEditPro17Hz->setEnabled(false);
		lineEditPro17Hz->setGeometry(QRect(250, 280, 50, 20));
		lineEditPro17Hz->setMinimumSize(QSize(50, 20));
		lineEditPro17Hz->setMaximumSize(QSize(50, 20));
		lineEditPro17Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro16Hz = new QLineEdit(ProbeSetClass);
		lineEditPro16Hz->setObjectName(QString::fromUtf8("lineEditPro16Hz"));
		lineEditPro16Hz->setEnabled(false);
		lineEditPro16Hz->setGeometry(QRect(250, 240, 50, 20));
		lineEditPro16Hz->setMinimumSize(QSize(50, 20));
		lineEditPro16Hz->setMaximumSize(QSize(50, 20));
		lineEditPro16Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro20Hz = new QLineEdit(ProbeSetClass);
		lineEditPro20Hz->setObjectName(QString::fromUtf8("lineEditPro20Hz"));
		lineEditPro20Hz->setEnabled(false);
		lineEditPro20Hz->setGeometry(QRect(250, 400, 50, 20));
		lineEditPro20Hz->setMinimumSize(QSize(50, 20));
		lineEditPro20Hz->setMaximumSize(QSize(50, 20));
		lineEditPro20Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro11Hz = new QLineEdit(ProbeSetClass);
		lineEditPro11Hz->setObjectName(QString::fromUtf8("lineEditPro11Hz"));
		lineEditPro11Hz->setEnabled(false);
		lineEditPro11Hz->setGeometry(QRect(60, 520, 50, 20));
		lineEditPro11Hz->setMinimumSize(QSize(50, 20));
		lineEditPro11Hz->setMaximumSize(QSize(50, 20));
		lineEditPro11Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro15Hz = new QLineEdit(ProbeSetClass);
		lineEditPro15Hz->setObjectName(QString::fromUtf8("lineEditPro15Hz"));
		lineEditPro15Hz->setEnabled(false);
		lineEditPro15Hz->setGeometry(QRect(250, 200, 50, 20));
		lineEditPro15Hz->setMinimumSize(QSize(50, 20));
		lineEditPro15Hz->setMaximumSize(QSize(50, 20));
		lineEditPro15Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro14Hz = new QLineEdit(ProbeSetClass);
		lineEditPro14Hz->setObjectName(QString::fromUtf8("lineEditPro14Hz"));
		lineEditPro14Hz->setEnabled(false);
		lineEditPro14Hz->setGeometry(QRect(250, 160, 50, 20));
		lineEditPro14Hz->setMinimumSize(QSize(50, 20));
		lineEditPro14Hz->setMaximumSize(QSize(50, 20));
		lineEditPro14Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro12Hz = new QLineEdit(ProbeSetClass);
		lineEditPro12Hz->setObjectName(QString::fromUtf8("lineEditPro12Hz"));
		lineEditPro12Hz->setEnabled(false);
		lineEditPro12Hz->setGeometry(QRect(60, 560, 50, 20));
		lineEditPro12Hz->setMinimumSize(QSize(50, 20));
		lineEditPro12Hz->setMaximumSize(QSize(50, 20));
		lineEditPro12Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro13Hz = new QLineEdit(ProbeSetClass);
		lineEditPro13Hz->setObjectName(QString::fromUtf8("lineEditPro13Hz"));
		lineEditPro13Hz->setEnabled(false);
		lineEditPro13Hz->setGeometry(QRect(250, 120, 50, 20));
		lineEditPro13Hz->setMinimumSize(QSize(50, 20));
		lineEditPro13Hz->setMaximumSize(QSize(50, 20));
		lineEditPro13Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro18Hz = new QLineEdit(ProbeSetClass);
		lineEditPro18Hz->setObjectName(QString::fromUtf8("lineEditPro18Hz"));
		lineEditPro18Hz->setEnabled(false);
		lineEditPro18Hz->setGeometry(QRect(250, 320, 50, 20));
		lineEditPro18Hz->setMinimumSize(QSize(50, 20));
		lineEditPro18Hz->setMaximumSize(QSize(50, 20));
		lineEditPro18Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro28Hz = new QLineEdit(ProbeSetClass);
		lineEditPro28Hz->setObjectName(QString::fromUtf8("lineEditPro28Hz"));
		lineEditPro28Hz->setEnabled(false);
		lineEditPro28Hz->setGeometry(QRect(440, 240, 50, 20));
		lineEditPro28Hz->setMinimumSize(QSize(50, 20));
		lineEditPro28Hz->setMaximumSize(QSize(50, 20));
		lineEditPro28Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro23Hz = new QLineEdit(ProbeSetClass);
		lineEditPro23Hz->setObjectName(QString::fromUtf8("lineEditPro23Hz"));
		lineEditPro23Hz->setEnabled(false);
		lineEditPro23Hz->setGeometry(QRect(250, 520, 50, 20));
		lineEditPro23Hz->setMinimumSize(QSize(50, 20));
		lineEditPro23Hz->setMaximumSize(QSize(50, 20));
		lineEditPro23Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro26Hz = new QLineEdit(ProbeSetClass);
		lineEditPro26Hz->setObjectName(QString::fromUtf8("lineEditPro26Hz"));
		lineEditPro26Hz->setEnabled(false);
		lineEditPro26Hz->setGeometry(QRect(440, 160, 50, 20));
		lineEditPro26Hz->setMinimumSize(QSize(50, 20));
		lineEditPro26Hz->setMaximumSize(QSize(50, 20));
		lineEditPro26Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro25Hz = new QLineEdit(ProbeSetClass);
		lineEditPro25Hz->setObjectName(QString::fromUtf8("lineEditPro25Hz"));
		lineEditPro25Hz->setEnabled(false);
		lineEditPro25Hz->setGeometry(QRect(440, 120, 50, 20));
		lineEditPro25Hz->setMinimumSize(QSize(50, 20));
		lineEditPro25Hz->setMaximumSize(QSize(50, 20));
		lineEditPro25Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro27Hz = new QLineEdit(ProbeSetClass);
		lineEditPro27Hz->setObjectName(QString::fromUtf8("lineEditPro27Hz"));
		lineEditPro27Hz->setEnabled(false);
		lineEditPro27Hz->setGeometry(QRect(440, 200, 50, 20));
		lineEditPro27Hz->setMinimumSize(QSize(50, 20));
		lineEditPro27Hz->setMaximumSize(QSize(50, 20));
		lineEditPro27Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro24Hz = new QLineEdit(ProbeSetClass);
		lineEditPro24Hz->setObjectName(QString::fromUtf8("lineEditPro24Hz"));
		lineEditPro24Hz->setEnabled(false);
		lineEditPro24Hz->setGeometry(QRect(250, 560, 50, 20));
		lineEditPro24Hz->setMinimumSize(QSize(50, 20));
		lineEditPro24Hz->setMaximumSize(QSize(50, 20));
		lineEditPro24Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro30Hz = new QLineEdit(ProbeSetClass);
		lineEditPro30Hz->setObjectName(QString::fromUtf8("lineEditPro30Hz"));
		lineEditPro30Hz->setEnabled(false);
		lineEditPro30Hz->setGeometry(QRect(440, 320, 50, 20));
		lineEditPro30Hz->setMinimumSize(QSize(50, 20));
		lineEditPro30Hz->setMaximumSize(QSize(50, 20));
		lineEditPro30Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro21Hz = new QLineEdit(ProbeSetClass);
		lineEditPro21Hz->setObjectName(QString::fromUtf8("lineEditPro21Hz"));
		lineEditPro21Hz->setEnabled(false);
		lineEditPro21Hz->setGeometry(QRect(250, 440, 50, 20));
		lineEditPro21Hz->setMinimumSize(QSize(50, 20));
		lineEditPro21Hz->setMaximumSize(QSize(50, 20));
		lineEditPro21Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro22Hz = new QLineEdit(ProbeSetClass);
		lineEditPro22Hz->setObjectName(QString::fromUtf8("lineEditPro22Hz"));
		lineEditPro22Hz->setEnabled(false);
		lineEditPro22Hz->setGeometry(QRect(250, 480, 50, 20));
		lineEditPro22Hz->setMinimumSize(QSize(50, 20));
		lineEditPro22Hz->setMaximumSize(QSize(50, 20));
		lineEditPro22Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro29Hz = new QLineEdit(ProbeSetClass);
		lineEditPro29Hz->setObjectName(QString::fromUtf8("lineEditPro29Hz"));
		lineEditPro29Hz->setEnabled(false);
		lineEditPro29Hz->setGeometry(QRect(440, 280, 50, 20));
		lineEditPro29Hz->setMinimumSize(QSize(50, 20));
		lineEditPro29Hz->setMaximumSize(QSize(50, 20));
		lineEditPro29Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonNo = new QPushButton(ProbeSetClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(690, 600, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(ProbeSetClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(590, 600, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_6 = new QLabel(ProbeSetClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(260, 90, 35, 15));
		label_6->setMinimumSize(QSize(35, 15));
		label_6->setMaximumSize(QSize(35, 15));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(ProbeSetClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(260, 70, 35, 18));
		label_7->setMinimumSize(QSize(35, 18));
		label_7->setMaximumSize(QSize(35, 18));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(ProbeSetClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(310, 90, 50, 15));
		label_8->setMinimumSize(QSize(50, 15));
		label_8->setMaximumSize(QSize(50, 15));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(ProbeSetClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(310, 70, 50, 18));
		label_9->setMinimumSize(QSize(50, 18));
		label_9->setMaximumSize(QSize(50, 18));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(ProbeSetClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(500, 70, 50, 18));
		label_10->setMinimumSize(QSize(50, 18));
		label_10->setMaximumSize(QSize(50, 18));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_11 = new QLabel(ProbeSetClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(450, 70, 35, 18));
		label_11->setMinimumSize(QSize(35, 18));
		label_11->setMaximumSize(QSize(35, 18));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_12 = new QLabel(ProbeSetClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(500, 90, 50, 15));
		label_12->setMinimumSize(QSize(50, 15));
		label_12->setMaximumSize(QSize(50, 15));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_13 = new QLabel(ProbeSetClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(450, 90, 35, 15));
		label_13->setMinimumSize(QSize(35, 15));
		label_13->setMaximumSize(QSize(35, 15));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditPro9Cor = new QLineEdit(ProbeSetClass);
		lineEditPro9Cor->setObjectName(QString::fromUtf8("lineEditPro9Cor"));
		lineEditPro9Cor->setEnabled(false);
		lineEditPro9Cor->setGeometry(QRect(120, 440, 50, 20));
		lineEditPro9Cor->setMinimumSize(QSize(50, 20));
		lineEditPro9Cor->setMaximumSize(QSize(50, 20));
		lineEditPro9Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro4Cor = new QLineEdit(ProbeSetClass);
		lineEditPro4Cor->setObjectName(QString::fromUtf8("lineEditPro4Cor"));
		lineEditPro4Cor->setEnabled(false);
		lineEditPro4Cor->setGeometry(QRect(120, 240, 50, 20));
		lineEditPro4Cor->setMinimumSize(QSize(50, 20));
		lineEditPro4Cor->setMaximumSize(QSize(50, 20));
		lineEditPro4Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro3Cor = new QLineEdit(ProbeSetClass);
		lineEditPro3Cor->setObjectName(QString::fromUtf8("lineEditPro3Cor"));
		lineEditPro3Cor->setEnabled(false);
		lineEditPro3Cor->setGeometry(QRect(120, 200, 50, 20));
		lineEditPro3Cor->setMinimumSize(QSize(50, 20));
		lineEditPro3Cor->setMaximumSize(QSize(50, 20));
		lineEditPro3Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro6Cor = new QLineEdit(ProbeSetClass);
		lineEditPro6Cor->setObjectName(QString::fromUtf8("lineEditPro6Cor"));
		lineEditPro6Cor->setEnabled(false);
		lineEditPro6Cor->setGeometry(QRect(120, 320, 50, 20));
		lineEditPro6Cor->setMinimumSize(QSize(50, 20));
		lineEditPro6Cor->setMaximumSize(QSize(50, 20));
		lineEditPro6Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro1Cor = new QLineEdit(ProbeSetClass);
		lineEditPro1Cor->setObjectName(QString::fromUtf8("lineEditPro1Cor"));
		lineEditPro1Cor->setEnabled(false);
		lineEditPro1Cor->setGeometry(QRect(120, 120, 50, 20));
		lineEditPro1Cor->setMinimumSize(QSize(50, 20));
		lineEditPro1Cor->setMaximumSize(QSize(50, 20));
		lineEditPro1Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro10Cor = new QLineEdit(ProbeSetClass);
		lineEditPro10Cor->setObjectName(QString::fromUtf8("lineEditPro10Cor"));
		lineEditPro10Cor->setEnabled(false);
		lineEditPro10Cor->setGeometry(QRect(120, 480, 50, 20));
		lineEditPro10Cor->setMinimumSize(QSize(50, 20));
		lineEditPro10Cor->setMaximumSize(QSize(50, 20));
		lineEditPro10Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro5Cor = new QLineEdit(ProbeSetClass);
		lineEditPro5Cor->setObjectName(QString::fromUtf8("lineEditPro5Cor"));
		lineEditPro5Cor->setEnabled(false);
		lineEditPro5Cor->setGeometry(QRect(120, 280, 50, 20));
		lineEditPro5Cor->setMinimumSize(QSize(50, 20));
		lineEditPro5Cor->setMaximumSize(QSize(50, 20));
		lineEditPro5Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro2Cor = new QLineEdit(ProbeSetClass);
		lineEditPro2Cor->setObjectName(QString::fromUtf8("lineEditPro2Cor"));
		lineEditPro2Cor->setEnabled(false);
		lineEditPro2Cor->setGeometry(QRect(120, 160, 50, 20));
		lineEditPro2Cor->setMinimumSize(QSize(50, 20));
		lineEditPro2Cor->setMaximumSize(QSize(50, 20));
		lineEditPro2Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro8Cor = new QLineEdit(ProbeSetClass);
		lineEditPro8Cor->setObjectName(QString::fromUtf8("lineEditPro8Cor"));
		lineEditPro8Cor->setEnabled(false);
		lineEditPro8Cor->setGeometry(QRect(120, 400, 50, 20));
		lineEditPro8Cor->setMinimumSize(QSize(50, 20));
		lineEditPro8Cor->setMaximumSize(QSize(50, 20));
		lineEditPro8Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro7Cor = new QLineEdit(ProbeSetClass);
		lineEditPro7Cor->setObjectName(QString::fromUtf8("lineEditPro7Cor"));
		lineEditPro7Cor->setEnabled(false);
		lineEditPro7Cor->setGeometry(QRect(120, 360, 50, 20));
		lineEditPro7Cor->setMinimumSize(QSize(50, 20));
		lineEditPro7Cor->setMaximumSize(QSize(50, 20));
		lineEditPro7Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro18Cor = new QLineEdit(ProbeSetClass);
		lineEditPro18Cor->setObjectName(QString::fromUtf8("lineEditPro18Cor"));
		lineEditPro18Cor->setEnabled(false);
		lineEditPro18Cor->setGeometry(QRect(310, 320, 50, 20));
		lineEditPro18Cor->setMinimumSize(QSize(50, 20));
		lineEditPro18Cor->setMaximumSize(QSize(50, 20));
		lineEditPro18Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro15Cor = new QLineEdit(ProbeSetClass);
		lineEditPro15Cor->setObjectName(QString::fromUtf8("lineEditPro15Cor"));
		lineEditPro15Cor->setEnabled(false);
		lineEditPro15Cor->setGeometry(QRect(310, 200, 50, 20));
		lineEditPro15Cor->setMinimumSize(QSize(50, 20));
		lineEditPro15Cor->setMaximumSize(QSize(50, 20));
		lineEditPro15Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro14Cor = new QLineEdit(ProbeSetClass);
		lineEditPro14Cor->setObjectName(QString::fromUtf8("lineEditPro14Cor"));
		lineEditPro14Cor->setEnabled(false);
		lineEditPro14Cor->setGeometry(QRect(310, 160, 50, 20));
		lineEditPro14Cor->setMinimumSize(QSize(50, 20));
		lineEditPro14Cor->setMaximumSize(QSize(50, 20));
		lineEditPro14Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro20Cor = new QLineEdit(ProbeSetClass);
		lineEditPro20Cor->setObjectName(QString::fromUtf8("lineEditPro20Cor"));
		lineEditPro20Cor->setEnabled(false);
		lineEditPro20Cor->setGeometry(QRect(310, 400, 50, 20));
		lineEditPro20Cor->setMinimumSize(QSize(50, 20));
		lineEditPro20Cor->setMaximumSize(QSize(50, 20));
		lineEditPro20Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro17Cor = new QLineEdit(ProbeSetClass);
		lineEditPro17Cor->setObjectName(QString::fromUtf8("lineEditPro17Cor"));
		lineEditPro17Cor->setEnabled(false);
		lineEditPro17Cor->setGeometry(QRect(310, 280, 50, 20));
		lineEditPro17Cor->setMinimumSize(QSize(50, 20));
		lineEditPro17Cor->setMaximumSize(QSize(50, 20));
		lineEditPro17Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro11Cor = new QLineEdit(ProbeSetClass);
		lineEditPro11Cor->setObjectName(QString::fromUtf8("lineEditPro11Cor"));
		lineEditPro11Cor->setEnabled(false);
		lineEditPro11Cor->setGeometry(QRect(120, 520, 50, 20));
		lineEditPro11Cor->setMinimumSize(QSize(50, 20));
		lineEditPro11Cor->setMaximumSize(QSize(50, 20));
		lineEditPro11Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro12Cor = new QLineEdit(ProbeSetClass);
		lineEditPro12Cor->setObjectName(QString::fromUtf8("lineEditPro12Cor"));
		lineEditPro12Cor->setEnabled(false);
		lineEditPro12Cor->setGeometry(QRect(120, 560, 50, 20));
		lineEditPro12Cor->setMinimumSize(QSize(50, 20));
		lineEditPro12Cor->setMaximumSize(QSize(50, 20));
		lineEditPro12Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro19Cor = new QLineEdit(ProbeSetClass);
		lineEditPro19Cor->setObjectName(QString::fromUtf8("lineEditPro19Cor"));
		lineEditPro19Cor->setEnabled(false);
		lineEditPro19Cor->setGeometry(QRect(310, 360, 50, 20));
		lineEditPro19Cor->setMinimumSize(QSize(50, 20));
		lineEditPro19Cor->setMaximumSize(QSize(50, 20));
		lineEditPro19Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro16Cor = new QLineEdit(ProbeSetClass);
		lineEditPro16Cor->setObjectName(QString::fromUtf8("lineEditPro16Cor"));
		lineEditPro16Cor->setEnabled(false);
		lineEditPro16Cor->setGeometry(QRect(310, 240, 50, 20));
		lineEditPro16Cor->setMinimumSize(QSize(50, 20));
		lineEditPro16Cor->setMaximumSize(QSize(50, 20));
		lineEditPro16Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro13Cor = new QLineEdit(ProbeSetClass);
		lineEditPro13Cor->setObjectName(QString::fromUtf8("lineEditPro13Cor"));
		lineEditPro13Cor->setEnabled(false);
		lineEditPro13Cor->setGeometry(QRect(310, 120, 50, 20));
		lineEditPro13Cor->setMinimumSize(QSize(50, 20));
		lineEditPro13Cor->setMaximumSize(QSize(50, 20));
		lineEditPro13Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro29Cor = new QLineEdit(ProbeSetClass);
		lineEditPro29Cor->setObjectName(QString::fromUtf8("lineEditPro29Cor"));
		lineEditPro29Cor->setEnabled(false);
		lineEditPro29Cor->setGeometry(QRect(500, 280, 50, 20));
		lineEditPro29Cor->setMinimumSize(QSize(50, 20));
		lineEditPro29Cor->setMaximumSize(QSize(50, 20));
		lineEditPro29Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro22Cor = new QLineEdit(ProbeSetClass);
		lineEditPro22Cor->setObjectName(QString::fromUtf8("lineEditPro22Cor"));
		lineEditPro22Cor->setEnabled(false);
		lineEditPro22Cor->setGeometry(QRect(310, 480, 50, 20));
		lineEditPro22Cor->setMinimumSize(QSize(50, 20));
		lineEditPro22Cor->setMaximumSize(QSize(50, 20));
		lineEditPro22Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro28Cor = new QLineEdit(ProbeSetClass);
		lineEditPro28Cor->setObjectName(QString::fromUtf8("lineEditPro28Cor"));
		lineEditPro28Cor->setEnabled(false);
		lineEditPro28Cor->setGeometry(QRect(500, 240, 50, 20));
		lineEditPro28Cor->setMinimumSize(QSize(50, 20));
		lineEditPro28Cor->setMaximumSize(QSize(50, 20));
		lineEditPro28Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro21Cor = new QLineEdit(ProbeSetClass);
		lineEditPro21Cor->setObjectName(QString::fromUtf8("lineEditPro21Cor"));
		lineEditPro21Cor->setEnabled(false);
		lineEditPro21Cor->setGeometry(QRect(310, 440, 50, 20));
		lineEditPro21Cor->setMinimumSize(QSize(50, 20));
		lineEditPro21Cor->setMaximumSize(QSize(50, 20));
		lineEditPro21Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro30Cor = new QLineEdit(ProbeSetClass);
		lineEditPro30Cor->setObjectName(QString::fromUtf8("lineEditPro30Cor"));
		lineEditPro30Cor->setEnabled(false);
		lineEditPro30Cor->setGeometry(QRect(500, 320, 50, 20));
		lineEditPro30Cor->setMinimumSize(QSize(50, 20));
		lineEditPro30Cor->setMaximumSize(QSize(50, 20));
		lineEditPro30Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro23Cor = new QLineEdit(ProbeSetClass);
		lineEditPro23Cor->setObjectName(QString::fromUtf8("lineEditPro23Cor"));
		lineEditPro23Cor->setEnabled(false);
		lineEditPro23Cor->setGeometry(QRect(310, 520, 50, 20));
		lineEditPro23Cor->setMinimumSize(QSize(50, 20));
		lineEditPro23Cor->setMaximumSize(QSize(50, 20));
		lineEditPro23Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro25Cor = new QLineEdit(ProbeSetClass);
		lineEditPro25Cor->setObjectName(QString::fromUtf8("lineEditPro25Cor"));
		lineEditPro25Cor->setEnabled(false);
		lineEditPro25Cor->setGeometry(QRect(500, 120, 50, 20));
		lineEditPro25Cor->setMinimumSize(QSize(50, 20));
		lineEditPro25Cor->setMaximumSize(QSize(50, 20));
		lineEditPro25Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro24Cor = new QLineEdit(ProbeSetClass);
		lineEditPro24Cor->setObjectName(QString::fromUtf8("lineEditPro24Cor"));
		lineEditPro24Cor->setEnabled(false);
		lineEditPro24Cor->setGeometry(QRect(310, 560, 50, 20));
		lineEditPro24Cor->setMinimumSize(QSize(50, 20));
		lineEditPro24Cor->setMaximumSize(QSize(50, 20));
		lineEditPro24Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro27Cor = new QLineEdit(ProbeSetClass);
		lineEditPro27Cor->setObjectName(QString::fromUtf8("lineEditPro27Cor"));
		lineEditPro27Cor->setEnabled(false);
		lineEditPro27Cor->setGeometry(QRect(500, 200, 50, 20));
		lineEditPro27Cor->setMinimumSize(QSize(50, 20));
		lineEditPro27Cor->setMaximumSize(QSize(50, 20));
		lineEditPro27Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro26Cor = new QLineEdit(ProbeSetClass);
		lineEditPro26Cor->setObjectName(QString::fromUtf8("lineEditPro26Cor"));
		lineEditPro26Cor->setEnabled(false);
		lineEditPro26Cor->setGeometry(QRect(500, 160, 50, 20));
		lineEditPro26Cor->setMinimumSize(QSize(50, 20));
		lineEditPro26Cor->setMaximumSize(QSize(50, 20));
		lineEditPro26Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_37 = new QLabel(ProbeSetClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(30, 320, 20, 20));
		label_37->setMinimumSize(QSize(20, 20));
		label_37->setMaximumSize(QSize(20, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_14 = new QLabel(ProbeSetClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(30, 120, 20, 20));
		label_14->setMinimumSize(QSize(20, 20));
		label_14->setMaximumSize(QSize(20, 20));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_39 = new QLabel(ProbeSetClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(30, 400, 20, 20));
		label_39->setMinimumSize(QSize(20, 20));
		label_39->setMaximumSize(QSize(20, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_38 = new QLabel(ProbeSetClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(30, 360, 20, 20));
		label_38->setMinimumSize(QSize(20, 20));
		label_38->setMaximumSize(QSize(20, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_34 = new QLabel(ProbeSetClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(30, 200, 20, 20));
		label_34->setMinimumSize(QSize(20, 20));
		label_34->setMaximumSize(QSize(20, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_41 = new QLabel(ProbeSetClass);
		label_41->setObjectName(QString::fromUtf8("label_41"));
		label_41->setGeometry(QRect(30, 480, 20, 20));
		label_41->setMinimumSize(QSize(20, 20));
		label_41->setMaximumSize(QSize(20, 20));
		label_41->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_15 = new QLabel(ProbeSetClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(30, 160, 20, 20));
		label_15->setMinimumSize(QSize(20, 20));
		label_15->setMaximumSize(QSize(20, 20));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(ProbeSetClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(30, 280, 20, 20));
		label_36->setMinimumSize(QSize(20, 20));
		label_36->setMaximumSize(QSize(20, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_40 = new QLabel(ProbeSetClass);
		label_40->setObjectName(QString::fromUtf8("label_40"));
		label_40->setGeometry(QRect(30, 440, 20, 20));
		label_40->setMinimumSize(QSize(20, 20));
		label_40->setMaximumSize(QSize(20, 20));
		label_40->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_35 = new QLabel(ProbeSetClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(30, 240, 20, 20));
		label_35->setMinimumSize(QSize(20, 20));
		label_35->setMaximumSize(QSize(20, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_49 = new QLabel(ProbeSetClass);
		label_49->setObjectName(QString::fromUtf8("label_49"));
		label_49->setGeometry(QRect(220, 160, 20, 20));
		label_49->setMinimumSize(QSize(20, 20));
		label_49->setMaximumSize(QSize(20, 20));
		label_49->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_50 = new QLabel(ProbeSetClass);
		label_50->setObjectName(QString::fromUtf8("label_50"));
		label_50->setGeometry(QRect(220, 200, 20, 20));
		label_50->setMinimumSize(QSize(20, 20));
		label_50->setMaximumSize(QSize(20, 20));
		label_50->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_46 = new QLabel(ProbeSetClass);
		label_46->setObjectName(QString::fromUtf8("label_46"));
		label_46->setGeometry(QRect(220, 360, 20, 20));
		label_46->setMinimumSize(QSize(20, 20));
		label_46->setMaximumSize(QSize(20, 20));
		label_46->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_45 = new QLabel(ProbeSetClass);
		label_45->setObjectName(QString::fromUtf8("label_45"));
		label_45->setGeometry(QRect(220, 320, 20, 20));
		label_45->setMinimumSize(QSize(20, 20));
		label_45->setMaximumSize(QSize(20, 20));
		label_45->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_44 = new QLabel(ProbeSetClass);
		label_44->setObjectName(QString::fromUtf8("label_44"));
		label_44->setGeometry(QRect(30, 560, 20, 20));
		label_44->setMinimumSize(QSize(20, 20));
		label_44->setMaximumSize(QSize(20, 20));
		label_44->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_48 = new QLabel(ProbeSetClass);
		label_48->setObjectName(QString::fromUtf8("label_48"));
		label_48->setGeometry(QRect(30, 520, 20, 20));
		label_48->setMinimumSize(QSize(20, 20));
		label_48->setMaximumSize(QSize(20, 20));
		label_48->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_51 = new QLabel(ProbeSetClass);
		label_51->setObjectName(QString::fromUtf8("label_51"));
		label_51->setGeometry(QRect(220, 240, 20, 20));
		label_51->setMinimumSize(QSize(20, 20));
		label_51->setMaximumSize(QSize(20, 20));
		label_51->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_47 = new QLabel(ProbeSetClass);
		label_47->setObjectName(QString::fromUtf8("label_47"));
		label_47->setGeometry(QRect(220, 120, 20, 20));
		label_47->setMinimumSize(QSize(20, 20));
		label_47->setMaximumSize(QSize(20, 20));
		label_47->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_43 = new QLabel(ProbeSetClass);
		label_43->setObjectName(QString::fromUtf8("label_43"));
		label_43->setGeometry(QRect(220, 280, 20, 20));
		label_43->setMinimumSize(QSize(20, 20));
		label_43->setMaximumSize(QSize(20, 20));
		label_43->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_42 = new QLabel(ProbeSetClass);
		label_42->setObjectName(QString::fromUtf8("label_42"));
		label_42->setGeometry(QRect(220, 400, 20, 20));
		label_42->setMinimumSize(QSize(20, 20));
		label_42->setMaximumSize(QSize(20, 20));
		label_42->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_54 = new QLabel(ProbeSetClass);
		label_54->setObjectName(QString::fromUtf8("label_54"));
		label_54->setGeometry(QRect(410, 200, 20, 20));
		label_54->setMinimumSize(QSize(20, 20));
		label_54->setMaximumSize(QSize(20, 20));
		label_54->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_57 = new QLabel(ProbeSetClass);
		label_57->setObjectName(QString::fromUtf8("label_57"));
		label_57->setGeometry(QRect(410, 120, 20, 20));
		label_57->setMinimumSize(QSize(20, 20));
		label_57->setMaximumSize(QSize(20, 20));
		label_57->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_61 = new QLabel(ProbeSetClass);
		label_61->setObjectName(QString::fromUtf8("label_61"));
		label_61->setGeometry(QRect(410, 320, 20, 20));
		label_61->setMinimumSize(QSize(20, 20));
		label_61->setMaximumSize(QSize(20, 20));
		label_61->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_60 = new QLabel(ProbeSetClass);
		label_60->setObjectName(QString::fromUtf8("label_60"));
		label_60->setGeometry(QRect(220, 560, 20, 20));
		label_60->setMinimumSize(QSize(20, 20));
		label_60->setMaximumSize(QSize(20, 20));
		label_60->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_53 = new QLabel(ProbeSetClass);
		label_53->setObjectName(QString::fromUtf8("label_53"));
		label_53->setGeometry(QRect(220, 480, 20, 20));
		label_53->setMinimumSize(QSize(20, 20));
		label_53->setMaximumSize(QSize(20, 20));
		label_53->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_52 = new QLabel(ProbeSetClass);
		label_52->setObjectName(QString::fromUtf8("label_52"));
		label_52->setGeometry(QRect(410, 160, 20, 20));
		label_52->setMinimumSize(QSize(20, 20));
		label_52->setMaximumSize(QSize(20, 20));
		label_52->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_59 = new QLabel(ProbeSetClass);
		label_59->setObjectName(QString::fromUtf8("label_59"));
		label_59->setGeometry(QRect(220, 520, 20, 20));
		label_59->setMinimumSize(QSize(20, 20));
		label_59->setMaximumSize(QSize(20, 20));
		label_59->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_56 = new QLabel(ProbeSetClass);
		label_56->setObjectName(QString::fromUtf8("label_56"));
		label_56->setGeometry(QRect(410, 280, 20, 20));
		label_56->setMinimumSize(QSize(20, 20));
		label_56->setMaximumSize(QSize(20, 20));
		label_56->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_55 = new QLabel(ProbeSetClass);
		label_55->setObjectName(QString::fromUtf8("label_55"));
		label_55->setGeometry(QRect(220, 440, 20, 20));
		label_55->setMinimumSize(QSize(20, 20));
		label_55->setMaximumSize(QSize(20, 20));
		label_55->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_58 = new QLabel(ProbeSetClass);
		label_58->setObjectName(QString::fromUtf8("label_58"));
		label_58->setGeometry(QRect(410, 240, 20, 20));
		label_58->setMinimumSize(QSize(20, 20));
		label_58->setMaximumSize(QSize(20, 20));
		label_58->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe1 = new QLineEdit(ProbeSetClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setEnabled(true);
		lineEditProbe1->setGeometry(QRect(670, 120, 60, 20));
		lineEditProbe1->setMinimumSize(QSize(60, 20));
		lineEditProbe1->setMaximumSize(QSize(60, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2 = new QLineEdit(ProbeSetClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setEnabled(true);
		lineEditProbe2->setGeometry(QRect(670, 160, 60, 20));
		lineEditProbe2->setMinimumSize(QSize(60, 20));
		lineEditProbe2->setMaximumSize(QSize(60, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe3 = new QLineEdit(ProbeSetClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setEnabled(true);
		lineEditProbe3->setGeometry(QRect(670, 200, 60, 20));
		lineEditProbe3->setMinimumSize(QSize(60, 20));
		lineEditProbe3->setMaximumSize(QSize(60, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe4 = new QLineEdit(ProbeSetClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setEnabled(true);
		lineEditProbe4->setGeometry(QRect(670, 240, 60, 20));
		lineEditProbe4->setMinimumSize(QSize(60, 20));
		lineEditProbe4->setMaximumSize(QSize(60, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe5 = new QLineEdit(ProbeSetClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setEnabled(true);
		lineEditProbe5->setGeometry(QRect(670, 280, 60, 20));
		lineEditProbe5->setMinimumSize(QSize(60, 20));
		lineEditProbe5->setMaximumSize(QSize(60, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_16 = new QLabel(ProbeSetClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(670, 70, 71, 18));
		label_16->setMinimumSize(QSize(50, 18));
		label_16->setMaximumSize(QSize(100, 18));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_17 = new QLabel(ProbeSetClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(590, 120, 70, 18));
		label_17->setMinimumSize(QSize(70, 18));
		label_17->setMaximumSize(QSize(70, 18));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_18 = new QLabel(ProbeSetClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(590, 160, 70, 18));
		label_18->setMinimumSize(QSize(70, 18));
		label_18->setMaximumSize(QSize(70, 18));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_19 = new QLabel(ProbeSetClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(590, 200, 70, 18));
		label_19->setMinimumSize(QSize(70, 18));
		label_19->setMaximumSize(QSize(70, 18));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(ProbeSetClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(590, 240, 70, 18));
		label_20->setMinimumSize(QSize(70, 18));
		label_20->setMaximumSize(QSize(70, 18));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_21 = new QLabel(ProbeSetClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(590, 280, 70, 18));
		label_21->setMinimumSize(QSize(70, 18));
		label_21->setMaximumSize(QSize(70, 18));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_22 = new QLabel(ProbeSetClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(740, 120, 20, 15));
		label_22->setMinimumSize(QSize(20, 15));
		label_22->setMaximumSize(QSize(20, 15));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(ProbeSetClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(740, 160, 20, 15));
		label_23->setMinimumSize(QSize(20, 15));
		label_23->setMaximumSize(QSize(20, 15));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_24 = new QLabel(ProbeSetClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(740, 200, 20, 15));
		label_24->setMinimumSize(QSize(20, 15));
		label_24->setMaximumSize(QSize(20, 15));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_25 = new QLabel(ProbeSetClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(740, 240, 20, 15));
		label_25->setMinimumSize(QSize(20, 15));
		label_25->setMaximumSize(QSize(20, 15));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_26 = new QLabel(ProbeSetClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(740, 280, 20, 15));
		label_26->setMinimumSize(QSize(20, 15));
		label_26->setMaximumSize(QSize(20, 15));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonSingle = new QRadioButton(ProbeSetClass);
		radioButtonSingle->setObjectName(QString::fromUtf8("radioButtonSingle"));
		radioButtonSingle->setGeometry(QRect(590, 440, 101, 16));
		radioButtonSingle->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonDouble = new QRadioButton(ProbeSetClass);
		radioButtonDouble->setObjectName(QString::fromUtf8("radioButtonDouble"));
		radioButtonDouble->setGeometry(QRect(590, 480, 101, 16));
		radioButtonDouble->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_27 = new QLabel(ProbeSetClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(590, 400, 70, 18));
		label_27->setMinimumSize(QSize(70, 18));
		label_27->setMaximumSize(QSize(70, 18));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_28 = new QLabel(ProbeSetClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(740, 400, 20, 15));
		label_28->setMinimumSize(QSize(20, 15));
		label_28->setMaximumSize(QSize(20, 15));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditStrangePoint = new QLineEdit(ProbeSetClass);
		lineEditStrangePoint->setObjectName(QString::fromUtf8("lineEditStrangePoint"));
		lineEditStrangePoint->setEnabled(true);
		lineEditStrangePoint->setGeometry(QRect(670, 400, 60, 20));
		lineEditStrangePoint->setMinimumSize(QSize(60, 20));
		lineEditStrangePoint->setMaximumSize(QSize(60, 20));
		lineEditStrangePoint->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro36Cor = new QLineEdit(ProbeSetClass);
		lineEditPro36Cor->setObjectName(QString::fromUtf8("lineEditPro36Cor"));
		lineEditPro36Cor->setEnabled(false);
		lineEditPro36Cor->setGeometry(QRect(500, 560, 50, 20));
		lineEditPro36Cor->setMinimumSize(QSize(50, 20));
		lineEditPro36Cor->setMaximumSize(QSize(50, 20));
		lineEditPro36Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro32Hz = new QLineEdit(ProbeSetClass);
		lineEditPro32Hz->setObjectName(QString::fromUtf8("lineEditPro32Hz"));
		lineEditPro32Hz->setEnabled(false);
		lineEditPro32Hz->setGeometry(QRect(440, 400, 50, 20));
		lineEditPro32Hz->setMinimumSize(QSize(50, 20));
		lineEditPro32Hz->setMaximumSize(QSize(50, 20));
		lineEditPro32Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro31Hz = new QLineEdit(ProbeSetClass);
		lineEditPro31Hz->setObjectName(QString::fromUtf8("lineEditPro31Hz"));
		lineEditPro31Hz->setEnabled(false);
		lineEditPro31Hz->setGeometry(QRect(440, 360, 50, 20));
		lineEditPro31Hz->setMinimumSize(QSize(50, 20));
		lineEditPro31Hz->setMaximumSize(QSize(50, 20));
		lineEditPro31Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro32Cor = new QLineEdit(ProbeSetClass);
		lineEditPro32Cor->setObjectName(QString::fromUtf8("lineEditPro32Cor"));
		lineEditPro32Cor->setEnabled(false);
		lineEditPro32Cor->setGeometry(QRect(500, 400, 50, 20));
		lineEditPro32Cor->setMinimumSize(QSize(50, 20));
		lineEditPro32Cor->setMaximumSize(QSize(50, 20));
		lineEditPro32Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro34Cor = new QLineEdit(ProbeSetClass);
		lineEditPro34Cor->setObjectName(QString::fromUtf8("lineEditPro34Cor"));
		lineEditPro34Cor->setEnabled(false);
		lineEditPro34Cor->setGeometry(QRect(500, 480, 50, 20));
		lineEditPro34Cor->setMinimumSize(QSize(50, 20));
		lineEditPro34Cor->setMaximumSize(QSize(50, 20));
		lineEditPro34Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro35Cor = new QLineEdit(ProbeSetClass);
		lineEditPro35Cor->setObjectName(QString::fromUtf8("lineEditPro35Cor"));
		lineEditPro35Cor->setEnabled(false);
		lineEditPro35Cor->setGeometry(QRect(500, 520, 50, 20));
		lineEditPro35Cor->setMinimumSize(QSize(50, 20));
		lineEditPro35Cor->setMaximumSize(QSize(50, 20));
		lineEditPro35Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro33Hz = new QLineEdit(ProbeSetClass);
		lineEditPro33Hz->setObjectName(QString::fromUtf8("lineEditPro33Hz"));
		lineEditPro33Hz->setEnabled(false);
		lineEditPro33Hz->setGeometry(QRect(440, 440, 50, 20));
		lineEditPro33Hz->setMinimumSize(QSize(50, 20));
		lineEditPro33Hz->setMaximumSize(QSize(50, 20));
		lineEditPro33Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro34Hz = new QLineEdit(ProbeSetClass);
		lineEditPro34Hz->setObjectName(QString::fromUtf8("lineEditPro34Hz"));
		lineEditPro34Hz->setEnabled(false);
		lineEditPro34Hz->setGeometry(QRect(440, 480, 50, 20));
		lineEditPro34Hz->setMinimumSize(QSize(50, 20));
		lineEditPro34Hz->setMaximumSize(QSize(50, 20));
		lineEditPro34Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_62 = new QLabel(ProbeSetClass);
		label_62->setObjectName(QString::fromUtf8("label_62"));
		label_62->setGeometry(QRect(410, 560, 20, 20));
		label_62->setMinimumSize(QSize(20, 20));
		label_62->setMaximumSize(QSize(20, 20));
		label_62->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_63 = new QLabel(ProbeSetClass);
		label_63->setObjectName(QString::fromUtf8("label_63"));
		label_63->setGeometry(QRect(410, 520, 20, 20));
		label_63->setMinimumSize(QSize(20, 20));
		label_63->setMaximumSize(QSize(20, 20));
		label_63->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditPro33Cor = new QLineEdit(ProbeSetClass);
		lineEditPro33Cor->setObjectName(QString::fromUtf8("lineEditPro33Cor"));
		lineEditPro33Cor->setEnabled(false);
		lineEditPro33Cor->setGeometry(QRect(500, 440, 50, 20));
		lineEditPro33Cor->setMinimumSize(QSize(50, 20));
		lineEditPro33Cor->setMaximumSize(QSize(50, 20));
		lineEditPro33Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_64 = new QLabel(ProbeSetClass);
		label_64->setObjectName(QString::fromUtf8("label_64"));
		label_64->setGeometry(QRect(410, 480, 20, 20));
		label_64->setMinimumSize(QSize(20, 20));
		label_64->setMaximumSize(QSize(20, 20));
		label_64->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_65 = new QLabel(ProbeSetClass);
		label_65->setObjectName(QString::fromUtf8("label_65"));
		label_65->setGeometry(QRect(410, 360, 20, 20));
		label_65->setMinimumSize(QSize(20, 20));
		label_65->setMaximumSize(QSize(20, 20));
		label_65->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_66 = new QLabel(ProbeSetClass);
		label_66->setObjectName(QString::fromUtf8("label_66"));
		label_66->setGeometry(QRect(410, 400, 20, 20));
		label_66->setMinimumSize(QSize(20, 20));
		label_66->setMaximumSize(QSize(20, 20));
		label_66->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditPro36Hz = new QLineEdit(ProbeSetClass);
		lineEditPro36Hz->setObjectName(QString::fromUtf8("lineEditPro36Hz"));
		lineEditPro36Hz->setEnabled(false);
		lineEditPro36Hz->setGeometry(QRect(440, 560, 50, 20));
		lineEditPro36Hz->setMinimumSize(QSize(50, 20));
		lineEditPro36Hz->setMaximumSize(QSize(50, 20));
		lineEditPro36Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro31Cor = new QLineEdit(ProbeSetClass);
		lineEditPro31Cor->setObjectName(QString::fromUtf8("lineEditPro31Cor"));
		lineEditPro31Cor->setEnabled(false);
		lineEditPro31Cor->setGeometry(QRect(500, 360, 50, 20));
		lineEditPro31Cor->setMinimumSize(QSize(50, 20));
		lineEditPro31Cor->setMaximumSize(QSize(50, 20));
		lineEditPro31Cor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditPro35Hz = new QLineEdit(ProbeSetClass);
		lineEditPro35Hz->setObjectName(QString::fromUtf8("lineEditPro35Hz"));
		lineEditPro35Hz->setEnabled(false);
		lineEditPro35Hz->setGeometry(QRect(440, 520, 50, 20));
		lineEditPro35Hz->setMinimumSize(QSize(50, 20));
		lineEditPro35Hz->setMaximumSize(QSize(50, 20));
		lineEditPro35Hz->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_67 = new QLabel(ProbeSetClass);
		label_67->setObjectName(QString::fromUtf8("label_67"));
		label_67->setGeometry(QRect(410, 440, 20, 20));
		label_67->setMinimumSize(QSize(20, 20));
		label_67->setMaximumSize(QSize(20, 20));
		label_67->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(ProbeSetClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(590, 320, 70, 18));
		label_29->setMinimumSize(QSize(70, 18));
		label_29->setMaximumSize(QSize(70, 18));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_30 = new QLabel(ProbeSetClass);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(740, 320, 20, 15));
		label_30->setMinimumSize(QSize(20, 15));
		label_30->setMaximumSize(QSize(20, 15));
		label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe6 = new QLineEdit(ProbeSetClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setEnabled(true);
		lineEditProbe6->setGeometry(QRect(670, 320, 60, 20));
		lineEditProbe6->setMinimumSize(QSize(60, 20));
		lineEditProbe6->setMaximumSize(QSize(60, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));

		retranslateUi(ProbeSetClass);

		QMetaObject::connectSlotsByName(ProbeSetClass);
	} // setupUi

	void retranslateUi(QDialog *ProbeSetClass)
	{
		ProbeSetClass->setWindowTitle(QApplication::translate("ProbeSetClass", "ProbeSet", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("ProbeSetClass", "\346\216\242\345\244\264\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("ProbeSetClass", "\351\242\221\347\216\207", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("ProbeSetClass", "\346\212\230\345\260\204\350\247\222", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("ProbeSetClass", " (\345\272\246)", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("ProbeSetClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("ProbeSetClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("ProbeSetClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("ProbeSetClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("ProbeSetClass", "\351\242\221\347\216\207", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("ProbeSetClass", " (\345\272\246)", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("ProbeSetClass", "\346\212\230\345\260\204\350\247\222", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("ProbeSetClass", "\346\212\230\345\260\204\350\247\222", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("ProbeSetClass", "\351\242\221\347\216\207", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("ProbeSetClass", " (\345\272\246)", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("ProbeSetClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("ProbeSetClass", "6", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("ProbeSetClass", "1", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("ProbeSetClass", "8", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("ProbeSetClass", "7", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("ProbeSetClass", "3", 0, QApplication::UnicodeUTF8));
		label_41->setText(QApplication::translate("ProbeSetClass", "10", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("ProbeSetClass", "2", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("ProbeSetClass", "5", 0, QApplication::UnicodeUTF8));
		label_40->setText(QApplication::translate("ProbeSetClass", "9", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("ProbeSetClass", "4", 0, QApplication::UnicodeUTF8));
		label_49->setText(QApplication::translate("ProbeSetClass", "14", 0, QApplication::UnicodeUTF8));
		label_50->setText(QApplication::translate("ProbeSetClass", "15", 0, QApplication::UnicodeUTF8));
		label_46->setText(QApplication::translate("ProbeSetClass", "19", 0, QApplication::UnicodeUTF8));
		label_45->setText(QApplication::translate("ProbeSetClass", "18", 0, QApplication::UnicodeUTF8));
		label_44->setText(QApplication::translate("ProbeSetClass", "12", 0, QApplication::UnicodeUTF8));
		label_48->setText(QApplication::translate("ProbeSetClass", "11", 0, QApplication::UnicodeUTF8));
		label_51->setText(QApplication::translate("ProbeSetClass", "16", 0, QApplication::UnicodeUTF8));
		label_47->setText(QApplication::translate("ProbeSetClass", "13", 0, QApplication::UnicodeUTF8));
		label_43->setText(QApplication::translate("ProbeSetClass", "17", 0, QApplication::UnicodeUTF8));
		label_42->setText(QApplication::translate("ProbeSetClass", "20", 0, QApplication::UnicodeUTF8));
		label_54->setText(QApplication::translate("ProbeSetClass", "27", 0, QApplication::UnicodeUTF8));
		label_57->setText(QApplication::translate("ProbeSetClass", "25", 0, QApplication::UnicodeUTF8));
		label_61->setText(QApplication::translate("ProbeSetClass", "30", 0, QApplication::UnicodeUTF8));
		label_60->setText(QApplication::translate("ProbeSetClass", "24", 0, QApplication::UnicodeUTF8));
		label_53->setText(QApplication::translate("ProbeSetClass", "22", 0, QApplication::UnicodeUTF8));
		label_52->setText(QApplication::translate("ProbeSetClass", "26", 0, QApplication::UnicodeUTF8));
		label_59->setText(QApplication::translate("ProbeSetClass", "23", 0, QApplication::UnicodeUTF8));
		label_56->setText(QApplication::translate("ProbeSetClass", "29", 0, QApplication::UnicodeUTF8));
		label_55->setText(QApplication::translate("ProbeSetClass", "21", 0, QApplication::UnicodeUTF8));
		label_58->setText(QApplication::translate("ProbeSetClass", "28", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("ProbeSetClass", "\346\265\213\345\216\232\346\240\241\345\207\206", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("ProbeSetClass", "\346\216\242\345\244\264\347\233\2221", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("ProbeSetClass", "\346\216\242\345\244\264\347\233\2222", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("ProbeSetClass", "\346\216\242\345\244\264\347\233\2223", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("ProbeSetClass", "\346\216\242\345\244\264\347\233\2224", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("ProbeSetClass", "\346\216\242\345\244\264\347\233\2225", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("ProbeSetClass", "mm", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("ProbeSetClass", "mm", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("ProbeSetClass", "mm", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("ProbeSetClass", "mm", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("ProbeSetClass", "mm", 0, QApplication::UnicodeUTF8));
		radioButtonSingle->setText(QApplication::translate("ProbeSetClass", "\345\215\225\345\220\221\346\240\241\345\207\206", 0, QApplication::UnicodeUTF8));
		radioButtonDouble->setText(QApplication::translate("ProbeSetClass", "\345\217\214\345\220\221\346\240\241\345\207\206", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("ProbeSetClass", "\345\216\273\345\245\207\347\202\271", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("ProbeSetClass", "mm", 0, QApplication::UnicodeUTF8));
		label_62->setText(QApplication::translate("ProbeSetClass", "36", 0, QApplication::UnicodeUTF8));
		label_63->setText(QApplication::translate("ProbeSetClass", "35", 0, QApplication::UnicodeUTF8));
		label_64->setText(QApplication::translate("ProbeSetClass", "34", 0, QApplication::UnicodeUTF8));
		label_65->setText(QApplication::translate("ProbeSetClass", "31", 0, QApplication::UnicodeUTF8));
		label_66->setText(QApplication::translate("ProbeSetClass", "32", 0, QApplication::UnicodeUTF8));
		label_67->setText(QApplication::translate("ProbeSetClass", "33", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("ProbeSetClass", "\346\216\242\345\244\264\347\233\2226", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("ProbeSetClass", "mm", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class ProbeSetClass: public Ui_ProbeSetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROBESET_H





/********************************************************************************
** Form generated from reading UI file 'refreq.ui'
**
** Created: Thu Jun 26 18:27:53 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFREQ_H
#define UI_REFREQ_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ReFreqClass
{
public:
	QLabel *label_2;
	QLabel *label_4;
	QLabel *label_3;
	QLabel *label;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QLabel *label_5;
	QLabel *label_6;
	QLabel *label_7;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QLabel *label_14;
	QLabel *label_15;
	QLabel *label_16;
	QLabel *label_17;
	QLabel *label_18;
	QLabel *label_19;
	QLabel *label_20;
	QLabel *label_21;
	QLabel *label_22;
	QLabel *label_23;
	QLabel *label_24;
	QLabel *label_25;
	QLabel *label_26;
	QLabel *label_27;
	QLabel *label_28;
	QLabel *label_29;
	QLabel *label_30;
	QLabel *label_31;
	QLabel *label_32;
	QLabel *label_33;
	QLineEdit *lineEditAll;
	QLineEdit *lineEditAny;
	QRadioButton *radioButtonAll;
	QRadioButton *radioButtonAny;
	QLineEdit *lineEditProbe10;
	QLineEdit *lineEditProbe7;
	QLineEdit *lineEditProbe2;
	QLineEdit *lineEditProbe9;
	QLineEdit *lineEditProbe8;
	QCheckBox *checkBoxProbe8;
	QCheckBox *checkBoxProbe1;
	QCheckBox *checkBoxProbe10;
	QCheckBox *checkBoxProbe4;
	QLineEdit *lineEditProbe4;
	QCheckBox *checkBoxProbe6;
	QLineEdit *lineEditProbe1;
	QCheckBox *checkBoxProbe7;
	QCheckBox *checkBoxProbe2;
	QLineEdit *lineEditProbe3;
	QCheckBox *checkBoxProbe9;
	QLineEdit *lineEditProbe6;
	QCheckBox *checkBoxProbe5;
	QCheckBox *checkBoxProbe3;
	QLineEdit *lineEditProbe5;
	QCheckBox *checkBoxProbe13;
	QLineEdit *lineEditProbe11;
	QLineEdit *lineEditProbe17;
	QLineEdit *lineEditProbe20;
	QLineEdit *lineEditProbe13;
	QCheckBox *checkBoxProbe15;
	QLineEdit *lineEditProbe16;
	QLineEdit *lineEditProbe18;
	QCheckBox *checkBoxProbe17;
	QLineEdit *lineEditProbe14;
	QCheckBox *checkBoxProbe20;
	QLineEdit *lineEditProbe15;
	QCheckBox *checkBoxProbe18;
	QLineEdit *lineEditProbe19;
	QLineEdit *lineEditProbe12;
	QCheckBox *checkBoxProbe19;
	QCheckBox *checkBoxProbe16;
	QCheckBox *checkBoxProbe11;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe12;
	QCheckBox *checkBoxProbe27;
	QCheckBox *checkBoxProbe21;
	QCheckBox *checkBoxProbe26;
	QCheckBox *checkBoxProbe25;
	QLineEdit *lineEditProbe26;
	QLineEdit *lineEditProbe25;
	QCheckBox *checkBoxProbe23;
	QLineEdit *lineEditProbe23;
	QLineEdit *lineEditProbe29;
	QCheckBox *checkBoxProbe28;
	QCheckBox *checkBoxProbe22;
	QLineEdit *lineEditProbe28;
	QCheckBox *checkBoxProbe24;
	QLineEdit *lineEditProbe22;
	QCheckBox *checkBoxProbe29;
	QLineEdit *lineEditProbe30;
	QLineEdit *lineEditProbe27;
	QCheckBox *checkBoxProbe30;
	QLineEdit *lineEditProbe21;
	QLineEdit *lineEditProbe24;
	QPushButton *pushButtonAnyIn;
	QPushButton *pushButtonAllIn;
	QCheckBox *checkBoxProbe32;
	QLabel *label_34;
	QCheckBox *checkBoxProbe34;
	QCheckBox *checkBoxProbe36;
	QLabel *label_35;
	QLabel *label_36;
	QCheckBox *checkBoxProbe33;
	QLineEdit *lineEditProbe34;
	QCheckBox *checkBoxProbe35;
	QLineEdit *lineEditProbe32;
	QLabel *label_37;
	QLineEdit *lineEditProbe35;
	QLineEdit *lineEditProbe33;
	QLabel *label_38;
	QCheckBox *checkBoxProbe31;
	QLineEdit *lineEditProbe31;
	QLineEdit *lineEditProbe36;
	QLabel *label_39;

	void setupUi(QDialog *ReFreqClass)
	{
		if (ReFreqClass->objectName().isEmpty())
			ReFreqClass->setObjectName(QString::fromUtf8("ReFreqClass"));
		ReFreqClass->resize(800, 600);
		ReFreqClass->setMinimumSize(QSize(800, 600));
		ReFreqClass->setMaximumSize(QSize(800, 600));
		ReFreqClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label_2 = new QLabel(ReFreqClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(330, 20, 180, 20));
		label_2->setMinimumSize(QSize(180, 20));
		label_2->setMaximumSize(QSize(180, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(ReFreqClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(170, 150, 30, 20));
		label_4->setMinimumSize(QSize(30, 20));
		label_4->setMaximumSize(QSize(30, 20));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_3 = new QLabel(ReFreqClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(210, 100, 30, 20));
		label_3->setMinimumSize(QSize(30, 20));
		label_3->setMaximumSize(QSize(30, 20));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label = new QLabel(ReFreqClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(210, 60, 30, 20));
		label->setMinimumSize(QSize(30, 20));
		label->setMaximumSize(QSize(30, 20));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(ReFreqClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(590, 550, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(ReFreqClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(690, 550, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_5 = new QLabel(ReFreqClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(170, 190, 30, 20));
		label_5->setMinimumSize(QSize(30, 20));
		label_5->setMaximumSize(QSize(30, 20));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_6 = new QLabel(ReFreqClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(170, 230, 30, 20));
		label_6->setMinimumSize(QSize(30, 20));
		label_6->setMaximumSize(QSize(30, 20));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(ReFreqClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(170, 270, 30, 20));
		label_7->setMinimumSize(QSize(30, 20));
		label_7->setMaximumSize(QSize(30, 20));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(ReFreqClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(170, 310, 30, 20));
		label_8->setMinimumSize(QSize(30, 20));
		label_8->setMaximumSize(QSize(30, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(ReFreqClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(170, 350, 30, 20));
		label_9->setMinimumSize(QSize(30, 20));
		label_9->setMaximumSize(QSize(30, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(ReFreqClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(170, 390, 30, 20));
		label_10->setMinimumSize(QSize(30, 20));
		label_10->setMaximumSize(QSize(30, 20));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_11 = new QLabel(ReFreqClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(170, 430, 30, 20));
		label_11->setMinimumSize(QSize(30, 20));
		label_11->setMaximumSize(QSize(30, 20));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_12 = new QLabel(ReFreqClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(170, 470, 30, 20));
		label_12->setMinimumSize(QSize(30, 20));
		label_12->setMaximumSize(QSize(30, 20));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_13 = new QLabel(ReFreqClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(170, 510, 30, 20));
		label_13->setMinimumSize(QSize(30, 20));
		label_13->setMaximumSize(QSize(30, 20));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_14 = new QLabel(ReFreqClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(360, 510, 30, 20));
		label_14->setMinimumSize(QSize(30, 20));
		label_14->setMaximumSize(QSize(30, 20));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_15 = new QLabel(ReFreqClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(360, 270, 30, 20));
		label_15->setMinimumSize(QSize(30, 20));
		label_15->setMaximumSize(QSize(30, 20));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_16 = new QLabel(ReFreqClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(360, 350, 30, 20));
		label_16->setMinimumSize(QSize(30, 20));
		label_16->setMaximumSize(QSize(30, 20));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_17 = new QLabel(ReFreqClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(360, 470, 30, 20));
		label_17->setMinimumSize(QSize(30, 20));
		label_17->setMaximumSize(QSize(30, 20));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_18 = new QLabel(ReFreqClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(360, 150, 30, 20));
		label_18->setMinimumSize(QSize(30, 20));
		label_18->setMaximumSize(QSize(30, 20));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_19 = new QLabel(ReFreqClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(360, 190, 30, 20));
		label_19->setMinimumSize(QSize(30, 20));
		label_19->setMaximumSize(QSize(30, 20));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(ReFreqClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(360, 390, 30, 20));
		label_20->setMinimumSize(QSize(30, 20));
		label_20->setMaximumSize(QSize(30, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_21 = new QLabel(ReFreqClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(360, 430, 30, 20));
		label_21->setMinimumSize(QSize(30, 20));
		label_21->setMaximumSize(QSize(30, 20));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_22 = new QLabel(ReFreqClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(360, 230, 30, 20));
		label_22->setMinimumSize(QSize(30, 20));
		label_22->setMaximumSize(QSize(30, 20));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(ReFreqClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(360, 310, 30, 20));
		label_23->setMinimumSize(QSize(30, 20));
		label_23->setMaximumSize(QSize(30, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_24 = new QLabel(ReFreqClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(550, 310, 30, 20));
		label_24->setMinimumSize(QSize(30, 20));
		label_24->setMaximumSize(QSize(30, 20));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_25 = new QLabel(ReFreqClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(550, 350, 30, 20));
		label_25->setMinimumSize(QSize(30, 20));
		label_25->setMaximumSize(QSize(30, 20));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_26 = new QLabel(ReFreqClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(550, 470, 30, 20));
		label_26->setMinimumSize(QSize(30, 20));
		label_26->setMaximumSize(QSize(30, 20));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_27 = new QLabel(ReFreqClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(550, 230, 30, 20));
		label_27->setMinimumSize(QSize(30, 20));
		label_27->setMaximumSize(QSize(30, 20));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_28 = new QLabel(ReFreqClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(550, 190, 30, 20));
		label_28->setMinimumSize(QSize(30, 20));
		label_28->setMaximumSize(QSize(30, 20));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(ReFreqClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(550, 510, 30, 20));
		label_29->setMinimumSize(QSize(30, 20));
		label_29->setMaximumSize(QSize(30, 20));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_30 = new QLabel(ReFreqClass);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(550, 150, 30, 20));
		label_30->setMinimumSize(QSize(30, 20));
		label_30->setMaximumSize(QSize(30, 20));
		label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_31 = new QLabel(ReFreqClass);
		label_31->setObjectName(QString::fromUtf8("label_31"));
		label_31->setGeometry(QRect(550, 390, 30, 20));
		label_31->setMinimumSize(QSize(30, 20));
		label_31->setMaximumSize(QSize(30, 20));
		label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_32 = new QLabel(ReFreqClass);
		label_32->setObjectName(QString::fromUtf8("label_32"));
		label_32->setGeometry(QRect(550, 270, 30, 20));
		label_32->setMinimumSize(QSize(30, 20));
		label_32->setMaximumSize(QSize(30, 20));
		label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_33 = new QLabel(ReFreqClass);
		label_33->setObjectName(QString::fromUtf8("label_33"));
		label_33->setGeometry(QRect(550, 430, 30, 20));
		label_33->setMinimumSize(QSize(30, 20));
		label_33->setMaximumSize(QSize(30, 20));
		label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditAll = new QLineEdit(ReFreqClass);
		lineEditAll->setObjectName(QString::fromUtf8("lineEditAll"));
		lineEditAll->setGeometry(QRect(130, 60, 80, 20));
		lineEditAll->setMinimumSize(QSize(80, 20));
		lineEditAll->setMaximumSize(QSize(80, 20));
		lineEditAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAny = new QLineEdit(ReFreqClass);
		lineEditAny->setObjectName(QString::fromUtf8("lineEditAny"));
		lineEditAny->setGeometry(QRect(130, 100, 80, 20));
		lineEditAny->setMinimumSize(QSize(80, 20));
		lineEditAny->setMaximumSize(QSize(80, 20));
		lineEditAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		radioButtonAll = new QRadioButton(ReFreqClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(40, 60, 89, 16));
		radioButtonAll->setMinimumSize(QSize(89, 16));
		radioButtonAll->setMaximumSize(QSize(89, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAny = new QRadioButton(ReFreqClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(40, 100, 89, 16));
		radioButtonAny->setMinimumSize(QSize(89, 16));
		radioButtonAny->setMaximumSize(QSize(89, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe10 = new QLineEdit(ReFreqClass);
		lineEditProbe10->setObjectName(QString::fromUtf8("lineEditProbe10"));
		lineEditProbe10->setGeometry(QRect(90, 510, 80, 20));
		lineEditProbe10->setMinimumSize(QSize(80, 20));
		lineEditProbe10->setMaximumSize(QSize(80, 20));
		lineEditProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7 = new QLineEdit(ReFreqClass);
		lineEditProbe7->setObjectName(QString::fromUtf8("lineEditProbe7"));
		lineEditProbe7->setGeometry(QRect(90, 390, 80, 20));
		lineEditProbe7->setMinimumSize(QSize(80, 20));
		lineEditProbe7->setMaximumSize(QSize(80, 20));
		lineEditProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2 = new QLineEdit(ReFreqClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setGeometry(QRect(90, 190, 80, 20));
		lineEditProbe2->setMinimumSize(QSize(80, 20));
		lineEditProbe2->setMaximumSize(QSize(80, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9 = new QLineEdit(ReFreqClass);
		lineEditProbe9->setObjectName(QString::fromUtf8("lineEditProbe9"));
		lineEditProbe9->setGeometry(QRect(90, 470, 80, 20));
		lineEditProbe9->setMinimumSize(QSize(80, 20));
		lineEditProbe9->setMaximumSize(QSize(80, 20));
		lineEditProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8 = new QLineEdit(ReFreqClass);
		lineEditProbe8->setObjectName(QString::fromUtf8("lineEditProbe8"));
		lineEditProbe8->setGeometry(QRect(90, 430, 80, 20));
		lineEditProbe8->setMinimumSize(QSize(80, 20));
		lineEditProbe8->setMaximumSize(QSize(80, 20));
		lineEditProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe8 = new QCheckBox(ReFreqClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(40, 430, 50, 20));
		checkBoxProbe8->setMinimumSize(QSize(50, 20));
		checkBoxProbe8->setMaximumSize(QSize(50, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe1 = new QCheckBox(ReFreqClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(40, 150, 50, 20));
		checkBoxProbe1->setMinimumSize(QSize(50, 20));
		checkBoxProbe1->setMaximumSize(QSize(50, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe10 = new QCheckBox(ReFreqClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(40, 510, 50, 20));
		checkBoxProbe10->setMinimumSize(QSize(50, 20));
		checkBoxProbe10->setMaximumSize(QSize(50, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe4 = new QCheckBox(ReFreqClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(40, 270, 50, 20));
		checkBoxProbe4->setMinimumSize(QSize(50, 20));
		checkBoxProbe4->setMaximumSize(QSize(50, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe4 = new QLineEdit(ReFreqClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setGeometry(QRect(90, 270, 80, 20));
		lineEditProbe4->setMinimumSize(QSize(80, 20));
		lineEditProbe4->setMaximumSize(QSize(80, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe6 = new QCheckBox(ReFreqClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(40, 350, 50, 20));
		checkBoxProbe6->setMinimumSize(QSize(50, 20));
		checkBoxProbe6->setMaximumSize(QSize(50, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe1 = new QLineEdit(ReFreqClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setGeometry(QRect(90, 150, 80, 20));
		lineEditProbe1->setMinimumSize(QSize(80, 20));
		lineEditProbe1->setMaximumSize(QSize(80, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe7 = new QCheckBox(ReFreqClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(40, 390, 50, 20));
		checkBoxProbe7->setMinimumSize(QSize(50, 20));
		checkBoxProbe7->setMaximumSize(QSize(50, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe2 = new QCheckBox(ReFreqClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(40, 190, 50, 20));
		checkBoxProbe2->setMinimumSize(QSize(50, 20));
		checkBoxProbe2->setMaximumSize(QSize(50, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe3 = new QLineEdit(ReFreqClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setGeometry(QRect(90, 230, 80, 20));
		lineEditProbe3->setMinimumSize(QSize(80, 20));
		lineEditProbe3->setMaximumSize(QSize(80, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe9 = new QCheckBox(ReFreqClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(40, 470, 50, 20));
		checkBoxProbe9->setMinimumSize(QSize(50, 20));
		checkBoxProbe9->setMaximumSize(QSize(50, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe6 = new QLineEdit(ReFreqClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setGeometry(QRect(90, 350, 80, 20));
		lineEditProbe6->setMinimumSize(QSize(80, 20));
		lineEditProbe6->setMaximumSize(QSize(80, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe5 = new QCheckBox(ReFreqClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(40, 310, 50, 20));
		checkBoxProbe5->setMinimumSize(QSize(50, 20));
		checkBoxProbe5->setMaximumSize(QSize(50, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(ReFreqClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(40, 230, 50, 20));
		checkBoxProbe3->setMinimumSize(QSize(50, 20));
		checkBoxProbe3->setMaximumSize(QSize(50, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe5 = new QLineEdit(ReFreqClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setGeometry(QRect(90, 310, 80, 20));
		lineEditProbe5->setMinimumSize(QSize(80, 20));
		lineEditProbe5->setMaximumSize(QSize(80, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe13 = new QCheckBox(ReFreqClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(220, 230, 50, 20));
		checkBoxProbe13->setMinimumSize(QSize(50, 20));
		checkBoxProbe13->setMaximumSize(QSize(50, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe11 = new QLineEdit(ReFreqClass);
		lineEditProbe11->setObjectName(QString::fromUtf8("lineEditProbe11"));
		lineEditProbe11->setGeometry(QRect(280, 150, 80, 20));
		lineEditProbe11->setMinimumSize(QSize(80, 20));
		lineEditProbe11->setMaximumSize(QSize(80, 20));
		lineEditProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe17 = new QLineEdit(ReFreqClass);
		lineEditProbe17->setObjectName(QString::fromUtf8("lineEditProbe17"));
		lineEditProbe17->setGeometry(QRect(280, 390, 80, 20));
		lineEditProbe17->setMinimumSize(QSize(80, 20));
		lineEditProbe17->setMaximumSize(QSize(80, 20));
		lineEditProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe20 = new QLineEdit(ReFreqClass);
		lineEditProbe20->setObjectName(QString::fromUtf8("lineEditProbe20"));
		lineEditProbe20->setGeometry(QRect(280, 510, 80, 20));
		lineEditProbe20->setMinimumSize(QSize(80, 20));
		lineEditProbe20->setMaximumSize(QSize(80, 20));
		lineEditProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe13 = new QLineEdit(ReFreqClass);
		lineEditProbe13->setObjectName(QString::fromUtf8("lineEditProbe13"));
		lineEditProbe13->setGeometry(QRect(280, 230, 80, 20));
		lineEditProbe13->setMinimumSize(QSize(80, 20));
		lineEditProbe13->setMaximumSize(QSize(80, 20));
		lineEditProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe15 = new QCheckBox(ReFreqClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(220, 310, 50, 20));
		checkBoxProbe15->setMinimumSize(QSize(50, 20));
		checkBoxProbe15->setMaximumSize(QSize(50, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe16 = new QLineEdit(ReFreqClass);
		lineEditProbe16->setObjectName(QString::fromUtf8("lineEditProbe16"));
		lineEditProbe16->setGeometry(QRect(280, 350, 80, 20));
		lineEditProbe16->setMinimumSize(QSize(80, 20));
		lineEditProbe16->setMaximumSize(QSize(80, 20));
		lineEditProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe18 = new QLineEdit(ReFreqClass);
		lineEditProbe18->setObjectName(QString::fromUtf8("lineEditProbe18"));
		lineEditProbe18->setGeometry(QRect(280, 430, 80, 20));
		lineEditProbe18->setMinimumSize(QSize(80, 20));
		lineEditProbe18->setMaximumSize(QSize(80, 20));
		lineEditProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe17 = new QCheckBox(ReFreqClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(220, 390, 50, 20));
		checkBoxProbe17->setMinimumSize(QSize(50, 20));
		checkBoxProbe17->setMaximumSize(QSize(50, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe14 = new QLineEdit(ReFreqClass);
		lineEditProbe14->setObjectName(QString::fromUtf8("lineEditProbe14"));
		lineEditProbe14->setGeometry(QRect(280, 270, 80, 20));
		lineEditProbe14->setMinimumSize(QSize(80, 20));
		lineEditProbe14->setMaximumSize(QSize(80, 20));
		lineEditProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe20 = new QCheckBox(ReFreqClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(220, 510, 50, 20));
		checkBoxProbe20->setMinimumSize(QSize(50, 20));
		checkBoxProbe20->setMaximumSize(QSize(50, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe15 = new QLineEdit(ReFreqClass);
		lineEditProbe15->setObjectName(QString::fromUtf8("lineEditProbe15"));
		lineEditProbe15->setGeometry(QRect(280, 310, 80, 20));
		lineEditProbe15->setMinimumSize(QSize(80, 20));
		lineEditProbe15->setMaximumSize(QSize(80, 20));
		lineEditProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe18 = new QCheckBox(ReFreqClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(220, 430, 50, 20));
		checkBoxProbe18->setMinimumSize(QSize(50, 20));
		checkBoxProbe18->setMaximumSize(QSize(50, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe19 = new QLineEdit(ReFreqClass);
		lineEditProbe19->setObjectName(QString::fromUtf8("lineEditProbe19"));
		lineEditProbe19->setGeometry(QRect(280, 470, 80, 20));
		lineEditProbe19->setMinimumSize(QSize(80, 20));
		lineEditProbe19->setMaximumSize(QSize(80, 20));
		lineEditProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12 = new QLineEdit(ReFreqClass);
		lineEditProbe12->setObjectName(QString::fromUtf8("lineEditProbe12"));
		lineEditProbe12->setGeometry(QRect(280, 190, 80, 20));
		lineEditProbe12->setMinimumSize(QSize(80, 20));
		lineEditProbe12->setMaximumSize(QSize(80, 20));
		lineEditProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe19 = new QCheckBox(ReFreqClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(220, 470, 50, 20));
		checkBoxProbe19->setMinimumSize(QSize(50, 20));
		checkBoxProbe19->setMaximumSize(QSize(50, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe16 = new QCheckBox(ReFreqClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(220, 350, 50, 20));
		checkBoxProbe16->setMinimumSize(QSize(50, 20));
		checkBoxProbe16->setMaximumSize(QSize(50, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(ReFreqClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(220, 150, 50, 20));
		checkBoxProbe11->setMinimumSize(QSize(50, 20));
		checkBoxProbe11->setMaximumSize(QSize(50, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe14 = new QCheckBox(ReFreqClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(220, 270, 50, 20));
		checkBoxProbe14->setMinimumSize(QSize(50, 20));
		checkBoxProbe14->setMaximumSize(QSize(50, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe12 = new QCheckBox(ReFreqClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(220, 190, 50, 20));
		checkBoxProbe12->setMinimumSize(QSize(50, 20));
		checkBoxProbe12->setMaximumSize(QSize(50, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe27 = new QCheckBox(ReFreqClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(410, 390, 50, 20));
		checkBoxProbe27->setMinimumSize(QSize(50, 20));
		checkBoxProbe27->setMaximumSize(QSize(50, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe21 = new QCheckBox(ReFreqClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(410, 150, 50, 20));
		checkBoxProbe21->setMinimumSize(QSize(50, 20));
		checkBoxProbe21->setMaximumSize(QSize(50, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe26 = new QCheckBox(ReFreqClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(410, 350, 50, 20));
		checkBoxProbe26->setMinimumSize(QSize(50, 20));
		checkBoxProbe26->setMaximumSize(QSize(50, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25 = new QCheckBox(ReFreqClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(410, 310, 50, 20));
		checkBoxProbe25->setMinimumSize(QSize(50, 20));
		checkBoxProbe25->setMaximumSize(QSize(50, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe26 = new QLineEdit(ReFreqClass);
		lineEditProbe26->setObjectName(QString::fromUtf8("lineEditProbe26"));
		lineEditProbe26->setGeometry(QRect(470, 350, 80, 20));
		lineEditProbe26->setMinimumSize(QSize(80, 20));
		lineEditProbe26->setMaximumSize(QSize(80, 20));
		lineEditProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe25 = new QLineEdit(ReFreqClass);
		lineEditProbe25->setObjectName(QString::fromUtf8("lineEditProbe25"));
		lineEditProbe25->setGeometry(QRect(470, 310, 80, 20));
		lineEditProbe25->setMinimumSize(QSize(80, 20));
		lineEditProbe25->setMaximumSize(QSize(80, 20));
		lineEditProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe23 = new QCheckBox(ReFreqClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(410, 230, 50, 20));
		checkBoxProbe23->setMinimumSize(QSize(50, 20));
		checkBoxProbe23->setMaximumSize(QSize(50, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe23 = new QLineEdit(ReFreqClass);
		lineEditProbe23->setObjectName(QString::fromUtf8("lineEditProbe23"));
		lineEditProbe23->setGeometry(QRect(470, 230, 80, 20));
		lineEditProbe23->setMinimumSize(QSize(80, 20));
		lineEditProbe23->setMaximumSize(QSize(80, 20));
		lineEditProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe29 = new QLineEdit(ReFreqClass);
		lineEditProbe29->setObjectName(QString::fromUtf8("lineEditProbe29"));
		lineEditProbe29->setGeometry(QRect(470, 470, 80, 20));
		lineEditProbe29->setMinimumSize(QSize(80, 20));
		lineEditProbe29->setMaximumSize(QSize(80, 20));
		lineEditProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe28 = new QCheckBox(ReFreqClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(410, 430, 50, 20));
		checkBoxProbe28->setMinimumSize(QSize(50, 20));
		checkBoxProbe28->setMaximumSize(QSize(50, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe22 = new QCheckBox(ReFreqClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(410, 190, 50, 20));
		checkBoxProbe22->setMinimumSize(QSize(50, 20));
		checkBoxProbe22->setMaximumSize(QSize(50, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe28 = new QLineEdit(ReFreqClass);
		lineEditProbe28->setObjectName(QString::fromUtf8("lineEditProbe28"));
		lineEditProbe28->setGeometry(QRect(470, 430, 80, 20));
		lineEditProbe28->setMinimumSize(QSize(80, 20));
		lineEditProbe28->setMaximumSize(QSize(80, 20));
		lineEditProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe24 = new QCheckBox(ReFreqClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(410, 270, 50, 20));
		checkBoxProbe24->setMinimumSize(QSize(50, 20));
		checkBoxProbe24->setMaximumSize(QSize(50, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe22 = new QLineEdit(ReFreqClass);
		lineEditProbe22->setObjectName(QString::fromUtf8("lineEditProbe22"));
		lineEditProbe22->setGeometry(QRect(470, 190, 80, 20));
		lineEditProbe22->setMinimumSize(QSize(80, 20));
		lineEditProbe22->setMaximumSize(QSize(80, 20));
		lineEditProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe29 = new QCheckBox(ReFreqClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(410, 470, 50, 20));
		checkBoxProbe29->setMinimumSize(QSize(50, 20));
		checkBoxProbe29->setMaximumSize(QSize(50, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe30 = new QLineEdit(ReFreqClass);
		lineEditProbe30->setObjectName(QString::fromUtf8("lineEditProbe30"));
		lineEditProbe30->setGeometry(QRect(470, 510, 80, 20));
		lineEditProbe30->setMinimumSize(QSize(80, 20));
		lineEditProbe30->setMaximumSize(QSize(80, 20));
		lineEditProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe27 = new QLineEdit(ReFreqClass);
		lineEditProbe27->setObjectName(QString::fromUtf8("lineEditProbe27"));
		lineEditProbe27->setGeometry(QRect(470, 390, 80, 20));
		lineEditProbe27->setMinimumSize(QSize(80, 20));
		lineEditProbe27->setMaximumSize(QSize(80, 20));
		lineEditProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe30 = new QCheckBox(ReFreqClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(410, 510, 50, 20));
		checkBoxProbe30->setMinimumSize(QSize(50, 20));
		checkBoxProbe30->setMaximumSize(QSize(50, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe21 = new QLineEdit(ReFreqClass);
		lineEditProbe21->setObjectName(QString::fromUtf8("lineEditProbe21"));
		lineEditProbe21->setGeometry(QRect(470, 150, 80, 20));
		lineEditProbe21->setMinimumSize(QSize(80, 20));
		lineEditProbe21->setMaximumSize(QSize(80, 20));
		lineEditProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe24 = new QLineEdit(ReFreqClass);
		lineEditProbe24->setObjectName(QString::fromUtf8("lineEditProbe24"));
		lineEditProbe24->setGeometry(QRect(470, 270, 80, 20));
		lineEditProbe24->setMinimumSize(QSize(80, 20));
		lineEditProbe24->setMaximumSize(QSize(80, 20));
		lineEditProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonAnyIn = new QPushButton(ReFreqClass);
		pushButtonAnyIn->setObjectName(QString::fromUtf8("pushButtonAnyIn"));
		pushButtonAnyIn->setGeometry(QRect(260, 100, 50, 23));
		pushButtonAnyIn->setMinimumSize(QSize(50, 23));
		pushButtonAnyIn->setMaximumSize(QSize(50, 23));
		pushButtonAnyIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllIn = new QPushButton(ReFreqClass);
		pushButtonAllIn->setObjectName(QString::fromUtf8("pushButtonAllIn"));
		pushButtonAllIn->setGeometry(QRect(260, 60, 50, 23));
		pushButtonAllIn->setMinimumSize(QSize(50, 23));
		pushButtonAllIn->setMaximumSize(QSize(50, 23));
		pushButtonAllIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe32 = new QCheckBox(ReFreqClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(600, 190, 50, 20));
		checkBoxProbe32->setMinimumSize(QSize(50, 20));
		checkBoxProbe32->setMaximumSize(QSize(50, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_34 = new QLabel(ReFreqClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(740, 270, 30, 20));
		label_34->setMinimumSize(QSize(30, 20));
		label_34->setMaximumSize(QSize(30, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(ReFreqClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(600, 270, 50, 20));
		checkBoxProbe34->setMinimumSize(QSize(50, 20));
		checkBoxProbe34->setMaximumSize(QSize(50, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe36 = new QCheckBox(ReFreqClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(600, 350, 50, 20));
		checkBoxProbe36->setMinimumSize(QSize(50, 20));
		checkBoxProbe36->setMaximumSize(QSize(50, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_35 = new QLabel(ReFreqClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(740, 350, 30, 20));
		label_35->setMinimumSize(QSize(30, 20));
		label_35->setMaximumSize(QSize(30, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(ReFreqClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(740, 230, 30, 20));
		label_36->setMinimumSize(QSize(30, 20));
		label_36->setMaximumSize(QSize(30, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(ReFreqClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(600, 230, 50, 20));
		checkBoxProbe33->setMinimumSize(QSize(50, 20));
		checkBoxProbe33->setMaximumSize(QSize(50, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe34 = new QLineEdit(ReFreqClass);
		lineEditProbe34->setObjectName(QString::fromUtf8("lineEditProbe34"));
		lineEditProbe34->setGeometry(QRect(660, 270, 80, 20));
		lineEditProbe34->setMinimumSize(QSize(80, 20));
		lineEditProbe34->setMaximumSize(QSize(80, 20));
		lineEditProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe35 = new QCheckBox(ReFreqClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(600, 310, 50, 20));
		checkBoxProbe35->setMinimumSize(QSize(50, 20));
		checkBoxProbe35->setMaximumSize(QSize(50, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe32 = new QLineEdit(ReFreqClass);
		lineEditProbe32->setObjectName(QString::fromUtf8("lineEditProbe32"));
		lineEditProbe32->setGeometry(QRect(660, 190, 80, 20));
		lineEditProbe32->setMinimumSize(QSize(80, 20));
		lineEditProbe32->setMaximumSize(QSize(80, 20));
		lineEditProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_37 = new QLabel(ReFreqClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(740, 150, 30, 20));
		label_37->setMinimumSize(QSize(30, 20));
		label_37->setMaximumSize(QSize(30, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe35 = new QLineEdit(ReFreqClass);
		lineEditProbe35->setObjectName(QString::fromUtf8("lineEditProbe35"));
		lineEditProbe35->setGeometry(QRect(660, 310, 80, 20));
		lineEditProbe35->setMinimumSize(QSize(80, 20));
		lineEditProbe35->setMaximumSize(QSize(80, 20));
		lineEditProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe33 = new QLineEdit(ReFreqClass);
		lineEditProbe33->setObjectName(QString::fromUtf8("lineEditProbe33"));
		lineEditProbe33->setGeometry(QRect(660, 230, 80, 20));
		lineEditProbe33->setMinimumSize(QSize(80, 20));
		lineEditProbe33->setMaximumSize(QSize(80, 20));
		lineEditProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_38 = new QLabel(ReFreqClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(740, 190, 30, 20));
		label_38->setMinimumSize(QSize(30, 20));
		label_38->setMaximumSize(QSize(30, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(ReFreqClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(600, 150, 50, 20));
		checkBoxProbe31->setMinimumSize(QSize(50, 20));
		checkBoxProbe31->setMaximumSize(QSize(50, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe31 = new QLineEdit(ReFreqClass);
		lineEditProbe31->setObjectName(QString::fromUtf8("lineEditProbe31"));
		lineEditProbe31->setGeometry(QRect(660, 150, 80, 20));
		lineEditProbe31->setMinimumSize(QSize(80, 20));
		lineEditProbe31->setMaximumSize(QSize(80, 20));
		lineEditProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe36 = new QLineEdit(ReFreqClass);
		lineEditProbe36->setObjectName(QString::fromUtf8("lineEditProbe36"));
		lineEditProbe36->setGeometry(QRect(660, 350, 80, 20));
		lineEditProbe36->setMinimumSize(QSize(80, 20));
		lineEditProbe36->setMaximumSize(QSize(80, 20));
		lineEditProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_39 = new QLabel(ReFreqClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(740, 310, 30, 20));
		label_39->setMinimumSize(QSize(30, 20));
		label_39->setMaximumSize(QSize(30, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));

		retranslateUi(ReFreqClass);

		QMetaObject::connectSlotsByName(ReFreqClass);
	} // setupUi

	void retranslateUi(QDialog *ReFreqClass)
	{
		ReFreqClass->setWindowTitle(QApplication::translate("ReFreqClass", "ReFreq", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("ReFreqClass", "\351\207\215\345\244\215\351\242\221\347\216\207\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("ReFreqClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("ReFreqClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_31->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_32->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_33->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("ReFreqClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("ReFreqClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("ReFreqClass", " 8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("ReFreqClass", " 1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("ReFreqClass", " 10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("ReFreqClass", " 4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("ReFreqClass", " 6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("ReFreqClass", " 7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("ReFreqClass", " 2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("ReFreqClass", " 9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("ReFreqClass", " 5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("ReFreqClass", " 3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("ReFreqClass", " 13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("ReFreqClass", " 15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("ReFreqClass", " 17", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("ReFreqClass", " 20", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("ReFreqClass", " 18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("ReFreqClass", " 19", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("ReFreqClass", " 16", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("ReFreqClass", " 11", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("ReFreqClass", " 14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("ReFreqClass", " 12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("ReFreqClass", " 27", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("ReFreqClass", " 21", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("ReFreqClass", " 26", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("ReFreqClass", " 25", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("ReFreqClass", " 23", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("ReFreqClass", " 28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("ReFreqClass", " 22", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("ReFreqClass", " 24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("ReFreqClass", " 29", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("ReFreqClass", " 30", 0, QApplication::UnicodeUTF8));
		pushButtonAnyIn->setText(QApplication::translate("ReFreqClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllIn->setText(QApplication::translate("ReFreqClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("ReFreqClass", " 32", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("ReFreqClass", " 34", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("ReFreqClass", " 36", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("ReFreqClass", " 33", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("ReFreqClass", " 35", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("ReFreqClass", " 31", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("ReFreqClass", "(Hz)", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class ReFreqClass: public Ui_ReFreqClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFREQ_H




/********************************************************************************
** Form generated from reading UI file 'soundspeed.ui'
**
** Created: Thu Jun 26 18:42:24 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOUNDSPEED_H
#define UI_SOUNDSPEED_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_SoundSpeedClass
{
public:
	QLabel *label_3;
	QLabel *label;
	QLabel *label_2;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QLabel *label_4;
	QLabel *label_5;
	QLabel *label_6;
	QLabel *label_7;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QLabel *label_14;
	QLabel *label_15;
	QLabel *label_16;
	QLabel *label_17;
	QLabel *label_18;
	QLabel *label_19;
	QLabel *label_20;
	QLabel *label_21;
	QLabel *label_22;
	QLabel *label_23;
	QLabel *label_24;
	QLabel *label_25;
	QLabel *label_26;
	QLabel *label_27;
	QLabel *label_28;
	QLabel *label_29;
	QLabel *label_30;
	QLabel *label_31;
	QLabel *label_32;
	QLabel *label_33;
	QLabel *label_34;
	QLineEdit *lineEditSpeed;
	QLabel *label_35;
	QLineEdit *lineEditAll;
	QLineEdit *lineEditAny;
	QRadioButton *radioButtonAll;
	QRadioButton *radioButtonAny;
	QLineEdit *lineEditProbe10;
	QLineEdit *lineEditProbe7;
	QLineEdit *lineEditProbe2;
	QLineEdit *lineEditProbe9;
	QLineEdit *lineEditProbe8;
	QCheckBox *checkBoxProbe8;
	QCheckBox *checkBoxProbe1;
	QCheckBox *checkBoxProbe10;
	QCheckBox *checkBoxProbe4;
	QLineEdit *lineEditProbe4;
	QCheckBox *checkBoxProbe6;
	QLineEdit *lineEditProbe1;
	QCheckBox *checkBoxProbe7;
	QCheckBox *checkBoxProbe2;
	QLineEdit *lineEditProbe3;
	QCheckBox *checkBoxProbe9;
	QLineEdit *lineEditProbe6;
	QCheckBox *checkBoxProbe5;
	QCheckBox *checkBoxProbe3;
	QLineEdit *lineEditProbe5;
	QCheckBox *checkBoxProbe13;
	QLineEdit *lineEditProbe11;
	QLineEdit *lineEditProbe17;
	QLineEdit *lineEditProbe20;
	QLineEdit *lineEditProbe13;
	QCheckBox *checkBoxProbe15;
	QLineEdit *lineEditProbe16;
	QLineEdit *lineEditProbe18;
	QCheckBox *checkBoxProbe17;
	QLineEdit *lineEditProbe14;
	QCheckBox *checkBoxProbe20;
	QLineEdit *lineEditProbe15;
	QCheckBox *checkBoxProbe18;
	QLineEdit *lineEditProbe19;
	QLineEdit *lineEditProbe12;
	QCheckBox *checkBoxProbe19;
	QCheckBox *checkBoxProbe16;
	QCheckBox *checkBoxProbe11;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe12;
	QCheckBox *checkBoxProbe27;
	QCheckBox *checkBoxProbe21;
	QCheckBox *checkBoxProbe26;
	QCheckBox *checkBoxProbe25;
	QLineEdit *lineEditProbe26;
	QLineEdit *lineEditProbe25;
	QCheckBox *checkBoxProbe23;
	QLineEdit *lineEditProbe23;
	QLineEdit *lineEditProbe29;
	QCheckBox *checkBoxProbe28;
	QCheckBox *checkBoxProbe22;
	QLineEdit *lineEditProbe28;
	QCheckBox *checkBoxProbe24;
	QLineEdit *lineEditProbe22;
	QCheckBox *checkBoxProbe29;
	QLineEdit *lineEditProbe30;
	QLineEdit *lineEditProbe27;
	QCheckBox *checkBoxProbe30;
	QLineEdit *lineEditProbe21;
	QLineEdit *lineEditProbe24;
	QPushButton *pushButtonAllIn;
	QPushButton *pushButtonAnyIn;
	QLabel *label_36;
	QLineEdit *lineEditProbe34;
	QCheckBox *checkBoxProbe36;
	QCheckBox *checkBoxProbe32;
	QLineEdit *lineEditProbe36;
	QLabel *label_37;
	QCheckBox *checkBoxProbe31;
	QCheckBox *checkBoxProbe33;
	QLineEdit *lineEditProbe32;
	QLineEdit *lineEditProbe33;
	QLabel *label_38;
	QCheckBox *checkBoxProbe35;
	QLabel *label_39;
	QLineEdit *lineEditProbe31;
	QLineEdit *lineEditProbe35;
	QLabel *label_40;
	QLabel *label_41;
	QCheckBox *checkBoxProbe34;

	void setupUi(QDialog *SoundSpeedClass)
	{
		if (SoundSpeedClass->objectName().isEmpty())
			SoundSpeedClass->setObjectName(QString::fromUtf8("SoundSpeedClass"));
		SoundSpeedClass->resize(800, 600);
		SoundSpeedClass->setMinimumSize(QSize(800, 600));
		SoundSpeedClass->setMaximumSize(QSize(800, 600));
		SoundSpeedClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label_3 = new QLabel(SoundSpeedClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(206, 110, 30, 20));
		label_3->setMinimumSize(QSize(30, 20));
		label_3->setMaximumSize(QSize(30, 20));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label = new QLabel(SoundSpeedClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(206, 70, 30, 20));
		label->setMinimumSize(QSize(30, 20));
		label->setMaximumSize(QSize(30, 20));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_2 = new QLabel(SoundSpeedClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(330, 20, 130, 20));
		label_2->setMinimumSize(QSize(130, 20));
		label_2->setMaximumSize(QSize(130, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(SoundSpeedClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(580, 560, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(SoundSpeedClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(680, 560, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(SoundSpeedClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(165, 160, 30, 20));
		label_4->setMinimumSize(QSize(30, 20));
		label_4->setMaximumSize(QSize(30, 20));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_5 = new QLabel(SoundSpeedClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(165, 200, 30, 20));
		label_5->setMinimumSize(QSize(30, 20));
		label_5->setMaximumSize(QSize(30, 20));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_6 = new QLabel(SoundSpeedClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(165, 240, 30, 20));
		label_6->setMinimumSize(QSize(30, 20));
		label_6->setMaximumSize(QSize(30, 20));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(SoundSpeedClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(165, 280, 30, 20));
		label_7->setMinimumSize(QSize(30, 20));
		label_7->setMaximumSize(QSize(30, 20));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(SoundSpeedClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(165, 320, 30, 20));
		label_8->setMinimumSize(QSize(30, 20));
		label_8->setMaximumSize(QSize(30, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(SoundSpeedClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(165, 360, 30, 20));
		label_9->setMinimumSize(QSize(30, 20));
		label_9->setMaximumSize(QSize(30, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(SoundSpeedClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(165, 400, 30, 20));
		label_10->setMinimumSize(QSize(30, 20));
		label_10->setMaximumSize(QSize(30, 20));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_11 = new QLabel(SoundSpeedClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(165, 440, 30, 20));
		label_11->setMinimumSize(QSize(30, 20));
		label_11->setMaximumSize(QSize(30, 20));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_12 = new QLabel(SoundSpeedClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(165, 480, 30, 20));
		label_12->setMinimumSize(QSize(30, 20));
		label_12->setMaximumSize(QSize(30, 20));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_13 = new QLabel(SoundSpeedClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(165, 520, 30, 20));
		label_13->setMinimumSize(QSize(30, 20));
		label_13->setMaximumSize(QSize(30, 20));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_14 = new QLabel(SoundSpeedClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(355, 520, 30, 20));
		label_14->setMinimumSize(QSize(30, 20));
		label_14->setMaximumSize(QSize(30, 20));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_15 = new QLabel(SoundSpeedClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(355, 360, 30, 20));
		label_15->setMinimumSize(QSize(30, 20));
		label_15->setMaximumSize(QSize(30, 20));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_16 = new QLabel(SoundSpeedClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(355, 240, 30, 20));
		label_16->setMinimumSize(QSize(30, 20));
		label_16->setMaximumSize(QSize(30, 20));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_17 = new QLabel(SoundSpeedClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(355, 160, 30, 20));
		label_17->setMinimumSize(QSize(30, 20));
		label_17->setMaximumSize(QSize(30, 20));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_18 = new QLabel(SoundSpeedClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(355, 400, 30, 20));
		label_18->setMinimumSize(QSize(30, 20));
		label_18->setMaximumSize(QSize(30, 20));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_19 = new QLabel(SoundSpeedClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(355, 200, 30, 20));
		label_19->setMinimumSize(QSize(30, 20));
		label_19->setMaximumSize(QSize(30, 20));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(SoundSpeedClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(355, 440, 30, 20));
		label_20->setMinimumSize(QSize(30, 20));
		label_20->setMaximumSize(QSize(30, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_21 = new QLabel(SoundSpeedClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(355, 280, 30, 20));
		label_21->setMinimumSize(QSize(30, 20));
		label_21->setMaximumSize(QSize(30, 20));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_22 = new QLabel(SoundSpeedClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(355, 320, 30, 20));
		label_22->setMinimumSize(QSize(30, 20));
		label_22->setMaximumSize(QSize(30, 20));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(SoundSpeedClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(355, 480, 30, 20));
		label_23->setMinimumSize(QSize(30, 20));
		label_23->setMaximumSize(QSize(30, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_24 = new QLabel(SoundSpeedClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(545, 480, 30, 20));
		label_24->setMinimumSize(QSize(30, 20));
		label_24->setMaximumSize(QSize(30, 20));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_25 = new QLabel(SoundSpeedClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(545, 520, 30, 20));
		label_25->setMinimumSize(QSize(30, 20));
		label_25->setMaximumSize(QSize(30, 20));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_26 = new QLabel(SoundSpeedClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(545, 240, 30, 20));
		label_26->setMinimumSize(QSize(30, 20));
		label_26->setMaximumSize(QSize(30, 20));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_27 = new QLabel(SoundSpeedClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(545, 160, 30, 20));
		label_27->setMinimumSize(QSize(30, 20));
		label_27->setMaximumSize(QSize(30, 20));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_28 = new QLabel(SoundSpeedClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(545, 280, 30, 20));
		label_28->setMinimumSize(QSize(30, 20));
		label_28->setMaximumSize(QSize(30, 20));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(SoundSpeedClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(545, 360, 30, 20));
		label_29->setMinimumSize(QSize(30, 20));
		label_29->setMaximumSize(QSize(30, 20));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_30 = new QLabel(SoundSpeedClass);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(545, 320, 30, 20));
		label_30->setMinimumSize(QSize(30, 20));
		label_30->setMaximumSize(QSize(30, 20));
		label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_31 = new QLabel(SoundSpeedClass);
		label_31->setObjectName(QString::fromUtf8("label_31"));
		label_31->setGeometry(QRect(545, 440, 30, 20));
		label_31->setMinimumSize(QSize(30, 20));
		label_31->setMaximumSize(QSize(30, 20));
		label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_32 = new QLabel(SoundSpeedClass);
		label_32->setObjectName(QString::fromUtf8("label_32"));
		label_32->setGeometry(QRect(545, 400, 30, 20));
		label_32->setMinimumSize(QSize(30, 20));
		label_32->setMaximumSize(QSize(30, 20));
		label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_33 = new QLabel(SoundSpeedClass);
		label_33->setObjectName(QString::fromUtf8("label_33"));
		label_33->setGeometry(QRect(545, 200, 30, 20));
		label_33->setMinimumSize(QSize(30, 20));
		label_33->setMaximumSize(QSize(30, 20));
		label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_34 = new QLabel(SoundSpeedClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(585, 70, 50, 20));
		label_34->setMinimumSize(QSize(50, 20));
		label_34->setMaximumSize(QSize(50, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditSpeed = new QLineEdit(SoundSpeedClass);
		lineEditSpeed->setObjectName(QString::fromUtf8("lineEditSpeed"));
		lineEditSpeed->setGeometry(QRect(645, 70, 80, 20));
		lineEditSpeed->setMinimumSize(QSize(80, 20));
		lineEditSpeed->setMaximumSize(QSize(80, 20));
		lineEditSpeed->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_35 = new QLabel(SoundSpeedClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(730, 70, 30, 20));
		label_35->setMinimumSize(QSize(30, 20));
		label_35->setMaximumSize(QSize(30, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditAll = new QLineEdit(SoundSpeedClass);
		lineEditAll->setObjectName(QString::fromUtf8("lineEditAll"));
		lineEditAll->setGeometry(QRect(120, 70, 80, 20));
		lineEditAll->setMinimumSize(QSize(80, 20));
		lineEditAll->setMaximumSize(QSize(80, 20));
		lineEditAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAny = new QLineEdit(SoundSpeedClass);
		lineEditAny->setObjectName(QString::fromUtf8("lineEditAny"));
		lineEditAny->setGeometry(QRect(120, 110, 80, 20));
		lineEditAny->setMinimumSize(QSize(80, 20));
		lineEditAny->setMaximumSize(QSize(80, 20));
		lineEditAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		radioButtonAll = new QRadioButton(SoundSpeedClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(30, 70, 89, 16));
		radioButtonAll->setMinimumSize(QSize(89, 16));
		radioButtonAll->setMaximumSize(QSize(89, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAny = new QRadioButton(SoundSpeedClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(30, 110, 89, 16));
		radioButtonAny->setMinimumSize(QSize(89, 16));
		radioButtonAny->setMaximumSize(QSize(89, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe10 = new QLineEdit(SoundSpeedClass);
		lineEditProbe10->setObjectName(QString::fromUtf8("lineEditProbe10"));
		lineEditProbe10->setGeometry(QRect(80, 520, 80, 20));
		lineEditProbe10->setMinimumSize(QSize(80, 20));
		lineEditProbe10->setMaximumSize(QSize(80, 20));
		lineEditProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe7 = new QLineEdit(SoundSpeedClass);
		lineEditProbe7->setObjectName(QString::fromUtf8("lineEditProbe7"));
		lineEditProbe7->setGeometry(QRect(80, 400, 80, 20));
		lineEditProbe7->setMinimumSize(QSize(80, 20));
		lineEditProbe7->setMaximumSize(QSize(80, 20));
		lineEditProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe2 = new QLineEdit(SoundSpeedClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setGeometry(QRect(80, 200, 80, 20));
		lineEditProbe2->setMinimumSize(QSize(80, 20));
		lineEditProbe2->setMaximumSize(QSize(80, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe9 = new QLineEdit(SoundSpeedClass);
		lineEditProbe9->setObjectName(QString::fromUtf8("lineEditProbe9"));
		lineEditProbe9->setGeometry(QRect(80, 480, 80, 20));
		lineEditProbe9->setMinimumSize(QSize(80, 20));
		lineEditProbe9->setMaximumSize(QSize(80, 20));
		lineEditProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe8 = new QLineEdit(SoundSpeedClass);
		lineEditProbe8->setObjectName(QString::fromUtf8("lineEditProbe8"));
		lineEditProbe8->setGeometry(QRect(80, 440, 80, 20));
		lineEditProbe8->setMinimumSize(QSize(80, 20));
		lineEditProbe8->setMaximumSize(QSize(80, 20));
		lineEditProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe8 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(30, 440, 50, 20));
		checkBoxProbe8->setMinimumSize(QSize(50, 20));
		checkBoxProbe8->setMaximumSize(QSize(50, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe1 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(30, 160, 50, 20));
		checkBoxProbe1->setMinimumSize(QSize(50, 20));
		checkBoxProbe1->setMaximumSize(QSize(50, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe10 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(30, 520, 50, 20));
		checkBoxProbe10->setMinimumSize(QSize(50, 20));
		checkBoxProbe10->setMaximumSize(QSize(50, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe4 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(30, 280, 50, 20));
		checkBoxProbe4->setMinimumSize(QSize(50, 20));
		checkBoxProbe4->setMaximumSize(QSize(50, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe4 = new QLineEdit(SoundSpeedClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setGeometry(QRect(80, 280, 80, 20));
		lineEditProbe4->setMinimumSize(QSize(80, 20));
		lineEditProbe4->setMaximumSize(QSize(80, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe6 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(30, 360, 50, 20));
		checkBoxProbe6->setMinimumSize(QSize(50, 20));
		checkBoxProbe6->setMaximumSize(QSize(50, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe1 = new QLineEdit(SoundSpeedClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setGeometry(QRect(80, 160, 80, 20));
		lineEditProbe1->setMinimumSize(QSize(80, 20));
		lineEditProbe1->setMaximumSize(QSize(80, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe7 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(30, 400, 50, 20));
		checkBoxProbe7->setMinimumSize(QSize(50, 20));
		checkBoxProbe7->setMaximumSize(QSize(50, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe2 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(30, 200, 50, 20));
		checkBoxProbe2->setMinimumSize(QSize(50, 20));
		checkBoxProbe2->setMaximumSize(QSize(50, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe3 = new QLineEdit(SoundSpeedClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setGeometry(QRect(80, 240, 80, 20));
		lineEditProbe3->setMinimumSize(QSize(80, 20));
		lineEditProbe3->setMaximumSize(QSize(80, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe9 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(30, 480, 50, 20));
		checkBoxProbe9->setMinimumSize(QSize(50, 20));
		checkBoxProbe9->setMaximumSize(QSize(50, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe6 = new QLineEdit(SoundSpeedClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setGeometry(QRect(80, 360, 80, 20));
		lineEditProbe6->setMinimumSize(QSize(80, 20));
		lineEditProbe6->setMaximumSize(QSize(80, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe5 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(30, 320, 50, 20));
		checkBoxProbe5->setMinimumSize(QSize(50, 20));
		checkBoxProbe5->setMaximumSize(QSize(50, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(30, 240, 50, 20));
		checkBoxProbe3->setMinimumSize(QSize(50, 20));
		checkBoxProbe3->setMaximumSize(QSize(50, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe5 = new QLineEdit(SoundSpeedClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setGeometry(QRect(80, 320, 80, 20));
		lineEditProbe5->setMinimumSize(QSize(80, 20));
		lineEditProbe5->setMaximumSize(QSize(80, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe13 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(210, 240, 50, 20));
		checkBoxProbe13->setMinimumSize(QSize(50, 20));
		checkBoxProbe13->setMaximumSize(QSize(50, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe11 = new QLineEdit(SoundSpeedClass);
		lineEditProbe11->setObjectName(QString::fromUtf8("lineEditProbe11"));
		lineEditProbe11->setGeometry(QRect(270, 160, 80, 20));
		lineEditProbe11->setMinimumSize(QSize(80, 20));
		lineEditProbe11->setMaximumSize(QSize(80, 20));
		lineEditProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe17 = new QLineEdit(SoundSpeedClass);
		lineEditProbe17->setObjectName(QString::fromUtf8("lineEditProbe17"));
		lineEditProbe17->setGeometry(QRect(270, 400, 80, 20));
		lineEditProbe17->setMinimumSize(QSize(80, 20));
		lineEditProbe17->setMaximumSize(QSize(80, 20));
		lineEditProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe20 = new QLineEdit(SoundSpeedClass);
		lineEditProbe20->setObjectName(QString::fromUtf8("lineEditProbe20"));
		lineEditProbe20->setGeometry(QRect(270, 520, 80, 20));
		lineEditProbe20->setMinimumSize(QSize(80, 20));
		lineEditProbe20->setMaximumSize(QSize(80, 20));
		lineEditProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe13 = new QLineEdit(SoundSpeedClass);
		lineEditProbe13->setObjectName(QString::fromUtf8("lineEditProbe13"));
		lineEditProbe13->setGeometry(QRect(270, 240, 80, 20));
		lineEditProbe13->setMinimumSize(QSize(80, 20));
		lineEditProbe13->setMaximumSize(QSize(80, 20));
		lineEditProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe15 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(210, 320, 50, 20));
		checkBoxProbe15->setMinimumSize(QSize(50, 20));
		checkBoxProbe15->setMaximumSize(QSize(50, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe16 = new QLineEdit(SoundSpeedClass);
		lineEditProbe16->setObjectName(QString::fromUtf8("lineEditProbe16"));
		lineEditProbe16->setGeometry(QRect(270, 360, 80, 20));
		lineEditProbe16->setMinimumSize(QSize(80, 20));
		lineEditProbe16->setMaximumSize(QSize(80, 20));
		lineEditProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe18 = new QLineEdit(SoundSpeedClass);
		lineEditProbe18->setObjectName(QString::fromUtf8("lineEditProbe18"));
		lineEditProbe18->setGeometry(QRect(270, 440, 80, 20));
		lineEditProbe18->setMinimumSize(QSize(80, 20));
		lineEditProbe18->setMaximumSize(QSize(80, 20));
		lineEditProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe17 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(210, 400, 50, 20));
		checkBoxProbe17->setMinimumSize(QSize(50, 20));
		checkBoxProbe17->setMaximumSize(QSize(50, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe14 = new QLineEdit(SoundSpeedClass);
		lineEditProbe14->setObjectName(QString::fromUtf8("lineEditProbe14"));
		lineEditProbe14->setGeometry(QRect(270, 280, 80, 20));
		lineEditProbe14->setMinimumSize(QSize(80, 20));
		lineEditProbe14->setMaximumSize(QSize(80, 20));
		lineEditProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe20 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(210, 520, 50, 20));
		checkBoxProbe20->setMinimumSize(QSize(50, 20));
		checkBoxProbe20->setMaximumSize(QSize(50, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe15 = new QLineEdit(SoundSpeedClass);
		lineEditProbe15->setObjectName(QString::fromUtf8("lineEditProbe15"));
		lineEditProbe15->setGeometry(QRect(270, 320, 80, 20));
		lineEditProbe15->setMinimumSize(QSize(80, 20));
		lineEditProbe15->setMaximumSize(QSize(80, 20));
		lineEditProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe18 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(210, 440, 50, 20));
		checkBoxProbe18->setMinimumSize(QSize(50, 20));
		checkBoxProbe18->setMaximumSize(QSize(50, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe19 = new QLineEdit(SoundSpeedClass);
		lineEditProbe19->setObjectName(QString::fromUtf8("lineEditProbe19"));
		lineEditProbe19->setGeometry(QRect(270, 480, 80, 20));
		lineEditProbe19->setMinimumSize(QSize(80, 20));
		lineEditProbe19->setMaximumSize(QSize(80, 20));
		lineEditProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe12 = new QLineEdit(SoundSpeedClass);
		lineEditProbe12->setObjectName(QString::fromUtf8("lineEditProbe12"));
		lineEditProbe12->setGeometry(QRect(270, 200, 80, 20));
		lineEditProbe12->setMinimumSize(QSize(80, 20));
		lineEditProbe12->setMaximumSize(QSize(80, 20));
		lineEditProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe19 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(210, 480, 50, 20));
		checkBoxProbe19->setMinimumSize(QSize(50, 20));
		checkBoxProbe19->setMaximumSize(QSize(50, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe16 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(210, 360, 50, 20));
		checkBoxProbe16->setMinimumSize(QSize(50, 20));
		checkBoxProbe16->setMaximumSize(QSize(50, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(210, 160, 50, 20));
		checkBoxProbe11->setMinimumSize(QSize(50, 20));
		checkBoxProbe11->setMaximumSize(QSize(50, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe14 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(210, 280, 50, 20));
		checkBoxProbe14->setMinimumSize(QSize(50, 20));
		checkBoxProbe14->setMaximumSize(QSize(50, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe12 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(210, 200, 50, 20));
		checkBoxProbe12->setMinimumSize(QSize(50, 20));
		checkBoxProbe12->setMaximumSize(QSize(50, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe27 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(400, 400, 50, 20));
		checkBoxProbe27->setMinimumSize(QSize(50, 20));
		checkBoxProbe27->setMaximumSize(QSize(50, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe21 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(400, 160, 50, 20));
		checkBoxProbe21->setMinimumSize(QSize(50, 20));
		checkBoxProbe21->setMaximumSize(QSize(50, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe26 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(400, 360, 50, 20));
		checkBoxProbe26->setMinimumSize(QSize(50, 20));
		checkBoxProbe26->setMaximumSize(QSize(50, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(400, 320, 50, 20));
		checkBoxProbe25->setMinimumSize(QSize(50, 20));
		checkBoxProbe25->setMaximumSize(QSize(50, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe26 = new QLineEdit(SoundSpeedClass);
		lineEditProbe26->setObjectName(QString::fromUtf8("lineEditProbe26"));
		lineEditProbe26->setGeometry(QRect(460, 360, 80, 20));
		lineEditProbe26->setMinimumSize(QSize(80, 20));
		lineEditProbe26->setMaximumSize(QSize(80, 20));
		lineEditProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe25 = new QLineEdit(SoundSpeedClass);
		lineEditProbe25->setObjectName(QString::fromUtf8("lineEditProbe25"));
		lineEditProbe25->setGeometry(QRect(460, 320, 80, 20));
		lineEditProbe25->setMinimumSize(QSize(80, 20));
		lineEditProbe25->setMaximumSize(QSize(80, 20));
		lineEditProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe23 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(400, 240, 50, 20));
		checkBoxProbe23->setMinimumSize(QSize(50, 20));
		checkBoxProbe23->setMaximumSize(QSize(50, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe23 = new QLineEdit(SoundSpeedClass);
		lineEditProbe23->setObjectName(QString::fromUtf8("lineEditProbe23"));
		lineEditProbe23->setGeometry(QRect(460, 240, 80, 20));
		lineEditProbe23->setMinimumSize(QSize(80, 20));
		lineEditProbe23->setMaximumSize(QSize(80, 20));
		lineEditProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe29 = new QLineEdit(SoundSpeedClass);
		lineEditProbe29->setObjectName(QString::fromUtf8("lineEditProbe29"));
		lineEditProbe29->setGeometry(QRect(460, 480, 80, 20));
		lineEditProbe29->setMinimumSize(QSize(80, 20));
		lineEditProbe29->setMaximumSize(QSize(80, 20));
		lineEditProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe28 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(400, 440, 50, 20));
		checkBoxProbe28->setMinimumSize(QSize(50, 20));
		checkBoxProbe28->setMaximumSize(QSize(50, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe22 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(400, 200, 50, 20));
		checkBoxProbe22->setMinimumSize(QSize(50, 20));
		checkBoxProbe22->setMaximumSize(QSize(50, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe28 = new QLineEdit(SoundSpeedClass);
		lineEditProbe28->setObjectName(QString::fromUtf8("lineEditProbe28"));
		lineEditProbe28->setGeometry(QRect(460, 440, 80, 20));
		lineEditProbe28->setMinimumSize(QSize(80, 20));
		lineEditProbe28->setMaximumSize(QSize(80, 20));
		lineEditProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe24 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(400, 280, 50, 20));
		checkBoxProbe24->setMinimumSize(QSize(50, 20));
		checkBoxProbe24->setMaximumSize(QSize(50, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe22 = new QLineEdit(SoundSpeedClass);
		lineEditProbe22->setObjectName(QString::fromUtf8("lineEditProbe22"));
		lineEditProbe22->setGeometry(QRect(460, 200, 80, 20));
		lineEditProbe22->setMinimumSize(QSize(80, 20));
		lineEditProbe22->setMaximumSize(QSize(80, 20));
		lineEditProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe29 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(400, 480, 50, 20));
		checkBoxProbe29->setMinimumSize(QSize(50, 20));
		checkBoxProbe29->setMaximumSize(QSize(50, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe30 = new QLineEdit(SoundSpeedClass);
		lineEditProbe30->setObjectName(QString::fromUtf8("lineEditProbe30"));
		lineEditProbe30->setGeometry(QRect(460, 520, 80, 20));
		lineEditProbe30->setMinimumSize(QSize(80, 20));
		lineEditProbe30->setMaximumSize(QSize(80, 20));
		lineEditProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe27 = new QLineEdit(SoundSpeedClass);
		lineEditProbe27->setObjectName(QString::fromUtf8("lineEditProbe27"));
		lineEditProbe27->setGeometry(QRect(460, 400, 80, 20));
		lineEditProbe27->setMinimumSize(QSize(80, 20));
		lineEditProbe27->setMaximumSize(QSize(80, 20));
		lineEditProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe30 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(400, 520, 50, 20));
		checkBoxProbe30->setMinimumSize(QSize(50, 20));
		checkBoxProbe30->setMaximumSize(QSize(50, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe21 = new QLineEdit(SoundSpeedClass);
		lineEditProbe21->setObjectName(QString::fromUtf8("lineEditProbe21"));
		lineEditProbe21->setGeometry(QRect(460, 160, 80, 20));
		lineEditProbe21->setMinimumSize(QSize(80, 20));
		lineEditProbe21->setMaximumSize(QSize(80, 20));
		lineEditProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe24 = new QLineEdit(SoundSpeedClass);
		lineEditProbe24->setObjectName(QString::fromUtf8("lineEditProbe24"));
		lineEditProbe24->setGeometry(QRect(460, 280, 80, 20));
		lineEditProbe24->setMinimumSize(QSize(80, 20));
		lineEditProbe24->setMaximumSize(QSize(80, 20));
		lineEditProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonAllIn = new QPushButton(SoundSpeedClass);
		pushButtonAllIn->setObjectName(QString::fromUtf8("pushButtonAllIn"));
		pushButtonAllIn->setGeometry(QRect(250, 70, 50, 23));
		pushButtonAllIn->setMinimumSize(QSize(50, 23));
		pushButtonAllIn->setMaximumSize(QSize(50, 23));
		pushButtonAllIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyIn = new QPushButton(SoundSpeedClass);
		pushButtonAnyIn->setObjectName(QString::fromUtf8("pushButtonAnyIn"));
		pushButtonAnyIn->setGeometry(QRect(250, 110, 50, 23));
		pushButtonAnyIn->setMinimumSize(QSize(50, 23));
		pushButtonAnyIn->setMaximumSize(QSize(50, 23));
		pushButtonAnyIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(SoundSpeedClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(735, 240, 30, 20));
		label_36->setMinimumSize(QSize(30, 20));
		label_36->setMaximumSize(QSize(30, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe34 = new QLineEdit(SoundSpeedClass);
		lineEditProbe34->setObjectName(QString::fromUtf8("lineEditProbe34"));
		lineEditProbe34->setGeometry(QRect(650, 280, 80, 20));
		lineEditProbe34->setMinimumSize(QSize(80, 20));
		lineEditProbe34->setMaximumSize(QSize(80, 20));
		lineEditProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe36 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(590, 360, 50, 20));
		checkBoxProbe36->setMinimumSize(QSize(50, 20));
		checkBoxProbe36->setMaximumSize(QSize(50, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe32 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(590, 200, 50, 20));
		checkBoxProbe32->setMinimumSize(QSize(50, 20));
		checkBoxProbe32->setMaximumSize(QSize(50, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe36 = new QLineEdit(SoundSpeedClass);
		lineEditProbe36->setObjectName(QString::fromUtf8("lineEditProbe36"));
		lineEditProbe36->setGeometry(QRect(650, 360, 80, 20));
		lineEditProbe36->setMinimumSize(QSize(80, 20));
		lineEditProbe36->setMaximumSize(QSize(80, 20));
		lineEditProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_37 = new QLabel(SoundSpeedClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(735, 160, 30, 20));
		label_37->setMinimumSize(QSize(30, 20));
		label_37->setMaximumSize(QSize(30, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(590, 160, 50, 20));
		checkBoxProbe31->setMinimumSize(QSize(50, 20));
		checkBoxProbe31->setMaximumSize(QSize(50, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(590, 240, 50, 20));
		checkBoxProbe33->setMinimumSize(QSize(50, 20));
		checkBoxProbe33->setMaximumSize(QSize(50, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe32 = new QLineEdit(SoundSpeedClass);
		lineEditProbe32->setObjectName(QString::fromUtf8("lineEditProbe32"));
		lineEditProbe32->setGeometry(QRect(650, 200, 80, 20));
		lineEditProbe32->setMinimumSize(QSize(80, 20));
		lineEditProbe32->setMaximumSize(QSize(80, 20));
		lineEditProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe33 = new QLineEdit(SoundSpeedClass);
		lineEditProbe33->setObjectName(QString::fromUtf8("lineEditProbe33"));
		lineEditProbe33->setGeometry(QRect(650, 240, 80, 20));
		lineEditProbe33->setMinimumSize(QSize(80, 20));
		lineEditProbe33->setMaximumSize(QSize(80, 20));
		lineEditProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_38 = new QLabel(SoundSpeedClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(735, 320, 30, 20));
		label_38->setMinimumSize(QSize(30, 20));
		label_38->setMaximumSize(QSize(30, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe35 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(590, 320, 50, 20));
		checkBoxProbe35->setMinimumSize(QSize(50, 20));
		checkBoxProbe35->setMaximumSize(QSize(50, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_39 = new QLabel(SoundSpeedClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(735, 200, 30, 20));
		label_39->setMinimumSize(QSize(30, 20));
		label_39->setMaximumSize(QSize(30, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe31 = new QLineEdit(SoundSpeedClass);
		lineEditProbe31->setObjectName(QString::fromUtf8("lineEditProbe31"));
		lineEditProbe31->setGeometry(QRect(650, 160, 80, 20));
		lineEditProbe31->setMinimumSize(QSize(80, 20));
		lineEditProbe31->setMaximumSize(QSize(80, 20));
		lineEditProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe35 = new QLineEdit(SoundSpeedClass);
		lineEditProbe35->setObjectName(QString::fromUtf8("lineEditProbe35"));
		lineEditProbe35->setGeometry(QRect(650, 320, 80, 20));
		lineEditProbe35->setMinimumSize(QSize(80, 20));
		lineEditProbe35->setMaximumSize(QSize(80, 20));
		lineEditProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_40 = new QLabel(SoundSpeedClass);
		label_40->setObjectName(QString::fromUtf8("label_40"));
		label_40->setGeometry(QRect(735, 280, 30, 20));
		label_40->setMinimumSize(QSize(30, 20));
		label_40->setMaximumSize(QSize(30, 20));
		label_40->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_41 = new QLabel(SoundSpeedClass);
		label_41->setObjectName(QString::fromUtf8("label_41"));
		label_41->setGeometry(QRect(735, 360, 30, 20));
		label_41->setMinimumSize(QSize(30, 20));
		label_41->setMaximumSize(QSize(30, 20));
		label_41->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(SoundSpeedClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(590, 280, 50, 20));
		checkBoxProbe34->setMinimumSize(QSize(50, 20));
		checkBoxProbe34->setMaximumSize(QSize(50, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));

		retranslateUi(SoundSpeedClass);

		QMetaObject::connectSlotsByName(SoundSpeedClass);
	} // setupUi

	void retranslateUi(QDialog *SoundSpeedClass)
	{
		SoundSpeedClass->setWindowTitle(QApplication::translate("SoundSpeedClass", "SoundSpeed", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("SoundSpeedClass", "\345\243\260\351\200\237\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("SoundSpeedClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("SoundSpeedClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_31->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_32->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_33->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("SoundSpeedClass", "\346\260\264\345\243\260\351\200\237", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("SoundSpeedClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("SoundSpeedClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("SoundSpeedClass", " 8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("SoundSpeedClass", " 1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("SoundSpeedClass", " 10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("SoundSpeedClass", " 4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("SoundSpeedClass", " 6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("SoundSpeedClass", " 7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("SoundSpeedClass", " 2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("SoundSpeedClass", " 9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("SoundSpeedClass", " 5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("SoundSpeedClass", " 3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("SoundSpeedClass", " 13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("SoundSpeedClass", " 15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("SoundSpeedClass", " 17", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("SoundSpeedClass", " 20", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("SoundSpeedClass", " 18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("SoundSpeedClass", " 19", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("SoundSpeedClass", " 16", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("SoundSpeedClass", " 11", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("SoundSpeedClass", " 14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("SoundSpeedClass", " 12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("SoundSpeedClass", " 27", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("SoundSpeedClass", " 21", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("SoundSpeedClass", " 26", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("SoundSpeedClass", " 25", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("SoundSpeedClass", " 23", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("SoundSpeedClass", " 28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("SoundSpeedClass", " 22", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("SoundSpeedClass", " 24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("SoundSpeedClass", " 29", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("SoundSpeedClass", " 30", 0, QApplication::UnicodeUTF8));
		pushButtonAllIn->setText(QApplication::translate("SoundSpeedClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyIn->setText(QApplication::translate("SoundSpeedClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("SoundSpeedClass", " 36", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("SoundSpeedClass", " 32", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("SoundSpeedClass", " 31", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("SoundSpeedClass", " 33", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("SoundSpeedClass", " 35", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_40->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		label_41->setText(QApplication::translate("SoundSpeedClass", "m/s", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("SoundSpeedClass", " 34", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class SoundSpeedClass: public Ui_SoundSpeedClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOUNDSPEED_H





/********************************************************************************
** Form generated from reading UI file 'stanoise.ui'
**
** Created: Thu Jun 19 11:15:39 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STANOISE_H
#define UI_STANOISE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_StaNoiseClass
{
public:
	QLabel *label_2;
	QLineEdit *lineEditIProbe4;
	QLineEdit *lineEditIProbe6;
	QLineEdit *lineEditIProbe0;
	QLineEdit *lineEditIProbe9;
	QLineEdit *lineEditIProbe2;
	QLineEdit *lineEditIProbe5;
	QLineEdit *lineEditIProbe1;
	QLineEdit *lineEditIProbe3;
	QLineEdit *lineEditIProbe8;
	QLineEdit *lineEditIProbe7;
	QLabel *label_8;
	QLabel *label_20;
	QLabel *label_23;
	QLabel *label_9;
	QLineEdit *lineEditAProbe7;
	QLineEdit *lineEditAProbe9;
	QLineEdit *lineEditAProbe6;
	QLineEdit *lineEditAProbe5;
	QLineEdit *lineEditAProbe0;
	QLineEdit *lineEditAProbe2;
	QLineEdit *lineEditAProbe3;
	QLineEdit *lineEditAProbe4;
	QLineEdit *lineEditAProbe8;
	QLineEdit *lineEditAProbe1;
	QLineEdit *lineEditBProbe1;
	QLineEdit *lineEditBProbe9;
	QLineEdit *lineEditBProbe0;
	QLineEdit *lineEditBProbe2;
	QLineEdit *lineEditBProbe4;
	QLineEdit *lineEditBProbe5;
	QLineEdit *lineEditBProbe6;
	QLineEdit *lineEditBProbe7;
	QLineEdit *lineEditBProbe8;
	QLineEdit *lineEditBProbe3;
	QLineEdit *lineEditI2Probe3;
	QLineEdit *lineEditI2Probe7;
	QLineEdit *lineEditI2Probe0;
	QLineEdit *lineEditI2Probe1;
	QLineEdit *lineEditI2Probe6;
	QLineEdit *lineEditI2Probe9;
	QLineEdit *lineEditI2Probe2;
	QLineEdit *lineEditI2Probe8;
	QLineEdit *lineEditI2Probe4;
	QLineEdit *lineEditI2Probe5;
	QLineEdit *lineEditAnyI2;
	QLineEdit *lineEditAnyA;
	QLineEdit *lineEditAllI2;
	QLineEdit *lineEditAllB;
	QLineEdit *lineEditAnyB;
	QLineEdit *lineEditAnyI;
	QLineEdit *lineEditAllA;
	QLineEdit *lineEditAllI;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QCheckBox *checkBoxProbe13;
	QCheckBox *checkBoxProbe11_20;
	QCheckBox *checkBoxProbe26;
	QCheckBox *checkBoxProbe25;
	QCheckBox *checkBoxProbe28;
	QCheckBox *checkBoxProbe27;
	QCheckBox *checkBoxProbe7;
	QCheckBox *checkBoxProbe12;
	QCheckBox *checkBoxProbe23;
	QCheckBox *checkBoxProbe8;
	QCheckBox *checkBoxProbe15;
	QCheckBox *checkBoxProbe24;
	QCheckBox *checkBoxProbe19;
	QRadioButton *radioButtonAny;
	QCheckBox *checkBoxProbe18;
	QCheckBox *checkBoxProbe3;
	QCheckBox *checkBoxProbe30;
	QCheckBox *checkBoxProbe1;
	QCheckBox *checkBoxProbe11;
	QCheckBox *checkBoxProbe17;
	QCheckBox *checkBoxProbe16;
	QCheckBox *checkBoxProbe2;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe4;
	QCheckBox *checkBoxProbe20;
	QCheckBox *checkBoxProbe22;
	QCheckBox *checkBoxProbe5;
	QCheckBox *checkBoxProbe21_30;
	QCheckBox *checkBoxProbe1_10;
	QCheckBox *checkBoxProbe6;
	QCheckBox *checkBoxProbe9;
	QCheckBox *checkBoxProbe29;
	QRadioButton *radioButtonAll;
	QCheckBox *checkBoxProbe10;
	QCheckBox *checkBoxProbe21;
	QPushButton *pushButtonAllI2;
	QPushButton *pushButtonAnyI2;
	QPushButton *pushButtonAllB;
	QPushButton *pushButtonAnyB;
	QPushButton *pushButtonAnyA;
	QPushButton *pushButtonAllA;
	QPushButton *pushButtonAllI;
	QPushButton *pushButtonAnyI;
	QCheckBox *checkBoxProbe31;
	QCheckBox *checkBoxProbe33;
	QCheckBox *checkBoxProbe32;
	QCheckBox *checkBoxProbe35;
	QCheckBox *checkBoxProbe34;
	QCheckBox *checkBoxProbe36;
	QLineEdit *lineEditBProbe11;
	QLineEdit *lineEditIProbe12;
	QLineEdit *lineEditAProbe11;
	QLineEdit *lineEditAProbe12;
	QLineEdit *lineEditBProbe12;
	QLineEdit *lineEditI2Probe11;
	QLineEdit *lineEditIProbe11;
	QLineEdit *lineEditI2Probe12;

	void setupUi(QDialog *StaNoiseClass)
	{
		if (StaNoiseClass->objectName().isEmpty())
			StaNoiseClass->setObjectName(QString::fromUtf8("StaNoiseClass"));
		StaNoiseClass->resize(800, 750);
		StaNoiseClass->setMinimumSize(QSize(800, 750));
		StaNoiseClass->setMaximumSize(QSize(800, 750));
		StaNoiseClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label_2 = new QLabel(StaNoiseClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(310, 20, 180, 20));
		label_2->setMinimumSize(QSize(180, 20));
		label_2->setMaximumSize(QSize(180, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		lineEditIProbe4 = new QLineEdit(StaNoiseClass);
		lineEditIProbe4->setObjectName(QString::fromUtf8("lineEditIProbe4"));
		lineEditIProbe4->setEnabled(false);
		lineEditIProbe4->setGeometry(QRect(190, 320, 80, 20));
		lineEditIProbe4->setMinimumSize(QSize(80, 20));
		lineEditIProbe4->setMaximumSize(QSize(80, 20));
		lineEditIProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe6 = new QLineEdit(StaNoiseClass);
		lineEditIProbe6->setObjectName(QString::fromUtf8("lineEditIProbe6"));
		lineEditIProbe6->setEnabled(false);
		lineEditIProbe6->setGeometry(QRect(190, 400, 80, 20));
		lineEditIProbe6->setMinimumSize(QSize(80, 20));
		lineEditIProbe6->setMaximumSize(QSize(80, 20));
		lineEditIProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe0 = new QLineEdit(StaNoiseClass);
		lineEditIProbe0->setObjectName(QString::fromUtf8("lineEditIProbe0"));
		lineEditIProbe0->setEnabled(false);
		lineEditIProbe0->setGeometry(QRect(190, 560, 80, 20));
		lineEditIProbe0->setMinimumSize(QSize(80, 20));
		lineEditIProbe0->setMaximumSize(QSize(80, 20));
		lineEditIProbe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe9 = new QLineEdit(StaNoiseClass);
		lineEditIProbe9->setObjectName(QString::fromUtf8("lineEditIProbe9"));
		lineEditIProbe9->setEnabled(false);
		lineEditIProbe9->setGeometry(QRect(190, 520, 80, 20));
		lineEditIProbe9->setMinimumSize(QSize(80, 20));
		lineEditIProbe9->setMaximumSize(QSize(80, 20));
		lineEditIProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe2 = new QLineEdit(StaNoiseClass);
		lineEditIProbe2->setObjectName(QString::fromUtf8("lineEditIProbe2"));
		lineEditIProbe2->setEnabled(false);
		lineEditIProbe2->setGeometry(QRect(190, 240, 80, 20));
		lineEditIProbe2->setMinimumSize(QSize(80, 20));
		lineEditIProbe2->setMaximumSize(QSize(80, 20));
		lineEditIProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe5 = new QLineEdit(StaNoiseClass);
		lineEditIProbe5->setObjectName(QString::fromUtf8("lineEditIProbe5"));
		lineEditIProbe5->setEnabled(false);
		lineEditIProbe5->setGeometry(QRect(190, 360, 80, 20));
		lineEditIProbe5->setMinimumSize(QSize(80, 20));
		lineEditIProbe5->setMaximumSize(QSize(80, 20));
		lineEditIProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe1 = new QLineEdit(StaNoiseClass);
		lineEditIProbe1->setObjectName(QString::fromUtf8("lineEditIProbe1"));
		lineEditIProbe1->setEnabled(false);
		lineEditIProbe1->setGeometry(QRect(190, 200, 80, 20));
		lineEditIProbe1->setMinimumSize(QSize(80, 20));
		lineEditIProbe1->setMaximumSize(QSize(80, 20));
		lineEditIProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe3 = new QLineEdit(StaNoiseClass);
		lineEditIProbe3->setObjectName(QString::fromUtf8("lineEditIProbe3"));
		lineEditIProbe3->setEnabled(false);
		lineEditIProbe3->setGeometry(QRect(190, 280, 80, 20));
		lineEditIProbe3->setMinimumSize(QSize(80, 20));
		lineEditIProbe3->setMaximumSize(QSize(80, 20));
		lineEditIProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe8 = new QLineEdit(StaNoiseClass);
		lineEditIProbe8->setObjectName(QString::fromUtf8("lineEditIProbe8"));
		lineEditIProbe8->setEnabled(false);
		lineEditIProbe8->setGeometry(QRect(190, 480, 80, 20));
		lineEditIProbe8->setMinimumSize(QSize(80, 20));
		lineEditIProbe8->setMaximumSize(QSize(80, 20));
		lineEditIProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe7 = new QLineEdit(StaNoiseClass);
		lineEditIProbe7->setObjectName(QString::fromUtf8("lineEditIProbe7"));
		lineEditIProbe7->setEnabled(false);
		lineEditIProbe7->setGeometry(QRect(190, 440, 80, 20));
		lineEditIProbe7->setMinimumSize(QSize(80, 20));
		lineEditIProbe7->setMaximumSize(QSize(80, 20));
		lineEditIProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_8 = new QLabel(StaNoiseClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(210, 70, 50, 20));
		label_8->setMinimumSize(QSize(50, 20));
		label_8->setMaximumSize(QSize(50, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(StaNoiseClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(510, 70, 50, 20));
		label_20->setMinimumSize(QSize(50, 20));
		label_20->setMaximumSize(QSize(50, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(StaNoiseClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(650, 71, 50, 20));
		label_23->setMinimumSize(QSize(50, 20));
		label_23->setMaximumSize(QSize(50, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(StaNoiseClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(360, 70, 50, 20));
		label_9->setMinimumSize(QSize(50, 20));
		label_9->setMaximumSize(QSize(50, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditAProbe7 = new QLineEdit(StaNoiseClass);
		lineEditAProbe7->setObjectName(QString::fromUtf8("lineEditAProbe7"));
		lineEditAProbe7->setEnabled(false);
		lineEditAProbe7->setGeometry(QRect(340, 440, 80, 20));
		lineEditAProbe7->setMinimumSize(QSize(80, 20));
		lineEditAProbe7->setMaximumSize(QSize(80, 20));
		lineEditAProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe9 = new QLineEdit(StaNoiseClass);
		lineEditAProbe9->setObjectName(QString::fromUtf8("lineEditAProbe9"));
		lineEditAProbe9->setEnabled(false);
		lineEditAProbe9->setGeometry(QRect(340, 520, 80, 20));
		lineEditAProbe9->setMinimumSize(QSize(80, 20));
		lineEditAProbe9->setMaximumSize(QSize(80, 20));
		lineEditAProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe6 = new QLineEdit(StaNoiseClass);
		lineEditAProbe6->setObjectName(QString::fromUtf8("lineEditAProbe6"));
		lineEditAProbe6->setEnabled(false);
		lineEditAProbe6->setGeometry(QRect(340, 400, 80, 20));
		lineEditAProbe6->setMinimumSize(QSize(80, 20));
		lineEditAProbe6->setMaximumSize(QSize(80, 20));
		lineEditAProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe5 = new QLineEdit(StaNoiseClass);
		lineEditAProbe5->setObjectName(QString::fromUtf8("lineEditAProbe5"));
		lineEditAProbe5->setEnabled(false);
		lineEditAProbe5->setGeometry(QRect(340, 360, 80, 20));
		lineEditAProbe5->setMinimumSize(QSize(80, 20));
		lineEditAProbe5->setMaximumSize(QSize(80, 20));
		lineEditAProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe0 = new QLineEdit(StaNoiseClass);
		lineEditAProbe0->setObjectName(QString::fromUtf8("lineEditAProbe0"));
		lineEditAProbe0->setEnabled(false);
		lineEditAProbe0->setGeometry(QRect(340, 560, 80, 20));
		lineEditAProbe0->setMinimumSize(QSize(80, 20));
		lineEditAProbe0->setMaximumSize(QSize(80, 20));
		lineEditAProbe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe2 = new QLineEdit(StaNoiseClass);
		lineEditAProbe2->setObjectName(QString::fromUtf8("lineEditAProbe2"));
		lineEditAProbe2->setEnabled(false);
		lineEditAProbe2->setGeometry(QRect(340, 240, 80, 20));
		lineEditAProbe2->setMinimumSize(QSize(80, 20));
		lineEditAProbe2->setMaximumSize(QSize(80, 20));
		lineEditAProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe3 = new QLineEdit(StaNoiseClass);
		lineEditAProbe3->setObjectName(QString::fromUtf8("lineEditAProbe3"));
		lineEditAProbe3->setEnabled(false);
		lineEditAProbe3->setGeometry(QRect(340, 280, 80, 20));
		lineEditAProbe3->setMinimumSize(QSize(80, 20));
		lineEditAProbe3->setMaximumSize(QSize(80, 20));
		lineEditAProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe4 = new QLineEdit(StaNoiseClass);
		lineEditAProbe4->setObjectName(QString::fromUtf8("lineEditAProbe4"));
		lineEditAProbe4->setEnabled(false);
		lineEditAProbe4->setGeometry(QRect(340, 320, 80, 20));
		lineEditAProbe4->setMinimumSize(QSize(80, 20));
		lineEditAProbe4->setMaximumSize(QSize(80, 20));
		lineEditAProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe8 = new QLineEdit(StaNoiseClass);
		lineEditAProbe8->setObjectName(QString::fromUtf8("lineEditAProbe8"));
		lineEditAProbe8->setEnabled(false);
		lineEditAProbe8->setGeometry(QRect(340, 480, 80, 20));
		lineEditAProbe8->setMinimumSize(QSize(80, 20));
		lineEditAProbe8->setMaximumSize(QSize(80, 20));
		lineEditAProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe1 = new QLineEdit(StaNoiseClass);
		lineEditAProbe1->setObjectName(QString::fromUtf8("lineEditAProbe1"));
		lineEditAProbe1->setEnabled(false);
		lineEditAProbe1->setGeometry(QRect(340, 200, 80, 20));
		lineEditAProbe1->setMinimumSize(QSize(80, 20));
		lineEditAProbe1->setMaximumSize(QSize(80, 20));
		lineEditAProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe1 = new QLineEdit(StaNoiseClass);
		lineEditBProbe1->setObjectName(QString::fromUtf8("lineEditBProbe1"));
		lineEditBProbe1->setEnabled(false);
		lineEditBProbe1->setGeometry(QRect(490, 200, 80, 20));
		lineEditBProbe1->setMinimumSize(QSize(80, 20));
		lineEditBProbe1->setMaximumSize(QSize(80, 20));
		lineEditBProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe9 = new QLineEdit(StaNoiseClass);
		lineEditBProbe9->setObjectName(QString::fromUtf8("lineEditBProbe9"));
		lineEditBProbe9->setEnabled(false);
		lineEditBProbe9->setGeometry(QRect(490, 520, 80, 20));
		lineEditBProbe9->setMinimumSize(QSize(80, 20));
		lineEditBProbe9->setMaximumSize(QSize(80, 20));
		lineEditBProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe0 = new QLineEdit(StaNoiseClass);
		lineEditBProbe0->setObjectName(QString::fromUtf8("lineEditBProbe0"));
		lineEditBProbe0->setEnabled(false);
		lineEditBProbe0->setGeometry(QRect(490, 560, 80, 20));
		lineEditBProbe0->setMinimumSize(QSize(80, 20));
		lineEditBProbe0->setMaximumSize(QSize(80, 20));
		lineEditBProbe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe2 = new QLineEdit(StaNoiseClass);
		lineEditBProbe2->setObjectName(QString::fromUtf8("lineEditBProbe2"));
		lineEditBProbe2->setEnabled(false);
		lineEditBProbe2->setGeometry(QRect(490, 240, 80, 20));
		lineEditBProbe2->setMinimumSize(QSize(80, 20));
		lineEditBProbe2->setMaximumSize(QSize(80, 20));
		lineEditBProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe4 = new QLineEdit(StaNoiseClass);
		lineEditBProbe4->setObjectName(QString::fromUtf8("lineEditBProbe4"));
		lineEditBProbe4->setEnabled(false);
		lineEditBProbe4->setGeometry(QRect(490, 320, 80, 20));
		lineEditBProbe4->setMinimumSize(QSize(80, 20));
		lineEditBProbe4->setMaximumSize(QSize(80, 20));
		lineEditBProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe5 = new QLineEdit(StaNoiseClass);
		lineEditBProbe5->setObjectName(QString::fromUtf8("lineEditBProbe5"));
		lineEditBProbe5->setEnabled(false);
		lineEditBProbe5->setGeometry(QRect(490, 360, 80, 20));
		lineEditBProbe5->setMinimumSize(QSize(80, 20));
		lineEditBProbe5->setMaximumSize(QSize(80, 20));
		lineEditBProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe6 = new QLineEdit(StaNoiseClass);
		lineEditBProbe6->setObjectName(QString::fromUtf8("lineEditBProbe6"));
		lineEditBProbe6->setEnabled(false);
		lineEditBProbe6->setGeometry(QRect(490, 400, 80, 20));
		lineEditBProbe6->setMinimumSize(QSize(80, 20));
		lineEditBProbe6->setMaximumSize(QSize(80, 20));
		lineEditBProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe7 = new QLineEdit(StaNoiseClass);
		lineEditBProbe7->setObjectName(QString::fromUtf8("lineEditBProbe7"));
		lineEditBProbe7->setEnabled(false);
		lineEditBProbe7->setGeometry(QRect(490, 440, 80, 20));
		lineEditBProbe7->setMinimumSize(QSize(80, 20));
		lineEditBProbe7->setMaximumSize(QSize(80, 20));
		lineEditBProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe8 = new QLineEdit(StaNoiseClass);
		lineEditBProbe8->setObjectName(QString::fromUtf8("lineEditBProbe8"));
		lineEditBProbe8->setEnabled(false);
		lineEditBProbe8->setGeometry(QRect(490, 480, 80, 20));
		lineEditBProbe8->setMinimumSize(QSize(80, 20));
		lineEditBProbe8->setMaximumSize(QSize(80, 20));
		lineEditBProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe3 = new QLineEdit(StaNoiseClass);
		lineEditBProbe3->setObjectName(QString::fromUtf8("lineEditBProbe3"));
		lineEditBProbe3->setEnabled(false);
		lineEditBProbe3->setGeometry(QRect(490, 280, 80, 20));
		lineEditBProbe3->setMinimumSize(QSize(80, 20));
		lineEditBProbe3->setMaximumSize(QSize(80, 20));
		lineEditBProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe3 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe3->setObjectName(QString::fromUtf8("lineEditI2Probe3"));
		lineEditI2Probe3->setEnabled(false);
		lineEditI2Probe3->setGeometry(QRect(640, 280, 80, 20));
		lineEditI2Probe3->setMinimumSize(QSize(80, 20));
		lineEditI2Probe3->setMaximumSize(QSize(80, 20));
		lineEditI2Probe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe7 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe7->setObjectName(QString::fromUtf8("lineEditI2Probe7"));
		lineEditI2Probe7->setEnabled(false);
		lineEditI2Probe7->setGeometry(QRect(640, 440, 80, 20));
		lineEditI2Probe7->setMinimumSize(QSize(80, 20));
		lineEditI2Probe7->setMaximumSize(QSize(80, 20));
		lineEditI2Probe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe0 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe0->setObjectName(QString::fromUtf8("lineEditI2Probe0"));
		lineEditI2Probe0->setEnabled(false);
		lineEditI2Probe0->setGeometry(QRect(640, 560, 80, 20));
		lineEditI2Probe0->setMinimumSize(QSize(80, 20));
		lineEditI2Probe0->setMaximumSize(QSize(80, 20));
		lineEditI2Probe0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe1 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe1->setObjectName(QString::fromUtf8("lineEditI2Probe1"));
		lineEditI2Probe1->setEnabled(false);
		lineEditI2Probe1->setGeometry(QRect(640, 200, 80, 20));
		lineEditI2Probe1->setMinimumSize(QSize(80, 20));
		lineEditI2Probe1->setMaximumSize(QSize(80, 20));
		lineEditI2Probe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe6 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe6->setObjectName(QString::fromUtf8("lineEditI2Probe6"));
		lineEditI2Probe6->setEnabled(false);
		lineEditI2Probe6->setGeometry(QRect(640, 400, 80, 20));
		lineEditI2Probe6->setMinimumSize(QSize(80, 20));
		lineEditI2Probe6->setMaximumSize(QSize(80, 20));
		lineEditI2Probe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe9 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe9->setObjectName(QString::fromUtf8("lineEditI2Probe9"));
		lineEditI2Probe9->setEnabled(false);
		lineEditI2Probe9->setGeometry(QRect(640, 520, 80, 20));
		lineEditI2Probe9->setMinimumSize(QSize(80, 20));
		lineEditI2Probe9->setMaximumSize(QSize(80, 20));
		lineEditI2Probe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe2 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe2->setObjectName(QString::fromUtf8("lineEditI2Probe2"));
		lineEditI2Probe2->setEnabled(false);
		lineEditI2Probe2->setGeometry(QRect(640, 240, 80, 20));
		lineEditI2Probe2->setMinimumSize(QSize(80, 20));
		lineEditI2Probe2->setMaximumSize(QSize(80, 20));
		lineEditI2Probe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe8 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe8->setObjectName(QString::fromUtf8("lineEditI2Probe8"));
		lineEditI2Probe8->setEnabled(false);
		lineEditI2Probe8->setGeometry(QRect(640, 480, 80, 20));
		lineEditI2Probe8->setMinimumSize(QSize(80, 20));
		lineEditI2Probe8->setMaximumSize(QSize(80, 20));
		lineEditI2Probe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe4 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe4->setObjectName(QString::fromUtf8("lineEditI2Probe4"));
		lineEditI2Probe4->setEnabled(false);
		lineEditI2Probe4->setGeometry(QRect(640, 320, 80, 20));
		lineEditI2Probe4->setMinimumSize(QSize(80, 20));
		lineEditI2Probe4->setMaximumSize(QSize(80, 20));
		lineEditI2Probe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe5 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe5->setObjectName(QString::fromUtf8("lineEditI2Probe5"));
		lineEditI2Probe5->setEnabled(false);
		lineEditI2Probe5->setGeometry(QRect(640, 360, 80, 20));
		lineEditI2Probe5->setMinimumSize(QSize(80, 20));
		lineEditI2Probe5->setMaximumSize(QSize(80, 20));
		lineEditI2Probe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyI2 = new QLineEdit(StaNoiseClass);
		lineEditAnyI2->setObjectName(QString::fromUtf8("lineEditAnyI2"));
		lineEditAnyI2->setGeometry(QRect(640, 150, 80, 20));
		lineEditAnyI2->setMinimumSize(QSize(80, 20));
		lineEditAnyI2->setMaximumSize(QSize(80, 20));
		lineEditAnyI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyA = new QLineEdit(StaNoiseClass);
		lineEditAnyA->setObjectName(QString::fromUtf8("lineEditAnyA"));
		lineEditAnyA->setGeometry(QRect(340, 150, 80, 20));
		lineEditAnyA->setMinimumSize(QSize(80, 20));
		lineEditAnyA->setMaximumSize(QSize(80, 20));
		lineEditAnyA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAllI2 = new QLineEdit(StaNoiseClass);
		lineEditAllI2->setObjectName(QString::fromUtf8("lineEditAllI2"));
		lineEditAllI2->setGeometry(QRect(640, 110, 80, 20));
		lineEditAllI2->setMinimumSize(QSize(80, 20));
		lineEditAllI2->setMaximumSize(QSize(80, 20));
		lineEditAllI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAllB = new QLineEdit(StaNoiseClass);
		lineEditAllB->setObjectName(QString::fromUtf8("lineEditAllB"));
		lineEditAllB->setGeometry(QRect(490, 110, 80, 20));
		lineEditAllB->setMinimumSize(QSize(80, 20));
		lineEditAllB->setMaximumSize(QSize(80, 20));
		lineEditAllB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyB = new QLineEdit(StaNoiseClass);
		lineEditAnyB->setObjectName(QString::fromUtf8("lineEditAnyB"));
		lineEditAnyB->setGeometry(QRect(490, 150, 80, 20));
		lineEditAnyB->setMinimumSize(QSize(80, 20));
		lineEditAnyB->setMaximumSize(QSize(80, 20));
		lineEditAnyB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAnyI = new QLineEdit(StaNoiseClass);
		lineEditAnyI->setObjectName(QString::fromUtf8("lineEditAnyI"));
		lineEditAnyI->setGeometry(QRect(190, 150, 80, 20));
		lineEditAnyI->setMinimumSize(QSize(80, 20));
		lineEditAnyI->setMaximumSize(QSize(80, 20));
		lineEditAnyI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAllA = new QLineEdit(StaNoiseClass);
		lineEditAllA->setObjectName(QString::fromUtf8("lineEditAllA"));
		lineEditAllA->setGeometry(QRect(340, 110, 80, 20));
		lineEditAllA->setMinimumSize(QSize(80, 20));
		lineEditAllA->setMaximumSize(QSize(80, 20));
		lineEditAllA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAllI = new QLineEdit(StaNoiseClass);
		lineEditAllI->setObjectName(QString::fromUtf8("lineEditAllI"));
		lineEditAllI->setGeometry(QRect(190, 110, 80, 20));
		lineEditAllI->setMinimumSize(QSize(80, 20));
		lineEditAllI->setMaximumSize(QSize(80, 20));
		lineEditAllI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonYes = new QPushButton(StaNoiseClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(500, 700, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(StaNoiseClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(640, 700, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13 = new QCheckBox(StaNoiseClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(70, 200, 35, 20));
		checkBoxProbe13->setMinimumSize(QSize(35, 20));
		checkBoxProbe13->setMaximumSize(QSize(35, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11_20 = new QCheckBox(StaNoiseClass);
		checkBoxProbe11_20->setObjectName(QString::fromUtf8("checkBoxProbe11_20"));
		checkBoxProbe11_20->setGeometry(QRect(20, 120, 60, 20));
		checkBoxProbe11_20->setMinimumSize(QSize(60, 20));
		checkBoxProbe11_20->setMaximumSize(QSize(60, 20));
		checkBoxProbe11_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe26 = new QCheckBox(StaNoiseClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(130, 240, 35, 20));
		checkBoxProbe26->setMinimumSize(QSize(35, 20));
		checkBoxProbe26->setMaximumSize(QSize(35, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe25 = new QCheckBox(StaNoiseClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(130, 200, 35, 20));
		checkBoxProbe25->setMinimumSize(QSize(35, 20));
		checkBoxProbe25->setMaximumSize(QSize(35, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe28 = new QCheckBox(StaNoiseClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(130, 320, 35, 20));
		checkBoxProbe28->setMinimumSize(QSize(35, 20));
		checkBoxProbe28->setMaximumSize(QSize(35, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe27 = new QCheckBox(StaNoiseClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(130, 280, 35, 20));
		checkBoxProbe27->setMinimumSize(QSize(35, 20));
		checkBoxProbe27->setMaximumSize(QSize(35, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe7 = new QCheckBox(StaNoiseClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(20, 440, 35, 20));
		checkBoxProbe7->setMinimumSize(QSize(35, 20));
		checkBoxProbe7->setMaximumSize(QSize(35, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe12 = new QCheckBox(StaNoiseClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(20, 640, 35, 20));
		checkBoxProbe12->setMinimumSize(QSize(35, 20));
		checkBoxProbe12->setMaximumSize(QSize(35, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe23 = new QCheckBox(StaNoiseClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(70, 600, 35, 20));
		checkBoxProbe23->setMinimumSize(QSize(35, 20));
		checkBoxProbe23->setMaximumSize(QSize(35, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe8 = new QCheckBox(StaNoiseClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(20, 480, 35, 20));
		checkBoxProbe8->setMinimumSize(QSize(35, 20));
		checkBoxProbe8->setMaximumSize(QSize(35, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe15 = new QCheckBox(StaNoiseClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(70, 280, 35, 20));
		checkBoxProbe15->setMinimumSize(QSize(35, 20));
		checkBoxProbe15->setMaximumSize(QSize(35, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe24 = new QCheckBox(StaNoiseClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(70, 640, 35, 20));
		checkBoxProbe24->setMinimumSize(QSize(35, 20));
		checkBoxProbe24->setMaximumSize(QSize(35, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe19 = new QCheckBox(StaNoiseClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(70, 440, 35, 20));
		checkBoxProbe19->setMinimumSize(QSize(35, 20));
		checkBoxProbe19->setMaximumSize(QSize(35, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAny = new QRadioButton(StaNoiseClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(90, 149, 81, 16));
		radioButtonAny->setMinimumSize(QSize(81, 16));
		radioButtonAny->setMaximumSize(QSize(81, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe18 = new QCheckBox(StaNoiseClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(70, 400, 35, 20));
		checkBoxProbe18->setMinimumSize(QSize(35, 20));
		checkBoxProbe18->setMaximumSize(QSize(35, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe3 = new QCheckBox(StaNoiseClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(20, 280, 35, 20));
		checkBoxProbe3->setMinimumSize(QSize(35, 20));
		checkBoxProbe3->setMaximumSize(QSize(35, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe30 = new QCheckBox(StaNoiseClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(130, 400, 35, 20));
		checkBoxProbe30->setMinimumSize(QSize(35, 20));
		checkBoxProbe30->setMaximumSize(QSize(35, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe1 = new QCheckBox(StaNoiseClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(20, 200, 35, 20));
		checkBoxProbe1->setMinimumSize(QSize(35, 20));
		checkBoxProbe1->setMaximumSize(QSize(35, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(StaNoiseClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(20, 600, 35, 20));
		checkBoxProbe11->setMinimumSize(QSize(35, 20));
		checkBoxProbe11->setMaximumSize(QSize(35, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe17 = new QCheckBox(StaNoiseClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(70, 360, 35, 20));
		checkBoxProbe17->setMinimumSize(QSize(35, 20));
		checkBoxProbe17->setMaximumSize(QSize(35, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe16 = new QCheckBox(StaNoiseClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(70, 320, 35, 20));
		checkBoxProbe16->setMinimumSize(QSize(35, 20));
		checkBoxProbe16->setMaximumSize(QSize(35, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe2 = new QCheckBox(StaNoiseClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(20, 240, 35, 20));
		checkBoxProbe2->setMinimumSize(QSize(35, 20));
		checkBoxProbe2->setMaximumSize(QSize(35, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe14 = new QCheckBox(StaNoiseClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(70, 240, 35, 20));
		checkBoxProbe14->setMinimumSize(QSize(35, 20));
		checkBoxProbe14->setMaximumSize(QSize(35, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe4 = new QCheckBox(StaNoiseClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(20, 320, 35, 20));
		checkBoxProbe4->setMinimumSize(QSize(35, 20));
		checkBoxProbe4->setMaximumSize(QSize(35, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe20 = new QCheckBox(StaNoiseClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(70, 480, 35, 20));
		checkBoxProbe20->setMinimumSize(QSize(35, 20));
		checkBoxProbe20->setMaximumSize(QSize(35, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe22 = new QCheckBox(StaNoiseClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(70, 560, 35, 20));
		checkBoxProbe22->setMinimumSize(QSize(35, 20));
		checkBoxProbe22->setMaximumSize(QSize(35, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe5 = new QCheckBox(StaNoiseClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(20, 360, 35, 20));
		checkBoxProbe5->setMinimumSize(QSize(35, 20));
		checkBoxProbe5->setMaximumSize(QSize(35, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe21_30 = new QCheckBox(StaNoiseClass);
		checkBoxProbe21_30->setObjectName(QString::fromUtf8("checkBoxProbe21_30"));
		checkBoxProbe21_30->setGeometry(QRect(20, 150, 60, 20));
		checkBoxProbe21_30->setMinimumSize(QSize(60, 20));
		checkBoxProbe21_30->setMaximumSize(QSize(60, 20));
		checkBoxProbe21_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe1_10 = new QCheckBox(StaNoiseClass);
		checkBoxProbe1_10->setObjectName(QString::fromUtf8("checkBoxProbe1_10"));
		checkBoxProbe1_10->setGeometry(QRect(20, 90, 60, 20));
		checkBoxProbe1_10->setMinimumSize(QSize(60, 20));
		checkBoxProbe1_10->setMaximumSize(QSize(60, 20));
		checkBoxProbe1_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe6 = new QCheckBox(StaNoiseClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(20, 400, 35, 20));
		checkBoxProbe6->setMinimumSize(QSize(35, 20));
		checkBoxProbe6->setMaximumSize(QSize(35, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe9 = new QCheckBox(StaNoiseClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(20, 520, 35, 20));
		checkBoxProbe9->setMinimumSize(QSize(35, 20));
		checkBoxProbe9->setMaximumSize(QSize(35, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe29 = new QCheckBox(StaNoiseClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(130, 360, 35, 20));
		checkBoxProbe29->setMinimumSize(QSize(35, 20));
		checkBoxProbe29->setMaximumSize(QSize(35, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAll = new QRadioButton(StaNoiseClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(90, 110, 81, 16));
		radioButtonAll->setMinimumSize(QSize(81, 16));
		radioButtonAll->setMaximumSize(QSize(81, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe10 = new QCheckBox(StaNoiseClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(20, 560, 35, 20));
		checkBoxProbe10->setMinimumSize(QSize(35, 20));
		checkBoxProbe10->setMaximumSize(QSize(35, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe21 = new QCheckBox(StaNoiseClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(70, 520, 35, 20));
		checkBoxProbe21->setMinimumSize(QSize(35, 20));
		checkBoxProbe21->setMaximumSize(QSize(35, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllI2 = new QPushButton(StaNoiseClass);
		pushButtonAllI2->setObjectName(QString::fromUtf8("pushButtonAllI2"));
		pushButtonAllI2->setGeometry(QRect(730, 110, 50, 20));
		pushButtonAllI2->setMinimumSize(QSize(50, 20));
		pushButtonAllI2->setMaximumSize(QSize(50, 20));
		pushButtonAllI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyI2 = new QPushButton(StaNoiseClass);
		pushButtonAnyI2->setObjectName(QString::fromUtf8("pushButtonAnyI2"));
		pushButtonAnyI2->setGeometry(QRect(730, 150, 50, 20));
		pushButtonAnyI2->setMinimumSize(QSize(50, 20));
		pushButtonAnyI2->setMaximumSize(QSize(50, 20));
		pushButtonAnyI2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllB = new QPushButton(StaNoiseClass);
		pushButtonAllB->setObjectName(QString::fromUtf8("pushButtonAllB"));
		pushButtonAllB->setGeometry(QRect(580, 110, 50, 20));
		pushButtonAllB->setMinimumSize(QSize(50, 20));
		pushButtonAllB->setMaximumSize(QSize(50, 20));
		pushButtonAllB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyB = new QPushButton(StaNoiseClass);
		pushButtonAnyB->setObjectName(QString::fromUtf8("pushButtonAnyB"));
		pushButtonAnyB->setGeometry(QRect(580, 150, 50, 20));
		pushButtonAnyB->setMinimumSize(QSize(50, 20));
		pushButtonAnyB->setMaximumSize(QSize(50, 20));
		pushButtonAnyB->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyA = new QPushButton(StaNoiseClass);
		pushButtonAnyA->setObjectName(QString::fromUtf8("pushButtonAnyA"));
		pushButtonAnyA->setGeometry(QRect(430, 150, 50, 20));
		pushButtonAnyA->setMinimumSize(QSize(50, 20));
		pushButtonAnyA->setMaximumSize(QSize(50, 20));
		pushButtonAnyA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllA = new QPushButton(StaNoiseClass);
		pushButtonAllA->setObjectName(QString::fromUtf8("pushButtonAllA"));
		pushButtonAllA->setGeometry(QRect(430, 110, 50, 20));
		pushButtonAllA->setMinimumSize(QSize(50, 20));
		pushButtonAllA->setMaximumSize(QSize(50, 20));
		pushButtonAllA->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllI = new QPushButton(StaNoiseClass);
		pushButtonAllI->setObjectName(QString::fromUtf8("pushButtonAllI"));
		pushButtonAllI->setGeometry(QRect(280, 110, 50, 20));
		pushButtonAllI->setMinimumSize(QSize(50, 20));
		pushButtonAllI->setMaximumSize(QSize(50, 20));
		pushButtonAllI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAnyI = new QPushButton(StaNoiseClass);
		pushButtonAnyI->setObjectName(QString::fromUtf8("pushButtonAnyI"));
		pushButtonAnyI->setGeometry(QRect(280, 150, 50, 20));
		pushButtonAnyI->setMinimumSize(QSize(50, 20));
		pushButtonAnyI->setMaximumSize(QSize(50, 20));
		pushButtonAnyI->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(StaNoiseClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(130, 440, 35, 20));
		checkBoxProbe31->setMinimumSize(QSize(35, 20));
		checkBoxProbe31->setMaximumSize(QSize(35, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe33 = new QCheckBox(StaNoiseClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(130, 520, 35, 20));
		checkBoxProbe33->setMinimumSize(QSize(35, 20));
		checkBoxProbe33->setMaximumSize(QSize(35, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe32 = new QCheckBox(StaNoiseClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(130, 480, 35, 20));
		checkBoxProbe32->setMinimumSize(QSize(35, 20));
		checkBoxProbe32->setMaximumSize(QSize(35, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe35 = new QCheckBox(StaNoiseClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(130, 600, 35, 20));
		checkBoxProbe35->setMinimumSize(QSize(35, 20));
		checkBoxProbe35->setMaximumSize(QSize(35, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(StaNoiseClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(130, 560, 35, 20));
		checkBoxProbe34->setMinimumSize(QSize(35, 20));
		checkBoxProbe34->setMaximumSize(QSize(35, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe36 = new QCheckBox(StaNoiseClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(130, 640, 35, 20));
		checkBoxProbe36->setMinimumSize(QSize(35, 20));
		checkBoxProbe36->setMaximumSize(QSize(35, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditBProbe11 = new QLineEdit(StaNoiseClass);
		lineEditBProbe11->setObjectName(QString::fromUtf8("lineEditBProbe11"));
		lineEditBProbe11->setEnabled(false);
		lineEditBProbe11->setGeometry(QRect(490, 600, 80, 20));
		lineEditBProbe11->setMinimumSize(QSize(80, 20));
		lineEditBProbe11->setMaximumSize(QSize(80, 20));
		lineEditBProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe12 = new QLineEdit(StaNoiseClass);
		lineEditIProbe12->setObjectName(QString::fromUtf8("lineEditIProbe12"));
		lineEditIProbe12->setEnabled(false);
		lineEditIProbe12->setGeometry(QRect(190, 640, 80, 20));
		lineEditIProbe12->setMinimumSize(QSize(80, 20));
		lineEditIProbe12->setMaximumSize(QSize(80, 20));
		lineEditIProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe11 = new QLineEdit(StaNoiseClass);
		lineEditAProbe11->setObjectName(QString::fromUtf8("lineEditAProbe11"));
		lineEditAProbe11->setEnabled(false);
		lineEditAProbe11->setGeometry(QRect(340, 600, 80, 20));
		lineEditAProbe11->setMinimumSize(QSize(80, 20));
		lineEditAProbe11->setMaximumSize(QSize(80, 20));
		lineEditAProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAProbe12 = new QLineEdit(StaNoiseClass);
		lineEditAProbe12->setObjectName(QString::fromUtf8("lineEditAProbe12"));
		lineEditAProbe12->setEnabled(false);
		lineEditAProbe12->setGeometry(QRect(340, 640, 80, 20));
		lineEditAProbe12->setMinimumSize(QSize(80, 20));
		lineEditAProbe12->setMaximumSize(QSize(80, 20));
		lineEditAProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditBProbe12 = new QLineEdit(StaNoiseClass);
		lineEditBProbe12->setObjectName(QString::fromUtf8("lineEditBProbe12"));
		lineEditBProbe12->setEnabled(false);
		lineEditBProbe12->setGeometry(QRect(490, 640, 80, 20));
		lineEditBProbe12->setMinimumSize(QSize(80, 20));
		lineEditBProbe12->setMaximumSize(QSize(80, 20));
		lineEditBProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe11 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe11->setObjectName(QString::fromUtf8("lineEditI2Probe11"));
		lineEditI2Probe11->setEnabled(false);
		lineEditI2Probe11->setGeometry(QRect(640, 600, 80, 20));
		lineEditI2Probe11->setMinimumSize(QSize(80, 20));
		lineEditI2Probe11->setMaximumSize(QSize(80, 20));
		lineEditI2Probe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditIProbe11 = new QLineEdit(StaNoiseClass);
		lineEditIProbe11->setObjectName(QString::fromUtf8("lineEditIProbe11"));
		lineEditIProbe11->setEnabled(false);
		lineEditIProbe11->setGeometry(QRect(190, 600, 80, 20));
		lineEditIProbe11->setMinimumSize(QSize(80, 20));
		lineEditIProbe11->setMaximumSize(QSize(80, 20));
		lineEditIProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditI2Probe12 = new QLineEdit(StaNoiseClass);
		lineEditI2Probe12->setObjectName(QString::fromUtf8("lineEditI2Probe12"));
		lineEditI2Probe12->setEnabled(false);
		lineEditI2Probe12->setGeometry(QRect(640, 640, 80, 20));
		lineEditI2Probe12->setMinimumSize(QSize(80, 20));
		lineEditI2Probe12->setMaximumSize(QSize(80, 20));
		lineEditI2Probe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));

		retranslateUi(StaNoiseClass);

		QMetaObject::connectSlotsByName(StaNoiseClass);
	} // setupUi

	void retranslateUi(QDialog *StaNoiseClass)
	{
		StaNoiseClass->setWindowTitle(QApplication::translate("StaNoiseClass", "StaNoise", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("StaNoiseClass", "\347\273\237\350\256\241\351\231\215\345\231\252\346\225\260\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("StaNoiseClass", "\351\227\270\351\227\250I", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("StaNoiseClass", "\351\227\270\351\227\250B", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("StaNoiseClass", "\351\227\270\351\227\250I2", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("StaNoiseClass", "\351\227\270\351\227\250A", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("StaNoiseClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("StaNoiseClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("StaNoiseClass", "13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11_20->setText(QApplication::translate("StaNoiseClass", "13~24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("StaNoiseClass", "26", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("StaNoiseClass", "25", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("StaNoiseClass", "28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("StaNoiseClass", "27", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("StaNoiseClass", "7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("StaNoiseClass", "12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("StaNoiseClass", "23", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("StaNoiseClass", "8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("StaNoiseClass", "15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("StaNoiseClass", "24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("StaNoiseClass", "19", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("StaNoiseClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("StaNoiseClass", "18", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("StaNoiseClass", "3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("StaNoiseClass", "30", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("StaNoiseClass", "1", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("StaNoiseClass", "11", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("StaNoiseClass", "17", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("StaNoiseClass", "16", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("StaNoiseClass", "2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("StaNoiseClass", "14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("StaNoiseClass", "4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("StaNoiseClass", "20", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("StaNoiseClass", "22", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("StaNoiseClass", "5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21_30->setText(QApplication::translate("StaNoiseClass", "25~36", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1_10->setText(QApplication::translate("StaNoiseClass", "1~12", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("StaNoiseClass", "6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("StaNoiseClass", "9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("StaNoiseClass", "29", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("StaNoiseClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("StaNoiseClass", "10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("StaNoiseClass", "21", 0, QApplication::UnicodeUTF8));
		pushButtonAllI2->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyI2->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllB->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyB->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyA->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllA->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAllI->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		pushButtonAnyI->setText(QApplication::translate("StaNoiseClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("StaNoiseClass", "31", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("StaNoiseClass", "33", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("StaNoiseClass", "32", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("StaNoiseClass", "35", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("StaNoiseClass", "34", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("StaNoiseClass", "36", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class StaNoiseClass: public Ui_StaNoiseClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STANOISE_H



/********************************************************************************
** Form generated from reading UI file 'width.ui'
**
** Created: Thu Jun 26 19:11:52 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDTH_H
#define UI_WIDTH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_WidthClass
{
public:
	QLineEdit *lineEditAny;
	QCheckBox *checkBoxProbe17;
	QPushButton *pushButtonAllIn;
	QLineEdit *lineEditProbe16;
	QCheckBox *checkBoxProbe19;
	QLineEdit *lineEditProbe14;
	QLineEdit *lineEditProbe10;
	QLabel *label_5;
	QCheckBox *checkBoxProbe13;
	QCheckBox *checkBoxProbe15;
	QCheckBox *checkBoxProbe10;
	QLineEdit *lineEditProbe18;
	QCheckBox *checkBoxProbe25;
	QLineEdit *lineEditProbe8;
	QCheckBox *checkBoxProbe6;
	QLineEdit *lineEditProbe19;
	QLineEdit *lineEditProbe30;
	QCheckBox *checkBoxProbe1;
	QLineEdit *lineEditProbe17;
	QLineEdit *lineEditProbe13;
	QPushButton *pushButtonAnyIn;
	QCheckBox *checkBoxProbe28;
	QCheckBox *checkBoxProbe5;
	QLineEdit *lineEditProbe2;
	QCheckBox *checkBoxProbe16;
	QPushButton *pushButtonYes;
	QLineEdit *lineEditProbe28;
	QCheckBox *checkBoxProbe26;
	QLineEdit *lineEditProbe9;
	QCheckBox *checkBoxProbe21;
	QCheckBox *checkBoxProbe7;
	QLineEdit *lineEditProbe6;
	QCheckBox *checkBoxProbe30;
	QLabel *label_4;
	QLineEdit *lineEditProbe5;
	QCheckBox *checkBoxProbe3;
	QLineEdit *lineEditProbe4;
	QCheckBox *checkBoxProbe29;
	QLineEdit *lineEditProbe29;
	QLineEdit *lineEditProbe22;
	QLabel *label_6;
	QLineEdit *lineEditProbe27;
	QLineEdit *lineEditProbe11;
	QCheckBox *checkBoxProbe23;
	QRadioButton *radioButtonAll;
	QRadioButton *radioButtonAny;
	QCheckBox *checkBoxProbe2;
	QCheckBox *checkBoxProbe9;
	QLineEdit *lineEditProbe1;
	QCheckBox *checkBoxProbe8;
	QLineEdit *lineEditProbe26;
	QCheckBox *checkBoxProbe18;
	QLineEdit *lineEditProbe25;
	QPushButton *pushButtonNo;
	QCheckBox *checkBoxProbe20;
	QLabel *label_2;
	QLineEdit *lineEditProbe20;
	QLineEdit *lineEditProbe21;
	QCheckBox *checkBoxProbe24;
	QLineEdit *lineEditProbe7;
	QLineEdit *lineEditAll;
	QCheckBox *checkBoxProbe4;
	QLineEdit *lineEditProbe3;
	QLineEdit *lineEditProbe24;
	QCheckBox *checkBoxProbe12;
	QLabel *label_3;
	QLineEdit *lineEditProbe15;
	QCheckBox *checkBoxProbe22;
	QCheckBox *checkBoxProbe11;
	QLineEdit *lineEditProbe12;
	QLineEdit *lineEditProbe23;
	QLabel *label;
	QCheckBox *checkBoxProbe14;
	QCheckBox *checkBoxProbe27;
	QLabel *label_7;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QLabel *label_14;
	QLabel *label_15;
	QLabel *label_16;
	QLabel *label_17;
	QLabel *label_18;
	QLabel *label_19;
	QLabel *label_20;
	QLabel *label_21;
	QLabel *label_22;
	QLabel *label_23;
	QLabel *label_24;
	QLabel *label_25;
	QLabel *label_26;
	QLabel *label_27;
	QLabel *label_28;
	QLabel *label_29;
	QLabel *label_30;
	QLabel *label_31;
	QLabel *label_32;
	QLabel *label_33;
	QCheckBox *checkBoxProbe36;
	QLabel *label_34;
	QLabel *label_35;
	QLineEdit *lineEditProbe34;
	QLineEdit *lineEditProbe32;
	QCheckBox *checkBoxProbe32;
	QLabel *label_36;
	QCheckBox *checkBoxProbe31;
	QCheckBox *checkBoxProbe34;
	QLineEdit *lineEditProbe33;
	QLineEdit *lineEditProbe36;
	QLabel *label_37;
	QLineEdit *lineEditProbe35;
	QCheckBox *checkBoxProbe33;
	QLabel *label_38;
	QLineEdit *lineEditProbe31;
	QLabel *label_39;
	QCheckBox *checkBoxProbe35;

	void setupUi(QDialog *WidthClass)
	{
		if (WidthClass->objectName().isEmpty())
			WidthClass->setObjectName(QString::fromUtf8("WidthClass"));
		WidthClass->resize(800, 586);
		WidthClass->setMinimumSize(QSize(800, 586));
		WidthClass->setMaximumSize(QSize(800, 586));
		WidthClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		lineEditAny = new QLineEdit(WidthClass);
		lineEditAny->setObjectName(QString::fromUtf8("lineEditAny"));
		lineEditAny->setGeometry(QRect(130, 100, 80, 20));
		lineEditAny->setMinimumSize(QSize(80, 20));
		lineEditAny->setMaximumSize(QSize(80, 20));
		lineEditAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe17 = new QCheckBox(WidthClass);
		checkBoxProbe17->setObjectName(QString::fromUtf8("checkBoxProbe17"));
		checkBoxProbe17->setGeometry(QRect(220, 390, 50, 20));
		checkBoxProbe17->setMinimumSize(QSize(50, 20));
		checkBoxProbe17->setMaximumSize(QSize(50, 20));
		checkBoxProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonAllIn = new QPushButton(WidthClass);
		pushButtonAllIn->setObjectName(QString::fromUtf8("pushButtonAllIn"));
		pushButtonAllIn->setGeometry(QRect(260, 60, 50, 23));
		pushButtonAllIn->setMinimumSize(QSize(50, 23));
		pushButtonAllIn->setMaximumSize(QSize(50, 23));
		pushButtonAllIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe16 = new QLineEdit(WidthClass);
		lineEditProbe16->setObjectName(QString::fromUtf8("lineEditProbe16"));
		lineEditProbe16->setGeometry(QRect(280, 350, 80, 20));
		lineEditProbe16->setMinimumSize(QSize(80, 20));
		lineEditProbe16->setMaximumSize(QSize(80, 20));
		lineEditProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe19 = new QCheckBox(WidthClass);
		checkBoxProbe19->setObjectName(QString::fromUtf8("checkBoxProbe19"));
		checkBoxProbe19->setGeometry(QRect(220, 470, 50, 20));
		checkBoxProbe19->setMinimumSize(QSize(50, 20));
		checkBoxProbe19->setMaximumSize(QSize(50, 20));
		checkBoxProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe14 = new QLineEdit(WidthClass);
		lineEditProbe14->setObjectName(QString::fromUtf8("lineEditProbe14"));
		lineEditProbe14->setGeometry(QRect(280, 270, 80, 20));
		lineEditProbe14->setMinimumSize(QSize(80, 20));
		lineEditProbe14->setMaximumSize(QSize(80, 20));
		lineEditProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe10 = new QLineEdit(WidthClass);
		lineEditProbe10->setObjectName(QString::fromUtf8("lineEditProbe10"));
		lineEditProbe10->setGeometry(QRect(90, 510, 80, 20));
		lineEditProbe10->setMinimumSize(QSize(80, 20));
		lineEditProbe10->setMaximumSize(QSize(80, 20));
		lineEditProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_5 = new QLabel(WidthClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(170, 190, 30, 20));
		label_5->setMinimumSize(QSize(30, 20));
		label_5->setMaximumSize(QSize(30, 20));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe13 = new QCheckBox(WidthClass);
		checkBoxProbe13->setObjectName(QString::fromUtf8("checkBoxProbe13"));
		checkBoxProbe13->setGeometry(QRect(220, 230, 50, 20));
		checkBoxProbe13->setMinimumSize(QSize(50, 20));
		checkBoxProbe13->setMaximumSize(QSize(50, 20));
		checkBoxProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe15 = new QCheckBox(WidthClass);
		checkBoxProbe15->setObjectName(QString::fromUtf8("checkBoxProbe15"));
		checkBoxProbe15->setGeometry(QRect(220, 310, 50, 20));
		checkBoxProbe15->setMinimumSize(QSize(50, 20));
		checkBoxProbe15->setMaximumSize(QSize(50, 20));
		checkBoxProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe10 = new QCheckBox(WidthClass);
		checkBoxProbe10->setObjectName(QString::fromUtf8("checkBoxProbe10"));
		checkBoxProbe10->setGeometry(QRect(40, 510, 50, 20));
		checkBoxProbe10->setMinimumSize(QSize(50, 20));
		checkBoxProbe10->setMaximumSize(QSize(50, 20));
		checkBoxProbe10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe18 = new QLineEdit(WidthClass);
		lineEditProbe18->setObjectName(QString::fromUtf8("lineEditProbe18"));
		lineEditProbe18->setGeometry(QRect(280, 430, 80, 20));
		lineEditProbe18->setMinimumSize(QSize(80, 20));
		lineEditProbe18->setMaximumSize(QSize(80, 20));
		lineEditProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe25 = new QCheckBox(WidthClass);
		checkBoxProbe25->setObjectName(QString::fromUtf8("checkBoxProbe25"));
		checkBoxProbe25->setGeometry(QRect(410, 310, 50, 20));
		checkBoxProbe25->setMinimumSize(QSize(50, 20));
		checkBoxProbe25->setMaximumSize(QSize(50, 20));
		checkBoxProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe8 = new QLineEdit(WidthClass);
		lineEditProbe8->setObjectName(QString::fromUtf8("lineEditProbe8"));
		lineEditProbe8->setGeometry(QRect(90, 430, 80, 20));
		lineEditProbe8->setMinimumSize(QSize(80, 20));
		lineEditProbe8->setMaximumSize(QSize(80, 20));
		lineEditProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe6 = new QCheckBox(WidthClass);
		checkBoxProbe6->setObjectName(QString::fromUtf8("checkBoxProbe6"));
		checkBoxProbe6->setGeometry(QRect(40, 350, 50, 20));
		checkBoxProbe6->setMinimumSize(QSize(50, 20));
		checkBoxProbe6->setMaximumSize(QSize(50, 20));
		checkBoxProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe19 = new QLineEdit(WidthClass);
		lineEditProbe19->setObjectName(QString::fromUtf8("lineEditProbe19"));
		lineEditProbe19->setGeometry(QRect(280, 470, 80, 20));
		lineEditProbe19->setMinimumSize(QSize(80, 20));
		lineEditProbe19->setMaximumSize(QSize(80, 20));
		lineEditProbe19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe30 = new QLineEdit(WidthClass);
		lineEditProbe30->setObjectName(QString::fromUtf8("lineEditProbe30"));
		lineEditProbe30->setGeometry(QRect(470, 510, 80, 20));
		lineEditProbe30->setMinimumSize(QSize(80, 20));
		lineEditProbe30->setMaximumSize(QSize(80, 20));
		lineEditProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe1 = new QCheckBox(WidthClass);
		checkBoxProbe1->setObjectName(QString::fromUtf8("checkBoxProbe1"));
		checkBoxProbe1->setGeometry(QRect(40, 150, 50, 20));
		checkBoxProbe1->setMinimumSize(QSize(50, 20));
		checkBoxProbe1->setMaximumSize(QSize(50, 20));
		checkBoxProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe17 = new QLineEdit(WidthClass);
		lineEditProbe17->setObjectName(QString::fromUtf8("lineEditProbe17"));
		lineEditProbe17->setGeometry(QRect(280, 390, 80, 20));
		lineEditProbe17->setMinimumSize(QSize(80, 20));
		lineEditProbe17->setMaximumSize(QSize(80, 20));
		lineEditProbe17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe13 = new QLineEdit(WidthClass);
		lineEditProbe13->setObjectName(QString::fromUtf8("lineEditProbe13"));
		lineEditProbe13->setGeometry(QRect(280, 230, 80, 20));
		lineEditProbe13->setMinimumSize(QSize(80, 20));
		lineEditProbe13->setMaximumSize(QSize(80, 20));
		lineEditProbe13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonAnyIn = new QPushButton(WidthClass);
		pushButtonAnyIn->setObjectName(QString::fromUtf8("pushButtonAnyIn"));
		pushButtonAnyIn->setGeometry(QRect(260, 100, 50, 23));
		pushButtonAnyIn->setMinimumSize(QSize(50, 23));
		pushButtonAnyIn->setMaximumSize(QSize(50, 23));
		pushButtonAnyIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe28 = new QCheckBox(WidthClass);
		checkBoxProbe28->setObjectName(QString::fromUtf8("checkBoxProbe28"));
		checkBoxProbe28->setGeometry(QRect(410, 430, 50, 20));
		checkBoxProbe28->setMinimumSize(QSize(50, 20));
		checkBoxProbe28->setMaximumSize(QSize(50, 20));
		checkBoxProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe5 = new QCheckBox(WidthClass);
		checkBoxProbe5->setObjectName(QString::fromUtf8("checkBoxProbe5"));
		checkBoxProbe5->setGeometry(QRect(40, 310, 50, 20));
		checkBoxProbe5->setMinimumSize(QSize(50, 20));
		checkBoxProbe5->setMaximumSize(QSize(50, 20));
		checkBoxProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe2 = new QLineEdit(WidthClass);
		lineEditProbe2->setObjectName(QString::fromUtf8("lineEditProbe2"));
		lineEditProbe2->setGeometry(QRect(90, 190, 80, 20));
		lineEditProbe2->setMinimumSize(QSize(80, 20));
		lineEditProbe2->setMaximumSize(QSize(80, 20));
		lineEditProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe16 = new QCheckBox(WidthClass);
		checkBoxProbe16->setObjectName(QString::fromUtf8("checkBoxProbe16"));
		checkBoxProbe16->setGeometry(QRect(220, 350, 50, 20));
		checkBoxProbe16->setMinimumSize(QSize(50, 20));
		checkBoxProbe16->setMaximumSize(QSize(50, 20));
		checkBoxProbe16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(WidthClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(590, 540, 70, 25));
		pushButtonYes->setMinimumSize(QSize(70, 25));
		pushButtonYes->setMaximumSize(QSize(70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe28 = new QLineEdit(WidthClass);
		lineEditProbe28->setObjectName(QString::fromUtf8("lineEditProbe28"));
		lineEditProbe28->setGeometry(QRect(470, 430, 80, 20));
		lineEditProbe28->setMinimumSize(QSize(80, 20));
		lineEditProbe28->setMaximumSize(QSize(80, 20));
		lineEditProbe28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe26 = new QCheckBox(WidthClass);
		checkBoxProbe26->setObjectName(QString::fromUtf8("checkBoxProbe26"));
		checkBoxProbe26->setGeometry(QRect(410, 350, 50, 20));
		checkBoxProbe26->setMinimumSize(QSize(50, 20));
		checkBoxProbe26->setMaximumSize(QSize(50, 20));
		checkBoxProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe9 = new QLineEdit(WidthClass);
		lineEditProbe9->setObjectName(QString::fromUtf8("lineEditProbe9"));
		lineEditProbe9->setGeometry(QRect(90, 470, 80, 20));
		lineEditProbe9->setMinimumSize(QSize(80, 20));
		lineEditProbe9->setMaximumSize(QSize(80, 20));
		lineEditProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe21 = new QCheckBox(WidthClass);
		checkBoxProbe21->setObjectName(QString::fromUtf8("checkBoxProbe21"));
		checkBoxProbe21->setGeometry(QRect(410, 150, 50, 20));
		checkBoxProbe21->setMinimumSize(QSize(50, 20));
		checkBoxProbe21->setMaximumSize(QSize(50, 20));
		checkBoxProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe7 = new QCheckBox(WidthClass);
		checkBoxProbe7->setObjectName(QString::fromUtf8("checkBoxProbe7"));
		checkBoxProbe7->setGeometry(QRect(40, 390, 50, 20));
		checkBoxProbe7->setMinimumSize(QSize(50, 20));
		checkBoxProbe7->setMaximumSize(QSize(50, 20));
		checkBoxProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe6 = new QLineEdit(WidthClass);
		lineEditProbe6->setObjectName(QString::fromUtf8("lineEditProbe6"));
		lineEditProbe6->setGeometry(QRect(90, 350, 80, 20));
		lineEditProbe6->setMinimumSize(QSize(80, 20));
		lineEditProbe6->setMaximumSize(QSize(80, 20));
		lineEditProbe6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe30 = new QCheckBox(WidthClass);
		checkBoxProbe30->setObjectName(QString::fromUtf8("checkBoxProbe30"));
		checkBoxProbe30->setGeometry(QRect(410, 510, 50, 20));
		checkBoxProbe30->setMinimumSize(QSize(50, 20));
		checkBoxProbe30->setMaximumSize(QSize(50, 20));
		checkBoxProbe30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_4 = new QLabel(WidthClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(170, 150, 30, 20));
		label_4->setMinimumSize(QSize(30, 20));
		label_4->setMaximumSize(QSize(30, 20));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe5 = new QLineEdit(WidthClass);
		lineEditProbe5->setObjectName(QString::fromUtf8("lineEditProbe5"));
		lineEditProbe5->setGeometry(QRect(90, 310, 80, 20));
		lineEditProbe5->setMinimumSize(QSize(80, 20));
		lineEditProbe5->setMaximumSize(QSize(80, 20));
		lineEditProbe5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe3 = new QCheckBox(WidthClass);
		checkBoxProbe3->setObjectName(QString::fromUtf8("checkBoxProbe3"));
		checkBoxProbe3->setGeometry(QRect(40, 230, 50, 20));
		checkBoxProbe3->setMinimumSize(QSize(50, 20));
		checkBoxProbe3->setMaximumSize(QSize(50, 20));
		checkBoxProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe4 = new QLineEdit(WidthClass);
		lineEditProbe4->setObjectName(QString::fromUtf8("lineEditProbe4"));
		lineEditProbe4->setGeometry(QRect(90, 270, 80, 20));
		lineEditProbe4->setMinimumSize(QSize(80, 20));
		lineEditProbe4->setMaximumSize(QSize(80, 20));
		lineEditProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe29 = new QCheckBox(WidthClass);
		checkBoxProbe29->setObjectName(QString::fromUtf8("checkBoxProbe29"));
		checkBoxProbe29->setGeometry(QRect(410, 470, 50, 20));
		checkBoxProbe29->setMinimumSize(QSize(50, 20));
		checkBoxProbe29->setMaximumSize(QSize(50, 20));
		checkBoxProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe29 = new QLineEdit(WidthClass);
		lineEditProbe29->setObjectName(QString::fromUtf8("lineEditProbe29"));
		lineEditProbe29->setGeometry(QRect(470, 470, 80, 20));
		lineEditProbe29->setMinimumSize(QSize(80, 20));
		lineEditProbe29->setMaximumSize(QSize(80, 20));
		lineEditProbe29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe22 = new QLineEdit(WidthClass);
		lineEditProbe22->setObjectName(QString::fromUtf8("lineEditProbe22"));
		lineEditProbe22->setGeometry(QRect(470, 190, 80, 20));
		lineEditProbe22->setMinimumSize(QSize(80, 20));
		lineEditProbe22->setMaximumSize(QSize(80, 20));
		lineEditProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_6 = new QLabel(WidthClass);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(170, 230, 30, 20));
		label_6->setMinimumSize(QSize(30, 20));
		label_6->setMaximumSize(QSize(30, 20));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe27 = new QLineEdit(WidthClass);
		lineEditProbe27->setObjectName(QString::fromUtf8("lineEditProbe27"));
		lineEditProbe27->setGeometry(QRect(470, 390, 80, 20));
		lineEditProbe27->setMinimumSize(QSize(80, 20));
		lineEditProbe27->setMaximumSize(QSize(80, 20));
		lineEditProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe11 = new QLineEdit(WidthClass);
		lineEditProbe11->setObjectName(QString::fromUtf8("lineEditProbe11"));
		lineEditProbe11->setGeometry(QRect(280, 150, 80, 20));
		lineEditProbe11->setMinimumSize(QSize(80, 20));
		lineEditProbe11->setMaximumSize(QSize(80, 20));
		lineEditProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe23 = new QCheckBox(WidthClass);
		checkBoxProbe23->setObjectName(QString::fromUtf8("checkBoxProbe23"));
		checkBoxProbe23->setGeometry(QRect(410, 230, 50, 20));
		checkBoxProbe23->setMinimumSize(QSize(50, 20));
		checkBoxProbe23->setMaximumSize(QSize(50, 20));
		checkBoxProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAll = new QRadioButton(WidthClass);
		radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
		radioButtonAll->setGeometry(QRect(40, 60, 89, 16));
		radioButtonAll->setMinimumSize(QSize(89, 16));
		radioButtonAll->setMaximumSize(QSize(89, 16));
		radioButtonAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		radioButtonAny = new QRadioButton(WidthClass);
		radioButtonAny->setObjectName(QString::fromUtf8("radioButtonAny"));
		radioButtonAny->setGeometry(QRect(40, 100, 89, 16));
		radioButtonAny->setMinimumSize(QSize(89, 16));
		radioButtonAny->setMaximumSize(QSize(89, 16));
		radioButtonAny->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe2 = new QCheckBox(WidthClass);
		checkBoxProbe2->setObjectName(QString::fromUtf8("checkBoxProbe2"));
		checkBoxProbe2->setGeometry(QRect(40, 190, 50, 20));
		checkBoxProbe2->setMinimumSize(QSize(50, 20));
		checkBoxProbe2->setMaximumSize(QSize(50, 20));
		checkBoxProbe2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe9 = new QCheckBox(WidthClass);
		checkBoxProbe9->setObjectName(QString::fromUtf8("checkBoxProbe9"));
		checkBoxProbe9->setGeometry(QRect(40, 470, 50, 20));
		checkBoxProbe9->setMinimumSize(QSize(50, 20));
		checkBoxProbe9->setMaximumSize(QSize(50, 20));
		checkBoxProbe9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe1 = new QLineEdit(WidthClass);
		lineEditProbe1->setObjectName(QString::fromUtf8("lineEditProbe1"));
		lineEditProbe1->setGeometry(QRect(90, 150, 80, 20));
		lineEditProbe1->setMinimumSize(QSize(80, 20));
		lineEditProbe1->setMaximumSize(QSize(80, 20));
		lineEditProbe1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe8 = new QCheckBox(WidthClass);
		checkBoxProbe8->setObjectName(QString::fromUtf8("checkBoxProbe8"));
		checkBoxProbe8->setGeometry(QRect(40, 430, 50, 20));
		checkBoxProbe8->setMinimumSize(QSize(50, 20));
		checkBoxProbe8->setMaximumSize(QSize(50, 20));
		checkBoxProbe8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe26 = new QLineEdit(WidthClass);
		lineEditProbe26->setObjectName(QString::fromUtf8("lineEditProbe26"));
		lineEditProbe26->setGeometry(QRect(470, 350, 80, 20));
		lineEditProbe26->setMinimumSize(QSize(80, 20));
		lineEditProbe26->setMaximumSize(QSize(80, 20));
		lineEditProbe26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe18 = new QCheckBox(WidthClass);
		checkBoxProbe18->setObjectName(QString::fromUtf8("checkBoxProbe18"));
		checkBoxProbe18->setGeometry(QRect(220, 430, 50, 20));
		checkBoxProbe18->setMinimumSize(QSize(50, 20));
		checkBoxProbe18->setMaximumSize(QSize(50, 20));
		checkBoxProbe18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe25 = new QLineEdit(WidthClass);
		lineEditProbe25->setObjectName(QString::fromUtf8("lineEditProbe25"));
		lineEditProbe25->setGeometry(QRect(470, 310, 80, 20));
		lineEditProbe25->setMinimumSize(QSize(80, 20));
		lineEditProbe25->setMaximumSize(QSize(80, 20));
		lineEditProbe25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonNo = new QPushButton(WidthClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(690, 540, 70, 25));
		pushButtonNo->setMinimumSize(QSize(70, 25));
		pushButtonNo->setMaximumSize(QSize(70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe20 = new QCheckBox(WidthClass);
		checkBoxProbe20->setObjectName(QString::fromUtf8("checkBoxProbe20"));
		checkBoxProbe20->setGeometry(QRect(220, 510, 50, 20));
		checkBoxProbe20->setMinimumSize(QSize(50, 20));
		checkBoxProbe20->setMaximumSize(QSize(50, 20));
		checkBoxProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_2 = new QLabel(WidthClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(340, 30, 120, 20));
		label_2->setMinimumSize(QSize(120, 20));
		label_2->setMaximumSize(QSize(120, 20));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 15pt \"\345\256\213\344\275\223\";"));
		lineEditProbe20 = new QLineEdit(WidthClass);
		lineEditProbe20->setObjectName(QString::fromUtf8("lineEditProbe20"));
		lineEditProbe20->setGeometry(QRect(280, 510, 80, 20));
		lineEditProbe20->setMinimumSize(QSize(80, 20));
		lineEditProbe20->setMaximumSize(QSize(80, 20));
		lineEditProbe20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe21 = new QLineEdit(WidthClass);
		lineEditProbe21->setObjectName(QString::fromUtf8("lineEditProbe21"));
		lineEditProbe21->setGeometry(QRect(470, 150, 80, 20));
		lineEditProbe21->setMinimumSize(QSize(80, 20));
		lineEditProbe21->setMaximumSize(QSize(80, 20));
		lineEditProbe21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe24 = new QCheckBox(WidthClass);
		checkBoxProbe24->setObjectName(QString::fromUtf8("checkBoxProbe24"));
		checkBoxProbe24->setGeometry(QRect(410, 270, 50, 20));
		checkBoxProbe24->setMinimumSize(QSize(50, 20));
		checkBoxProbe24->setMaximumSize(QSize(50, 20));
		checkBoxProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe7 = new QLineEdit(WidthClass);
		lineEditProbe7->setObjectName(QString::fromUtf8("lineEditProbe7"));
		lineEditProbe7->setGeometry(QRect(90, 390, 80, 20));
		lineEditProbe7->setMinimumSize(QSize(80, 20));
		lineEditProbe7->setMaximumSize(QSize(80, 20));
		lineEditProbe7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditAll = new QLineEdit(WidthClass);
		lineEditAll->setObjectName(QString::fromUtf8("lineEditAll"));
		lineEditAll->setGeometry(QRect(130, 60, 80, 20));
		lineEditAll->setMinimumSize(QSize(80, 20));
		lineEditAll->setMaximumSize(QSize(80, 20));
		lineEditAll->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe4 = new QCheckBox(WidthClass);
		checkBoxProbe4->setObjectName(QString::fromUtf8("checkBoxProbe4"));
		checkBoxProbe4->setGeometry(QRect(40, 270, 50, 20));
		checkBoxProbe4->setMinimumSize(QSize(50, 20));
		checkBoxProbe4->setMaximumSize(QSize(50, 20));
		checkBoxProbe4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe3 = new QLineEdit(WidthClass);
		lineEditProbe3->setObjectName(QString::fromUtf8("lineEditProbe3"));
		lineEditProbe3->setGeometry(QRect(90, 230, 80, 20));
		lineEditProbe3->setMinimumSize(QSize(80, 20));
		lineEditProbe3->setMaximumSize(QSize(80, 20));
		lineEditProbe3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe24 = new QLineEdit(WidthClass);
		lineEditProbe24->setObjectName(QString::fromUtf8("lineEditProbe24"));
		lineEditProbe24->setGeometry(QRect(470, 270, 80, 20));
		lineEditProbe24->setMinimumSize(QSize(80, 20));
		lineEditProbe24->setMaximumSize(QSize(80, 20));
		lineEditProbe24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe12 = new QCheckBox(WidthClass);
		checkBoxProbe12->setObjectName(QString::fromUtf8("checkBoxProbe12"));
		checkBoxProbe12->setGeometry(QRect(220, 190, 50, 20));
		checkBoxProbe12->setMinimumSize(QSize(50, 20));
		checkBoxProbe12->setMaximumSize(QSize(50, 20));
		checkBoxProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_3 = new QLabel(WidthClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(210, 100, 30, 20));
		label_3->setMinimumSize(QSize(30, 20));
		label_3->setMaximumSize(QSize(30, 20));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe15 = new QLineEdit(WidthClass);
		lineEditProbe15->setObjectName(QString::fromUtf8("lineEditProbe15"));
		lineEditProbe15->setGeometry(QRect(280, 310, 80, 20));
		lineEditProbe15->setMinimumSize(QSize(80, 20));
		lineEditProbe15->setMaximumSize(QSize(80, 20));
		lineEditProbe15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe22 = new QCheckBox(WidthClass);
		checkBoxProbe22->setObjectName(QString::fromUtf8("checkBoxProbe22"));
		checkBoxProbe22->setGeometry(QRect(410, 190, 50, 20));
		checkBoxProbe22->setMinimumSize(QSize(50, 20));
		checkBoxProbe22->setMaximumSize(QSize(50, 20));
		checkBoxProbe22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe11 = new QCheckBox(WidthClass);
		checkBoxProbe11->setObjectName(QString::fromUtf8("checkBoxProbe11"));
		checkBoxProbe11->setGeometry(QRect(220, 150, 50, 20));
		checkBoxProbe11->setMinimumSize(QSize(50, 20));
		checkBoxProbe11->setMaximumSize(QSize(50, 20));
		checkBoxProbe11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe12 = new QLineEdit(WidthClass);
		lineEditProbe12->setObjectName(QString::fromUtf8("lineEditProbe12"));
		lineEditProbe12->setGeometry(QRect(280, 190, 80, 20));
		lineEditProbe12->setMinimumSize(QSize(80, 20));
		lineEditProbe12->setMaximumSize(QSize(80, 20));
		lineEditProbe12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe23 = new QLineEdit(WidthClass);
		lineEditProbe23->setObjectName(QString::fromUtf8("lineEditProbe23"));
		lineEditProbe23->setGeometry(QRect(470, 230, 80, 20));
		lineEditProbe23->setMinimumSize(QSize(80, 20));
		lineEditProbe23->setMaximumSize(QSize(80, 20));
		lineEditProbe23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label = new QLabel(WidthClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(210, 60, 30, 20));
		label->setMinimumSize(QSize(30, 20));
		label->setMaximumSize(QSize(30, 20));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe14 = new QCheckBox(WidthClass);
		checkBoxProbe14->setObjectName(QString::fromUtf8("checkBoxProbe14"));
		checkBoxProbe14->setGeometry(QRect(220, 270, 50, 20));
		checkBoxProbe14->setMinimumSize(QSize(50, 20));
		checkBoxProbe14->setMaximumSize(QSize(50, 20));
		checkBoxProbe14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe27 = new QCheckBox(WidthClass);
		checkBoxProbe27->setObjectName(QString::fromUtf8("checkBoxProbe27"));
		checkBoxProbe27->setGeometry(QRect(410, 390, 50, 20));
		checkBoxProbe27->setMinimumSize(QSize(50, 20));
		checkBoxProbe27->setMaximumSize(QSize(50, 20));
		checkBoxProbe27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_7 = new QLabel(WidthClass);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(170, 350, 30, 20));
		label_7->setMinimumSize(QSize(30, 20));
		label_7->setMaximumSize(QSize(30, 20));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_8 = new QLabel(WidthClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(170, 310, 30, 20));
		label_8->setMinimumSize(QSize(30, 20));
		label_8->setMaximumSize(QSize(30, 20));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_9 = new QLabel(WidthClass);
		label_9->setObjectName(QString::fromUtf8("label_9"));
		label_9->setGeometry(QRect(170, 270, 30, 20));
		label_9->setMinimumSize(QSize(30, 20));
		label_9->setMaximumSize(QSize(30, 20));
		label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_10 = new QLabel(WidthClass);
		label_10->setObjectName(QString::fromUtf8("label_10"));
		label_10->setGeometry(QRect(170, 430, 30, 20));
		label_10->setMinimumSize(QSize(30, 20));
		label_10->setMaximumSize(QSize(30, 20));
		label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_11 = new QLabel(WidthClass);
		label_11->setObjectName(QString::fromUtf8("label_11"));
		label_11->setGeometry(QRect(170, 510, 30, 20));
		label_11->setMinimumSize(QSize(30, 20));
		label_11->setMaximumSize(QSize(30, 20));
		label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_12 = new QLabel(WidthClass);
		label_12->setObjectName(QString::fromUtf8("label_12"));
		label_12->setGeometry(QRect(170, 390, 30, 20));
		label_12->setMinimumSize(QSize(30, 20));
		label_12->setMaximumSize(QSize(30, 20));
		label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_13 = new QLabel(WidthClass);
		label_13->setObjectName(QString::fromUtf8("label_13"));
		label_13->setGeometry(QRect(170, 470, 30, 20));
		label_13->setMinimumSize(QSize(30, 20));
		label_13->setMaximumSize(QSize(30, 20));
		label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_14 = new QLabel(WidthClass);
		label_14->setObjectName(QString::fromUtf8("label_14"));
		label_14->setGeometry(QRect(360, 470, 30, 20));
		label_14->setMinimumSize(QSize(30, 20));
		label_14->setMaximumSize(QSize(30, 20));
		label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_15 = new QLabel(WidthClass);
		label_15->setObjectName(QString::fromUtf8("label_15"));
		label_15->setGeometry(QRect(360, 430, 30, 20));
		label_15->setMinimumSize(QSize(30, 20));
		label_15->setMaximumSize(QSize(30, 20));
		label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_16 = new QLabel(WidthClass);
		label_16->setObjectName(QString::fromUtf8("label_16"));
		label_16->setGeometry(QRect(360, 390, 30, 20));
		label_16->setMinimumSize(QSize(30, 20));
		label_16->setMaximumSize(QSize(30, 20));
		label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_17 = new QLabel(WidthClass);
		label_17->setObjectName(QString::fromUtf8("label_17"));
		label_17->setGeometry(QRect(360, 190, 30, 20));
		label_17->setMinimumSize(QSize(30, 20));
		label_17->setMaximumSize(QSize(30, 20));
		label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_18 = new QLabel(WidthClass);
		label_18->setObjectName(QString::fromUtf8("label_18"));
		label_18->setGeometry(QRect(360, 350, 30, 20));
		label_18->setMinimumSize(QSize(30, 20));
		label_18->setMaximumSize(QSize(30, 20));
		label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_19 = new QLabel(WidthClass);
		label_19->setObjectName(QString::fromUtf8("label_19"));
		label_19->setGeometry(QRect(360, 510, 30, 20));
		label_19->setMinimumSize(QSize(30, 20));
		label_19->setMaximumSize(QSize(30, 20));
		label_19->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_20 = new QLabel(WidthClass);
		label_20->setObjectName(QString::fromUtf8("label_20"));
		label_20->setGeometry(QRect(360, 270, 30, 20));
		label_20->setMinimumSize(QSize(30, 20));
		label_20->setMaximumSize(QSize(30, 20));
		label_20->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_21 = new QLabel(WidthClass);
		label_21->setObjectName(QString::fromUtf8("label_21"));
		label_21->setGeometry(QRect(360, 230, 30, 20));
		label_21->setMinimumSize(QSize(30, 20));
		label_21->setMaximumSize(QSize(30, 20));
		label_21->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_22 = new QLabel(WidthClass);
		label_22->setObjectName(QString::fromUtf8("label_22"));
		label_22->setGeometry(QRect(360, 310, 30, 20));
		label_22->setMinimumSize(QSize(30, 20));
		label_22->setMaximumSize(QSize(30, 20));
		label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_23 = new QLabel(WidthClass);
		label_23->setObjectName(QString::fromUtf8("label_23"));
		label_23->setGeometry(QRect(360, 150, 30, 20));
		label_23->setMinimumSize(QSize(30, 20));
		label_23->setMaximumSize(QSize(30, 20));
		label_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_24 = new QLabel(WidthClass);
		label_24->setObjectName(QString::fromUtf8("label_24"));
		label_24->setGeometry(QRect(550, 150, 30, 20));
		label_24->setMinimumSize(QSize(30, 20));
		label_24->setMaximumSize(QSize(30, 20));
		label_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_25 = new QLabel(WidthClass);
		label_25->setObjectName(QString::fromUtf8("label_25"));
		label_25->setGeometry(QRect(550, 310, 30, 20));
		label_25->setMinimumSize(QSize(30, 20));
		label_25->setMaximumSize(QSize(30, 20));
		label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_26 = new QLabel(WidthClass);
		label_26->setObjectName(QString::fromUtf8("label_26"));
		label_26->setGeometry(QRect(550, 350, 30, 20));
		label_26->setMinimumSize(QSize(30, 20));
		label_26->setMaximumSize(QSize(30, 20));
		label_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_27 = new QLabel(WidthClass);
		label_27->setObjectName(QString::fromUtf8("label_27"));
		label_27->setGeometry(QRect(550, 470, 30, 20));
		label_27->setMinimumSize(QSize(30, 20));
		label_27->setMaximumSize(QSize(30, 20));
		label_27->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_28 = new QLabel(WidthClass);
		label_28->setObjectName(QString::fromUtf8("label_28"));
		label_28->setGeometry(QRect(550, 390, 30, 20));
		label_28->setMinimumSize(QSize(30, 20));
		label_28->setMaximumSize(QSize(30, 20));
		label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_29 = new QLabel(WidthClass);
		label_29->setObjectName(QString::fromUtf8("label_29"));
		label_29->setGeometry(QRect(550, 230, 30, 20));
		label_29->setMinimumSize(QSize(30, 20));
		label_29->setMaximumSize(QSize(30, 20));
		label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_30 = new QLabel(WidthClass);
		label_30->setObjectName(QString::fromUtf8("label_30"));
		label_30->setGeometry(QRect(550, 270, 30, 20));
		label_30->setMinimumSize(QSize(30, 20));
		label_30->setMaximumSize(QSize(30, 20));
		label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_31 = new QLabel(WidthClass);
		label_31->setObjectName(QString::fromUtf8("label_31"));
		label_31->setGeometry(QRect(550, 510, 30, 20));
		label_31->setMinimumSize(QSize(30, 20));
		label_31->setMaximumSize(QSize(30, 20));
		label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_32 = new QLabel(WidthClass);
		label_32->setObjectName(QString::fromUtf8("label_32"));
		label_32->setGeometry(QRect(550, 190, 30, 20));
		label_32->setMinimumSize(QSize(30, 20));
		label_32->setMaximumSize(QSize(30, 20));
		label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_33 = new QLabel(WidthClass);
		label_33->setObjectName(QString::fromUtf8("label_33"));
		label_33->setGeometry(QRect(550, 430, 30, 20));
		label_33->setMinimumSize(QSize(30, 20));
		label_33->setMaximumSize(QSize(30, 20));
		label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe36 = new QCheckBox(WidthClass);
		checkBoxProbe36->setObjectName(QString::fromUtf8("checkBoxProbe36"));
		checkBoxProbe36->setGeometry(QRect(600, 350, 50, 20));
		checkBoxProbe36->setMinimumSize(QSize(50, 20));
		checkBoxProbe36->setMaximumSize(QSize(50, 20));
		checkBoxProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_34 = new QLabel(WidthClass);
		label_34->setObjectName(QString::fromUtf8("label_34"));
		label_34->setGeometry(QRect(740, 310, 30, 20));
		label_34->setMinimumSize(QSize(30, 20));
		label_34->setMaximumSize(QSize(30, 20));
		label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_35 = new QLabel(WidthClass);
		label_35->setObjectName(QString::fromUtf8("label_35"));
		label_35->setGeometry(QRect(740, 190, 30, 20));
		label_35->setMinimumSize(QSize(30, 20));
		label_35->setMaximumSize(QSize(30, 20));
		label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe34 = new QLineEdit(WidthClass);
		lineEditProbe34->setObjectName(QString::fromUtf8("lineEditProbe34"));
		lineEditProbe34->setGeometry(QRect(660, 270, 80, 20));
		lineEditProbe34->setMinimumSize(QSize(80, 20));
		lineEditProbe34->setMaximumSize(QSize(80, 20));
		lineEditProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe32 = new QLineEdit(WidthClass);
		lineEditProbe32->setObjectName(QString::fromUtf8("lineEditProbe32"));
		lineEditProbe32->setGeometry(QRect(660, 190, 80, 20));
		lineEditProbe32->setMinimumSize(QSize(80, 20));
		lineEditProbe32->setMaximumSize(QSize(80, 20));
		lineEditProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe32 = new QCheckBox(WidthClass);
		checkBoxProbe32->setObjectName(QString::fromUtf8("checkBoxProbe32"));
		checkBoxProbe32->setGeometry(QRect(600, 190, 50, 20));
		checkBoxProbe32->setMinimumSize(QSize(50, 20));
		checkBoxProbe32->setMaximumSize(QSize(50, 20));
		checkBoxProbe32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_36 = new QLabel(WidthClass);
		label_36->setObjectName(QString::fromUtf8("label_36"));
		label_36->setGeometry(QRect(740, 150, 30, 20));
		label_36->setMinimumSize(QSize(30, 20));
		label_36->setMaximumSize(QSize(30, 20));
		label_36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe31 = new QCheckBox(WidthClass);
		checkBoxProbe31->setObjectName(QString::fromUtf8("checkBoxProbe31"));
		checkBoxProbe31->setGeometry(QRect(600, 150, 50, 20));
		checkBoxProbe31->setMinimumSize(QSize(50, 20));
		checkBoxProbe31->setMaximumSize(QSize(50, 20));
		checkBoxProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe34 = new QCheckBox(WidthClass);
		checkBoxProbe34->setObjectName(QString::fromUtf8("checkBoxProbe34"));
		checkBoxProbe34->setGeometry(QRect(600, 270, 50, 20));
		checkBoxProbe34->setMinimumSize(QSize(50, 20));
		checkBoxProbe34->setMaximumSize(QSize(50, 20));
		checkBoxProbe34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe33 = new QLineEdit(WidthClass);
		lineEditProbe33->setObjectName(QString::fromUtf8("lineEditProbe33"));
		lineEditProbe33->setGeometry(QRect(660, 230, 80, 20));
		lineEditProbe33->setMinimumSize(QSize(80, 20));
		lineEditProbe33->setMaximumSize(QSize(80, 20));
		lineEditProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		lineEditProbe36 = new QLineEdit(WidthClass);
		lineEditProbe36->setObjectName(QString::fromUtf8("lineEditProbe36"));
		lineEditProbe36->setGeometry(QRect(660, 350, 80, 20));
		lineEditProbe36->setMinimumSize(QSize(80, 20));
		lineEditProbe36->setMaximumSize(QSize(80, 20));
		lineEditProbe36->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_37 = new QLabel(WidthClass);
		label_37->setObjectName(QString::fromUtf8("label_37"));
		label_37->setGeometry(QRect(740, 270, 30, 20));
		label_37->setMinimumSize(QSize(30, 20));
		label_37->setMaximumSize(QSize(30, 20));
		label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe35 = new QLineEdit(WidthClass);
		lineEditProbe35->setObjectName(QString::fromUtf8("lineEditProbe35"));
		lineEditProbe35->setGeometry(QRect(660, 310, 80, 20));
		lineEditProbe35->setMinimumSize(QSize(80, 20));
		lineEditProbe35->setMaximumSize(QSize(80, 20));
		lineEditProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		checkBoxProbe33 = new QCheckBox(WidthClass);
		checkBoxProbe33->setObjectName(QString::fromUtf8("checkBoxProbe33"));
		checkBoxProbe33->setGeometry(QRect(600, 230, 50, 20));
		checkBoxProbe33->setMinimumSize(QSize(50, 20));
		checkBoxProbe33->setMaximumSize(QSize(50, 20));
		checkBoxProbe33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		label_38 = new QLabel(WidthClass);
		label_38->setObjectName(QString::fromUtf8("label_38"));
		label_38->setGeometry(QRect(740, 230, 30, 20));
		label_38->setMinimumSize(QSize(30, 20));
		label_38->setMaximumSize(QSize(30, 20));
		label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		lineEditProbe31 = new QLineEdit(WidthClass);
		lineEditProbe31->setObjectName(QString::fromUtf8("lineEditProbe31"));
		lineEditProbe31->setGeometry(QRect(660, 150, 80, 20));
		lineEditProbe31->setMinimumSize(QSize(80, 20));
		lineEditProbe31->setMaximumSize(QSize(80, 20));
		lineEditProbe31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		label_39 = new QLabel(WidthClass);
		label_39->setObjectName(QString::fromUtf8("label_39"));
		label_39->setGeometry(QRect(740, 350, 30, 20));
		label_39->setMinimumSize(QSize(30, 20));
		label_39->setMaximumSize(QSize(30, 20));
		label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));
		checkBoxProbe35 = new QCheckBox(WidthClass);
		checkBoxProbe35->setObjectName(QString::fromUtf8("checkBoxProbe35"));
		checkBoxProbe35->setGeometry(QRect(600, 310, 50, 20));
		checkBoxProbe35->setMinimumSize(QSize(50, 20));
		checkBoxProbe35->setMaximumSize(QSize(50, 20));
		checkBoxProbe35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 75 12pt \"\345\256\213\344\275\223\";"));

		retranslateUi(WidthClass);

		QMetaObject::connectSlotsByName(WidthClass);
	} // setupUi

	void retranslateUi(QDialog *WidthClass)
	{
		WidthClass->setWindowTitle(QApplication::translate("WidthClass", "Width", 0, QApplication::UnicodeUTF8));
		checkBoxProbe17->setText(QApplication::translate("WidthClass", " 17", 0, QApplication::UnicodeUTF8));
		pushButtonAllIn->setText(QApplication::translate("WidthClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		checkBoxProbe19->setText(QApplication::translate("WidthClass", " 19", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe13->setText(QApplication::translate("WidthClass", " 13", 0, QApplication::UnicodeUTF8));
		checkBoxProbe15->setText(QApplication::translate("WidthClass", " 15", 0, QApplication::UnicodeUTF8));
		checkBoxProbe10->setText(QApplication::translate("WidthClass", " 10", 0, QApplication::UnicodeUTF8));
		checkBoxProbe25->setText(QApplication::translate("WidthClass", " 25", 0, QApplication::UnicodeUTF8));
		checkBoxProbe6->setText(QApplication::translate("WidthClass", " 6", 0, QApplication::UnicodeUTF8));
		checkBoxProbe1->setText(QApplication::translate("WidthClass", " 1", 0, QApplication::UnicodeUTF8));
		pushButtonAnyIn->setText(QApplication::translate("WidthClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
		checkBoxProbe28->setText(QApplication::translate("WidthClass", " 28", 0, QApplication::UnicodeUTF8));
		checkBoxProbe5->setText(QApplication::translate("WidthClass", " 5", 0, QApplication::UnicodeUTF8));
		checkBoxProbe16->setText(QApplication::translate("WidthClass", " 16", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("WidthClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		checkBoxProbe26->setText(QApplication::translate("WidthClass", " 26", 0, QApplication::UnicodeUTF8));
		checkBoxProbe21->setText(QApplication::translate("WidthClass", " 21", 0, QApplication::UnicodeUTF8));
		checkBoxProbe7->setText(QApplication::translate("WidthClass", " 7", 0, QApplication::UnicodeUTF8));
		checkBoxProbe30->setText(QApplication::translate("WidthClass", " 30", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe3->setText(QApplication::translate("WidthClass", " 3", 0, QApplication::UnicodeUTF8));
		checkBoxProbe29->setText(QApplication::translate("WidthClass", " 29", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe23->setText(QApplication::translate("WidthClass", " 23", 0, QApplication::UnicodeUTF8));
		radioButtonAll->setText(QApplication::translate("WidthClass", "\345\205\250\351\203\250\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		radioButtonAny->setText(QApplication::translate("WidthClass", "\346\235\241\344\273\266\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
		checkBoxProbe2->setText(QApplication::translate("WidthClass", " 2", 0, QApplication::UnicodeUTF8));
		checkBoxProbe9->setText(QApplication::translate("WidthClass", " 9", 0, QApplication::UnicodeUTF8));
		checkBoxProbe8->setText(QApplication::translate("WidthClass", " 8", 0, QApplication::UnicodeUTF8));
		checkBoxProbe18->setText(QApplication::translate("WidthClass", " 18", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("WidthClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		checkBoxProbe20->setText(QApplication::translate("WidthClass", " 20", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("WidthClass", "\346\212\221\345\210\266\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		checkBoxProbe24->setText(QApplication::translate("WidthClass", " 24", 0, QApplication::UnicodeUTF8));
		checkBoxProbe4->setText(QApplication::translate("WidthClass", " 4", 0, QApplication::UnicodeUTF8));
		checkBoxProbe12->setText(QApplication::translate("WidthClass", " 12", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe22->setText(QApplication::translate("WidthClass", " 22", 0, QApplication::UnicodeUTF8));
		checkBoxProbe11->setText(QApplication::translate("WidthClass", " 11", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe14->setText(QApplication::translate("WidthClass", " 14", 0, QApplication::UnicodeUTF8));
		checkBoxProbe27->setText(QApplication::translate("WidthClass", " 27", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_9->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_10->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_11->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_12->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_13->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_14->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_15->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_16->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_17->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_18->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_19->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_20->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_21->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_22->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_23->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_24->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_25->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_26->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_27->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_28->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_29->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_30->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_31->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_32->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_33->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe36->setText(QApplication::translate("WidthClass", " 36", 0, QApplication::UnicodeUTF8));
		label_34->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_35->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe32->setText(QApplication::translate("WidthClass", " 32", 0, QApplication::UnicodeUTF8));
		label_36->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe31->setText(QApplication::translate("WidthClass", " 31", 0, QApplication::UnicodeUTF8));
		checkBoxProbe34->setText(QApplication::translate("WidthClass", " 34", 0, QApplication::UnicodeUTF8));
		label_37->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe33->setText(QApplication::translate("WidthClass", " 33", 0, QApplication::UnicodeUTF8));
		label_38->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		label_39->setText(QApplication::translate("WidthClass", "(%)", 0, QApplication::UnicodeUTF8));
		checkBoxProbe35->setText(QApplication::translate("WidthClass", " 35", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class WidthClass: public Ui_WidthClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDTH_H




/********************************************************************************
** Form generated from reading UI file 'formitem.ui'
**
** Created: Mon Oct 28 14:04:55 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMITEM_H
#define UI_FORMITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormItemClass
{
public:
	QLabel *label;
	QWidget *widget;
	QLabel *label_2;
	QLabel *label_3;
	QComboBox *comboBoxOperator;
	QPushButton *pushButtonOperatorAdd;
	QPushButton *pushButtonOperatorDel;
	QComboBox *comboBoxClasse;
	QPushButton *pushButtonClasseDel;
	QPushButton *pushButtonClasseAdd;
	QLabel *label_4;
	QRadioButton *radioButtonOnLine;
	QRadioButton *radioButtonOffLine;
	QWidget *widget_2;
	QLabel *label_5;
	QLabel *label_6;
	QComboBox *comboBoxTestStandard;
	QPushButton *pushButtonStandardAdd;
	QPushButton *pushButtonStandardDel;
	QPushButton *pushButtonRankDel;
	QLabel *label_7;
	QComboBox *comboBoxTestRank;
	QPushButton *pushButtonRankAdd;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QLabel *label_8;
	QLineEdit *lineEditNumber;
	QPushButton *pushButtonIn;

	void setupUi(QDialog *FormItemClass)
	{
		if (FormItemClass->objectName().isEmpty())
			FormItemClass->setObjectName(QString::fromUtf8("FormItemClass"));
		FormItemClass->resize(501, 468);
		FormItemClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label = new QLabel(FormItemClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(180, 10, 131, 31));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 16pt \"\346\226\260\345\256\213\344\275\223\";"));
		widget = new QWidget(FormItemClass);
		widget->setObjectName(QString::fromUtf8("widget"));
		widget->setGeometry(QRect(20, 80, 461, 131));
		widget->setStyleSheet(QString::fromUtf8("QWidget#widget{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		label_2 = new QLabel(widget);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(20, 10, 81, 18));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_3 = new QLabel(widget);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(20, 60, 61, 22));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		comboBoxOperator = new QComboBox(widget);
		comboBoxOperator->setObjectName(QString::fromUtf8("comboBoxOperator"));
		comboBoxOperator->setGeometry(QRect(100, 60, 121, 22));
		comboBoxOperator->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxOperator->setEditable(true);
		pushButtonOperatorAdd = new QPushButton(widget);
		pushButtonOperatorAdd->setObjectName(QString::fromUtf8("pushButtonOperatorAdd"));
		pushButtonOperatorAdd->setGeometry(QRect(100, 90, 51, 21));
		pushButtonOperatorAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonOperatorDel = new QPushButton(widget);
		pushButtonOperatorDel->setObjectName(QString::fromUtf8("pushButtonOperatorDel"));
		pushButtonOperatorDel->setGeometry(QRect(170, 90, 51, 21));
		pushButtonOperatorDel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		comboBoxClasse = new QComboBox(widget);
		comboBoxClasse->setObjectName(QString::fromUtf8("comboBoxClasse"));
		comboBoxClasse->setGeometry(QRect(320, 60, 121, 22));
		comboBoxClasse->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxClasse->setEditable(true);
		pushButtonClasseDel = new QPushButton(widget);
		pushButtonClasseDel->setObjectName(QString::fromUtf8("pushButtonClasseDel"));
		pushButtonClasseDel->setGeometry(QRect(390, 90, 51, 21));
		pushButtonClasseDel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonClasseAdd = new QPushButton(widget);
		pushButtonClasseAdd->setObjectName(QString::fromUtf8("pushButtonClasseAdd"));
		pushButtonClasseAdd->setGeometry(QRect(320, 90, 51, 21));
		pushButtonClasseAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_4 = new QLabel(widget);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(270, 60, 41, 22));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		radioButtonOnLine = new QRadioButton(FormItemClass);
		radioButtonOnLine->setObjectName(QString::fromUtf8("radioButtonOnLine"));
		radioButtonOnLine->setGeometry(QRect(80, 50, 101, 21));
		radioButtonOnLine->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		radioButtonOffLine = new QRadioButton(FormItemClass);
		radioButtonOffLine->setObjectName(QString::fromUtf8("radioButtonOffLine"));
		radioButtonOffLine->setGeometry(QRect(310, 50, 101, 20));
		radioButtonOffLine->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		widget_2 = new QWidget(FormItemClass);
		widget_2->setObjectName(QString::fromUtf8("widget_2"));
		widget_2->setGeometry(QRect(20, 240, 461, 131));
		widget_2->setStyleSheet(QString::fromUtf8("QWidget#widget_2{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		label_5 = new QLabel(widget_2);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(20, 10, 81, 18));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_6 = new QLabel(widget_2);
		label_6->setObjectName(QString::fromUtf8("label_6"));
		label_6->setGeometry(QRect(20, 60, 81, 22));
		label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		comboBoxTestStandard = new QComboBox(widget_2);
		comboBoxTestStandard->setObjectName(QString::fromUtf8("comboBoxTestStandard"));
		comboBoxTestStandard->setGeometry(QRect(103, 60, 121, 22));
		comboBoxTestStandard->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxTestStandard->setEditable(true);
		pushButtonStandardAdd = new QPushButton(widget_2);
		pushButtonStandardAdd->setObjectName(QString::fromUtf8("pushButtonStandardAdd"));
		pushButtonStandardAdd->setGeometry(QRect(103, 90, 51, 21));
		pushButtonStandardAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonStandardDel = new QPushButton(widget_2);
		pushButtonStandardDel->setObjectName(QString::fromUtf8("pushButtonStandardDel"));
		pushButtonStandardDel->setGeometry(QRect(173, 90, 51, 21));
		pushButtonStandardDel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonRankDel = new QPushButton(widget_2);
		pushButtonRankDel->setObjectName(QString::fromUtf8("pushButtonRankDel"));
		pushButtonRankDel->setGeometry(QRect(403, 90, 51, 21));
		pushButtonRankDel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_7 = new QLabel(widget_2);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(250, 60, 81, 22));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		comboBoxTestRank = new QComboBox(widget_2);
		comboBoxTestRank->setObjectName(QString::fromUtf8("comboBoxTestRank"));
		comboBoxTestRank->setGeometry(QRect(333, 60, 121, 22));
		comboBoxTestRank->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		comboBoxTestRank->setEditable(true);
		pushButtonRankAdd = new QPushButton(widget_2);
		pushButtonRankAdd->setObjectName(QString::fromUtf8("pushButtonRankAdd"));
		pushButtonRankAdd->setGeometry(QRect(333, 90, 51, 21));
		pushButtonRankAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(FormItemClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(330, 430, 60, 25));
		pushButtonYes->setMinimumSize(QSize(60, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(FormItemClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(420, 430, 60, 25));
		pushButtonNo->setMinimumSize(QSize(60, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_8 = new QLabel(FormItemClass);
		label_8->setObjectName(QString::fromUtf8("label_8"));
		label_8->setGeometry(QRect(40, 390, 231, 25));
		label_8->setMinimumSize(QSize(0, 25));
		label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditNumber = new QLineEdit(FormItemClass);
		lineEditNumber->setObjectName(QString::fromUtf8("lineEditNumber"));
		lineEditNumber->setGeometry(QRect(280, 390, 111, 25));
		lineEditNumber->setMinimumSize(QSize(0, 25));
		lineEditNumber->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"border:1px solid rgb(255,255,255);"));
		pushButtonIn = new QPushButton(FormItemClass);
		pushButtonIn->setObjectName(QString::fromUtf8("pushButtonIn"));
		pushButtonIn->setGeometry(QRect(420, 390, 60, 25));
		pushButtonIn->setMinimumSize(QSize(60, 25));
		pushButtonIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));

		retranslateUi(FormItemClass);

		QMetaObject::connectSlotsByName(FormItemClass);
	} // setupUi

	void retranslateUi(QDialog *FormItemClass)
	{
		FormItemClass->setWindowTitle(QApplication::translate("FormItemClass", "FormItem", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("FormItemClass", "\346\212\245\350\241\250\350\276\223\345\207\272\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("FormItemClass", "\344\272\272\345\221\230\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("FormItemClass", "\346\223\215\344\275\234\345\267\245", 0, QApplication::UnicodeUTF8));
		pushButtonOperatorAdd->setText(QApplication::translate("FormItemClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		pushButtonOperatorDel->setText(QApplication::translate("FormItemClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		pushButtonClasseDel->setText(QApplication::translate("FormItemClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		pushButtonClasseAdd->setText(QApplication::translate("FormItemClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("FormItemClass", "\347\217\255\346\254\241", 0, QApplication::UnicodeUTF8));
		radioButtonOnLine->setText(QApplication::translate("FormItemClass", "\345\234\250\347\272\277\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
		radioButtonOffLine->setText(QApplication::translate("FormItemClass", "\347\246\273\347\272\277\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("FormItemClass", "\346\240\207\345\207\206\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
		label_6->setText(QApplication::translate("FormItemClass", "\346\243\200\346\265\213\346\240\207\345\207\206", 0, QApplication::UnicodeUTF8));
		pushButtonStandardAdd->setText(QApplication::translate("FormItemClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		pushButtonStandardDel->setText(QApplication::translate("FormItemClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		pushButtonRankDel->setText(QApplication::translate("FormItemClass", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("FormItemClass", "\346\243\200\346\265\213\347\272\247\345\210\253", 0, QApplication::UnicodeUTF8));
		pushButtonRankAdd->setText(QApplication::translate("FormItemClass", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("FormItemClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("FormItemClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		label_8->setText(QApplication::translate("FormItemClass", "\350\256\276\347\275\256\345\215\263\345\260\206\346\243\200\346\265\213\347\232\204\351\222\242\347\256\241\347\274\226\345\217\267:", 0, QApplication::UnicodeUTF8));
		pushButtonIn->setText(QApplication::translate("FormItemClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class FormItemClass: public Ui_FormItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMITEM_H

/********************************************************************************
** Form generated from reading UI file 'formonline.ui'
**
** Created: Mon Nov 4 16:32:35 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMONLINE_H
#define UI_FORMONLINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FormOnLineClass
{
public:
	QCheckBox *checkBoxSingle;
	QLabel *label;
	QCheckBox *checkBoxWhole;
	QCheckBox *checkBoxTestOutPut;
	QCheckBox *checkBoxQualified;
	QCheckBox *checkBoxDefect;
	QCheckBox *checkBoxBadCouple;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;

	void setupUi(QDialog *FormOnLineClass)
	{
		if (FormOnLineClass->objectName().isEmpty())
			FormOnLineClass->setObjectName(QString::fromUtf8("FormOnLineClass"));
		FormOnLineClass->resize(315, 392);
		FormOnLineClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		checkBoxSingle = new QCheckBox(FormOnLineClass);
		checkBoxSingle->setObjectName(QString::fromUtf8("checkBoxSingle"));
		checkBoxSingle->setGeometry(QRect(60, 90, 171, 21));
		checkBoxSingle->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}\n"
			""));
		label = new QLabel(FormOnLineClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(60, 20, 191, 21));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 18pt \"\346\226\260\345\256\213\344\275\223\";"));
		checkBoxWhole = new QCheckBox(FormOnLineClass);
		checkBoxWhole->setObjectName(QString::fromUtf8("checkBoxWhole"));
		checkBoxWhole->setGeometry(QRect(60, 140, 171, 21));
		checkBoxWhole->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}\n"
			""));
		checkBoxTestOutPut = new QCheckBox(FormOnLineClass);
		checkBoxTestOutPut->setObjectName(QString::fromUtf8("checkBoxTestOutPut"));
		checkBoxTestOutPut->setGeometry(QRect(60, 190, 171, 21));
		checkBoxTestOutPut->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}\n"
			""));
		checkBoxQualified = new QCheckBox(FormOnLineClass);
		checkBoxQualified->setObjectName(QString::fromUtf8("checkBoxQualified"));
		checkBoxQualified->setGeometry(QRect(90, 240, 81, 21));
		checkBoxQualified->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}\n"
			""));
		checkBoxDefect = new QCheckBox(FormOnLineClass);
		checkBoxDefect->setObjectName(QString::fromUtf8("checkBoxDefect"));
		checkBoxDefect->setGeometry(QRect(90, 270, 101, 21));
		checkBoxDefect->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}\n"
			""));
		checkBoxBadCouple = new QCheckBox(FormOnLineClass);
		checkBoxBadCouple->setObjectName(QString::fromUtf8("checkBoxBadCouple"));
		checkBoxBadCouple->setGeometry(QRect(90, 300, 121, 21));
		checkBoxBadCouple->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}\n"
			""));
		pushButtonYes = new QPushButton(FormOnLineClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(120, 350, 61, 23));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(FormOnLineClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(210, 350, 61, 23));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));

		retranslateUi(FormOnLineClass);

		QMetaObject::connectSlotsByName(FormOnLineClass);
	} // setupUi

	void retranslateUi(QDialog *FormOnLineClass)
	{
		FormOnLineClass->setWindowTitle(QApplication::translate("FormOnLineClass", "FormOnLine", 0, QApplication::UnicodeUTF8));
		checkBoxSingle->setText(QApplication::translate("FormOnLineClass", "\345\215\225\344\270\200\346\212\245\350\241\250\350\276\223\345\207\272", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("FormOnLineClass", "\345\234\250\347\272\277\346\211\223\345\215\260\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
		checkBoxWhole->setText(QApplication::translate("FormOnLineClass", "\346\225\264\344\275\223\346\212\245\350\241\250\350\276\223\345\207\272", 0, QApplication::UnicodeUTF8));
		checkBoxTestOutPut->setText(QApplication::translate("FormOnLineClass", "\346\243\200\346\265\213\347\225\214\351\235\242\350\276\223\345\207\272", 0, QApplication::UnicodeUTF8));
		checkBoxQualified->setText(QApplication::translate("FormOnLineClass", "\345\220\210\346\240\274\345\223\201", 0, QApplication::UnicodeUTF8));
		checkBoxDefect->setText(QApplication::translate("FormOnLineClass", "\344\270\215\345\220\210\346\240\274\345\223\201", 0, QApplication::UnicodeUTF8));
		checkBoxBadCouple->setText(QApplication::translate("FormOnLineClass", "\350\200\246\345\220\210\344\270\215\350\211\257\345\223\201", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("FormOnLineClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("FormOnLineClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class FormOnLineClass: public Ui_FormOnLineClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMONLINE_H


/********************************************************************************
** Form generated from reading UI file 'formoffline.ui'
**
** Created: Wed Aug 14 14:16:33 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMOFFLINE_H
#define UI_FORMOFFLINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormOffLineClass
{
public:
	QLabel *label;
	QLineEdit *lineEditOrderNumber;
	QLabel *label_2;
	QWidget *widget_2;
	QCheckBox *checkBoxQualified;
	QCheckBox *checkBoxFeature;
	QCheckBox *checkBoxBadCouple;
	QCheckBox *checkBoxNumberBegin;
	QCheckBox *checkBoxNumberEnd;
	QLineEdit *lineEditNumberBegin;
	QLineEdit *lineEditNumberEnd;
	QPushButton *pushButtonSingle;
	QWidget *widget_3;
	QPushButton *pushButtonPainter;
	QPushButton *pushButtonWhole;
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QPushButton *pushButtonIn;

	void setupUi(QDialog *FormOffLineClass)
	{
		if (FormOffLineClass->objectName().isEmpty())
			FormOffLineClass->setObjectName(QString::fromUtf8("FormOffLineClass"));
		FormOffLineClass->resize(514, 377);
		FormOffLineClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		label = new QLabel(FormOffLineClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(180, 20, 151, 21));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 18pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditOrderNumber = new QLineEdit(FormOffLineClass);
		lineEditOrderNumber->setObjectName(QString::fromUtf8("lineEditOrderNumber"));
		lineEditOrderNumber->setGeometry(QRect(100, 60, 191, 25));
		lineEditOrderNumber->setStyleSheet(QString::fromUtf8("QLineEdit{color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"border:1px solid rgb(255,255,255);\n"
			"}\n"
			""));
		label_2 = new QLabel(FormOffLineClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(20, 60, 68, 25));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		widget_2 = new QWidget(FormOffLineClass);
		widget_2->setObjectName(QString::fromUtf8("widget_2"));
		widget_2->setGeometry(QRect(20, 100, 271, 221));
		widget_2->setStyleSheet(QString::fromUtf8("QWidget#widget_2{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		checkBoxQualified = new QCheckBox(widget_2);
		checkBoxQualified->setObjectName(QString::fromUtf8("checkBoxQualified"));
		checkBoxQualified->setGeometry(QRect(40, 60, 81, 16));
		checkBoxQualified->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		checkBoxFeature = new QCheckBox(widget_2);
		checkBoxFeature->setObjectName(QString::fromUtf8("checkBoxFeature"));
		checkBoxFeature->setGeometry(QRect(40, 90, 101, 16));
		checkBoxFeature->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		checkBoxBadCouple = new QCheckBox(widget_2);
		checkBoxBadCouple->setObjectName(QString::fromUtf8("checkBoxBadCouple"));
		checkBoxBadCouple->setGeometry(QRect(40, 120, 121, 16));
		checkBoxBadCouple->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		checkBoxNumberBegin = new QCheckBox(widget_2);
		checkBoxNumberBegin->setObjectName(QString::fromUtf8("checkBoxNumberBegin"));
		checkBoxNumberBegin->setGeometry(QRect(40, 150, 145, 16));
		checkBoxNumberBegin->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		checkBoxNumberEnd = new QCheckBox(widget_2);
		checkBoxNumberEnd->setObjectName(QString::fromUtf8("checkBoxNumberEnd"));
		checkBoxNumberEnd->setGeometry(QRect(40, 180, 145, 16));
		checkBoxNumberEnd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditNumberBegin = new QLineEdit(widget_2);
		lineEditNumberBegin->setObjectName(QString::fromUtf8("lineEditNumberBegin"));
		lineEditNumberBegin->setGeometry(QRect(190, 150, 71, 20));
		lineEditNumberBegin->setStyleSheet(QString::fromUtf8("QLineEdit{color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"border:1px solid rgb(255,255,255);\n"
			"}\n"
			""));
		lineEditNumberEnd = new QLineEdit(widget_2);
		lineEditNumberEnd->setObjectName(QString::fromUtf8("lineEditNumberEnd"));
		lineEditNumberEnd->setGeometry(QRect(190, 180, 71, 20));
		lineEditNumberEnd->setStyleSheet(QString::fromUtf8("QLineEdit{color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"border:1px solid rgb(255,255,255);\n"
			"}\n"
			""));
		pushButtonSingle = new QPushButton(widget_2);
		pushButtonSingle->setObjectName(QString::fromUtf8("pushButtonSingle"));
		pushButtonSingle->setGeometry(QRect(40, 20, 175, 30));
		pushButtonSingle->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 16pt \"\346\226\260\345\256\213\344\275\223\";"));
		widget_3 = new QWidget(FormOffLineClass);
		widget_3->setObjectName(QString::fromUtf8("widget_3"));
		widget_3->setGeometry(QRect(310, 100, 181, 221));
		widget_3->setStyleSheet(QString::fromUtf8("QWidget#widget_3{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		pushButtonPainter = new QPushButton(widget_3);
		pushButtonPainter->setObjectName(QString::fromUtf8("pushButtonPainter"));
		pushButtonPainter->setGeometry(QRect(3, 100, 175, 30));
		pushButtonPainter->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 16pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonWhole = new QPushButton(widget_3);
		pushButtonWhole->setObjectName(QString::fromUtf8("pushButtonWhole"));
		pushButtonWhole->setGeometry(QRect(5, 20, 175, 30));
		pushButtonWhole->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 16pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonYes = new QPushButton(FormOffLineClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(310, 340, 70, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonNo = new QPushButton(FormOffLineClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(420, 340, 70, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonIn = new QPushButton(FormOffLineClass);
		pushButtonIn->setObjectName(QString::fromUtf8("pushButtonIn"));
		pushButtonIn->setGeometry(QRect(310, 60, 60, 25));
		pushButtonIn->setMinimumSize(QSize(60, 25));
		pushButtonIn->setMaximumSize(QSize(60, 25));
		pushButtonIn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));

		retranslateUi(FormOffLineClass);

		QMetaObject::connectSlotsByName(FormOffLineClass);
	} // setupUi

	void retranslateUi(QDialog *FormOffLineClass)
	{
		FormOffLineClass->setWindowTitle(QApplication::translate("FormOffLineClass", "FormOffLine", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("FormOffLineClass", "\347\246\273\347\272\277\346\211\223\345\215\260\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("FormOffLineClass", "\350\256\242\345\215\225\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
		checkBoxQualified->setText(QApplication::translate("FormOffLineClass", "\345\220\210\346\240\274\345\223\201", 0, QApplication::UnicodeUTF8));
		checkBoxFeature->setText(QApplication::translate("FormOffLineClass", "\344\270\215\345\220\210\346\240\274\345\223\201", 0, QApplication::UnicodeUTF8));
		checkBoxBadCouple->setText(QApplication::translate("FormOffLineClass", "\350\200\246\345\220\210\344\270\215\350\211\257\345\223\201", 0, QApplication::UnicodeUTF8));
		checkBoxNumberBegin->setText(QApplication::translate("FormOffLineClass", "\351\222\242\347\256\241\347\274\226\345\217\267\350\265\267\347\202\271\357\274\232", 0, QApplication::UnicodeUTF8));
		checkBoxNumberEnd->setText(QApplication::translate("FormOffLineClass", "\351\222\242\347\256\241\347\274\226\345\217\267\347\273\210\347\202\271\357\274\232", 0, QApplication::UnicodeUTF8));
		pushButtonSingle->setText(QApplication::translate("FormOffLineClass", "\345\215\225\344\270\200\346\212\245\350\241\250\350\276\223\345\207\272", 0, QApplication::UnicodeUTF8));
		pushButtonPainter->setText(QApplication::translate("FormOffLineClass", "\347\225\214\351\235\242\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
		pushButtonWhole->setText(QApplication::translate("FormOffLineClass", "\346\225\264\344\275\223\346\212\245\350\241\250\350\276\223\345\207\272", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("FormOffLineClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("FormOffLineClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		pushButtonIn->setText(QApplication::translate("FormOffLineClass", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class FormOffLineClass: public Ui_FormOffLineClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMOFFLINE_H


/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created: Wed Oct 30 15:42:20 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginClass
{
public:
	QGridLayout *gridLayout_4;
	QWidget *widget;
	QGridLayout *gridLayout_3;
	QGridLayout *gridLayout_2;
	QSpacerItem *verticalSpacer;
	QGridLayout *gridLayout;
	QSpacerItem *horizontalSpacer_2;
	QHBoxLayout *horizontalLayout_3;
	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout;
	QLabel *label;
	QComboBox *comboBoxAdmin;
	QHBoxLayout *horizontalLayout_2;
	QLabel *label_2;
	QLineEdit *lineEditAdminPassWord;
	QSpacerItem *horizontalSpacer_3;
	QPushButton *pushButtonLogin;
	QSpacerItem *horizontalSpacer;
	QSpacerItem *verticalSpacer_2;

	void setupUi(QDialog *LoginClass)
	{
		if (LoginClass->objectName().isEmpty())
			LoginClass->setObjectName(QString::fromUtf8("LoginClass"));
		LoginClass->resize(952, 588);
		LoginClass->setMinimumSize(QSize(300, 200));
		LoginClass->setMaximumSize(QSize(16777215, 16777215));
		LoginClass->setStyleSheet(QString::fromUtf8("\n"
			"background-color: rgb(73, 73, 73);\n"
			"color: rgb(255, 255, 255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		gridLayout_4 = new QGridLayout(LoginClass);
		gridLayout_4->setSpacing(0);
		gridLayout_4->setContentsMargins(0, 0, 0, 0);
		gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
		widget = new QWidget(LoginClass);
		widget->setObjectName(QString::fromUtf8("widget"));
		widget->setStyleSheet(QString::fromUtf8("QWidget#widget{\n"
			"border-image: url(:/Login/Resources/DengLuBeiJing.png);\n"
			"}"));
		gridLayout_3 = new QGridLayout(widget);
		gridLayout_3->setSpacing(0);
		gridLayout_3->setContentsMargins(0, 0, 0, 0);
		gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
		gridLayout_2 = new QGridLayout();
		gridLayout_2->setSpacing(0);
		gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
		verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

		gridLayout_2->addItem(verticalSpacer, 0, 0, 1, 1);

		gridLayout = new QGridLayout();
		gridLayout->setSpacing(0);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

		gridLayout->addItem(horizontalSpacer_2, 0, 0, 1, 1);

		horizontalLayout_3 = new QHBoxLayout();
		horizontalLayout_3->setSpacing(6);
		horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
		verticalLayout = new QVBoxLayout();
		verticalLayout->setSpacing(6);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		horizontalLayout = new QHBoxLayout();
		horizontalLayout->setSpacing(6);
		horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
		label = new QLabel(widget);
		label->setObjectName(QString::fromUtf8("label"));
		label->setMinimumSize(QSize(70, 20));
		label->setMaximumSize(QSize(70, 20));
		label->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"background:transparent;\n"
			"color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));

		horizontalLayout->addWidget(label);

		comboBoxAdmin = new QComboBox(widget);
		comboBoxAdmin->setObjectName(QString::fromUtf8("comboBoxAdmin"));
		comboBoxAdmin->setMinimumSize(QSize(0, 20));
		comboBoxAdmin->setLayoutDirection(Qt::LeftToRight);
		comboBoxAdmin->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);\n"
			"color: rgb(0, 0, 0);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"background:transparent;"));
		comboBoxAdmin->setEditable(true);
		comboBoxAdmin->setInsertPolicy(QComboBox::InsertAtBottom);

		horizontalLayout->addWidget(comboBoxAdmin);


		verticalLayout->addLayout(horizontalLayout);

		horizontalLayout_2 = new QHBoxLayout();
		horizontalLayout_2->setSpacing(6);
		horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
		label_2 = new QLabel(widget);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setMinimumSize(QSize(70, 20));
		label_2->setMaximumSize(QSize(70, 20));
		label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"background:transparent;\n"
			"color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));

		horizontalLayout_2->addWidget(label_2);

		lineEditAdminPassWord = new QLineEdit(widget);
		lineEditAdminPassWord->setObjectName(QString::fromUtf8("lineEditAdminPassWord"));
		lineEditAdminPassWord->setMinimumSize(QSize(150, 20));
		lineEditAdminPassWord->setMaximumSize(QSize(150, 20));
		lineEditAdminPassWord->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"border:1px solid rgb(255,255,255);\n"
			"color: rgb(0, 0, 0);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"background:transparent;\n"
			"}\n"
			""));
		lineEditAdminPassWord->setFrame(true);
		lineEditAdminPassWord->setEchoMode(QLineEdit::Password);
		lineEditAdminPassWord->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

		horizontalLayout_2->addWidget(lineEditAdminPassWord);


		verticalLayout->addLayout(horizontalLayout_2);


		horizontalLayout_3->addLayout(verticalLayout);

		horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

		horizontalLayout_3->addItem(horizontalSpacer_3);

		pushButtonLogin = new QPushButton(widget);
		pushButtonLogin->setObjectName(QString::fromUtf8("pushButtonLogin"));
		pushButtonLogin->setMinimumSize(QSize(60, 60));
		pushButtonLogin->setMaximumSize(QSize(60, 60));
		pushButtonLogin->setStyleSheet(QString::fromUtf8("QPushButton{\n"
			"image: url(:/Login/Resources/DengLuAnNiu.png);\n"
			"background:transparent;\n"
			"}\n"
			"QPushButton::hover{\n"
			"border-image: url(:/Login/Resources/DengLuAnNiu.png);\n"
			"}\n"
			""));

		horizontalLayout_3->addWidget(pushButtonLogin);


		gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

		horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

		gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);


		gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

		verticalSpacer_2 = new QSpacerItem(20, 160, QSizePolicy::Minimum, QSizePolicy::Fixed);

		gridLayout_2->addItem(verticalSpacer_2, 2, 0, 1, 1);


		gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);


		gridLayout_4->addWidget(widget, 0, 0, 1, 1);


		retranslateUi(LoginClass);

		comboBoxAdmin->setCurrentIndex(-1);


		QMetaObject::connectSlotsByName(LoginClass);
	} // setupUi

	void retranslateUi(QDialog *LoginClass)
	{
		LoginClass->setWindowTitle(QApplication::translate("LoginClass", "Login", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("LoginClass", "\347\224\250\346\210\267\345\220\215:", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("LoginClass", "\345\257\206  \347\240\201:", 0, QApplication::UnicodeUTF8));
		lineEditAdminPassWord->setInputMask(QString());
		lineEditAdminPassWord->setText(QString());
		pushButtonLogin->setText(QString());
	} // retranslateUi

};

namespace Ui {
	class LoginClass: public Ui_LoginClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H




/********************************************************************************
** Form generated from reading UI file 'usermanagement.ui'
**
** Created: Thu Oct 31 15:53:01 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMANAGEMENT_H
#define UI_USERMANAGEMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserManagementClass
{
public:
	QLabel *label;
	QLabel *label_2;
	QWidget *widget;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QLineEdit *lineEditNewUser;
	QLineEdit *lineEditNewUserPassword;
	QLineEdit *lineEditNewUserPasswordAgain;
	QPushButton *pushButtonAdd;
	QWidget *widget_2;
	QLabel *label_7;
	QComboBox *comboBoxUser;
	QPushButton *pushButtonCut;
	QLineEdit *lineEditAdmin;
	QPushButton *pushButtonTest;

	void setupUi(QDialog *UserManagementClass)
	{
		if (UserManagementClass->objectName().isEmpty())
			UserManagementClass->setObjectName(QString::fromUtf8("UserManagementClass"));
		UserManagementClass->resize(669, 371);
		UserManagementClass->setMinimumSize(QSize(669, 371));
		UserManagementClass->setMaximumSize(QSize(669, 371));
		UserManagementClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);\n"
			"color: rgb(255, 255, 255);\n"
			"font: 10pt \"\346\226\260\345\256\213\344\275\223\";"));
		label = new QLabel(UserManagementClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(250, 20, 171, 21));
		label->setMinimumSize(QSize(171, 21));
		label->setMaximumSize(QSize(171, 21));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 17pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_2 = new QLabel(UserManagementClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(20, 70, 111, 25));
		label_2->setMinimumSize(QSize(111, 25));
		label_2->setMaximumSize(QSize(111, 25));
		label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		widget = new QWidget(UserManagementClass);
		widget->setObjectName(QString::fromUtf8("widget"));
		widget->setGeometry(QRect(20, 120, 300, 231));
		widget->setMinimumSize(QSize(300, 231));
		widget->setMaximumSize(QSize(300, 231));
		widget->setStyleSheet(QString::fromUtf8("QWidget#widget{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		label_3 = new QLabel(widget);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(40, 30, 71, 25));
		label_3->setMinimumSize(QSize(71, 25));
		label_3->setMaximumSize(QSize(71, 25));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_4 = new QLabel(widget);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(60, 80, 51, 25));
		label_4->setMinimumSize(QSize(51, 25));
		label_4->setMaximumSize(QSize(51, 25));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_5 = new QLabel(widget);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(20, 130, 91, 25));
		label_5->setMinimumSize(QSize(91, 25));
		label_5->setMaximumSize(QSize(91, 25));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditNewUser = new QLineEdit(widget);
		lineEditNewUser->setObjectName(QString::fromUtf8("lineEditNewUser"));
		lineEditNewUser->setGeometry(QRect(130, 30, 151, 25));
		lineEditNewUser->setMinimumSize(QSize(151, 25));
		lineEditNewUser->setMaximumSize(QSize(151, 25));
		lineEditNewUser->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditNewUserPassword = new QLineEdit(widget);
		lineEditNewUserPassword->setObjectName(QString::fromUtf8("lineEditNewUserPassword"));
		lineEditNewUserPassword->setGeometry(QRect(130, 80, 151, 25));
		lineEditNewUserPassword->setMinimumSize(QSize(151, 25));
		lineEditNewUserPassword->setMaximumSize(QSize(151, 25));
		lineEditNewUserPassword->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditNewUserPassword->setEchoMode(QLineEdit::PasswordEchoOnEdit);
		lineEditNewUserPasswordAgain = new QLineEdit(widget);
		lineEditNewUserPasswordAgain->setObjectName(QString::fromUtf8("lineEditNewUserPasswordAgain"));
		lineEditNewUserPasswordAgain->setGeometry(QRect(130, 130, 151, 25));
		lineEditNewUserPasswordAgain->setMinimumSize(QSize(151, 25));
		lineEditNewUserPasswordAgain->setMaximumSize(QSize(151, 25));
		lineEditNewUserPasswordAgain->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditNewUserPasswordAgain->setEchoMode(QLineEdit::PasswordEchoOnEdit);
		pushButtonAdd = new QPushButton(widget);
		pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
		pushButtonAdd->setGeometry(QRect(180, 180, 101, 25));
		pushButtonAdd->setMinimumSize(QSize(101, 25));
		pushButtonAdd->setMaximumSize(QSize(101, 25));
		pushButtonAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		widget_2 = new QWidget(UserManagementClass);
		widget_2->setObjectName(QString::fromUtf8("widget_2"));
		widget_2->setGeometry(QRect(350, 120, 300, 231));
		widget_2->setMinimumSize(QSize(300, 231));
		widget_2->setMaximumSize(QSize(300, 231));
		widget_2->setStyleSheet(QString::fromUtf8("QWidget#widget_2{\n"
			"border:1px solid rgb(255,255,255);\n"
			"}"));
		label_7 = new QLabel(widget_2);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(40, 30, 71, 25));
		label_7->setMinimumSize(QSize(71, 25));
		label_7->setMaximumSize(QSize(71, 25));
		label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		comboBoxUser = new QComboBox(widget_2);
		comboBoxUser->setObjectName(QString::fromUtf8("comboBoxUser"));
		comboBoxUser->setGeometry(QRect(130, 30, 151, 25));
		comboBoxUser->setMinimumSize(QSize(151, 25));
		comboBoxUser->setMaximumSize(QSize(151, 25));
		comboBoxUser->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		pushButtonCut = new QPushButton(widget_2);
		pushButtonCut->setObjectName(QString::fromUtf8("pushButtonCut"));
		pushButtonCut->setGeometry(QRect(180, 180, 101, 25));
		pushButtonCut->setMinimumSize(QSize(101, 25));
		pushButtonCut->setMaximumSize(QSize(101, 25));
		pushButtonCut->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditAdmin = new QLineEdit(UserManagementClass);
		lineEditAdmin->setObjectName(QString::fromUtf8("lineEditAdmin"));
		lineEditAdmin->setGeometry(QRect(150, 70, 151, 25));
		lineEditAdmin->setMinimumSize(QSize(151, 25));
		lineEditAdmin->setMaximumSize(QSize(151, 25));
		lineEditAdmin->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditAdmin->setFrame(true);
		lineEditAdmin->setEchoMode(QLineEdit::Password);
		pushButtonTest = new QPushButton(UserManagementClass);
		pushButtonTest->setObjectName(QString::fromUtf8("pushButtonTest"));
		pushButtonTest->setGeometry(QRect(350, 70, 101, 25));
		pushButtonTest->setMinimumSize(QSize(101, 25));
		pushButtonTest->setMaximumSize(QSize(101, 25));
		pushButtonTest->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 14pt \"\346\226\260\345\256\213\344\275\223\";"));

		retranslateUi(UserManagementClass);

		QMetaObject::connectSlotsByName(UserManagementClass);
	} // setupUi

	void retranslateUi(QDialog *UserManagementClass)
	{
		UserManagementClass->setWindowTitle(QApplication::translate("UserManagementClass", "UserManagement", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("UserManagementClass", "\347\224\250  \346\210\267  \347\256\241  \347\220\206", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("UserManagementClass", "\347\256\241\347\220\206\345\221\230\345\257\206\347\240\201:", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("UserManagementClass", "\347\224\250\346\210\267\345\220\215:", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("UserManagementClass", "\345\257\206\347\240\201:", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("UserManagementClass", "\347\241\256\350\256\244\345\257\206\347\240\201:", 0, QApplication::UnicodeUTF8));
		pushButtonAdd->setText(QApplication::translate("UserManagementClass", "\346\267\273\345\212\240\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
		label_7->setText(QApplication::translate("UserManagementClass", "\347\224\250\346\210\267\345\220\215:", 0, QApplication::UnicodeUTF8));
		pushButtonCut->setText(QApplication::translate("UserManagementClass", "\345\210\240\351\231\244\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
		pushButtonTest->setText(QApplication::translate("UserManagementClass", "\351\252\214\350\257\201\346\235\203\351\231\220", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class UserManagementClass: public Ui_UserManagementClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMANAGEMENT_H


/********************************************************************************
** Form generated from reading UI file 'deadzone.ui'
**
** Created: Sat Dec 14 16:32:58 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEADZONE_H
#define UI_DEADZONE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DeadZoneClass
{
public:
	QPushButton *pushButtonYes;
	QPushButton *pushButtonNo;
	QLabel *label;
	QLabel *label_2;
	QLabel *label_3;
	QLineEdit *lineEditStart;
	QLineEdit *lineEditEnd;
	QLabel *label_4;
	QLabel *label_5;

	void setupUi(QDialog *DeadZoneClass)
	{
		if (DeadZoneClass->objectName().isEmpty())
			DeadZoneClass->setObjectName(QString::fromUtf8("DeadZoneClass"));
		DeadZoneClass->resize(540, 300);
		DeadZoneClass->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
		pushButtonYes = new QPushButton(DeadZoneClass);
		pushButtonYes->setObjectName(QString::fromUtf8("pushButtonYes"));
		pushButtonYes->setGeometry(QRect(330, 260, 60, 25));
		pushButtonYes->setMinimumSize(QSize(60, 25));
		pushButtonYes->setMaximumSize(QSize(60, 25));
		pushButtonYes->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		pushButtonNo = new QPushButton(DeadZoneClass);
		pushButtonNo->setObjectName(QString::fromUtf8("pushButtonNo"));
		pushButtonNo->setGeometry(QRect(440, 260, 60, 25));
		pushButtonNo->setMinimumSize(QSize(60, 25));
		pushButtonNo->setMaximumSize(QSize(60, 25));
		pushButtonNo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
		label = new QLabel(DeadZoneClass);
		label->setObjectName(QString::fromUtf8("label"));
		label->setGeometry(QRect(100, 110, 91, 21));
		label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_2 = new QLabel(DeadZoneClass);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(220, 30, 95, 25));
		label_2->setMinimumSize(QSize(95, 25));
		label_2->setMaximumSize(QSize(95, 25));
		label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
			"color: rgb(255, 255, 255);\n"
			"font: 75 18pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"}"));
		label_3 = new QLabel(DeadZoneClass);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		label_3->setGeometry(QRect(100, 170, 91, 21));
		label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		lineEditStart = new QLineEdit(DeadZoneClass);
		lineEditStart->setObjectName(QString::fromUtf8("lineEditStart"));
		lineEditStart->setGeometry(QRect(230, 110, 121, 28));
		lineEditStart->setMinimumSize(QSize(60, 28));
		lineEditStart->setMaximumSize(QSize(16777215, 28));
		lineEditStart->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"border:1px solid rgb(255,255,255);\n"
			"color: rgb(255,255,255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"background:transparent;\n"
			"}"));
		lineEditEnd = new QLineEdit(DeadZoneClass);
		lineEditEnd->setObjectName(QString::fromUtf8("lineEditEnd"));
		lineEditEnd->setGeometry(QRect(230, 170, 121, 28));
		lineEditEnd->setMinimumSize(QSize(60, 28));
		lineEditEnd->setMaximumSize(QSize(16777215, 28));
		lineEditEnd->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
			"border:1px solid rgb(255,255,255);\n"
			"color: rgb(255,255,255);\n"
			"font: 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
			"background:transparent;\n"
			"}"));
		label_4 = new QLabel(DeadZoneClass);
		label_4->setObjectName(QString::fromUtf8("label_4"));
		label_4->setGeometry(QRect(360, 110, 21, 21));
		label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));
		label_5 = new QLabel(DeadZoneClass);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		label_5->setGeometry(QRect(360, 180, 21, 21));
		label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
			"font: 15pt \"\346\226\260\345\256\213\344\275\223\";"));

		retranslateUi(DeadZoneClass);

		QMetaObject::connectSlotsByName(DeadZoneClass);
	} // setupUi

	void retranslateUi(QDialog *DeadZoneClass)
	{
		DeadZoneClass->setWindowTitle(QApplication::translate("DeadZoneClass", "DeadZone", 0, QApplication::UnicodeUTF8));
		pushButtonYes->setText(QApplication::translate("DeadZoneClass", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
		pushButtonNo->setText(QApplication::translate("DeadZoneClass", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("DeadZoneClass", "\347\233\262\345\214\272\345\247\213\347\253\257\357\274\232", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("DeadZoneClass", "\347\233\262\345\214\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("DeadZoneClass", "\347\233\262\345\214\272\346\234\253\347\253\257\357\274\232", 0, QApplication::UnicodeUTF8));
		lineEditStart->setText(QString());
		label_4->setText(QApplication::translate("DeadZoneClass", "cm", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("DeadZoneClass", "cm", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui {
	class DeadZoneClass: public Ui_DeadZoneClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEADZONE_H
