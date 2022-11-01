#pragma once

#include <Arduino.h>

class Potmeter
{
public:
    Potmeter(uint8_t pin) {
        this->m_pin = pin;
    }

    uint16_t read_raw() {
        return analogRead(m_pin);
    }

    uint8_t read() {
        return analogRead(m_pin) / 16;
    }

private:
    uint8_t m_pin;
};