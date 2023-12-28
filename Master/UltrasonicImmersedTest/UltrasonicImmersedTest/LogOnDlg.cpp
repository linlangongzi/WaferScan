// LogOnDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LogOnDlg.h"
#include "UltrasonicImmersedTest.h"



// CLogOnDlg �Ի���

IMPLEMENT_DYNAMIC(CLogOnDlg, CDialog)

CLogOnDlg::CLogOnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogOnDlg::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPassWord(_T(""))
{

}

CLogOnDlg::~CLogOnDlg()
{
}

void CLogOnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassWord);
}


BEGIN_MESSAGE_MAP(CLogOnDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLogOnDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogOnDlg ��Ϣ�������

void CLogOnDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
/*
	UpdateData(TRUE);
	
	// �������ݿ�
	HRESULT hr;
	_ConnectionPtr	pConnection = NULL;
	hr = pConnection.CreateInstance(__uuidof(Connection));
	
	if (FAILED(hr))
	{
		return;
	}

	_bstr_t strCnn(g_strDBConnection);
	try                 
	{	
		// �򿪱���Access��Demo.mdb
		hr = pConnection->Open(strCnn,"","",adConnectUnspecified);
	}
	catch(_com_error e)
	{
		CString strError;
		strError.Format(_T("���ݿ�����ʧ��:%s"), e.ErrorMessage());
		AfxMessageBox(strError, MB_ICONSTOP);
		
		return;
	}

	CString strSQL;
	strSQL.Format(_T("select * from ars_uc_user where un = '%s'"), m_strUserName);

	// �򿪼�¼
	_RecordsetPtr	m_pRecordset = NULL;
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	_bstr_t bstrSQL(strSQL);

	try
	{
		HRESULT hr = m_pRecordset->Open(bstrSQL,                // ��ѯDemoTable���������ֶ�
			_variant_t((IDispatch *)pConnection,true), //m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);

		_variant_t var;

		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			AfxMessageBox(_T("��������Ϊ��"));
			return;
		}

		// ������и��ֶβ������б����
		BOOL bExist = FALSE;
		while(!m_pRecordset->adoEOF)
		{
 			var = m_pRecordset->GetCollect(_T("pwd"));
 			if(var.vt != VT_NULL)
			{
				CString strPwd =  (LPCWSTR)var.bstrVal;
				strPwd.TrimRight();

				// �������ת��
				CString strEncode = PwdEncode(m_strPassWord);

//				if (strEncode.CompareNoCase(strPwd))
				if(1)
				{
					bExist = TRUE;
					break;
				}
			}

			m_pRecordset->MoveNext();
		}

		if (m_pRecordset)
			if (m_pRecordset->State == adStateOpen)
				m_pRecordset->Close();

		if (!bExist)
		{
			MessageBox(_T("ϵͳ�޴��û������������룡"), _T("��ʾ"), MB_OK);

			return;
		}
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
*/
	OnOK();
}

CString CLogOnDlg::PwdEncode(CString& strPwd)
{
	CString strEncode;

	CLSID classId ;

	TCHAR strClas[10] = _T("ars.vb");

	HRESULT hr = CLSIDFromString(strClas, &classId);

	if (FAILED(hr))
	{
		return _T("");
	}

	IUnknown *p = NULL;
	HRESULT hr2 = CoCreateInstance(classId, NULL, CLSCTX_INPROC_SERVER, IID_AsyncIUnknown, (LPVOID*)&p);

	if (FAILED(hr2))
	{
		return _T("");
	}


	return strEncode;
}
BOOL CLogOnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd* pWnd = GetDlgItem(IDC_EDIT_USERNAME);
	pWnd->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
