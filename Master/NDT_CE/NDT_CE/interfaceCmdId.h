#ifndef _INTERFACE_COMMAND_ID_H_
#define _INTERFACE_COMMAND_ID_H_

#define IDC_TSMENU_MODE			0x1000
#define IDC_TSMENU_PARAM		0X1001
#define IDC_TSMENU_OTHERCFG		0X1002
#define IDC_TSMENU_FILE			0X1003
#define IDC_TSMENU_HOTKEY		0X1004

#define IDC_PARAM_SEND		0x1010
#define IDC_PARAM_RECV		0x1011
#define IDC_PARAM_DISPLAY	0x1012
#define IDC_PARAM_CURVE		0x1013
#define IDC_PARAM_GATE		0x1014

#define IDC_MODE_1OR2		0x1021
#define IDC_MODE_1PLUS2		0x1022
#define IDC_MODE_1TO2		0x1023
#define IDC_MODE_DANJING	0x1024
#define IDC_MODE_SHUANGJING	0x1025

#define IDC_SENDPARAM_VOLTAGE		0x1030
#define IDC_SENDPARAM_MAIKUAN		0x1031
#define IDC_SENDPARAM_REPEATFREQ	0x1032
#define IDC_SENDPARAM_MODIFY		0x1033
#define IDC_SENDPARAM_HEADQIANYAN	0x1034

#define IDC_RECVPARAM_WAVEMODE		0x1040
#define IDC_RECVPARAM_GAIN			0x1041
#define IDC_RECVPARAM_DELAY			0x1042
#define IDC_RECVPARAM_WAVEBAND		0x1043
#define IDC_RECVPARAM_REFERENCEGAIN	0x1044
#define IDC_RECVPARAM_LITTLE_MODIFY	0x1045

#define IDC_DISPLAY_REFRACTMODE     0x1050
#define IDC_DISPLAY_SOUNDDISTANCE   0x1051
#define IDC_DISPLAY_SOUNDSPEED		0x1052
#define IDC_DISPLAY_REFRACTANGLE	0x1053
#define IDC_DISPLAY_RESTRAINT		0x1054

#define IDC_HOTKEY_SELECT			0x1060
#define IDC_HOTKEY_SETTING			0x1061
#define IDC_HOTKEY_DELETE			0x1062

#define IDC_FILEMANAGE_CRAFTPARAM		0x1070
#define IDC_FILEMANAGE_REPORTFORM		0x1071
#define IDC_FILEMANAGE_SCREENCOPY		0x1072

#define IDC_OTHERCFG_COMMUNICATION		0x1080
#define IDC_OTHERCFG_PRINT				0x1081
#define IDC_OTHERCFG_LCD				0x1082
#define IDC_OTHERCFG_VOLUME				0x1083
#define IDC_OTHERCFG_STOREMODE			0x1084

#define IDC_CRAFTPARAM_NUMBER			0x1090
#define IDC_CRAFTPARAM_REVIEW			0x1091
#define IDC_CRAFTPARAM_SAVE				0x1092
#define IDC_CRAFTPARAM_GET				0x1093
#define IDC_CRAFTPARAM_DELETE			0x1094

#define IDC_REPORTFORM_NUMBER			0x10A0
#define IDC_REPORTFORM_SAVE				0x10A1
#define IDC_REPORTFORM_REVIEW			0x10A2
#define IDC_REPORTFORM_PRINT			0x10A3

#define IDC_SCREENCOPY_NUMBER			0x10B0
#define IDC_SCREENCOPY_SAVE				0x10B1
#define IDC_SCREENCOPY_PRINT			0x10B2
#define IDC_SCREENCOPY_DELETE			0x10B3

#define IDC_GATE_SELECT					0x10C0
#define IDC_GATE_ALARMMODE				0x10C1
#define IDC_GATE_SWITCH					0x10C2
#define IDC_GATE_QIANYAN				0x10C3
#define IDC_GATE_HOUYAN					0x10C4
#define IDC_GATE_FUZHI					0x10C5

#define IDC_GATEDISPLAY_DGSGATECFG			0x10C8
#define IDC_GATEDISPLAY_SELECT				0x10C9
#define IDC_GATEDISPLAY_QUEXIANDAXIAO		0x10CA
#define IDC_GATEDISPLAY_SHENGCHENGWEIZHI	0x10CB
#define IDC_GATEDISPLAY_SHENDUWEIZHI		0x10CC
#define IDC_GATEDISPLAY_SHUIPINGWEIZHI		0x10CD

