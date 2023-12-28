/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ͼ�����㷨ͷ�ļ�
*Filename : DibImage.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-08-08 
*/
#pragma once
// �ж��Ƿ���Win 3.0��DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
DECLARE_HANDLE(HDIB);		// DIB���
#define PALVERSION   0x300	// DIB����
// ������
#define PI 3.1415926535
//�Ƕȵ�����ת���ĺ�
#define RADIAN(angle) ((angle)*PI/180.0) 

class CDibImage
{
public:
	CDibImage(void);
	~CDibImage(void);

public:
	LPSTR FindDIBBits (LPSTR lpbi);
	WORD PaletteSize(LPSTR lpbi);
	WORD DIBNumColors(LPSTR lpbi);
	BOOL Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
		int iTempH, int iTempW, int iTempMX, int iTempMY,FLOAT * fpArray, FLOAT fCoef);
	DWORD DIBHeight(LPSTR lpDIB);
	DWORD DIBWidth(LPSTR lpDIB);
	BOOL MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
		int iFilterH, int iFilterW, int iFilterMX, int iFilterMY);
	unsigned char GetMedianNum(unsigned char * bArray, int iFilterLen);
	BOOL CreateDIBPalette(HANDLE hDIB, CPalette* pPal);
	HGLOBAL RotateDIB(LPSTR lpDIB, int iRotateAngle);
	HGLOBAL ZoomDIB(LPSTR lpDIB, float fXZoomRatio, float fYZoomRatio);
	
	void RotateImage(CDC* pDC,double angle,CRect& rect);

};
