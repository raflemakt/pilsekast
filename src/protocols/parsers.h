
enum ProtocolDescriptor : uint8_t{
    // ØLKAST-pakketyper: Kommandoer for lys, aktuatorer e.l.
    OELKAST_LIGHT_SIMPLE = 2,
    OELKAST_LIGHT_ENVELOPED = 3,
    OELKAST_LIGHT_ANIMATION_MODE = 4,
    
    // TELEPILS-pakketyper: Rapportering av telemetriske data
    TELEPILS_TEMPERATURE = 65
};


// PLAN
// Vi må bestemme ølkast/telepils-header (packet_type)
//          ProtocolDescriptor find_protocol_of(binary_pkg)
// Vi må sjekke om binærdataen stemmer overens med størrelse definert i protokollen
//          sizeof(binary_pkg) == sizeof(implemented_protocols_array[packet_type]) <-???
// Vi må kunne pakke binærdataen over i en passende struct
//          TelepilsTemperature incoming_packet;     <--- Må nok bruke c++ templated class, lykke til
