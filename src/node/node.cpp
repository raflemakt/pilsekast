#include "node/node.h"


uint32_t last_pkg_receive_timer;

namespace Node
{
void on_local_data_receive() {
    Serial.println("  on_local_data_receive called");
    last_pkg_receive_timer = millis();
    NodeTriggers::decide_action_on_pkg_receive();
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
    PacketHandler::update_telepils_announce_packet();
    LocalNetworkInterface::send<TelepilsAnnounce>(&telepils_announce, BROADCAST);

    #ifdef HAS_TTGO_SCREEN
    Screen::init();
    Screen::display_info_screen();
    #endif


    #if LED_STRIP_LED_AMOUNT > 0
    LedStripCustom_setup();  // Kræsjer når vi bruker TTGO-skjerm med melding:
    //                             [esp32-hal-gpio.c:102] __pinMode(): Invalid pin selected
    //                             E (246) g pio: gpio_set_level_(226): GPIO output gpio_num error
    //                          Brukes denne? Evt bytt til annen pin.
    #endif


    #ifdef HAS_ICM_SENSOR
    ICMsetup();  // Denne kræsjer med melding: [Wire.cpp:499] requestFrom(): i2cWriteReadNonStop returned Error -1: Initialization of the sensor returned: Data Underflow
    #endif
}


void loop() {
    /////////////////////////////////////////
    //        Hovedloop for noder          //
    /////////////////////////////////////////

    #ifdef HAS_TTGO_SCREEN
    // Gå tilbake til infoskjermen 5 sekunder etter ny pakke har kommet inn
    if (millis() - last_pkg_receive_timer > 5000) Screen::display_info_screen();
    #endif


    #if LED_STRIP_LED_AMOUNT > 0
    // Oppdater LED-stripe
    lightcontroll_write();
    #endif


    #ifdef HAS_ICM_SENSOR
    // Oppdater ICM-sensor
    float acc = ICMupdate();
    if (acc > 0.5)
    {
        uint8_t accv=mapfloat(acc,0,3.5,0,255);
        telepils_acceleration.acceleration = accv;
        LocalNetworkInterface::send<TelePilsAcceleration>(&telepils_acceleration, BROADCAST);
        delay(500);
    }
    #endif


    #ifdef HAS_SOUND_SENSOR
    // Oppdater lydsensor
    uint8_t noise = mySound.getNoise();
    if (noise > mySound.noiseThreshold)
    {
        telepils_noise.noise_level = noise;
        telepils_noise.noise_record = mySound.noiseRecord;
        LocalNetworkInterface::send<TelepilsNoise>(&telepils_noise, BROADCAST);
    }
    #endif
}
}