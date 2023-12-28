#include <QCoreApplication>
#include "pid.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Block obj(101);          //定义一个10kg的重物
    PID pid(10,0,0);         //定义一个PID器
    float goal = 100;
    float force = 0;
    float err = 1;
    float actual = 0;
    cout << actual << endl;
    int count = 0;
    cout << "actual position is: " << actual << endl;

    while((err > 0.01 || err < -0.01) && count < 100)
    {
        count ++;
        force = pid.PID_out(goal,actual);
        cout << "                   force is: " << force << endl;
        actual = obj.GetPosition(force);
        err = goal - actual;
        cout << " error is: " << err << endl;
        cout << " the speed is: " << obj.GetSpeed() << endl;
    }
    return a.exec();
}
