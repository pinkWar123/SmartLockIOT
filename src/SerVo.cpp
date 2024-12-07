#include "header/SerVo.h"


void SerVo::Lock()
{
    servo.write(Servo_Lock_POS);
}
void SerVo::Unlock()
{
    servo.write(Servo_Unlock_POS);
}
void SerVo::Setup()
{
    servo.attach(Servo_PIN);
    Lock();
}