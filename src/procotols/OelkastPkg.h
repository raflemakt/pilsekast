// ØLKAST - Øyeblikkelig Lyskringkastingsprotokoll for Studentorchester

#pragma once

#include "Arduino.h"

uint8_t OELKAST_PKG_SIZE;
// bool check_oelkast_pkg_integrity(OelkastPkg pkg);


typedef struct OelkastPkg {
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
} OelkastPkg;


//OelkastPkg oelkast_pkg_from_binary((sizeof(OelkastPkg)*) &binary_pkg);

