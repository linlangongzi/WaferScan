#pragma once

#include <QtGlobal>

struct GlobalItemValue
{
    // 来源：电气
    char    PLCState             = 0; // PLC状态, (0:未就绪, 1:就绪)
    char    KeepTesting          = 0; // 断点续扫标志, (0:关闭断点续扫, 1:开启断点续扫)
    char    ReturnMark           = 0; // 缺陷回位标志, (0:关闭回位标志, 1:开启回位标志)
    char    TestStartFlag        = 0; // 探伤开始标志
    ulong   SWTestMode           = 5; // 探伤模式, (0:手动模式, 1:FinalBar, 2:FinalPipe, 3:SampleBar, 4:SamplePipe)
    quint32 ProbeBoxStatus       = 0; // 探头盒使用序列标志, (从低位到高位，一次代表1~5盒，1下落，0抬起)

    // 来源：上位机软件
    bool    IsInstrumentReady    = 0; // 仪器就绪状态
    bool    IsLocked             = 0; // 参数锁定
};
