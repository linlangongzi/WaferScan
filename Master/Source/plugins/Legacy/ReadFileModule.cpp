#include "ReadFileModule.h"
#include <Ars500/DataDef.h>
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QDebug>
#include <QThread>
#include <QtMath>
#include <allrising/Util.h>

ReadFileModule::ReadFileModule(QObject *parent) :
    QObject(parent),
    dataFile(nullptr)
{}

/*!
 * \brief ReadFileModule::SetFileName 设置读取的旧数据文件名称
 * \param fileName  文件名称
 * \return  true: 成功
 *          false: 失败
 */
bool ReadFileModule::SetFileName(const QString &fileName)
{
    dataFile = new QFile(fileName);
    if ( !dataFile->open(QIODevice::ReadOnly) ) {
        qCritical() << "ReadFileModule::SetFileName the file can not be open";
        delete dataFile;
        dataFile = nullptr;
        return false;
    }
    return true;
}

/*!
 * \brief ReadFileModule::ReadFile 开始读文件
 */
void ReadFileModule::ReadFile()
{
    ARS_CHECK(dataFile);
    // 读取数据包头
//    qDebug() << "int" << sizeof(int) << "float" << sizeof(float) << "double" << sizeof(double)
//             << "bool" << sizeof(bool) << "qint16" << sizeof(qint16) << "qint32" << sizeof(qint32)
//             << "long" << sizeof(long) << "char*" << sizeof(char*) << "longlong" << sizeof(qlonglong)
//             << "GateInfo" << sizeof(GateInfo) << sizeof(parameter.header.m_GateInfo) << "header" << sizeof(ScanFileHeader);
    memset( &(parameter.header), 0, sizeof(ScanFileHeader) );
    dataFile->read( reinterpret_cast<char *>( &(parameter.header) ), sizeof(ScanFileHeader) );

    for (auto &gates : parameter.header.m_GateInfo) {
        for (auto &gate : gates) {
            auto peak = float(gate.m_fPeakGate);
            gate.m_fPeakGate = int(peak * 10) / 10.0f;
        }
    }
    // 各通道采集数据点数
    qint32 NumAllPoints = 0;
    for (int i = 0; i < parameter.header.m_wChannelNum; i++) {
        dataFile->read( reinterpret_cast<char *>(&parameter.NumChannelPoints[i]), sizeof(qint32) );
        NumAllPoints += parameter.NumChannelPoints[i];
    }
    //    BOOL    m_bMPFTimeDisplay;        // 最高波过阈值时间            4
    //    BOOL    m_bMPMTimeDisplay;        // 最高波过伤时               5
    //    BOOL    m_bMPMAltitudeDisplay;    // 最高波高度                 7
    //    BOOL    m_bFPFTimeDisplay;        // 一次波过阈值时间            2
    //    BOOL    m_bFPMTimeDisplay;        // 一次波伤时                 3
    //    BOOL    m_bFPMAltitudeDisplay;    // 一次波高度                 6

    // 特征值开关
    parameter.enableFearture.append(1);
    if (parameter.header.m_bFPFTimeDisplay) {
        parameter.enableFearture.append(2);
    }
    if (parameter.header.m_bFPMTimeDisplay) {
        parameter.enableFearture.append(3);
    }
    if (parameter.header.m_bMPFTimeDisplay) {
        parameter.enableFearture.append(4);
    }
    if (parameter.header.m_bMPMTimeDisplay) {
        parameter.enableFearture.append(5);
    }
    if (parameter.header.m_bFPMAltitudeDisplay) {
        parameter.enableFearture.append(6);
    }
    if (parameter.header.m_bMPMAltitudeDisplay) {
        parameter.enableFearture.append(7);
    }
    // 闸门开关
    for (int channel = 0; channel < parameter.header.m_wChannelNum; ++channel) {
        parameter.nGateNum[channel] = 0;
        for (int i = 0; i < 4; i++) {
            if (parameter.header.m_GateInfo[channel][i].m_bOpen) {
                parameter.nGateNum[channel]++;
            }
        }
    }
    // 分析参数，并且发送参数信息
    ParseParameter(parameter.header);
    // 分析扫描数据
    if (NumAllPoints == 0) {                                                    // 对于旧水浸最新版本，NumChannelPoints[i]永远为0
        ReadNewPoints();
    } else {
        ReadOldPoints();                                                        // 暂时不作处理
    }
    // 文件读完，需要关闭文件并且清空当前文件指针
    dataFile->close();
    dataFile = nullptr;
    // 发送读取文件完成信号
    emit DataReadCompleted();
}

