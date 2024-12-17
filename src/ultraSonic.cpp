#include "header/ultraSonic.h"

void UltraSonic::Setup(){
    pinMode(Trigger_PIN, OUTPUT);
    pinMode(Echo_PIN, INPUT);
}
void UltraSonic::Detect_object()
{
    float distance = GetDistance_cm();
    if(distance == 0)
        return;
    if(distance <= 40)
    {
        if(Ison)
            return;
        Ison = true;
        digitalWrite(Led_Pin, HIGH);
    }
    else
    {
        if(!Ison)
            return;
        
        Ison = false;
        digitalWrite(Led_Pin, LOW);
    }
}
float UltraSonic::GetDistance_cm()
{
    digitalWrite(Trigger_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger_PIN, LOW);

    float duration = pulseIn(Echo_PIN, HIGH);

    return (0.034 * duration) / 2.0;
}