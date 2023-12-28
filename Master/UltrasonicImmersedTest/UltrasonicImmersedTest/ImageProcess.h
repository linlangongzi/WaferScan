/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 图像处理头文件
*Filename : ImageProcess.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-08-08 
*/

#pragma once

class CCScanView;
class CImageProcess
{
public:
	CImageProcess(void);
public:
	~CImageProcess(void);
public:
	void Smooth();//平滑
	void MidFilter();//中值滤波
	void GeomRota();//旋转
	void GeomZoom();//缩放

};
