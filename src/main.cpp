#include "Arduino.h"

// Denne sketsjen "main.cpp" brukes til å programmere både
//   nodene (på instrumenter) og aksesspunktet (på basstromma).
//   For å programmere aksesspunktet: la IS_ACCESS_POINT være definert.
//   For å programmere node: kommenter linja ut.
//#define IS_ACCESS_POINT


// Importerer riktig headerfil avhengig av konfigurasjonen over
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
