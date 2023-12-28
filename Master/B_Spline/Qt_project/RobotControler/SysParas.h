#ifndef SYSPARAS_H
#define SYSPARAS_H
#include <QVector>

class SysParas
{
public:
    SysParas();
    QVector<qreal> getMaxJoint() {
        return maxJoint;
    }
    QVector<qreal> getMinJoint(){
        return minJoint;
    }

    QVector<qreal> getMaxTCP(){
        return maxTCP;
    }

    QVector<qreal> getMinTCP(){
        return minTCP;
    }

    QVector<qreal> getOriJoint(){
        return oriJoint;
    }

    QVector<qreal> getOriTCP(){
        return oriTCP;
    }

    void setOriJoint(QVector<qreal> joint){
        oriJoint = joint;
    }
    void setOriTCP(QVector<qreal> tcp){
        oriTCP = tcp;
    }

private:
    QVector<qreal> maxJoint;
    QVector<qreal> minJoint;
    QVector<qreal> oriJoint;

    QVector<qreal> maxTCP;
    QVector<qreal> minTCP;
    QVector<qreal> oriTCP;
};

#endif // SYSPARAS_H
