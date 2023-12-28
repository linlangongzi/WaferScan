#include <windows.h>
#include "const.h"
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI¿¨²Ù×÷¶¯Ì¬Á´½Ó¿â
#include "..\pci9054_dll\p9054_lib.h"
#include <QtGui>
#include "DataDefine.h"
#include <QEvent>
#include "EventDefine.h"

ASCANEvent::ASCANEvent()
: QEvent( Type(A_SCAN_EVENT) )
{
	
}

FEATUREINFOEvent::FEATUREINFOEvent()
: QEvent( Type(FEATURE_INFO_EVENT) )
{

}
