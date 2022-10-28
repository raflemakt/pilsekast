#include <Arduino.h>
#include "AP/access_point.h"
#include "../configuration.h"
#include "network/LocalNetworkInterface.h"
#include "UserInterface/StringFormatters.h"
#include "UserInterface/Messages.h"
#include "sensors/snare_sensors.h"

struct TestData
{
    uint8_t a;
    uint8_t b;
    uint16_t c;
};
TestData test_data;
uint8_t test_data_array[sizeof(TestData)];

namespace AccessPoint
{
    void on_local_data_receive()
    {
        Serial.println("  on_local_data_receive called");
    }

    void on_local_data_send()
    {
        Serial.println("  on_local_data_send called");
    }

    void setup()
    {
        pinMode(0, INPUT_PULLUP);

        Messages::on_boot();
        LocalNetworkInterface::initialize();
        LocalNetworkInterface::register_recv_callback(on_local_data_receive);
        LocalNetworkInterface::register_send_callback(on_local_data_send);
        Serial.println("Init complete from access_point.cpp --> starting loop.\n");

        test_data.a = 0;
        test_data.b = 123;
        test_data.c = 45678;
    }

    void main()
    {
        uint8_t drum_reading = getDrumSensor();
        test_data.b = drum_reading;
        // if(!digitalRead(0)) {
        if (drum_reading > 0)
        {
            memcpy(test_data_array, &test_data, sizeof(test_data));
            LocalNetworkInterface::send_binary_package(0, test_data_array, sizeof(test_data));
            test_data.a++;

            // we have button debounce at home
            // delay(150); // <-- button debounce at home
        }
    }
}