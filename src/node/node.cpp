#include <Arduino.h>

void node_setup(){
    Serial.begin(9600);
}

void node_main(){
    Serial.write("hello from node.cpp");
}
