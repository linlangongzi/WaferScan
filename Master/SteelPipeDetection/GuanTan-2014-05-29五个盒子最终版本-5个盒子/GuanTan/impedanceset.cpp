#include "impedanceset.h"

ImpedanceSet::ImpedanceSet(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("�迹��ʾ"));
}

ImpedanceSet::~ImpedanceSet()
{

}