/*!
 * \brief ReadFileModule::ReadNewPoints 旧水浸最新数据读取
 */
void ReadFileModule::ReadNewPoints()
{
    ARS_CHECK(dataFile);
    qint64 fileSize = dataFile->size();

    int nFeatureSize = 7 * sizeof(double) + 3 * sizeof(int);
    while (dataFile->pos() < fileSize) {
        int channel = 0;
        ulong dlength = 0;
        dataFile->read( reinterpret_cast<char *>(&channel), sizeof(int) );
        dataFile->read( reinterpret_cast<char *>(&dlength), sizeof(ulong) );

        FeatureInfo featureInfo( parameter.nGateNum[channel], parameter.enableFearture.count() );

        dataFile->read(reinterpret_cast<char *>(&featureInfo), nFeatureSize);

        UTDataFrame frame = UTDataFrame();
        frame.Pose << featureInfo.m_dX << featureInfo.m_dY << featureInfo.m_dZ << featureInfo.m_dR
                    << featureInfo.m_dA << featureInfo.m_dB << featureInfo.m_dW;
        frame.Context.SetChannelID(channel);
        frame.CData.resize(Ars500::NUM_FEATURES);

        for (int j = 0; j < featureInfo.m_nGateNum; ++j) {
            dataFile->read(reinterpret_cast<char *>(featureInfo.m_pfFeatures[j]), sizeof(float) * featureInfo.m_nFeatureNum);
            FullCDataByFeature(frame, featureInfo, j);
        }
        int process = dataFile->pos() / fileSize * 100;
        emit DataReadReady(frame, process);
    }
//    qDebug() << "ReadFileModule::ReadNewPoints fileSize" << fielSize << "FilePos" << dataFile->pos();
}

/*!
 * \brief ReadFileModule::ReadOldPoints 旧水浸早期数据读取
 */
void ReadFileModule::ReadOldPoints()
{
    for (int channel = 0; channel < parameter.header.m_wChannelNum; ++channel) {
        FeatureInfo featureInfo( parameter.nGateNum[channel], parameter.enableFearture.count() );   // 特征数据
        float fFeatureData[28];
        int nSize = 4 * sizeof(double) + 2 * sizeof(int);
        int nFeatureSize = sizeof(float) * parameter.nGateNum[channel] * parameter.enableFearture.count();

        for (int k = 0; k < parameter.NumChannelPoints[channel]; k++) {
            dataFile->read(reinterpret_cast<char *>(&featureInfo), nSize);
            dataFile->read(reinterpret_cast<char *>(&fFeatureData), nFeatureSize);
            for (int i = 0; i < parameter.nGateNum[channel]; i++) {
                for (int j = 0; j < parameter.enableFearture.count(); j++) {
                    featureInfo.m_pfFeatures[i][j] = fFeatureData[i * parameter.enableFearture.count() + j];
                }
            }
        }
    }
}

/*!
 * \brief ReadFileModule::ParseParameter 根据数据包头解析成像参数
 * \param header    数据包头
 */
