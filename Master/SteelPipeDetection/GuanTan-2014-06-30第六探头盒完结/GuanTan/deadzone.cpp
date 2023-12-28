#include "deadzone.h"
#include <QValidator>

DeadZone::DeadZone(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("Ã¤ÇøÉèÖÃ"));
	ui.lineEditStart->setValidator(new QIntValidator(0,880,this));
	ui.lineEditEnd->setValidator(new QIntValidator(0,880,this));
}

DeadZone::~DeadZone()
{

}
