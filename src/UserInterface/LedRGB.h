#pragma once

#include <Arduino.h>

#define PWM_R_CH    4
#define PWM_G_CH    5
#define PWM_B_CH    6
#define PWM_RES     8
#define PWM_FREQ    1000


class LedRGB
{
public:
    LedRGB(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b) {
        this->m_pin_r = pin_r;
        this->m_pin_g = pin_g;
        this->m_pin_b = pin_b;

        ledcSetup(PWM_R_CH, PWM_FREQ, PWM_RES);
        ledcSetup(PWM_G_CH, PWM_FREQ, PWM_RES);
        ledcSetup(PWM_B_CH, PWM_FREQ, PWM_RES);
        ledcAttachPin(pin_r, PWM_R_CH);
        ledcAttachPin(pin_g, PWM_G_CH);
        ledcAttachPin(pin_b, PWM_B_CH);
        
    }

    void set_colors(uint8_t r, uint8_t g, uint8_t b) {
        m_value_r = r;
        m_value_g = g;
        m_value_b = b;
        update_led();
    }

    void set_colors(uint8_t hue) {
        hue_to_rgb(hue);
        update_led();
    }

private:
    uint8_t m_pin_r;
    uint8_t m_pin_g;
    uint8_t m_pin_b;

    uint8_t m_value_r;
    uint8_t m_value_g;
    uint8_t m_value_b;

    void update_led() {
        Serial.print("  writing to LED, RGB: ");
        Serial.print(m_value_r);
        Serial.print(" ");
        Serial.print(m_value_g);
        Serial.print(" ");
        Serial.println(m_value_b);

        ledcWrite(PWM_R_CH, m_value_r);
        ledcWrite(PWM_G_CH, m_value_g);
        ledcWrite(PWM_B_CH, m_value_b);
    }

    void hue_to_rgb(uint8_t hue) {
        Serial.print("hue: ");
        Serial.print(hue);
        if (hue <= 85) {
            m_value_r = (85 - hue) * 3;
            m_value_g = hue * 3;
            m_value_b = 0;
        }
        if (hue >= 86 && hue <= 170) {
            m_value_r = 0;
            m_value_g = (170 - hue) * 3;
            m_value_b = (hue - 86) * 3;
        }
        if (hue >= 171 && hue <= 255) {
            m_value_r = (hue - 171) * 3;
            m_value_g = 0;
            m_value_b = (255 - hue) * 3;
        }
    }
    
};
