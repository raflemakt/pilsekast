#include <Arduino.h>
#include "configuration.h"

/* ======= Logging.h =======
*  Sentralisert logge-macro som gjør det enkelt
*  å utvide/endre/deaktivere logging. Per nå logger
*  vi kun over serieporten, men vi kan finne på å
*  logge til skjerm eller til fil eller endre LOG-nivåer
*  i framtida.
*/

#ifdef DEBUG_PRINT_TO_SERIAL

#define LOG(x)   Serial.print(x)
#define LOGLN(x) Serial.println(x)

#else

#define LOG(x)
#define LOGLN(x)

#endif