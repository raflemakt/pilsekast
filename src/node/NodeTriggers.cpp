#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"
#include "node/HardwareInstance.h"

namespace NodeTriggers
{
void decide_action_on_pkg_receive() {
    Serial.println("Executing action from packet");
    ProtocolDescriptor header = (ProtocolDescriptor) LocalNetworkInterface::transmission_buffer[0];
    
    switch (header) {
        case OELKAST_LIGHT_SIMPLE: {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE packet");
            oelkast_light_simple.intensity;
            oelkast_light_simple.duration;
            //TurnOnStrip(oelkast_light_simple.intensity, oelkast_light_simple.duration);
        } break;

        case OELKAST_LIGHT_SIMPLE_HUE: {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE_HUE packet");
            Serial.print("    intensity: ");
            Serial.println(oelkast_light_simple_hue.intensity);
            Serial.print("    hue: ");
            Serial.println(oelkast_light_simple_hue.hue);
            
            test_led.set_color(oelkast_light_simple_hue.hue);
        } break;

        case OELKAST_LIGHT_ENVELOPED: {
            Serial.println("  detected OELKAST_LIGHT_ENVELOPED packet");
        } break;

        case OELKAST_LIGHT_ANIMATION_MODE: {
            Serial.println("  detected OELKAST_LIGHT_ANIMATION_MODE packet");
        } break;

        case TELEPILS_ANNOUNCE: {
            Serial.println("  detected TELEPILS_ANNOUNCE packet");
        } break;

        case TELEPILS_NODE_STATUS: {
            Serial.println("  detected TELEPILS_NODE_STATUS packet");
        } break;

        case TELEPILS_TEMPERATURE: {
            Serial.println("  detected TELEPILS_TEMPERATURE packet");
        } break;

        default: {
            Serial.println("  unknown pkg_type --> no action taken");
        }
    }
}
}