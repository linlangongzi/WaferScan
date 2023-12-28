#include "formitem.h"

FormItem::FormItem(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

FormItem::~FormItem()
{

}
