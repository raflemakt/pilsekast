#include "Arduino.h"
#include "configuration.h"

#include "AP/access_point.h"
#include "node/node.h"



void setup(){
  if (DEBUG_PRINT_TO_SERIAL) Serial.begin(SERIAL_BAUD_RATE);

  if (IS_ACCESS_POINT) AccessPoint::setup();
  if (!IS_ACCESS_POINT) Node::setup();
}

void loop(){
  if (IS_ACCESS_POINT) AccessPoint::main();
  if (!IS_ACCESS_POINT) Node::main();
}
