#ifndef __FNQUANTITATIVEANALYSISWIN_H__
#define __FNQUANTITATIVEANALYSISWIN_H__


ATOM registerQuantitativeAnalysisChild(HINSTANCE hInstance);
LRESULT CALLBACK quantitativeAnalysisChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int quantitativeAnalysis_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnQuantitativeAnalysisDgsCurveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnQuantitativeAnalysisAutoGainPlusProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnQuantitativeAnalysisAdjustProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnQuantitativeAnalysisPeakRememberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNQUANTITATIVEANALYSISWIN_H__