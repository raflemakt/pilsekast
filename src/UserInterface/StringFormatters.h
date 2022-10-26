#pragma once

#include "Arduino.h"

namespace Format
{
String mac_addr_from_array(const uint8_t *mac_addr);
String transmission_buffer(int size);
}