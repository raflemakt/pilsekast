#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"
#include "node/HardwareInstance.h"
#include "LedStrip/LedStripAdsr.h"
#include "LedStrip/LedStripCustom.h"
#include "LedStrip/lightController.h"
#include "LedStrip/LedStripSimpleHue.h"

#ifdef HAS_TTGO_SCREEN
#include "utils/TTGO_screen/TTGO_Screen.h"
#endif

namespace NodeTriggers
{
    void decide_action_on_pkg_receive()
    {
        Serial.println("Executing action from packet");
        ProtocolDescriptor header = (ProtocolDescriptor)LocalNetworkInterface::transmission_buffer[0];

        switch (header)
        {
        case OELKAST_LIGHT_SIMPLE:
        {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE packet");
            oelkast_light_simple.intensity;
            oelkast_light_simple.duration;
            TurnOnStrip(mapfloat(oelkast_light_enveloped.intensity, 0, 255, 0, 0.5), (float)oelkast_light_simple.duration);
        }
        break;

        case OELKAST_LIGHT_SIMPLE_HUE:
        {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE_HUE packet");
            Serial.print("    intensity: ");
            Serial.println(oelkast_light_simple_hue.intensity);
            Serial.print("    hue: ");
            Serial.println(oelkast_light_simple_hue.hue);
            TurnOnSimpleStrip(mapfloat(oelkast_light_simple_hue.intensity, 0, 255, 0, 0.5), mapfloat(oelkast_light_simple_hue.hue, 0, 255, 0, 360));
        }
        break;

        case OELKAST_LIGHT_ENVELOPED:
        {
            Serial.println("  detected OELKAST_LIGHT_ENVELOPED packet");
            Serial.print("    intensity: ");
            Serial.println(oelkast_light_enveloped.intensity);
            Serial.print("    color_red: ");
            Serial.println(oelkast_light_enveloped.color_red);
            Serial.print("    color_green: ");
            Serial.println(oelkast_light_enveloped.color_green);
            Serial.print("    color_blue: ");
            Serial.println(oelkast_light_enveloped.color_blue);
            Serial.print("    duration: ");
            Serial.println(oelkast_light_enveloped.duration);
            Serial.print("    env_attack_time: ");
            Serial.println(oelkast_light_enveloped.env_attack_time);
            Serial.print("    env_decay_time: ");
            Serial.println(oelkast_light_enveloped.env_decay_time);
            Serial.print("    env_sustain_level: ");
            Serial.println(oelkast_light_enveloped.env_sustain_level);
            Serial.print("    env_release_time: ");
            Serial.println(oelkast_light_enveloped.env_release_time);

            #ifdef HAS_TTGO_SCREEN
            // Lager et "vindu" midt på skjermen med ADSR-graf. Om vi vil gå videre med denne "vindu"-stilen burde vi
            // lage en skikkelig "window manager".
            auto y_pos = 40;
            Screen::display_adsrd_envelope_transient_component(5, y_pos, Screen::SCR_WIDTH-10, Screen::SCR_HEIGTH-y_pos-5);
            Screen::tft.drawString("OELKAST_LIGHT_ENVELOPED", 80, y_pos + 2, 1);
            #endif
            
            TurnOnStripAttack(mapfloat(oelkast_light_enveloped.intensity, 0, 255, 0, 0.5), (float)oelkast_light_enveloped.env_attack_time * 2, oelkast_light_enveloped.color_red, oelkast_light_enveloped.color_green, oelkast_light_enveloped.color_blue);
        }
        break;

        case OELKAST_LIGHT_ANIMATION_MODE:
        {
            Serial.println("  detected OELKAST_LIGHT_ANIMATION_MODE packet");
        }
        break;

        case TELEPILS_ANNOUNCE:
        {
            Serial.println("  detected TELEPILS_ANNOUNCE packet");
        }
        break;

        case TELEPILS_NODE_STATUS:
        {
            Serial.println("  detected TELEPILS_NODE_STATUS packet");
        }
        break;

        case TELEPILS_TEMPERATURE:
        {
            Serial.println("  detected TELEPILS_TEMPERATURE packet");
        }
        break;

        default:
        {
            Serial.println("  unknown pkg_type --> no action taken");
        }
        }
    }
}