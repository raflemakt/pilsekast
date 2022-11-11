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

#ifdef HAS_TTGO_SCREEN
#include "UserInterface/TTGO_screen/TTGO_Screen.h"
#endif

uint32_t last_pkg_receive_timer;

namespace Node
{
    void on_local_data_receive()
    {
        Serial.println("  on_local_data_receive called");
        last_pkg_receive_timer = millis();
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

#ifdef HAS_TTGO_SCREEN
        Screen::init();
        Screen::display_info_screen();
#endif

        LedStripCustom_setup();  // Kræsjer når vi bruker TTGO-skjerm med melding:
        //                             [esp32-hal-gpio.c:102] __pinMode(): Invalid pin selected
        //                             E (246) g pio: gpio_set_level_(226): GPIO output gpio_num error
        //                          Brukes denne? Evt bytt til annen pin.

        ICMsetup();  // Denne kræsjer med melding: [Wire.cpp:499] requestFrom(): i2cWriteReadNonStop returned Error -1
        //                 Initialization of the sensor returned: Data Underflow
    }

    void loop()
    {
#ifdef HAS_TTGO_SCREEN
        // Gå tilbake til infoskjermen 5 sekunder etter ny pakke har kommet inn
        if (millis() - last_pkg_receive_timer > 5000)
            Screen::display_info_screen();
#endif

        lightcontroll_write();
        float acc = ICMupdate();
        if (acc > 0.5)
        {
            uint8_t accv=mapfloat(acc,0,3.5,0,255);
            telepils_acceleration.acceleration = accv;
            LocalNetworkInterface::send<TelePilsAcceleration>(&telepils_acceleration, BROADCAST);


        }
        uint8_t noise = mySound.getNoise();
        if (noise > mySound.noiseThreshold)
        {
            telepils_noise.noise_level = noise;
            telepils_noise.noise_record = mySound.noiseRecord;
            LocalNetworkInterface::send<TelepilsNoise>(&telepils_noise, BROADCAST);
        }
    }
}