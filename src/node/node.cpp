#include <Arduino.h>
#include "LedStrip/LedStripShow.h"
#include "network/LocalNetworkInterface.h"
#include "LedStrip/LedStripCustom.h"
#include "UserInterface/Messages.h"
#include "node/NodeTriggers.h"
#include "node/HardwareInstance.h"


namespace Node
{
void on_local_data_receive() {
    Serial.println("  on_local_data_receive called");
    NodeTriggers::decide_action_on_pkg_receive();
    //TurnOnStrip(0.25,5);
}

void on_local_data_send() {
    Serial.println("  on_local_data_send called");
}

void setup() {
    Messages::on_boot();
    LocalNetworkInterface::initialize();
    LocalNetworkInterface::register_recv_callback(on_local_data_receive);
    LocalNetworkInterface::register_send_callback(on_local_data_send);

    Serial.println("Sending announcement package to AP");
    uint8_t test_data = 32;
    LocalNetworkInterface::send_binary_package(AP_MAC_ADDRESS, &test_data, 1);

    LedStripCustom_setup();
}

void loop() {
    //LedStripCustomUpdate(0.25,90);
}
}