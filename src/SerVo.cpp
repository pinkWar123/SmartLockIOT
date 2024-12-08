#include "header/SerVo.h"

void SerVo::Lock()
{
    Serial.println(Servo_Lock_POS);
    servo->write(Servo_Lock_POS);
}
void SerVo::Unlock()
{
    Serial.println(Servo_Unlock_POS);
    servo->write(Servo_Unlock_POS);
}
void SerVo::Setup()
{
    servo->attach(Servo_PIN);
    Lock();
}