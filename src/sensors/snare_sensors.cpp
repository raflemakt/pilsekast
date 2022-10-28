#include <Arduino.h>

const int drum = 33;
const unsigned long waitTime = 200;
const int minHit = 100;
const int maxHit = 1000;

int hitSignal;
int hitIntensity;
unsigned long hitWait = 0;
int maxSignal = 0;
bool inHit = false;

int getDrumSensor()
{
    // put your main code here, to run repeatedly:
    if (millis() - hitWait > waitTime)
    {
        hitSignal = analogRead(drum);
        if (hitSignal >= minHit)
        {
            inHit = true;
            if (hitSignal > maxSignal)
            {
                maxSignal = hitSignal;
            }
        }
        else
        {
            if (inHit)
            {
                if (maxSignal > maxHit)
                {
                    maxSignal = maxHit;
                }
                hitIntensity = map(maxSignal, minHit, maxHit, 1, 100);
                Serial.print("Hit intensity : ");
                Serial.println(hitIntensity);
                inHit = false;
                maxSignal = 0;
                hitWait = millis();
                return hitIntensity;
            }
        }
    }
    return 0;
}
