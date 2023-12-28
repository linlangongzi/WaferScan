#include <QCoreApplication>
#include <QMatrix4x4>
#include <QtDebug>
#include <QtMath>

float TRtable[4][6];
void initTable(float x,float y,float z,float a,float b,float wD)            //wD为水距
{
    float offsetBAZ = 50;
    float offsetBAX = 25;
    float offsetAToolZ = 100;
    float offsetAToolY = 10;
    float waterDepth = wD;
    float lengthOfTool = offsetAToolZ + waterDepth;
    float jx = x;
    float jy = y;
    float jz = z;
    float ja = a/180*M_PI;                                                   //弧度
    float jb = b/180*M_PI;

    TRtable[0][3] = jx;
    TRtable[0][4] = jy;
    TRtable[0][5] = jz;
    TRtable[1][1] = jb;
    TRtable[2][0] = ja;
    TRtable[2][3] = -offsetBAX;
    TRtable[2][5] = -offsetBAZ;
    TRtable[3][4] = -offsetAToolY;
    TRtable[3][5] = -lengthOfTool;
/*  TRtable[0][] = {0, 0, 0, jx, jy, jz};
    TRtable[1][] = {0;jb;0;0;0;0};
    TRtable[2][] = {ja 0 0 -offsetBAX 0 -offsetBAZ};
    TRtable[3][] = {0 0 0 0 -offsetAToolY -lengthOfTool};

*/
}

QMatrix4x4 TR(float table[][6],int rowOfTable)
{
    float ca,sa,cb,sb,cc,sc;                                        //分别对应cos(ja)等
    QMatrix4x4 I(1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1);
    QMatrix4x4 A = I;
    for(int i = 0;i<rowOfTable;i++)                                 //处理每一行的6个参数
    {
        ca = qFastCos(table[i][0]);
        sa = qFastSin(table[i][0]);
        cb = qFastCos(table[i][1]);
        sb = qFastSin(table[i][1]);
        cc = qFastCos(table[i][2]);
        sc = qFastSin(table[i][2]);

        QMatrix4x4 T(1,0,0,table[i][3],                            //平移参量
                     0,1,0,table[i][4],
                     0,0,1,table[i][5],
                     0,0,0,1);
        //qDebug()<<"T:\n"<<T;
        QMatrix4x4 Rx(1,0,0,0,                                      //绕x轴旋转参量
                      0,ca,-sa,0,
                      0,sa,ca,0,
                      0,0,0,1);
        //qDebug()<<"Rx:\n"<<Rx;
        QMatrix4x4 Ry(cb,0,-sb,0,
                      0,1,0,0,
                      sb,0,cb,0,
                      0,0,0,1);
        //qDebug()<<"Ry:\n"<<Ry;
        QMatrix4x4 Rz(cc,-sc,0,0,
                      sc,cc,0,0,
                      0,0,1,0,
                      0,0,0,1);
        //qDebug()<<"Rz:\n"<<Rz;
        QMatrix4x4 R = ( table[i][0] != 0)* Rx +
                       ( table[i][1] != 0)* Ry +
                       ( table[i][2] != 0)* Rz +
                       (table[i][0] == 0 && table[i][1] == 0 &&table[i][2] == 0)*I;
        //qDebug()<<"R:\n"<<R;
        A = A*T*R;
        //qDebug()<<"A"<<i<<'\n'<<A;
    }
    return A;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    initTable(0,0,0,90,0,0);  //设置设备参数
    int row = (sizeof(TRtable)/sizeof(float)) / (sizeof(TRtable[0])/sizeof(float));//DH table的行数
    qDebug() << "column num: " << sizeof(TRtable[0]) / sizeof(float);
    qDebug() << "row num: " << row;
    qDebug() << TR(TRtable,row);                                                    //打印出变换矩阵
    return a.exec();
}
