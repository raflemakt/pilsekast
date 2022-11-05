#include "Arduino.h"
#include "NeoPixelBus.h"
#include <NeoPixelAnimator.h>
#include "LedStripCustom.h"
#include "network/Registers.h"
#include "LedStripCustom.h"
#include "LedStripAdsr.h"

// ADSR
uint8_t Red = oelkast_light_enveloped.color_red;
uint8_t Green = oelkast_light_enveloped.color_green;
uint8_t Blue = oelkast_light_enveloped.color_blue;
RgbColor color = RgbColor(Red, Green, Blue);
HslColor Actual_color = HslColor(color);
float time_attack = oelkast_light_enveloped.env_attack_time;
float time_duration = oelkast_light_enveloped.env_decay_time;
float time_release = oelkast_light_enveloped.env_release_time;
float duration_adsr = oelkast_light_enveloped.duration * 100;
float intensity_adsr = oelkast_light_enveloped.intensity * 0.01;
float level = oelkast_light_enveloped.env_sustain_level;
float function = oelkast_light_enveloped.packet_type;

// Custom
float duration_custom = oelkast_light_simple.duration * 100;
float intensity_custom = oelkast_light_simple.intensity * 0.01;

// void LedStripAdsrUpdate(float intensity, float time_attack, float time_duration, float level, float time_release, float duration);
// uint8_t packet_type;
// uint8_t intensity;
// uint8_t color_red;
// uint8_t color_green;
// uint8_t color_blue;
// uint8_t duration;
// uint8_t env_attack_time;
// uint8_t env_decay_time;
// uint8_t env_sustain_level;
// uint8_t env_release_time;

// uint8_t packet_type;
// uint8_t intensity;
// uint8_t duration;