#define IDC_CURVE_TCG					0x10D0
#define IDC_CURVE_DAC					0x10D1
#define IDC_CURVE_AVG					0x10D2
#define IDC_CURVE_QUMIANXIUZHENG		0x10D3

#define IDC_MAKECURVE_CREATE			0x10E0
#define IDC_MAKECURVE_MODIFY			0x10E1

#define IDC_CREATECURVE_ADDPOINT		0x10F0
#define IDC_CREATECURVE_DELETEPOINT		0x10F1
#define IDC_CREATECURVE_END				0x10F2

#define IDC_MODIFYCURVE_ADDPOINT		0x1100
#define IDC_MODIFYCURVE_SELPOINT		0x1101
#define IDC_MODIFYCURVE_MODIFYAMP		0x1102
#define IDC_MODIFYCURVE_DELETEPOINT		0x1103
#define IDC_MODIFYCURVE_END				0x1104

#define IDC_FNMENU_QUANTITATIVEANALYSIS		0x1110
#define IDC_FNMENU_SPECIALDISPLAY			0x1111
#define IDC_FNMENU_SPECIALFUNCTION			0x1112
#define IDC_FNMENU_VIDEORECORD				0x1113
#define IDC_FNMENU_CHANGETYPE				0x1114

#define IDC_SPECIALDISPLAY_DYNAMICINCLUDE	0x1120
#define IDC_SPECIALDISPLAY_FREEZE			0x1121
#define IDC_SPECIALDISPLAY_REFRENCEWAVE		0x1122
#define IDC_SPECIALDISPLAY_HUIBOBIANMA		0x1123
#define IDC_SPECIALDISPLAY_MENMEIZHANKUAN	0x1124
#define IDC_SPECIALDISPLAY_110DISPLAY		0x1125

#define IDC_REFERENCEWAVE_SELECT			0x1130
#define IDC_REFERENCEWAVE_SAVE				0x1131
#define IDC_REFERENCEWAVE_ONOROFF			0x1132

#define IDC_HUIBOBIANMA_SELECT			0x1140
#define IDC_HUIBOBIANMA_GONGJIANHOUDU	0x1141
#define IDC_HUIBOBIANMA_ONOROFF			0x1142

#define IDC_MENMEIZHANKUAN_QIANYAN			0x1150
#define IDC_MENMEIZHANKUAN_HOUYAN			0x1151
#define IDC_MENMEIZHANKUAN_ONOROFF			0x1152

#define IDC_QUANTITATIVEANALYSIS_DGSCURVE		0x1160
#define IDC_QUANTITATIVEANALYSIS_AUTOGAINPLUS	0x1161
#define IDC_QUANTITATIVEANALYSIS_ADJUST			0x1162
#define IDC_QUANTITATIVEANALYSIS_PEAKREMEMBER	0x1163


#define IDC_DGSCURVE_MAKE				0x1171
#define IDC_DGSCURVE_ONOROFF			0x1172
#define IDC_DGSCURVE_QUEXIANDAXIAO		0x1173
#define IDC_DGSCURVE_SHENGCHENGWEIZHI	0x1174
#define IDC_DGSCURVE_SHENDUWEIZHI		0x1175
#define IDC_DGSCURVE_SHUIPINGWEIZHI		0x1176

#define IDC_TCGCURVE_MAKE				0x11A0
#define IDC_TCGCURVE_ONOFF				0x11A1

#define IDC_SPECIALFUNCTION_BSCAN					0x1180
#define IDC_SPECIALFUNCTION_ZHONGHOUBANTANSHANG		0x1181
#define IDC_SPECIALFUNCTION_HANFENGTANSHANG			0x1182
#define IDC_SPECIALFUNCTION_FUHECAILIAOJIANCE		0x1183
#define IDC_SPECIALFUNCTION_ZHUJIANTANSHANG			0x1184
#define IDC_SPECIALFUNCTION_LIEWENCEGAO				0x1185


