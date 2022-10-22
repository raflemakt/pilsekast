// TELEPILS - Telemetriprotokoll for Illuminert Studentorchester

#pragma once

#include <Arduino.h>


struct TelepilsAnnounce {
    uint8_t pkg_header;
    uint8_t[6] node_mac_address;
    char[32] node_name;             // <-- kan gjøres mer effektivt, men vi kan parse disse
    char[32] instrument_type;       //     ved å fjerne "leading spaces"
};

struct TelepilsNodeStatus {
    uint8_t pkg_header;
    uint8_t battery_level;
};

struct TelepilsTemperature {
    uint8_t pkg_header;
    uint8_t temperature;
};

