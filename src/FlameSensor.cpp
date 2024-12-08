#include "header/FlameSensor.h"

int FlameSensor::Get_Flame_intensity()
{
    return analogRead(Analog_PIN);
}

bool FlameSensor::Is_Flame()
{
    if(IsDigital)
        return digitalRead(Digital_PIN) == HIGH;
    
    return Get_Flame_intensity() > 0;
}



