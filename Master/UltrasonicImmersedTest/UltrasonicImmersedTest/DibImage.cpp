/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ͼ�����㷨Դ�ļ�
*Filename : DibImage.cpp
*
*@author       zhaoli
*@version      1.0         Date: 2011-08-08 
*/
#include "StdAfx.h"
#include "DibImage.h"
#include "math.h"

CDibImage::CDibImage(void)
{
}

CDibImage::~CDibImage(void)
{
}
LPSTR  CDibImage::FindDIBBits (LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
}
/*************************************************************************
* �������ƣ�
*   PaletteSize()
* ����:
*   LPSTR lpbi         - ָ��DIB�����ָ��
* ����ֵ:
*   WORD               - DIB�е�ɫ��Ĵ�С
* ˵��:
*   �ú�������DIB�е�ɫ��Ĵ�С������Windows 3.0 DIB��������ɫ��Ŀ��
* RGBQUAD�Ĵ�С����������������ɫ��Ŀ��RGBTRIPLE�Ĵ�С��
************************************************************************/
WORD CDibImage::PaletteSize(LPSTR lpbi)
{
	// ����DIB�е�ɫ��Ĵ�С
	if (IS_WIN30_DIB (lpbi))
	{
		//������ɫ��Ŀ��RGBQUAD�Ĵ�С
		return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}
	else
	{
		//������ɫ��Ŀ��RGBTRIPLE�Ĵ�С
		return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
	}
}
/*************************************************************************
* �������ƣ�
*   DIBNumColors()
* ����:
*   LPSTR lpbi         - ָ��DIB�����ָ��
* ����ֵ:
*   WORD               - ���ص�ɫ������ɫ������
* ˵��:
*   �ú�������DIB�е�ɫ�����ɫ�����������ڵ�ɫλͼ������2��
* ����16ɫλͼ������16������256ɫλͼ������256���������ɫ
* λͼ��24λ����û�е�ɫ�壬����0��
************************************************************************/
WORD CDibImage::DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;

	// ����Windows��DIB, ʵ����ɫ����Ŀ���Ա����ص�λ��Ҫ�١�
	// ��������������򷵻�һ�����Ƶ���ֵ��

	// �ж��Ƿ���WIN3.0 DIB
	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;
		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;	// ��ȡdwClrUsedֵ

		if (dwClrUsed != 0)
		{
			// ���dwClrUsed��ʵ���õ�����ɫ������Ϊ0��ֱ�ӷ��ظ�ֵ
			return (WORD)dwClrUsed;
		}
	}

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{		
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;	// ��ȡbiBitCountֵ
	}
	else
	{		
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;	// ��ȡbiBitCountֵ
	}

	// �������ص�λ��������ɫ��Ŀ
	switch (wBitCount)
	{
	case 1:
		return 2;
		break;
	case 4:
		return 16;
		break;
	case 8:
		return 256;
		break;
	default:
		return 0;
		break;
	}
}

