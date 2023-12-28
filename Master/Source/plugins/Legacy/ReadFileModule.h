#pragma once

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <Ultrasonic/UTData.h>
#include <QVector>

const int CHANNEL_NUM = 8;
const int GATE_NUM = 4;

typedef int BOOL;

struct GateInfo
{
    BOOL m_bOpen;                                                               // 开启标志
    double m_fFrontGate;                                                        // 前沿
    double m_fBackGate;                                                         // 宽度
    double m_fPeakGate;                                                         // 峰值
    qint32 m_colorGate;                                                         //闸门颜色
    int m_nMtpMode;                                                             //同步模式
    double m_fMTPPeak;                                                          //同步阈值
};

struct ScanFileHeader
{
    float m_fRangeX;                                                            // 扫查架X范围
    float m_fRangeY;                                                            // 扫查架Y范围
    GateInfo m_GateInfo[CHANNEL_NUM][4];                                        // 闸门数据
    BOOL m_bMPFTimeDisplay;                                                     // 最高波过阈值时间
    BOOL m_bMPMTimeDisplay;                                                     // 最高波过伤时
    BOOL m_bMPMAltitudeDisplay;                                                 // 最高波高度
    BOOL m_bFPFTimeDisplay;                                                     // 一次波过阈值时间
    BOOL m_bFPMTimeDisplay;                                                     // 一次波伤时
    BOOL m_bFPMAltitudeDisplay;                                                 // 一次波高度
    float m_fThick;                                                             // 工件厚度
    float m_fGridX;                                                             // X网格间隔
    float m_fGridY;                                                             // Y网格间隔
    qint16 m_wWaveSpeed;                                                        // 声速
    qint16 m_wChannelNum;                                                       // 通道个数
    float m_dXStart;                                                            // 起始位置  （原点位置）
    float m_dYStart;
    float m_dZStart;
    float m_dXEnd;
    float m_dYEnd;
    float m_dZEnd;
    int m_nAxisSelect;                                                          // 成像平面    0：xy  1:xz  2:yz  3/4 xr(R棒/R管)  5暂时不做处理
    double m_dPipeDiameter;                                                     // 直径
    int m_nProbeNum;                                                            // 探头个数
    int m_nScanType;                                                            // 扫查方式：并行或深度
    int m_nFillType;                                                            // 填充方式：纵向或横向
    int m_nTwoProbeIndex;                                                       // 两个探头组合方式
    float m_fProbeDistance;                                                     // 探头间距
    float m_fChongDie;                                                          // 重叠长度
    int m_nWorkMode;                                                            //工作模式
};

class FeatureInfo
{
public:
    FeatureInfo()
    {
        m_dX = 0.0;
        m_dY = 0.0;
        m_dZ = 0.0;
        m_dR = 0.0;
        m_dA = 0.0;
        m_dB = 0.0;
        m_dW = 0.0;

        m_nGateNum = 1;
        m_nFeatureNum = 1;

        m_nCount = 0;

        m_pfFeatures = new float *[m_nGateNum];
        if (NULL != m_pfFeatures)                                               //add by wangqianfeng on 20121106
        {
            for (int j = 0; j < m_nGateNum; j++)
            {
                m_pfFeatures[j] = NULL;
            }
        }
        for (int i = 0; i < m_nGateNum; i++)
        {
            m_pfFeatures[i] = new float[m_nFeatureNum];
            if (NULL != m_pfFeatures[i])                                        //add by wangqianfeng on 20121106
            {
                memset(m_pfFeatures[i], 0, sizeof(float) * m_nFeatureNum);
            }
        }
    }

    FeatureInfo(int nGateNum, int nFeatureNum)
    {
        m_dX = 0.0;
        m_dY = 0.0;
        m_dZ = 0.0;
        m_dR = 0.0;
        m_dA = 0.0;
        m_dB = 0.0;
        m_dW = 0.0;

        if (nGateNum < 1)
        {
            nGateNum = 1;
        }
        if (nFeatureNum < 1)
        {
            nFeatureNum = 1;
        }

        m_nCount = 0;

        m_pfFeatures = new float *[nGateNum];
        if (NULL != m_pfFeatures)                                               //add by wangqianfeng on 20121106
        {
            for (int j = 0; j < nGateNum; j++)
            {
                m_pfFeatures[j] = NULL;
            }
        }
        for (int i = 0; i < nGateNum; i++)
        {
            m_pfFeatures[i] = new float[nFeatureNum];
            if (NULL != m_pfFeatures[i])                                        //add by wangqianfeng on 20121106
            {
                memset(m_pfFeatures[i], 0, sizeof(float) * nFeatureNum);
            }
        }

        m_nGateNum = nGateNum;
        m_nFeatureNum = nFeatureNum;
    }

