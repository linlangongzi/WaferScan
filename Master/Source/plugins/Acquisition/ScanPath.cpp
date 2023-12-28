#include "ScanPath.h"
#include "arsCore/Range.h"
#include "arsCore/Util/JsonConverts.h"
#include <QDebug>
#include <QtMath>
#include "arsCore/Platform.h"
#include "arsCore/Axis.h"

const float ScanPath::RECTANGLE_MIN_WIDTH = 0/*10*/;
const ScanPath::SetPathPointerMap ScanPath::InitializeFuncMap()                 /* 初始化类型函数Map */
{
    ScanPath::SetPathPointerMap map;
    map["NONE"] = &ScanPath::SetPathNone;
    map["LOCATE"] = &ScanPath::SetPathLocate;
    map["LINE"] = &ScanPath::SetPathLine;
    map["RECTANGLE"] = &ScanPath::SetPathRectangle;
    map["DISK"] = &ScanPath::SetPathDisk;
    map["CYLINDER"] = &ScanPath::SetPathCylinder;
    map["COORD_LIST"] = &ScanPath::SetPathCoordList;
    return map;
}

const ScanPath::SetPathPointerMap ScanPath::pathFuncMap = ScanPath::InitializeFuncMap();

/* 获取路径的总长度 */
float ScanPath::GetLengthTotal()
{
    return GetLength(0);
}

float ScanPath::GetLength(int index) const
{
    if ( (index < 0) || ( index + 1 > count() ) )
    {
        return 0.0f;
    }
    float total = 0;
    const_iterator prev = constBegin() + index;
    const_iterator it = constBegin() + index + 1;
    const_iterator end = constEnd();
    while (it < end)
    {
        total += it->DistanceTo(*prev);
        it++;
        prev++;
    }
    //qDebug() << "Path length" << count() << total;
    return total;
}

ScanPath::PathError ScanPath::SetPath(const QString &type, const QJsonObject &info)
{
    if ( !pathFuncMap.contains(type) )
    {
        qWarning() << "ScanPath::SetPath: Invalid path type" << type;
        return ERR_TYPE_INVALID;
    }
    SetPathPointer funcPointer = pathFuncMap.value(type);
    return (this->*funcPointer)(info);
}

/**
 * 面板上绘制扫查路径图形
 * @brief ScanPath::Paint
 * @param painter 面板画图器引用 axisX 作为绘图x坐标的轴 axisY 作为绘图y坐标的轴
 */
void ScanPath::Paint(QPainter &painter, int axisX, int axisY)
{
    //    double minX = at(0).at(0).value;
    //    double minY = at(0).at(1).value;
    //    double maxX = at(0).at(0).value;
    //    double maxY = at(0).at(1).value;
    //    foreach (UTPose pose, *this)
    //    {
    //        if (minX > pose.at(0).value)
    //        {
    //            minX = pose.at(0).value;
    //        }
    //        if (minY > pose.at(1).value)
    //        {
    //            minY = pose.at(1).value;
    //        }
    //        if (maxX < pose.at(0).value)
    //        {
    //            maxX = pose.at(0).value;
    //        }
    //        if (maxY < pose.at(1).value)
    //        {
    //            maxY = pose.at(1).value;
    //        }
    //    }
    //    painter.setWindow(minX - 50, minY- 50, maxX + 50, maxY + 50); // 对应逻辑坐标
    int nDataNum = size();                                                      // 检测数据个数
    if (nDataNum > 0)
    {
        double dX1 = at(0).at(axisX);
        double dY1 = at(1).at(axisY);                                           // 由于坐标系统变换为左下角为原点，所以绘图Y取反
        for (int nNum = 1; nNum < nDataNum; nNum++)
        {
            double dX2 = at(nNum).at(axisX);
            double dY2 = at(nNum).at(axisY);
            painter.drawLine(dX1, dY1, dX2, dY2);
            dX1 = dX2;
            dY1 = dY2;
        }
    }
}

QRectF ScanPath::GetBoundingRect(int axisX, int axisY) const
{
    float minX = qInf();
    float maxX = -qInf();
    float minY = qInf();
    float maxY = -qInf();
//    foreach (UTPose pose, *this)
//    {
//        minX = qMin( minX, pose.value(axisX) );
//        maxX = qMax( maxX, pose.value(axisX) );
//        minY = qMin( minY, pose.value(axisY) );
//        maxY = qMax( maxY, pose.value(axisY) );
//    }

    minX = qMin(pathStart.value(axisX), pathEnd.value(axisX));
    maxX = qMax(pathStart.value(axisX), pathEnd.value(axisX));
    minY = qMin(pathStart.value(axisY), pathEnd.value(axisY));
    maxY = qMax(pathStart.value(axisY), pathEnd.value(axisY));

    return minX <= maxX ? QRectF(minX, minY, maxX - minX, maxY - minY) : QRectF();
}

