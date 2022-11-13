#pragma once

#include <Arduino.h>
#include "network/LocalNetworkInterface.h"
#include "network/Registers.h"
#include "utils/Messages.h"
#include "node/NodeTriggers.h"
#include "node/HardwareInstance.h"

#ifdef HAS_ICM_SENSOR
#include "sensors/icm_sensors/icm_sensors.h"
#endif

#if LED_STRIP_LED_AMOUNT > 0
#include "LedStrip/lightController.h"
#include "LedStrip/LedStripShow.h"
#include "LedStrip/LedStripCustom.h"
#endif

#ifdef HAS_TTGO_SCREEN
#include "utils/TTGO_screen/TTGO_Screen.h"
#endif


namespace Node
{
void setup();
void loop();
}
