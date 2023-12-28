#ifndef MANIPULATORSOLVER_H
#define MANIPULATORSOLVER_H
#include <QVector>

class ManipulatorSolver
{
public:
    ManipulatorSolver();
    QVector<float> forwardSolver(QVector<float> joint);
    QVector<float> inverseSolver(QVector<float> tcp);
private:
    QVector<float> joints;
    QVector<float> TCP;
};

#endif // MANIPULATORSOLVER_H
