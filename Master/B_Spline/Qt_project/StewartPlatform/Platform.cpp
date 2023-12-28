#include "Platform.h"
#include <QMatrix4x4>
#include <QDebug>
#include <qmath.h>
#include <QFile>

Platform::Platform(Type type)
{
    stewartType = type;
    /* 机械参数初始化，根据不同平台进行修正 */
    if (stewartType == LinkType) {                                                   //舵机结构
        QFile file(QString("link.csv"));
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "fail read paras";
            topRadius = 244.95;
            topInterval = 100;
            bottomRadius = 332.54;
            bottomInterval = 340;
            lengthOfSteerWheel = 150;
            lengthOfCardan = 44;
            lengthOfBar = 368;
            topPlatform = QVector<QVector3D>(6);
            bottomPlatform = QVector<QVector3D>(6);
            endOfSteelWheel = QVector<QVector3D>(6);
            cardanCenter = QVector<QVector3D>(6);
            motorBar = qSqrt(qPow(lengthOfSteerWheel, 2) + qPow(lengthOfCardan, 2));
            theta0 = qRadiansToDegrees(qAtan(lengthOfCardan / lengthOfSteerWheel));                 /* 弧度制 */
            range = QVector<QVector3D>(6);
            range[0] = QVector3D(-200, 0, 200);
            range[1] = QVector3D(-200, 0, 200);
            baseLength = 278;                                                       //平台基本高度
            range[2] = QVector3D(baseLength - 10, baseLength, baseLength + 100);
            range[3] = QVector3D(-30, 0, 30);
            range[4] = QVector3D(-30, 0, 30);
            range[5] = QVector3D(-30, 0, 30);
            linkType = false;                                                        //false为外摆结构，true为内摆结构
        } else {
            range = QVector<QVector3D>(6);
            qDebug() << "Into get paras";
            QTextStream stream(&file);
            while(!stream.atEnd()) {
                QString line = stream.readLine();
                const QStringList list = line.split(",");
                if (list.first() == "topRadius") {
                    topRadius = list.last().toDouble();
                } else if (list.first() == "topInterval") {
                    topInterval = list.last().toDouble();
                } else if (list.first() == "bottomRadius") {
                    bottomRadius = list.last().toDouble();
                } else if (list.first() == "bottomInterval") {
                    bottomInterval = list.last().toDouble();
                } else if (list.first() == "lengthOfSteerWheel") {
                    lengthOfSteerWheel = list.last().toDouble();
                } else if (list.first() == "lengthOfCardan") {
                    lengthOfCardan = list.last().toDouble();
                } else if (list.first() == "lengthOfBar") {
                    lengthOfBar = list.last().toDouble();
                } else if (list.first() == "range_0") {
                    range[0] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "range_1") {
                    range[1] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "baseLength") {
                    baseLength = list.last().toDouble();                                                       //平台基本高度
                } else if (list.first() == "range_2") {
                    range[2] = QVector3D(baseLength - 10, baseLength, baseLength + list.last().toDouble());
                } else if (list.first() == "range_3") {
                    range[3] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "range_4") {
                    range[4] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "range_5") {
                    range[5] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "linkType") {
                    linkType = bool(list.last().toInt());                                                        //false为外摆结构，true为内摆结构
                }
            }
            topPlatform = QVector<QVector3D>(6);
            bottomPlatform = QVector<QVector3D>(6);
            endOfSteelWheel = QVector<QVector3D>(6);
            cardanCenter = QVector<QVector3D>(6);
            motorBar = qSqrt(qPow(lengthOfSteerWheel, 2) + qPow(lengthOfCardan, 2));
            theta0 = qRadiansToDegrees(qAtan(lengthOfCardan / lengthOfSteerWheel));                 /* 弧度制 */
        }
    } else if (stewartType == ClassicType) {                                          //步进电机结构
        QFile file(QString("classic.csv"));
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "fail read paras";
            topRadius = 165.78;
            topInterval = 374.28;
            bottomRadius = 282.84;
            bottomInterval = 200;
            lengthOfSteerWheel = 16.5;                                              //参数未使用
            lengthOfCardan = 0;                                                     //参数未使用
            lengthOfBar = 200;                                                      //参数未使用
            topPlatform = QVector<QVector3D>(6);
            bottomPlatform = QVector<QVector3D>(6);
            endOfSteelWheel = QVector<QVector3D>(6);
            cardanCenter = QVector<QVector3D>(6);
            motorBar = qSqrt(qPow(lengthOfSteerWheel, 2) + qPow(lengthOfCardan, 2));//参数未使用
            theta0 = qRadiansToDegrees(qAtan(lengthOfCardan / lengthOfSteerWheel)); //参数未使用
            range = QVector<QVector3D>(6);
            range[0] = QVector3D(-50, 0, 50);
            range[1] = QVector3D(-50, 0, 50);
            baseLength = 580;                                                       //电动缸基本长度
            range[2] = QVector3D(baseLength - 17, baseLength - 17, baseLength + 100);
            range[3] = QVector3D(-15, 0, 15);
            range[4] = QVector3D(-15, 0, 15);
            range[5] = QVector3D(-15, 0, 15);
        } else {
            range = QVector<QVector3D>(6);
            qDebug() << "Into get paras";
            QTextStream stream(&file);
            while(!stream.atEnd()) {
                QString line = stream.readLine();
                const QStringList list = line.split(",");
                if (list.first() == "topRadius") {
                    topRadius = list.last().toDouble();
                } else if (list.first() == "topInterval") {
                    topInterval = list.last().toDouble();
                } else if (list.first() == "bottomRadius") {
                    bottomRadius = list.last().toDouble();
                } else if (list.first() == "bottomInterval") {
                    bottomInterval = list.last().toDouble();
                } else if (list.first() == "lengthOfSteerWheel") {
                    lengthOfSteerWheel = list.last().toDouble();
                } else if (list.first() == "lengthOfCardan") {
                    lengthOfCardan = list.last().toDouble();
                } else if (list.first() == "lengthOfBar") {
                    lengthOfBar = list.last().toDouble();
                } else if (list.first() == "range_0") {
                    range[0] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "range_1") {
                    range[1] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "baseLength") {
                    baseLength = list.last().toDouble();                                                       //平台基本高度
                } else if (list.first() == "range_2") {
                    range[2] = QVector3D(baseLength - 17, baseLength - 17, baseLength + list.last().toDouble());
                } else if (list.first() == "range_3") {
                    range[3] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "range_4") {
                    range[4] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                } else if (list.first() == "range_5") {
                    range[5] = QVector3D(-list.last().toDouble(), 0, list.last().toDouble());
                }
            }
            topPlatform = QVector<QVector3D>(6);
            bottomPlatform = QVector<QVector3D>(6);
            endOfSteelWheel = QVector<QVector3D>(6);
            cardanCenter = QVector<QVector3D>(6);
            motorBar = qSqrt(qPow(lengthOfSteerWheel, 2) + qPow(lengthOfCardan, 2));
            theta0 = qRadiansToDegrees(qAtan(lengthOfCardan / lengthOfSteerWheel));                 /* 弧度制 */
        }
    }

}

