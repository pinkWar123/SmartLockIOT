#pragma once
#include "Device.h"

class UltraSonic : public Device
{
private:
    int Trigger_PIN;
    int Echo_PIN;

public:
    // Constructor sử dụng initializer list
    UltraSonic(int trig_pin, int echo_pin)
        : Trigger_PIN(trig_pin), Echo_PIN(echo_pin) {}

    void Setup() override;

    int GetDistance_cm();

    // Destructor mặc định không cần thiết nếu không có tài nguyên động
    ~UltraSonic() = default;
};