#define IDC_BSCAN_GANGBANHOUDU		0x1190
#define IDC_BSCAN_SCANMODE			0x1191
#define IDC_BSCAN_GATEQIANYAN		0x1192
#define IDC_BSCAN_GATEHOUYAN		0x1193
#define IDC_BSCAN_GATEFUZHI			0x1194
#define IDC_BSCAN_ONOROFF			0x1195
/*
#define IDC_ZHONGHOUBANTANSHANG_THICKNESS		0x11A0
#define IDC_ZHONGHOUBANTANSHANG_HEADMODEL		0x11A1
#define IDC_ZHONGHOUBANTANSHANG_DACINPUT		0x11A2
#define IDC_ZHONGHOUBANTANSHANG_DACONOROFF		0x11A3
#define IDC_ZHONGHOUBANTANSHANG_UPBLINDAREA		0x11A4
#define IDC_ZHONGHOUBANTANSHANG_DOWNBLINDAREA	0x11A5
#define IDC_ZHONGHOUBANTANSHANG_SENSITIVE		0x11A6
*/
#define IDC_ZHONGHOUBANTANSHANG_THICKNESS		0x11A0
#define IDC_ZHONGHOUBANTANSHANG_DELAY			0x11A1
#define IDC_ZHONGHOUBANTANSHANG_UPBLINDAREA		0x11A2
#define IDC_ZHONGHOUBANTANSHANG_DOWNBLINDAREA	0x11A3
#define IDC_ZHONGHOUBANTANSHANG_SENSITIVE		0x11A4


#define IDC_VIDEORECORD_FILENUMBER		0x11B0
#define IDC_VIDEORECORD_RECORDONOFF		0x11B1
#define IDC_VIDEORECORD_PLAY			0x11B2

#define IDC_HANFENGTANSHANG_POKOUSHEZHI			0x11C0
#define IDC_HANFENGTANSHANG_GANGBANHOUDU		0x11C1
#define IDC_HANFENGTANSHANG_DISTANCE			0x11C2
#define IDC_HANFENGTANSHANG_REFRACTANGLE		0x11C3
#define IDC_HANFENGTANSHANG_ONOROFF				0x11C4

#define IDC_DGSMAKE_TYPE		0x11D0
#define IDC_DGSMAKE_CREATE		0x11D1
#define IDC_DGSMAKE_MODIFY		0x11D2

#define IDC_CREATEDGSCURVE_ADDPOINT		0x11E0
#define IDC_CREATEDGSCURVE_DELETEPOINT	0x11E1
#define IDC_CREATEDGSCURVE_PINGDING		0x11E2
#define IDC_CREATEDGSCURVE_DINGLIANG	0x11E3
#define IDC_CREATEDGSCURVE_PANHUI		0x11E4
#define IDC_CREATEDGSCURVE_END			0x11E5
#define IDC_CREATEDGSCURVE_SELECT		0x11E6
#define IDC_CREATEDGSCURVE_MODIFY_AMP	0x11E7

#define IDC_MODIFYDGSCURVE_ADDPOINT		0x11F0
#define IDC_MODIFYDGSCURVE_DELETEPOINT	0x11F1
#define IDC_MODIFYDGSCURVE_PINGDING		0x11F2
#define IDC_MODIFYDGSCURVE_DINGLIANG	0x11F3
#define IDC_MODIFYDGSCURVE_PANHUI		0x11F4
#define IDC_MODIFYDGSCURVE_END			0x11F5
#define IDC_MODIFYDGSCURVE_SELPOINT		0x11F6
#define IDC_MODIFYDGSCURVE_MODIFYAMP	0x11F7




#define IDC_POKOUSHEZHI_LEIXING			0x1200
#define IDC_POKOUSHEZHI_POKOUJIAODU		0x1201
#define IDC_POKOUSHEZHI_POKOUSHENDU		0x1202

#define IDC_AUTOGAINPLUS_START			0x1210
#define IDC_AUTOGAINPLUS_WAVEHEIGHT		0x1211

#define IDC_WAVESPEED_TYPE				0x1220
#define IDC_WAVESPEED_VALUE				0x1221

#define IDC_ADJUST_HEADTYPE			0x1220
#define IDC_ADJUST_DISTANCE1		0x1221
#define IDC_ADJUST_DISTANCE2		0x1222
#define IDC_ADJUST_OFFSET			0x1223
#define IDC_ADJUST_START			0x1224

#define IDC_REFRACTANGLE_KVALUE		0x1230
#define IDC_REFRACTANGLE_JIAODU		0x1231

