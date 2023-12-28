/**
* Copyright(c) 2014 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 数据库文件结构定义
* Filename: CAScanLock.h
*
* @author        Chen Yuzhu
* @version       1.00         Date: 2014-07-1
*/

//钢板参数结构体
//#pragma pack(push,1)//对齐
typedef struct platefile
{
	int nId;				//编号
	char strPlateType[50];	//钢板号
	char strLu[50];			//炉号
	char strMuBan[50];		//模板号（钢坯号）
	char strShenChan[50];	//生产号（批号）
	char strHeTong[50];		//合同号
	char strGangJi[50];		//钢级
	char strJiaoHuoStatus[50];//交货状态
	char  strName[50];		//钢板号
	char strGangZhong[50];//钢种
	
	float fLength;			//板长（mm）
	float fWidth;			//板宽
	float fThick;			//板厚
	float fLWaveSpeed;		//纵波声速（m/s）
	float fTWaveSpeed;		//横波声速
	float fWeight;			//重量（吨）
	char fInfo[128];		//规格
	int nflag;				//判断是否是试板1为试板，0为非试板
	char strThbh[50];		//
	char strKuaiShu[50];	//块数
	char strBeiZhu[50];		//备注
	int nSdbs;				//是否是人工合计

}PLATEFILE_PARAM;


//钢板探伤结果结构体
typedef struct platecheckinfo
{
	int nId;				//序列号
	char strCheckTime[50];	//检测时间
	char strCheckClass[16]; //检测班次
	char strCheckName[16];	//检测员
	int nPlate_id;          //钢板编号
	char strCheckMethod[100];//探伤工艺
	char strName[50];		//探伤模式（探伤工艺）
	char strResult[10];		//探伤结果
	int nMianShangCount;	//面伤总数
	float fMianShangAllMianJi;//面伤总面积
	float fQueXianPercent;	//缺陷比例
	float fQueXianMaxLength;//通用最大指示长度（cm）
	float fQuexianMaxWidth; //通用最大指示宽度（cm）
	char strDiscript;		//探伤资格内容
	int nDianShangCount;	//点伤总数
	char strCheckBodyLevel[16];//本体探伤级别
	char strCheckEdgeLevel[16];//边缘探伤级别
	unsigned int bIsReEvaluate:1;         //是否重评
	char strCheckBiaoZhun[50];	//探伤标准
	float fAllMaxQueXianLv; //通用缺陷率
	float fQueXianMaxArea;  //通用缺陷最大面积cm^2
	float fBodyMaxZhiShiLength;//本体最大指示长度
	float fBodyMaxQueXianarea;//本体最大缺陷面积
	float fBodyMaxLength;//本体最大指示长度cm
	float fBodyMaxWidth;//本体最大宽度cm
	float fEdgeMaxZhiShiLength;//边缘最大指示长度
	float fEdgeMaxArea;//边缘最大面积
	float fEdgeMaxLength;//边缘最大实际长度cm
	float fEdgeMaxWidth;//边缘最大实际宽度cm
	float fBodyAvgQueXianLv;//本体平均缺陷率%
	float fEdgeAvgQueXianLv;//边缘平均缺陷率%
	float fBodyMaxQueXianLv;//本体最大缺陷率%
	float fEdgeMaxQueXianLv;//边缘最大缺陷率%
	float fBodyMaxCountSquarMeter; //本体最大单位面积缺陷个数/m^2
	float fEdgeMaxCountPerMeter;//边缘最大单位长度缺陷个数/m
	float fAllMaxCountSquarMeter;//通用最大单位面积缺陷个数/m^2
	float fBodyYanZhongQueXianNum;//本体严重缺陷数
	float fEdgeYanZhongQueXianNum;//边缘严重缺陷数
	int  nFlag;

}PLATECHECKINFOFILE;

//设备通道参数
typedef struct channel_param
{
	int nId;			//通道流水号
	int nNum;			//物理通道号
	char strTangShangGongYi[50];//探伤工艺
	char strDetector[50];	//探头型号
	int nZhongZhiDian;	//中值点
	char strIsOpen[50];	//通道开启
	float fICIV;			//底波初始增益值(db)
	float fTCIV;
	float fDelay;		//延迟（mm）
	float fShangmangqu;// 上盲区 fHuaMenQianYan;//闸门前沿（mm）
	float fXiamangqu;// 下盲区fHuaMenHouYan;//闸门后沿（mm）
	float fDibozhamenkuandu;// 底播闸门宽度fDiBoHuaMenHouYan;//底波闸门后沿（mm）
	float fDiBoYuZhi;		//底波阀值（%）
	float fYiZhi;			//抑制（%）
	int nRefNum;			//逻辑通道号
	float fShangDiCha;		//伤波增益差值（db）
	int nDemarcateResult;	//
	float fZengyixishu;		//增益系数
}CHANNEL_PARAM;

