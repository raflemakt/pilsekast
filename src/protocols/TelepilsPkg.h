// TELEPILS - Telemetrisamband for Praktfult Illuminert Studentorchester

#pragma once

#include "Arduino.h"

typedef struct TelepilsPkg {
    uint8_t pkg_header;
    uint8_t test_data;
} TelepilsPkg;