Platform::~Platform()
{

}


QVector3D Platform::Inverse(QVector3D point)
{
    QMatrix4x4 Txyz;
    Txyz.translate(x, y, z);                                               /* 此处的顺序跟matlab相反 */
    QMatrix4x4 Ra;
    Ra.rotate(a, 1, 0, 0);
    QMatrix4x4 Rb;
    Rb.rotate(b, 0, 1, 0);
    QMatrix4x4 Rc;
    Rc.rotate(c, 0, 0, 1);

    QVector3D p = Txyz * Rc * Rb * Ra * point;

    return p;
}

void Platform::SetPos(double xp, double yp, double zp, double ap, double bp, double cp, Type type)
{
    stewartType = type;
    x = xp;
    y = yp;
    z = zp;
    a = ap;
    b = bp;
    c = cp;
    QVector<double> joints(6);
    GetJoints(joints);
}

void Platform::SetPos(QVector<double> pos, Type type)
{
    SetPos(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5], type);
}
/*!
 * \brief Platform::GetJoints
 * \return 当前位姿下是否有解，若有解，返回true.
 *         joints:电机转角
 */
bool Platform::GetJoints(QVector<double> &joints)
{
    if (joints.size() != 6){
        return false;
    }
    //计算动平台参考点的位置
    topPlatform[0]= QVector3D(-topInterval / 2, -topRadius, 0);
    topPlatform[1] = QVector3D(topInterval / 2, -topRadius, 0);
    QMatrix4x4 rotation120;
    rotation120.rotate(120, 0, 0, 1);
    topPlatform[2] = rotation120 * topPlatform.at(0);
    topPlatform[3] = rotation120 * topPlatform.at(1);
    topPlatform[4] = rotation120 * topPlatform.at(2);
    topPlatform[5] = rotation120 * topPlatform.at(3);
    for (int index = 0; index < 6; ++index) {
        topPlatform[index] = Inverse(topPlatform.at(index));                  /* 得到去平台参考点的位置 */
    }
    //计算定平台电机轴位置
    bottomPlatform[0] = QVector3D(-bottomInterval / 2, -bottomRadius, 0);
    bottomPlatform[1] = QVector3D(bottomInterval / 2, -bottomRadius, 0);
    bottomPlatform[2] = rotation120 * bottomPlatform.at(0);
    bottomPlatform[3] = rotation120 * bottomPlatform.at(1);
    bottomPlatform[4] = rotation120 * bottomPlatform.at(2);
    bottomPlatform[5] = rotation120 * bottomPlatform.at(3);

//    QVector<double> joints(6,0);
    if (stewartType == ClassicType) {
        for (int index = 0; index < 6; ++index) {
            joints[index] = bottomPlatform[index].distanceToPoint(topPlatform[index]) - baseLength;
            if (joints[index] > 100 || joints[index] < 0) {                     //杆长范围约束
                return false;
            }
        }
    } else if (stewartType == LinkType) {
        for (int index = 0; index < 6; ++index) {
            QVector3D p0, p1;
            if (index == 0 || index == 2 || index == 4) {                       //针对不同位置的舵机，指定p0
                p0 = bottomPlatform[0];
            } else {
                p0 = bottomPlatform[1];
            }
            if (index == 0 || index == 1) {                                     //针对不同位置的舵机，指定p1
                p1 = topPlatform[index];
            } else if (index == 2 || index == 3) {
                QMatrix4x4 rotation_120;
                rotation_120.rotate(-120, 0, 0, 1);
                p1 = rotation_120 * topPlatform[index];
            } else if (index == 4 || index == 5) {
                QMatrix4x4 rotation_240;
                rotation_240.rotate(-240, 0, 0, 1);
                p1 = rotation_240 * topPlatform[index];
            }
            if (!CalculateAngle(joints[index], p0, p1, qSqrt(qPow(lengthOfSteerWheel,2) + qPow(lengthOfCardan, 2)), lengthOfBar, index)) {
                return false;
            }
        }
    }
    emit jointsChanged(joints[0], joints[1], joints[2], joints[3],joints[4], joints[5]);
    return true;
}

