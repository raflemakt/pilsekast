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
  int mic_pin = 32;
  int sig;
  unsigned long sampleMillis; // sample window

  unsigned int peakToPeak = 0; // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 4095;

  uint8_t getNoise()
  {

    // collect data for 50 mS
    if (millis() - sampleMillis < sampleWindow)
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
      return (0);
    }
    else
    {
      peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
      sig = map(peakToPeak, 0, 4095, 0, 255);
      uint8_t sig_uint8_t = sig;

      sampleMillis = millis();
      peakToPeak = 0;
      signalMax = 0;
      signalMin = 4095;

      return sig_uint8_t;
    }
  }
};