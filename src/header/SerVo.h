#pragma once
#include "Device.h"

class SerVo : public Device
{
private:
    int Servo_PIN;
    int Servo_Lock_POS;
    int Servo_Unlock_POS;

    Servo servo;

public:
    // Constructor sử dụng initializer list
    SerVo(int servo_pin, int Servo_Lock_POS, int Servo_Unlock_POS)
        : Servo_PIN(servo_pin), Servo_Lock_POS(Servo_Lock_POS), Servo_Unlock_POS(Servo_Unlock_POS)  {}

    void Setup() override;


    void Lock();
    void Unlock();
    ~SerVo() = default;
};
