// ØLKAST - Øyeblikkelig Lyskringkastingsprotokoll for Studentorchester

#pragma once

#include <Arduino.h>


struct OelkastLightSimple {
    uint8_t packet_type;
    uint8_t intensity;
    uint8_t color_red;
    uint8_t color_green;
    uint8_t color_blue;
};

struct OelkastLightEnveloped {
    uint8_t packet_type;
    uint8_t intensity;
    uint8_t color_red;
    uint8_t color_green;
    uint8_t color_blue;
    uint8_t duration;
    uint8_t env_attack_time;
    uint8_t env_decay_time;
    uint8_t env_sustain_level;
    uint8_t env_release_time;
};

struct OelkastLightAnimationMode {
    uint8_t packet_type;
    uint8_t animation_mode;
    uint8_t parameter_1;    // <--- hmmm
    uint8_t parameter_2;
    uint8_t parameter_3;
};

