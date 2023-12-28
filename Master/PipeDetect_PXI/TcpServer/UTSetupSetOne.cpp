#include "UTSetupSetOne.h"
#include "Platform/Platform.h"
#include "ConfigObject/UTSetup.h"
#include "CardWork/AECWork.h"

UTSetupSetOne::UTSetupSetOne(QObject *parent) :
    QObject(parent)
{
//    ConnectTo();
}

void UTSetupSetOne::ConnectTo()
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    for(AecWorkVector::iterator it = pAecWorkVec->begin(); it != pAecWorkVec->end(); ++it)
    {
        sharedAecWork aecWork = *it;
        connect(this, SIGNAL(SetOne(UTSharedDataParam, qint32)), aecWork.data(), SLOT(OnSetOne(UTSharedDataParam, qint32)));
    }
}

void UTSetupSetOne::ReceiveSetOneData(UTSharedDataParam param, qint32 channel)
{
    UTSetup *setup = dynamic_cast<UTSetup *>(platform()->Setups()->GetByID(1)); // TODO 1为临时测试

    if(setup == NULL) {
        return;
    }

    setup->SetParameter("Range", param.data()->Range);          // 声程
    setup->SetParameter("Gain", param.data()->Plus);            // 增益
    setup->SetParameter("GainRef", param.data()->CPlus);        // 参考增益
    setup->SetParameter("Velocity", param.data()->Velocity);    // 声速

    setup->SetParameter("SurfaceDelay", param.data()->stWaveConfigure[0].Delay);  // 延迟
    setup->SetParameter("FlawDelay", param.data()->stWaveConfigure[1].Delay);     // 延迟
    setup->SetParameter("SurfaceSuppress", param.data()->stWaveConfigure[0].Suppression);          // 抑制
    setup->SetParameter("FlawSuppress", param.data()->stWaveConfigure[1].Suppression);             // 抑制
    setup->SetParameter("SurfaceRectifierZero", param.data()->stWaveConfigure[0].MiddleValue);     // 零点修正监视器
    setup->SetParameter("FlawRectifierZero", param.data()->stWaveConfigure[1].MiddleValue);        // 零点修正监视器
    setup->SetParameter("SurfaceRectifierMode", param.data()->stWaveConfigure[0].DetectMode);      // 检波模式
    setup->SetParameter("FlawRectifierMode", param.data()->stWaveConfigure[1].DetectMode);         // 检波模式

    setup->SetParameter("SurfaceGate1Amp", param.data()->stWaveConfigure[0].stGate[0].GateAmp);   // 闸门1幅值
    setup->SetParameter("SurfaceGate2Amp", param.data()->stWaveConfigure[0].stGate[1].GateAmp);   // 闸门2幅值
    setup->SetParameter("SurfaceGate3Amp", param.data()->stWaveConfigure[0].stGate[2].GateAmp);   // 闸门3幅值
    setup->SetParameter("SurfaceGate4Amp", param.data()->stWaveConfigure[0].stGate[3].GateAmp);   // 闸门4幅值
    setup->SetParameter("FlawGate1Amp", param.data()->stWaveConfigure[1].stGate[0].GateAmp);   // 闸门1幅值
    setup->SetParameter("FlawGate2Amp", param.data()->stWaveConfigure[1].stGate[1].GateAmp);   // 闸门2幅值
    setup->SetParameter("FlawGate3Amp", param.data()->stWaveConfigure[1].stGate[2].GateAmp);   // 闸门3幅值
    setup->SetParameter("FlawGate4Amp", param.data()->stWaveConfigure[1].stGate[3].GateAmp);   // 闸门4幅值

    setup->SetParameter("SurfaceGate1Start", param.data()->stWaveConfigure[0].stGate[0].GateStart);   // 闸门1前沿
    setup->SetParameter("SurfaceGate2Start", param.data()->stWaveConfigure[0].stGate[1].GateStart);   // 闸门2前沿                                         // 闸门2前沿
    setup->SetParameter("SurfaceGate3Start", param.data()->stWaveConfigure[0].stGate[2].GateStart);   // 闸门3前沿                                       // 闸门3前沿
    setup->SetParameter("SurfaceGate4Start", param.data()->stWaveConfigure[0].stGate[3].GateStart);   // 闸门4前沿
    setup->SetParameter("FlawGate1Start", param.data()->stWaveConfigure[1].stGate[0].GateStart);   // 闸门1前沿
    setup->SetParameter("FlawGate2Start", param.data()->stWaveConfigure[1].stGate[1].GateStart);   // 闸门2前沿                                         // 闸门2前沿
    setup->SetParameter("FlawGate3Start", param.data()->stWaveConfigure[1].stGate[2].GateStart);   // 闸门3前沿                                       // 闸门3前沿
    setup->SetParameter("FlawGate4Start", param.data()->stWaveConfigure[1].stGate[3].GateStart);   // 闸门4前沿

    setup->SetParameter("SurfaceGate1Width", param.data()->stWaveConfigure[0].stGate[0].GateWidth);   // 闸门1宽度
    setup->SetParameter("SurfaceGate2Width", param.data()->stWaveConfigure[0].stGate[1].GateWidth);   // 闸门2宽度
    setup->SetParameter("SurfaceGate3Width", param.data()->stWaveConfigure[0].stGate[2].GateWidth);   // 闸门3宽度
    setup->SetParameter("SurfaceGate4Width", param.data()->stWaveConfigure[0].stGate[3].GateWidth);   // 闸门4宽度
    setup->SetParameter("FlawGate1Width", param.data()->stWaveConfigure[1].stGate[0].GateWidth);   // 闸门1宽度
    setup->SetParameter("FlawGate2Width", param.data()->stWaveConfigure[1].stGate[1].GateWidth);   // 闸门2宽度
    setup->SetParameter("FlawGate3Width", param.data()->stWaveConfigure[1].stGate[2].GateWidth);   // 闸门3宽度
    setup->SetParameter("FlawGate4Width", param.data()->stWaveConfigure[1].stGate[3].GateWidth);   // 闸门4宽度

    // AECWork初始化与该对象绑定，暂时不支持PXI端修改参数，只接收上位机下发的参数配置，所以在这里发送SetOne信号
    // 如果PXI端需要有主动修改参数动能时
    // ConfigObject::SetRaw()发ValueChanged()给channel, channel发信号给AECWork，完成往AEC卡写参数
    // ConfigObject::SetRaw()发ValueChanged()给channel, channel发信号给TCP通信，完成上传给上位机
    emit SetOne(param, channel);
}
