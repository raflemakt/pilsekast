#pragma once

#include <Arduino.h>


class LedRGB
{
public:
    LedRGB(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b,
           uint8_t pwm_r_channel, uint8_t pwm_g_channel, uint8_t pwm_b_channel,
           uint8_t pwm_resolution, uint16_t pwm_frequency) {

        this->m_pin_r = pin_r;
        this->m_pin_g = pin_g;
        this->m_pin_b = pin_b;
        this->m_pwm_r_channel = pwm_r_channel;
        this->m_pwm_g_channel = pwm_g_channel;
        this->m_pwm_b_channel = pwm_b_channel;
        this->m_pwm_resolution = pwm_resolution;
        this->m_pwm_frequency = pwm_frequency;

        ledcSetup(pwm_r_channel, pwm_frequency, pwm_resolution);
        ledcSetup(pwm_g_channel, pwm_frequency, pwm_resolution);
        ledcSetup(pwm_b_channel, pwm_frequency, pwm_resolution);
        ledcAttachPin(pin_r, pwm_r_channel);
        ledcAttachPin(pin_g, pwm_g_channel);
        ledcAttachPin(pin_b, pwm_b_channel);
        
    }

    void set_color(uint8_t r, uint8_t g, uint8_t b) {
        m_value_r = r;
        m_value_g = g;
        m_value_b = b;
        update_led();
    }

    void set_color(uint8_t hue) {
        hue_to_rgb(hue);
        update_led();
    }

private:
    uint8_t m_pin_r;
    uint8_t m_pin_g;
    uint8_t m_pin_b;

    uint8_t m_pwm_r_channel;
    uint8_t m_pwm_g_channel;
    uint8_t m_pwm_b_channel;
    uint8_t m_pwm_resolution;
    uint16_t m_pwm_frequency;

    uint8_t m_value_r;
    uint8_t m_value_g;
    uint8_t m_value_b;

    void update_led() {
        ledcWrite(m_pwm_r_channel, m_value_r);
        ledcWrite(m_pwm_g_channel, m_value_g);
        ledcWrite(m_pwm_b_channel, m_value_b);
    }

    void hue_to_rgb(uint8_t hue) {
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
