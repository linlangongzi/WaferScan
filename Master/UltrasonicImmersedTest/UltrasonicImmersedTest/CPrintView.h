// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

//#import "C:\\Program Files\\�����ӣ��������Ƽ����޹�˾\\��άˮ�����ϵͳ\\mdsocx\\ActiveXExtension.dll" no_namespace
// CPrintView ��װ��

class CPrintView : public COleDispatchDriver
{
public:
	CPrintView(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CPrintView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPrintView(const CPrintView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// _PrintView ����
public:
	void ExLoad()
	{
		InvokeHelper(0x60030010, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ExShow()
	{
		InvokeHelper(0x60030011, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ExSetTxt(LPCTSTR p_rg, LPCTSTR p_val)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_rg, p_val);
	}
	void ExSetPic(LPCTSTR p_rg, LPCTSTR p_path)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, p_rg, p_path);
	}

	// _PrintView ����
public:

};
