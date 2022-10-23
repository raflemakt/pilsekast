#include "Arduino.h"

// Importerer riktig headerfil avhengig av konfigurasjonen i "configuration.h"
#ifdef IS_ACCESS_POINT
#include "AP/access_point.h"
#else
#include "node/node.h"
#endif




void setup(){
  #ifdef IS_ACCESS_POINT
  access_point_setup();
  #else
  node_setup();
  #endif
}

void loop(){
  #ifdef IS_ACCESS_POINT
  access_point_main();
  #else
  node_main();
  #endif
}
