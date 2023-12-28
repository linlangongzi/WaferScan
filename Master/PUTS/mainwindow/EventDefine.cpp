#include <windows.h>
#include "const.h"
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI卡操作动态链接库
#include "..\pci9054_dll\p9054_lib.h"
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
