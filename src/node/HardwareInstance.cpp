#include "node/HardwareInstance.h"


const uint8_t LED_R_PIN = 21;
const uint8_t LED_G_PIN = 22;
const uint8_t LED_B_PIN = 17;
const uint8_t PWM_R_CH = 4;
const uint8_t PWM_G_CH = 5;
const uint8_t PWM_B_CH = 6;
const uint8_t PWM_RES = 8;
const uint16_t PWM_FREQ = 1000;

ezButton button_zero(0);
LedRGB test_led(LED_R_PIN, LED_G_PIN, LED_B_PIN, PWM_R_CH, PWM_G_CH, PWM_B_CH, PWM_RES, PWM_FREQ);

