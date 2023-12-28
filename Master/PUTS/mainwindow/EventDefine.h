#pragma once

#define A_SCAN_EVENT QEvent::User+100
#define FEATURE_INFO_EVENT QEvent::User+200

class ASCANEvent : public QEvent
{
public:
	ASCANEvent();
public:
	CPicData* pData;
	WORD wChannelNo;
	int nFlawOrBottom;	//伤波底波切换变量
};

class FEATUREINFOEvent : public QEvent
{
public:
	FEATUREINFOEvent();
public:
	CH_FLAWDATA_CSCAN_D* pData;

};