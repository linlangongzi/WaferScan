/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 图像处理算法源文件
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
* 函数名称：
*   PaletteSize()
* 参数:
*   LPSTR lpbi         - 指向DIB对象的指针
* 返回值:
*   WORD               - DIB中调色板的大小
* 说明:
*   该函数返回DIB中调色板的大小。对于Windows 3.0 DIB，返回颜色数目×
* RGBQUAD的大小；对于其它返回颜色数目×RGBTRIPLE的大小。
************************************************************************/
WORD CDibImage::PaletteSize(LPSTR lpbi)
{
	// 计算DIB中调色板的大小
	if (IS_WIN30_DIB (lpbi))
	{
		//返回颜色数目×RGBQUAD的大小
		return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}
	else
	{
		//返回颜色数目×RGBTRIPLE的大小
		return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
	}
}
/*************************************************************************
* 函数名称：
*   DIBNumColors()
* 参数:
*   LPSTR lpbi         - 指向DIB对象的指针
* 返回值:
*   WORD               - 返回调色板中颜色的种数
* 说明:
*   该函数返回DIB中调色板的颜色的种数。对于单色位图，返回2，
* 对于16色位图，返回16，对于256色位图，返回256；对于真彩色
* 位图（24位），没有调色板，返回0。
************************************************************************/
WORD CDibImage::DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;

	// 对于Windows的DIB, 实际颜色的数目可以比象素的位数要少。
	// 对于这种情况，则返回一个近似的数值。

	// 判断是否是WIN3.0 DIB
	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;
		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;	// 读取dwClrUsed值

		if (dwClrUsed != 0)
		{
			// 如果dwClrUsed（实际用到的颜色数）不为0，直接返回该值
			return (WORD)dwClrUsed;
		}
	}

	// 读取象素的位数
	if (IS_WIN30_DIB(lpbi))
	{		
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;	// 读取biBitCount值
	}
	else
	{		
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;	// 读取biBitCount值
	}

	// 按照象素的位数计算颜色数目
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
* 函数名称：
*   Template()
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数）
*   LONG  lHeight      - 源图像高度（象素数）
*   int   iTempH		- 模板的高度
*   int   iTempW		- 模板的宽度
*   int   iTempMX		- 模板的中心元素X坐标 ( < iTempW - 1)
*   int   iTempMY		- 模板的中心元素Y坐标 ( < iTempH - 1)
*	 FLOAT * fpArray	- 指向模板数组的指针
*	 FLOAT fCoef		- 模板系数
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用指定的模板（任意大小）来对图像进行操作，参数iTempH指定模板
* 的高度，参数iTempW指定模板的宽度，参数iTempMX和iTempMY指定模板的中心
* 元素坐标，参数fpArray指定模板元素，fCoef指定系数。
************************************************************************/
BOOL CDibImage::Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int iTempH, int iTempW, int iTempMX, int iTempMY,
						 FLOAT * fpArray, FLOAT fCoef)
{	
	LPSTR	lpNewDIBBits;		// 指向复制图像的指针
	HLOCAL	hNewDIBBits;	
	unsigned char*	lpSrc;		// 指向源图像的指针	
	unsigned char*	lpDst;		// 指向要复制区域的指针	
	LONG	i,j,k,l;			// 循环变量	
	FLOAT	fResult;			// 计算结果

	LONG lLineBytes;			// 图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}

	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	for(i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)	// 行(除去边缘几行)
	{		
		for(j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)// 列(除去边缘几列)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 
						- i + iTempMY - k) + j - iTempMX + l;

					// 保存象素值
					fResult += (* lpSrc) * fpArray[k * iTempW + l];
				}
			}

			fResult *= fCoef;					// 乘上系数			
			fResult = (FLOAT ) fabs(fResult);	// 取绝对值
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

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}
/*************************************************************************
* 函数名称：
*   DIBHeight()
* 参数:
*   LPSTR lpDIB        - 指向DIB对象的指针
* 返回值:
*   DWORD              - DIB中图像的高度
* 说明:
*   该函数返回DIB中图像的高度。对于Windows 3.0 DIB，返回BITMAPINFOHEADER
* 中的biHeight值；对于其它返回BITMAPCOREHEADER中的bcHeight值。
************************************************************************/
DWORD CDibImage::DIBHeight(LPSTR lpDIB)
{	
	LPBITMAPINFOHEADER lpbmi;	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPCOREHEADER lpbmc;	// 指向BITMAPCOREINFO结构的指针
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// 返回DIB中图像的宽度
	if (IS_WIN30_DIB(lpDIB))
	{		
		return lpbmi->biHeight;		// 对于Windows 3.0 DIB，返回lpbmi->biHeight
	}
	else
	{		
		return (DWORD)lpbmc->bcHeight;	// 对于其它格式的DIB，返回lpbmc->bcHeight
	}
}
/*************************************************************************
* 函数名称：
*   DIBWidth()
* 参数:
*   LPSTR lpbi         - 指向DIB对象的指针
* 返回值:
*   DWORD              - DIB中图像的宽度
* 说明:
*   该函数返回DIB中图像的宽度。对于Windows 3.0 DIB，返回BITMAPINFOHEADER
* 中的biWidth值；对于其它返回BITMAPCOREHEADER中的bcWidth值。
************************************************************************/
DWORD CDibImage::DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPCOREHEADER lpbmc;	// 指向BITMAPCOREINFO结构的指针
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// 返回DIB中图像的宽度
	if (IS_WIN30_DIB(lpDIB))
	{		
		return lpbmi->biWidth;		// 对于Windows 3.0 DIB，返回lpbmi->biWidth
	}
	else
	{		
		return (DWORD)lpbmc->bcWidth;	// 对于其它格式的DIB，返回lpbmc->bcWidth
	}
}
/*************************************************************************
* 函数名称：
*   MedianFilter()
* 参数:
*   LPSTR lpDIBBits		- 指向源DIB图像指针
*   LONG  lWidth			- 源图像宽度（象素数）
*   LONG  lHeight			- 源图像高度（象素数）
*   int   iFilterH			- 滤波器的高度
*   int   iFilterW			- 滤波器的宽度
*   int   iFilterMX		- 滤波器的中心元素X坐标
*   int   iFilterMY		- 滤波器的中心元素Y坐标
* 返回值:
*   BOOL					- 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数对DIB图像进行中值滤波。
************************************************************************/
BOOL CDibImage::MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
							 int iFilterH, int iFilterW, int iFilterMX, int iFilterMY)
{		
	unsigned char*	lpSrc;			// 指向源图像的指针	
	unsigned char*	lpDst;			// 指向要复制区域的指针	
	LPSTR			lpNewDIBBits;	// 指向复制图像的指针
	HLOCAL			hNewDIBBits;	
	unsigned char	* aValue;		// 指向滤波器数组的指针
	HLOCAL			hArray;	
	LONG			i,j,k,l;		// 循环变量	

	LONG			lLineBytes;		// 图像每行的字节数	
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}

	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// 暂时分配内存，以保存滤波器数组
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);
	if (hArray == NULL)
	{
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);

		return FALSE;
	}

	aValue = (unsigned char * )LocalLock(hArray);

	// 开始中值滤波
	// 行(除去边缘几行)
	for(i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// 列(除去边缘几列)
		for(j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 读取滤波器数组
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// 指向DIB第i - iFilterMY + k行，第j - iFilterMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 
						- i + iFilterMY - k) + j - iFilterMX + l;

					// 保存象素值
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}

			// 获取中值
			* lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);

	return TRUE;
}
/*************************************************************************
* 函数名称：
*   GetMedianNum()
* 参数:
*   unsigned char * bpArray	- 指向要获取中值的数组指针
*   int   iFilterLen			- 数组长度
* 返回值:
*   unsigned char      - 返回指定数组的中值。
* 说明:
*   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
************************************************************************/
unsigned char CDibImage::GetMedianNum(unsigned char * bArray, int iFilterLen)
{
	int		i,j;			// 循环变量
	unsigned char bTemp;

	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// 互换
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}

	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}

	return bTemp;
}
/*************************************************************************
* 函数名称：
*   RotateDIB()
* 参数:
*   LPSTR lpDIB		- 指向源DIB的指针
*   int iRotateAngle	- 旋转的角度（0-360度）
* 返回值:
*   HGLOBAL            - 旋转成功返回新DIB句柄，否则返回NULL。
* 说明:
*   该函数用来以图像中心为中心旋转DIB图像，返回新生成DIB的句柄。
* 调用该函数会自动扩大图像以显示所有的象素。函数中采用最邻近插
* 值算法进行插值。
************************************************************************/
HGLOBAL CDibImage::RotateDIB(LPSTR lpDIB, int iRotateAngle)
{	
	LONG	lWidth;				// 源图像的宽度
	LONG	lHeight;			// 源图像的高度	
	LONG	lNewWidth;			// 旋转后图像的宽度
	LONG	lNewHeight;			// 旋转后图像的高度	
	LONG	lLineBytes;			// 图像每行的字节数	
	LONG	lNewLineBytes;		// 旋转后图像的宽度（lNewWidth'，必须是4的倍数）	
	LPSTR	lpDIBBits;			// 指向源图像的指针	
	LPSTR	lpSrc;				// 指向源象素的指针	
	HANDLE	hDIB;				// 旋转后新DIB句柄	
	LPSTR	lpDst;				// 指向旋转图像对应象素的指针	
	LPSTR	lpNewDIB;			// 指向旋转图像的指针
	LPSTR	lpNewDIBBits;	
	LPBITMAPINFOHEADER lpbmi;	// 指向BITMAPINFO结构的指针（Win3.0）	
	LPBITMAPCOREHEADER lpbmc;	// 指向BITMAPCOREINFO结构的指针

	LONG	i;					// 循环变量（象素在新DIB中的坐标）
	LONG	j;	
	LONG	i0;					// 象素在源DIB中的坐标
	LONG	j0;	

	float	fRotateAngle;		// 旋转角度（弧度）	
	float	fSina, fCosa;		// 旋转角度的正弦和余弦	
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;	
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	float	f1,f2;

	lpDIBBits = FindDIBBits(lpDIB);		// 找到源DIB图像象素起始位置	
	lWidth = DIBWidth(lpDIB);			// 获取图像的"宽度"（4的倍数）	
	lLineBytes = WIDTHBYTES(lWidth * 8);// 计算图像每行的字节数	
	lHeight = DIBHeight(lpDIB);			// 获取图像的高度

	// 将旋转角度从度转换到弧度
	fRotateAngle = (float) RADIAN(iRotateAngle);	
	fSina = (float) sin((double)fRotateAngle);	// 计算旋转角度的正弦
	fCosa = (float) cos((double)fRotateAngle);	// 计算旋转角度的余弦

	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float) (- (lWidth  - 1) / 2);
	fSrcY1 = (float) (  (lHeight - 1) / 2);
	fSrcX2 = (float) (  (lWidth  - 1) / 2);
	fSrcY2 = (float) (  (lHeight - 1) / 2);
	fSrcX3 = (float) (- (lWidth  - 1) / 2);
	fSrcY3 = (float) (- (lHeight - 1) / 2);
	fSrcX4 = (float) (  (lWidth  - 1) / 2);
	fSrcY4 = (float) (- (lHeight - 1) / 2);

	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// 计算旋转后的图像实际宽度
	lNewWidth  = (LONG)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

	// 计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	// 计算旋转后的图像高度
	lNewHeight = (LONG)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	// 两个常数，这样不用以后每次都计算了
	f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth  - 1));
	f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));

	// 分配内存，以保存新DIB
	hDIB = (HANDLE) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + 
		*(LPDWORD)lpDIB + PaletteSize(lpDIB));
	if (hDIB == NULL)
	{
		return NULL;
	}

	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);

	// 复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + PaletteSize(lpDIB));

	// 找到新DIB象素起始位置
	lpNewDIBBits = FindDIBBits(lpNewDIB);

	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// 更新DIB中图像的高度和宽度
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// 对于Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// 对于其它格式的DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}

	for(i = 0; i < lNewHeight; i++)		// 针对图像每行进行操作
	{		
		for(j = 0; j < lNewWidth; j++)	// 针对图像每列进行操作
		{
			// 指向新DIB第i行，第j个象素的指针
			// 注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			// 计算该象素在源DIB中的坐标
			i0 = (LONG) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
			j0 = (LONG) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);

			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// 指向源DIB第i0行，第j0个象素的指针
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

				// 复制象素
				*lpDst = *lpSrc;
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				* ((unsigned char*)lpDst) = 255;
			}			
		}		
	}

	return hDIB;
}
/*************************************************************************
* 函数名称：
*   CreateDIBPalette()
* 参数:
*   HDIB hDIB          - 指向DIB对象的指针
*   CPalette* pPal     - 指向DIB对象调色板的指针
* 返回值:
*   BOOL               - 创建成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数按照DIB创建一个逻辑调色板，从DIB中读取颜色表并存到调色板中，
* 最后按照该逻辑调色板创建一个新的调色板，并返回该调色板的句柄。这样
* 可以用最好的颜色来显示DIB图像。
************************************************************************/
BOOL CDibImage::CreateDIBPalette(HANDLE hDIB, CPalette* pPal)
{

	LPLOGPALETTE lpPal;		// 指向逻辑调色板的指针
	HANDLE hLogPal;			// 逻辑调色板的句柄
	HPALETTE hPal = NULL;	// 调色板的句柄
	int i;					// 循环变量	
	WORD wNumColors;		// 颜色表中的颜色数目	
	LPSTR lpbi;				// 指向DIB的指针
	LPBITMAPINFO lpbmi;		// 指向BITMAPINFO结构的指针（Win3.0）	
	LPBITMAPCOREINFO lpbmc;	// 指向BITMAPCOREINFO结构的指针	
	BOOL bWinStyleDIB;		// 表明是否是Win3.0 DIB的标记	
	BOOL bResult = FALSE;	// 创建结果

	if (hDIB == NULL)
	{
		return FALSE;
	}

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);	// 锁定DIB
	lpbmi = (LPBITMAPINFO)lpbi;		// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmc = (LPBITMAPCOREINFO)lpbi;	// 获取指向BITMAPCOREINFO结构的指针
	wNumColors = DIBNumColors(lpbi);// 获取DIB中颜色表中的颜色数目

	if (wNumColors != 0)
	{
		// 分配为逻辑调色板内存
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
			+ sizeof(PALETTEENTRY)
			* wNumColors);	
		// 如果内存不足，退出
		if (hLogPal == 0)
		{	
			::GlobalUnlock((HGLOBAL) hDIB);	// 解除锁定
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);		
		lpPal->palVersion = PALVERSION;			// 设置版本号
		lpPal->palNumEntries = (WORD)wNumColors;// 设置颜色数目
		bWinStyleDIB = IS_WIN30_DIB(lpbi);		// 判断是否是WIN3.0的DIB

		// 读取调色板
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// 读取红色绿色蓝色分量
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;				
				// 保留位
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				// 读取红色绿色蓝色分量
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;				
				// 保留位
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		bResult = pPal->CreatePalette(lpPal);// 按照逻辑调色板创建调色板，并返回指针
		::GlobalUnlock((HGLOBAL) hLogPal);	// 解除锁定
		::GlobalFree((HGLOBAL) hLogPal);	// 释放逻辑调色板
	}

	::GlobalUnlock((HGLOBAL) hDIB);			// 解除锁定
	return bResult;
}
/*************************************************************************
* 函数名称：
*   ZoomDIB()
* 参数:
*   LPSTR lpDIB		- 指向源DIB的指针
*   float fXZoomRatio	- X轴方向缩放比率
*   float fYZoomRatio	- Y轴方向缩放比率
* 返回值:
*   HGLOBAL            - 缩放成功返回新DIB句柄，否则返回NULL。
* 说明:
*   该函数用来缩放DIB图像，返回新生成DIB的句柄。
************************************************************************/
HGLOBAL CDibImage::ZoomDIB(LPSTR lpDIB, float fXZoomRatio, float fYZoomRatio)
{
	LONG	lWidth;				// 源图像的宽度
	LONG	lHeight;			// 源图像的高度	
	LONG	lNewWidth;			// 缩放后图像的宽度
	LONG	lNewHeight;			// 缩放后图像的高度	
	LONG	lNewLineBytes;		// 缩放后图像的宽度（lNewWidth'，必须是4的倍数）	
	LPSTR	lpDIBBits;			// 指向源图像的指针	
	LPSTR	lpSrc;				// 指向源象素的指针	
	HDIB	hDIB;				// 缩放后新DIB句柄	
	LPSTR	lpDst;				// 指向缩放图像对应象素的指针	
	LPSTR	lpNewDIB;			// 指向缩放图像的指针
	LPSTR	lpNewDIBBits;	
	LPBITMAPINFOHEADER lpbmi;	// 指向BITMAPINFO结构的指针（Win3.0）	
	LPBITMAPCOREHEADER lpbmc;	// 指向BITMAPCOREINFO结构的指针	

	LONG	i;					// 循环变量（象素在新DIB中的坐标）
	LONG	j;	
	LONG	i0;					// 象素在源DIB中的坐标
	LONG	j0;


	LONG lLineBytes;			// 图像每行的字节数	
	lpDIBBits = FindDIBBits(lpDIB);	// 找到源DIB图像象素起始位置		
	lWidth = DIBWidth(lpDIB);	// 获取图像的宽度
	lLineBytes = WIDTHBYTES(lWidth * 8);	// 计算图像每行的字节数
	lHeight = DIBHeight(lpDIB);	// 获取图像的高度

	// 计算缩放后的图像实际宽度
	// 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
	lNewWidth = (LONG) (DIBWidth(lpDIB) * fXZoomRatio + 0.5);

	// 计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	// 计算缩放后的图像高度
	lNewHeight = (LONG) (lHeight * fYZoomRatio + 0.5);

	// 分配内存，以保存新DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + 
		*(LPDWORD)lpDIB + PaletteSize(lpDIB));	
	if (hDIB == NULL)
	{
		return NULL;
	}

	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);

	// 复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + PaletteSize(lpDIB));

	// 找到新DIB象素起始位置
	lpNewDIBBits = FindDIBBits(lpNewDIB);
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// 更新DIB中图像的高度和宽度
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// 对于Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// 对于其它格式的DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}

	for(i = 0; i < lNewHeight; i++)		// 针对图像每行进行操作
	{		
		for(j = 0; j < lNewWidth; j++)	// 针对图像每列进行操作
		{			
			// 指向新DIB第i行，第j个象素的指针
			// 注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			// 计算该象素在源DIB中的坐标
			i0 = (LONG) (i / fYZoomRatio + 0.5);
			j0 = (LONG) (j / fXZoomRatio + 0.5);

			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{				
				// 指向源DIB第i0行，第j0个象素的指针
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;				
				// 复制象素
				*lpDst = *lpSrc;
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				* ((unsigned char*)lpDst) = 255;
			}			
		}		
	}

	return hDIB;
}
//旋转与设备相关的图片
/*************************************************************************
* 函数名称：
*   RotateImage()
* 参数:
*   CDC* pDC		- 指向图形设备描述表
*   double angle	- 偏转角度（弧度）
* 返回值:
*  void
* 说明:
*   旋转与设备相关的图片
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