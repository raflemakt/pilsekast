#include "network/PacketHandler.h"
#include "network/Registers.h"
#include "network/LocalNetworkInterface.h"
#include "node/HardwareInstance.h"

namespace AccessPointTriggers
{
void decide_action_on_pkg_receive() {
    Serial.println("Executing action from packet");
    ProtocolDescriptor header = (ProtocolDescriptor) LocalNetworkInterface::transmission_buffer[0];
    
    switch (header) {
        case OELKAST_LIGHT_SIMPLE: {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE packet");
            Serial.println("  no action taken");
        } break;

        case OELKAST_LIGHT_SIMPLE_HUE: {
            Serial.println("  detected OELKAST_LIGHT_SIMPLE_HUE packet");
            Serial.println("  no action taken");
        } break;

        case OELKAST_LIGHT_ENVELOPED: {
            Serial.println("  detected OELKAST_LIGHT_ENVELOPED packet");
            Serial.println("  no action taken");
        } break;

        case OELKAST_LIGHT_ANIMATION_MODE: {
            Serial.println("  detected OELKAST_LIGHT_ANIMATION_MODE packet");
            Serial.println("  no action taken");
        } break;

        case TELEPILS_ANNOUNCE: {
            Serial.println("  detected TELEPILS_ANNOUNCE packet");
            Serial.print("    node_name: ");
            Serial.println(telepils_announce.node_name);
            Serial.print("    instrument_type: ");
            Serial.println(telepils_announce.instrument_type);
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