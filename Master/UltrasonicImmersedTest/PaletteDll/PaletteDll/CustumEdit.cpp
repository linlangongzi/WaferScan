// CustumEdit.cpp : ʵ���ļ�
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



// CCustumEdit ��Ϣ�������



void CCustumEdit::OnEnSetfocus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStaticDlg* pParent = (CStaticDlg*)GetParent();
	pParent->m_hFocusEdit = this->GetSafeHwnd();
//	pParent->ChangeEditValue();
	pParent->OnEnableButton();
	this->SetFocus();
	this->SetSel(0, -1);
}
