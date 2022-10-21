#include <Arduino.h>
#include "AP/access_point.h"
#include "../configuration.h"


void test_print(){
    Serial.println("hello from access_point.cpp");
    Serial.print(" node name: ");
    Serial.println(NODE_NAME);
    Serial.print(" intrument type: ");
    Serial.println(INSTRUMENT_TYPE);
    Serial.println();
    delay(1000);
}


void access_point_setup(){
    Serial.begin(9600);
}

void access_point_main(){
    test_print();
}
