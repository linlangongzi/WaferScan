#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>

typedef int BOOL;

struct DogInfo
{
    wchar_t wcMechineType[32];  // 设备型号
    wchar_t wcMechineNo[32];    // 设备序列号
    wchar_t wcUserCompany[32];  // 客户名称
    quint64   SaleTime;         // 出厂日期 原类型为CTime，根据MSDN，CTime为8字节长
    int     nCardNum;           // 卡数
    quint8    FieldValue1[4];     // 模块1,2 IP
    quint8    FieldValue2[4];
    quint8    FieldValue3[4];
    quint8    FieldValue4[4];
    float fXRange;              // 扫查X范围
    float fYRange;              // 扫查Y范围
    float fZRange;              // 扫查Z范围
    int nXCoderPerRingNum;      // X编码器单圈脉冲数
    int nYCoderPerRingNum;      // Y编码器单圈脉冲数
    int nZCoderPerRingNum;      // Z编码器单圈脉冲数
    int nW1CoderPerRingNum;     // W1编码器单圈脉冲数
    int nACoderPerRingNum;      // A编码器单圈脉冲数
    int nBCoderPerRingNum;      // B编码器单圈脉冲数
    int nW2CoderPerRingNum;     // W2编码器单圈脉冲数
    float fXDiameter;           // X编码器直径
    float fYDiameter;           // Y编码器直径
    float fZDiameter;           // Z编码器直径
    float fW1Diameter;          // W1直径
    float fADiameter;           // A直径
    float fBDiameter;           // B直径
    float fW2Diameter;          // W2直径
    float fXJianSuBi;           // X减速比
    float fYJianSuBi;           // Y减速比
    float fZJianSuBi;           // Z减速比
    float fW1JianSuBi;          // W1减速比
    float fAJianSuBi;           // A减速比
    float fBJianSuBi;           // B减速比
    float fW2JianSuBi;          // W2减速比
    BOOL bXReverse;             // X方向反转
    BOOL bYReverse;             // Y方向反转
    BOOL bZReverse;             // Z方向反转
    float fXCorrect;            // X方向修正值
    float fYCorrect;            // Y方向修正值
    float fZCorrect;            // Z方向修正值
    float fW1Correct;           // W1方向修正值
    float fACorrect;            // A方向修正值
    float fBCorrect;            // B方向修正值
    float fW2Correct;           // W2方向修正值
    BOOL bXEnable;              // X轴使能标志
    BOOL bYEnable;              // Y轴使能标志
    BOOL bZEnable;              // Z轴使能标志
    BOOL bAEnable;              // A轴使能标志
    BOOL bBEnable;              // B轴使能标志
    BOOL bW1Enable;             // W1轴使能标志
    BOOL bW2Enable;             // W2轴使能标志
    int nCount;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(QFileDialog::getOpenFileName());
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed opening file" << file.fileName() << ": " << file.errorString();
        return 2;
    }
    file.seek(0x10);
    const QByteArray ba = file.readAll();
    const qint64 offset = 0x480;
    if (ba.size() < offset + sizeof(DogInfo)) {
        qDebug() << "Failed reading file: insufficient length" << ba.size();
        return 1;
    }

    const DogInfo *dog = reinterpret_cast<const DogInfo *>(ba.data() + offset);
    qDebug() << QString::fromWCharArray(dog->wcUserCompany);
    qDebug() << QString::fromWCharArray(dog->wcMechineNo);
    qDebug() << QString::fromWCharArray(dog->wcMechineType);
    qDebug() << "SaleTime" << QDateTime::fromTime_t(dog->SaleTime);         // 出厂日期 原类型为CTime，根据MSDN，CTime为8字节长
    qDebug() << "nCardNum" << dog->nCardNum;           // 卡数
    for (int index: {0, 1, 2, 3}) {
        qDebug() << "IP" << index << ":" << dog->FieldValue1[index] << dog->FieldValue2[index] << dog->FieldValue3[index] << dog->FieldValue4[index];     // 模块1,2 IP
    }
    qDebug() << "fXRange" << dog->fXRange;              // 扫查X范围
    qDebug() << "fYRange" << dog->fYRange;              // 扫查Y范围
    qDebug() << "fZRange" << dog->fZRange;              // 扫查Z范围
    qDebug() << "nXCoderPerRingNum" << dog->nXCoderPerRingNum;      // X编码器单圈脉冲数
    qDebug() << "nYCoderPerRingNum" << dog->nYCoderPerRingNum;      // Y编码器单圈脉冲数
    qDebug() << "nZCoderPerRingNum" << dog->nZCoderPerRingNum;      // Z编码器单圈脉冲数
    qDebug() << "nW1CoderPerRingNum" << dog->nW1CoderPerRingNum;     // W1编码器单圈脉冲数
    qDebug() << "nACoderPerRingNum" << dog->nACoderPerRingNum;      // A编码器单圈脉冲数
    qDebug() << "nBCoderPerRingNum" << dog->nBCoderPerRingNum;      // B编码器单圈脉冲数
    qDebug() << "nW2CoderPerRingNum" << dog->nW2CoderPerRingNum;     // W2编码器单圈脉冲数
    qDebug() << "fXDiameter" << dog->fXDiameter;           // X编码器直径
    qDebug() << "fYDiameter" << dog->fYDiameter;           // Y编码器直径
    qDebug() << "fZDiameter" << dog->fZDiameter;           // Z编码器直径
    qDebug() << "fW1Diameter" << dog->fW1Diameter;          // W1直径
    qDebug() << "fADiameter" << dog->fADiameter;           // A直径
    qDebug() << "fBDiameter" << dog->fBDiameter;           // B直径
    qDebug() << "fW2Diameter" << dog->fW2Diameter;          // W2直径
    qDebug() << "fXJianSuBi" << dog->fXJianSuBi;           // X减速比
    qDebug() << "fYJianSuBi" << dog->fYJianSuBi;           // Y减速比
    qDebug() << "fZJianSuBi" << dog->fZJianSuBi;           // Z减速比
    qDebug() << "fW1JianSuBi" << dog->fW1JianSuBi;          // W1减速比
    qDebug() << "fAJianSuBi" << dog->fAJianSuBi;           // A减速比
    qDebug() << "fBJianSuBi" << dog->fBJianSuBi;           // B减速比
    qDebug() << "fW2JianSuBi" << dog->fW2JianSuBi;          // W2减速比
    qDebug() << "bXReverse" << dog->bXReverse;             // X方向反转
    qDebug() << "bYReverse" << dog->bYReverse;             // Y方向反转
    qDebug() << "bZReverse" << dog->bZReverse;             // Z方向反转
    qDebug() << "fXCorrect" << dog->fXCorrect;            // X方向修正值
    qDebug() << "fYCorrect" << dog->fYCorrect;            // Y方向修正值
    qDebug() << "fZCorrect" << dog->fZCorrect;            // Z方向修正值
    qDebug() << "fW1Correct" << dog->fW1Correct;           // W1方向修正值
    qDebug() << "fACorrect" << dog->fACorrect;            // A方向修正值
    qDebug() << "fBCorrect" << dog->fBCorrect;            // B方向修正值
    qDebug() << "fW2Correct" << dog->fW2Correct;           // W2方向修正值
    qDebug() << "bXEnable" << dog->bXEnable;              // X轴使能标志
    qDebug() << "bYEnable" << dog->bYEnable;              // Y轴使能标志
    qDebug() << "bZEnable" << dog->bZEnable;              // Z轴使能标志
    qDebug() << "bAEnable" << dog->bAEnable;              // A轴使能标志
    qDebug() << "bBEnable" << dog->bBEnable;              // B轴使能标志
    qDebug() << "bW1Enable" << dog->bW1Enable;             // W1轴使能标志
    qDebug() << "bW2Enable" << dog->bW2Enable;             // W2轴使能标志
    qDebug() << "nCount" << dog->nCount;

    qDebug() << "============";
    qDebug() << "Scale X" << dog->nXCoderPerRingNum * dog->fXJianSuBi / dog->fXDiameter;
    qDebug() << "Scale Y" << dog->nYCoderPerRingNum * dog->fYJianSuBi / dog->fYDiameter;
    qDebug() << "Scale Z" << dog->nZCoderPerRingNum * dog->fZJianSuBi / dog->fZDiameter;
    qDebug() << "Scale W1" << dog->nW1CoderPerRingNum * dog->fW1JianSuBi / 360.0f;
    qDebug() << "Scale A" << dog->nACoderPerRingNum * dog->fAJianSuBi  / 360.0f; // Song chenguang modified on 2013-03-29
    qDebug() << "Scale B" << dog->nBCoderPerRingNum * dog->fBJianSuBi  / 360.0f; // Song chenguang modified on 2013-03-29
    qDebug() << "Scale W2" << dog->nW2CoderPerRingNum * dog->fW2JianSuBi / 360.0f;

    qDebug() << endl;

//    return 0;
    return a.exec();
}
