#include "OelkastPkg.h"
#include "TelepilsPkg.h"
#include "parsers.h"
#include "network/LocalNetworkInterface.h"

TelepilsAnnounce telepils_announce = {0};
TelepilsNodeStatus telepils_node_status = {0};
TelepilsTemperature telepils_temperature = {0};

OelkastLightSimple oelkast_light_simple = {0};
OelkastLightEnveloped oelkast_light_enveloped = {0};
OelkastLightAnimationMode oelkast_light_animation_mode = {0};


using namespace LocalNetworkInterface;
void Parse() {
    ProtocolDescriptor header = (ProtocolDescriptor) transmission_buffer[0];

    switch (header) {
        case OELKAST_LIGHT_SIMPLE: {
            OelkastLightSimple* destination = &oelkast_light_simple;
            memcpy(destination, transmission_buffer, transmission_size);
            break;
        }
        case OELKAST_LIGHT_ENVELOPED: {
            OelkastLightEnveloped* destination = &oelkast_light_enveloped;
            memcpy(destination, transmission_buffer, transmission_size);
            break;
        }
        case OELKAST_LIGHT_ANIMATION_MODE: {
            OelkastLightAnimationMode* destination = &oelkast_light_animation_mode;
            memcpy(destination, transmission_buffer, transmission_size);
            break;
        }
        case TELEPILS_ANNOUNCE: {
            TelepilsAnnounce* destination = &telepils_announce;
            memcpy(destination, transmission_buffer, transmission_size);
            break;
        }
        case TELEPILS_NODE_STATUS: {
            TelepilsNodeStatus* destination = &telepils_node_status;
            memcpy(destination, transmission_buffer, transmission_size);
            break;
        }
        case TELEPILS_TEMPERATURE: {
            TelepilsTemperature* destination = &telepils_temperature;
            memcpy(destination, transmission_buffer, transmission_size);
            break;
        }
    }
}
