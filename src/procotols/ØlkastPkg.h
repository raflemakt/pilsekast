// ØLKAST - Øyeblikkelig Lyskringkastingsprotokoll for Studentorchester

#pragma once

#include "Arduino.h"

uint8_t ØLKAST_PKG_SIZE;
bool check_ølkast_pkg_integrity(ØlkastPkg pkg);


typedef struct ØlkastPkg {
    uint8_t pkg_header;
    uint8_t animation_mode;
    uint8_t color_r;
    uint8_t color_g;
    uint8_t color_b;
    uint8_t intensity;
    uint8_t duration;
    uint8_t envelope_a;
    uint8_t envelope_d;
    uint8_t envelope_s;
    uint8_t envelope_r;
} ØlkastPkg;


ØlkastPkg ølkast_pkg_from_binary((sizeof(ØlkastPkg)*) &binary_pkg);

