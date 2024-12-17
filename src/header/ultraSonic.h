#pragma once
#include "Device.h"

class UltraSonic : public Device
{
private:
    int Trigger_PIN;
    int Echo_PIN;
    bool Ison = false;

    int Led_Pin;

    float GetDistance_cm();
public:
    // Constructor sử dụng initializer list
    UltraSonic(int trig_pin, int echo_pin, int led_pin)
        : Trigger_PIN(trig_pin), Echo_PIN(echo_pin), Led_Pin(led_pin) {}

    void Setup() override;

    void Detect_object();

    // Destructor mặc định không cần thiết nếu không có tài nguyên động
    ~UltraSonic() = default;
};