void ReadFileModule::ParseParameter(const ScanFileHeader &header)
{
    QJsonObject obj;
    QPair<int, int> pair = GetAxesByPlane(header.m_nAxisSelect, header.m_nFillType);
    obj.insert("XAxis", pair.first);
    obj.insert("YAxis", pair.second);
    QRectF rect = GetRectFByHeader(header);
    QSizeF size = QSizeF(header.m_fGridX, header.m_fGridY);
    if (header.m_nFillType == 1) {
        size.transpose();
    }
    ImageOptions transf(rect, size);
    obj.insert( "Transfrom", transf.toJson() );
    emit ParameterReady(obj);
}

/*!
 * \brief ReadFileModule::GetAxesByPlane 根据成像平面获取成像轴
 * \param plane         // 成像平面
 * \param filltype      // 填充方式
 * \return              // 成像轴
 */
QPair<int, int> ReadFileModule::GetAxesByPlane(int plane, int filltype)
{
    Axis *xAxis = MotionManager::axis(0);
    Axis *yAxis = MotionManager::axis(1);
    Axis *zAxis = MotionManager::axis(2);
    Axis *rAxis = MotionManager::axis(3);
    QPair<int, int> AxesPair(0, 1);                                             // 如果轴不存在，需要给定默认轴
    switch (plane) {
    case 0:
        if ( (xAxis != nullptr) && (yAxis != nullptr) ) {
            AxesPair.first = xAxis->config().axisId;
            AxesPair.second = yAxis->config().axisId;
        }
        break;
    case 1:
        if ( (xAxis != nullptr) && (zAxis != nullptr) ) {
            AxesPair.first = xAxis->config().axisId;
            AxesPair.second = zAxis->config().axisId;
        }
        break;
    case 2:
        if ( (yAxis != nullptr) && (zAxis != nullptr) ) {
            AxesPair.first = yAxis->config().axisId;
            AxesPair.second = zAxis->config().axisId;
        }
        break;
    case 3:
    case 4:
        if ( (xAxis != nullptr) && (rAxis != nullptr) ) {
            AxesPair.first = xAxis->config().axisId;
            AxesPair.second = rAxis->config().axisId;
        }
        break;
    default:
        break;
    }
    if (filltype == 1) {                                                        // 纵向成像，需要交换xy轴
        QPair<int, int> pair(AxesPair);
        AxesPair.first = pair.second;
        AxesPair.second = pair.first;
    }
    return AxesPair;
}

/*!
 * \brief ReadFileModule::GetRectFByHeader 根据数据头获取成像范围
 * \param header    数据包头
 * \return          成像范围
 */
QRectF ReadFileModule::GetRectFByHeader(const ScanFileHeader &header)
{
    QRectF rect;                                   // 默认即为scene的大小
    switch (header.m_nAxisSelect) {
    case 0:
        rect.setTopLeft( QPointF(header.m_dXStart, header.m_dYStart) );
        rect.setBottomRight( QPointF(header.m_dXEnd, header.m_dYEnd) );
        break;
    case 1:
        rect.setTopLeft( QPointF(header.m_dXStart, header.m_dZStart) );
        rect.setBottomRight( QPointF(header.m_dXEnd, header.m_dZEnd) );
        break;
    case 2:
        rect.setTopLeft( QPointF(header.m_dYStart, header.m_dZStart) );
        rect.setBottomRight( QPointF(header.m_dYEnd, header.m_dZEnd) );
        break;
    case 3:
    case 4:
        rect.setTopLeft( QPointF(header.m_dXStart, 0) );
        rect.setBottomRight( QPointF(header.m_dXEnd, M_PI * header.m_dPipeDiameter) );
        break;
    default:
        break;
    }
    if (header.m_nFillType == 1) {                                              // 纵向成像，范围也应该交换一下
        QPointF topleft( QPointF( rect.top(), rect.left() ) );
        QPointF bottomRight( QPointF( rect.bottom(), rect.right() ) );
        rect.setTopLeft(topleft);
        rect.setBottomRight(bottomRight);
    }
    rect = rect.marginsAdded( QMarginsF(10, 10, 10, 10) );
    return rect;
}

