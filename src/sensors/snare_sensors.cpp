#include <Arduino.h>

// Sensor constants
const int drum = 33;
const unsigned long waitTime = 200;
const int minHit = 100;
const int maxHit = 2000;

// Sensor Variables
int hitSignal;
int hitIntensity;
unsigned long hitWait = 0;
int maxSignal = 0;
bool inHit = false;

// main function
uint8_t getDrumSensor()
{
    // The sensor only samples data every x milliseconds
    if (millis() - hitWait > waitTime)
    {
        hitSignal = analogRead(drum); // Reads sensor
        if (hitSignal >= minHit)
        {
            inHit = true;
            if (hitSignal > maxSignal) // Only returns the highest signal in the timeframe
            {
                maxSignal = hitSignal;
            }
        }
        else
        {
            if (inHit)
            {
                if (maxSignal > maxHit) // reverts to a max value if signal is very high
                {
                    maxSignal = maxHit;
                }
                hitIntensity = map(maxSignal, minHit, maxHit, 1, 255); // maps to 8-bit
                // Serial.print("Hit intensity : ");
                // Serial.println(hitIntensity);

                // reset data
                inHit = false;
                maxSignal = 0;
                hitWait = millis();

                // convert and return hit intensity
                uint8_t hitIntensityx = hitIntensity;
                return hitIntensityx;
            }
        }
    }
    return 0; // returns 0 if outside timing window
}