#define IDC_CHMENU_MEASURECFG		0x1300
#define IDC_CHMENU_PARAM			0X1301
#define IDC_CHMENU_ADJUST			0X1302
#define IDC_CHMENU_OTHERCFG			0X1303
#define IDC_CHMENU_FILE				0X1304
#define IDC_CHMENU_HOTKEY			0X1305

#define IDC_CHPARAM_SEND			0x1310
#define IDC_CHPARAM_RECV			0X1311
#define IDC_CHPARAM_WAVESPEED		0X1312
#define IDC_CHPARAM_DISPLAY			0X1313

#define IDC_CHMEASURECFG_HEADTYPE		0x1320
#define IDC_CHMEASURECFG_METHOD			0X1321
#define IDC_CHMEASURECFG_CONTENT		0X1322
#define IDC_CHMEASURECFG_ALARMCFG		0X1323

#define IDC_CHADJUST_WAVESPEED			0x1330
#define IDC_CHADJUST_ZERO				0X1331
#define IDC_CHADJUST_TEMPCOMPENSATE		0X1332
#define IDC_CHADJUST_HEADVCOMPENSATE	0X1333

#define IDC_CHALARMCFG_NUMBER			0x1340
#define IDC_CHALARMCFG_MODE				0X1341
#define IDC_CHALARMCFG_CHAOCHAYUZHI		0X1342
#define IDC_CHALARMCFG_SWITCH			0X1343

#define IDC_CHWAVESPEED_STEEL			0x1350
#define IDC_CHWAVESPEED_IRON			0X1351
#define IDC_CHWAVESPEED_ALUMINIUM		0X1352
#define IDC_CHWAVESPEED_SELFDEFINE		0X1353

#define IDC_CHWAVESPEEDADJUST_THICKNESS			0x1360
#define IDC_CHWAVESPEEDADJUST_STANDARD			0X1361
#define IDC_CHWAVESPEEDADJUST_START				0X1362
#define IDC_CHWAVESPEEDADJUST_ADJUSTWAVESPEED	0X1363
#define IDC_CHWAVESPEEDADJUST_ACCEPT			0X1364

#define IDC_CHZEROADJUST_SINGLE				0x1370
#define IDC_CHZEROADJUST_DOUBLE				0x1371

#define IDC_CHTEMPCOMPENSATE_MATERIAL		0x1380
#define IDC_CHTEMPCOMPENSATE_TEMPERATURE	0x1381
#define IDC_CHTEMPCOMPENSATE_CURVEMAKE		0x1382
#define IDC_CHTEMPCOMPENSATE_SWITCH			0x1383

#define IDC_CHDISPLAY_SWITCH				0x1390
#define IDC_CHDISPLAY_MAX					0x1391
#define IDC_CHDISPLAY_MIN					0x1392
#define IDC_CHDISPLAY_AVERAGE				0x1393
#define IDC_CHDISPLAY_CURRENT				0x1394
#define IDC_CHDISPLAY_CHAZHI				0x1395
#define IDC_CHDISPLAY_TUCENG				0x1396

#define IDC_CHMEASURECONTENT_DISPLAY		0x13A0
#define IDC_CHMEASURECONTENT_ONOFF			0X13A1
#define IDC_CHMEASURECONTENT_STANDARD		0X13A2
#define IDC_CHMEASURECONTENT_MODE			0X13A3
#define IDC_CHMEASURECONTENT_TUCENGMATERIAL	0X13A4
#define IDC_CHMEASURECONTENT_TUCENGSPEED	0X13A5

#define IDC_CHZEROSINGLE_THICKNESS			0X13B0
#define IDC_CHZEROSINGLE_START				0X13B1
#define IDC_CHZEROSINGLE_RESULT				0X13B2
#define IDC_CHZEROSINGLE_ACCEPT				0X13B3

#define IDC_CHZERODOUBLE_MINTHICKNESS		0X13C0
#define IDC_CHZERODOUBLE_MINADJUST			0X13C1
#define IDC_CHZERODOUBLE_MAXTHICKNESS		0X13C2
#define IDC_CHZERODOUBLE_START				0X13C3
#define IDC_CHZERODOUBLE_RESULT				0X13C4
#define IDC_CHZERODOUBLE_ACCEPT				0X13C5