/*!
 * \brief ReadFileModule::FullCDataByFeature 根据特征值填充C扫数据
 * \param frame     数据块
 * \param info      特征值信息
 * \param nGate     闸门序号
 */
void ReadFileModule::FullCDataByFeature(UTDataFrame &frame, const FeatureInfo &featureInfo, int nGate)
{
    if ((nGate >= featureInfo.m_nGateNum) || (nGate < 0) ) {
        return;
    }

    for (int nfearture = 0; nfearture < featureInfo.m_nFeatureNum; ++nfearture) {// 遍历所有特征值
        int index = parameter.enableFearture.value(nfearture);
        switch (index) {
        case 2:
            if (nGate == 0) {
                frame.CData.replace(0, featureInfo.m_pfFeatures[nGate][nfearture]);
            } else {
                int nCDataIndex = 3 + (nGate - 1) * 5;
                frame.CData.replace(nCDataIndex + 2, featureInfo.m_pfFeatures[nGate][nfearture]);
            }
            break;
        case 3:
            if (nGate != 0) {
                int nCDataIndex = 3 + (nGate - 1) * 5;
                frame.CData.replace(nCDataIndex + 3, featureInfo.m_pfFeatures[nGate][nfearture]);
            }
            break;
        case 4:
            break;
        case 5:
            if (nGate == 0) {
                frame.CData.replace(1, featureInfo.m_pfFeatures[nGate][nfearture]);
            } else {
                int nCDataIndex = 3 + (nGate - 1) * 5;
                frame.CData.replace(nCDataIndex + 0, featureInfo.m_pfFeatures[nGate][nfearture]);
            }
            break;
        case 6:
            if (nGate != 0) {
                int nCDataIndex = 3 + (nGate - 1) * 5;
                frame.CData.replace(nCDataIndex + 4, featureInfo.m_pfFeatures[nGate][nfearture]);
            }
            break;
        case 7:
            if (nGate == 0) {
                frame.CData.replace(2, featureInfo.m_pfFeatures[nGate][nfearture]);
            } else {
                int nCDataIndex = 3 + (nGate - 1) * 5;
                frame.CData.replace(nCDataIndex + 1, featureInfo.m_pfFeatures[nGate][nfearture]);
            }
            break;
        default:
            break;
        }
    }
    //    qint16 nGate1SyncTime;          // 闸门1同步时间                2
    //    qint16 nGate1MPTime;            // 闸门1内最大波峰值时间          5
    //    qint16 nGate1MPAmp;             // 闸门1内最大波峰值幅度          7

    //    qint16 nGate2MPTime;            // 闸门2内最大波峰值时间          5
    //    qint16 nGate2MPAmp;             // 闸门2内最大波峰值幅度          7
    //    qint16 nGate2FPFirstTime;       // 闸门2内第一个波峰过阈值时间     2
    //    qint16 nGate2FPTime;            // 闸门2内第一个波峰波峰值时间     3
    //    qint16 nGate2FPAmp;             // 闸门2内第一个波峰波峰值幅度     6

    //    BOOL    m_bMPFTimeDisplay;        // 最高波过阈值时间            4
    //    BOOL    m_bMPMTimeDisplay;        // 最高波过伤时               5
    //    BOOL    m_bMPMAltitudeDisplay;    // 最高波高度                 7
    //    BOOL    m_bFPFTimeDisplay;        // 一次波过阈值时间            2
    //    BOOL    m_bFPMTimeDisplay;        // 一次波伤时                 3
    //    BOOL    m_bFPMAltitudeDisplay;    // 一次波高度                 6

    //  1、触发到界面（第一个回波）检测
    //  2、门内第一个峰值前的阈值时间
    //  3、第一个峰值（门内）时间
    //  4、门内最大峰值前的阈值时间
    //  5、最大峰值（门内）时间 MPMT
    //  6、门内第一个峰值幅度 FPMA
    //  7、门内最大峰值幅度 MPMA
}
