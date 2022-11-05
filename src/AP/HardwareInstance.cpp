#include "AP/HardwareInstance.h"

const uint8_t POTMETER_A_PIN = 36;
const uint8_t POTMETER_B_PIN = 37;
const uint8_t POTMETER_C_PIN = 38;
const uint8_t POTMETER_D_PIN = 39;
const uint8_t POTMETER_E_PIN = 32;
const uint8_t LED_R_PIN = 21;
const uint8_t LED_G_PIN = 22;
const uint8_t LED_B_PIN = 17;
const uint8_t PWM_R_CH = 4;
const uint8_t PWM_G_CH = 5;
const uint8_t PWM_B_CH = 6;
const uint8_t PWM_RES = 8;
const uint16_t PWM_FREQ = 1000;


ezButton button_bottom(0);
Potmeter potmeter_a(POTMETER_A_PIN);
Potmeter potmeter_b(POTMETER_B_PIN);
Potmeter potmeter_c(POTMETER_C_PIN);
Potmeter potmeter_d(POTMETER_D_PIN);
Potmeter potmeter_e(POTMETER_E_PIN);
LedRGB led_a(LED_R_PIN, LED_G_PIN, LED_B_PIN, PWM_R_CH, PWM_G_CH, PWM_B_CH, PWM_RES, PWM_FREQ);
