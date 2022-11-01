#include <Arduino.h>
#include "configuration.h"

namespace Messages
{
void on_boot(){
    #ifdef IS_ACCESS_POINT
    Serial.println("\nPILSEKAST ACCESS POINT BOOT");
    #else
    Serial.println("\nPILSEKAST INSTRUMENT NODE BOOT");
    #endif
    Serial.print("  node name: ");
    Serial.println(NODE_NAME);
    Serial.print("  intrument type: ");
    Serial.println(INSTRUMENT_TYPE);
    Serial.println();
}
}