#ifndef _NETCMD_H_
#define _NETCMD_H_

const DWORD NETHEAD_SIZE = sizeof(PACKET_HEAD_t);

const WORD CMD_DQTD = 0x0001;
const DWORD PARAM_DQTD_LEN = sizeof(WORD);

const WORD CMD_GZMS = 0x0011;
const DWORD PARAM_GZMS_LEN = sizeof( WORKMODE);

const WORD CMD_FSCS = 0x0101;
//const DWORD PARAM_FSCS_LEN = sizeof( SEND_PARAM_DATA);
const WORD CMD_FSCS_DY = 0x0102;
const DWORD PARAM_FSCS_DY_LEN = sizeof(WORD);
const WORD CMD_FSCS_MK = 0x0103;
const DWORD PARAM_FSCS_MK_LEN = sizeof(WORD);
const WORD CMD_FSCS_CFPL = 0x0104;
const DWORD PARAM_FSCS_CFPL_LEN = sizeof(DWORD);
const WORD CMD_FSCS_LD = 0x0105;
const DWORD PARAM_FSCS_LD_LEN = sizeof(float);
const WORD CMD_FSCS_QY = 0x0106;
const DWORD PARAM_FSCS_QY_LEN = sizeof(float);

const WORD CMD_JSCS = 0x0201;
//const DWORD PARAM_JSCS_LEN = sizeof( RECEIVE_PARAM_DATA);
const WORD CMD_JSCS_JBFS = 0x0202;
const DWORD PARAM_JSCS_JBFS_LEN = sizeof(WORD);
const WORD CMD_JSCS_ZY = 0x0203;
const DWORD PARAM_JSCS_ZY_LEN = sizeof(float);
const WORD CMD_JSCS_CKZY = 0x0204;
const DWORD PARAM_JSCS_CKZY_LEN = sizeof(float);
const WORD CMD_JSCS_YC = 0x0205;
const DWORD PARAM_JSCS_YC_LEN = sizeof(float);
const WORD CMD_JSCS_GZPL = 0x0206;
const DWORD PARAM_JSCS_GZPL_LEN = sizeof(WORD);
const WORD CMD_JSCS_ZK = 0x0207;
const DWORD PARAM_JSCS_ZK_LEN = sizeof(WORD);

const WORD CMD_XSCS = 0x0301;
//const DWORD PARAM_XSCS_LEN = sizeof( DISPLAY_SET_DLG_DATA);
const WORD CMD_XSCS_SX = 0x0302;
const DWORD PARAM_XSCS_SX_LEN = sizeof( BOOL);
const WORD CMD_XSCS_SC = 0x0303;
const DWORD PARAM_XSCS_SC_LEN = sizeof( float);
const WORD CMD_XSCS_SS = 0x0304;
const DWORD PARAM_XSCS_SS_LEN = sizeof( WORD);
const WORD CMD_XSCS_ZSJ = 0x0305;
const DWORD PARAM_XSCS_ZSJ_LEN = sizeof( float);
const WORD CMD_XSCS_YZ = 0x0306;
const DWORD PARAM_XSCS_YZ_LEN = sizeof( WORD);

const WORD CMD_ZM = 0x0401;
//const DWORD PARAM_ZM_LEN = sizeof( GATE_INFO);
const WORD CMD_ZM_BJMS = 0x0402;
//const DWORD PARAM_ZM_BJMS_LEN = sizeof( alarm_modes);
const WORD CMD_ZM_ZMKG = 0x0403;
const DWORD PARAM_ZM_ZMKG_LEN = sizeof( BOOL);
const WORD CMD_ZM_QD = 0x0404;
const DWORD PARAM_ZM_QD_LEN = sizeof( float);
const WORD CMD_ZM_KD = 0x0405;
const DWORD PARAM_ZM_KD_LEN = sizeof( float);
const WORD CMD_ZM_FZ = 0x0406;
const DWORD PARAM_ZM_FZ_LEN = sizeof( float);

const WORD CMD_QTCS = 0x0501;
//const DWORD PARAM_QTCS_LEN = sizeof( OTHER_PARAM_t);
const WORD CMD_QTCS_YS = 0x0502;
const DWORD PARAM_QTCS_YS_LEN = sizeof( int);
const WORD CMD_QTCS_BG = 0x0503;
const DWORD PARAM_QTCS_BG_LEN = sizeof( int);
const WORD CMD_QTCS_YL = 0x0504;
const DWORD PARAM_QTCS_YL_LEN = sizeof( int);
const WORD CMD_QTCS_CCJZ = 0x0505;
const DWORD PARAM_QTCS_CCJZ_LEN = sizeof( int);

const WORD CMD_SET_ALL_PARAM = 0x0601;
//const DWORD PARAM_SET_ALL_PARAM_lEN = sizeof( WIRELESS_ALL_PARAM_t);
const WORD CMD_GET_ALL_PARAM = 0x1001;
const WORD CMD_GET_ALL_PARAM_RET = 0x2002;

const WORD CMD_ASCAN = 0x1002;
const WORD CDM_ASCAN_RET_WAVE = 0x2001;
const WORD CDM_ASCAN_RET_PEAK = 0x2003;
//const DWORD DATA_ASCAN_RET_LEN = sizeof( REAL_SEND_DATA_t);
//const DWORD DATA_PEAK_RET_LEN = sizeof( PEAK_INFO);

const DWORD MAX_BUF_LEN = 1024;
const DWORD MIN_BUF_LEN = 6;

const DWORD CMD_SET_PARAM_RET = 0xff01;

#define COMMAND_RESET				0x0701	//上位机复位请求
#define COMMAND_VERSION_REQ			0x1003	//版本号请求
#endif