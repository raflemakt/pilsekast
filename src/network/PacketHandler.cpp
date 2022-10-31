#include "network/protocols/OelkastPkg.h"
#include "network/protocols/TelepilsPkg.h"
#include "network/PacketHandler.h"
#include "network/LocalNetworkInterface.h"
#include "network/Registers.h"
#include "UserInterface/StringFormatters.h"


namespace PacketHandler
{
void move_data_to_buffer(const uint8_t *data, const uint8_t size) {
    if (size > TRANSMISSION_BUFFER_SIZE){
        Serial.println("  discard data: too large for buffer");
        return;
    }

    memset(&LocalNetworkInterface::transmission_buffer, 0, TRANSMISSION_BUFFER_SIZE);  // <-- strengt tatt unødvendig
    memcpy(&LocalNetworkInterface::transmission_buffer, data, size);
    LocalNetworkInterface::transmission_size = size;

    Serial.println("  overwriting transmission buffer");
    Serial.print("    bytes: ");
    Serial.println(size);

    Serial.print("    data passed to function: ");
    Serial.println(Format::array_as_decimal(data, size));

    Serial.print("    transmission buffer (dec): ");
    Serial.println(Format::array_as_decimal(LocalNetworkInterface::transmission_buffer, size));

    Serial.print("    transmission buffer (hex): ");
    Serial.println(Format::array_as_hex(LocalNetworkInterface::transmission_buffer, size));
}

void move_data_to_register() {
    Serial.println("  deserializing package");
    ProtocolDescriptor header = (ProtocolDescriptor) LocalNetworkInterface::transmission_buffer[0];
    uint8_t size = LocalNetworkInterface::transmission_size;
    uint8_t* data = LocalNetworkInterface::transmission_buffer;

    switch (header) {
        // TODO:  Dette er spaghettiversjonen. Kanskje vi kan refaktorisere
        //        vha. cpp-templates?
        // FIXME: Vi gjør kun en memcpy og ikke ordentlig serialisering. Dette vil
        //        kun fungere om representasjon (endianness bl.a.) er likt på alle
        //        noder i nettverket.
        case OELKAST_LIGHT_SIMPLE: {
            Serial.println("    pkg_type: OELKAST_LIGHT_SIMPLE");
            memcpy(&oelkast_light_simple, data, size);
        } break;
        case OELKAST_LIGHT_ENVELOPED: {
            Serial.println("    pkg_type: OELKAST_LIGHT_ENVELOPED");
            memcpy(&oelkast_light_enveloped, data, size);
        } break;
        case OELKAST_LIGHT_ANIMATION_MODE: {
            Serial.println("    pkg_type: OELKAST_LIGHT_ANIMATION_MODE");
            memcpy(&oelkast_light_animation_mode, data, size);
        } break;
        case TELEPILS_ANNOUNCE: {
            Serial.println("    pkg_type: TELEPILS_ANNOUNCE");
            memcpy(&telepils_announce, data, size);
        } break;
        case TELEPILS_NODE_STATUS: {
            Serial.println("    pkg_type: TELEPILS_NODE_STATUS");
            memcpy(&telepils_node_status, data, size);
        } break;
        case TELEPILS_TEMPERATURE: {
            Serial.println("    pkg_type: TELEPILS_TEMPERATURE");
            memcpy(&telepils_temperature, data, size);
        } break;
        default: {
            Serial.println("    unknown pkg_type --> discarding data");
        }
    }
}
}