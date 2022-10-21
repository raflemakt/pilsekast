#include <Arduino.h>
#include "LedStrip/LedStripShow.h"

void node_setup(){
    Serial.begin(512000);
    LedStrip_setup();
}

void node_main(){
    
    LedStrip_loop();


}


