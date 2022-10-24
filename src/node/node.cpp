#include <Arduino.h>
#include "LedStrip/LedStripShow.h"
#include "network/LocalNetworkInterface.h"
#include "LedStrip/LedStripCustom.h"

namespace Node
{
    void on_local_data_receive()
    {
        Serial.print("Received data: ");
        Serial.write(LocalNetworkInterface::transmission_buffer, BIN);
        Serial.println("\n");
        LedStripCustom_loop();
    }

    void on_local_data_send()
    {
        Serial.println("Sent data: ");
        Serial.write(LocalNetworkInterface::transmission_buffer, BIN);
        Serial.println("\n");
    }

    void setup()
    {
        pinMode(0, INPUT_PULLUP);

        LocalNetworkInterface::initialize();
        LocalNetworkInterface::register_recv_callback(on_local_data_receive);
        LocalNetworkInterface::register_send_callback(on_local_data_send);

        Serial.println("Sending announcement package to AP");
        uint8_t test_data = 32;
        LocalNetworkInterface::send_binary_package(AP_MAC_ADDRESS, &test_data, 1);

        LedStripCustom_setup(); // FIXME: pin 2 kræsjer med radiokommunikasjon
    }

    void main()
    {
        if (!digitalRead(0))
        {
            uint8_t test_data = 137;
            LocalNetworkInterface::send_binary_package(AP_MAC_ADDRESS, &test_data, 1);
            delay(150); // neiognei
        }
    }
}