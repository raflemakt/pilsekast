#pragma once

#include "configuration.h"

#ifdef HAS_TTGO_SCREEN
#include <TFT_eSPI.h>

// VIKTIG: Biblioteket må konfigureres for riktig skjerm. Dette gjøres i 
//         .pio/libdeps/esp32dev/TFT_eSPI/User_Setup_Select.h
//         Her må riktig linje stå ukommentert, for ESP32 TTGO Lora v1
//         fungerer konfigurasjon 25: 'Setup25_TTGO_T_Display.h'
//         Du må også kommentere ut '#include <User_Setup.h>'

#include "motstanden_logo_120px.h"
#include "motstanden_logo_64px.h"

namespace Screen
{
extern TFT_eSPI tft;

void init();
void display_test_screen();
}




#endif