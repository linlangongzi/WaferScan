#ifndef MATLAB_H
#define MATLAB_H

#include <QMainWindow>
#include "engine.h"
#define SIZE 256

namespace Ui {
class matlab;
}

class matlab : public QMainWindow
{
    Q_OBJECT

public:
    explicit matlab(QWidget *parent = 0);
    ~matlab();
private slots:
    void response();//response to matlab command
private:
    Ui::matlab *ui;
    Engine *ep;
    char buffer[SIZE+1];
    bool flag_eng;//用来记录matlab engine是否打开或关闭，true表示打开
};

#endif // MATLAB_H
