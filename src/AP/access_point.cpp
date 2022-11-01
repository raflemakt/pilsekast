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


// FIXME: Finnes det en bedre plass/løsning for global state/objekter?
enum AccessPointMenuState : uint8_t {
    TEST_STATE = 0,
    OELKAST_LIGHT_SIMPLE_HUE_STATE = 1
} access_point_menu_state;


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
    Messages::on_boot();
    LocalNetworkInterface::initialize();
    LocalNetworkInterface::register_recv_callback(on_local_data_receive);
    LocalNetworkInterface::register_send_callback(on_local_data_send);
    Serial.println("Init complete from access_point.cpp --> starting loop.\n");
}

void loop()
{   
    button_top.loop();
    uint8_t drum_reading = getDrumSensor();
    uint8_t potmeter_reading = potmeter_a.read();
    led_a.set_color(potmeter_reading);


    // TODO: Flytt til egen fil? AccessPointTriggers.h og AccessPointMenu.h
    if (drum_reading > 0){
        switch (access_point_menu_state) {
            case AccessPointMenuState::TEST_STATE: {
                Serial.println("\n## Drum was hit while in TEST_STATE");
            } break;
            
            case AccessPointMenuState::OELKAST_LIGHT_SIMPLE_HUE_STATE: {
                Serial.println("\n## Drum was hit while in OELKAST_LIGHT_SIMPLE_HUE_STATE");
                oelkast_light_simple_hue.packet_type = ProtocolDescriptor::OELKAST_LIGHT_SIMPLE_HUE;
                oelkast_light_simple_hue.intensity = drum_reading;
                oelkast_light_simple_hue.hue = potmeter_reading;


                uint8_t temp_array[sizeof(oelkast_light_simple_hue)];     // HMMMMmmmm
                memcpy(temp_array, &oelkast_light_simple_hue, sizeof(oelkast_light_simple_hue));

                LocalNetworkInterface::send_binary_package(0, temp_array, sizeof(oelkast_light_simple_hue));
            } break;

            default:
                Serial.print("\n## Drum was hit in unknown 'AccessPointMenuState': ");
                Serial.print(access_point_menu_state);
                Serial.println();
        }
    }

    if (button_top.isPressed()) {
        Serial.print("\n## Changing menu state: ");
        if (access_point_menu_state == AccessPointMenuState::TEST_STATE) {
            access_point_menu_state = AccessPointMenuState::OELKAST_LIGHT_SIMPLE_HUE_STATE;
            Serial.println("OELKAST_LIGHT_SIMPLE_HUE_STATE");
        }
        else if (access_point_menu_state == AccessPointMenuState::OELKAST_LIGHT_SIMPLE_HUE_STATE) {
            access_point_menu_state = AccessPointMenuState::TEST_STATE;
            Serial.println("TEST_STATE");
        }
    }

}
}