#define IDC_CHHEADVCOMPENSATE_HEADNUM		0x13D0
#define IDC_CHHEADVCOMPENSATE_CURVEMAKE		0x13D1
#define IDC_CHHEADVCOMPENSATE_SWITCH		0x13D2

#define IDC_HEADVMAKE_CURVENUM				0x13E0
#define IDC_HEADVMAKE_CREATE				0x13E1
#define IDC_HEADVMAKE_MODIFY				0x13E2

#define IDC_CHHEADVCREATE_ACTUALHOUDU		0x13F0
#define IDC_CHHEADVCREATE_MEASUREHOUDU		0x13F1
#define IDC_CHHEADVCREATE_ADDPOINT			0x13F2
#define IDC_CHHEADVCREATE_DELETEPOINT		0x13F3
#define IDC_CHHEADVCREATE_END				0x13F4

#define IDC_TEMPCURVEMAKE_CURVENUM			0x1400
#define IDC_TEMPCURVEMAKE_CREATE			0x1401
#define IDC_TEMPCURVEMAKE_MODIFY			0x1402

#define IDC_CHTEMPCURVECREATE_HOUDU			0x1410
#define IDC_CHTEMPCURVECREATE_TEMP			0x1411
#define IDC_CHTEMPCURVECREATE_MEASURESPEED	0x1412
#define IDC_CHTEMPCURVECREATE_ADDPOINT		0x1413
#define IDC_CHTEMPCURVECREATE_DELETEPOINT	0x1414
#define IDC_CHTEMPCURVECREATE_END			0x1415

#define IDC_CHAVERAGE_CURRENT1			0x1420
#define IDC_CHAVERAGE_CURRENT2			0x1421
#define IDC_CHAVERAGE_CURRENT3			0x1422
#define IDC_CHAVERAGE_CURRENT4			0x1423
#define IDC_CHAVERAGE_CURRENT5			0x1424
#define IDC_CHAVERAGE_CURRENT6			0x1425

#define IDC_ZHUJIAN_HOUDU				0x1430
#define IDC_ZHUJIAN_UPBLINDAREA			0x1431
#define IDC_ZHUJIAN_DOWNBLINDAREA		0x1432
#define IDC_ZHUJIAN_SENSITIVE			0x1433
#define IDC_ZHUJIAN_DEFECTZHI			0x1434

#define IDC_QUMIANXIUZHENG_ONOFF		0x1440
#define IDC_QUMIANXIUZHENG_WAIJING		0x1441
#define IDC_QUMIANXIUZHENG_NEIJING		0x1442

#define IDC_AVGCURVE_MAKE				0x1450
#define IDC_AVGCURVE_SWITCH				0x1451
#define IDC_AVGCURVE_DEFECTZHI1			0x1452
#define IDC_AVGCURVE_DEFECTZHI2			0x1453
#define IDC_AVGCURVE_DEFECTZHI3			0x1454

#define IDC_MAKEAVGCURVE_TYPE			0x1460
#define IDC_MAKEAVGCURVE_CREATE			0x1461
#define IDC_MAKEAVGCURVE_MODIFY			0x1462

#define IDC_CREATEAVGCURVE_ADDPOINT		0x1470
#define IDC_CREATEAVGCURVE_DELETEPOINT	0x1471
#define IDC_CREATEAVGCURVE_END			0x1472

#define IDC_MODIFYAVGCURVE_ADDPOINT		0x1476
#define IDC_MODIFYAVGCURVE_SELPOINT		0x1477
#define IDC_MODIFYAVGCURVE_MODIFYAMP	0x1478
#define IDC_MODIFYAVGCURVE_DELETEPOINT	0x1479
#define IDC_MODIFYAVGCURVE_END			0x147A




#define IDC_FUHECAILIAOJIANCE_THICKNESS		0x1480
#define IDC_FUHECAILIAOJIANCE_UPBLINDAREA	0x1481
#define IDC_FUHECAILIAOJIANCE_DOWNBLINDAREA	0x1482
#define IDC_FUHECAILIAOJIANCE_SENSITIVE		0x1483
#define IDC_FUHECAILIAOJIANCE_SPEEDCHAOCHA	0x1484
#define IDC_FUHECAILIAOJIANCE_WAVESPEED		0x1485

