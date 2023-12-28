#pragma once

#include "const.h"
#include "p9054_lib.h"
#include <QSharedPointer>

class CPLXCommunication;
typedef QSharedPointer<CPLXCommunication> SharedPLXCommunication;

typedef struct
{
    int index;
    P9054_HANDLE hDev;
    WORD wSelChannel;
    SharedPLXCommunication pPLX;
    int nFlawOrBottom;	//伤波底波切换变量
}CARD_THREAD_PARAM;

typedef QSharedPointer<CARD_THREAD_PARAM> SharedCardParam;
typedef QVector<SharedCardParam> CardParamVec;
typedef QSharedPointer<CardParamVec> SharedCardParamVec;
