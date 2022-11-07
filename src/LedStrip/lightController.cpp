#include "Arduino.h"
#include "NeoPixelBus.h"
#include <NeoPixelAnimator.h>
#include "LedStripCustom.h"
#include "network/Registers.h"
#include "LedStripCustom.h"
#include "LedStripAdsr.h"
#include "LedStripSimpleHue.h"

uint8_t Chosen_Option;

float mapfloat(byte x, float in_min, float in_max, float out_min, float out_max)
{
    return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void lightcontroll_write()
{
    switch (Chosen_Option)
    {
    case 1:
        LedStripAdsrUpdate(mapfloat(oelkast_light_enveloped.intensity, 0, 255, 0, 0.5), (float)oelkast_light_enveloped.env_attack_time * 2, (float)oelkast_light_enveloped.env_decay_time * 2, mapfloat(oelkast_light_enveloped.env_sustain_level, 0, 255, 0, 0.25), (float)oelkast_light_enveloped.env_release_time * 2, (float)oelkast_light_enveloped.duration * 6, oelkast_light_enveloped.color_red, oelkast_light_enveloped.color_green, oelkast_light_enveloped.color_blue);
        break;

    case 2:
        LedStripCustomUpdate(mapfloat(oelkast_light_enveloped.intensity, 0, 255, 0, 0.5), oelkast_light_simple.duration);
        break;

    case 3:
        LedStripSimpleUpdate(mapfloat(oelkast_light_simple_hue.intensity, 0, 255, 0, 0.5));
    }
}