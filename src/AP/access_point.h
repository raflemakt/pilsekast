#pragma once

#include <Arduino.h>
#include "AP/HardwareInstance.h"
#include "AP/AccessPointTriggers.h"
#include "configuration.h"
#include "utils/StringFormatters.h"
#include "utils/Messages.h"
#include "sensors/snare_sensors.h"
#include "network/LocalNetworkInterface.h"
#include "network/PacketHandler.h"
#include "network/Registers.h"

#ifdef HAS_TTGO_SCREEN
#include "utils/TTGO_screen/TTGO_Screen.h"
#endif


namespace AccessPoint
{
void setup();
void loop();
}
