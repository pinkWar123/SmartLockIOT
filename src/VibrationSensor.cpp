#include "header/Vibration_Sensor.h"


float VibrationSensor::Get_Vibrant_Intensity()
{
    float value = analogRead(Analog_Pin);
    Serial.println(value);
    return value;
}
bool VibrationSensor::Is_Vibrant() 
{
    if(is_Digital)
        return digitalRead(Digital_PIN) == LOW;
    else
        return Get_Vibrant_Intensity() == 1023;
}



void VibrationSensor::Detect_Vibrant()
{
    unsigned long currentTime = millis();
    if(Is_Vibrant()  && (currentTime - lastCheckTime >= 10000  || lastCheckTime == 0) && servo->get_Is_locked())
    {
        buzzer->Sound(10000);
        lastCheckTime = currentTime;

        // send MQTT here
    }
    else
    {
    }
}