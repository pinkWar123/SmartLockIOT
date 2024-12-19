#include "header/FlameSensor.h"

int FlameSensor::Get_Flame_intensity()
{
    return analogRead(Analog_PIN);
}

bool FlameSensor::Is_Flame()
{
    if(IsDigital)
        return digitalRead(Digital_PIN) == LOW;
    
    return Get_Flame_intensity() > 700;
}



void FlameSensor::Detect_Flame()
{
    unsigned long currentTime = millis();
    if(Is_Flame() && currentTime - lastCheckTime >= 2000)
    {
        if(servo->get_Is_locked() == true)
            servo->Unlock();
        delay(1000);
        buzzer->Sound(5000);
        delay(1000);
        lastCheckTime = currentTime;

        // Send MQTT here
    }
}