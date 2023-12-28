// CustumEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "CustumEdit.h"
#include "StaticDlg.h"


// CCustumEdit

IMPLEMENT_DYNAMIC(CCustumEdit, CEdit)

CCustumEdit::CCustumEdit()
{

}

CCustumEdit::~CCustumEdit()
{
}


BEGIN_MESSAGE_MAP(CCustumEdit, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CCustumEdit::OnEnSetfocus)
END_MESSAGE_MAP()



// CCustumEdit 消息处理程序



void CCustumEdit::OnEnSetfocus()
{
	// TODO: 在此添加控件通知处理程序代码
	CStaticDlg* pParent = (CStaticDlg*)GetParent();
	pParent->m_hFocusEdit = this->GetSafeHwnd();
//	pParent->ChangeEditValue();
	pParent->OnEnableButton();
	this->SetFocus();
	this->SetSel(0, -1);
}