typedef struct channel_param_old
{
	int nId;			//通道流水号
	int nNum;			//物理通道号
	char strTangShangGongYi[50];//探伤工艺
	char strDetector[50];	//探头型号
	int nZhongZhiDian;	//中值点
	char strIsOpen[50];	//通道开启
	float fICIV;			//底波初始增益值(db)
	float fTCIV;
	float fDelay;		//延迟（mm）
	float fShangmangqu;// 上盲区 fHuaMenQianYan;//闸门前沿（mm）
	float fXiamangqu;// 下盲区fHuaMenHouYan;//闸门后沿（mm）
	float fDibozhamenkuandu;// 底播闸门宽度fDiBoHuaMenHouYan;//底波闸门后沿（mm）
	float fDiBoYuZhi;		//底波阀值（%）
	float fYiZhi;			//抑制（%）
	int nRefNum;			//逻辑通道号
	float fShangDiCha;		//伤波增益差值（db）
	int nDemarcateResult;	//
}CHANNEL_PARAM_OLD;

//钢板型号
typedef struct platetype
{
	int nId;			//编号
	char strName[50];	//钢板号
	float fLength;		//板长（mm）
	float fWidth;		//板宽（mm）
	float fThick;		//板厚（mm）
	float fLWaveSpeed;	//纵波声速（m/s）
	float fTWaveSpeed;	//横波声速（m/s）
	float fWeight;		//重量（吨）
	char strInfo[128];
}PLATE_TYPE;

//探伤工艺
typedef struct tanshanggongyi
{
	int nId;			//编号
	char strName[50];	//工艺名称
	char strPlateType[50];//钢板规格
	char strYiQiXingHao[50];//仪器型号
	char strDetector[50];		//探头型号
	char strTanShangMethod[50];//探伤方法
	char strTangShangBiaoZhun[50];//探伤标准
	char strTanShangJiBieBody[16]; //本体探伤级别
	char strTanShangJiBieEdge[16];//边缘探伤级别
	char strTanShangLinMingDu[50];//探伤灵敏度
	char strJianBoMoShi[50];	//检波模式
	float fShangMangQu;			//上盲区（mm）
	float fXiaMangQu;			//下盲区（mm）
	float fTouWeiMangQu;		//头/尾盲区（mm）
	float fCeMangQu;			//侧边盲区（mm）
	char strTouWeiTian;			//头尾探开关
	char strBianTan;			//边探开关
	int nNetTanTongDaoShu;		//内探通道数
	char strDACOpen[16];		//DAC开启
	char strAuToZengYiOpen[16];	//自动增益开启
	char strAutoHuaMenOpen[16];	//自动闸门跟踪
	char strBaoJing[16];		//报警开关
	char strOuHeJi[50];			//耦合剂
	float fHouDuChaoCha;		//厚度超差允许百分比（%）
	char strHouDuAutoRead[10];	//厚度自动读取开关
	char strDuiBiShiYang[64];	//对比式样型号
	float fChuShiZengYi;		//初始增益（dB）
	float fDiBoYuZhi;			//底波阈值(%)
	float fChuShiYiZhi;			//初始抑制（%）
	float fShaoMiaoJianGe;		//扫描间隔（mm）
	char strModel[10];			//探伤模式
	int nflag;
}TANSHANGGONGYI;

//设备参数_探头参数
typedef struct detector_param
{
	int nId;		//探头编号
	char strType[50];	//探头类型
	float fZhongXinPinLv; //中心频率（MHZ）
	float fLenght;	//长度(mm)
	float fWidth;	//宽度(mm)
	float fYouXiaoWidth;	//有效宽度(mm)
	float fK_Value;	//K值
	float fMax_P_Distance;	//最大正偏差(mm)
	float fMax_N_Distance;	//最大负偏差(mm)
	float fCeHouBuChang;	//测厚补偿系数
	int nDAC_id;	//DAC补偿系数
	float fDelay;	//延迟块长度（mm）
	char strName[50];	//探头型号
	int flag;
}DETECTOR_PARAM;

