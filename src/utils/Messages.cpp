#include <Arduino.h>
#include "configuration.h"
#include "utils/Logging.h"

namespace Messages
{
void on_boot(){
    #ifdef IS_ACCESS_POINT
    LOGLN("\nPILSEKAST ACCESS POINT BOOT");
    #else
    LOGLN("\nPILSEKAST INSTRUMENT NODE BOOT");
    #endif
    LOG("  node name: ");
    LOGLN(NODE_NAME);
    LOG("  intrument type: ");
    LOGLN(INSTRUMENT_TYPE);
    LOGLN();
}
}