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
    ProtocolDescriptor header = transmisson_buffer[0];
    uint8_t* destination;

    switch (header) {
        case OELKAST_LIGHT_SIMPLE:
            destination = &oelkast_light_simple;
            break;
        case OELKAST_LIGHT_ENVELOPED:
            destination = &oelkast_light_enveloped;
            break;
        case OELKAST_LIGHT_ANIMATION_MODE:
            destination = &oelkast_light_animation_mode;
            break;
        case TELEPILS_ANNOUNCE:
            destination = &telepils_announce;
            break;
        case TELEPILS_NODE_STATUS:
            destination = &telepils_node_status;
            break;
        case TELEPILS_TEMPERATURE:
            destination = &telepils_temperature;
            break;
    }

//    memcpy(&destination, transmission_buffer, transmission_size);
}
