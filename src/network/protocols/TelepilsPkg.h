// TELEPILS - Telemetriprotokoll for Illuminert Studentorchester

#pragma once

#include <Arduino.h>


struct TelepilsAnnounce {
    uint8_t pkg_header;
    uint8_t node_mac_address[6];
    char node_name[32];             // <-- kan gjøres mer effektivt, men vi kan parse disse
    char instrument_type[32];       //     ved å fjerne "leading spaces"
};

struct TelepilsNodeStatus {
    uint8_t pkg_header;
    uint8_t battery_level;
};

struct TelepilsTemperature {
    uint8_t pkg_header;
    uint8_t temperature;
};

struct TelepilsNoise {
    uint8_t pkg_header;
    uint8_t noise_level;
};

