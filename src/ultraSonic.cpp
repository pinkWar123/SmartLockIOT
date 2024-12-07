#include "header/ultraSonic.h"

void UltraSonic::Setup(){
    pinMode(Trigger_PIN, OUTPUT);
    pinMode(Echo_PIN, INPUT);
}
int UltraSonic::GetDistance_cm()
{
    digitalWrite(Trigger_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger_PIN, LOW);

    int duration = pulseIn(Echo_PIN, HIGH);

    return (0.034 * duration) / 2.0;
}