Range ScanPath::GetRange(int axisNum) const
{
//    Axis * axis = platform->Axes->FindObject<Axis>("AxisID", axisNum);
//    if (!axis)
//    {
//        return
//    }
    float min = qInf();
    float max = -qInf();
    foreach (UTPose pose, *this)
    {
        min = qMin( min, pose.value(axisNum) );
        max = qMax( max, pose.value(axisNum) );
    }

//    min = qMin(pathStart.value(axisNum), pathEnd.value(axisNum));
//    max = qMax(pathStart.value(axisNum), pathEnd.value(axisNum));

    return min <= max ? Range(min, max) : Range();
}

void ScanPath::SetParent(ScanPlan *plan)
{
    parent = plan;
}

ScanPlan *ScanPath::GetParent()
{
    return parent;
}

ScanPath::PathError ScanPath::SetPathNone(const QJsonObject &info)
{
    Q_UNUSED(info)
    clear();
    return ERR_NONE;
}

ScanPath::PathError ScanPath::SetPathLocate(const QJsonObject &info)
{
    clear();
    float x = info.value("X1").toDouble();                                      //  借用直线的面板使用，仍然保存的是两个点，但值使用第一个点
    float y = info.value("Y1").toDouble();
    float z = info.value("Z1").toDouble();
    push_back( QVector3D(x, y, z) );
    push_back( QVector3D(x, y, z) );
    return ERR_NONE;
}

ScanPath::PathError ScanPath::SetPathLine(const QJsonObject &info)
{
    clear();
    QList<Axis *> axisList = Axis::GetAxesSortByAxisID();
    int count = axisList.isEmpty() ? 0 : axisList.last()->GetAxisID() + 1;
    UTPose start(count);
    UTPose end(count);
    int NumAxes = platform()->Axes()->GetNumItems();
    for (int id = 1; id <= NumAxes; ++id)
    {
        Axis *axis = platform()->Axes()->GetByID<Axis>(id);
        if (!axis)
        {
            continue;
        }
        int axisIndex = axis->GetAxisID();
        start.replace( axisIndex, info.value( QString("Start_%1").arg(id) ).toDouble() );
        end.replace( axisIndex, info.value( QString("End_%1").arg(id) ).toDouble() );
    }
    push_back(start);
    push_back(end);
    return ERR_NONE;
}

ScanPath::PathError ScanPath::SetPathArc(const QJsonObject &info)
{
    Q_UNUSED(info);
    return ERR_NOT_IMPLEMENTED;
}

/*!
 * \brief 按照矩形初始化扫描路径。
 * \param info 矩形的信息表
 * \return 错误码。如果成功，则返回ERR_NONE
 */
