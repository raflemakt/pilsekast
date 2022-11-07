#include <Arduino.h>
#include "LedStrip/LedStripShow.h"
#include "network/LocalNetworkInterface.h"
#include "LedStrip/LedStripCustom.h"
#include "UserInterface/Messages.h"
#include "node/NodeTriggers.h"
#include "node/HardwareInstance.h"
#include "network/Registers.h"
#include "sensors/icm_sensors/icm_sensors.h"
#include "LedStrip/lightController.h"

namespace Node
{
    void on_local_data_receive()
    {
        Serial.println("  on_local_data_receive called");
        NodeTriggers::decide_action_on_pkg_receive();
    }

    void on_local_data_send()
    {
        Serial.println("  on_local_data_send called");
    }

    void setup()
    {
        Messages::on_boot();
        LocalNetworkInterface::initialize();
        LocalNetworkInterface::register_recv_callback(on_local_data_receive);
        LocalNetworkInterface::register_send_callback(on_local_data_send);

        Serial.println("Sending announcement package to AP");
        PacketHandler::update_telepils_announce_packet();
        LocalNetworkInterface::send<TelepilsAnnounce>(&telepils_announce, BROADCAST);

        LedStripCustom_setup();

        // ICMsetup();  // Denne kræsjer med melding: [Wire.cpp:499] requestFrom(): i2cWriteReadNonStop returned Error -1
        //                 Initialization of the sensor returned: Data Underflow
    }

    void loop()
    {
        // LedStripCustomUpdate(0.25,90);
        // ICMloop();
        uint8_t noise = mySound.getNoise();
        if (noise > 100)
        {
            telepils_noise.noise_level = noise;
            LocalNetworkInterface::send<TelepilsNoise>(&telepils_noise, BROADCAST);
        }
    }
}