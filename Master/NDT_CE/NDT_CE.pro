ARS_LIB_ROOT = ../../Lib
include(../Libs/lua.pri)

HEADERS += \
    NDT_CE/AllParamSetDlg.h \
    NDT_CE/ars200fpga.h \
    NDT_CE/chMenuWin.h \
    NDT_CE/const.h \
    NDT_CE/craftParamWin.h \
    NDT_CE/createCurveWin.h \
    NDT_CE/curveWin.h \
    NDT_CE/DisplaySetDlg.h \
    NDT_CE/displayWin.h \
    NDT_CE/fileManageWin.h \
    NDT_CE/fnBScanWin.h \
    NDT_CE/fnCreateDgsWin.h \
    NDT_CE/fnDgsCurveWin.h \
    NDT_CE/fnDgsMakeWin.h \
    NDT_CE/fnHanFengTanShangWin.h \
    NDT_CE/fnHuiBoBianMaWin.h \
    NDT_CE/fnMenMeiZhanKuanWin.h \
    NDT_CE/fnMenuWin.h \
    NDT_CE/fnModifyDgsWin.h \
    NDT_CE/fnPoKouSheZhiWin.h \
    NDT_CE/fnQuantitativeAnalysisWin.h \
    NDT_CE/fnReferenceWaveWin.h \
    NDT_CE/fnSpecialDisplayWin.h \
    NDT_CE/fnSpecialFunctionWin.h \
    NDT_CE/fnVideoRecordWin.h \
    NDT_CE/fnZhongHouBanTanShangWin.h \
    NDT_CE/fnZhuJianTanShangWin.h \
    NDT_CE/gateDisplayWin.h \
    NDT_CE/gateWin.h \
    NDT_CE/hotKeyWin.h \
    NDT_CE/IniFile.h \
    NDT_CE/IniParse.h \
    NDT_CE/interfaceCmdId.h \
    NDT_CE/makeCurveWin.h \
    NDT_CE/modeWin.h \
    NDT_CE/modifyCurveWin.h \
    NDT_CE/NDT_CE.h \
    NDT_CE/netCommand.h \
    NDT_CE/otherConfigureWin.h \
    NDT_CE/ParamSet2Dlg.h \
    NDT_CE/ParamSetDlg.h \
    NDT_CE/paramWin.h \
    NDT_CE/ReceiveParamDlg.h \
    NDT_CE/recvParamWin.h \
    NDT_CE/reportFormWin.h \
    NDT_CE/Resource.h \
    NDT_CE/screenCopyWin.h \
    NDT_CE/SendParamDlg.h \
    NDT_CE/sendParamWin.h \
    NDT_CE/stdafx.h \
    NDT_CE/tcgCurveWin.h \
    NDT_CE/TcpACScan.h \
    NDT_CE/TcpCScan.h \
    NDT_CE/TcpMonitor.h \
    NDT_CE/TcpParam.h \
    NDT_CE/TcpParamString.h \
    NDT_CE/TcpServer.h \
    NDT_CE/tlAdjustWin.h \
    NDT_CE/tsMenuWin.h \
    NDT_CE/waveSpeedWin.h \
    NDT_CE/win_com.h

