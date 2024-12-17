#include "header/SerVo.h"

void SerVo::Lock()
{
    if (is_locked == true)
        return;
    // Serial.println(Servo_Lock_POS);
    int servopos = 90;
    while (servopos >= 0)
    {
        Roate(servopos--);
        delay(20);
        Serial.println(servopos);
    }
    is_locked = true;
}
void SerVo::Unlock()
{
    if (is_locked == false)
        return;
    Serial.println(Servo_Unlock_POS);
    int servopos = 0;
    while (servopos <= 90)
    {
        Roate(servopos++);
        delay(20);
        Serial.println(servopos);
    }
    is_locked = false;
}
void SerVo::Setup()
{
    servo->attach(Servo_PIN);
    servo->write(Servo_Lock_POS);
    is_locked = true;
}
void SerVo::Roate(int pos)
{
    servo->write(pos);
}