#define IDC_LIEWENCEGAO_UPDOWNPOINT			0x1490
#define IDC_LIEWENCEGAO_CACULATEAGAIN		0x1491
#define IDC_LIEWENCEGAO_HEIGHT				0x1492

#define IDC_TLADJUST_WAVESPEEDZERO			0X1600
#define IDC_TLADJUST_DACWAVE				0X1601
#define IDC_TLADJUST_SENSITIVE				0X1602
#define IDC_TLADJUST_BIAOMIANBUCHANG		0X1603
#define IDC_TLADJUST_OTHER					0X1604

#define IDC_TLMENU_TLMENUSTANDARD			0X1610
#define IDC_TLMENU_POSITION					0X1611
#define IDC_TLMENU_ADJUST					0X1612
#define IDC_TLMENU_COMMON_TEST				0X1613



#define IDC_ALLPARAMCFG_GONGYICANSHU		0X1800
#define IDC_ALLPARAMCFG_VOLTAGE				0X1801
#define IDC_ALLPARAMCFG_MAIKUAN				0X1802
#define IDC_ALLPARAMCFG_RATE				0X1803
#define IDC_ALLPARAMCFG_AMEND				0X1804
#define IDC_ALLPARAMCFG_JIANBO				0X1805
#define IDC_ALLPARAMCFG_PLUS				0X1806
#define IDC_ALLPARAMCFG_REFPLUS				0X1807
#define IDC_ALLPARAMCFG_DELAY				0X1808
#define IDC_ALLPARAMCFG_LVBO				0X1809
#define IDC_ALLPARAMCFG_REFRACTMODE			0X180A
#define IDC_ALLPARAMCFG_SPEEDTYPE			0X180B
#define IDC_ALLPARAMCFG_SPEED				0X180C
#define IDC_ALLPARAMCFG_RANGE				0X180D
#define IDC_ALLPARAMCFG_ANGLE				0X180E
#define IDC_ALLPARAMCFG_KVALUE				0X180F
#define IDC_ALLPARAMCFG_BATE				0X1810
#define IDC_ALLPARAMCFG_TIMEYEAR			0X1811
#define IDC_ALLPARAMCFG_TIMEMONTH			0X1812
#define IDC_ALLPARAMCFG_TIMEDAY				0X1813
#define IDC_ALLPARAMCFG_TIMEHOUR			0X1814
#define IDC_ALLPARAMCFG_TIMEMINUTE			0X1815
#define IDC_ALLPARAMCFG_TIMESECOND			0X1816
#define IDC_ALLPARAMCFG_GONGJIANHOUDU		0X1817
#define IDC_ALLPARAMCFG_SCANMODE			0X1818
#define IDC_ALLPARAMCFG_BSCANONOROFF		0X1819
#define IDC_ALLPARAMCFG_PLATEHEIGHT			0X1820
#define IDC_ALLPARAMCFG_PROBETYPE			0X1821
#define IDC_ALLPARAMCFG_UPBLINDVALUE		0X1822
#define IDC_ALLPARAMCFG_DOWNBLINDVALUE		0X1823
#define IDC_ALLPARAMCFG_SENSITIVE			0X1824
#define IDC_ALLPARAMCFG_POKOUTYPE			0X1825
#define IDC_ALLPARAMCFG_POKOUANGLE			0X1826
#define IDC_ALLPARAMCFG_POKOUDEEP			0X1827
#define IDC_ALLPARAMCFG_HANFENGTANSHANGHOUDU 0X1828
#define IDC_ALLPARAMCFG_HANFENGTANSHANGDISTANCE 0X1828
#define IDC_ALLPARAMCFG_HANFENGTANSHANGONOROFF 0X1829
#define IDC_ALLPARAMCFG_LIANGDU				0X1830
#define IDC_ALLPARAMCFG_ZUKANGPIPEI			0X1831
#define IDC_ALLPARAMCFG_QUXIANFENDUAN		0X1832
#define IDC_ALLPARAMCFG_PINPUFENXI			0X1833
#define IDC_ALLPARAMCFG_XINGNENGJIAOYAN		0X1834
#define IDC_ALLPARAMCFG_JULIZUOBIAO			0X1835
#define IDC_ALLPARAMCFG_GUANCAIWAIJING		0X1836
#define IDC_ALLPARAMCFG_GUANCAINEIJING		0X1837
#define IDC_ALLPARAMCFG_XIANGYINGSPEED		0X1838
#define IDC_ALLPARAMCFG_BIAOMIANBUCHANG		0X1839
#define IDC_ALLPARAMCFG_PRINTTYPE			0X1840
#define IDC_ALLPARAMCFG_LIEWENQIBO			0X1841
#define IDC_ALLPARAMCFG_PINGDING			0X1842
#define IDC_ALLPARAMCFG_DINGLIANG			0X1843
#define IDC_ALLPARAMCFG_PANFEI				0X1844
#define IDC_ALLPARAMCFG_TANTOUTYPE			0X1845
#define IDC_ALLPARAMCFG_CANSHUQINLING		0X1846
#define IDC_ALLPARAMCFG_TONGDAOQINLING		0X1847
#define IDC_ALLPARAMCFG_CANSHUSUODING		0X1848
#define IDC_ALLPARAMCFG_QUXIANGUIGE 		0X1849
#define IDC_ALLPARAMCFG_ZIDONGBOGAO			0X1850
#define IDC_ALLPARAMCFG_PLUSTYPE			0X1851
#define IDC_ALLPARAMCFG_WORKTYPE			0X1852
#define IDC_ALLPARAMCFG_HARDWARE			0X1853
#define IDC_ALLPARAMCFG_SOFTWARE			0X1854
#define IDC_ALLPARAMCFG_EQUIPMENTMODEL		0X1855
#define IDC_ALLPARAMCFG_QIANYAN				0X1856

