#pragma once

#include <Arduino.h>

class SoundSensor
{
public:
    SoundSensor(int pin)
    {
        this->mic_pin = pin;
    }
    const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
    unsigned int sample;
    int mic_pin;
    int sig;

    uint8_t getNoise()
    {

        unsigned long startMillis = millis(); // Start of sample window
        unsigned int peakToPeak = 0;          // peak-to-peak level

        unsigned int signalMax = 0;
        unsigned int signalMin = 4095;

        // collect data for 50 mS
        while (millis() - startMillis < sampleWindow)
        {
            sample = analogRead(mic_pin);
            if (sample < 4095) // toss out spurious readings
            {
                if (sample > signalMax)
                {
                    signalMax = sample; // save just the max levels
                }
                else if (sample < signalMin)
                {
                    signalMin = sample; // save just the min levels
                }
            }
        }
        peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
        sig = map(peakToPeak, 0, 4095, 0, 255);
        uint8_t sig_uint8_t = sig;

        return sig_uint8_t;
    }
};
/*
class SoundSensor {
  public:
    const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
    unsigned int sample;
    const int mic = 32;
    int sig;
    unsigned long lastMillis// sample window

    uint8_t getNoise() {
      unsigned int peakToPeak = 0;   // peak-to-peak level
      unsigned int signalMax = 0;
      unsigned int signalMin = 4095;

      // collect data for 50 mS
      if (millis() - lastMillis >= sampleWindow)
      {
        sample = analogRead(mic);
        if (sample < 4095)  // toss out spurious readings
        {
          if (sample > signalMax)
          {
            signalMax = sample;  // save just the max levels
          }
          else if (sample < signalMin)
          {
            signalMin = sample;  // save just the min levels
          }
        }
      }
      lastMillis = millis();
      peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
      sig = map(peakToPeak, 0, 4095, 0, 255);
      uint8_t sig_uint8_t = sig;

      return sig_uint8_t;

    }

    uint8_t getNoiseScore() {

      unsigned long lasttMillisSecond = millis();
      unsigned long lasttMillisReading = millis();
      int sumOfReadings;
      if (millis() - lastMillisSecond >= 10000) {
        int readings[200];
        lastMillisSecond = millis();
        int readingNo = 0;
        if (millis() - lastMillisReading >= 50) {
          int readings[readingNo] = getNoise();
          readingNo ++;
          lastMillisReading = millis();
        }
        for (int reading : readings) {
          sumOfReadings += reading;
        }
        int noiseScore
        return noiseScore

      }

    }
};
*/