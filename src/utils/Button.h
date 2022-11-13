#include <Arduino.h>

// FIXME: Denne klassen fungerer ikke helt. Bruker ezButton enn så lenge.
class Button
{
private:
    bool button_reading;
    uint8_t pin;
    unsigned long t;
    unsigned long t_0;
    const uint8_t bounce_delay = 5;

public:
    bool state_has_changed;
    bool pressed_this_cycle;
    bool pressed;

    enum State : uint8_t {
        RESET = 0,
        START,
        FIRST_LOW,
        WAIT,
        PRESSED,
        RELEASED
    };
    State state = State::RESET;
    State state_prev;

    Button(uint8_t pin) {
        this->pin = pin;
        Init();
    }

    void Init() {
        pinMode(pin, INPUT_PULLUP);
    }

    void Update() {
        state_prev = state;

        switch (state) {
            case State::RESET:
                state = State::START;
            break;

            case State::START:
                button_reading = digitalRead(pin);
                if (button_reading == LOW) {
                    state = State::FIRST_LOW;
                }
            break;

            case State::FIRST_LOW:
                t_0 = millis();
                state = State::WAIT;
            break;

            case State::WAIT:
                button_reading = digitalRead(pin);
                t = millis();
                if (button_reading == LOW) {
                    state = State::RESET;
                }
                if (t - t_0 > bounce_delay) {
                    state = State::PRESSED;
                }
            break;

            case State::PRESSED:
                button_reading = digitalRead(pin);
                t_0 = millis();
                if (button_reading == HIGH) {
                    state = State::RELEASED;
                }
            break;

            case State::RELEASED:
                t = millis();
                if (t - t_0 > bounce_delay) {
                    state = State::RESET;
                }
            break;
        }
        state_has_changed = (state_prev != state);
        pressed_this_cycle = state_has_changed && (state == State::PRESSED);
        pressed = (state == State::PRESSED);
    }
};