ScanPath::PathError ScanPath::SetPathRectangle(const QJsonObject &info)
{
    clear();                                                                    /* 丢弃Path中原有的路径点 */
    float x1 = info.value("X1").toDouble();                                     /* 获取参数表中的各参数 */
    float y1 = info.value("Y1").toDouble();                                     /* start1(x1, y1, z1)                 start2(x2, y1, z1)    */
    float z1 = info.value("Z1").toDouble();                                     /*          ----------------------------------->|
                                                                                 *          |<----------------------------------|           */
    float x2 = info.value("X2").toDouble();                                     /*          |---------------------------------->|           */
    float y2 = info.value("Y2").toDouble();                                     /*          <-----------------------------------|           */
    float z2 = info.value("Z2").toDouble();                                     /*  end1(x1, y2, z2)                    end2(x2, y2, z2)    */

    pathStart = UTPose(QVector3D(x1, y1, z1));
    pathEnd = UTPose(QVector3D(x2, y2, z2));

    float step = info.value("Step").toDouble();                                 /* 步进长度，这个长度是空间中两条扫描线之间的欧氏距离 */
    int nScanAxis = info.value("ScanAxisID").toInt(-1);                         /* 扫描轴的索引号 */

    float scanWidth = 0;
    QVector3D ProbeOffset = JsonToVector3D(info.value("ProbeOffset"));
    QVector3D end1;
    QVector3D start2;
    switch (nScanAxis)
    {
    case 0: scanWidth = x1 - x2; start2 = QVector3D(x2, y1, z1); end1 = QVector3D(x1, y2, z2); break;
    case 1: scanWidth = y1 - y2; start2 = QVector3D(x1, y2, z1); end1 = QVector3D(x2, y1, z2); break;
    case 2: scanWidth = z1 - z2; start2 = QVector3D(x1, y1, z2); end1 = QVector3D(x2, y2, z1); break;
    default:
        qWarning() << "ScanPath::SetPathRectangle: invalid axis index" << nScanAxis;
        return ERR_INVALID_AXIS_INDEX;
    }
    if (qAbs(scanWidth) < RECTANGLE_MIN_WIDTH)
    {
        return ERR_VALUE_INVALID;
    }

    QVector3D start1(x1, y1, z1);
//    qDebug() << "Starting" << start1;
//    qDebug() << "Ending" << QVector3D(x2, y2, z2);
    QVector3D directionVec = end1 - start1;                                     /* 步进方向上的一个向量 */
    QVector3D stepVec = directionVec.normalized() * step;                       /* 单步步进的增量向量 */
    float directionLength = directionVec.length();                              /* 步进的总距离 */
    QVector3D current;                                                          /* 当前坐标 */

    float intervalProjection = QVector3D::dotProduct(ProbeOffset, directionVec.normalized()); /*探头偏移在步进方向上的投影*/
    intervalProjection = qFabs(intervalProjection);
    int i = 0;

    QJsonArray array = info.value("ChannelSelect").toArray();
    int channelNum = array.size();                                                         /*通道数*/


    if (!info.value("MultipleChannelSelect").toBool() || (channelNum < 2) || (intervalProjection == 0) ) /*如果为单通道或者通道数小于2或者探头偏移在步进方向投影为0*/
    {                                                                                                    /*将投影偏移设置为步进方向的总距离*/
        intervalProjection = directionLength;
        channelNum = 1;
    }
    QVector3D start = start1;
    QVector3D startcurrett;
    bool fullScan = false;
    while ((start - end1).length() > channelNum * intervalProjection)
    {
        i = 0;
        current = start;
        while((current - start).length() <= intervalProjection)
        {
            current = start + stepVec * i;
            push_back( UTPose(current) );
            push_back( UTPose(current + start2 - start1) );
            i++;
            current = start + stepVec * i;
            push_back( UTPose(current + start2 - start1) );
            push_back( UTPose(current) );
            i++;
        }
        startcurrett = current + stepVec;                                           /*记录当前探头移动到的位置，并步进个间距，用于剩余路径小于一个探头间距路径的起始*/
        start = start + directionVec.normalized() * (channelNum * intervalProjection);   /*扫查前的偏移亮加上每个通道扫查一个通道间距后 偏移量应移动的距离*/
        fullScan = true;
    }
    if (fullScan)
    {
        if ((start - end1).length() < intervalProjection)                               /*当剩余小于一个探头间距时，用最后一个探头继续扫查剩余的部分*/
        {
            double residualDistance = (start - end1).length();                                            /*剩余距离*/
            //start = end1 - directionVec.normalized() * ((channelNum - 1) * intervalProjection) - (start - end1);   /*回到之前的位置*/
            start = startcurrett;
            i = 0;
            current = start;
            while((current - start).length() <= residualDistance)
            {
                current = start + stepVec * i;
                push_back( UTPose(current) );
                push_back( UTPose(current + start2 - start1) );
                i++;
                current = start + stepVec * i;
                push_back( UTPose(current + start2 - start1) );
                push_back( UTPose(current) );
                i++;
            }
        }
        else
        {
            start = end1 - directionVec.normalized() * (channelNum * intervalProjection);
            i = 0;
            current = start;
            while((current - start).length() <= intervalProjection)
            {
                current = start + stepVec * i;
                push_back( UTPose(current) );
                push_back( UTPose(current + start2 - start1) );
                i++;
                current = start + stepVec * i;
                push_back( UTPose(current + start2 - start1) );
                push_back( UTPose(current) );
                i++;
            }
        }
    }
    else
    {
        current = start;
        while((current - start).length() < qMin(intervalProjection, directionLength))
        {
            current = start + stepVec * i;
            push_back( UTPose(current) );
            push_back( UTPose(current + start2 - start1) );
            i++;
            current = start + stepVec * i;
            push_back( UTPose(current + start2 - start1) );
            push_back( UTPose(current) );
            i++;
        }
    }

    return ERR_NONE;
}