/*!
 * \brief Platform::CalculateAngle
 * \param angle
 * \param p0                                舵盘中心点
 * \param p1                                动平台上的点
 * \param L1                                舵杆长度
 * \param L2                                连杆长度
 * \param jointNO                           关节编号
 * \return
 */
bool Platform::CalculateAngle(qreal &angle, QVector3D p0, QVector3D p1, qreal L1, qreal L2, int jointNO)
{
//    qDebug() << p0 << p1 << L1 << L2;
    QVector3D dp = p1- p0;
    qreal stheta, theta, temp;
    if (qFuzzyCompare(dp.x(), 0)) {
        stheta = (qPow(L1, 2) + dp.lengthSquared() - qPow(L2, 2)) / (2 * dp.z() * L1);
        theta = qAsin(stheta);
    } else {
        temp = (dp.lengthSquared() + qPow(L1, 2) - qPow(L2, 2)) / (2 * dp.x() * L1);
    }
    qreal aeffi = qPow(dp.z() / dp.x(), 2) + 1;
    qreal beffi = -2 * temp * dp.z()/ dp.x();
    qreal ceffi = qPow(temp, 2) - 1;
    if (qPow(beffi, 2) - 4 * aeffi * ceffi < 0) {
        return false;
    } else {
        stheta = (-beffi - qSqrt(qPow(beffi, 2) - 4 * aeffi * ceffi)) / (2 * aeffi);
        if (stheta > -1 && stheta < 1) {
            theta = qAsin(stheta);
            //结果验证
            QVector3D dp1;
            if (jointNO == 0 || jointNO == 2 || jointNO == 4) {
                if (linkType) {
                    dp1 = QVector3D(L1 * qCos(theta), 0, L1 * qSin(theta));         //内摆结构
                } else {
                    dp1 = QVector3D(-L1 * qCos(theta), 0, L1 * qSin(theta));        //外摆结构
                }
            } else {
                if (linkType) {
                    dp1 = QVector3D(-L1 * qCos(theta), 0, L1 * qSin(theta));        //内摆结构
                } else {
                    dp1 = QVector3D(L1 * qCos(theta), 0, L1 * qSin(theta));         //外摆结构
                }
            }
            if (qAbs(p1.distanceToPoint(dp1 + p0) - L2) < 0.1) {
                angle = qRadiansToDegrees(theta) - theta0;
//                qDebug() << "theta 1: " << theta;
                return true;
            }
        }
        stheta = (-beffi + qSqrt(qPow(beffi, 2) - 4 * aeffi * ceffi)) / (2 * aeffi);
        if (stheta > -1 && stheta < 1) {
            theta = qAsin(stheta);
            //结果验证
            QVector3D dp1;
            if (jointNO == 0 || jointNO == 2 || jointNO == 4) {
                if (linkType) {
                    dp1 = QVector3D(L1 * qCos(theta), 0, L1 * qSin(theta));         //内摆结构
                } else {
                    dp1 = QVector3D(-L1 * qCos(theta), 0, L1 * qSin(theta));        //外摆结构
                }
            } else {
                if (linkType) {
                    dp1 = QVector3D(-L1 * qCos(theta), 0, L1 * qSin(theta));        //内摆结构
                } else {
                    dp1 = QVector3D(L1 * qCos(theta), 0, L1 * qSin(theta));         //外摆结构
                }
            }
            if (qAbs(p1.distanceToPoint(dp1 + p0) - L2) < 0.1) {
//                qDebug() << "theta 2: " << theta;
                angle = qRadiansToDegrees(theta) - theta0;
                return true;
            }
        }
    }
    return false;
}

