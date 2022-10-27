#include "StringFormatters.h"
#include "Arduino.h"
#include "configuration.h"
#include "network/LocalNetworkInterface.h"

namespace Format
{
String byte_to_ascii_hex(uint8_t input_byte) {
    String temp_buffer;
    int msb = input_byte / 16;
    int lsb = input_byte - (msb*16);

    // Ascii [0-9]: [0-9],   [A-F]: [65-70]
    if (msb < 10) {temp_buffer += msb;} else temp_buffer += (char)(msb + 55);
    if (lsb < 10) {temp_buffer += lsb;} else temp_buffer += (char)(lsb + 55);
    return temp_buffer;
}

String mac_addr_from_array(const uint8_t *mac_addr) {
    if (mac_addr == 0) return "ff:ff:ff:ff:ff:ff (broadcast)";

    String temp_buffer;
    char separator = ':';

    for (auto i = 0; i<6; i++){
        temp_buffer += byte_to_ascii_hex(mac_addr[i]);
        if (i != 5) temp_buffer += separator;
    }
    return temp_buffer;
}

String array_as_hex(const uint8_t* data, const int size) {
    String temp_buffer;
    char separator = ' ';

    for (auto i=0; i<size; i++) {
        temp_buffer += byte_to_ascii_hex(
            data[i]
        );
        if (i < size-1) temp_buffer += separator;
    }
    return temp_buffer;
}

String array_as_decimal(const uint8_t* data, const int size) {
    String temp_buffer;
    char separator = ' ';

    for (auto i=0; i<size; i++) {
        temp_buffer += data[i];
        if (i < size-1) temp_buffer += separator;
    }
    return temp_buffer;
}
}