/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ͼ����ͷ�ļ�
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
	void Smooth();//ƽ��
	void MidFilter();//��ֵ�˲�
	void GeomRota();//��ת
	void GeomZoom();//����

};
