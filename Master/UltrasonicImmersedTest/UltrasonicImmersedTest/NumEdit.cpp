#include "StdAfx.h"
#include "NumEdit.h"

CNumEdit::CNumEdit(void)
{
}

CNumEdit::~CNumEdit(void)
{
}
BEGIN_MESSAGE_MAP(CNumEdit,CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CNumEdit::OnChar(UINT nChar , UINT nRepCnt , UINT nFlags)
{
	if (((nChar >= _T('0'))&&(nChar <= _T('9')))  || (nChar == _T('.')))
	{
		CString strEdit;
		GetWindowText(strEdit);
		int length=strEdit.GetLength();
		int pos=strEdit.Find('.');
		if ( pos!=-1)
		{
			if((length - pos)<=2)
				CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
			CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

