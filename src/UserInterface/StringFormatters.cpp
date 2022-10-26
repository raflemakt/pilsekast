#include "StringFormatters.h"
#include "Arduino.h"
#include "configuration.h"
#include "network/LocalNetworkInterface.h"

namespace Format
{
String byte_to_ascii_hex(byte input_byte) {
    String temp_buffer;
    int msb = input_byte / 16;
    int lsb = input_byte - (msb*16);

    // Ascii [0-9]: [0-9],   [A-F]: [65-70]
    if (msb < 10) {temp_buffer += msb;} else temp_buffer += (char)(msb + 55);
    if (lsb < 10) {temp_buffer += lsb;} else temp_buffer += (char)(lsb + 55);
    return temp_buffer;
}

String mac_addr_from_array(const uint8_t *mac_addr) {
    String temp_buffer;
    char separator = ':';

    for (auto i = 0; i<6; i++){
        temp_buffer += byte_to_ascii_hex(mac_addr[i]);
        if (i != 5 || i%2==0) temp_buffer += separator;
    }
    return temp_buffer;
}

String transmission_buffer(int size) {
    String temp_buffer = String(size);
    char separator = ' ';

    for (auto i = 0; i<size; i++) {
        temp_buffer += byte_to_ascii_hex(
            LocalNetworkInterface::transmission_buffer[i]
        );
        if (i < size || i%2==0) temp_buffer += separator;
    }
    return temp_buffer;
}
}