// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

//#import "C:\\Program Files\\�����ӣ��������Ƽ����޹�˾\\��άˮ�����ϵͳ\\mdsocx\\ActiveXExtension.dll" no_namespace
// CProbeSet ��װ��

class CProbeSet : public COleDispatchDriver
{
public:
	CProbeSet(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CProbeSet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CProbeSet(const CProbeSet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// _ProbeSet ����
public:
	LPDISPATCH get_Doc()
	{
		LPDISPATCH result;
		InvokeHelper(0x40030000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_Doc(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x40030000, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	void show()
	{
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString getVal(LPCTSTR key)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, key);
		return result;
	}
	VARIANT GetMaterialID(BSTR * S)
	{
		VARIANT result;
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, S);
		return result;
	}
	VARIANT SetDir(LPCTSTR S)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, S);
		return result;
	}

	// _ProbeSet ����
public:

};
