#ifndef _CARD_HARDWARE_ADDR_H_
#define _CARD_HARDWARE_ADDR_H_

#ifdef PIPE_DETECT
const unsigned int AEC_CARD_SELFTEST_RESULT_ADDRESS = 0x20; // 卡自检结果 PipeDetect项目为0x1E; TrainWheelDetect项目为0x22，FPGA没有统一为同一个地址，暂时先条件编译
#else
const unsigned int AEC_CARD_SELFTEST_RESULT_ADDRESS = 0x22;
#endif

const unsigned int HARDWARE_VERSION_ADDRESS = 0x0;             // 版本信息
const unsigned int SYS_SELF_TEST_ADDRESS = 0x00C00;            // 系统自检SRAM（512）
const unsigned int SYS_SELF_TEST_ADDRESS_END = 0x00FFF;        // 系统自检SRAM尾地址

// 多功能卡
const unsigned int MPB_CARD_READY_ADDR = 0x02;          // 板卡 ready/error
#define MPB_CARD_READY_OK                   0x1         // ready
#define MPB_CARD_READY_ERROR                0x0         // error

const unsigned int MPB_SYS_READY_ADDR = 0x08;           // 系统 ready/error
#define MPB_SYS_READY_OK                    0x1         // ready
#define MPB_SYS_READY_ERROR                 0x0         // error

const unsigned int  MULTI_CARD_SELF_TEST_ADDR = 0x80;   // 板卡内部测试
#define MULTI_CARD_SELF_TEST_ADDR_SIZE      8           // bit

#define SUB_DEVICE_ID_H                     0x2e
#define SUB_DEVICE_ID_L                     0x2c
#define MUTLI_FUNCTION_CARD_ID              0x101

#endif  // _CARD_HARDWARE_ADDR_H_
