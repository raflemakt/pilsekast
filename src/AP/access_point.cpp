#include "AP/access_point.h"

/* =========== access_point.cpp ===========
*  Prosedyre som kjøres på aksesspunktet (AP) som er montert på basstromme.
*
*  AccessPoint::setup()                    kjøres én gang når AP starter
*  AccessPoint::loop()                     kjøres kontinuerlig så lenge AP er på
*  AccessPoint::on_local_data_receive()    kjøres automatisk av esp_now når vi får pakke
*  AccessPoint::on_local_data_send()       kjøres automatisk av esp_now når vi sender pakke
*/


// Menyvalg for demoprogram. Når prosjektarbeidet er over kan denne fjernes
enum AccessPointMenuState : uint8_t {
    RANDOM_COLOR_ON_HIT = 0,
    POTMETER_DECIDE_COLOR_ON_HIT = 1,
    ADSR_FROM_POTMETERS_ON_HIT = 2
} access_point_menu_state;



namespace AccessPoint
{
void on_local_data_receive() {
    Serial.println("  on_local_data_receive called");
    AccessPointTriggers::decide_action_on_pkg_receive();

    #ifdef HAS_TTGO_SCREEN
    Screen::display_pkg_stat_component(4, 12, 121, 120);
    #endif
}

void on_local_data_send() {
    Serial.println("  on_local_data_send called");
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
    Screen::update();
    #endif

    Serial.println("Init complete from access_point.cpp --> starting loop.\n");
}


void loop() {   
    ////////////////////////////////////////////////
    //  Hovedloop for aksesspunkt (basstromme)    //
    ////////////////////////////////////////////////
    button_bottom.loop();
    uint8_t drum_reading = getDrumSensor();
    

    #ifdef HAS_TTGO_SCREEN
    Screen::update();
    #endif


    // ========= Demoprosedyre for prosjektarbeidet =========
    // --> Brukeren kan trykke på knapp '0' for å bytte 'access_point_menu_state'
    // --> Trommesensoren er aktiv, og gjør forskjellige ting avhengig av hvilken meny vi er i
    switch (access_point_menu_state) {
        case AccessPointMenuState::RANDOM_COLOR_ON_HIT: {
            if (drum_reading > 0){
                Serial.println("\n## Drum was hit while in RANDOM_COLOR_ON_HIT");
                oelkast_light_simple_hue.intensity = drum_reading;
                oelkast_light_simple_hue.hue += 53;
                led_a.set_color(oelkast_light_simple_hue.hue);

                LocalNetworkInterface::send<OelkastLightSimpleHue>(&oelkast_light_simple_hue, BROADCAST);
            }
        } break;
        
        case AccessPointMenuState::POTMETER_DECIDE_COLOR_ON_HIT: {
            uint8_t potmeter_a_reading = potmeter_a.read();
            led_a.set_color(potmeter_a_reading);

            #ifdef HAS_TTGO_SCREEN
            Screen::display_window("Modus: POTMETER_DECIDE_COLOR_ON_HIT", 10, 90, 225, 35);
            Screen::buffer.drawString("hue:", 20, 105, 1);
            Screen::buffer.drawString(String(potmeter_a_reading), 50, 105, 1);
            #endif

            if (drum_reading > 0){
                Serial.println("\n## Drum was hit while in POTMETER_DECIDE_COLOR_ON_HIT");
                oelkast_light_simple_hue.intensity = drum_reading;
                oelkast_light_simple_hue.hue = potmeter_a_reading;

                LocalNetworkInterface::send<OelkastLightSimpleHue>(&oelkast_light_simple_hue, BROADCAST);
            }
        } break;

        case AccessPointMenuState::ADSR_FROM_POTMETERS_ON_HIT: {
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

            #ifdef HAS_TTGO_SCREEN
            Screen::display_adsr_screen();
            #endif

            if (drum_reading > 0){
                Serial.println("\n## Drum was hit while in ADSR_FROM_POTMETERS_ON_HIT");
                LocalNetworkInterface::send<OelkastLightEnveloped>(&oelkast_light_enveloped, BROADCAST);
            }
        } break;

        default:
            if (drum_reading > 0){
                Serial.print("\n## Drum was hit in unknown 'AccessPointMenuState': ");
                Serial.print(access_point_menu_state);
                Serial.println();
            }
    }  // end of switch (access_point_menu_state)


    // Bytter 'access_point_menu_state' når brukeren trykker på knapp '0'
    if (button_bottom.isPressed()) {

        #ifdef HAS_TTGO_SCREEN
        Screen::display_info_screen();
        #endif

        int("\n## Changing menu state: ");
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

} // end of void loop()
} // end of namespace AccessPoint
