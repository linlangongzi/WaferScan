#ifndef PID_H
#define PID_H


class PID
{
public:
    PID();
    PID(float p,float i,float d);
    void SetP(float p);                       //单独设置PID参数
    void SetI(float i);
    void SetD(float d);
    float PID_out(float goal_,float actual_); //给定当前值及目标值，返回经过一次PID后的输出值
    float ShowErr();
private:
    float goal;             //期望值
    float actual;           //当前值
    float err;              //当前偏差
    float err_next;         //上一次偏差
    float err_last;         //上上一次偏差
    float Kp,Ki,Kd;         //PID参数
    float inter;
};

class Block
{
public:
    Block();
    Block(float weight_);
    float GetPosition(float force);
    float GetSpeed();
private:
    float weight;
    float actualPosition;
    float speed;
    float acceleration;
};
#endif // PID_H