SOURCES += \
    NDT_CE/allParamConfigWih.cpp \
    NDT_CE/allParamConfirmWin.cpp \
    NDT_CE/AllParamSetDlg.cpp \
    NDT_CE/ars200fpga.cpp \
    NDT_CE/ars200fpga_cscan.cpp \
    NDT_CE/autoGainPlusWin.cpp \
    NDT_CE/avgCurveMakeWin.cpp \
    NDT_CE/avgCurveWin.cpp \
    NDT_CE/chAdjustWin.cpp \
    NDT_CE/chAlarmCfg.cpp \
    NDT_CE/chAverageWin.cpp \
    NDT_CE/chDisplayWin.cpp \
    NDT_CE/chHeadVCompensateWin.cpp \
    NDT_CE/chHeadVCreatWin.cpp \
    NDT_CE/chHeadVMakeWin.cpp \
    NDT_CE/chMeasureCfgWin.cpp \
    NDT_CE/chMeasureConentWin.cpp \
    NDT_CE/chMenuWin.cpp \
    NDT_CE/chParamWin.cpp \
    NDT_CE/chTempCompensateWin.cpp \
    NDT_CE/chTempCurveCreateWin.cpp \
    NDT_CE/chTemperatureMakeWin.cpp \
    NDT_CE/chWaveSpeedAdjustWin.cpp \
    NDT_CE/chWaveSpeedWin.cpp \
    NDT_CE/chZeroAdjust.cpp \
    NDT_CE/chZeroAdjustDoubleWin.cpp \
    NDT_CE/chZeroAdjustSingleWin.cpp \
    NDT_CE/craftParamWin.cpp \
    NDT_CE/createAvgCurveWin.cpp \
    NDT_CE/createCurveWin.cpp \
    NDT_CE/curveWin.cpp \
    NDT_CE/DisplaySetDlg.cpp \
    NDT_CE/displayWin.cpp \
    NDT_CE/fileManageWin.cpp \
    NDT_CE/fnAdjustWin.cpp \
    NDT_CE/fnBScanWin.cpp \
    NDT_CE/fnCreateDgsWin.cpp \
    NDT_CE/fnDgsCurveWin.cpp \
    NDT_CE/fnDgsMakeWin.cpp \
    NDT_CE/fnFuHeCaiLiaoJianCeWin.cpp \
    NDT_CE/fnHanFengTanShangWin.cpp \
    NDT_CE/fnHuiBoBianMaWin.cpp \
    NDT_CE/fnLieWenCeGaoWin.cpp \
    NDT_CE/fnMenMeiZhanKuanWin.cpp \
    NDT_CE/fnMenuWin.cpp \
    NDT_CE/fnModifyDgsWin.cpp \
    NDT_CE/fnPoKouSheZhiWin.cpp \
    NDT_CE/fnQuantitativeAnalysisWin.cpp \
    NDT_CE/fnReferenceWaveWin.cpp \
    NDT_CE/fnSpecialDisplayWin.cpp \
    NDT_CE/fnSpecialFunctionWin.cpp \
    NDT_CE/fnVideoRecordWin.cpp \
    NDT_CE/fnZhongHouBanTanShangWin.cpp \
    NDT_CE/fnZhuJianTanShangWin.cpp \
    NDT_CE/gateDisplayWin.cpp \
    NDT_CE/gateWin.cpp \
    NDT_CE/hotKeyWin.cpp \
    NDT_CE/IniFile.cpp \
    NDT_CE/IniParse.cpp \
    NDT_CE/logoWin.cpp \
    NDT_CE/makeCurveWin.cpp \
    NDT_CE/modeWin.cpp \
    NDT_CE/modifyAvgCurveWin.cpp \
    NDT_CE/modifyCurveWin.cpp \
    NDT_CE/NDT_CE-old.cpp \
    NDT_CE/NDT_CE.cpp \
    NDT_CE/otherConfigureWin.cpp \
    NDT_CE/ParamSet2Dlg.cpp \
    NDT_CE/ParamSetDlg.cpp \
    NDT_CE/paramWin.cpp \
    NDT_CE/quMianXiuZhengWin.cpp \
    NDT_CE/ReceiveParamDlg.cpp \
    NDT_CE/recvParamWin.cpp \
    NDT_CE/refractAngleWin.cpp \
    NDT_CE/reportFormViewWin.cpp \
    NDT_CE/reportFormWin.cpp \
    NDT_CE/screenCopyWin.cpp \
    NDT_CE/SendParamDlg.cpp \
    NDT_CE/sendParamWin.cpp \
    NDT_CE/stdafx.cpp \
    NDT_CE/tcgCurveWin.cpp \
    NDT_CE/TcpACScan.cpp \
    NDT_CE/TcpCScan.cpp \
    NDT_CE/TcpMonitor.cpp \
    NDT_CE/TcpParam.cpp \
    NDT_CE/TcpParamString.cpp \
    NDT_CE/TcpServer.cpp \
    NDT_CE/tlAdjustWin.cpp \
    NDT_CE/tlMenuWin.cpp \
    NDT_CE/tsMenuWin.cpp \
    NDT_CE/waveSpeedWin.cpp \
    NDT_CE/win_com.cpp