ScanPath::PathError ScanPath::SetPathDisk(const QJsonObject &info)
{
    clear();

    float x1 = info.value("X1").toDouble();
    float y1 = info.value("Y1").toDouble();
    float z1 = info.value("Z1").toDouble();

    float x2 = info.value("X2").toDouble();
    float y2 = info.value("Y2").toDouble();
    float z2 = info.value("Z2").toDouble();

    float CenterX = info.value("CenterX").toDouble();
    float CenterY = info.value("CenterY").toDouble();
    float CenterZ = info.value("CenterZ").toDouble();

    QVector3D ProbeOffset = JsonToVector3D(info.value("ProbeOffset"));


    QVector3D center(CenterX, CenterY, CenterZ);
    QVector3D direction(0, 0, 1);

    QVector3D point1(x1, y1, z1);
    QVector3D point2(x2, y2, z2);
    QVector3D start1;
    QVector3D end1;
    float radius1 = point1.distanceToLine(center, direction);
    float radius2 = point2.distanceToLine(center, direction);
    if (radius1 >= radius2)    //盘环件从距离轴心比较远的点开始扫查
    {
        start1 = point1;
        end1 = point2;
    }
    else
    {
        start1 = point2;
        end1 = point1;
    }

    pathStart = UTPose(start1);
    pathEnd = UTPose(end1);

    float step = info.value("Step").toDouble();
    QVector3D directionVec = end1 - start1;
    QVector3D stepVec = directionVec.normalized() * step;
    float directionLength = directionVec.length();

    float intervalProjection = QVector3D::dotProduct(ProbeOffset, directionVec.normalized()); /*探头偏移在步进方向上的投影*/
    intervalProjection = qFabs(intervalProjection);

    QJsonArray array = info.value("ChannelSelect").toArray();
    int channelNum = array.size();
    if (!info.value("MultipleChannelSelect").toBool() || (channelNum < 2) || (intervalProjection == 0) ) /*如果为单通道或者通道数小于2或者探头偏移在步进方向投影为0*/
    {                                                                                                    /*将投影偏移设置为步进方向的总距离*/
        intervalProjection = directionLength;
        channelNum = 1;
    }
    QVector3D current;
    int i = 0;

    QVector3D start = start1;
    QVector3D startcurrett;
    bool fullScan = false;
    while ((start - end1).length() > channelNum * intervalProjection)
    {
        i = 0;
        current = start;
        while((current - start).length() <= intervalProjection)
        {
            current = start + stepVec * i;
            push_back( UTPose(current) );
            i++;
        }
//        current = start + stepVec * i;          /* 多步进一些 */
//        push_back( UTPose(current) );
        startcurrett = current + stepVec;
        start = start + directionVec.normalized() * (channelNum * intervalProjection);   /*扫查前的偏移亮加上每个通道扫查一个通道间距后 偏移量应移动的距离*/
        fullScan = true;
    }
    if (fullScan)
    {
        if ((start - end1).length() < intervalProjection)
        {
            double residualDistance = (start - end1).length();                                            /*剩余距离*/
            //start = start - directionVec.normalized() * (channelNum * intervalProjection);   /*回到之前的位置*/
            start = startcurrett;
            i = 0;
            current = start;
            while((current - start).length() <= residualDistance)
            {
                current = start + stepVec * i;
                push_back( UTPose(current) );
                i++;
            }
        }
        else
        {
            start = end1 - directionVec.normalized() * (channelNum * intervalProjection);
            i = 0;
            current = start;
            while((current - start).length() <= intervalProjection)
            {
                current = start + stepVec * i;
                push_back( UTPose(current) );
                i++;
            }
        }
//        current = start + stepVec * i;          /* 多步进一些 */
//        push_back( UTPose(current) );
    }
    else
    {
        current = start;
       // qDebug() << "sdfadsfasdfasdfasdfadf" << qMin(intervalProjection, directionLength);
        while((current - start).length() < qMin(intervalProjection, directionLength))
        {
            current = start + stepVec * i;
            push_back( UTPose(current) );
            i++;
        }
//        current = start + stepVec * i;          /* 多步进一些 */
//        push_back( UTPose(current) );
    }
    return ERR_NONE;
}

