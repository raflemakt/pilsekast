#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"

namespace Triggers
{
void decide_action_on_pkg_receive() {
    Serial.println("Executing action from package");
    ProtocolDescriptor header = (ProtocolDescriptor) LocalNetworkInterface::transmission_buffer[0];
    
    switch (header) {
        case OELKAST_LIGHT_SIMPLE: {
            Serial.println("Executing action from package");
            oelkast_light_simple.intensity;
            oelkast_light_simple.duration;
            //TurnOnStrip(oelkast_light_simple.intensity, oelkast_light_simple.duration);
        } break;

        case OELKAST_LIGHT_ENVELOPED: {
        } break;

        case OELKAST_LIGHT_ANIMATION_MODE: {
        } break;

        case TELEPILS_ANNOUNCE: {
        } break;

        case TELEPILS_NODE_STATUS: {
        } break;

        case TELEPILS_TEMPERATURE: {
        } break;

        default: {
            Serial.println("  unknown pkg_type --> no action taken");
        }
    }
}
}