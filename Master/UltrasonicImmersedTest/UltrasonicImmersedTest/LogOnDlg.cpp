// LogOnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LogOnDlg.h"
#include "UltrasonicImmersedTest.h"



// CLogOnDlg 对话框

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


// CLogOnDlg 消息处理程序

void CLogOnDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
/*
	UpdateData(TRUE);
	
	// 连接数据库
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
		// 打开本地Access库Demo.mdb
		hr = pConnection->Open(strCnn,"","",adConnectUnspecified);
	}
	catch(_com_error e)
	{
		CString strError;
		strError.Format(_T("数据库连接失败:%s"), e.ErrorMessage());
		AfxMessageBox(strError, MB_ICONSTOP);
		
		return;
	}

	CString strSQL;
	strSQL.Format(_T("select * from ars_uc_user where un = '%s'"), m_strUserName);

	// 打开记录
	_RecordsetPtr	m_pRecordset = NULL;
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	_bstr_t bstrSQL(strSQL);

	try
	{
		HRESULT hr = m_pRecordset->Open(bstrSQL,                // 查询DemoTable表中所有字段
			_variant_t((IDispatch *)pConnection,true), //m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);

		_variant_t var;

		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			AfxMessageBox(_T("表内数据为空"));
			return;
		}

		// 读入库中各字段并加入列表框中
		BOOL bExist = FALSE;
		while(!m_pRecordset->adoEOF)
		{
 			var = m_pRecordset->GetCollect(_T("pwd"));
 			if(var.vt != VT_NULL)
			{
				CString strPwd =  (LPCWSTR)var.bstrVal;
				strPwd.TrimRight();

				// 密码加密转换
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
			MessageBox(_T("系统无此用户，请重新输入！"), _T("提示"), MB_OK);

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

	// TODO:  在此添加额外的初始化
	CWnd* pWnd = GetDlgItem(IDC_EDIT_USERNAME);
	pWnd->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
