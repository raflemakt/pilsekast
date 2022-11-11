#include "UserInterface/TTGO_screen/TTGO_Screen.h"
#include "UserInterface/StringFormatters.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"

#ifdef HAS_TTGO_SCREEN

namespace Screen
{
const uint16_t SCR_WIDTH = 239;     // Nullindeksert
const uint16_t SCR_HEIGTH = 135;    // Nullindeksert
const auto BACKGROUND_COLOR = TFT_BLACK;
const uint32_t TFT_MOTSTANDEN_GREEN = 13100;
const uint32_t TFT_MOTSTANDEN_BLUE = 13371;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite envelope_window = TFT_eSprite(&tft);
TFT_eSprite generic_window = TFT_eSprite(&tft);
TFT_eSprite motstanden_logo_120 = TFT_eSprite(&tft);
TFT_eSprite motstanden_logo_64 = TFT_eSprite(&tft);

void configure_sprites() {
    background.createSprite(SCR_WIDTH+1, SCR_HEIGTH+1);
    background.fillSprite(BACKGROUND_COLOR);

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

void display_window(const char *window_name, uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t heigth) {
    const auto window_decoration_heigth = 9;
    const auto window_border_color = TFT_RED;

    generic_window.createSprite(width, heigth);
    generic_window.fillSprite(BACKGROUND_COLOR);
    generic_window.drawRect(0, 0, width, heigth, window_border_color);
    generic_window.drawFastHLine(0, window_decoration_heigth, width, window_border_color);
    generic_window.drawString(window_name, 3, 1, 1);
    generic_window.pushToSprite(&background, origin_x, origin_y);
    background.pushSprite(0, 0);
}

void display_adsrd_envelope_transient(uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t heigth) {
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

    // Draw the sprite
    envelope_window.pushToSprite(&background, origin_x, origin_y);
    background.pushSprite(0, 0);
}

void display_info_screen() {
    background.fillSprite(BACKGROUND_COLOR);
    motstanden_logo_120.pushToSprite(&background, 4, 12, TFT_BLACK);
    background.drawRect(0, 0, SCR_WIDTH, SCR_HEIGTH, TFT_MOTSTANDEN_GREEN);
    #ifdef IS_ACCESS_POINT
    background.drawString("Pilsekast AKSESSPUNKT", 4, 0, 1);
    #else
    background.drawString("Pilsekast INSTRUMENTNODE", 4, 0, 1);
    #endif
    background.drawString("MAC: ", 130, 20, 1);
    background.drawString(Format::mac_addr_from_array(LocalNetworkInterface::my_mac_address), 130, 29, 1);

    background.drawString("Nodenavn:", 130, 50, 1);
    background.drawString(NODE_NAME, 130, 59, 1);

    background.drawString("Instrumenttype:", 130, 70, 1);
    background.drawString(INSTRUMENT_TYPE, 130, 79, 1);
    background.pushSprite(0, 0);
}

void display_test_screen() {
    motstanden_logo_120.pushToSprite(&background, 4, 12, TFT_BLACK);
    motstanden_logo_64.pushToSprite(&background, 133, 33, TFT_BLACK);
    background.pushSprite(0, 0);
    tft.drawRect(0, 0, 237, 135, TFT_MOTSTANDEN_GREEN);  // Heile skjermen (238x136 ??)
    tft.setCursor(0, 0);
    tft.print("Pilsekast AKSESSPUNKT");
}

}


#endif