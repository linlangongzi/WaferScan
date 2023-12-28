#ifndef FILETRANSLATE_H
#define FILETRANSLATE_H

#include <QWidget>
#include <QVector>
#include <QSharedPointer>

#define WORD unsigned short
#define BOOL bool

/// 精简版特征数据 WORD 为 unsigned short
typedef struct{
	WORD wXLow16;                   // X方向低16位
	WORD wXHigh16;                  // X方向高16位
	WORD wYLow16;                   // Y方向低16位
	WORD wYHigh16;                  // Y方向高16位
	WORD wZLow16;                   // z方向低16位
	WORD wZHigh16;                  // z方向高16位
	WORD wW1Low16;                  // W1方向低16位
	WORD wW1High16;                 // W1方向高16位
	WORD wALow16;                   // A方向低16位
	WORD wAHigh16;                  // A方向高16位
	WORD wBLow16;                   // B方向低16位
	WORD wBHigh16;                  // B方向高16位
	WORD wW2Low16;                  // W2方向低16位
	WORD wW2High16;                 // W2方向高16位
	WORD wGate1SyncTime;            // 闸门1同步时间
	WORD wGate1MPTime;              // 闸门1内最大波峰值时间
	WORD wGate1MPAmp;               // 闸门1内最大波峰值幅度
	WORD wGate2MPTime;              // 闸门2内最大波峰值时间
	WORD wGate2MPAmp;               // 闸门2内最大波峰值幅度
	WORD wGate2FPFirstTime;         // 闸门2内第一个波峰过阈值时间
	WORD wGate2FPTime;              // 闸门2内第一个波峰波峰值时间
	WORD wGate2FPAmp;               // 闸门2内第一个波峰波峰值幅度
	WORD wGate3MPTime;              // 闸门3内最大波峰值时间
	WORD wGate3MPAmp;               // 闸门3内最大波峰值幅度
	WORD wGate3FPFirstTime;         // 闸门3内第一个波峰过阈值时间
	WORD wGate3FPTime;              // 闸门3内第一个波峰波峰值时间
	WORD wGate3FPAmp;               // 闸门3内第一个波峰波峰值幅度
	WORD wGate4MPTime;              // 闸门4内最大波峰值时间
	WORD wGate4MPAmp;               // 闸门4内最大波峰值幅度
	WORD wGate4FPFirstTime;         // 闸门4内第一个波峰过阈值时间
	WORD wGate4FPTime;              // 闸门4内第一个波峰波峰值时间
	WORD wGate4FPAmp;               // 闸门4内第一个波峰波峰值幅度
} CH_FLAWDATA_CSCAN_S;

class CGateInfo
{
public:
	BOOL m_bOpen;                   // 开启标志int
	double m_fFrontGate;            // 前沿
	double m_fBackGate;             // 宽度
	double m_fPeakGate;             // 峰值
	unsigned long m_colorGate;      // 闸门颜色unsigned long
	int m_nMtpMode;                 // 同步模式
	double m_fMTPPeak;              // 同步阈值
};


class CPicData_L
{
public:
	WORD m_wData[512];              // 521个点A扫数据unsigned short
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fA;
	float m_fB;
	float m_fW;
	float m_fR;
	CH_FLAWDATA_CSCAN_S m_stFlawData;       // 特征数据
	int m_nCardIndex;                       // 卡索引
	int m_nChannelIndex;                    // 通道索引
};

struct HeadStruct
{
    double xStart;
    double yStart;
    double xEnd;
    double yEnd;
    double pipeDiameter;
    int axisSelect;
    float gridX;
    float gridY;
    int gridXSize;
    unsigned long gridSize;
};

namespace Ui {
class FileTranslate;
}

class FileTranslate : public QWidget
{
    Q_OBJECT

public:
    explicit FileTranslate(QWidget *parent = 0);
    ~FileTranslate();

private:
    Ui::FileTranslate *ui;

private:
    void Connect();

private slots:
    void ReadPath();
    void Read();
    void MakePath();

private:
    QString m_ReadPath;
    QString m_MakePath;
    HeadStruct m_HeadStruct[1];
    CGateInfo m_CGateInfo[8];
};

#endif // FILETRANSLATE_H
