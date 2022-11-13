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
extern TFT_eSprite buffer;  // <-- Dårlig idé å la bruker styre med denne...

extern const uint16_t SCR_HEIGTH;
extern const uint16_t SCR_WIDTH;

void init();
void update();
void display_window(const char *window_name, uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t heigth);
void display_generic_background(const char* header_text);
void display_test_screen();
void display_info_screen();
void display_adsr_screen();
void display_adsrd_envelope_transient_component(uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t heigth);

}




#endif