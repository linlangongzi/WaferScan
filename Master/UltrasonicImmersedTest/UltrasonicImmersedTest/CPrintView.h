// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

//#import "C:\\Program Files\\奥瑞视（北京）科技有限公司\\三维水浸检测系统\\mdsocx\\ActiveXExtension.dll" no_namespace
// CPrintView 包装类

class CPrintView : public COleDispatchDriver
{
public:
	CPrintView(){} // 调用 COleDispatchDriver 默认构造函数
	CPrintView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPrintView(const CPrintView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// _PrintView 方法
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

	// _PrintView 属性
public:

};
