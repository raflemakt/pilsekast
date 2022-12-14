#pragma once

#include "network/protocols/TelepilsPkg.h"
#include "network/protocols/OelkastPkg.h"

enum ProtocolDescriptor : uint8_t
{
    // ØLKAST-pakketyper: Kommandoer for lys, aktuatorer e.l.
    OELKAST_LIGHT_SIMPLE = 2,
    OELKAST_LIGHT_SIMPLE_HUE = 3,
    OELKAST_LIGHT_ENVELOPED = 4,
    OELKAST_LIGHT_ANIMATION_MODE = 5,

    // TELEPILS-pakketyper: Rapportering av telemetriske data
    TELEPILS_ANNOUNCE = 64,
    TELEPILS_NODE_STATUS = 65,
    TELEPILS_TEMPERATURE = 70,
    TELEPILS_ACCELERATION = 71,
    TELEPILS_NOISE = 80
};

namespace PacketHandler
{
    void move_data_to_buffer(const uint8_t data[], const uint8_t size);
    void move_data_to_register();
    void update_telepils_announce_packet();
    void update_protocol_descriptors_in_registers();
}

// TODO: Plan for parsers.h og parsers.cpp
// Vi må bestemme ølkast/telepils-header (packet_type)
//          ProtocolDescriptor find_protocol_of(binary_pkg)
// Vi må sjekke om binærdataen stemmer overens med størrelse definert i protokollen
//          sizeof(binary_pkg) == sizeof(implemented_protocols_array[packet_type]) <-???
// Vi må kunne pakke binærdataen over i en passende struct
//          TelepilsTemperature incoming_packet;     <--- Må nok bruke c++ templated class, lykke til
// Disse funksjonene må også kunne gjøres "baklengs"
//          pakke en struct over til telepils/ølkast-binærdata klar for transmisjon

// TODO: Optimaliseringer (ikke strengt nødvendig)
// - Pakke datatypene ned så små som mulig (f.eks. fra 8->4 bit). Vi kan nok spare
//   omtrent 40-50% overføring med dette  --> raskere overføring og mindre energibruk. Nais!
// - Gjøre integritetssjekk av pakkene. Egentlig helt unødvendig siden EspNow allerede
//   gjør dette, men det er veldig relevant for IELET2001 og sikkert gøy å implementere.
