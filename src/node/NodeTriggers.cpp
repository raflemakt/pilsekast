#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"
#include "node/HardwareInstance.h"
#include "LedStrip/LedStripAdsr.h"
#include "LedStrip/LedStripCustom.h"
#include "LedStrip/lightController.h"
#include "LedStrip/LedStripSimpleHue.h"
#include "utils/Logging.h"

#ifdef HAS_TTGO_SCREEN
#include "utils/TTGO_screen/TTGO_Screen.h"
#endif

namespace NodeTriggers
{
    void decide_action_on_pkg_receive()
    {
        LOGLN("Executing action from packet");
        ProtocolDescriptor header = (ProtocolDescriptor)LocalNetworkInterface::transmission_buffer[0];

        switch (header)
        {
        case OELKAST_LIGHT_SIMPLE:
        {
            LOGLN("  detected OELKAST_LIGHT_SIMPLE packet");
            oelkast_light_simple.intensity;
            oelkast_light_simple.duration;
            TurnOnStrip(mapfloat(oelkast_light_enveloped.intensity, 0, 255, 0, 0.5), (float)oelkast_light_simple.duration);
        }
        break;

        case OELKAST_LIGHT_SIMPLE_HUE:
        {
            LOGLN("  detected OELKAST_LIGHT_SIMPLE_HUE packet");
            LOG("    intensity: ");
            LOGLN(oelkast_light_simple_hue.intensity);
            LOG("    hue: ");
            LOGLN(oelkast_light_simple_hue.hue);
            TurnOnSimpleStrip(mapfloat(oelkast_light_simple_hue.intensity, 0, 255, 0, 0.5), mapfloat(oelkast_light_simple_hue.hue, 0, 255, 0, 360));
        }
        break;

        case OELKAST_LIGHT_ENVELOPED:
        {
            LOGLN("  detected OELKAST_LIGHT_ENVELOPED packet");
            LOG("    intensity: ");
            LOGLN(oelkast_light_enveloped.intensity);
            LOG("    color_red: ");
            LOGLN(oelkast_light_enveloped.color_red);
            LOG("    color_green: ");
            LOGLN(oelkast_light_enveloped.color_green);
            LOG("    color_blue: ");
            LOGLN(oelkast_light_enveloped.color_blue);
            LOG("    duration: ");
            LOGLN(oelkast_light_enveloped.duration);
            LOG("    env_attack_time: ");
            LOGLN(oelkast_light_enveloped.env_attack_time);
            LOG("    env_decay_time: ");
            LOGLN(oelkast_light_enveloped.env_decay_time);
            LOG("    env_sustain_level: ");
            LOGLN(oelkast_light_enveloped.env_sustain_level);
            LOG("    env_release_time: ");
            LOGLN(oelkast_light_enveloped.env_release_time);

            #ifdef HAS_TTGO_SCREEN
            // Lager et "vindu" midt på skjermen med ADSR-graf. Om vi vil gå videre med denne "vindu"-stilen burde vi
            // lage en skikkelig "window manager".
            Screen::display_adsrd_envelope_transient_component(0, 0, Screen::SCR_WIDTH, Screen::SCR_HEIGTH-40);
            Screen::buffer.drawString("OELKAST_LIGHT_ENVELOPED", 80, 2, 1);
            #endif
            
            TurnOnStripAttack(mapfloat(oelkast_light_enveloped.intensity, 0, 255, 0, 0.5), (float)oelkast_light_enveloped.env_attack_time * 2, oelkast_light_enveloped.color_red, oelkast_light_enveloped.color_green, oelkast_light_enveloped.color_blue);
        }
        break;

        case OELKAST_LIGHT_ANIMATION_MODE:
        {
            LOGLN("  detected OELKAST_LIGHT_ANIMATION_MODE packet");
        }
        break;

        case TELEPILS_ANNOUNCE:
        {
            LOGLN("  detected TELEPILS_ANNOUNCE packet");
        }
        break;

        case TELEPILS_NODE_STATUS:
        {
            LOGLN("  detected TELEPILS_NODE_STATUS packet");
        }
        break;

        case TELEPILS_TEMPERATURE:
        {
            LOGLN("  detected TELEPILS_TEMPERATURE packet");
        }
        break;

        default:
        {
            LOGLN("  unknown pkg_type --> no action taken");
        }
        }
    }
}