typedef struct yiqi_param
{
	int nId;	//编号
	char strName[50];	//仪器型号
	float fTanTouZhongXinJuLi;	//探头中心距离(mm)
	int nTouWeiTanTongDaoShu;	//头尾探通道数
	int nTouWeiTanLogicBegin;	//头尾探逻辑通道起始编号
	int nBianTan1TongDaoShu;	//参考边通道数
	int nBianTan1LogicBegin;	//参考边逻辑通道起始编号
	int nNeiTan1TongDaoShu;		//内探1通道数
	int nNeiTan1LogicBegin;		//内探1逻辑通道起始编号
	int nNeiTan2TongDaoShu;		//内探2通道数
	int nNeiTan2LogicBegin;		//内探2逻辑通道起始编号
	float fDistanceTanCha2NeiTanYi1;	//内探拨叉1到内探1距离(mm)
	float fDistanceTanCha2NeiTanYi2;	//内探拨叉2到内探2距离(mm)
	float fDistanceTanCha2BianTan;	//内探拨叉3到内探3距离(mm)
	float fDistanceTanCha2DaoLun;	//内探拨叉4到内探4距离(mm)
	int nADCaiYangShu;	//	AD采样位数
	float fADCFreq;	//	AD采样速度(MHZ)
	int nWuLiTongDaoShu;	//	物理通道数
	float fCengXiangSaoChaMaxDistance;	//	纵向扫查最大间距(mm)
	int nTouWeiTanWuLiBegin;//	头尾探物理通道起始编号
	int nBianTan1WuLiBegin;	//参考边物理通道起始编号
	int nBianTan2TongDaoShu;	//非参考边通道数
	int nBianTan2LogicBegin;	//非参考边逻辑通道起始编号
	int nBianTan2WuLiBegin;	//非参考边物理通道起始编号
	int nNeiTan1WuLiBegin;	//内探1物理通道起始编号
	int nNeiTan2WuLiBegin;	//内探2物理通道起始编号
	float fCeChang1;	//测长编码器1直径(mm)
	float fCeChang2;	//测长编码器2直径(mm)
	float fCeChang3;	//测长编码器3直径(mm)
	float fBianTanXiaoChe1;	//边探小车1编码器直径(mm)
	float fBianTanXiaoChe2;	//边探小车2编码器直径(mm)
	float fTouTanMangQu;	//头探盲区(mm)
	float fWeiTanMangQu;	//尾探盲区(mm)
	float fLeftTanMangQu;	//左边探盲区(mm)
	float fRightTanMangQu;	//右边探盲区(mm)
	float fLeftTanNeiMangQu;	//左边内探盲区(mm)
	float fRightTanNeiMangQu;	//右边内探盲区(mm)
	int nNeiTan3TongDaoShu;	//内探3通道数
	int nNeiTan3LogicBegin;	//内探3逻辑通道起始编号
	int nNeiTan3WuLiBegin;	//内探3物理通道起始编号
	int nNeiTan4TongDaoShu;	//内探4通道数
	int nNeiTan4LogicBegin;	//内探4逻辑通道起始编号
	int nNeiTan4WuLiBegin;	//内探4物理通道起始编号
	char strTanShangMethod[50];	//	探伤方法
	char strOuHeJi[50];	//耦合剂
	float fDistanceTanCha52NeiTanYi1;	//	内探拨叉5到内探1距离(mm)
	int nFlag;	

}YIQI_PARAM;




//设备参数文件
typedef struct shebeicanshu
{
	CHANNEL_PARAM	ChannelParam[500];	// 通道参数
	DETECTOR_PARAM	DetectorParam;	 //设备参数_探头参数
	PLATE_TYPE   Plate_Type;		//钢板型号
	TANSHANGGONGYI TanShangGongYi;	//探伤工艺
	YIQI_PARAM	YiQiParam;
}SHEBEICANSHU;


//老的设备参数文件
typedef struct shebeicanshu_old
{
	CHANNEL_PARAM_OLD	ChannelParam[500];
	DETECTOR_PARAM	DetectorParam;
	PLATE_TYPE   Plate_Type;
	TANSHANGGONGYI TanShangGongYi;
	YIQI_PARAM	YiQiParam;
}SHEBEICANSHU_OLD;