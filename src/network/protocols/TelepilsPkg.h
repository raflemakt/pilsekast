// TELEPILS - Telemetriprotokoll for Illuminert Studentorchester

#pragma once

#include <Arduino.h>


struct TelepilsAnnounce {
    uint8_t packet_type;
    uint8_t node_mac_address[6];
    char node_name[32];             // <-- kan gjøres mer effektivt, men vi kan parse disse
    char instrument_type[32];       //     ved å fjerne "leading spaces"
    uint8_t led_strip_led_amount;
    bool has_sound_sensor;
    bool has_icm_sensor;
    bool has_ttgo_screen;
    bool is_access_point;

};

struct TelepilsNodeStatus {
    uint8_t packet_type;
    uint8_t battery_level;
};

struct TelepilsTemperature {
    uint8_t packet_type;
    uint8_t temperature;
};

struct TelepilsNoise {
    uint8_t packet_type;
    uint8_t noise_level;
    uint8_t noise_record;
};