ScanPath::PathError ScanPath::SetPathCylinder(const QJsonObject &info)
{
    clear();

    float x1 = info.value("X1").toDouble();
    float y1 = info.value("Y1").toDouble();
    float z1 = info.value("Z1").toDouble();

    float x2 = info.value("X2").toDouble();
    float y2 = info.value("Y2").toDouble();
    float z2 = info.value("Z2").toDouble();

    QVector3D ProbeOffset = JsonToVector3D(info.value("ProbeOffset"));
    pathStart = UTPose(QVector3D(x1, y1, z1));
    pathEnd = UTPose(QVector3D(x2, y2, z2));

    float step = info.value("Step").toDouble();
    QVector3D start1(x1, y1, z1);
    QVector3D end1(x2, y2, z2);
    QVector3D directionVec = end1 - start1;
    QVector3D stepVec = directionVec.normalized() * step;
    float directionLength = directionVec.length();

    float intervalProjection = QVector3D::dotProduct(ProbeOffset, directionVec.normalized()); /*探头偏移在步进方向上的投影*/
    intervalProjection = qFabs(intervalProjection);

    QJsonArray array = info.value("ChannelSelect").toArray();
    int channelNum = array.size();                                                         /*通道数*/


    if (!info.value("MultipleChannelSelect").toBool() || (channelNum < 2) || (intervalProjection == 0) ) /*如果为单通道或者通道数小于2或者探头偏移在步进方向投影为0*/
    {                                                                                                    /*将投影偏移设置为步进方向的总距离*/
        intervalProjection = directionLength;
        channelNum = 1;
    }
    QVector3D current;                                                          /* 当前坐标 */
    int i = 0;

    QVector3D start = start1;
    QVector3D startcurrett;
    bool fullScan = false;
    while ((start - end1).length() > channelNum * intervalProjection)
    {
        i = 0;
        current = start;
        while((current - start).length() <= intervalProjection)
        {
            current = start + stepVec * i;
            push_back( UTPose(current) );
            i++;
        }
        current = start + stepVec * i;          /* 多步进一些 */
        push_back( UTPose(current) );
        startcurrett = current + stepVec;
        start = start + directionVec.normalized() * (channelNum * intervalProjection);   /*扫查前的偏移亮加上每个通道扫查一个通道间距后 偏移量应移动的距离*/
        fullScan = true;
    }
    if (fullScan)
    {
        if ((start - end1).length() < intervalProjection)
        {
            double residualDistance = (start - end1).length();                                            /*剩余距离*/
            //start = start - directionVec.normalized() * (channelNum * intervalProjection);   /*回到之前的位置*/
            start = startcurrett;
            i = 0;
            current = start;
            while((current - start).length() <= residualDistance)
            {
                current = start + stepVec * i;
                push_back( UTPose(current) );
                i++;
            }
        }
        else
        {
            start = end1 - directionVec.normalized() * (channelNum * intervalProjection);
            i = 0;
            current = start;
            while((current - start).length() <= intervalProjection)
            {
                current = start + stepVec * i;
                push_back( UTPose(current) );
                i++;
            }
        }
        current = start + stepVec * i;          /* 多步进一些 */
        push_back( UTPose(current) );
    }
    else
    {
        current = start;
        while((current - start).length() < qMin(intervalProjection, directionLength))
        {
            current = start + stepVec * i;
            push_back( UTPose(current) );
            i++;
        }
        current = start + stepVec * i;          /* 多步进一些 */
        push_back( UTPose(current) );
    }

    return ERR_NONE;
}

ScanPath::PathError ScanPath::SetPathCoordList(const QJsonObject &info)
{
    clear();
    QJsonArray list = info["List"].toArray();
    if ( list.isEmpty() )
    {
        return ERR_VALUE_NOT_FOUND;
    }

    for (int index = 0; index < list.size(); ++index)
    {
        QJsonValue item = list[index];
        if ( item.isObject() )
        {
            QJsonObject obj = item.toObject();
            double x = obj["X"].toDouble();
            double y = obj["Y"].toDouble();
            double z = obj["Z"].toDouble();
            push_back( UTPose( QVector3D(x, y, z) ) );
        }
        else
        {
            push_back( UTPose( item.toArray() ) );
        }
    }
    return ERR_NONE;
}

//ScanPath path;
//QJsonObject info;
//info["X1"] = 10;
//info["Y1"] = 0;
//info["Z1"] = 30;
//info["X2"] = 130;
//info["Y2"] = 180;
//info["Z2"] = 200;
//info["ScanAxis"] = 2;
//info["Step"] = 1;
//qDebug() << path.SetPath("RECTANGLE", info);
//qDebug() << path;
