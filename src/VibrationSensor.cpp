#include "header/Vibration_Sensor.h"


bool VibrationSensor::Is_Vibrant() 
{
    return digitalRead(Digital_PIN) == HIGH;
}