/*************************************************************************
* �������ƣ�
*   Template()
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*   int   iTempH		- ģ��ĸ߶�
*   int   iTempW		- ģ��Ŀ��
*   int   iTempMX		- ģ�������Ԫ��X���� ( < iTempW - 1)
*   int   iTempMY		- ģ�������Ԫ��Y���� ( < iTempH - 1)
*	 FLOAT * fpArray	- ָ��ģ�������ָ��
*	 FLOAT fCoef		- ģ��ϵ��
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�����ָ����ģ�壨�����С������ͼ����в���������iTempHָ��ģ��
* �ĸ߶ȣ�����iTempWָ��ģ��Ŀ�ȣ�����iTempMX��iTempMYָ��ģ�������
* Ԫ�����꣬����fpArrayָ��ģ��Ԫ�أ�fCoefָ��ϵ����
************************************************************************/
BOOL CDibImage::Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int iTempH, int iTempW, int iTempMX, int iTempMY,
						 FLOAT * fpArray, FLOAT fCoef)
{	
	LPSTR	lpNewDIBBits;		// ָ����ͼ���ָ��
	HLOCAL	hNewDIBBits;	
	unsigned char*	lpSrc;		// ָ��Դͼ���ָ��	
	unsigned char*	lpDst;		// ָ��Ҫ���������ָ��	
	LONG	i,j,k,l;			// ѭ������	
	FLOAT	fResult;			// ������

	LONG lLineBytes;			// ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}

	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	for(i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)	// ��(��ȥ��Ե����)
	{		
		for(j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)// ��(��ȥ��Ե����)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 
						- i + iTempMY - k) + j - iTempMX + l;

					// ��������ֵ
					fResult += (* lpSrc) * fpArray[k * iTempW + l];
				}
			}

			fResult *= fCoef;					// ����ϵ��			
			fResult = (FLOAT ) fabs(fResult);	// ȡ����ֵ
			if(fResult > 255)
			{
				* lpDst = 255;
			}
			else
			{
				* lpDst = (unsigned char) (fResult + 0.5);
			}			
		}
	}

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}
/*************************************************************************
* �������ƣ�
*   DIBHeight()
* ����:
*   LPSTR lpDIB        - ָ��DIB�����ָ��
* ����ֵ:
*   DWORD              - DIB��ͼ��ĸ߶�
* ˵��:
*   �ú�������DIB��ͼ��ĸ߶ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
* �е�biHeightֵ��������������BITMAPCOREHEADER�е�bcHeightֵ��
************************************************************************/
DWORD CDibImage::DIBHeight(LPSTR lpDIB)
{	
	LPBITMAPINFOHEADER lpbmi;	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPCOREHEADER lpbmc;	// ָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ��
	if (IS_WIN30_DIB(lpDIB))
	{		
		return lpbmi->biHeight;		// ����Windows 3.0 DIB������lpbmi->biHeight
	}
	else
	{		
		return (DWORD)lpbmc->bcHeight;	// ����������ʽ��DIB������lpbmc->bcHeight
	}
}
/*************************************************************************
* �������ƣ�
*   DIBWidth()
* ����:
*   LPSTR lpbi         - ָ��DIB�����ָ��
* ����ֵ:
*   DWORD              - DIB��ͼ��Ŀ��
* ˵��:
*   �ú�������DIB��ͼ��Ŀ�ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
* �е�biWidthֵ��������������BITMAPCOREHEADER�е�bcWidthֵ��
************************************************************************/
DWORD CDibImage::DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPCOREHEADER lpbmc;	// ָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ��
	if (IS_WIN30_DIB(lpDIB))
	{		
		return lpbmi->biWidth;		// ����Windows 3.0 DIB������lpbmi->biWidth
	}
	else
	{		
		return (DWORD)lpbmc->bcWidth;	// ����������ʽ��DIB������lpbmc->bcWidth
	}
}
/*************************************************************************
* �������ƣ�
*   MedianFilter()
* ����:
*   LPSTR lpDIBBits		- ָ��ԴDIBͼ��ָ��
*   LONG  lWidth			- Դͼ���ȣ���������
*   LONG  lHeight			- Դͼ��߶ȣ���������
*   int   iFilterH			- �˲����ĸ߶�
*   int   iFilterW			- �˲����Ŀ��
*   int   iFilterMX		- �˲���������Ԫ��X����
*   int   iFilterMY		- �˲���������Ԫ��Y����
* ����ֵ:
*   BOOL					- �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�����DIBͼ�������ֵ�˲���
************************************************************************/
BOOL CDibImage::MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
							 int iFilterH, int iFilterW, int iFilterMX, int iFilterMY)
{		
	unsigned char*	lpSrc;			// ָ��Դͼ���ָ��	
	unsigned char*	lpDst;			// ָ��Ҫ���������ָ��	
	LPSTR			lpNewDIBBits;	// ָ����ͼ���ָ��
	HLOCAL			hNewDIBBits;	
	unsigned char	* aValue;		// ָ���˲��������ָ��
	HLOCAL			hArray;	
	LONG			i,j,k,l;		// ѭ������	

	LONG			lLineBytes;		// ͼ��ÿ�е��ֽ���	
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}

	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��ʱ�����ڴ棬�Ա����˲�������
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);
	if (hArray == NULL)
	{
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);

		return FALSE;
	}

	aValue = (unsigned char * )LocalLock(hArray);

	// ��ʼ��ֵ�˲�
	// ��(��ȥ��Ե����)
	for(i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// ��ȡ�˲�������
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// ָ��DIB��i - iFilterMY + k�У���j - iFilterMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 
						- i + iFilterMY - k) + j - iFilterMX + l;

					// ��������ֵ
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}

			// ��ȡ��ֵ
			* lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
		}
	}

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);

	return TRUE;
}
/*************************************************************************
* �������ƣ�
*   GetMedianNum()
* ����:
*   unsigned char * bpArray	- ָ��Ҫ��ȡ��ֵ������ָ��
*   int   iFilterLen			- ���鳤��
* ����ֵ:
*   unsigned char      - ����ָ���������ֵ��
* ˵��:
*   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
************************************************************************/
unsigned char CDibImage::GetMedianNum(unsigned char * bArray, int iFilterLen)
{
	int		i,j;			// ѭ������
	unsigned char bTemp;

	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// ����
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}

	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}

	return bTemp;
}
/*************************************************************************
* �������ƣ�
*   RotateDIB()
* ����:
*   LPSTR lpDIB		- ָ��ԴDIB��ָ��
*   int iRotateAngle	- ��ת�ĽǶȣ�0-360�ȣ�
* ����ֵ:
*   HGLOBAL            - ��ת�ɹ�������DIB��������򷵻�NULL��
* ˵��:
*   �ú���������ͼ������Ϊ������תDIBͼ�񣬷���������DIB�ľ����
* ���øú������Զ�����ͼ������ʾ���е����ء������в������ڽ���
* ֵ�㷨���в�ֵ��
************************************************************************/
HGLOBAL CDibImage::RotateDIB(LPSTR lpDIB, int iRotateAngle)
{	
	LONG	lWidth;				// Դͼ��Ŀ��
	LONG	lHeight;			// Դͼ��ĸ߶�	
	LONG	lNewWidth;			// ��ת��ͼ��Ŀ��
	LONG	lNewHeight;			// ��ת��ͼ��ĸ߶�	
	LONG	lLineBytes;			// ͼ��ÿ�е��ֽ���	
	LONG	lNewLineBytes;		// ��ת��ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����	
	LPSTR	lpDIBBits;			// ָ��Դͼ���ָ��	
	LPSTR	lpSrc;				// ָ��Դ���ص�ָ��	
	HANDLE	hDIB;				// ��ת����DIB���	
	LPSTR	lpDst;				// ָ����תͼ���Ӧ���ص�ָ��	
	LPSTR	lpNewDIB;			// ָ����תͼ���ָ��
	LPSTR	lpNewDIBBits;	
	LPBITMAPINFOHEADER lpbmi;	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��	
	LPBITMAPCOREHEADER lpbmc;	// ָ��BITMAPCOREINFO�ṹ��ָ��

	LONG	i;					// ѭ����������������DIB�е����꣩
	LONG	j;	
	LONG	i0;					// ������ԴDIB�е�����
	LONG	j0;	

	float	fRotateAngle;		// ��ת�Ƕȣ����ȣ�	
	float	fSina, fCosa;		// ��ת�Ƕȵ����Һ�����	
	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;	
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	float	f1,f2;

	lpDIBBits = FindDIBBits(lpDIB);		// �ҵ�ԴDIBͼ��������ʼλ��	
	lWidth = DIBWidth(lpDIB);			// ��ȡͼ���"���"��4�ı�����	
	lLineBytes = WIDTHBYTES(lWidth * 8);// ����ͼ��ÿ�е��ֽ���	
	lHeight = DIBHeight(lpDIB);			// ��ȡͼ��ĸ߶�

	// ����ת�ǶȴӶ�ת��������
	fRotateAngle = (float) RADIAN(iRotateAngle);	
	fSina = (float) sin((double)fRotateAngle);	// ������ת�Ƕȵ�����
	fCosa = (float) cos((double)fRotateAngle);	// ������ת�Ƕȵ�����

	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float) (- (lWidth  - 1) / 2);
	fSrcY1 = (float) (  (lHeight - 1) / 2);
	fSrcX2 = (float) (  (lWidth  - 1) / 2);
	fSrcY2 = (float) (  (lHeight - 1) / 2);
	fSrcX3 = (float) (- (lWidth  - 1) / 2);
	fSrcY3 = (float) (- (lHeight - 1) / 2);
	fSrcX4 = (float) (  (lWidth  - 1) / 2);
	fSrcY4 = (float) (- (lHeight - 1) / 2);

	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// ������ת���ͼ��ʵ�ʿ��
	lNewWidth  = (LONG)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	// ������ת���ͼ��߶�
	lNewHeight = (LONG)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	// �������������������Ժ�ÿ�ζ�������
	f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth  - 1));
	f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));

	// �����ڴ棬�Ա�����DIB
	hDIB = (HANDLE) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + 
		*(LPDWORD)lpDIB + PaletteSize(lpDIB));
	if (hDIB == NULL)
	{
		return NULL;
	}

	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);

	// ����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + PaletteSize(lpDIB));

	// �ҵ���DIB������ʼλ��
	lpNewDIBBits = FindDIBBits(lpNewDIB);

	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// ����DIB��ͼ��ĸ߶ȺͿ��
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// ����Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// ����������ʽ��DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}

	for(i = 0; i < lNewHeight; i++)		// ���ͼ��ÿ�н��в���
	{		
		for(j = 0; j < lNewWidth; j++)	// ���ͼ��ÿ�н��в���
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			// �����������ԴDIB�е�����
			i0 = (LONG) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
			j0 = (LONG) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);

			// �ж��Ƿ���Դͼ��Χ��
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				* ((unsigned char*)lpDst) = 255;
			}			
		}		
	}

	return hDIB;
}
/*************************************************************************
* �������ƣ�
*   CreateDIBPalette()
* ����:
*   HDIB hDIB          - ָ��DIB�����ָ��
*   CPalette* pPal     - ָ��DIB�����ɫ���ָ��
* ����ֵ:
*   BOOL               - �����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�������DIB����һ���߼���ɫ�壬��DIB�ж�ȡ��ɫ���浽��ɫ���У�
* ����ո��߼���ɫ�崴��һ���µĵ�ɫ�壬�����ظõ�ɫ��ľ��������
* ��������õ���ɫ����ʾDIBͼ��
************************************************************************/
BOOL CDibImage::CreateDIBPalette(HANDLE hDIB, CPalette* pPal)
{

	LPLOGPALETTE lpPal;		// ָ���߼���ɫ���ָ��
	HANDLE hLogPal;			// �߼���ɫ��ľ��
	HPALETTE hPal = NULL;	// ��ɫ��ľ��
	int i;					// ѭ������	
	WORD wNumColors;		// ��ɫ���е���ɫ��Ŀ	
	LPSTR lpbi;				// ָ��DIB��ָ��
	LPBITMAPINFO lpbmi;		// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��	
	LPBITMAPCOREINFO lpbmc;	// ָ��BITMAPCOREINFO�ṹ��ָ��	
	BOOL bWinStyleDIB;		// �����Ƿ���Win3.0 DIB�ı��	
	BOOL bResult = FALSE;	// �������

	if (hDIB == NULL)
	{
		return FALSE;
	}

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);	// ����DIB
	lpbmi = (LPBITMAPINFO)lpbi;		// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmc = (LPBITMAPCOREINFO)lpbi;	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	wNumColors = DIBNumColors(lpbi);// ��ȡDIB����ɫ���е���ɫ��Ŀ

	if (wNumColors != 0)
	{
		// ����Ϊ�߼���ɫ���ڴ�
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
			+ sizeof(PALETTEENTRY)
			* wNumColors);	
		// ����ڴ治�㣬�˳�
		if (hLogPal == 0)
		{	
			::GlobalUnlock((HGLOBAL) hDIB);	// �������
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);		
		lpPal->palVersion = PALVERSION;			// ���ð汾��
		lpPal->palNumEntries = (WORD)wNumColors;// ������ɫ��Ŀ
		bWinStyleDIB = IS_WIN30_DIB(lpbi);		// �ж��Ƿ���WIN3.0��DIB

		// ��ȡ��ɫ��
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// ��ȡ��ɫ��ɫ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;				
				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				// ��ȡ��ɫ��ɫ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;				
				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		bResult = pPal->CreatePalette(lpPal);// �����߼���ɫ�崴����ɫ�壬������ָ��
		::GlobalUnlock((HGLOBAL) hLogPal);	// �������
		::GlobalFree((HGLOBAL) hLogPal);	// �ͷ��߼���ɫ��
	}

	::GlobalUnlock((HGLOBAL) hDIB);			// �������
	return bResult;
}
/*************************************************************************
* �������ƣ�
*   ZoomDIB()
* ����:
*   LPSTR lpDIB		- ָ��ԴDIB��ָ��
*   float fXZoomRatio	- X�᷽�����ű���
*   float fYZoomRatio	- Y�᷽�����ű���
* ����ֵ:
*   HGLOBAL            - ���ųɹ�������DIB��������򷵻�NULL��
* ˵��:
*   �ú�����������DIBͼ�񣬷���������DIB�ľ����
************************************************************************/
HGLOBAL CDibImage::ZoomDIB(LPSTR lpDIB, float fXZoomRatio, float fYZoomRatio)
{
	LONG	lWidth;				// Դͼ��Ŀ��
	LONG	lHeight;			// Դͼ��ĸ߶�	
	LONG	lNewWidth;			// ���ź�ͼ��Ŀ��
	LONG	lNewHeight;			// ���ź�ͼ��ĸ߶�	
	LONG	lNewLineBytes;		// ���ź�ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����	
	LPSTR	lpDIBBits;			// ָ��Դͼ���ָ��	
	LPSTR	lpSrc;				// ָ��Դ���ص�ָ��	
	HDIB	hDIB;				// ���ź���DIB���	
	LPSTR	lpDst;				// ָ������ͼ���Ӧ���ص�ָ��	
	LPSTR	lpNewDIB;			// ָ������ͼ���ָ��
	LPSTR	lpNewDIBBits;	
	LPBITMAPINFOHEADER lpbmi;	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��	
	LPBITMAPCOREHEADER lpbmc;	// ָ��BITMAPCOREINFO�ṹ��ָ��	

	LONG	i;					// ѭ����������������DIB�е����꣩
	LONG	j;	
	LONG	i0;					// ������ԴDIB�е�����
	LONG	j0;


	LONG lLineBytes;			// ͼ��ÿ�е��ֽ���	
	lpDIBBits = FindDIBBits(lpDIB);	// �ҵ�ԴDIBͼ��������ʼλ��		
	lWidth = DIBWidth(lpDIB);	// ��ȡͼ��Ŀ��
	lLineBytes = WIDTHBYTES(lWidth * 8);	// ����ͼ��ÿ�е��ֽ���
	lHeight = DIBHeight(lpDIB);	// ��ȡͼ��ĸ߶�

	// �������ź��ͼ��ʵ�ʿ��
	// �˴�ֱ�Ӽ�0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
	lNewWidth = (LONG) (DIBWidth(lpDIB) * fXZoomRatio + 0.5);

	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	// �������ź��ͼ��߶�
	lNewHeight = (LONG) (lHeight * fYZoomRatio + 0.5);

	// �����ڴ棬�Ա�����DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + 
		*(LPDWORD)lpDIB + PaletteSize(lpDIB));	
	if (hDIB == NULL)
	{
		return NULL;
	}

	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);

	// ����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + PaletteSize(lpDIB));

	// �ҵ���DIB������ʼλ��
	lpNewDIBBits = FindDIBBits(lpNewDIB);
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// ����DIB��ͼ��ĸ߶ȺͿ��
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// ����Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// ����������ʽ��DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}

	for(i = 0; i < lNewHeight; i++)		// ���ͼ��ÿ�н��в���
	{		
		for(j = 0; j < lNewWidth; j++)	// ���ͼ��ÿ�н��в���
		{			
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			// �����������ԴDIB�е�����
			i0 = (LONG) (i / fYZoomRatio + 0.5);
			j0 = (LONG) (j / fXZoomRatio + 0.5);

			// �ж��Ƿ���Դͼ��Χ��
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{				
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;				
				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				* ((unsigned char*)lpDst) = 255;
			}			
		}		
	}

	return hDIB;
}
//��ת���豸��ص�ͼƬ
/*************************************************************************
* �������ƣ�
*   RotateImage()
* ����:
*   CDC* pDC		- ָ��ͼ���豸������
*   double angle	- ƫת�Ƕȣ����ȣ�
* ����ֵ:
*  void
* ˵��:
*   ��ת���豸��ص�ͼƬ
************************************************************************/
void CDibImage::RotateImage(CDC* pDC,double angle,CRect& rect)
{
	HDC hDc = pDC->GetSafeHdc();
	POINT centerPt;
	centerPt.x = (rect.right - rect.left)/2 ;
	centerPt.y = (rect.bottom - rect.top)/2 ;

	int nGraphicsMode = SetGraphicsMode(hDc, GM_ADVANCED);
	XFORM xform;
	if ( angle != 0 )
	{
		double fangle = RADIAN(angle);
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(centerPt.x - cos(fangle)*centerPt.x + sin(fangle)*centerPt.y);
		xform.eDy = (float)(centerPt.y - cos(fangle)*centerPt.y - sin(fangle)*centerPt.x);

		SetWorldTransform(hDc, &xform);
	}

}