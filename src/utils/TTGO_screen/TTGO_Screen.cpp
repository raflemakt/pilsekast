#include <Arduino.h>
#include "utils/TTGO_screen/TTGO_Screen.h"
#include "utils/StringFormatters.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"

#ifdef HAS_TTGO_SCREEN

/* ======= velkommen til skjermkoden =======
* Her finner du mange magiske tall og annet fælt. En vakker dag vil vi kanskje
* refaktorisere denne koden. Kanskje til en skikkelig skjermprotokoll...
* Hva med wayland?
*
*/


namespace Screen
{
const uint32_t TTGO_SCREEN_UPDATE_FPS = 15;
const uint32_t TTGO_SCREEN_UPDATE_DELAY_MS = 1000 / TTGO_SCREEN_UPDATE_FPS;
const uint16_t SCR_WIDTH = 239;     // Nullindeksert
const uint16_t SCR_HEIGTH = 135;    // Nullindeksert
const auto BACKGROUND_COLOR = TFT_BLACK;
const uint32_t TFT_MOTSTANDEN_GREEN = 13100;
const uint32_t TFT_MOTSTANDEN_BLUE = 13371;

uint32_t last_screen_update_timer;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite buffer = TFT_eSprite(&tft);
TFT_eSprite envelope_window = TFT_eSprite(&tft);
TFT_eSprite pkg_stat_window = TFT_eSprite(&tft);
TFT_eSprite generic_window = TFT_eSprite(&tft);
TFT_eSprite motstanden_logo_120 = TFT_eSprite(&tft);
TFT_eSprite motstanden_logo_64 = TFT_eSprite(&tft);

void configure_sprites() {
    buffer.createSprite(SCR_WIDTH+1, SCR_HEIGTH+1);
    buffer.fillSprite(BACKGROUND_COLOR);

    motstanden_logo_120.createSprite(120, 120);
    motstanden_logo_120.setSwapBytes(true);
    motstanden_logo_120.pushImage(0, 0, 120, 120, motstanden_logo_120px);

    motstanden_logo_64.createSprite(64, 64);
    motstanden_logo_64.setSwapBytes(true);
    motstanden_logo_64.pushImage(0, 0, 64, 64, motstanden_logo_64px);
}

void init() {
    tft.init();
    tft.setRotation(1);
    configure_sprites();
}

void push() {
    buffer.pushSprite(0, 0);
}

void update() {
    if (millis() - last_screen_update_timer > 30){//TTGO_SCREEN_UPDATE_DELAY_MS) {
        buffer.pushSprite(0, 0);
        last_screen_update_timer = millis();
    }
}

void display_window(const char *window_name, uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t heigth) {
    const auto window_decoration_heigth = 9;
    const auto window_border_color = TFT_RED;

    generic_window.createSprite(width, heigth);
    generic_window.fillSprite(BACKGROUND_COLOR);
    generic_window.drawRect(0, 0, width, heigth, window_border_color);
    generic_window.drawFastHLine(0, window_decoration_heigth, width, window_border_color);
    generic_window.drawString(window_name, 3, 1, 1);
    generic_window.pushToSprite(&buffer, origin_x, origin_y);
}

void display_adsrd_envelope_transient_component(uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t heigth) {
    auto attack = oelkast_light_enveloped.env_attack_time;
    auto decay = oelkast_light_enveloped.env_decay_time;
    auto sustain = oelkast_light_enveloped.env_sustain_level;
    auto release = oelkast_light_enveloped.env_release_time;
    auto duration = oelkast_light_enveloped.duration;
    auto graph_color = TFT_RED;

    envelope_window.createSprite(width, heigth);
    envelope_window.fillSprite(BACKGROUND_COLOR);
    envelope_window.drawRect(0, 0, width, heigth, TFT_MOTSTANDEN_BLUE);

    // Attack line from zero to max
    auto attack_end_x = attack*width/256/4;  // 256: sizeof(uint8_t),   4: parts of the transient
    envelope_window.drawLine(0, heigth, attack_end_x, 0, graph_color);

    // Decay line down to sustain level
    auto decay_end_x = attack_end_x + (decay*width/256/4);
    auto sustain_y = (256-sustain)*heigth/256;
    envelope_window.drawLine(attack_end_x, 0, decay_end_x, sustain_y, graph_color);

    // Sustain line, horizontal
    auto duration_length = duration*width/256/4;
    envelope_window.drawFastHLine(decay_end_x, sustain_y, duration_length, graph_color);

    // Release line down to zero
    auto duration_end_x = decay_end_x + duration_length;
    auto release_end_x = duration_end_x + (release*width/256/4);
    envelope_window.drawLine(duration_end_x, sustain_y, release_end_x, heigth, graph_color);

    // Draw to background
    envelope_window.pushToSprite(&buffer, origin_x, origin_y);
}

void display_pkg_stat_component(uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t heigth) {
    auto pad_x = 3;
    auto pad_y = 3;
    auto col_width = (2*pad_x + width) / 4;
    auto col_pkg_x = col_width + pad_x;
    auto col_byte_x = col_pkg_x + col_width;
    auto col_failed_x = col_byte_x + col_width;
    auto row_heigth_y = 9;

    pkg_stat_window.createSprite(width, heigth);
    pkg_stat_window.fillSprite(BACKGROUND_COLOR);
    pkg_stat_window.drawRect(0, 0, width, heigth, TFT_MOTSTANDEN_BLUE);

    // Column 1
    pkg_stat_window.drawString("TOT:", pad_x, pad_y, 1);
    pkg_stat_window.drawString("Ut:",  pad_x, pad_y + row_heigth_y, 1);
    pkg_stat_window.drawString("Inn:", pad_x, pad_y + (row_heigth_y*2), 1);

    // Column 2
    pkg_stat_window.drawString("pkg:", col_pkg_x, pad_y, 1);
    pkg_stat_window.drawString(String(LocalNetworkInterface::transmission_stats.total_pkg_sent),     col_pkg_x, pad_y + row_heigth_y, 1);
    pkg_stat_window.drawString(String(LocalNetworkInterface::transmission_stats.total_pkg_received), col_pkg_x, pad_y + (row_heigth_y*2), 1);

    // Column 3
    pkg_stat_window.drawString("byte:", col_byte_x, pad_y, 1);
    pkg_stat_window.drawString(String(LocalNetworkInterface::transmission_stats.total_bytes_sent),     col_byte_x, pad_y + row_heigth_y, 1);
    pkg_stat_window.drawString(String(LocalNetworkInterface::transmission_stats.total_bytes_received), col_byte_x, pad_y + (row_heigth_y*2), 1);

    // Column 4
    pkg_stat_window.drawString("tapt:", col_failed_x, pad_y, 1);
    pkg_stat_window.drawString(String(LocalNetworkInterface::transmission_stats.total_pkg_send_fails),     col_failed_x, pad_y + row_heigth_y, 1);
    pkg_stat_window.drawString("-", col_failed_x, pad_y + (row_heigth_y*2), 1);

    // Draw to background
    pkg_stat_window.pushToSprite(&buffer, origin_x, origin_y);
}

void display_generic_background(const char* header_text) {
    buffer.fillSprite(BACKGROUND_COLOR);
    buffer.drawRect(0, 0, SCR_WIDTH, SCR_HEIGTH, TFT_MOTSTANDEN_GREEN);
    motstanden_logo_64.pushToSprite(&buffer, SCR_WIDTH-66, 3, TFT_BLACK);
    buffer.drawString(header_text, 4, 0, 1);
}

void display_info_screen() {
    buffer.fillSprite(BACKGROUND_COLOR);
    motstanden_logo_120.pushToSprite(&buffer, 4, 12, TFT_BLACK);
    buffer.drawRect(0, 0, SCR_WIDTH, SCR_HEIGTH, TFT_MOTSTANDEN_GREEN);
    #ifdef IS_ACCESS_POINT
    buffer.drawString("Pilsekast AKSESSPUNKT", 4, 0, 1);
    #else
    buffer.drawString("Pilsekast INSTRUMENTNODE", 4, 0, 1);
    #endif
    buffer.drawString("MAC: ", 130, 20, 1);
    buffer.drawString(Format::mac_addr_from_array(LocalNetworkInterface::my_mac_address), 130, 29, 1);

    buffer.drawString("Nodenavn:", 130, 50, 1);
    buffer.drawString(NODE_NAME, 130, 59, 1);

    buffer.drawString("Instrumenttype:", 130, 70, 1);
    buffer.drawString(INSTRUMENT_TYPE, 130, 79, 1);
}

void display_adsr_screen() {
    display_generic_background("Modus: ADSR_FROM_POTMETERS_ON_HIT");
    buffer.drawString("env_attack_time: ", 10, 12, 1); 
    buffer.drawString(String(oelkast_light_enveloped.env_attack_time), 125, 12, 1);
    buffer.drawString("env_decay_time: ", 10, 22, 1); 
    buffer.drawString(String(oelkast_light_enveloped.env_decay_time), 125, 22, 1);
    buffer.drawString("env_sustain_level: ", 10, 32, 1); 
    buffer.drawString(String(oelkast_light_enveloped.env_sustain_level), 125, 32, 1);
    buffer.drawString("env_release_time: ", 10, 42, 1); 
    buffer.drawString(String(oelkast_light_enveloped.env_release_time), 125, 42, 1);
    buffer.drawString("duration: ", 10, 52, 1); 
    buffer.drawString(String(oelkast_light_enveloped.duration), 125, 52, 1);
    display_adsrd_envelope_transient_component(5, 70, 229, 60);
}

void display_test_screen() {
    motstanden_logo_120.pushToSprite(&buffer, 4, 12, TFT_BLACK);
    motstanden_logo_64.pushToSprite(&buffer, 133, 33, TFT_BLACK);
    tft.drawRect(0, 0, 237, 135, TFT_MOTSTANDEN_GREEN);  // Heile skjermen (238x136 ??)
    tft.setCursor(0, 0);
    tft.print("Pilsekast AKSESSPUNKT");
}

}


#endif