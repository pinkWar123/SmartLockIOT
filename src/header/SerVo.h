#pragma once
#include "Device.h"

class SerVo : public Device
{
private:
    int Servo_PIN;
    int Servo_Lock_POS;
    int Servo_Unlock_POS;

    Servo* servo;

public:
    // Constructor sử dụng initializer list
    SerVo(int servo_pin, int Servo_Lock_POS, int Servo_Unlock_POS)
        : Servo_PIN(servo_pin), Servo_Lock_POS(Servo_Lock_POS), Servo_Unlock_POS(Servo_Unlock_POS) {
            servo = new Servo();
            Serial.println((uintptr_t)servo);
        }

    SerVo(const SerVo& other)
        : Servo_PIN(other.Servo_PIN), Servo_Lock_POS(other.Servo_Lock_POS), Servo_Unlock_POS(other.Servo_Unlock_POS) {
            servo = (other.servo);  
            servo->attach(Servo_PIN, 500, 2400);
            Serial.println("copy constructor");
        }
    void Setup() override;

    void Lock();
    void Unlock();
    Servo * GetServo(){
        return servo;
    }
    ~SerVo() = default;
};
