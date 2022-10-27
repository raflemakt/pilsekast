#pragma once

#include "Arduino.h"

namespace Format
{
String byte_to_ascii_hex(uint8_t input_byte);
String mac_addr_from_array(const uint8_t *mac_addr);
String array_as_hex(const uint8_t* data, const int size);
String array_as_decimal(const uint8_t* data, const int size);
}