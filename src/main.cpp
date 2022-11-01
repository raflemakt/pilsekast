#include "Arduino.h"
#include "configuration.h"

#include "AP/access_point.h"
#include "node/node.h"



void setup(){
  if (DEBUG_PRINT_TO_SERIAL) Serial.begin(SERIAL_BAUD_RATE);

  #ifdef IS_ACCESS_POINT
  AccessPoint::setup();
  #else
  Node::setup();
  #endif
}

void loop(){
  #ifdef IS_ACCESS_POINT
  AccessPoint::loop();
  #else
  Node::loop();
  #endif
}
