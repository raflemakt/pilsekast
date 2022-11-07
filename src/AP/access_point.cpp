#include <Arduino.h>
#include "AP/access_point.h"
#include "../configuration.h"
#include "network/LocalNetworkInterface.h"
#include "UserInterface/StringFormatters.h"
#include "UserInterface/Messages.h"
#include "sensors/snare_sensors.h"
#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "AP/HardwareInstance.h"
#include "AP/AccessPointTriggers.h"

#ifdef HAS_TTGO_SCREEN
#include "UserInterface/TTGO_screen/TTGO_Screen.h"
#endif


// FIXME: Finnes det en bedre plass/løsning for global state/objekter?
enum AccessPointMenuState : uint8_t {
    RANDOM_COLOR_ON_HIT = 0,
    POTMETER_DECIDE_COLOR_ON_HIT = 1,
    ADSR_FROM_POTMETERS_ON_HIT = 2
} access_point_menu_state;


namespace AccessPoint
{
void on_local_data_receive()
{
    Serial.println("  on_local_data_receive called");
    AccessPointTriggers::decide_action_on_pkg_receive();
}

void on_local_data_send()
{
    Serial.println("  on_local_data_send called");
}

void setup()
{
    #ifdef HAS_TTGO_SCREEN
    Screen::init();
    Screen::display_test_screen();
    #endif

    Messages::on_boot();
    LocalNetworkInterface::initialize();
    LocalNetworkInterface::register_recv_callback(on_local_data_receive);
    LocalNetworkInterface::register_send_callback(on_local_data_send);
    Serial.println("Init complete from access_point.cpp --> starting loop.\n");
}

void loop()
{   
    button_bottom.loop();
    uint8_t drum_reading = getDrumSensor();
    uint8_t potmeter_reading = potmeter_a.read();


    // TODO: Flytt til egen fil? AccessPointTriggers.h og AccessPointMenu.h
    
    switch (access_point_menu_state) {
        case AccessPointMenuState::RANDOM_COLOR_ON_HIT: {
            if (drum_reading > 0){
                Serial.println("\n## Drum was hit while in RANDOM_COLOR_ON_HIT");
                oelkast_light_simple_hue.packet_type = ProtocolDescriptor::OELKAST_LIGHT_SIMPLE_HUE;
                oelkast_light_simple_hue.intensity = drum_reading;
                oelkast_light_simple_hue.hue += 53;
                led_a.set_color(oelkast_light_simple_hue.hue);

                LocalNetworkInterface::send<OelkastLightSimpleHue>(&oelkast_light_simple_hue, BROADCAST);
            }
            

        } break;
        
        case AccessPointMenuState::POTMETER_DECIDE_COLOR_ON_HIT: {
            led_a.set_color(potmeter_reading);

            if (drum_reading > 0){
                Serial.println("\n## Drum was hit while in POTMETER_DECIDE_COLOR_ON_HIT");

                oelkast_light_simple_hue.packet_type = ProtocolDescriptor::OELKAST_LIGHT_SIMPLE_HUE;
                oelkast_light_simple_hue.intensity = drum_reading;
                oelkast_light_simple_hue.hue = potmeter_reading;

                LocalNetworkInterface::send<OelkastLightSimpleHue>(&oelkast_light_simple_hue, BROADCAST);
            }
        } break;

        case AccessPointMenuState::ADSR_FROM_POTMETERS_ON_HIT: {
            if (drum_reading > 0){
                Serial.println("\n## Drum was hit while in ADSR_FROM_POTMETERS_ON_HIT");
                oelkast_light_enveloped.packet_type = ProtocolDescriptor::OELKAST_LIGHT_ENVELOPED;
                oelkast_light_enveloped.intensity = drum_reading;
                oelkast_light_enveloped.color_red = 0;
                oelkast_light_enveloped.color_green = 255;
                oelkast_light_enveloped.color_blue = 0;
                oelkast_light_enveloped.duration = potmeter_e.read();
                oelkast_light_enveloped.env_attack_time = potmeter_a.read();
                oelkast_light_enveloped.env_decay_time = potmeter_b.read();
                oelkast_light_enveloped.env_sustain_level = potmeter_c.read();
                oelkast_light_enveloped.env_release_time = potmeter_d.read();

                LocalNetworkInterface::send<OelkastLightEnveloped>(&oelkast_light_enveloped, BROADCAST);
            }
        } break;

        default:
            if (drum_reading > 0){
                Serial.print("\n## Drum was hit in unknown 'AccessPointMenuState': ");
                Serial.print(access_point_menu_state);
                Serial.println();
            }
    }

    if (button_bottom.isPressed()) {
        Serial.print("\n## Changing menu state: ");
        if (access_point_menu_state == AccessPointMenuState::RANDOM_COLOR_ON_HIT) {
            access_point_menu_state = AccessPointMenuState::POTMETER_DECIDE_COLOR_ON_HIT;
            Serial.println("POTMETER_DECIDE_COLOR_ON_HIT");
        }
        else if (access_point_menu_state == AccessPointMenuState::POTMETER_DECIDE_COLOR_ON_HIT) {
            access_point_menu_state = AccessPointMenuState::ADSR_FROM_POTMETERS_ON_HIT;
            Serial.println("ADSR_FROM_POTMETERS_ON_HIT");
        }
        else if (access_point_menu_state == AccessPointMenuState::ADSR_FROM_POTMETERS_ON_HIT) {
            access_point_menu_state = AccessPointMenuState::RANDOM_COLOR_ON_HIT;
            Serial.println("RANDOM_COLOR_ON_HIT");
        }
    }

}
}
