// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

//#import "C:\\Program Files\\奥瑞视（北京）科技有限公司\\三维水浸检测系统\\mdsocx\\ActiveXExtension.dll" no_namespace
// CProbeSet 包装类

class CProbeSet : public COleDispatchDriver
{
public:
	CProbeSet(){} // 调用 COleDispatchDriver 默认构造函数
	CProbeSet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CProbeSet(const CProbeSet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// _ProbeSet 方法
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

	// _ProbeSet 属性
public:

};
