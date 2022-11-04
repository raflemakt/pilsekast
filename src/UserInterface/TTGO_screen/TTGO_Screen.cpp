#include "UserInterface/TTGO_screen/TTGO_Screen.h"

#ifdef HAS_TTGO_SCREEN

namespace Screen
{
const auto SCR_WIDTH = 320;
const auto SCR_HEIGTH = 170;
const auto BACKGROUND_COLOR = TFT_BLACK;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite motstanden_logo_120 = TFT_eSprite(&tft);
TFT_eSprite motstanden_logo_64 = TFT_eSprite(&tft);

void init() {
    tft.init();
    tft.setRotation(1);

    background.createSprite(SCR_WIDTH, SCR_HEIGTH);
    background.fillSprite(BACKGROUND_COLOR);

    motstanden_logo_120.createSprite(120, 120);
    motstanden_logo_120.setSwapBytes(true);
    motstanden_logo_120.pushImage(0, 0, 120, 120, motstanden_logo_120px);

    motstanden_logo_64.createSprite(64, 64);
    motstanden_logo_64.setSwapBytes(true);
    motstanden_logo_64.pushImage(0, 0, 64, 64, motstanden_logo_64px);
}

void display_test_screen() {
    motstanden_logo_120.pushToSprite(&background, 0, 15, TFT_BLACK);
    motstanden_logo_64.pushToSprite(&background, 133, 33, TFT_BLACK);
    background.pushSprite(0, 0);
    tft.setCursor(0, 0);
    tft.print("Pilsekast ACCESS POINT");
}

}


#endif