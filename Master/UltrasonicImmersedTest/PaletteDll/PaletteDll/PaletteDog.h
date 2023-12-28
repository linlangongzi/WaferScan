#pragma once

// CPaletteDog command target
#include <vector>
using namespace std;
struct PLATTE
{
	COLORREF color;
	float fMaxPercent;
	float fMinPercent;
};

struct MCOLOR
{
	float fPercent;
	int nGradualColor;
};

class AFX_EXT_CLASS CPaletteDog : public CObject
{
public:
	CPaletteDog();
	virtual ~CPaletteDog();
public:
//	void PlatteConfig();
	BOOL GetPlatteData(PLATTE * array, int& nSize, vector<int> &ColorNums, COLORREF& clrBack1, COLORREF& clrBack2);
};