#define IDC_ALLPARAMCFG_NEXTADJUSTTIME		0X1857
#define IDC_ALLPARAMCFG_THISADJUSTTIME		0X1858


#define IDC_ALLPARAMCONFIRM_SAVE			0x1900
#define IDC_ALLPARAMCONFIRM_NOSAVE			0x1901
#define IDC_ALLPARAMCONFIRM_CANCEL			0x1902

#define IDL_MODE_TEST		0x2000

#define IDL_SENDPARAM_VOLTAGE		0x2001
#define IDL_SENDPARAM_MAIKUAN		0x2002
#define IDL_SENDPARAM_REPEATFREQ	0x2003
#define IDL_SENDPARAM_MODIFY		0x2004

//#define IDL_RECVPARAM_HEADFREQ		0X2010
#define IDL_RECVPARAM_WAVEMODE		0X2011
#define IDL_RECVPARAM_GAIN			0X2012
#define IDL_RECVPARAM_DELAY			0X2013
#define IDL_RECVPARAM_WAVEBAND		0X2014
#define IDL_RECVPARAM_REFERENCEGAIN 0X2015
#define IDL_RECVPARAM_LITTLE_MODIFY 0X2016

#define IDL_DISPLAY_REFRACTMODE     0x2020
#define IDL_DISPLAY_SOUNDDISTANCE	0x2021
#define IDL_DISPLAY_SOUNDSPEED		0x2022
#define IDL_DISPLAY_REFRACTANGLE	0x2023
#define IDL_DISPLAY_RESTRAINT		0x2024

#define IDL_HOTKEY_SELECT			0x2030

#define IDL_OTHERCFG_PRINT			0x2040
#define IDL_OTHERCFG_LCD			0x2041
#define IDL_OTHERCFG_VOLUME			0x2042
#define IDL_OTHERCFG_STOREMODE		0x2043

#define IDL_CRAFTPARAM_NUMBER		0x2050

#define IDL_REPORTFORM_NUMBER		0x2060

#define IDL_SCREENCOPY_NUMBER		0x2070

#define IDL_GATE_SELECT					0x2080
#define IDL_GATE_ALARMMODE				0x2081
#define IDL_GATE_SWITCH					0x2082
#define IDL_GATE_QIANYAN				0x2083
#define IDL_GATE_HOUYAN					0x2084
#define IDL_GATE_FUZHI					0x2085

#define IDL_CREATECURVE_ADDPOINT			0x2090
#define IDL_CREATECURVE_DELETEPOINT		0x2091

#define IDL_CURVE_TYPE					0x20A0
#define IDL_CURVE_ONOFF					0x20A1

#define IDL_MODIFYCURVE_ADDPOINT		0x20B0
#define IDL_MODIFYCURVE_DELETEPOINT		0x20B1
#endif //_INTERFACE_COMMAND_ID_H_
