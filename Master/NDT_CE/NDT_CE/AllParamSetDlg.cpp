
#include "StdAfx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "AllParamSetDlg.h"


const dlgMessageProc AllParamSetDlgMessages[] = {
	WM_CREATE, OnCreateAllParamSetDlg,
	WM_INITDIALOG, OnInitialAllParamSetDlg,
	WM_COMMAND, goCommandAllParamSetDlg,
	WM_NOTIFY, goNotifyAllParamSetDlg,
	WM_HSCROLL, goHScrollAllParamSetDlg,
};





BOOL CALLBACK AllParamSetDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i< dim(AllParamSetDlgMessages); i++)
	{
		if(msgCode == AllParamSetDlgMessages[i].uCode)
			return (*AllParamSetDlgMessages[i].functionName)(hWnd, msgCode, wParam, lParam);
	}

	return FALSE;
}

BOOL OnCreateAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL OnInitialAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL goCommandAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{	
	//单击“ok”按钮，结束对话框
	if (LOWORD(wParam) == IDOK)
	{
		EndDialog(hWnd,LOWORD(wParam));
		return TRUE;
	}
	//单击“退出”按钮，结束进程
	if(LOWORD(wParam) == IDCANCEL)
	{
		ExitProcess(0);
	}
	

	return FALSE;
}

BOOL goNotifyAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL goHScrollAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}