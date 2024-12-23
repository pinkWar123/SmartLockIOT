#pragma once
#include "Device.h"

class SerVo : public Device
{
private:
    int Servo_PIN;
    int Servo_Lock_POS;
    int Servo_Unlock_POS;

    bool is_locked = false;

    Servo *servo;

public:
    // Constructor sử dụng initializer list
    SerVo(int servo_pin, int Servo_Lock_POS, int Servo_Unlock_POS)
        : Servo_PIN(servo_pin), Servo_Lock_POS(Servo_Lock_POS), Servo_Unlock_POS(Servo_Unlock_POS)
    {
        servo = new Servo();
        is_locked = false;
        Setup();
    }

    void Setup() override;

    void Lock(bool publish);
    void Unlock(bool publish);

    void Roate(int pos);

    bool get_Is_locked() { return is_locked; }
    ~SerVo() = default;
};
