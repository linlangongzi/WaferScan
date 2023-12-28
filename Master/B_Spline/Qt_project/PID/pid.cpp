#include "pid.h"

PID::PID()
{
    goal = 0;
    actual = 0;
    err = 0;
    err_next = 0;
    err_last = 0;
    inter = 0;
    Kp = 1;             //默认参数
    Ki = 1;
    Kd = 1;
}

PID::PID(float p,float i,float d)
{
    goal = 0;
    actual = 0;
    err = 0;
    err_next = 0;
    err_last = 0;
    inter = 0;
    Kp = p;             //设置参数
    Ki = i;
    Kd = d;
}

float PID::PID_out(float goal_, float actual_)
{
    goal = goal_;
    actual = actual_;
    err = goal - actual;
    inter += err;
    float increment = Kp * ( err ) + Ki * inter + Kd * ( err - err_next );
    actual = increment;
    err_next = err;
//    err_last = err_next;
//    actual = actual>100?100:actual;
//    actual = actual<-100?-100:actual;
    return actual;
}

void PID::SetP(float p)
{
    Kp = p;
}

void PID::SetI(float i)
{
    Ki = i;
}

void PID::SetD(float d)
{
    Kd = d;
}

float PID::ShowErr()
{
    return err;
}

Block::Block()
{

}
Block::Block(float weight_)
{
    weight = weight_;
    actualPosition = 0;
    speed = 0;
    acceleration = 0;
}
float Block::GetPosition(float force)
{
    acceleration = force / weight;
    speed += acceleration * 1;
    speed = speed>21?21:speed;
    speed = speed<-21?-21:speed;
    actualPosition += speed * 1;
    return actualPosition;
}
float Block::GetSpeed()
{
    return speed;
}