    ~FeatureInfo()
    {
        if (m_pfFeatures != NULL)
        {
            for (int i = 0; i < m_nGateNum; i++)
            {
                delete [] (m_pfFeatures[i]);
                m_pfFeatures[i] = NULL;
            }
            delete [] m_pfFeatures;
            m_pfFeatures = NULL;
        }
    }
    inline FeatureInfo& operator = (const FeatureInfo& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        m_dX = rhs.m_dX;
        m_dY = rhs.m_dY;
        m_dZ = rhs.m_dZ;
        m_dR = rhs.m_dR;
        m_dA = rhs.m_dA;
        m_dB = rhs.m_dB;
        m_dW = rhs.m_dW;

        if (m_pfFeatures != NULL)
        {
            for (int i = 0; i < m_nGateNum; i++)
            {
                delete [] (m_pfFeatures[i]);
                m_pfFeatures[i] = NULL;
            }
            delete [] m_pfFeatures;
            m_pfFeatures = NULL;
        }

        m_nGateNum = rhs.m_nGateNum;
        m_nFeatureNum = rhs.m_nFeatureNum;

        m_nCount = rhs.m_nCount;

        if (m_nGateNum > 0)
        {
            m_pfFeatures = new float *[m_nGateNum];
            if (NULL != m_pfFeatures)                                           //add by wangqianfeng on 20121106
            {
                for (int j = 0; j < m_nGateNum; j++)
                {
                    m_pfFeatures[j] = NULL;
                }
            }
            for (int i = 0; i < m_nGateNum; i++)
            {
                m_pfFeatures[i] = new float[m_nFeatureNum];
                if (NULL != m_pfFeatures[i])                                    //add by wangqianfeng on 20121106
                {
                    memcpy(m_pfFeatures[i], rhs.m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
                }
            }
        }

        return *this;
    }

    FeatureInfo(const FeatureInfo& rhs)
    {
        m_dX = rhs.m_dX;
        m_dY = rhs.m_dY;
        m_dZ = rhs.m_dZ;
        m_dR = rhs.m_dR;
        m_dA = rhs.m_dA;
        m_dB = rhs.m_dB;
        m_dW = rhs.m_dW;

        m_nGateNum = rhs.m_nGateNum;
        m_nFeatureNum = rhs.m_nFeatureNum;

        m_nCount = rhs.m_nCount;

        if (m_nGateNum > 0)
        {
            m_pfFeatures = new float *[m_nGateNum];
            if (NULL != m_pfFeatures)                                           //add by wangqianfeng on 20121106
            {
                for (int j = 0; j < m_nGateNum; j++)
                {
                    m_pfFeatures[j] = NULL;
                }
            }
            for (int i = 0; i < m_nGateNum; i++)
            {
                m_pfFeatures[i] = new float[m_nFeatureNum];
                if (NULL != m_pfFeatures[i])                                    //add by wangqianfeng on 20121106
                {
                    memcpy(m_pfFeatures[i], rhs.m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
                }
            }
        }
    }

public:
    double m_dX;
    double m_dY;
    double m_dZ;
    double m_dR;
    double m_dA;
    double m_dB;
    double m_dW;
    int m_nGateNum;
    int m_nFeatureNum;
    int m_nCount;                                                               // 相同叠加次数， 网格化算平均值时使用
    float **m_pfFeatures;                                                       // 特征值二级指针
    //特征信号,4个门，每个门7个特征，共4*7=28个
    //  1、触发到界面（第一个回波）检测
    //  2、门内第一个峰值前的阈值时间
    //  3、第一个峰值（门内）时间
    //  4、门内最大峰值前的阈值时间
    //  5、最大峰值（门内）时间 MPMT
    //  6、门内第一个峰值幅度 FPMA
    //  7、门内最大峰值幅度 MPMA
};

class ReadFileModule : public QObject
{
    Q_OBJECT

    struct FileParameter
    {
        int nGateNum[CHANNEL_NUM];
        qint32 NumChannelPoints[CHANNEL_NUM];
        QVector<int> enableFearture;
        ScanFileHeader header;
    };

public:
    explicit ReadFileModule(QObject *parent = nullptr);

public:
    bool SetFileName(const QString &fileName);
    void ReadFile();

signals:
    void ParameterReady(const QJsonObject &obj);
    void DataReadReady(UTDataFrame frame, int value);
    void DataReadCompleted();

public slots:

private:
    void ReadNewPoints();
    void ReadOldPoints();

    void ParseParameter(const ScanFileHeader &header);
    QPair<int, int> GetAxesByPlane(int plane, int filltype);
    QRectF GetRectFByHeader(const ScanFileHeader &header);

    void FullCDataByFeature(UTDataFrame &frame, const FeatureInfo &featureInfo, int nGate);

private:
    QFile *dataFile;
    FileParameter parameter;
};
