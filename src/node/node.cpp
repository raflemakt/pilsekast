#include "node/node.h"

/* =========== NODE.CPP ===========
*  Prosedyre som kjøres på instrumentnoder.
*
*  Node::setup()                    kjøres én gang når noden starter
*  Node::loop()                     kjøres kontinuerlig så lenge noden er på
*  Node::on_local_data_receive()    kjøres automatisk av esp_now når vi får pakke
*  Node::on_local_data_send()       kjøres automatisk av esp_now når vi sender pakke
*/


uint32_t last_pkg_receive_timer;
uint32_t last_icm_pkg_sent_timer;

namespace Node
{
void on_local_data_receive() {
    Serial.println("  on_local_data_receive called");
    last_pkg_receive_timer = millis();
    NodeTriggers::decide_action_on_pkg_receive();

    #ifdef HAS_TTGO_SCREEN
    Screen::display_pkg_stat_component(0, Screen::SCR_HEIGTH-40, Screen::SCR_WIDTH, 40);
    #endif
}

void on_local_data_send() {
    Serial.println("  on_local_data_send called");

    #ifdef HAS_TTGO_SCREEN
    Screen::display_pkg_stat_component(0, Screen::SCR_HEIGTH-40, Screen::SCR_WIDTH, 40);
    #endif
}

void setup() {
    Messages::on_boot();
    LocalNetworkInterface::initialize();
    LocalNetworkInterface::register_recv_callback(on_local_data_receive);
    LocalNetworkInterface::register_send_callback(on_local_data_send);


    #ifdef HAS_TTGO_SCREEN
    Serial.println("## Initializing TTGO screen");
    Screen::init();
    Screen::display_info_screen();
    #endif


    #if LED_STRIP_LED_AMOUNT > 0
    Serial.println("## Initializing LED strip");
    LedStripCustom_setup();
    #endif


    #ifdef HAS_ICM_SENSOR
    Serial.println("## Initializing ICM sensor");
    ICMsetup();  // Denne kræsjer med melding: [Wire.cpp:499] requestFrom(): i2cWriteReadNonStop returned Error -1: Initialization of the sensor returned: Data Underflow
    #endif

    Serial.println("Sending announcement package to AP");
    PacketHandler::update_telepils_announce_packet();
    LocalNetworkInterface::send<TelepilsAnnounce>(&telepils_announce, AP_MAC_ADDRESS);
}


void loop() {
    /////////////////////////////////////////
    //        Hovedloop for noder          //
    /////////////////////////////////////////

    #ifdef HAS_TTGO_SCREEN
    // Info som vises på skjermen styres av NodeTriggers::decide_action_on_pkg_receive()
    // Gå tilbake til infoskjermen 5 sekunder etter ny pakke har kommet inn
    if (millis() - last_pkg_receive_timer > 5000) Screen::display_info_screen();
    Screen::update();
    #endif


    #if LED_STRIP_LED_AMOUNT > 0
    // Oppdater LED-stripe
    lightcontroll_write();
    #endif


    #ifdef HAS_ICM_SENSOR
    // Oppdater ICM-sensor
    float acc = ICMupdate();
    if ((acc > 0.5) && (millis() - last_icm_pkg_sent_timer > 200))
    {
        uint8_t accv=mapfloat(acc,0,3.5,0,255);
        telepils_acceleration.acceleration = accv;
        LocalNetworkInterface::send<TelePilsAcceleration>(&telepils_acceleration, AP_MAC_ADDRESS);
        last_icm_pkg_sent_timer = millis();
    }
    #endif


    #ifdef HAS_SOUND_SENSOR
    // Oppdater lydsensor
    uint8_t noise = mySound.getNoise();
    if (noise > mySound.noiseThreshold)
    {
        telepils_noise.noise_level = noise;
        telepils_noise.noise_record = mySound.noiseRecord;
        LocalNetworkInterface::send<TelepilsNoise>(&telepils_noise, AP_MAC_ADDRESS);
    }
    #endif
}
}
