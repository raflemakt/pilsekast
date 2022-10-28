#pragma once

#include "TelepilsPkg.h"
#include "OelkastPkg.h"

enum ProtocolDescriptor : uint8_t {
    // ØLKAST-pakketyper: Kommandoer for lys, aktuatorer e.l.
    OELKAST_LIGHT_SIMPLE = 2,
    OELKAST_LIGHT_ENVELOPED = 3,
    OELKAST_LIGHT_ANIMATION_MODE = 4,
    
    // TELEPILS-pakketyper: Rapportering av telemetriske data
    TELEPILS_ANNOUNCE = 64,
    TELEPILS_NODE_STATUS = 65,
    TELEPILS_TEMPERATURE = 70
};



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
